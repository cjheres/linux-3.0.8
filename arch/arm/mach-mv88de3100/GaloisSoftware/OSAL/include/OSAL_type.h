/********************************************************************************
 * Marvell GPL License Option
 *
 * If you received this File from Marvell, you may opt to use, redistribute and/or
 * modify this File in accordance with the terms and conditions of the General
 * Public License Version 2, June 1991 (the "GPL License"), a copy of which is
 * available along with the File in the license.txt file or by writing to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or
 * on the worldwide web at http://www.gnu.org/licenses/gpl.txt.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY
 * DISCLAIMED.  The GPL License provides additional details about this warranty
 * disclaimer.
 ********************************************************************************/


////////////////////////////////////////////////////////////////////////////////
//! \file OSAL_type.h
//! \brief OSAL module common constant definition and data structure type definition
//!			
//!	
//! Purpose:
//!	   Shared the all code of the OSAL module
//!
//!	Version, Date and Author :
//!		V 1.00,	3/14/2007,	Fang Bao	
//!		V 1.01,	5/15/2007,	Fang Bao,       Add MV_ASSERT into the MV_OSAL_ErrorHandle
//!							                Delete the marco MV_OSAL_ErrorExit, which has been replaced by MV_OSAL_ErrorHandle
//!		V 1.10,	5/28/2007,	Fang Bao,       Delete the MV_OSAL_STATUS and Error Code List, replaced by "ErrorCode.h"
//!		V 1.11,	3/07/2008,	Fang Bao,       Add MV_OSAL_DBG_Error_Ptr() and MV_OSAL_DBG_Error_Task() to remove warnings on ENABLE_DEBUG_OSAL off
//!		V 1.12,	6/02/2008,	Fang Bao,       Modify the MV_TIMER_TASK_PRIORITY_LINUX and MV_TIMER_TASK_PRIORITY_ECOS for [Bug 819]
//!		V 1.13,6/05/2008,	Fang Bao,       Replace error code E_NOPERMISSION by E_ACCESSDENIED
//!		V 1.13a,6/10/2008,	Fang Bao,       Remove E_ACCESSDENIED redefiniton in Win32 by OSAL_type.h
//!		V 1.14, 9/09/2008,	Fang Bao,       Add new CC Service Type - MV_CC_SrvType_MsgQEx
//!							                Add new Message Queue type - MV_CC_MsgQType_ExICC and MV_CC_MsgQType_ExITC
//!		V 1.15, 10/17/2008,	Fang Bao,       Add MV_CC_BitSet(), MV_CC_BitClean(), MV_CC_BitGet()
//!							                Add MV_CC_STATUS_BIT_START, MV_CC_STATUS_SrcCPU, MV_CC_STATUS_DstCPU
//!		V 1.16, 12/10/2008,	Fang Bao,       Modify MV_OSAL_DBG_Error(), MV_OSAL_DBG_Error_Ptr(), MV_OSAL_DBG_Error_Task()
//!							                Modify MV_CC_DBG_Error(), MV_CC_DBG_Error_Ptr(), MV_CC_DBG_Error_Task()
//!		V 1.17, 12/18/2008,	Fang Bao,       Modify MV_OSAL_Malloc(), use GaloisMalloc(), not malloc()
//!							                Modify MV_OSAL_Free(), use GaloisFree(), not free()
//!		V 1.18, 2/19/2008,	Fang Bao,       Modify MV_OSAL_DBG_Info() and MV_CC_DBG_Info() parameter
//!		V 1.19, 6/11/2008,	Fang Bao,       Add MV_CC_DBG_Error_UINT8 to remove a warning in CC_RPC_new.c
//!		V 1.20, 10/23/2009,	Fang Bao,       Add CCDSS, CCDSP support
//!     V 1.21, 02/04/2010, Fang Bao,       add data type MV_CC_SHM_PoolInfo_t for Shared Memory
//!     V 1.22, 05/06/2010, Fang Bao,       add command SHM_IOC_GetSize for Linux Device SHM
//!     V 1.23, 05/10/2010, Fang Bao,       add command SHM_IOC_GetAddr for Linux Device SHM
//!     V 1.24, 06/10/2010, Fang Bao,       add MV_CC_DSS_ServiceInfo_CBuf_t
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _OSAL_TYPE_H_
#define _OSAL_TYPE_H_

#if defined( __LINUX_KERNEL__ )

	#include <linux/slab.h>   /* kmalloc() */

#else
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>

#endif

#ifndef INLINE

#if defined( WIN32 )

#define INLINE 

#else

#define INLINE inline

#endif

#endif // <- INLINE

#include "com_type.h"
#include "ErrorCode.h"
#include "debug.h"


