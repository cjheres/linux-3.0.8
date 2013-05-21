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

///////////////////////////////////////////////////////////////////////////////
//! \file		BoundChecker.h
//! \brief 
//! \author		Cheng Du
//! \version	0.1
//! \date		August 2007
///////////////////////////////////////////////////////////////////////////////

#ifndef _BOUNDCHECKER_H_
#define _BOUNDCHECKER_H_

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////
//	Memory Pool For small memory allocations. 
//  By Wanyong, 20090605.
/////////////////////////////////////////////////////////////
#if defined ( __ECOS__ ) && defined ( SMALL_MEM_POOL_SUPPORT )
HRESULT CommonMemPoolCreate(void);
void CommonMemPoolDestroy();                                                                 
void *CommonMemPoolMalloc(UINT size);
void CommonMemPoolFree(void *pMemAddr);
#endif

#ifdef ENABLE_BOUNDCHECKER

typedef struct _MV_MON_LogType_Memory {
	
	UINT32	m_cmd;
	UINT32	m_data1;
	UINT32	m_data2;
	UINT32	m_data3;
	UINT32	m_data4;
	char	m_filename[50];
	UINT32	m_data5[20];
} MV_MON_LogType_Memory_t, *pMV_MON_LogType_Memory_t;

HRESULT BoundChecker_Create();
HRESULT BoundChecker_Create2(UINT uiPoolSize);
HRESULT BoundChecker_Destroy();
HRESULT BoundChecker_Report();
HRESULT BoundChecker_ReportDelta();
PVOID BoundChecker_Malloc(UINT uiLength, UINT uiAlign, CHAR* szFileName, UINT uiLineNumber);
PVOID BoundChecker_Calloc(UINT numElements, UINT sizeOfElement, CHAR* szFileName, UINT uiLineNumber);
HRESULT BoundChecker_Free(PVOID pData, BOOL bAligned);
HRESULT  TEST_StartMemCheck(UINT32 *uiBytesUsed);
INT  TEST_CheckMemLeak(UINT32 *uiBytesUsed);


#else
#define BoundChecker_Create()		S_OK
#define BoundChecker_Destroy()		S_OK
#define BoundChecker_Report()		S_OK
#define BoundChecker_Malloc()		S_OK
#define BoundChecker_Calloc()		S_OK
#define BoundChecker_Free()		S_OK
#define BoundChecker_ReportDelta()		S_OK
#define TEST_StartMemCheck(...)		S_OK
#define TEST_CheckMemLeak(...)		S_OK
#endif //<! ENABLE_BOUNDCHECKER

#ifdef __cplusplus
}
#endif

#endif //<! _BOUNDCHECKER_H_
