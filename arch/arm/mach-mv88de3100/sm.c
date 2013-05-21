#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/io.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>
#include <mach/galois_generic.h>
#include <linux/sched.h>
#include "SM_common.h"
#include "SysMgr.h"

#ifdef SM_DEBUG
#define DPRINT(fmt, args...) printk(fmt, ## args)
#else
#define DPRINT(fmt, args...)
#endif

#define SM_Q_PUSH( pSM_Q ) {				\
	pSM_Q->m_iWrite += SM_MSG_SIZE;			\
	if( pSM_Q->m_iWrite >= SM_MSGQ_SIZE )		\
		pSM_Q->m_iWrite -= SM_MSGQ_SIZE;	\
	pSM_Q->m_iWriteTotal += SM_MSG_SIZE; }

#define SM_Q_POP( pSM_Q ) {				\
	pSM_Q->m_iRead += SM_MSG_SIZE;			\
	if( pSM_Q->m_iRead >= SM_MSGQ_SIZE )		\
		pSM_Q->m_iRead -= SM_MSGQ_SIZE;		\
	pSM_Q->m_iReadTotal += SM_MSG_SIZE; }

static int bsm_link_msg(MV_SM_MsgQ *q, MV_SM_Message *m, spinlock_t *lock)
{
	unsigned long flags;
	MV_SM_Message *p;
	int ret = 0;

	if (lock)
		spin_lock_irqsave(lock, flags);

	if (q->m_iWrite < 0 || q->m_iWrite >= SM_MSGQ_SIZE) {
		/* buggy ? */
		ret = -EIO;
		goto out;
	}

	/* message queue full, ignore the newest message */
	if (q->m_iRead == q->m_iWrite && q->m_iReadTotal != q->m_iWriteTotal) {
		ret = -EBUSY;
		goto out;
	}

	p = (MV_SM_Message*)(&(q->m_Queue[q->m_iWrite]));
	memcpy(p, m, sizeof(*p));
	SM_Q_PUSH(q);
out:
	if (lock)
		spin_unlock_irqrestore(lock, flags);

	return ret;
}

static int bsm_unlink_msg(MV_SM_MsgQ *q, MV_SM_Message *m, spinlock_t *lock)
{
	unsigned long flags;
	MV_SM_Message *p;
	int ret = -EAGAIN; /* means no data */

	if (lock)
		spin_lock_irqsave(lock, flags);

	if (q->m_iRead < 0 || q->m_iRead >= SM_MSGQ_SIZE ||
			q->m_iReadTotal > q->m_iWriteTotal) {
		/* buggy ? */
		ret = -EIO;
		goto out;
	}

	/* if buffer was overflow written, only the last messages are
	 * saved in queue. move read pointer into the same position of
	 * write pointer and keep buffer full.
	 */
	if (q->m_iWriteTotal - q->m_iReadTotal > SM_MSGQ_SIZE) {
		int iTotalDataSize = q->m_iWriteTotal - q->m_iReadTotal;

		q->m_iReadTotal += iTotalDataSize - SM_MSGQ_SIZE;
		q->m_iRead += iTotalDataSize % SM_MSGQ_SIZE;
		if (q->m_iRead >= SM_MSGQ_SIZE)
			q->m_iRead -= SM_MSGQ_SIZE;
	}

	if (q->m_iReadTotal < q->m_iWriteTotal) {
		/* alright get one message */
		p = (MV_SM_Message*)(&(q->m_Queue[q->m_iRead]));
		memcpy(m, p, sizeof(*m));
		SM_Q_POP(q);
		ret = 0;
	}
out:
	if (lock)
		spin_unlock_irqrestore(lock, flags);

	return ret;
}

static spinlock_t sm_lock;

static inline int bsm_link_msg_to_sm(MV_SM_Message *m)
{
	MV_SM_MsgQ *q = (MV_SM_MsgQ *)(SOC_DTCM(SM_CPU0_INPUT_QUEUE_ADDR));

	return bsm_link_msg(q, m, &sm_lock);
}

static inline int bsm_unlink_msg_from_sm(MV_SM_Message *m)
{
	MV_SM_MsgQ *q = (MV_SM_MsgQ *)(SOC_DTCM(SM_CPU0_OUTPUT_QUEUE_ADDR));

	return bsm_unlink_msg(q, m, &sm_lock);
}

#define DEFINE_SM_MODULES(id)				\
	{						\
		.m_iModuleID  = id,			\
		.m_bWaitQueue = false,			\
	}

typedef struct {
	int m_iModuleID;
	bool m_bWaitQueue;
	wait_queue_head_t m_Queue;
	spinlock_t m_Lock;
	MV_SM_MsgQ m_MsgQ;
	struct mutex m_Mutex;
} MV_SM_Module;