#ifdef __cplusplus
extern "C"
{
#endif	// #ifdef __cplusplus


////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL Data Structure: MV_OSAL_HANDLE | Handle to common OSAL object
//!
////////////////////////////////////////////////////////////////////////////////
typedef void *MV_OSAL_HANDLE;

////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL Data Structure: MV_CC_HANDLE | Handle to common CC object
//!
////////////////////////////////////////////////////////////////////////////////
typedef void *MV_CC_HANDLE;


////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL CPU constant definition: MV_OSAL_CPUID_U8_t
//!
////////////////////////////////////////////////////////////////////////////////
typedef UINT8 	MV_OSAL_CPUID_U8_t;

////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL Process constant definition: MV_OSAL_ProcessID_U8_t
//!
////////////////////////////////////////////////////////////////////////////////
typedef UINT8 	MV_OSAL_ProcessID_U8_t;

////////////////////////////////////////////////////////////////////////////////
//! \brief definition
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_OSAL_TRUE				(1)
#define MV_OSAL_FALSE				(0)

////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL Macro definition
//!
////////////////////////////////////////////////////////////////////////////////
#if defined( __LINUX_KERNEL__ )
	#define MV_OSAL_Malloc(x)			kmalloc((x), GFP_KERNEL)
#else
	#define MV_OSAL_Malloc(x)			CommonMemPoolMalloc(x)
#endif

#if defined( __LINUX_KERNEL__ )
	#define MV_OSAL_Free(p)		{		\
				if ((p) != NULL) {		\
					kfree(p);			\
					(p) = NULL;			\
				}						\
			}
#else
	#define MV_OSAL_Free(p)		CommonMemPoolFree(p)
#endif

#define UINT32_ADDR(a, b)		((UINT32 *)(a) + ((b) >> 2) )
#define UINT16_ADDR(a, b)		((UINT16 *)(a) + ((b) >> 1) )
#define UINT8_ADDR(a, b)		((UINT8 *)(a) + (b) )

#define INT32_ADDR(a, b)		((INT32 *)(a) + ((b) >> 2) )
#define INT16_ADDR(a, b)		((INT16 *)(a) + ((b) >> 1) )
#define INT8_ADDR(a, b)			((INT8 *)(a) + (b) )




#ifdef ENABLE_DEBUG_OSAL

#if defined( __LINUX_KERNEL__ )

	#define	MV_OSAL_ErrorHandle(s,x)		{		\
				ERRPRINTF((KERN_ALERT "[ERROR] - [%s] Return MV_OSAL_Err_Code [0x%08x]\n",(s),(x)));	\
				return(x); \
			}
			
	#define	MV_OSAL_ErrorPrint(s,x)			{		\
				ERRPRINTF((KERN_ALERT "[Warning] - [%s] Return MV_OSAL_Err_Code [0x%08x]\n",(s),(x)));	\
			}
			
#else

	#define	MV_OSAL_ErrorHandle(s,x)		{		\
				ERRPRINTF(("[ERROR] - [%s] Return MV_OSAL_Err_Code [0x%08x]\n",(s),(x)));	\
				MV_ASSERT(0); \
			}
			
	#define	MV_OSAL_ErrorPrint(s,x)			{		\
				ERRPRINTF(("[Warning] - [%s] Return MV_OSAL_Err_Code [0x%08x]\n",(s),(x)));	\
			}
			
	#define	MV_OSAL_TASK_ErrorExit(s,x)		{									\
				ERRPRINTF(("[ERROR] - [%s] Exit MV_OSAL_Err_Code [0x%08x]\n",(s),(x)));	\
				MV_OSAL_Task_Exit(NULL);													\
			}
#endif
		
#else

#if defined( __LINUX_KERNEL__ )

	#define	MV_OSAL_ErrorHandle(s,x)		return x
	#define	MV_OSAL_ErrorPrint(s,x)

#else

	#define	MV_OSAL_ErrorHandle(s,x)		return x
	#define	MV_OSAL_ErrorPrint(s,x)
	#define	MV_OSAL_TASK_ErrorExit(s,x)		exit(x)

#endif
	
#endif

#ifdef ENABLE_DEBUG_OSAL

#define	MV_OSAL_DBG_Warning(x, s, ...)		dbg_Out(DBG_OSAL DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
#define	MV_OSAL_DBG_Info(...)				dbg_Out(DBG_OSAL DBG_LV5 " " __VA_ARGS__)
#define	MV_OSAL_DBG_Error(x, s, ...)		{ dbg_Out(DBG_OSAL DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }
#define	MV_OSAL_DBG_Error_Ptr(x, s, ...)	{ dbg_Out(DBG_OSAL DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }
#define	MV_OSAL_DBG_Error_Task(x, s, ...)	{ dbg_Out(DBG_OSAL DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }

#define	MV_CC_DBG_Error_UINT8(x, s, ...)	{ dbg_Out(DBG_CC DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }
#define	MV_CC_DBG_Error_Ptr(x, s, ...)		{ dbg_Out(DBG_CC DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }
#define	MV_CC_DBG_Error_Task(x, s, ...)		{ dbg_Out(DBG_CC DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }

	#if defined( __LINUX_KERNEL__ )
	
		#define	MV_CC_DBG_Warning(x, s, ...)	printk(KERN_ALERT "(ECode=%08x) | " s "\n", x)
		#define	MV_CC_DBG_Info(...)				printk(KERN_INFO " " __VA_ARGS__)
		#define	MV_CC_DBG_Error(x, s, ...)		{ printk(KERN_ALERT "(ECode=%08x) | " s "\n", x); return x; }
	
	#else
	
		#define	MV_CC_DBG_Warning(x, s, ...)	dbg_Out(DBG_CC DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
		#define	MV_CC_DBG_Info(...)				dbg_Out(DBG_CC DBG_LV5 " " __VA_ARGS__)
		#define	MV_CC_DBG_Error(x, s, ...)		{ dbg_Out(DBG_CC DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }

	#endif

