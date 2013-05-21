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
//! \file MV_CC_GSConfig.c
//! \brief This is the config file of the OSAL Global Service
//!	
//! Purpose:
//!
//!
//!	Version, Date and Author :
//!		V 1.00,	4/03/2007,	Fang Bao
//!		V 1.10,	4/03/2007,	Fang Bao
//!							Add the Dynamic Mode for Global Service, clean up the Test Service ID.
//!							Add the Dynamic Service Server - DSS service ID, MV_GS_SysRPC_DSS
//!		V 1.11,	6/24/2008,	Fang Bao
//!							clean up the list
//!							Add MV_GS_UISrv1 and MV_GS_UISrv2 for Junfeng Geng.
//!		V 1.12,	4/22/2009,	Fang Bao
//!							Add some services for OSAL Test Case
//!		V 1.13,	9/15/2009,	Fang Bao
//!							Clean some services not used, MV_DRM_AACS, MV_DRM_IMAGETOOL
//!		V 1.14, 10/23/2009,	Fang Bao
//!							Add CCDSS, CCDSP support
//!	
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef MV_CC_GSConfig_H
#define MV_CC_GSConfig_H

#include "com_type.h"
#include "OSAL_config.h"
#include "OSAL_type.h"

#ifdef __cplusplus
extern "C"
{
#endif	// #ifdef __cplusplus


////////////////////////////////////////////////////////////////////////////////
//! \brief Global Service ID (GS)Definition :
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_CC_ServiceID_None				(0x00000000)
#define MV_CC_ServiceID_StaticStart			(0x00000001)
#define MV_CC_GSConfigList_EndFlagID		(0x7FFFFFFF)
#define MV_CC_ServiceID_DynamicStart		(0x80000000)
#define MV_CC_ServiceID_DynamicApply		(0x80000000)


////////////////////////////////////////////////////////////////////////////////
//! \brief Global Service ID (GS)Definition : Modified by User on Static Mode
//! 
//! Static Mode - Service ID > MV_CC_ServiceID_StaticStart
//! Static Mode - Service ID < MV_CC_ServiceID_DynamicStart
//! Static Mode - Service ID <> MV_CC_GSConfigList_EndFlagID
////////////////////////////////////////////////////////////////////////////////

/*
** Service ID for Test Service
*/

#define MV_GS_AppMsgQ00						(0x00000001)
#define MV_GS_AppMsgQ01						(0x00000002)
#define MV_GS_AppMsgQ02						(0x00000003)

#define MV_GS_AppRPC01						(0x00000004)
#define MV_GS_AppRPC02						(0x00000005)
#define MV_GS_AppRPC03						(0x00000006)

#define MV_GS_AppCBUF01						(0x00000007)
#define MV_GS_AppCBUF02						(0x00000008)
#define MV_GS_AppCBUF03						(0x00000009)
#define MV_GS_AppCBUF04						(0x0000000a)

#define MV_GS_AppCBUF01_CPU1				(0x0000000b)
#define MV_GS_AppCBUF02_CPU1				(0x0000000c)
#define MV_GS_AppCBUF03_CPU1				(0x0000000d)
#define MV_GS_AppCBUF04_CPU1				(0x0000000e)

#define MV_GS_AppRPC01_CPU1					(0x0000000f)

#define MV_GS_TestMsgQEx0					(0x00000010)
#define MV_GS_TestMsgQEx1					(0x00000011)

#define MV_GS_CUnitMsgQInCPU0				(0x00000020)
#define MV_GS_CUnitMsgQInCPU1				(0x00000021)
#define MV_GS_CUnitRPCServer1				(0x00000022)
#define MV_GS_CUnitRPCServer2				(0x00000023)
#define MV_GS_CUnitRPCServer3				(0x00000024)
#define MV_GS_CUnitCBufServer1				(0x00000025)
#define MV_GS_CUnitCBufServer2				(0x00000026)
#define MV_GS_CUnitCBufServer3				(0x00000027)

/*
** Service ID for System Service
*/

#define MV_GS_SysSrvStatus_CPU0				(0x00000100)
#define MV_GS_SysSrvStatus_CPU1				(0x00000101)
#define MV_GS_SysRPC_SHM					(0x00000102)
#define MV_GS_SysRPC_DSS					(0x00000103)
#define MV_GS_EventMsgQ						(0x00000104)
#define MV_GS_MonitorCPU1					(0x00000105)
#define MV_GS_AppRPC_GFX					(0x00800001)


/*
**service id for PE
*/

#define MV_GS_AppSrv						(0x00010001)
#define MV_GS_NetSrv						(0x00010002)
#define MV_GS_InbSrv						(0x00020000)
#define MV_GS_SrcSrv						(0x00030001)
#define MV_GS_StmSrv						(0x00040001)
#define MV_GS_PspSrv						(0x00050001)
#define MV_GS_CbkInvoke						(0x00060001)
#define MV_GS_CbkReg						(0x00060002)
#define MV_GS_CbkCbufSrv					(0x00060003)
#define MV_GS_Gfx_CmdQ_0_1 			        (0x00060004)
#define MV_GS_Gfx_CmdQ_0_2 			        (0x00060005)
#define MV_GS_Gfx_CmdQ_0_3 			        (0x00060006)
#define MV_GS_Gfx_CmdQ_0_4 			        (0x00060007)


/*
**define service id for DRM module
*/
#ifdef BERLIN_SINGLE_CPU
#define MV_GS_DMX_Serv                      (0x00060008)
#define MV_GS_VPP_Serv 	                    (0x00060009)
#define MV_GS_AUD_Serv                      (0x0006000a)
#define MV_GS_ZSP_Serv                      (0x0006000b)
#define MV_GS_VDEC_Serv                     (0x0006000c)
#define MV_GS_RLE_Serv                      (0x0006000d)
#define MV_GS_VIP_Serv 	                    (0x0006000e)
#define MV_GS_AIP_Serv 	                    (0x0006000f)
#define MV_GS_VDM_Serv 	                    (0x00060010)
#define MV_GS_AGENT_Serv                    (0x00060011)

#endif

/*
**define service id for UI module
*/

#define MV_GS_UISrv1						(0x00200001)
#define MV_GS_UISrv2						(0x00200002)

//#define MV_AACS_Decode_Server


/*
 * define service id for OnlineUpgrade
 */
#define	MV_OU_MsgQ				(0x00300001)


////////////////////////////////////////////////////////////////////////////////
//! \brief Global Service (GS)Definition : MV_CC_GSConfigItem_t
//!
////////////////////////////////////////////////////////////////////////////////
typedef struct _MV_CC_GSConfigItem {

	char m_ServiceName[20];

	MV_CC_ServiceID_U32_t m_ServiceID;	/* when m_SrvLevel = IPC, m_ServiceID is just as id for IPC */

	MV_CC_SrvLevel_U8_t m_SrvLevel;
	MV_CC_SrvType_U8_t m_SrvType;
	
	MV_CC_ICCAddr_t m_SrvAddr;
					
	UINT32 Option;

} MV_CC_GSConfigItem_t;


////////////////////////////////////////////////////////////////////////////////
//! \brief Global Service (GS) Constant Definition : MV_CC_GSConfigList
//!
////////////////////////////////////////////////////////////////////////////////
extern const MV_CC_GSConfigItem_t MV_CC_GSConfigList[];


////////////////////////////////////////////////////////////////////////////////
//! \brief Global Service (GS) Macro Definition : MV_CC_GSConfigList_GetSrvAddr
//!
////////////////////////////////////////////////////////////////////////////////

#define MV_CC_GSConfigList_GetSrvAddr(x)	(MV_CC_ICCAddr_t *)(&(MV_CC_GSConfigList[x].m_SrvAddr))



////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    GSConfigList_Search
//!
//! Description: search the index of MV_CC_GSConfigList by ServiceID
//!
//! \param ServiceID 	(IN): -- the ID of the Service in the MV_CC_GSConfigList
//!
//! \return Return:		
//!						>=0: the index of the MV_CC_GSConfigList
//!						E_FAIL:	not found the service id in the MV_CC_GSConfigList
//! 
////////////////////////////////////////////////////////////////////////////////
INT GSConfigList_Search(MV_CC_ServiceID_U32_t ServiceID);


#ifdef __cplusplus
}		// extern "C"
#endif	// #ifdef __cplusplus

#endif
