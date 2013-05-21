////////////////////////////////////////////////////////////////////////////////
//! \file memory_engine.c
//! \brief shared memory managerment engine
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!     V 1.00,          ,  Fang Bao
//!     V 1.02, 6/10/2010,  Fang Bao,   replace all type (uint, unsigned int) with size_t
//!     v 1.03, 10/18/2011, Qiang Zhang,kernel dependent on this file, move from GS to kernel tree
//!     v 1.04, 10/23/2011, Qiang Zhang,add mechanism to track shm usage info
//! Note:
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
    System head files
*/
#include <linux/kernel.h>   /* printk() */
#include <linux/sched.h>
#include <linux/slab.h>     /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/seq_file.h> /*seq_file api*/

/*******************************************************************************
    Local head files
*/

#include <mach/galois_generic.h>
#include "sys_memmap.h"
#include "shm_type.h"
#include "memory_engine.h"


/*******************************************************************************
    Local variable
*/


/*******************************************************************************
    Private API
*/

static memory_node_t * _FindNode_size(memory_engine_t *engine, size_t size,
		size_t alignment)
{
	memory_node_t *node;
	size_t offset;

	/* Walk all free nodes until we have one that is big enough or we have
	   reached the root. (root.m_size == 0) */
	for (node = engine->m_root.m_next_free; node->m_size != 0; node = node->m_next_free) {
		/* Compute number of bytes to skip for alignment. */
		offset = (alignment == 0)
					? 0
					: (alignment - (node->m_addr % alignment));

		/* Node is already aligned. */
		if (offset == alignment)
			offset = 0;

		if (node->m_size >= size + offset) {
			/* This node is big enough. */
			node->m_offset = offset;
			return node;
		}
	}

	/* Not enough memory. */
	return NULL;
}

static int _Split(memory_engine_t *engine, memory_node_t *node, size_t size)
{
	memory_node_t *new_node;

	/* Make sure the byte boundary makes sense. */
	if ((size == 0) || (size >= node->m_size))
		return EINVAL;

	/* Allocate a new node object */
	new_node = kmalloc(sizeof(memory_node_t), GFP_KERNEL);
	if (new_node == NULL)
		return -ENOMEM;
	memset(new_node, 0, sizeof(memory_node_t));

	/* Initialize node structure. */
	new_node->m_addr        = node->m_addr + size;
	new_node->m_size        = node->m_size - size;
	new_node->m_alignment   = 0;
	new_node->m_offset      = 0;

	/* Insert node behind specified node. */
	new_node->m_next    = node->m_next;
	new_node->m_prev    = node;
	node->m_next        = new_node->m_next->m_prev = new_node;

	/* Insert free node behind specified node. */
	new_node->m_next_free   = node->m_next_free;
	new_node->m_prev_free   = node;
	node->m_next_free       = new_node->m_next_free->m_prev_free = new_node;

	/* Adjust size of specified node. */
	node->m_size = size;

	/* Success. */
	return 0;
}

static int _Merge(memory_engine_t *engine, memory_node_t *node)
{
	memory_node_t *new_node;

	/* Save pointer to next node. */
	new_node = node->m_next;

	/* This is a good time to make sure the heap is not corrupted. */
	if (node->m_addr + node->m_size != new_node->m_addr) {
		/* Corrupted heap. */
        /* Error. */
        shm_error("Found memory corrupted !!! (0x%08X + 0x%08X != 0x%08X)\n",
					node->m_addr, node->m_size, new_node->m_addr);
		return -EFAULT;
	}

	/* Adjust byte count. */
	node->m_size += new_node->m_size;

	/* Unlink next node from linked list. */
	node->m_next        = new_node->m_next;
	node->m_next_free   = new_node->m_next_free;

	node->m_next->m_prev            =
	node->m_next_free->m_prev_free  = node;

	/* Free next node. */
	kfree(new_node);

	return 0;
}