#else

#define	MV_OSAL_DBG_Warning(x, s, ...)
#define	MV_OSAL_DBG_Info(...)
#define	MV_OSAL_DBG_Error(x, s, ...)		{return x;}
#define	MV_OSAL_DBG_Error_Ptr(x, s, ...)	{return NULL;}
#define	MV_OSAL_DBG_Error_Task(x, s, ...)	{MV_OSAL_Task_Exit(NULL);}

#define	MV_CC_DBG_Warning(x, s, ...)
#define	MV_CC_DBG_Info(...)
#define	MV_CC_DBG_Error(x, s, ...)			{return x;}
#define	MV_CC_DBG_Error_UINT8(x, s, ...)	{return 0;}
#define	MV_CC_DBG_Error_Ptr(x, s, ...)		{return NULL;}
#define	MV_CC_DBG_Error_Task(x, s, ...)		{MV_OSAL_Task_Exit(NULL);}

#endif


////////////////////////////////////////////////////////////////////////////////
//! \brief ICCFIFO constant definition
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_CC_CreateFrameBuf() 			(UCHAR *)MV_OSAL_Malloc(MV_CC_ICCFIFO_FRAME_SIZE)
#define MV_CC_DestroyFrameBuf(x)		MV_OSAL_Free(x)



////////////////////////////////////////////////////////////////////////////////
//! \brief ICCP Datagram Macro definition (For Default Frame Buffer pointer x)
//!
////////////////////////////////////////////////////////////////////////////////
#define ICCP_SrcCPU(x)					(((UCHAR *)(x))[MV_CC_ICCP_HEAD_BYTEOFFSET + 0])
#define ICCP_SrcProcess(x)				(((UCHAR *)(x))[MV_CC_ICCP_HEAD_BYTEOFFSET + 1])
#define ICCP_DstCPU(x)					(((UCHAR *)(x))[MV_CC_ICCP_HEAD_BYTEOFFSET + 2])
#define ICCP_DstProcess(x)				(((UCHAR *)(x))[MV_CC_ICCP_HEAD_BYTEOFFSET + 3])
#define ICCP_DatagramID(x)				(((UINT16 *)(x))[(MV_CC_ICCP_HEAD_BYTEOFFSET / 2) + 2])
#define ICCP_Protocol(x)				(((UCHAR *)(x))[MV_CC_ICCP_HEAD_BYTEOFFSET + 6])
#define ICCP_Padding(x)					(((UCHAR *)(x))[MV_CC_ICCP_HEAD_BYTEOFFSET + 7])
#define ICCP_TimeStamp(x)				(((UINT32 *)(x))[(MV_CC_ICCP_HEAD_BYTEOFFSET / 4) + 2])

typedef enum _MV_CC_Protocol_U8 {
	MV_CC_Protocol_TEST 	= 0,
	MV_CC_Protocol_CCUDP	= 1,
	MV_CC_Protocol_CCDSP	= 2,
	MV_CC_Protocol_END		= 3,
} MV_CC_Protocol_U8_t;

#define MV_CC_Protocol_MAX				(MV_CC_Protocol_END)


////////////////////////////////////////////////////////////////////////////////
//! \brief CC-UDP Packet Macro definition (For Default Frame Buffer pointer x)
//!
////////////////////////////////////////////////////////////////////////////////
#define CCUDP_SrcPort(x)				(((UINT16 *)(x))[(MV_CC_CCUDP_HEAD_BYTEOFFSET / 2) + 0])
#define CCUDP_DstPort(x)				(((UINT16 *)(x))[(MV_CC_CCUDP_HEAD_BYTEOFFSET / 2) + 1])
#define CCUDP_PacketLen(x)				(((UINT16 *)(x))[(MV_CC_CCUDP_HEAD_BYTEOFFSET / 2) + 2])
#define CCUDP_PacketType(x)				(((UINT16 *)(x))[(MV_CC_CCUDP_HEAD_BYTEOFFSET / 2) + 3])
#define CCUDP_PacketSrvLevel(x)			(((UINT8 *)(x))[MV_CC_CCUDP_HEAD_BYTEOFFSET + 6])
#define CCUDP_PacketSrvType(x)			(((UINT8 *)(x))[MV_CC_CCUDP_HEAD_BYTEOFFSET + 7])

////////////////////////////////////////////////////////////////////////////////
//! \brief CC-DSP Packet Macro definition (For Default Frame Buffer pointer x)
//!
////////////////////////////////////////////////////////////////////////////////
#define CCDSP_Command(x)				(((UINT8 *)(x))[MV_CC_CCDSP_HEAD_BYTEOFFSET + 0])
#define CCDSP_Param1(x)					(((UINT8 *)(x))[MV_CC_CCDSP_HEAD_BYTEOFFSET + 1])
#define CCDSP_Param2(x)					(((UINT8 *)(x))[MV_CC_CCDSP_HEAD_BYTEOFFSET + 2])
#define CCDSP_Param3(x)					(((UINT8 *)(x))[MV_CC_CCDSP_HEAD_BYTEOFFSET + 3])

