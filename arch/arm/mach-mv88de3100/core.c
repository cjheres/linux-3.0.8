#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>

#include <asm/page.h>
#include <asm/memory.h>
#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/localtimer.h>
#include <asm/cacheflush.h>

#include <mach/galois_generic.h>
#include <mach/galois_platform.h>
#include <asm/mach/time.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/hardware/cache-tauros3.h>
#include <mach/apb_timer.h>
#include <mach/freq.h>

#include "common.h"

#include "Galois_memmap.h"
#include "apb_perf_base.h"
#include "SM_common.h"

static const char proc_banner[] __initconst =
	"CPU clock %ldHz, timer clock %ldHz\n"
	"Linux memory start: 0x%08x\n";

static void __init print_version(void)
{
	printk(proc_banner,
#ifdef CONFIG_MV88DE3100_DYNAMIC_FREQ
		socFreqVec.cpu0, socFreqVec.cfg,
#else
		(unsigned long)GALOIS_CPU_CLK, (unsigned long)GALOIS_TIMER_CLK,
#endif
		(unsigned int)(CONFIG_MV88DE3100_CPU0MEM_START));
}

/*
 * the virtual address mapped should be < PAGE_OFFSET or >= VMALLOC_END.
 * Generally, it should be >= VMALLOC_END.
 */
static struct map_desc mv88de3100_io_desc[] __initdata = {
	{/* map phys space 0xF6000000~0xF9000000(doesn't cover Flash area) to same virtual space*/
		.virtual = 0xF6000000,
		.pfn = __phys_to_pfn(0xF6000000),
		.length = 0x03000000,
		.type = MT_DEVICE
	},
	/* Don't map ROM,vectors space: 0xFF800000~0xFFFFFFFF */
};

/*
 * do very early than any initcalls, it's better in setup_arch()
 */
static void __init soc_initialize(void)
{
#ifdef CONFIG_LOCAL_TIMERS
	twd_base = __io(MEMMAP_CPU_PMR_REG_BASE + 0x600);
	BUG_ON(!twd_base);
#endif
	apbt_virt_address = __io(APB_TIMER_INST_BASE);
	apbt_start_counter(GALOIS_CLOCKSOURCE);
	query_board_freq(&socFreqVec);
}

static void __init mv88de3100_map_io(void)
{
	iotable_init(mv88de3100_io_desc, ARRAY_SIZE(mv88de3100_io_desc));
	soc_initialize(); /* some ugly to put here, but it works:-) */
}

static void galois_power_off(void)
{
#ifdef CONFIG_MV88DE3100_SM
	int msg = MV_SM_POWER_WARMDOWN_REQUEST;
	bsm_msg_send(MV_SM_ID_POWER, &msg, sizeof(msg));
	for (;;);
#endif
}

/*
 * mv88de3100 SoC initialization, in arch_initcall()
 */
static void __init mv88de3100_init(void)
{
	mv88de3100_devs_init();

	pm_power_off = galois_power_off;

	print_version();
}

#ifdef CONFIG_CACHE_TAUROS3
static int __init mv88de3015_l2x0_init(void)
{
	return tauros3_init(__io(MEMMAP_CPU_SL2C_REG_BASE), 0x0, 0x0);
}
postcore_initcall(mv88de3015_l2x0_init);
#endif

void galois_arch_reset(char mode, const char *cmd)
{
#ifdef CONFIG_MV88DE3100_SM
#define SM_MSG_EXTRA_BUF_ADDR (MEMMAP_SM_REG_BASE+SM_ITCM_ALIAS_SIZE+SM_DTCM_SIZE-512)
	int len = 0;
	int msg = MV_SM_POWER_SYS_RESET;
	void *p = __io(SM_MSG_EXTRA_BUF_ADDR);

	if (cmd != NULL) {
		len = strlen(cmd) + 1;
		if (len > SM_MSG_EXTRA_BUF_SIZE - sizeof(len)) {
			len = SM_MSG_EXTRA_BUF_SIZE - sizeof(len);
			printk("cut off too long reboot args to %d bytes\n", len);
		}
		printk("reboot cmd is %s@%d\n", cmd, len);
		memcpy(p, &len, sizeof(len));
		memcpy(p + sizeof(len), cmd, len);
	} else
		memset(p, 0, sizeof(int));

	flush_cache_all();

	bsm_msg_send(MV_SM_ID_POWER, &msg, sizeof(int));
	for (;;);
#endif
}

static void __init mv88de3100_fixup(struct machine_desc *desc, struct tag *tag,
		char **cmdline, struct meminfo *mi)
{
	struct tag *tmp_tag = tag;
	/*
	 * MV88DE3100 only receives ATAG_CORE, ATAG_MEM, ATAG_CMDLINE, ATAG_NONE
	 * Here change ATAG_MEM to be controlled by Linux itself.
	 */
	if (tmp_tag->hdr.tag == ATAG_CORE) {
		for (; tmp_tag->hdr.size; tmp_tag = tag_next(tmp_tag)) {
			if (tmp_tag->hdr.tag == ATAG_MEM) {
				tmp_tag->u.mem.size = CONFIG_MV88DE3100_CPU0MEM_SIZE;
				tmp_tag->u.mem.start = CONFIG_MV88DE3100_CPU0MEM_START;
				break;
			}
		}
	}
}

#define MV88DE3100_PHYSIO_BASE 0xF0000000
#define ARM_BOOT_PARAMS_ADDR (CONFIG_MV88DE3100_CPU0MEM_START + 0x00000100)

MACHINE_START(MV88DE3100, "MV88DE3100")
	/* Maintainer: Yufeng Zheng<yfzheng@marvell.com> */
//	.phys_io	= MV88DE3100_PHYSIO_BASE,
//	.io_pg_offst	= (((u32)MV88DE3100_PHYSIO_BASE) >> 18) & 0xfffc,
	.boot_params	= ARM_BOOT_PARAMS_ADDR,

	.fixup		= mv88de3100_fixup,
	.map_io		= mv88de3100_map_io,
	.init_irq	= mv88de3100_init_irq,
	.timer		= &apbt_timer,
	.init_machine	= mv88de3100_init,
MACHINE_END