int _FindNode_alignaddr(memory_engine_t *engine, size_t alignaddr, memory_node_t **node)
{
	memory_node_t *new_node;
	shm_debug("memory_engine_find_alignaddr start. (0x%08X)\n", alignaddr);

	/* Walk all nodes until we have one which alignaddr is equal to or we have
		reached the root. (root.m_size == 0) */
	for (new_node = engine->m_root.m_next; new_node->m_size != 0; new_node = new_node->m_next) {
        if (MEMNODE_ALIGN_ADDR(new_node) == alignaddr) {
			*node = new_node;
			shm_debug("memory_engine_find_alignaddr OK.\n");
			return 0;
		}
	}

	*node = NULL;

	return -EFAULT;
}

/*******************************************************************************
    Module API
*/

int memory_engine_create(memory_engine_t **engine, size_t base, size_t size, size_t threshold)
{
	int res;
	memory_node_t * new_node;
	memory_engine_t *new_engine;

	shm_debug("memory_engine_create start. (0x%08X, %u, %u) \n", base, size, threshold);

	if (engine == NULL) {
		res = -EINVAL;
		goto err_add_engine;
	}

	new_engine = kmalloc(sizeof(memory_engine_t), GFP_KERNEL);
	if (new_engine == NULL) {
		res = -ENOMEM;
		goto err_add_engine;
	}
	memset(new_engine, 0 , sizeof(memory_engine_t));

	new_engine->m_base = base;
	new_engine->m_size = new_engine->m_size_free = size;
	new_engine->m_size_used = 0;
	new_engine->m_threshold = threshold;

	new_engine->m_peak_usedmem =
	new_engine->m_max_usedblock =
	new_engine->m_min_usedblock = new_engine->m_size_used;

	new_engine->m_max_freeblock =
	new_engine->m_min_freeblock = new_engine->m_size_free;

	new_engine->m_num_freeblock = 1;
	new_engine->m_num_usedblock = 0;

	/* Allocate a new node object */
	new_node = kmalloc(sizeof(memory_node_t), GFP_KERNEL);
	if (new_node == NULL) {
		res = -ENOMEM;
		goto err_add_node;
	}
	memset(new_node, 0, sizeof(memory_node_t));

	new_node->m_next =
	new_node->m_prev =
	new_node->m_next_free =
	new_node->m_prev_free = &(new_engine->m_root);

	new_node->m_addr = new_engine->m_base;
	new_node->m_size = new_engine->m_size_free;
	new_node->m_alignment = 0;
	new_node->m_offset = 0;

	/* Initialize the linked list of nodes. */
	new_engine->m_root.m_next =
	new_engine->m_root.m_prev =
	new_engine->m_root.m_next_free =
	new_engine->m_root.m_prev_free = new_node;

	new_engine->m_root.m_size = 0;

	/* Initialize the semaphore, come up in unlocked state. */
	sema_init(&(new_engine->m_mutex),1);

	*engine = new_engine;

	shm_debug("memory_engine_create OK.\n");

	return 0;

err_add_node:

	kfree(new_engine);

err_add_engine:

	shm_error("memory_engine_create failed !!! (0x%08X, %u, %u) \n", base, size, threshold);

	return res;
}


int memory_engine_destroy(memory_engine_t **engine)
{
	int res;
	memory_node_t   *node, *next;

	shm_debug("memory_engine_destroy start.\n");

	if (engine == NULL) {
		res = -EINVAL;
		goto err_del_engine;
	}

	/* Walk all the nodes until we reached the root. (root.m_size == 0) */
	for (node = (*engine)->m_root.m_next; node->m_size != 0; node = next) {
		/* Save pointer to the next node. */
		next = node->m_next;

		/* Free the node. */
		kfree(node);
	}

	/* Free the mutex. */

	/* Free the engine object. */
	kfree(*engine);

	*engine = NULL;

	shm_debug("memory_engine_destroy OK.\n");

	return 0;

err_del_engine:

	shm_error("memory_engine_destroy failed !!!\n");

	return res;
}