////////////////////////////////////////////////////////////////////////////////
//! \brief CC-Service Macro definition
//!
////////////////////////////////////////////////////////////////////////////////
typedef enum _MV_CC_SrvLevel_U8 {
	MV_CC_SrvLevel_ITC 		= 0,	/* Inter-Thread Communication */
	MV_CC_SrvLevel_IPC 		= 1,	/* Inter-Process Communication */
	MV_CC_SrvLevel_ICC 		= 2,	/* Inter-CPU Communication */
	MV_CC_NoCCSrvLevel		= 0xFF	/*  No CC Service */
} MV_CC_SrvLevel_U8_t;

typedef enum _MV_CC_SrvType_U8 {
	MV_CC_SrvType_MsgQ 		= 0,	/* Message Queue Service */
	MV_CC_SrvType_RPC 		= 1,	/* RPC Service */
	MV_CC_SrvType_CBuf 		= 2,	/* Circular Buffer Service */
	MV_CC_SrvType_MsgQEx	= 3,	/* Message Queue Extension Service */	
	MV_CC_SrvType_RPCClnt	= 4,	/* RPC Client */
	MV_CC_SrvType_Max		= 5,
	MV_CC_NoCCSrvType		= 0xFF	/*  No CC Service */	
} MV_CC_SrvType_U8_t;


////////////////////////////////////////////////////////////////////////////////
//! \brief ICC Address Data and Macro definition
//!
////////////////////////////////////////////////////////////////////////////////

typedef UINT16 	MV_OSAL_PortID_U16_t;

typedef struct _MV_CC_ICCAddr {

	MV_OSAL_CPUID_U8_t		m_CPU;
	MV_OSAL_ProcessID_U8_t	m_Process;
	MV_OSAL_PortID_U16_t 	m_Port;	
	
} MV_CC_ICCAddr_t, *pMV_CC_ICCAddr_t;


#define AddrToCCUDPSrc(a,b)	{									\
				ICCP_SrcCPU(b) = (a)->m_CPU;					\
				ICCP_SrcProcess(b) = (a)->m_Process;			\
				CCUDP_SrcPort(b) = (a)->m_Port;					\
			}

#define CCUDPSrcToAddr(b,a)	{									\
				(a)->m_CPU = ICCP_SrcCPU(b);					\
				(a)->m_Process = ICCP_SrcProcess(b);			\
				(a)->m_Port = CCUDP_SrcPort(b);					\
			}
			
#define AddrToCCUDPDst(a,b)	{									\
				ICCP_DstCPU(b) = (a)->m_CPU;					\
				ICCP_DstProcess(b) = (a)->m_Process;			\
				CCUDP_DstPort(b) = (a)->m_Port;					\
			}

#define CCUDPDstToAddr(b,a)	{									\
				(a)->m_CPU = ICCP_DstCPU(b);					\
				(a)->m_Process = ICCP_DstProcess(b);			\
				(a)->m_Port = CCUDP_DstPort(b);					\
			}
	
#define ICCAddr_CopyAtoB(a,b)	{								\
				(b)->m_CPU = (a)->m_CPU;						\
				(b)->m_Process = (a)->m_Process;				\
				(b)->m_Port = (a)->m_Port;						\
			}

#define ICCAddr_DiffAB(a,b)		(((b)->m_CPU != (a)->m_CPU) ||			\
								 ((b)->m_Process != (a)->m_Process) ||	\
								 ((b)->m_Port != (a)->m_Port))


////////////////////////////////////////////////////////////////////////////////
//! \brief CC Service definition: MV_OSAL_ServiceID_U32_t
//!
////////////////////////////////////////////////////////////////////////////////
typedef UINT32 	MV_OSAL_ServiceID_U32_t;
typedef UINT32 	MV_CC_ServiceID_U32_t;



////////////////////////////////////////////////////////////////////////////////
//! \brief Definition: the command of ICC Net operation
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_CC_NET_CMD_NODO					(0x00000000)
#define MV_CC_NET_CMD_RM					(0x00000001)
#define MV_CC_NET_CMD_SET					(0x00000002)


////////////////////////////////////////////////////////////////////////////////
//! \brief CC Flag Operation Macro definition
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_CC_FlagSet(x,a,n,m)				((a)?((x)|=(n)):((x)&=(m)))
#define MV_CC_FlagGet(x,n)					(((x)&(n))?(1):(0))
#define MV_CC_FlagCopyXtoY(x,y,n,m)			(((x)&(n))?((y)|=(n)):((y)&=(m)))
	
#define MV_CC_BitSet(x,n)					((x)|=(n))
#define MV_CC_BitClean(x,n)					((x)&=(~n))
#define MV_CC_BitGet(x,n)					(((x)&(n))?(1):(0))


