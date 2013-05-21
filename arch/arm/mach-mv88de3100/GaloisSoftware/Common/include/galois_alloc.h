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
//! \file galois_alloc.h
//! \functions for memmory alloc have align address request
//!	
//! Purpose:
//!	Defines the allign alloc
//!
//! Note:
//!	Version, Date and Author : Yongchun
////////////////////////////////////////////////////////////////////////////////

#ifndef __GALOIS_ALLOC_H__
#define __GALOIS_ALLOC_H__

void *Galois_align_malloc(UINT32 size, UINT32 align);
void Galois_align_free(void * buf);

#endif
