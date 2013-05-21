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
#ifndef __GALOIS_HAL_COM_H__
#define __GALOIS_HAL_COM_H__


#define INT_UNMASK_BIT		0
#define INT_MASK_BIT		1
#define INT_ENABLE_BIT		1
#define INT_DISABLE_BIT		0
#define INT_UNMASK_ALL		0x00000000
#define INT_MASK_ALL		0xFFFFFFFF
#define INT_ENABLE_ALL		0xFFFFFFFF	
#define INT_DISABLE_ALL		0x00000000

#define INT_ACTIVE_HIGH		1
#define INT_ACTIVE_LOW		0
#define INT_EDGE_TRIG		1
#define INT_LEVEL_TRIG		0

#define APB_ICTL_REG_L		0
#define APB_ICTL_REG_H		1

#endif // __GALOIS_HAL_COM_H__