int memory_engine_allocate(memory_engine_t *engine, size_t size,
								size_t alignment, memory_node_t **node)
{
	int res = 0;
	memory_node_t *new_node = NULL;
	struct task_struct *grptask = NULL;

	shm_debug("memory_engine_allocate start. (%d, %d)\n", size, alignment);

	if ((engine == NULL) || (node == NULL))
		return -EINVAL;

	down(&(engine->m_mutex));

	if (size > engine->m_size_free) {
		shm_error("heap has not enough (%u) bytes for (%u) bytes\n", engine->m_size_free, size);
		res = -ENOMEM;
		goto err_exit;
	}

	/* Find a free node in heap */
	new_node = _FindNode_size(engine, size, alignment);
	if (new_node == NULL) {
		memory_node_t *pLastNode = NULL;
		pLastNode = engine->m_root.m_prev_free;
		if (pLastNode)
			shm_error("heap has not enough liner memory for (%u) bytes, free blocks:%u(max free block:%u)\n",
					size, engine->m_num_freeblock, pLastNode->m_size);
		else
			shm_error("heap has not enough liner memory, no free blocks!!!\n");
		res = -ENOMEM;
		goto err_exit;
	}

	/* Do we have enough memory after the allocation to split it? */
	if (MEMNODE_ALIGN_SIZE(new_node) - size > engine->m_threshold)
		_Split(engine, new_node, size + new_node->m_offset);/* Adjust the node size. */
	else
		engine->m_num_freeblock--;

	engine->m_num_usedblock++;

	/* Remove the node from the free list. */
	new_node->m_prev_free->m_next_free = new_node->m_next_free;
	new_node->m_next_free->m_prev_free = new_node->m_prev_free;
	new_node->m_next_free =
	new_node->m_prev_free = NULL;

	/* Fill in the information. */
	new_node->m_alignment = alignment;
	/*record pid/thread name in node info, for debug usage*/
	new_node->m_threadid = task_pid_vnr(current);/*(current)->pid;*/

	/* qzhang@marvell
	 * record creating task id,user task id
	 * by default user task id is creating task id
	 * until memory_engine_lock invoked
	 */
	new_node->m_taskid   =
	new_node->m_usrtaskid= task_tgid_vnr(current);
	strncpy(new_node->m_threadname, current->comm, 16);
	grptask = pid_task(task_tgid(current),PIDTYPE_PID);
	if (NULL != grptask) {
		strncpy(new_node->m_taskname,grptask->comm,16);
		strncpy(new_node->m_usrtaskname,grptask->comm,16);
	} else {
		memset(new_node->m_taskname,0,16);
		memset(new_node->m_usrtaskname,0,16);
	}

	/* Adjust the number of free bytes. */
	engine->m_size_free -= new_node->m_size;
	engine->m_size_used += new_node->m_size;

	engine->m_peak_usedmem = max(engine->m_peak_usedmem, engine->m_size_used);

	/* Return the pointer to the node. */
	*node = new_node;

	up(&(engine->m_mutex));

	shm_debug("Allocated %u (%u) bytes @ 0x%08X (0x%08X) for align (%u)\n",
			MEMNODE_ALIGN_SIZE(new_node), new_node->m_size,
			MEMNODE_ALIGN_ADDR(new_node), new_node->m_addr,
			new_node->m_alignment);

	shm_debug("memory_engine_allocate OK.\n");

	return 0;

err_exit:

	up(&(engine->m_mutex));
	*node = NULL;

	shm_error("memory_engine_allocate failed !!! (%d, %d) (%d %s)\n",
							size, alignment, current->pid, current->comm);
	return res;
}

