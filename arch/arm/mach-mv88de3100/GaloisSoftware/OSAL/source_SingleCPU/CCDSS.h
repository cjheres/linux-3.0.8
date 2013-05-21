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
//! \file CCDSS.c
//! \brief
//!
//! Purpose:
//!
//!
//!	Version, Date and Author :
//!		V 1.00,	10/23/2007,	Fang Bao
//!		V 1.01,	10/28/2007,	Fang Bao, 	replace (struct semaphore) by (spinlock_t).
//!										spin_lock_bh could disable SoftIRQ (MV_ICC_Rx_action)
//!		V 2.00,	06/10/2010,	Fang Bao, 	design for Single CPU system
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _CCDSS_H_
#define _CCDSS_H_

#if defined( __LINUX_KERNEL__ )

#include <linux/spinlock.h>

#endif

#include "OSAL_api.h"
#include "GSList.h"


////////////////////////////////////////////////////////////////////////////////
//! \brief DSS Data Structure Macro (x = ICCP datagram buffer)
//!
////////////////////////////////////////////////////////////////////////////////
#define DSPPacket_HeadStart(x)				(UCHAR *)((x) + MV_CC_CCDSP_HEAD_BYTEOFFSET)
#define DSPPacket_DataStart(x)				(UCHAR *)((x) + MV_CC_CCDSP_DATA_BYTEOFFSET)
#define DSPPacket_HeadGEN(a, b, c, d)		((((UINT32)(d))<<24) |(((UINT32)(c))<<16) | (((UINT32)(b))<<8) | ((UINT32)(a)))
#define MV_CC_DSS_REMOTECPU_NONE 			(MV_OSAL_CPU_NUM)

#define MV_CC_SID_BIT_DYNAMIC 				(0x80000000)
#define MV_CC_SID_BIT_NORMAL 				(0x00000000)
#define MV_CC_SID_BIT_CPU0	 				(0x00000000)
#define MV_CC_SID_BIT_CPU1	 				(0x01000000)
#define MV_CC_SID_BIT_CPUX	 				(0x3F000000)

#if ( CPUINDEX == MV_OSAL_CPU_0 )
	#define MV_CC_DSS_REMOTECPU_INIT		MV_OSAL_CPU_1
	#define MV_CC_SID_BIT_LOCAL				MV_CC_SID_BIT_CPU0
#elif ( CPUINDEX == MV_OSAL_CPU_1 )
	#define MV_CC_DSS_REMOTECPU_INIT		MV_OSAL_CPU_0
	#define MV_CC_SID_BIT_LOCAL				MV_CC_SID_BIT_CPU1
#else
	#define MV_CC_DSS_REMOTECPU_INIT		MV_CC_DSS_REMOTECPU_NONE
	#define MV_CC_SID_BIT_LOCAL				MV_CC_SID_BIT_CPUX
#endif

typedef struct _MV_CC_DSS_Status {

	UINT32 m_RegCount;
	UINT32 m_RegErrCount;

	UINT32 m_FreeCount;
	UINT32 m_FreeErrCount;

	UINT32 m_InquiryCount;
	UINT32 m_InquiryErrCount;

	UINT32 m_UpdateCount;
	UINT32 m_UpdateErrCount;

	UINT32 m_ServiceCount;
	UINT32 m_LastServiceID;

	UINT32 m_SeqID;

} MV_CC_DSS_Status_t, *pMV_CC_DSS_Status_t;

#if defined( __LINUX_KERNEL__ )

typedef struct cc_node {
	unsigned int serverid;
	struct cc_node * pnext;
} MV_CC_Node;

typedef struct mv_cc_task {
	pid_t cc_taskid;
	char cc_taskname[16];
	MV_CC_Node *serverid_head;
} MV_CC_Task;
#endif

typedef struct _MV_CC_DSP {

	/* CC-DSP private data structure */
#if defined( __LINUX_KERNEL__ )
	spinlock_t 				m_hGSListMutex;
#else
	MV_OSAL_HANDLE_MUTEX_t 	m_hGSListMutex;
#endif

	MV_CC_ServiceID_U32_t 	m_SeqID;

#if defined( __LINUX_KERNEL__ )
	spinlock_t 				m_SeqIDMutex;
#else
	MV_OSAL_HANDLE_SEM_t 	m_SeqIDMutex;
#endif

    MV_CC_DSS_Status_t      m_Status;

} MV_CC_DSP_t, *pMV_CC_DSP_t;

// Public APIs
HRESULT MV_CC_DSS_Init(void);
HRESULT MV_CC_DSS_Exit(void);
#if defined( __LINUX_KERNEL__ )
HRESULT MV_CC_DSS_Reg(pMV_CC_DSS_ServiceInfo_t pSrvInfo,MV_CC_Task *cc_task);
HRESULT MV_CC_DSS_Free(pMV_CC_DSS_ServiceInfo_t pSrvInfo,MV_CC_Task *cc_task);
HRESULT MV_CC_DSS_Release_By_Taskid(MV_CC_Task *cc_task);
HRESULT singlenode_init(MV_CC_Node **head);
#else
HRESULT MV_CC_DSS_Reg(pMV_CC_DSS_ServiceInfo_t pSrvInfo);
HRESULT MV_CC_DSS_Free(pMV_CC_DSS_ServiceInfo_t pSrvInfo);
#endif
HRESULT MV_CC_DSS_Inquiry(pMV_CC_DSS_ServiceInfo_t pSrvInfo);
HRESULT MV_CC_DSS_Update(pMV_CC_DSS_ServiceInfo_t pSrvInfo);
HRESULT MV_CC_DSS_GetList(pMV_CC_DSS_ServiceInfo_DataList_t pSrvInfoList);
HRESULT MV_CC_DSS_GetStatus(pMV_CC_DSS_Status_t pStatus);


#endif	// #define _CCDSS_H_
