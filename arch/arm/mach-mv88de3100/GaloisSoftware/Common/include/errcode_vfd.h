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
//! \file errcode_vfd.h
//! \brief Header file for vfd error codes
///////////////////////////////////////////////////////////////////////////////

#ifndef	_ERRCODE_VFD_H_
#define	_ERRCODE_VFD_H_

#define E_VFD( code ) ( E_ERR | E_VFD_BASE | ( (code) & 0xFFFF ) )

#define S_VFD_SUCCESS 						(S_OK)


/* VFD error code, 0x0000~0x00FF */
#define E_VFD_START                     (0x0000)

#define E_VFD_ENODEV                    (E_VFD(E_VFD_START|0x01)) /* No device found*/
#define E_VFD_EBADPARAM                 (E_VFD(E_VFD_START|0x02)) /* Bad parameter*/
#define E_VFD_EUNSUPPORT                (E_VFD(E_VFD_START|0x03)) /* Not support device */
#define E_VFD_EIOFAIL                   (E_VFD(E_VFD_START|0x04)) /* I/O Failed*/




#endif //!< #ifndef _ERRCODE_VFD_H_