int memory_engine_free(memory_engine_t *engine, int alignaddr)
{
	int res;
	memory_node_t   *new_node, *node;

	shm_debug("memory_engine_free start. (0x%08X)\n", alignaddr);

	if ((engine == NULL) || (alignaddr == 0))
		return -EINVAL;

	down(&(engine->m_mutex));

	/* find alignaddr */
	res = _FindNode_alignaddr(engine, alignaddr, &node);
	if (res != 0)
		goto err_exit;

	/* if the node to be freed is a free one, there could be invalid operations*/
	if (node->m_next_free != NULL) {
		res = -EFAULT;
		goto err_exit;
	}

	/* clean node */
	node->m_offset = 0;
	node->m_alignment = 0;

	/* Update the number of free bytes. */
	engine->m_size_free += node->m_size;
	engine->m_size_used -= node->m_size;

	/* Find the next free node(go through node list, find the first node which is free). */
	for (new_node = node->m_next; new_node->m_next_free == NULL; new_node = new_node->m_next) ;

	/* Insert this node in the free list. */
	node->m_next_free = new_node;
	node->m_prev_free = new_node->m_prev_free;

	node->m_prev_free->m_next_free =
	new_node->m_prev_free          = node;

	engine->m_num_usedblock--;
	engine->m_num_freeblock++;

	/* Is the next node a free node and not the root? */
	if ((node->m_next == node->m_next_free) &&
		(node->m_next->m_size != 0)) {
		/* Merge this node with the next node. */
		new_node = node;
		res = _Merge(engine, new_node);

		if((new_node->m_next_free == new_node) ||
			(new_node->m_prev_free == new_node) || (res != 0)) {
			/* Error. */
			shm_error("_Merge next node failed.\n");
			goto err_exit;
		}

		engine->m_num_freeblock--;

		shm_debug("_Merge next node OK.\n");
	}

	/* Is the previous node a free node and not the root? */
	if ((node->m_prev == node->m_prev_free) && (node->m_prev->m_size != 0)) {
		/* Merge this node with the previous node. */
		new_node = node->m_prev;
		res = _Merge(engine, new_node);

		if((new_node->m_next_free == new_node) ||
			(new_node->m_prev_free == new_node) || (res != 0)) {
			/* Error. */
			shm_error("_Merge previous node failed.\n");
			goto err_exit;
		}

		engine->m_num_freeblock--;

		shm_debug("_Merge previous node OK.\n");
	}

	up(&(engine->m_mutex));

	shm_debug("memory_engine_free OK.\n");

	return 0;

err_exit:

	up(&(engine->m_mutex));

	if (shm_lowmem_debug_level > 2) {
		shm_error("memory_engine_free failed !!! (0x%08X)\n", alignaddr);
		dump_stack();
	}

	return res;
}

int memory_engine_takeover(memory_engine_t *engine,int alignaddr)
{
	int res = 0;
	memory_node_t *node = NULL;
	struct task_struct *grptask = NULL;

	shm_debug("mem_engine_takeover start, (0x%08X)\n",alignaddr);

	if ((NULL == engine) || (0 == alignaddr))
		return -EINVAL;

	down(&(engine->m_mutex));

	/* find alignaddr */
	res = _FindNode_alignaddr(engine, alignaddr, &node);
	if (0 != res)
		goto err_exit;

	/* if the node found is a free one, there could be invalid operations */
	if (NULL != node->m_next_free) {
		shm_error("node(%#.8x) already freed\n",alignaddr);
		res = -EFAULT;
		goto err_exit;
	}

	/* change usrtaskid to current */
	node->m_usrtaskid = task_tgid_vnr(current);
	grptask = pid_task(task_tgid(current),PIDTYPE_PID);
	if (NULL == grptask)
		memset(node->m_usrtaskname,0,16);
	else
		strncpy(node->m_usrtaskname,grptask->comm,16);

	up(&(engine->m_mutex));

	shm_debug("memory_engine_takeover OK.\n");
	return 0;

err_exit:

	up(&(engine->m_mutex));
	shm_error("memory_engine_takeover failed!!! (0x%08X)\n", alignaddr);
	return res;
}