////////////////////////////////////////////////////////////////////////////////
//! \brief CC Data Structure: MV_CC_CBufCtrlType_t | CBuf Control Object type
//!
////////////////////////////////////////////////////////////////////////////////
typedef enum _MV_CC_CBufCtrlType {
//	MV_CC_CBufCtrlType_None		= 0,	/* Cbuf NULL Object */
	MV_CC_CBufCtrlType_Consumer	= 1,	/* Cbuf Consumer(Server) Object */
	MV_CC_CBufCtrlType_Producer	= 2,	/* Cbuf Producer(client) Object */
} MV_CC_CBufCtrlType_t;



////////////////////////////////////////////////////////////////////////////////
//! \brief CC Data Structure: MV_CC_CBufCtrlType_t | CBuf Control Object type
//!
////////////////////////////////////////////////////////////////////////////////
typedef enum _MV_CC_MsgQType {
	MV_CC_MsgQType_None		= 0,	/* Message Queue (None) */
	MV_CC_MsgQType_ICC		= 1,	/* Message Queue (Inter-CPU) */
	MV_CC_MsgQType_ITC		= 2,	/* Message Queue (Inter-Task) */
	MV_CC_MsgQType_ExICC 	= 3,	/* Message Queue (Inter-CPU) Extension */
	MV_CC_MsgQType_ExITC 	= 4,	/* Message Queue (Inter-Task)Extension */
} MV_CC_MsgQType_t;




////////////////////////////////////////////////////////////////////////////////
//! \brief RPC Message Data Structure: MV_CC_RPC_CmdID_U16_t, MV_CC_RPC_RtnSt_U16_t
//!
////////////////////////////////////////////////////////////////////////////////
typedef UINT16	MV_CC_RPC_CmdID_U16_t;
typedef UINT16	MV_CC_RPC_RtnSt_U16_t;

#define MV_CC_RPC_SUCCESS					(MV_CC_RPC_RtnSt_U16_t)(0x0000)  /* RPC success */
#define MV_CC_RPC_FAILURE					(MV_CC_RPC_RtnSt_U16_t)(0x0001)  /* RPC failure (general)*/
#define MV_CC_RPC_ERRRTN_CMD				(MV_CC_RPC_RtnSt_U16_t)(0x0002)  /* RPC error  - command id*/
#define MV_CC_RPC_ERRRTN_PARAM				(MV_CC_RPC_RtnSt_U16_t)(0x0003)  /* RPC error  - parameter */
#define MV_CC_RPC_ERRRTN_NOSRVFUNC			(MV_CC_RPC_RtnSt_U16_t)(0x0004)  /* RPC error  - no rpc server function on dst address */
#define MV_CC_RPC_ERRRTN_SRVBUSY			(MV_CC_RPC_RtnSt_U16_t)(0x0005)  /* RPC error  - rpc server busy */



////////////////////////////////////////////////////////////////////////////////
//! \brief Data Structure: MV_CC_RPC_RegCmdItem_t, *pMV_CC_RPC_RegCmdItem_t
//! 
//! 	The m_pList should be a list sorted by m_Cmd key. And a[n] < a[n+1]
//!		So it is an ascending order list.
////////////////////////////////////////////////////////////////////////////////
typedef struct _MV_CC_RPC_RegCmdItem {
	
	MV_CC_RPC_CmdID_U16_t m_Cmd;

	/* Warning - If the OSAL Status code CmdFunc_RPCSrv return is not S_OK, 
	RPC Server will auto-return the RtnSt error code (MV_CC_RPC_FAILURE) only. */	
	INT32 (*pCmdFunc_RPCSrv)(MV_CC_RPC_RtnSt_U16_t 	*pRPC_Rtn, 
							UCHAR 					*pParamBuf, 
							UINT8 					*pParamSize, 
							PVOID		 			*pShmSlot);
	
} MV_CC_RPC_RegCmdItem_t, *pMV_CC_RPC_RegCmdItem_t;


//! SHM_cache_operation_t is used in Galois_Linux and Galois_Linux_Kernel
typedef struct _SHM_cache_operation {
               unsigned int adr;
               int size;
} SHM_cache_operation_t;

// IO command of Linux Device
#define SHM_IOC_Invalidate	    	    0x1F01
#define SHM_IOC_Clean	    		    0x1F03
#define SHM_IOC_CleanAndInvalidate    	0x1F05
#define SHM_IOC_GetSize	    	        0x1F07
#define SHM_IOC_GetAddr                 0x1F09

//! SHM_cache_operation_t is used in Galois_Linux and Galois_Linux_Kernel
typedef struct _MV_CC_SHM_PoolInfo {
        INT32 m_totalmem;                       // total size of entire shared memory pool
        INT32 m_freemem;                        // total space in bytes not in use
        INT32 m_origsize;                       // size of original region used when shared memory pool created
        INT32 m_maxfree;                        // size of largest unused block
        INT32 m_maxused;                        // size of largest memory used 
} MV_CC_SHM_PoolInfo_t, *pMV_CC_SHM_PoolInfo_t;

////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL Macro: MV_OSAL_PRIORITY_t |  The different priority levels for OSAL tasks. Low numbers indiciate higher priority. 
//!
////////////////////////////////////////////////////////////////////////////////
#if defined( __ECOS__ )

