#ifndef __ASM_ARM_ARCH_MMC_PRIV_H
#define __ASM_ARM_ARCH_MMC_PRIV_H

#define MV_FLAG_ENABLE_CLOCK_GATING (1<<0)

#define MV_FLAG_CARD_PERMANENT (1 << 1)

#define MV_FLAG_SD_8_BIT_CAPABLE_SLOT (1 << 2)

#define MV_FLAG_MMC_CARD	(1<<3)

struct sdhci_mv_mmc_platdata {
	unsigned int 	flags;
	unsigned int 	clk_delay_cycles;
	unsigned int 	clk_delay_sel;
	bool 		clk_delay_enable;
	unsigned int	ext_cd_gpio;
	bool 		ext_cd_gpio_invert;
	unsigned int	max_speed;
	unsigned int	host_caps;
	unsigned int	quirks;
	unsigned int	pm_caps;
};

struct sdhci_mv_mmc {
	u8 clk_enable;
	u8 power_mode;
};
#endif
