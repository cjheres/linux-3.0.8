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
//! \file CC_DSS_Client.c
//! \brief CC Dynamic Service Server Client APIs
//!
//!	   Platform = Common
//!
//! Purpose:
//!
//!
//!	Version, Date and Author :
//!		V 1.00,	12/05/2007,	Fang Bao
//!		V 1.01, 12/24/2007, Fang Bao  Remove checking from MV_CC_DSS_CBufData_Fill()
//!		V 1.02, 01/18/2008, Fang Bao - Fix bug for enum type size (sizeof(CBufCtrlType)) in different OS
//!		V 1.03, 06/05/2008, Fang Bao - Fix bug in MV_CC_DSS_ServiceList_Delete(); when deleting node in list head or tail, the list maybe reset.
//!		V 1.04, 06/13/2008, Fang Bao - modify the code in MV_CC_DSS_ServiceList_Delete() for clearer flow.
//!		V 1.05, 07/02/2008, Fang Bao - Fix bug in MV_CC_CBufBody_Create(); pCBufBody->m_StartOffset is computed error when MV_CC_CBUF_FLAGS_ALIGNMENT is set
//!		V 1.06, 07/24/2008, Fang Bao - Fix bug in MV_CC_CBuf_RPCSrv_Destroy(); add MV_CC_DSS_CBufData_Destroy(pCBufData) to free the unused memory
//!		V 1.07,	10/14/2009,	Fang Bao,	modify MV_CC_CBuf_RPCSrvWrapper_Create() RPC parameter size for RPC parameter size check
//!		V 1.08,	10/23/2009,	Fang Bao,	remove Service ID check in MV_CC_CBuf_RPCSrv_Create()
//!
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#include "shm_api.h"
#include "OSAL_api.h"
#include "CC_CBuf_type.h"

HRESULT  MV_CC_CBufBody_Create(UINT32   				*pSHMOffset,
						   	   UINT32					BufSize,
						   	   UINT32					EndBlockSize,
						   	   UINT32					Flags)
{
	pMV_CC_CBufBody_t pCBufBody;
	UINT32 CBuf_FullSize, SHMOffset;
	HRESULT res;

#ifdef ENABLE_DEBUG_OSAL
	//Fill the end padding
	UINT32 buflen = BufSize;
#endif

//	MV_CC_DBG_Info("MV_CC_CBufBody_Create Flags =%08xh\n", Flags);

	/* Parameter Check */
	if (pSHMOffset == NULL)
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_CBufBody_Create", NULL);

	CBuf_FullSize = sizeof(MV_CC_CBufBody_t) + BufSize;

	/* Check Flag->MV_CC_CBUF_FLAGS_ALIGNMENT */
	if ((MV_CC_FlagGet(Flags, MV_CC_CBUF_FLAGS_ALIGNMENT)) == MV_CC_CBUF_FLAGS_ALIGNMENT_Yes)
		CBuf_FullSize += MV_CC_HAL_MEMBOUNDARY;

	/* If EndBlockSize == 0, no End Block for the MV_CC_CBufBody_t and no limitation to the Block Operation */
	if (EndBlockSize != 0)
		CBuf_FullSize += EndBlockSize;
//Cbuf use cacheable memory in kernel driver, then use related virt addr
	SHMOffset = MV_SHM_Malloc(CBuf_FullSize, MV_CC_HAL_MEMBOUNDARY);
	if (SHMOffset == ERROR_SHM_MALLOC_FAILED)
	{
		*pSHMOffset = ERROR_SHM_MALLOC_FAILED;
		res = E_OUTOFMEMORY;
		MV_CC_DBG_Error(res, "MV_CC_CBufBody_Create MV_SHM_Malloc", NULL);
		return res;
	}

	pCBufBody = (pMV_CC_CBufBody_t)MV_SHM_GetCacheVirtAddr(SHMOffset);
	if (pCBufBody == NULL) {
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_CBufBody_Create", NULL);
	}
	GaloisMemClear((void *)pCBufBody, CBuf_FullSize);

	/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Warning!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
	/*-----------------------------Warning------------------------------------*/
	/* Warning - pCBufBody pointer malloced by SHM is assumed at memory boundary */

	/* m_StartOffset is dependent of m_Flag->MV_CC_CBUF_FLAGS_ALIGNMENT */
	pCBufBody->m_StartOffset = sizeof(MV_CC_CBufBody_t);

	if ((MV_CC_FlagGet(Flags, MV_CC_CBUF_FLAGS_ALIGNMENT)) == MV_CC_CBUF_FLAGS_ALIGNMENT_Yes)
		pCBufBody->m_StartOffset += MV_CC_HAL_MEMBOUNDARY - (((UINT32)pCBufBody + pCBufBody->m_StartOffset) % MV_CC_HAL_MEMBOUNDARY);

	//Fill the padding with 0xFF for debug easily
#ifdef ENABLE_DEBUG_OSAL

	//Fill the front padding
	GaloisMemSet((void *)((UCHAR *)pCBufBody + sizeof(MV_CC_CBufBody_t)),
					0xFF, pCBufBody->m_StartOffset - sizeof(MV_CC_CBufBody_t));

	/* If EndBlockSize == 0, no End Block for the MV_CC_CBufBody_t and no limitation to the Block Operation */
	if ( EndBlockSize != 0 )
		buflen += EndBlockSize;

	GaloisMemSet((void *)((UCHAR *)pCBufBody + pCBufBody->m_StartOffset + buflen),
					0xFF, CBuf_FullSize - pCBufBody->m_StartOffset - buflen);
#endif

	pCBufBody->m_BufSize = BufSize;
	pCBufBody->m_EndBlockSize = EndBlockSize;
	pCBufBody->m_Flags = Flags;

	MV_CC_HAL_FlushMemory(pCBufBody->m_Flags);

	*pSHMOffset = SHMOffset;

	MV_CC_DBG_Info("DSS MV_CC_CBufBody_Create SHM Offset = [0x%08X]", SHMOffset);

	return S_OK;
}

HRESULT  MV_CC_CBufBody_Destroy(UINT32					SHMOffset)
{
	HRESULT res;

	MV_CC_DBG_Info("MV_CC_CBufBody_Destroy SHM Offset = [0x%08X]", SHMOffset);

	/* Parameter Check */
	if (SHMOffset == ERROR_SHM_MALLOC_FAILED)
		MV_CC_DBG_Error(E_INVALIDARG, "MV_CC_CBufBody_Destroy", NULL);

	res = MV_SHM_Free( SHMOffset );
	if (res != S_OK)
	    MV_CC_DBG_Error(res, "MV_CC_CBufBody_Destroy MV_SHM_Free", NULL);

	return res;
}