static MV_SM_Module SMModules[MAX_MSG_TYPE] = {
	DEFINE_SM_MODULES(MV_SM_ID_SYS),
	DEFINE_SM_MODULES(MV_SM_ID_COMM),
	DEFINE_SM_MODULES(MV_SM_ID_IR),
	DEFINE_SM_MODULES(MV_SM_ID_KEY),
	DEFINE_SM_MODULES(MV_SM_ID_POWER),
	DEFINE_SM_MODULES(MV_SM_ID_WD),
	DEFINE_SM_MODULES(MV_SM_ID_TEMP),
	DEFINE_SM_MODULES(MV_SM_ID_VFD),
	DEFINE_SM_MODULES(MV_SM_ID_SPI),
	DEFINE_SM_MODULES(MV_SM_ID_I2C),
	DEFINE_SM_MODULES(MV_SM_ID_UART),
	DEFINE_SM_MODULES(MV_SM_ID_CEC),
};

static inline MV_SM_Module *bsm_search_module(int id)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(SMModules); i++)
		if (SMModules[i].m_iModuleID == id)
			return &(SMModules[i]);

	return id;
}

static int bsm_link_msg_to_module(MV_SM_Message *m)
{
	MV_SM_Module *module;
	int ret;

	module = bsm_search_module(m->m_iModuleID);
	if (!module)
		return -EINVAL;

	ret = bsm_link_msg(&(module->m_MsgQ), m, &(module->m_Lock));
	if (ret == 0) {
		/* wake up any process pending on wait-queue */
		wake_up_interruptible(&(module->m_Queue));
	}

	return ret;
}

static int bsm_unlink_msg_from_module(MV_SM_Message *m)
{
	MV_SM_Module *module;
	DEFINE_WAIT(__wait);
	unsigned long flags;
	int ret;

	module = bsm_search_module(m->m_iModuleID);
	if (!module)
		return -EINVAL;

repeat:
	spin_lock_irqsave(&(module->m_Lock), flags);

	if (!in_interrupt())
		prepare_to_wait(&(module->m_Queue), &__wait,
						TASK_INTERRUPTIBLE);

	ret = bsm_unlink_msg(&(module->m_MsgQ), m, NULL);
	if (ret == -EAGAIN && module->m_bWaitQueue && !in_interrupt()) {
		spin_unlock_irqrestore(&(module->m_Lock), flags);

		if (!signal_pending(current))
			schedule();
		else {
			finish_wait(&(module->m_Queue), &__wait);
			return -ERESTARTSYS;
		}

		goto repeat;
	}

	if (!in_interrupt())
		finish_wait(&(module->m_Queue), &__wait);

	spin_unlock_irqrestore(&(module->m_Lock), flags);

	return ret;
}

#ifdef CONFIG_MV88DE3100_IR
extern void girl_sm_int(void);
#else
#define girl_sm_int()	do {} while (0)
#endif

int bsm_msg_send(int id, void *msg, int len)
{
	MV_SM_Module *module;
	MV_SM_Message m;
	int msgdata = *(int *)msg;
	int ret;

	if (unlikely(len < 4) || unlikely(len > SM_MSG_BODY_SIZE))
		return -EINVAL;

	module = bsm_search_module(id);
	if (!module)
		return -EINVAL;

	if (MV_SM_ID_POWER == id &&
		(MV_SM_POWER_SYS_RESET == msgdata ||
		MV_SM_POWER_WARMDOWN_REQUEST == msgdata ||
		MV_SM_POWER_WARMDOWN_REQUEST_2 == msgdata)) {
		printk("Linux warm down.\n");
		/* wait for 50ms to make sure that NAND transaction
		 * is done
		 */
		mdelay(50);
	}

	m.m_iModuleID = id;
	m.m_iMsgLen   = len;
	memcpy(m.m_pucMsgBody, msg, len);
	for (;;) {
		ret = bsm_link_msg_to_sm(&m);
		if (ret != -EBUSY)
			break;
		mdelay(10);
	}

	return ret;
}
EXPORT_SYMBOL(bsm_msg_send);

int bsm_msg_receive(int id, void *pMsgBody, int *piLen)
{
	MV_SM_Message m;
	int ret;

	m.m_iModuleID = id;
	ret = bsm_unlink_msg_from_module(&m);
	if (ret)
		return ret;

	if (pMsgBody)
		memcpy(pMsgBody, m.m_pucMsgBody, m.m_iMsgLen);

	if (piLen)
		*piLen = m.m_iMsgLen;

	return 0;
}
EXPORT_SYMBOL(bsm_msg_receive);

static void bsm_msg_dispatch(void)
{
	MV_SM_Message m;
	int ret;

	/* read all messages from SM buffers and dispatch them */
	for (;;) {
		ret = bsm_unlink_msg_from_sm(&m);
		if (ret)
			break;

		/* try best to dispatch received message */
		ret = bsm_link_msg_to_module(&m);
		if (ret != 0) {
			printk(KERN_ERR "Drop SM message\n");
			continue;
		}

		/* special case for IR events */
		if (m.m_iModuleID == MV_SM_ID_IR)
			girl_sm_int();
	}
}

