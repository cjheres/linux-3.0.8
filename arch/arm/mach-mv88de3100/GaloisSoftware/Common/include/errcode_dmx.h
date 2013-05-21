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
//! \file errcode_dmx.h
//! \brief Header file for DMX error codes
///////////////////////////////////////////////////////////////////////////////

#ifndef	_ERRCODE_DMX_H_
#define	_ERRCODE_DMX_H_

#define S_DMX( code ) ( E_SUC | E_DEMUX_BASE | ( (code) & 0xFFFF ) )
#define E_DMX( code ) ( E_ERR | E_DEMUX_BASE | ( (code) & 0xFFFF ) )

#define S_DMX_OK 						(S_OK)

/* error code */
#define	DMX_E_NULL_PTR				E_DMX(1)
#define	DMX_E_INVALID_ARG			E_DMX(2)
#define	DMX_E_FAIL					E_DMX(3)
#define	DMX_OUT_OF_RESOURCE			E_DMX(4)
#define DMX_E_ABORT					E_DMX(5)

#define	DMX_NOT_IMPLEMENTED			E_DMX(6)

#define	DMX_S_INPUT_UNDERRUN	S_DMX(4)
#define	DMX_S_OUTPUT_OVERRUN	S_DMX(5)
#define	DMX_S_DMX_STOPPED		S_DMX(6)


#endif //!< #ifndef _ERRCODE_DMX_H_