typedef enum _MV_OSAL_PRIORITY
{
    MV_OSAL_TASK_PRIORITY_1 = 1,           /* The highest task priority in eCos ( 0 is reserved by OSAL ) */
    MV_OSAL_TASK_PRIORITY_2,
    MV_OSAL_TASK_PRIORITY_3,
    MV_OSAL_TASK_PRIORITY_4,
    MV_OSAL_TASK_PRIORITY_5,
    MV_OSAL_TASK_PRIORITY_6,
    MV_OSAL_TASK_PRIORITY_7,
    MV_OSAL_TASK_PRIORITY_8,
    MV_OSAL_TASK_PRIORITY_9,
    MV_OSAL_TASK_PRIORITY_10,
    MV_OSAL_TASK_PRIORITY_11,
    MV_OSAL_TASK_PRIORITY_12,
    MV_OSAL_TASK_PRIORITY_13,
    MV_OSAL_TASK_PRIORITY_14,
    MV_OSAL_TASK_PRIORITY_15,
    MV_OSAL_TASK_PRIORITY_16,
    MV_OSAL_TASK_PRIORITY_17,
    MV_OSAL_TASK_PRIORITY_18,
    MV_OSAL_TASK_PRIORITY_19,
    MV_OSAL_TASK_PRIORITY_20,				
    MV_OSAL_TASK_PRIORITY_21,
    MV_OSAL_TASK_PRIORITY_22,
    MV_OSAL_TASK_PRIORITY_23,
    MV_OSAL_TASK_PRIORITY_24,
    MV_OSAL_TASK_PRIORITY_25,
    MV_OSAL_TASK_PRIORITY_26,
    MV_OSAL_TASK_PRIORITY_27,
    MV_OSAL_TASK_PRIORITY_28,
    MV_OSAL_TASK_PRIORITY_29,
    MV_OSAL_TASK_PRIORITY_30 = 30			/* The lowest task priority in eCos  ( 31 is reserved by OSAL and it is used by idle task in eCos ) */
} MV_OSAL_PRIORITY_t;

#define MV_OSAL_TASK_PRIORITY_HIGHEST			MV_OSAL_TASK_PRIORITY_1
#define MV_OSAL_TASK_PRIORITY_LOWEST			MV_OSAL_TASK_PRIORITY_30
#define MV_OSAL_TASK_PRIORITY_DEFAULT			MV_OSAL_TASK_PRIORITY_LOWEST	/* The default task priotiry in eCos */
#define MV_OSAL_TASK_PRIORITY_MIN				MV_OSAL_TASK_PRIORITY_1
#define MV_OSAL_TASK_PRIORITY_MAX				MV_OSAL_TASK_PRIORITY_30

#elif defined( __LINUX__ ) || defined( __LINUX_KERNEL__ )

typedef enum _MV_OSAL_PRIORITY
{
	MV_OSAL_TASK_PRIORITY_DEFAULT = 0,		/* The default task schedpolicy, no task priority when schedpolicy = SCHED_OTHER in Linux */
    MV_OSAL_TASK_PRIORITY_30 = 1,        	/* The lowest task priority, schedpolicy = SCHED_RR in Linux ( 0 is unavailable ) */
    MV_OSAL_TASK_PRIORITY_29,
    MV_OSAL_TASK_PRIORITY_28,
    MV_OSAL_TASK_PRIORITY_27,
    MV_OSAL_TASK_PRIORITY_26,
    MV_OSAL_TASK_PRIORITY_25,
    MV_OSAL_TASK_PRIORITY_24,
    MV_OSAL_TASK_PRIORITY_23,
    MV_OSAL_TASK_PRIORITY_22,
    MV_OSAL_TASK_PRIORITY_21,
    MV_OSAL_TASK_PRIORITY_20,
    MV_OSAL_TASK_PRIORITY_19,
    MV_OSAL_TASK_PRIORITY_18,
    MV_OSAL_TASK_PRIORITY_17,
    MV_OSAL_TASK_PRIORITY_16,
    MV_OSAL_TASK_PRIORITY_15,
    MV_OSAL_TASK_PRIORITY_14,
    MV_OSAL_TASK_PRIORITY_13,
    MV_OSAL_TASK_PRIORITY_12,
    MV_OSAL_TASK_PRIORITY_11,
    MV_OSAL_TASK_PRIORITY_10,
    MV_OSAL_TASK_PRIORITY_9,
    MV_OSAL_TASK_PRIORITY_8,
    MV_OSAL_TASK_PRIORITY_7,
    MV_OSAL_TASK_PRIORITY_6,
    MV_OSAL_TASK_PRIORITY_5,
    MV_OSAL_TASK_PRIORITY_4,
    MV_OSAL_TASK_PRIORITY_3,
    MV_OSAL_TASK_PRIORITY_2,
    MV_OSAL_TASK_PRIORITY_1 = 30            /* The highest task priority, schedpolicy = SCHED_RR in Linux ( 99 is the best value in Linux ) */
} MV_OSAL_PRIORITY_t;

