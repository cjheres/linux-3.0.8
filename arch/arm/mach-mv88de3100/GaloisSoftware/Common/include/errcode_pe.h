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
//! \file errcode_pe.h
//! \brief Header file for pe error codes
///////////////////////////////////////////////////////////////////////////////

#ifndef	_ERRCODE_PE_H_
#define	_ERRCODE_PE_H_

#define E_PE( code ) ( E_ERR | E_PE_BASE | ( (code) & 0xFFFF ) )

#define S_PE_SUCCESS 						(S_OK)

#ifndef ENABLE_DEBUG
#define	BYPASS_NONIMPLEMENT
#endif

#define E_PE_FAIL                          (E_FAIL)
#define E_PE_OUTOFMEMORY                   (E_OUTOFMEMORY)
#define E_PE_INVALIDARG                    (E_INVALIDARG)
#define E_PE_ACCESSDENIED                  (E_ACCESSDENIED)
#define E_PE_SERVER_NOTREADY               (E_NOTREADY) /* pe server not ready */
#define E_PE_NOTREADY                      (E_NOTREADY)
#define E_PE_RPC_SUCCESS                   (E_PE(0x00)) /* RPC success */
#define E_PE_RPC_FAILED                    (E_PE(0x01)) /* RPC failure (general)*/
#define E_PE_RPC_ERRCMD                    (E_PE(0x02)) /* RPC error  - command id*/
#define E_PE_RPC_ERRARG                    (E_PE(0x03)) /* RPC error  - parameter */
#define E_PE_RPC_FUNCNA                    (E_PE(0x04)) /* RPC error  - rpc server function on dst address N/A*/
#define E_PE_RPC_BUSY                      (E_PE(0x05)) /* RPC error  - rpc server busy */
#define E_PE_OUTOFRES                      (E_PE(0x06)) /*out of resources*/
#define E_PE_FUN_NOTREGSTERED              (E_PE(0x07)) /*function not registered when caller do unregister*/
#define E_PE_INVALIDEVENT                  (E_PE(0x08)) /*a invalid event code*/

#ifdef BYPASS_NONIMPLEMENT
#define E_PE_NOTIMPL                       (S_OK)
#else
#define E_PE_NOTIMPL                       (E_NOTIMPL)
#endif

#endif //!< #ifndef _ERRCODE_PE_H_

