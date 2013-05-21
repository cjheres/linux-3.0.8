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
#ifndef __APB_PERF_BASE_H__
#define __APB_PERF_BASE_H__
#include "Galois_memmap.h"
#if !defined(BERLIN)
#define APB_I2C_INST0_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x0400)
#define APB_ICTL_INST0_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x0800)
#define APB_GPIO_INST0_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x0C00)
#define APB_UART_INST0_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x1000)
#define APB_WDT_INST0_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x1400)
#define APB_SSI_INST1_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x1800)
#define APB_UART_INST1_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x1C00)
#define APB_SSI_INST0_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x2000)
#define APB_WDT_INST1_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x2400)
#define APB_TIMER_INST_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x2800)
#define APB_WDT_INST2_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x2C00)
#define APB_I2C_INST1_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x3000)
#define APB_I2C_INST2_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x3400)
#define APB_ICTL_INST1_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x3800)
#define APB_ICTL_INST2_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x3C00)
#define APB_SSI_INST2_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x4000)

#define APB_I2C_INST3_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x8400)
#define APB_GPIO_INST1_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x8800)
#define APB_GPIO_INST2_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x8C00)
#define APB_GPIO_INST3_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x9000)
#else  // BERLIN (Email from Tzun-wei, 12/26/07)
#define APB_GPIO_INST0_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x0400) // gpioInst0   = 0x0400
#define APB_GPIO_INST1_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x0800) // gpioInst1   = 0x0800
#define APB_GPIO_INST2_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x0C00) // gpioInst2   = 0x0C00
#define APB_GPIO_INST3_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x1000) // gpioInst3   = 0x1000
#define APB_I2C_INST0_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x1400) // i2cInst0    = 0x1400
#define APB_I2C_INST1_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x1800) // i2cInst1    = 0x1800
#define APB_SSI_INST1_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x1C00) // ssiInst1    = 0x1C00
#define APB_WDT_INST0_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x2000) // wdtInst0    = 0x2000
#define APB_WDT_INST1_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x2400) // wdtInst1    = 0x2400
#define APB_WDT_INST2_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x2800) // wdtInst2    = 0x2800
#define APB_TIMER_INST_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x2C00) // timerInst   = 0x2C00
#define APB_ICTL_INST0_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x3000) // ictlInst0   = 0x3000
#define APB_ICTL_INST1_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x3400) // ictlInst1   = 0x3400
#define APB_ICTL_INST2_BASE		(MEMMAP_APBPERIF_REG_BASE + 0x3800) // ictlInst2   = 0x3800

// redirect these function to SM.
#define APB_UART_INST0_BASE		(SM_APB_UART0_BASE)
#define APB_UART_INST1_BASE		(SM_APB_UART1_BASE)
#define APB_UART_INST2_BASE		(SM_APB_UART2_BASE)
#define APB_SSI_INST2_BASE		(SM_APB_SPI_BASE)
#define APB_I2C_INST2_BASE		(SM_APB_I2C0_BASE)
#define APB_I2C_INST3_BASE		(SM_APB_I2C1_BASE)

// SM base
#define SOC_SM_ITCM_BASE		(MEMMAP_SM_REG_BASE)
#define SOC_SM_DTCM_BASE		(MEMMAP_SM_REG_BASE + 0x20000)
#define SOC_SM_APB_REG_BASE		(MEMMAP_SM_REG_BASE + 0x40000)

#define SM_APB_ICTL_BASE		(SOC_SM_APB_REG_BASE)

#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
#define SM_APB_ICTL1_BASE               (SOC_SM_APB_REG_BASE + 0xE000)
#endif

#define SM_APB_WDT0_BASE		(SOC_SM_APB_REG_BASE + 0x1000)
#define SM_APB_WDT1_BASE		(SOC_SM_APB_REG_BASE + 0x2000)
#define SM_APB_WDT2_BASE		(SOC_SM_APB_REG_BASE + 0x3000)
#define SM_APB_TIMERS_BASE		(SOC_SM_APB_REG_BASE + 0x4000)

#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
#define SM_APB_GPIO1_BASE		(SOC_SM_APB_REG_BASE + 0x5000)
#else
#define SM_APB_COUNTER_BASE		(SM_APB_REG_BASE + 0x5000)
#endif

#define SM_APB_SPI_BASE			(SOC_SM_APB_REG_BASE + 0x6000)
#define SM_APB_I2C0_BASE		(SOC_SM_APB_REG_BASE + 0x7000)
#define SM_APB_I2C1_BASE		(SOC_SM_APB_REG_BASE + 0x8000)
#define SM_APB_UART0_BASE		(SOC_SM_APB_REG_BASE + 0x9000)
#define SM_APB_UART1_BASE		(SOC_SM_APB_REG_BASE + 0xA000)
#define SM_APB_UART2_BASE		(SOC_SM_APB_REG_BASE + 0xB000)

#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
#define SM_APB_GPIO0_BASE		(SOC_SM_APB_REG_BASE + 0xC000)
#define SM_APB_GPIO_BASE		(SM_APB_GPIO0_BASE)
#else
#define SM_APB_GPIO_BASE		(SOC_SM_APB_REG_BASE + 0xC000)
#endif

#define SM_SYS_CTRL_REG_BASE	(SOC_SM_APB_REG_BASE + 0xD000)
#define SM_PIN_MUX_REG			(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL)

#endif
#endif // __APB_PERF_BASE_H__