int memory_engine_giveup(memory_engine_t *engine, int alignaddr)
{
	int res = 0;
	memory_node_t *node = NULL;

	shm_debug("memory_engine_giveup start, (0x%08X)\n",alignaddr);

	if ((NULL == engine) || (0 == alignaddr))
		return -EINVAL;

	down(&(engine->m_mutex));

	/* find note from alignaddr */
	res = _FindNode_alignaddr(engine, alignaddr, &node);
	if (0 != res)
		goto err_exit;

	/* if the node found is a free one, there could be invalid operations */
	if ((NULL == node) || (NULL != node->m_next_free)) {
		shm_error("node(%#.8x) not exists\n",alignaddr);
		res = -EFAULT;
		goto err_exit;
	}

	/* change usrtaskid to taskid */
	node->m_usrtaskid = node->m_taskid;
	strncpy(node->m_usrtaskname,node->m_taskname,16);

	up(&(engine->m_mutex));

	shm_debug("memory_engine_giveup OK.\n");
	return 0;

err_exit:

	up(&(engine->m_mutex));
	shm_error("memory_engine_giveup fail!!! (0x%08X)\n", alignaddr);
	return res;
}

int memory_engine_gothrough(memory_engine_t *engine, char *buffer, int count)
{
	int len = 0, i = 0;
	memory_node_t   *new_node;
	char tag_used[] = " Yes ";
	char tag_free[] = "     ";
	char *ptr_tag;

	shm_debug("memory_engine_gothrough start. ( 0x%08X, %d)\n", (size_t)buffer, count);

	if ((engine == NULL) || (buffer == NULL) || (count <= 0))
		return -EINVAL;

	len += sprintf(buffer + len, "  No | Alloc |    Node    |    Addr     (Aligned)   |  Offset  |    Size    (Aligned)  |   Align  |\n");
	len += sprintf(buffer + len, "---------------------------------------------------------------------------------------------------\n");

	down(&(engine->m_mutex));

	/* Walk all nodes until we have
		reached the root. (root.m_size == 0) */
	for (new_node = engine->m_root.m_next; new_node->m_size != 0;
			new_node = new_node->m_next) {
		/* check buffer length to avoid buffer overflow */
		if (len > count - 102) {
			shm_debug("memory_engine_gothrough buffer is full !!!\n");
			goto done;
		}

		if (new_node->m_next_free != NULL)
			ptr_tag = tag_free;
		else
			ptr_tag = tag_used;

		len += sprintf(buffer + len, " %3d | %5s | 0x%08X | 0x%08X (0x%08X) | %8d | %9d (%9d) | %8d |\n",
			++i, ptr_tag, (size_t)new_node,
			new_node->m_addr, MEMNODE_ALIGN_ADDR(new_node), (MEMNODE_ALIGN_ADDR(new_node) - engine->m_base),
			new_node->m_size, MEMNODE_ALIGN_SIZE(new_node),
			new_node->m_alignment);
	}

done:

	up(&(engine->m_mutex));

	shm_debug("memory_engine_gothrough OK. (node = %d, len = %d)\n", i, len);

	return len;
}

int memory_engine_show(memory_engine_t *engine, struct seq_file *file)
{
	int len = 0, i = 0;
	memory_node_t   *new_node;
	char tag_used[] = " Yes ";
	char tag_free[] = "     ";
	char *ptr_tag;


	if ((engine == NULL) || (file == NULL))
		return -EINVAL;

	len += seq_printf(file, "  No | Alloc |    Node    |    Addr     (Aligned)   |  Offset  |    Size    (Aligned)  |   Align  | thread id(name) \n");
	len += seq_printf(file, "---------------------------------------------------------------------------------------------------\n");

	down(&(engine->m_mutex));

	/* Walk all nodes until we have
		reached the root. (root.m_size == 0) */
	for (new_node = engine->m_root.m_next; new_node->m_size != 0;
			new_node = new_node->m_next) {
		if (new_node->m_next_free != NULL)
			ptr_tag = tag_free;
		else
			ptr_tag = tag_used;

		len += seq_printf(file, " %3d | %5s | 0x%08X | 0x%08X (0x%08X) | %8d | %9d (%9d) | %8d | 0x%08X(%s) \n",
			++i, ptr_tag, (size_t)new_node,
			new_node->m_addr, MEMNODE_ALIGN_ADDR(new_node), (MEMNODE_ALIGN_ADDR(new_node) - engine->m_base),
			new_node->m_size, MEMNODE_ALIGN_SIZE(new_node),
			new_node->m_alignment, new_node->m_threadid, new_node->m_threadname);
	}


	up(&(engine->m_mutex));

	shm_debug("memory_engine_gothrough OK. (node = %d, len = %d)\n", i, len);

	return len;
}

