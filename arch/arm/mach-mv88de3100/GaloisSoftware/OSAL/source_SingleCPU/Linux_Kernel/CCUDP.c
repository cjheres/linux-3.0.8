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
//! \file CCUDP.c
//! \brief Common Communication Module Middle Service - User Datagram Protocol API and data structure
//!			type definition
//!
//! Purpose:
//!	   		Common Communication Module Middle Service - User Datagram Protocol
//!
//!	Version, Date and Author :
//!		V 1.00,	3/24/2007,	Fang Bao
//!		V 1.01,	4/19/2007,	Fang Bao,	Bugfix: m_Mutex of the MV_CC_UDPPortItem_t in the UDP table was handled in
//!										competition, so it should be all created in MV_CC_UDP_Constructor.
//!		V 1.02,	9/07/2007,	Fang Bao,	Add: 	MV_CC_UDPPort_GetStatus
//!		V 1.10,	5/08/2007,	Fang Bao
//!				Major change in UDP port table management
//!				In order to improve the efficiency of OS resource usage, the mutex of every UDP port will not be created in CCUDPPortTable_Init()
//!				It will be created when its port is used first time.
//!				To avoid competition on port creating, a new mutex (m_hPortTableMutex) of port table is designed.
//!				It is a important change in CC module! So the testing should be careful!
//!		V 1.11,	10/23/2009,	Fang Bao	Add CCDSS, CCDSP support (Multi-process)
//!		V 1.12,	11/25/2009,	Fang Bao	Add MV_CC_UDP_SendTo retry when CC FIFO is full.
//!										The try times is defined by MV_CC_UDP_SEND_TRYTIMES;
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#include "com_type.h"
#include "CCUDP.h"

// ???? socket could be used to send msg by two thread ????
pMV_CC_UDPPort_t 		pUDP_DEFAULT_PORT = NULL;

static void MV_CC_UDP_RecvMsg_Handle(struct sk_buff* skb);

HRESULT MV_CC_UDP_Init(void)
{
    if (pUDP_DEFAULT_PORT != NULL)
        return E_NOTREADY;

    pUDP_DEFAULT_PORT = MV_CC_UDP_Open(MV_CC_ServiceID_None);
	if (pUDP_DEFAULT_PORT == NULL)
	    MV_CC_DBG_Error(E_FAIL, "MV_CC_UDP_Init MV_CC_UDP_Open", NULL);

    return S_OK;
}

HRESULT MV_CC_UDP_Exit(void)
{
    HRESULT res;

    if (pUDP_DEFAULT_PORT == NULL)
        return E_NOTREADY;

    res = MV_CC_UDP_Close(pUDP_DEFAULT_PORT);
	if (res != S_OK)
	    MV_CC_DBG_Error(res, "MV_CC_UDP_Exit MV_CC_UDP_Close", NULL);

	pUDP_DEFAULT_PORT = NULL;

    return S_OK;
}

pMV_CC_UDPPort_t MV_CC_UDP_Open(MV_CC_ServiceID_U32_t    ServiceID)
{
    pMV_CC_UDPPort_t pPort;

    pPort = MV_OSAL_Malloc(sizeof(MV_CC_UDPPort_t));
	if (pPort == NULL)
	{
	    MV_CC_DBG_Warning(E_FAIL, "MV_CC_UDP_Open MV_OSAL_Malloc", NULL);
	    return NULL;
	}

    pPort->m_Socket = netlink_kernel_create(&init_net, NETLINK_GALOIS_CC_SINGLECPU, 0, MV_CC_UDP_RecvMsg_Handle, NULL, THIS_MODULE);
	if (pPort->m_Socket == NULL)
	{
		MV_CC_DBG_Warning(E_FAIL, "MV_CC_UDP_Open socket failed", NULL);
		goto error_exit_1;
	}

    netlink_set_nonroot(NETLINK_GALOIS_CC_SINGLECPU, NL_NONROOT_RECV | NL_NONROOT_SEND);

	MV_CC_DBG_Info("MV_CC_UDP_Open Socket = 0x%08X, SID = 0x%08X\n", (unsigned int)pPort->m_Socket, 0);

    return pPort;

error_exit_1:

    MV_OSAL_Free(pPort);

    return NULL;
}

HRESULT MV_CC_UDP_Close(pMV_CC_UDPPort_t 		pPort)
{
	/* Parameter Check */
	if (pPort == NULL)
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_UDP_Close", NULL);

	MV_CC_DBG_Info("MV_CC_UDP_Close Socket = 0x%08X\n", (unsigned int)pPort->m_Socket);

	sock_release(pPort->m_Socket->sk_socket);

	MV_OSAL_Free(pPort);

    return S_OK;
}

HRESULT MV_CC_UDP_SendMsg(pMV_CC_UDPPort_t 		pPort,
				   	   	  MV_CC_ServiceID_U32_t DstServiceID,
				   		  UCHAR 				*pMsgBuf,
				   		  UINT 				    MsgLen)
{
    struct sk_buff * skb;
    struct nlmsghdr *nlhdr;

	/* Parameter Check */
	if ((pMsgBuf == NULL) || (MsgLen > MV_CC_ICCFIFO_FRAME_SIZE))
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_UDP_SendMsg", NULL);

    if (pPort == NULL)
        pPort = pUDP_DEFAULT_PORT;

    skb = nlmsg_new(MsgLen, GFP_ATOMIC);
    if (skb == NULL)
    {
        MV_CC_DBG_Error(E_OUTOFMEMORY, "MV_CC_UDP_SendMsg nlmsg_new", NULL);
		return E_OUTOFMEMORY;
    }

	NETLINK_CB(skb).pid = 0;
	NETLINK_CB(skb).dst_group = NETLINK_GALOIS_CC_GROUP_SINGLECPU;

    nlhdr = NLMSG_PUT(skb, 0, 0, NLMSG_DONE, MsgLen);
	GaloisMemcpy(NLMSG_DATA(nlhdr), pMsgBuf, MsgLen);

    netlink_unicast(pPort->m_Socket, skb, DstServiceID, MSG_DONTWAIT);

    MV_CC_DBG_Info("MV_CC_UDP_SendMsg (SID = 0x%08X, size = %d )\n", DstServiceID, MsgLen);

    return S_OK;

nlmsg_failure:

    MV_CC_DBG_Info("MV_CC_UDP_SendMsg (SID = 0x%08X, size = %d ) nlmsg_failure !!!\n", DstServiceID, MsgLen);

    kfree_skb(skb);

    return E_FAIL;
}

static void MV_CC_UDP_RecvMsg_Handle(struct sk_buff* skb)
{
//    wake_up_interruptible(sk->sk_sleep);
	return;
}

