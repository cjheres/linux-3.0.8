#ifndef __ASM_MACH_SYSTEM_H
#define __ASM_MACH_SYSTEM_H

extern void galois_arch_reset(char mode, const char *cmd);
static inline void arch_idle(void)
{
	cpu_do_idle();
}

static inline void arch_reset(char mode, const char *cmd)
{
	galois_arch_reset(mode, cmd);
}
#endif /* __ASM_MACH_SYSTEM_H */