/*
 * release memory allocated by process pid
 */
int memory_engine_release_by_taskid(memory_engine_t *engine, pid_t taskid)
{
	size_t alignaddr = 0 ;
	memory_node_t *new_node = NULL;
	int rlsCnt = 0 ;
	int idx = 0;
	int totalSize = 0 ;
	int *release_array = NULL;
	int static_release_array[64] = {0};
	int *dynamic_release_array = NULL;

	if (NULL == engine)
		return -EINVAL;

	release_array = static_release_array;
    /* Walk all nodes until we have reached the root. (root.m_size == 0) */
	down(&(engine->m_mutex));
	for (new_node = engine->m_root.m_next ; new_node->m_size != 0 ; new_node = new_node->m_next) {
		if (NULL != new_node->m_next_free)
			continue;/* free node */

		if (taskid == new_node->m_taskid) {
			release_array[rlsCnt++] = MEMNODE_ALIGN_ADDR(new_node);
			totalSize += new_node->m_size;
		} else
			continue;

		/* For berlin_avservice, the shm node number is larger than 64,
		* than swift to dynamic allocate array */
		if (64 == rlsCnt) {
			printk("%s,%d memory note for pid(%d) exceed 64\n", __FUNCTION__,__LINE__,taskid);
			dynamic_release_array = kzalloc(1024 * sizeof(int), GFP_KERNEL);
			if (NULL == dynamic_release_array) {
				shm_error("%s,%d fail to allocate memory for dynamic_release_array\n",
							__FUNCTION__,__LINE__);
				break;
			} else {
				memcpy(dynamic_release_array, release_array, rlsCnt * sizeof(int));
				release_array = dynamic_release_array;
			}
		}
		/* basically, it will not exceed 1024 shm memory node */
		if (rlsCnt >= 1024) {
			shm_error("%s,%d, un released node exceed 1024 max value, ingore others!!!!\n",__FUNCTION__,__LINE__);
			break;
		}
	}
	up(&(engine->m_mutex));

	for (idx = 0 ; idx < rlsCnt ; idx++) {
		alignaddr = release_array[idx];

		if (0 != memory_engine_free(engine,alignaddr)) {
			shm_error("%s,%d fail to relesae shm alignaddr %#.8x,for taskid:%d\n",
					__FUNCTION__, __LINE__, alignaddr, taskid);
		}
	}

	if (NULL != dynamic_release_array)
		kfree(dynamic_release_array);

	shm_debug("%s,%d should release shm block %d size %d for taskid %d\n",
			__FUNCTION__,__LINE__,rlsCnt,totalSize,taskid);
    return 0;
}

/*
 * release memory allocated by process pid
 */
