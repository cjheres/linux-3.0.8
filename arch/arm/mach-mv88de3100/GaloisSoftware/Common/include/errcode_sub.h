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
//! \file errcode_sub.h
//!
//! \brief Error code definition for generic subtitle decoder
//!
//! Author:	Jun Ma
//! Version:	1.0
//! Date:	January 2010
//!
///////////////////////////////////////////////////////////////////////////////


#ifndef	__ERRCODE_SUB_H__
#define	__ERRCODE_SUB_H__

#define E_SUB(code)								(E_ERR | E_SUB_BASE | ((code) & 0xFFFF))

/*
 * error code base of subtitle module
 */
#define	E_SUB_API(code)						E_SUB(0x0100 | ((code) & 0xFF))
#define	E_SUB_DEC(code)						E_SUB(0x0200 | ((code) & 0xFF))
#define	E_SUB_GFX(code)						E_SUB(0x0300 | ((code) & 0xFF))

/*
 * error code base of specific subtitle decoder
 */
#define	E_SUB_PLAINTEXT(code)					E_SUB(0x0800 | ((code) & 0xFF))
#define	E_SUB_XSUB(code)						E_SUB(0x0900 | ((code) & 0xFF))
#define	E_SUB_VOBSUB(code)					E_SUB(0x0A00 | ((code) & 0xFF))
#define	E_SUB_SUBRIP(code)						E_SUB(0x0B00 | ((code) & 0xFF))
#define	E_SUB_TEXT(code)						E_SUB(0x0C00 | ((code) & 0xFF))

/*
 * Sub decoder API error code
 */
#define	E_SUB_API_CREATE_FAILED				(E_SUB_API(0x01))
#define	E_SUB_API_INVALID_STATE				(E_SUB_API(0x02))
#define	E_SUB_API_INVALID_HANDLE				(E_SUB_API(0x03))
#define	E_SUB_API_INVALID_PARAMETER			(E_SUB_API(0x04))
#define	E_SUB_API_UNSUPPORTED_TYPE			(E_SUB_API(0x05))


/*
 * Sub decoder error code
 */
#define	E_SUB_DEC_CREATE_FAILED				(E_SUB_DEC(0x01))
#define	E_SUB_DEC_BUFFER_ERROR				(E_SUB_DEC(0x02))
#define	E_SUB_DEC_STREAM_ADDED				(E_SUB_DEC(0x03))
#define	E_SUB_DEC_INPUT_CREATE_FAILED		(E_SUB_DEC(0x04))
#define	E_SUB_DEC_INVALID_XBV_BUF			(E_SUB_DEC(0x05))
#define	E_SUB_DEC_OUT_OF_STREAM				(E_SUB_DEC(0x06))
#define	E_SUB_DEC_NO_ENOUGH_DATA			(E_SUB_DEC(0x07))
#define	E_SUB_DEC_RENDERER_CREATE_FAILED	(E_SUB_DEC(0x08))
#define	E_SUB_DEC_INVALID_HANDLE				(E_SUB_DEC(0x09))
#define	E_SUB_DEC_INVALID_STATE				(E_SUB_DEC(0x0A))
#define	E_SUB_DEC_INVALID_EVENT				(E_SUB_DEC(0x0B))
#define	E_SUB_DEC_INVALID_DATA				(E_SUB_DEC(0x0C))
#define	E_SUB_DEC_OUT_OF_MEMORY				(E_SUB_DEC(0x0D))
#define	E_SUB_DEC_UNSUPPORTED_TYPE			(E_SUB_DEC(0x0E))
#define	E_SUB_DEC_NOT_AVAILABLE				(E_SUB_DEC(0x0F))

/*
 * Sub GFX wrapper error code
 */
#define	E_SUB_GFX_PLANE_CREATE_FAILED		(E_SUB_GFX(0x01))
#define	E_SUB_GFX_PLANE_SET_SRC_FAILED		(E_SUB_GFX(0x02))
#define	E_SUB_GFX_OBJ_CREATE_FAILED			(E_SUB_GFX(0x03))
#define	E_SUB_GFX_DRAW_OBJ_FAILED			(E_SUB_GFX(0x04))
#define	E_SUB_GFX_NOT_READY					(E_SUB_GFX(0x05))

#define	E_SUB_PLAINTEXT_DEC_CREATE_FAILED	(E_SUB_PLAINTEXT(0x01))

#define	E_SUB_XSUB_DEC_CREATE_FAILED		(E_SUB_XSUB(0x01))

#define	E_SUB_SUBRIP_DEC_CREATE_FAILED		(E_SUB_SUBRIP(0x01))

#define	E_SUB_TEXT_DEC_CREATE_FAILED		(E_SUB_TEXT(0x01))


#endif	//!< #ifndef __ERRCODE_SUB_H__