//#define MV_OSAL_TASK_PRIORITY_HIGHEST			MV_OSAL_TASK_PRIORITY_1
#define MV_OSAL_TASK_PRIORITY_HIGHEST			99
#define MV_OSAL_TASK_PRIORITY_LOWEST			MV_OSAL_TASK_PRIORITY_30	
#define MV_OSAL_TASK_PRIORITY_MIN				MV_OSAL_TASK_PRIORITY_DEFAULT
#if defined(BERLIN_SINGLE_CPU) && defined(SINGLE_CPU_FLICK_HACK)
//#define MV_OSAL_TASK_PRIORITY_MAX				31
#define MV_OSAL_TASK_PRIORITY_MAX				99
#else
#define MV_OSAL_TASK_PRIORITY_MAX				MV_OSAL_TASK_PRIORITY_1
#endif

#elif defined( WIN32 )

typedef enum _MV_OSAL_PRIORITY
{
    MV_OSAL_TASK_PRIORITY_1,				/* The highest task priority in Win32 */
    MV_OSAL_TASK_PRIORITY_2,
    MV_OSAL_TASK_PRIORITY_3,
    MV_OSAL_TASK_PRIORITY_4,
    MV_OSAL_TASK_PRIORITY_5,
    MV_OSAL_TASK_PRIORITY_6,
    MV_OSAL_TASK_PRIORITY_7,
    MV_OSAL_TASK_PRIORITY_8,
    MV_OSAL_TASK_PRIORITY_9,
    MV_OSAL_TASK_PRIORITY_10,
    MV_OSAL_TASK_PRIORITY_11,
    MV_OSAL_TASK_PRIORITY_12,
    MV_OSAL_TASK_PRIORITY_13,
    MV_OSAL_TASK_PRIORITY_14,
    MV_OSAL_TASK_PRIORITY_15,
    MV_OSAL_TASK_PRIORITY_16,
    MV_OSAL_TASK_PRIORITY_17,
    MV_OSAL_TASK_PRIORITY_18,
    MV_OSAL_TASK_PRIORITY_19,
    MV_OSAL_TASK_PRIORITY_20,
    MV_OSAL_TASK_PRIORITY_21,
    MV_OSAL_TASK_PRIORITY_22,
    MV_OSAL_TASK_PRIORITY_23,
    MV_OSAL_TASK_PRIORITY_24,
    MV_OSAL_TASK_PRIORITY_25,
    MV_OSAL_TASK_PRIORITY_26,
    MV_OSAL_TASK_PRIORITY_27,
    MV_OSAL_TASK_PRIORITY_28,
    MV_OSAL_TASK_PRIORITY_29,
    MV_OSAL_TASK_PRIORITY_30           		/* The lowest task priority in Win32 */
} MV_OSAL_PRIORITY_t;

#define MV_OSAL_TASK_PRIORITY_HIGHEST			MV_OSAL_TASK_PRIORITY_1
#define MV_OSAL_TASK_PRIORITY_LOWEST			MV_OSAL_TASK_PRIORITY_1
#define MV_OSAL_TASK_PRIORITY_DEFAULT			MV_OSAL_TASK_PRIORITY_1
#define MV_OSAL_TASK_PRIORITY_MIN				MV_OSAL_TASK_PRIORITY_1
#define MV_OSAL_TASK_PRIORITY_MAX				MV_OSAL_TASK_PRIORITY_1

#endif


////////////////////////////////////////////////////////////////////////////////
//! \brief CBuf Flags Definition (m_Flags in MV_CC_CBufBody_t, MV_CC_CBuf_Producer_t, MV_CC_CBuf_Consumer_t)
//!
////////////////////////////////////////////////////////////////////////////////
// Flag->MV_CC_CBUF_FLAGS_ALIGNMENT :	memory boundary block should be added to the Circular Buffer Length.
// If MV_CC_CBUF_FLAGS_ALIGNMENT_Yes exist, memory boundary block should be added to the Circular Buffer Length.
#define MV_CC_CBUF_FLAGS_ALIGNMENT				(UINT32)(0x00000001)		/* n */
#define MV_CC_CBUF_FLAGS_ALIGNMENT_MASK			(UINT32)(0xFFFFFFFE)		/* m */
#define MV_CC_CBUF_FLAGS_ALIGNMENT_No			(UINT32)(0)					/* a */
#define MV_CC_CBUF_FLAGS_ALIGNMENT_Yes			(UINT32)(1)					/* a */

// Flag->MV_CC_CBUF_FLAGS_NOTICE :   create a message queue for producer to post a message when writing data to inform comsuer to read cbuf.
// The notice status is used for MV_CC_CBuf_Create to create a message queue for producer to post a message when writing data to inform comsuer to read cbuf.
#define MV_CC_CBUF_FLAGS_NOTICESTAT				(UINT32)(0x00000002)		/* n */
#define MV_CC_CBUF_FLAGS_NOTICESTAT_MASK		(UINT32)(0xFFFFFFFD)		/* m */
#define MV_CC_CBUF_FLAGS_NOTICESTAT_No			(UINT32)(0)					/* a */
#define MV_CC_CBUF_FLAGS_NOTICESTAT_Yes			(UINT32)(1)					/* a */

