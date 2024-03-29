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
#ifndef __APB_UART_H__
#define __APB_UART_H__
#define APB_UART_RBR		0x00 
#define APB_UART_THR		0x00
#define APB_UART_DLL		0x00
#define APB_UART_DLH		0x04 
#define APB_UART_IER		0x04
#define APB_UART_IIR		0x08 
#define APB_UART_FCR		0x08 
#define APB_UART_LCR		0x0C 
#define APB_UART_MCR		0x10 
#define APB_UART_LSR		0x14 
#define APB_UART_MSR		0x18 
#define APB_UART_SCR		0x1C 
#define APB_UART_LPDLL		0x20 
#define APB_UART_LPDLH		0x24 
#define APB_UART_SRBR		0x30 //	0x30-0X6C
#define APB_UART_STHR		0x30 //	0x30-0X6C
#define APB_UART_FAR		0x70 
#define APB_UART_TFR		0x74 
#define APB_UART_RFW		0x78 
#define APB_UART_USR		0x7C 
#define APB_UART_TFL		0x80
#define APB_UART_RFL		0x84
#define APB_UART_SRR		0x88
#define APB_UART_SRTS		0x8C 
#define APB_UART_SBCR		0x90 
#define APB_UART_SDMAM		0x94
#define APB_UART_SFE		0x98 
#define APB_UART_SRT		0x9C 
#define APB_UART_STET		0xA0
#define APB_UART_HTX		0xA4 
#define APB_UART_DMASA		0xA8
// RESERVED			0xAC- 0XF0
#define APB_UART_CPR		0xF4
#define APB_UART_UCV		0xF8
#define APB_UART_CTR		0xFC
#endif //__APB_ICTL_H__

