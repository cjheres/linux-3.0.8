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
//! \file galois_cache_flush.h
//! \functions to flush D$
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __GALOIS_CACHE_FLUSH_H__
#define __GALOIS_CACHE_FLUSH_H__
////////////////////////////////////////////////////////////////////////////////
//! Function:    CleanEntierDCache
//!
//! Description: Clean Entier DCache without invalidate the data 
//!
//! Inputs:     None 
//!
//! Outputs:     none
//!
//! Return:      none
////////////////////////////////////////////////////////////////////////////////
void CleanEntireDCache(void);

////////////////////////////////////////////////////////////////////////////////
//! Function:    CleanAndInvalidateEntierDCache
//!
//! Description: Clean Entier DCache and invalidate the data 
//!
//! Inputs:     None 
//!
//! Outputs:     none
//!
//! Return:      none
////////////////////////////////////////////////////////////////////////////////
void CleanAndInvalidateEntireDCache(void);

////////////////////////////////////////////////////////////////////////////////
//! Function:    InvalidateDCacheRegion
//!
//! Description: Invalidate DCache Region data. The operation need to make sure
//!              that the adress and size and cache line based. Use this one with
//!              cautions.(Typically, it is used on DMA write, CPU read only case)
//!
//! Inputs:     adr--- contain base adress
//!             size--- contains size of Region in bytes
//!
//! Outputs:     none
//!
//! Return:      none
////////////////////////////////////////////////////////////////////////////////
void InvalidateDCacheRegion(int *adr, int size);

////////////////////////////////////////////////////////////////////////////////
//! Function:    CleanDCacheRegion
//!
//! Description: Clean DCache Region data 
//!
//! Inputs:     adr--- contain base adress
//! 			size--- contains size of Region in bytes
//!
//! Outputs:     none
//!
//! Return:      none
////////////////////////////////////////////////////////////////////////////////
void CleanDCacheRegion(int *adr, int size);

////////////////////////////////////////////////////////////////////////////////
//! Function:    CleanAndInvalidateDCacheRegion
//!
//! Description: Clean  and invalidate DCache Region data 
//!
//! Inputs:     adr--- contain base adress
//! 			size--- contains size of Region in bytes
//!
//! Outputs:     none
//!
//! Return:      none
////////////////////////////////////////////////////////////////////////////////
void CleanAndInvalidateDCacheRegion(int *adr, int size);
#endif //__GALOIS_CACHE_FLUSH_H__
