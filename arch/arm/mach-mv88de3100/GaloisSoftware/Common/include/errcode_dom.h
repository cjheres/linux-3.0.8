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
//! \file errcode_dom.h
//! \brief Header file for DOM error codes
///////////////////////////////////////////////////////////////////////////////

#ifndef	_ERRCODE_DOM_H_
#define	_ERRCODE_DOM_H_

#define E_DOM( code ) (((code)==0)? S_OK : (E_ERR | E_DOM_BASE | ( (code) & 0xFFFF )) )

#define MV_DOM_INDEX_SIZE_ERR E_DOM(1)
#define MV_DOM_DOMSTRING_SIZE_ERR E_DOM(2)
#define MV_DOM_HIERARCHY_REQUEST_ERR E_DOM(3)
#define MV_DOM_WRONG_DOCUMENT_ERR E_DOM(4)
#define MV_DOM_INVALID_CHARACTER_ERR E_DOM(5)
#define MV_DOM_NO_DATA_ALLOWED_ERR E_DOM(6)
#define MV_DOM_NO_MODIFICATION_ALLOWED_ERR E_DOM(7)
#define MV_DOM_NOT_FOUND_ERR E_DOM(8)
#define MV_DOM_NOT_SUPPORTED_ERR E_DOM(9)
#define MV_DOM_INUSE_ATTRIBUTE_ERR E_DOM(10)
#define MV_DOM_INVALID_STATE_ERR E_DOM(11)
#define MV_DOM_SYNTAX_ERR E_DOM(12)
#define MV_DOM_INVALID_MODIFICATION_ERR E_DOM(13)
#define MV_DOM_NAMESPACE_ERR E_DOM(14)
#define MV_DOM_INVALID_ACCESS_ERR E_DOM(15)
#define MV_DOM_NULL_POINTER_ERR E_DOM(100)

#endif //!< #ifndef _ERRCODE_DOM_H_
