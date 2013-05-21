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
//! \file		Trace.h
//! \brief		Trace Back Buffer
//! \author		Junfeng Geng
//! \version	0.1
//! \date		June 2007
///////////////////////////////////////////////////////////////////////////////

#ifndef _TRACE_H_
#define _TRACE_H_

#ifdef WIN32
#define ENABLE_TRACE
#endif

#ifdef ENABLE_TRACE

#define TRACE_PUSH Trace_Push((CHAR*)__FILE__, (UINT)__LINE__, (CHAR*)__FUNCTION__, (HRESULT)(hr), (CHAR*)"")

HRESULT Trace_Push(CHAR* szFileName, UINT uiLineNumber, CHAR* szFunctionName, HRESULT hr, CHAR* szReserved);
HRESULT Trace_Dump();
HRESULT Trace_Clear();

#else //<! #ifdef ENABLE_TRACE

#define TRACE_PUSH
#define Trace_Push()
#define Trace_Dump()
#define Trace_Clear()

#endif //<! #ifdef ENABLE_TRACE

#endif //<! _TRACE_H_