int memory_engine_release_by_usrtaskid(memory_engine_t *engine, pid_t usrtaskid)
{
	size_t alignaddr = 0 ;
	memory_node_t *new_node = NULL;
	int rlsCnt = 0 ;
	int idx = 0;
	int totalSize = 0 ;
	int *release_usr_array = NULL;
	int static_release_array[64] = {0};
	int *dynamic_release_array = NULL;


	if (NULL == engine)
		return -EINVAL;

	release_usr_array = static_release_array;
	/* Walk all nodes until we have reached the root. (root.m_size == 0) */
	down(&(engine->m_mutex));
	for (new_node = engine->m_root.m_next ; new_node->m_size != 0 ; new_node = new_node->m_next) {
		if (NULL != new_node->m_next_free)
			continue;/* free node */

		if (usrtaskid == new_node->m_usrtaskid) {
			release_usr_array[rlsCnt++] = MEMNODE_ALIGN_ADDR(new_node);
			totalSize += new_node->m_size;
		} else
			continue;

		if (64 == rlsCnt) {
			dynamic_release_array = kzalloc(1024 * sizeof(int), GFP_KERNEL);
			if (NULL == dynamic_release_array) {
				shm_error("%s,%d fail to allocate memory for dynamic_release_array\n",
							__FUNCTION__,__LINE__);
				break;
			} else {
				memcpy(dynamic_release_array, release_usr_array, rlsCnt * sizeof(int));
				release_usr_array = dynamic_release_array;
			}
		}

		if (rlsCnt >= 1024) {
			shm_error("%s,%d, un released node exceed 1024 max value, ingore others!!!!\n",__FUNCTION__,__LINE__);
			break;
		}
	}
	up(&(engine->m_mutex));

	for (idx = 0 ; idx < rlsCnt ; idx++) {
		alignaddr = release_usr_array[idx];

		if (0 != memory_engine_free(engine,alignaddr)) {
			shm_error("%s,%d fail to relesae shm alignaddr %#.8x,for usrtaskid:%d\n",
					__FUNCTION__,__LINE__,alignaddr,usrtaskid);
		}
	}

	if (NULL != dynamic_release_array)
		kfree(dynamic_release_array);

	shm_debug("%s,%d should release shm block %d size %d for usrtaskid %d\n",
			__FUNCTION__,__LINE__,rlsCnt,totalSize,usrtaskid);
    return 0;
}


int memory_engine_get_stat(memory_engine_t *engine, struct shm_stat_info *stat)
{
	memory_node_t *mem_node = NULL;
	struct shm_usr_node *usr_node = NULL;
	int i = 0;

	shm_debug("memory_engine_get_stat enter. \n");
	if (NULL == engine || NULL == stat)
		return -EINVAL;

	down(&(engine->m_mutex));

	stat->m_size = engine->m_size;
	stat->m_used = engine->m_size_used;
	for (mem_node = engine->m_root.m_next ; mem_node->m_size != 0 ;
			mem_node = mem_node->m_next) {
		pid_t taskid;
		pid_t usrtaskid;

		/* free node*/
		if (NULL != mem_node->m_next_free)
			continue;

		taskid = mem_node->m_taskid;
		usrtaskid = mem_node->m_usrtaskid;

		//update allocate size
		for (i = 0 ; i < MAX_SHM_USR_NODE_NUM ; i++) {
			usr_node = &(stat->m_nodes[i]);
			/* find a usr_node with the same taskid */
			if (taskid == usr_node->m_taskid)
				break;
			/* occupy a usr_node for taskid */
			if (0 == usr_node->m_taskid) {
				stat->m_count++;
				usr_node->m_taskid = taskid;
				strncpy(usr_node->m_taskname,mem_node->m_taskname,16);

				usr_node->m_oom_adj = shm_find_oomadj_by_pid(usr_node->m_taskid);
				break;
			}
		}

		/* by default the total number of usr_node for shm user should not exceed MAX_SHM_USR_NODE_NUM */
		if (i >= MAX_SHM_USR_NODE_NUM) {
			shm_error("%s,%d stat out of memory\n",__FUNCTION__,__LINE__);
			goto err_exit;
		}

		usr_node->m_size_alloc += mem_node->m_size;

		/* if taskid equals to usrtaskid, then follow the shortcut*/
		if (taskid == usrtaskid) {
			usr_node->m_size_use += mem_node->m_size;
			continue;
		}

		usr_node = NULL;
		for (i = 0 ; i < MAX_SHM_USR_NODE_NUM ; i++) {
			usr_node = &(stat->m_nodes[i]);

			if (usrtaskid == usr_node->m_taskid)
				break;

			if (0 == usr_node->m_taskid) {
				stat->m_count++;
				usr_node->m_taskid = usrtaskid;
				strncpy(usr_node->m_taskname,mem_node->m_usrtaskname,16);

				usr_node->m_oom_adj = shm_find_oomadj_by_pid(usr_node->m_taskid);
				break;
			}
		}

		if (i >= MAX_SHM_USR_NODE_NUM ) {
			shm_error("%s,%d stat out of memory\n",__FUNCTION__,__LINE__);
			goto err_exit;
		}

		usr_node->m_size_use += mem_node->m_size;
	}
	up(&(engine->m_mutex));
	shm_debug("memory_engine_get_stat OK.\n");
	return 0;

err_exit:
	up(&(engine->m_mutex));
	shm_error("memory_engine_get_stat fail\n");
	return -EINVAL;
}

