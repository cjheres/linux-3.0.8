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
//! \file errcode_vip.h
//! \brief Header file for VIP error codes
///////////////////////////////////////////////////////////////////////////////

#ifndef	_ERRCODE_VIP_H_
#define	_ERRCODE_VIP_H_

#define S_VIP( code ) ( E_SUC | E_VIP_BASE | ( (code) & 0xFFFF ) )
#define E_VIP( code ) ( E_ERR | E_VIP_BASE | ( (code) & 0xFFFF ) )

#define S_VIP_OK 						(S_OK)

/* error code */
#define VIP_E_NODEV         E_VIP(1)
#define VIP_E_BADPARAM      E_VIP(2)
#define VIP_E_BADCALL       E_VIP(3)
#define VIP_E_UNSUPPORT     E_VIP(4)
#define VIP_E_IOFAIL        E_VIP(5)
#define VIP_E_UNCONFIG      E_VIP(6)
#define VIP_E_CMDQFULL      E_VIP(7)
#define VIP_E_FRAMEQFULL    E_VIP(8)
#define VIP_E_BCMBUFFULL    E_VIP(9)
#define VIP_E_NOMEM         E_VIP(10)
#define VIP_EVBIBUFFULL  E_VIP(11)
#define VIP_EHARDWAREBUSY  E_VIP(12)
#endif //!< #ifndef _ERRCODE_VIP_H_
