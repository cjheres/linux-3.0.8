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
//! \file errcode_gfx.h
//! \brief Header file for GFX error codes
///////////////////////////////////////////////////////////////////////////////

#ifndef	_ERRCODE_GFX_H_
#define	_ERRCODE_GFX_H_

#define E_GFX( code )       ( E_ERR | E_GFX_BASE | ( (code) & 0xFFFF ) )
#define E_GFX_SUC( code )   ( E_SUC | E_GFX_BASE | ( (code) & 0xFFFF ) )

// Success
#define E_GFX_OK                S_OK

// Success with condition
#define E_GFX_NO_MORE_DATA      E_GFX_SUC(1)
#define E_GFX_CACHED            E_GFX_SUC(2)

// Errors
#define E_GFX_INVALID_ARGUMENT  E_INVALIDARG
#define E_GFX_NOT_SUPPORTED     E_NOTIMPL
#define E_GFX_OUT_OF_MEMORY     E_OUTOFMEMORY

#define E_GFX_INVALID_OBJECT    E_GFX(0)
#define E_GFX_MEMORY_LOCKED     E_GFX(1)
#define E_GFX_MEMORY_UNLOCKED   E_GFX(2)
#define E_GFX_HEAP_CORRUPTED    E_GFX(3)
#define E_GFX_GENERIC_IO        E_GFX(4)
#define E_GFX_INVALID_ADDRESS   E_GFX(5)
#define E_GFX_CONTEXT_LOSSED    E_GFX(6)
#define E_GFX_TOO_COMPLEX       E_GFX(7)
#define E_GFX_BUFFER_TOO_SMALL  E_GFX(8)
#define E_GFX_INTERFACE_ERROR   E_GFX(9)
#define E_GFX_MORE_DATA         E_GFX(10)
#define E_GFX_TIMEOUT           E_GFX(11)
#define E_GFX_OUT_OF_RESOURCES  E_GFX(12)
#define E_GFX_INVALID_DEST_TYPE E_GFX(13)
#define E_GFX_INVALID_SRC_TYPE  E_GFX(14)
#define E_GFX_INVALID_CEL_TYPE  E_GFX(15)
#define E_GFX_INVALID_PARAM     E_GFX(16)
#define E_GFX_INVALID_LEN       E_GFX(17)
#define E_GFX_INVALID_RECT      E_GFX(18)
#define E_GFX_INVALID_CEL       E_GFX(19)
#define E_GFX_INVALID_SRC       E_GFX(20)
#define E_GFX_INVALID_DEST      E_GFX(21)
#define E_GFX_INVALID_PEN       E_GFX(22)
#define E_GFX_INVALID_BRUSH     E_GFX(23)
#define E_GFX_INVALID_PALETTE   E_GFX(24)
#define E_GFX_INVALID_FONT      E_GFX(25)
#define E_GFX_INVALID_SCREEN    E_GFX(26)

#define E_MV_PE_GFX_MORE_DATA         E_GFX_MORE_DATA
#define E_MV_PE_GFX_INVALID_PARAM     E_GFX_INVALID_PARAM
#define E_MV_PE_GFX_INVALID_LEN       E_GFX_INVALID_LEN
#define E_MV_PE_GFX_INVALID_RECT      E_GFX_INVALID_RECT
#define E_MV_PE_GFX_INVALID_CEL       E_GFX_INVALID_CEL
#define E_MV_PE_GFX_INVALID_SRC       E_GFX_INVALID_SRC
#define E_MV_PE_GFX_INVALID_DST       E_GFX_INVALID_DEST
#define E_MV_PE_GFX_INVALID_PEN       E_GFX_INVALID_PEN
#define E_MV_PE_GFX_INVALID_BRUSH     E_GFX_INVALID_BRUSH
#define E_MV_PE_GFX_INVALID_PALETTE   E_GFX_INVALID_PALETTE 
#define E_MV_PE_GFX_INVALID_PIX_TYPE  E_GFX_INVALID_CEL_TYPE
#define E_MV_PE_GFX_INVALID_FONT      E_GFX_INVALID_FONT

#endif //ndef _ERRCODE_GFX_H_