int memory_engine_dump_stat(memory_engine_t *engine)
{
	struct shm_stat_info *stat_info = NULL;
	int res = 0;
	int i = 0;

	shm_debug("memory_engine_dump_stat enter. \n");
	if (NULL == engine) {
		shm_error("memory_engine_dump_stat invalid param\n");
		return -EINVAL;
	}

	stat_info = kzalloc(sizeof(struct shm_stat_info),GFP_KERNEL);
	if (NULL == stat_info) {
		shm_error("memory_engine_dump_stat kmalloc fail\n");
		return -ENOMEM;
	}

	res = memory_engine_get_stat(engine,stat_info);
	if (0 != res) {
		shm_error("memory_engine_dump_stat fail to gat stat info\n");
		goto err_exit;
	}

	printk("total size : %d \nused : %d \ntask count : %d\n",
			stat_info->m_size,stat_info->m_used,stat_info->m_count);

	printk("  No |  task id(      name      ) | oom_adj |    alloc   |     use    |\n");
	printk("-----------------------------------------------------------------------\n");
	for (i = 0 ; i < stat_info->m_count ; i++)
		printk(" %3d | %8d(%16s) |   %3d   | %10d | %10d |\n", i, stat_info->m_nodes[i].m_taskid,
					stat_info->m_nodes[i].m_taskname, stat_info->m_nodes[i].m_oom_adj,
					stat_info->m_nodes[i].m_size_alloc, stat_info->m_nodes[i].m_size_use);


	shm_debug("memory_engine_dump_stat OK.\n");
	if (NULL != stat_info) {
		kfree(stat_info);
		stat_info = NULL;
	}
	return 0;

err_exit:
	if (NULL != stat_info) {
		kfree(stat_info);
		stat_info = NULL;
	}
	shm_error("memory_engine_dump_stat fail\n");
	return res;
}

/*static data struct for /proc/galois_shm/stat to avoid two many kzalloc */
static struct shm_stat_info gshm_stat;
int memory_engine_show_stat(memory_engine_t *engine, struct seq_file *file)
{
	int len = 0, i = 0;
	int res = 0;
	struct shm_stat_info *stat_info = &gshm_stat;

	shm_debug("memory_engine_show_stat enter. \n");
	if ((NULL == engine) || (NULL == file))
		return -EINVAL;

	memset(stat_info,0,sizeof(struct shm_stat_info));

	res = memory_engine_get_stat(engine, stat_info);
	if (0 != res) {
		shm_error("memory_engine_dump_stat fail to gat stat info\n");
		goto err_exit;
	}

	len += seq_printf(file,"total size : %d \nused : %d \ntask count : %d\n",
			stat_info->m_size,stat_info->m_used,stat_info->m_count);

	len += seq_printf(file,"  No |  task id(      name      ) | oom_adj |    alloc   |     use    |\n");
	len += seq_printf(file,"-----------------------------------------------------------------------\n");
	for (i = 0 ; i < stat_info->m_count ; i++)
		len += seq_printf(file, " %3d | %8d(%16s) |   %3d   | %10d | %10d |\n", i, stat_info->m_nodes[i].m_taskid,
					stat_info->m_nodes[i].m_taskname, stat_info->m_nodes[i].m_oom_adj,
					stat_info->m_nodes[i].m_size_alloc, stat_info->m_nodes[i].m_size_use);

	shm_debug("memory_engine_show_stat OK.\n");
	return len;
err_exit:
	shm_error("memory_engine_show_stat fail\n");
	return res;
}
