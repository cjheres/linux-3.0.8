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
//! \file CCUDP.h
//! \brief Common Communication Module Middle Service - User Datagram Protocol API and data structure 
//!			type definition
//!	
//! Purpose:
//!	   		Common Communication Module Middle Service - User Datagram Protocol
//!
//!	Version, Date and Author :
//!		V 1.00,	3/24/2007,	Fang Bao
//!		V 2.00,	5/31/2010,	Fang Bao	
//!						
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _CCUDP_H_
#define _CCUDP_H_

#include <linux/netlink.h>
#include <net/sock.h>

#include "OSAL_api.h"

////////////////////////////////////////////////////////////////////////////////
//! \brief Data Structure: 
//!
////////////////////////////////////////////////////////////////////////////////

typedef struct _MV_CC_UDPPort {
		
	struct sock * m_Socket;

} MV_CC_UDPPort_t, *pMV_CC_UDPPort_t;

HRESULT MV_CC_UDP_Init(void);

HRESULT MV_CC_UDP_Exit(void);

pMV_CC_UDPPort_t MV_CC_UDP_Open(MV_CC_ServiceID_U32_t    ServiceID);

HRESULT MV_CC_UDP_Close(pMV_CC_UDPPort_t 		pPort);

HRESULT MV_CC_UDP_SendMsg(pMV_CC_UDPPort_t 		pPort, 
				   	   	  MV_CC_ServiceID_U32_t DstServiceID,
				   		  UCHAR 				*pMsgBuf,
				   		  UINT 				    MsgLen);           
			   		  
#endif