static irqreturn_t bsm_intr(int irq, void *dev_id)
{
	T32smSysCtl_SM_CTRL reg;

	reg.u32 = readl_relaxed(SM_APBC(SM_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL));
	reg.uSM_CTRL_SM2SOC_SW_INTR = 0;
	writel_relaxed(reg.u32, SM_APBC(SM_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL));

	bsm_msg_dispatch();

	return IRQ_HANDLED;
}

static ssize_t berlin_sm_read(struct file *file, char __user *buf,
				size_t count, loff_t *ppos)
{
	MV_SM_Message m;
	int id = (int)(*ppos);
	int ret;

	if (count < SM_MSG_SIZE)
		return -EINVAL;

	m.m_iModuleID = id;
	ret = bsm_unlink_msg_from_module(&m);
	if (!ret) {
		if (copy_to_user(buf, (void *)&m, SM_MSG_SIZE))
			return -EFAULT;
		return SM_MSG_SIZE;
	}

	return 0;
}

static ssize_t berlin_sm_write(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	MV_SM_Message SM_Msg;
	int ret;
	int id = (int)(*ppos);

	if (count < 4 || count > SM_MSG_BODY_SIZE)
		return -EINVAL;

	if (copy_from_user(SM_Msg.m_pucMsgBody, buf, count))
		return -EFAULT;

	ret = bsm_msg_send(id, SM_Msg.m_pucMsgBody, count);
	if (ret < 0)
		return -EFAULT;
	else
		return count;
}

static long berlin_sm_unlocked_ioctl(struct file *file,
			unsigned int cmd, unsigned long arg)
{
	MV_SM_Module *module;
	MV_SM_Message m;
	int ret = 0, id;

	if (cmd == SM_Enable_WaitQueue || cmd == SM_Disable_WaitQueue)
		id = (int)arg;
	else {
		if (copy_from_user(&m, (void __user *)arg, SM_MSG_SIZE))
			return -EFAULT;
		id = m.m_iModuleID;
	}

	module = bsm_search_module(id);
	if (!module)
		return -EINVAL;

	mutex_lock(&(module->m_Mutex));

	switch (cmd) {
	case SM_Enable_WaitQueue:
		module->m_bWaitQueue = true;
		break;
	case SM_Disable_WaitQueue:
		module->m_bWaitQueue = false;
		wake_up_interruptible(&(module->m_Queue));
		break;
	case SM_READ:
		ret = bsm_unlink_msg_from_module(&m);
		if (!ret) {
			if (copy_to_user((void __user *)arg, &m, SM_MSG_SIZE))
				ret = -EFAULT;
		}
		break;
	case SM_WRITE:
		ret = bsm_msg_send(m.m_iModuleID, m.m_pucMsgBody, m.m_iMsgLen);
		break;
	case SM_RDWR:
		ret = bsm_msg_send(m.m_iModuleID, m.m_pucMsgBody, m.m_iMsgLen);
		if (ret)
			break;
		ret = bsm_unlink_msg_from_module(&m);
		if (!ret) {
			if (copy_to_user((void __user *)arg, &m, SM_MSG_SIZE))
				ret = -EFAULT;
		}
		break;
	default:
		ret = -EINVAL;
		break;
	}

	mutex_unlock(&(module->m_Mutex));

	return ret;
}

static struct file_operations berlin_sm_fops = {
	.owner		= THIS_MODULE,
	.write		= berlin_sm_write,
	.read		= berlin_sm_read,
	.unlocked_ioctl	= berlin_sm_unlocked_ioctl,
};

static struct miscdevice sm_dev = {
	.minor	= BERLIN_SM_MISC_MINOR,
	.name	= BERLIN_SM_NAME,
	.fops	= &berlin_sm_fops,
};

static int __init berlin_sm_init(void)
{
	int i;

	spin_lock_init(&sm_lock);

	for (i = 0; i < ARRAY_SIZE(SMModules); i++) {
		init_waitqueue_head(&(SMModules[i].m_Queue));
		spin_lock_init(&(SMModules[i].m_Lock));
		mutex_init(&(SMModules[i].m_Mutex));
		memset(&(SMModules[i].m_MsgQ), 0, sizeof(MV_SM_MsgQ));
	}

	if (misc_register(&sm_dev))
		return -ENODEV;

	return request_irq(IRQ_SM2SOCINT, bsm_intr, 0, "bsm", &sm_dev);
}

static void __exit berlin_sm_exit(void)
{
	misc_deregister(&sm_dev);
	free_irq(IRQ_SM2SOCINT, &sm_dev);
}

module_init(berlin_sm_init);
module_exit(berlin_sm_exit);

MODULE_AUTHOR("Marvell-Galois");
MODULE_DESCRIPTION("System Manager Driver");
MODULE_LICENSE("GPL");
