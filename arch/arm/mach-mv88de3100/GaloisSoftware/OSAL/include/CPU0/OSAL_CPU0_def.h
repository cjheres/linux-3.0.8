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
//! \file OSAL_CPU0_def.h
//! \brief OSAL module common constant definition and data structure type definition
//!			on CPU0
//!
//!
//! Purpose:
//!	   Shared the all code of the OSAL module on CPU0
//!
//!	Version, Date and Author :
//!		V 1.00,	3/19/2007,	Fang Bao
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _OSAL_CPU0_DEF_H_
#define _OSAL_CPU0_DEF_H_

#include "OSAL_type.h"
#include "OSAL_debug.h"

#ifdef __cplusplus
extern "C"
{
#endif	// #ifdef __cplusplus


////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL CPU0 constant definition
//!
////////////////////////////////////////////////////////////////////////////////

#define MV_OSAL_CPU_LOCAL			(MV_OSAL_CPU_0)

#define MV_OSAL_GetLocalCPU()			(MV_OSAL_CPU_LOCAL)


#ifdef __cplusplus
}		// extern "C"
#endif	// #ifdef __cplusplus

#endif