// Flag->MV_CC_CBUF_FLAGS_LOCALCPU :   create a message queue used only on local CPU (allowed by Consumer)
// The MV_CC_CBUF_FLAGS_LOCALCPU_Yes status is used for MV_CC_CBuf_Create to create a message queue whihc used only on local CPU, so the buffer is in cache memory for faster access
#define MV_CC_CBUF_FLAGS_LOCALCPU				(UINT32)(0x00000004)		/* n */
#define MV_CC_CBUF_FLAGS_LOCALCPU_MASK			(UINT32)(0xFFFFFFFB)		/* m */
#define MV_CC_CBUF_FLAGS_LOCALCPU_No			(UINT32)(0)					/* a */
#define MV_CC_CBUF_FLAGS_LOCALCPU_Yes			(UINT32)(1)					/* a */

// MV_CC_StartStatus :
#define MV_CC_STATUS_BIT_START					(UINT32)(0x00000001)		/* n */
#define MV_CC_STATUS_OK							(UINT32)(0x00000001)


////////////////////////////////////////////////////////////////////////////////
//! \brief Global Service (GS)Definition : MV_CC_DSS_ServiceInfo_t
//!
////////////////////////////////////////////////////////////////////////////////
typedef struct _MV_CC_DSS_ServiceInfo {

	// public info
	MV_CC_ServiceID_U32_t 	m_ServiceID;

	UINT8 					m_SrvLevel;
	UINT8 					m_SrvType;
	UINT8					m_Pad1;
	UINT8					m_Pad2;
	
	// private info
	UINT8					m_Data[24];

} MV_CC_DSS_ServiceInfo_t, *pMV_CC_DSS_ServiceInfo_t;

typedef struct _MV_CC_DSS_ServiceInfo_MsgQ {

	// public info
	MV_CC_ServiceID_U32_t 	m_ServiceID;

	UINT8 					m_SrvLevel;
	UINT8 					m_SrvType;
	UINT8					m_Pad1;
	UINT8					m_Pad2;
	
	// private info
	MV_CC_ICCAddr_t 		m_SrvAddr;
	UINT32 		 			m_NMsgsMax;
	UINT8					m_Blank[16];
		
} MV_CC_DSS_ServiceInfo_MsgQ_t, *pMV_CC_DSS_ServiceInfo_MsgQ_t;

typedef struct _MV_CC_DSS_ServiceInfo_MsgQEx {

	// public info
	MV_CC_ServiceID_U32_t 	m_ServiceID;

	UINT8 					m_SrvLevel;
	UINT8 					m_SrvType;
	UINT8					m_Pad1;
	UINT8					m_Pad2;
		
	// private info
	MV_CC_ICCAddr_t 		m_SrvAddr;
	UINT32 		 			m_NMsgsMax;
	UINT32					m_MsgLenMax;
	CHAR					m_Blank[12];
	
} MV_CC_DSS_ServiceInfo_MsgQEx_t, *pMV_CC_DSS_ServiceInfo_MsgQEx_t;

typedef struct _MV_CC_DSS_ServiceInfo_RPC {

	// public info
	MV_CC_ServiceID_U32_t 	m_ServiceID;

	UINT8 					m_SrvLevel;
	UINT8 					m_SrvType;
	UINT8					m_Pad1;
	UINT8					m_Pad2;

	// private info
	MV_CC_ICCAddr_t 		m_SrvAddr;
	UINT32 					m_RegCmdListNum;
	CHAR					m_Blank[16];
	
} MV_CC_DSS_ServiceInfo_RPC_t, *pMV_CC_DSS_ServiceInfo_RPC_t;

typedef struct _MV_CC_DSS_ServiceInfo_CBuf {

	// public info
	MV_CC_ServiceID_U32_t 	m_ServiceID;

	UINT8 					m_SrvLevel;
	UINT8 					m_SrvType;
	UINT8					m_Pad1;
	UINT8					m_Pad2;

	// private info
	UINT32                  m_seqid;                //! the number of CBuf client
    UINT32           		m_CBufBody_SHMOffset;   //! pointer to the CBuf Body
	UINT32 					m_BufSize;			    //! Circular Buffer data buffer byte size exclude the CBuf head and pending
	UINT32 					m_EndBlockSize;		    //! Circular Buffer Max Block byte size for reading or writing at a time, and also as the size of addition block after buffer end for circular operation
	UINT32 					m_Flags;			    //! flags of the Circular Buffer Client, initialize from CBuf settings flags when init;
	MV_CC_ServiceID_U32_t 	m_NoticeMsgQSID;	    //! It is only available on m_Flags = MV_CC_CBUF_FLAGS_NOTICESTAT_Yes
	
} MV_CC_DSS_ServiceInfo_CBuf_t, *pMV_CC_DSS_ServiceInfo_CBuf_t;

typedef struct _MV_CC_DSS_ServiceInfo_DataList {

	UINT32					m_ListNum;
	UINT32					m_DataNum;
	UINT32					m_MaxNum;
	
	MV_CC_DSS_ServiceInfo_t	m_SrvInfo[];

} MV_CC_DSS_ServiceInfo_DataList_t, *pMV_CC_DSS_ServiceInfo_DataList_t;

#ifdef __cplusplus
}		// extern "C"
#endif	// #ifdef __cplusplus

#endif
