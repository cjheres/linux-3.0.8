#ifndef __GALOIS_PLATFORM_H
#define __GALOIS_PLATFORM_H


#ifdef CONFIG_MV88DE3100_ASIC
  #define GALOIS_CPU_CLK 600000000
  #define GALOIS_TIMER_CLK 75000000
  #define GALOIS_UART_TCLK 25000000
#else	/* FPGA platform */
  #define GALOIS_CPU_CLK 36000000
  #define GALOIS_TIMER_CLK 4500000 /* ??? */
  #define GALOIS_UART_TCLK 36000000
#endif /* ASIC */

#define GALOIS_UART_CHANNEL 0
#ifdef CONFIG_MV88DE3100_ASIC
  #define GALOIS_UART_BAUDRATE 115200
#else
  #define GALOIS_UART_BAUDRATE 14400
#endif


#define _MEMMAP_CPU_PMR_REG_BASE	0xF7AD0000
#define _MEMMAP_SM_REG_BASE		0xF7F80000
#define MEMMAP_GIC_CPU_BASE		(_MEMMAP_CPU_PMR_REG_BASE+0x100)
#define MEMMAP_GIC_DIST_BASE		(_MEMMAP_CPU_PMR_REG_BASE+0x1000)

#endif