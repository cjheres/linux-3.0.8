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
//! \file CC_CBuf_type.h
//! \brief CC CBuf type define
//!	
//!	   Platform = Common
//!	
//! Purpose:
//!
//!
//!	Version, Date and Author :
//!		V 1.00,	12/04/2007,	Fang Bao
//!		V 1.01,	01/11/2007,	Fang Bao
//!							Modify the m_MaxBlockSize by m_EndBlockSize in struct of MV_CC_CBufBody_t
//!	
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _MV_CC_CBUF_TYPE_H_
#define _MV_CC_CBUF_TYPE_H_

#include "OSAL_api.h"
#include "generic_buf.h"
#include "CCDSS.h"


////////////////////////////////////////////////////////////////////////////////
//! \brief Data Structure: MV_CC_CBufBody_t, pMV_CC_CBufBody_t;
//!
////////////////////////////////////////////////////////////////////////////////
typedef struct _MV_CC_CBuf_CBufBody {
	
volatile	UINT32 m_StartOffset;	//! the offset of the Circular Buffer start
volatile	UINT32 m_RdOffset;		//! read offset of the Circular Buffer
volatile	UINT32 m_WrOffset;		//! write offset of the Circular Buffer
volatile	UINT32 m_BufSize;		//! Circular Buffer data buffer byte size exclude the CBuf head and pending
volatile	UINT32 m_EndBlockSize;	//! The min linear space byte size for reading or writing at the end of the buffer , and which is the size of addition block after buffer end for circular operation
							//! Limitation: Circular Buffer End Block byte size <= (m_BufSize / 2 ) - 1
							//! If m_EndBlockSize == 0, No Min linear space to the Circular Buffer Block Operation, and no End Block for the Circular Buffer!!!!!!
volatile	UINT32 m_Flags;			//! Circular Buffer settings flags
	
} MV_CC_CBufBody_t, *pMV_CC_CBufBody_t;


////////////////////////////////////////////////////////////////////////////////
//! \brief CBuf Data Structure Macro (x = pMV_CC_CBufCtrl_t)
//!
////////////////////////////////////////////////////////////////////////////////
#define CBuf_BufferStart(x)						(UCHAR *)((UCHAR *)((x)->m_pCBufBody) + (x)->m_pCBufBody->m_StartOffset)


////////////////////////////////////////////////////////////////////////////////
//! \brief Data Structure: MV_CC_CBufCtrl_t, *pMV_CC_CBufCtrl_t
//! CBuf Client - Consumer & Producer
////////////////////////////////////////////////////////////////////////////////
typedef struct _MV_CC_CBufCtrl {
#ifdef BERLIN_SINGLE_CPU
	Buffer_Notifier_t       m_BufNtf;
#endif
	MV_CC_CBufCtrlType_t	m_type;
	MV_CC_ServiceID_U32_t	m_id;
	
	UINT32					m_seqid;			//! It is the sequence id for every CBufCtrl given by DSS.
	
	pMV_CC_CBufBody_t 		m_pCBufBody;		//! pointer to the CBuf Body (Shared memory non-cache address)

	UINT32 					m_BufSize;			//! Circular Buffer data buffer byte size exclude the CBuf head and pending
	UINT32 					m_EndBlockSize;		//! Circular Buffer Max Block byte size for reading or writing at a time, and also as the size of addition block after buffer end for circular operation
	UINT32 					m_Flags;			//! flags of the Circular Buffer Client, initialize from CBuf settings flags when init;

	//! Limitation: Circular Buffer Max Block byte size <= (m_BufSize / 2 ) - 1
	//! If m_EndBlockSize == 0, No Limitation to the Circular Buffer Block Operation, and no End Block for the Circular Buffer!!!!!!

    UINT32           		m_CBufBody_SHMOffset;

	MV_CC_ServiceID_U32_t 	m_NoticeMsgQSID;	//! It is only available on m_Flags = MV_CC_CBUF_FLAGS_NOTICESTAT_Yes
												//! And the NoticeMsgQ is created by Consumer, the address is stored in DSS for Producer.

	//! Only for Consumer
	UINT32 					m_EndBlockDataSize;	//! the data size of End Block copied from Circular Buffer Start Block for rolling back reading
												//! It will be cleared when Rd pointer rolls back
	MV_CC_HANDLE_MsgQ_t 	m_NoticeMsgQHandle;
	
	UINT32 					m_RWCount;
	UINT32 					m_RWErrCount;

} MV_CC_CBufCtrl_t, *pMV_CC_CBufCtrl_t;


#define MV_CC_HAL_FlushMemory(x)		MV_CC_HAL_FlushMemory_Platform(x)

#define MV_CC_HAL_FlushMemory_Platform(x)	{			\
	volatile UINT i;										\
	i = (UINT)(x);											\
}


HRESULT  MV_CC_CBufBody_Create(UINT32   				*pSHMOffset,
						   	   UINT32					BufSize,
						   	   UINT32					EndBlockSize,
						   	   UINT32					Flags);
						   	   
HRESULT  MV_CC_CBufBody_Destroy(UINT32					SHMOffset);

#if defined( __LINUX_KERNEL__ )
HRESULT MV_CC_CBufSrv_Create( pMV_CC_DSS_ServiceInfo_CBuf_t pSrvInfo, MV_CC_Task *cc_task);
HRESULT MV_CC_CBufSrv_Destroy(pMV_CC_DSS_ServiceInfo_CBuf_t pSrvInfo, MV_CC_Task *cc_task);
#else
HRESULT MV_CC_CBufSrv_Create( pMV_CC_DSS_ServiceInfo_CBuf_t pSrvInfo);
HRESULT MV_CC_CBufSrv_Destroy(pMV_CC_DSS_ServiceInfo_CBuf_t pSrvInfo);
#endif
						   	   
#endif
