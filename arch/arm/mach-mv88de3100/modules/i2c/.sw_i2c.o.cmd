cmd_arch/arm/mach-mv88de3100/modules/i2c/sw_i2c.o := arm-unknown-linux-gnueabi-gcc -Wp,-MD,arch/arm/mach-mv88de3100/modules/i2c/.sw_i2c.o.d  -nostdinc -isystem /home/build/toolchain/prebuilt/linux-x86_64/toolchain/arm-unknown-linux-gnueabi-4.5.3-glibc/target-arm-unknown-linux-gnueabi/usr/lib/gcc/arm-unknown-linux-gnueabi/4.5.3/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-mv88de3100/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DBERLIN -DSoC_Galois -D__LINUX_KERNEL__ -D__CODE_LINK__=0 -DCPUINDEX=0 -DBERLIN_B_0=0x1000 -DBERLIN_C_0=0x2000 -DBERLIN_BG2=0x3000 -DBERLIN_BG2_Z2=0x3010 -DBERLIN_BG2_A0=0x3020 -DBERLIN_CHIP_VERSION=BERLIN_BG2_A0 -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/APB/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include/Firmware_Berlin_BG2_A0 -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/DHUB/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/MISC/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/PINMUX/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/DEV_INIT/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/GPIO/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/MISC/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Common/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/OSAL/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/OSAL/include/CPU0 -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/modules/shm   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(sw_i2c)"  -D"KBUILD_MODNAME=KBUILD_STR(i2c)"  -c -o arch/arm/mach-mv88de3100/modules/i2c/sw_i2c.o arch/arm/mach-mv88de3100/modules/i2c/sw_i2c.c

deps_arch/arm/mach-mv88de3100/modules/i2c/sw_i2c.o := \
  arch/arm/mach-mv88de3100/modules/i2c/sw_i2c.c \
    $(wildcard include/config/mv88de3100/dynamic/freq.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include/Firmware_Berlin_BG2_A0/Galois_memmap.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include/ctypes.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include/galois_io.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include/galois_cfg.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include/galois_type.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include/galois_common.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/MISC/include/galois_hal_com.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include/Firmware_Berlin_BG2_A0/global.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include/Firmware_Berlin_BG2_A0/SysMgr.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/APB/include/apb_perf_base.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/GPIO/include/gpio.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/APB/include/i2c_master.h \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/posix_types.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/bitops.h \
    $(wildcard include/config/smp.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/system.h \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/arch/has/barriers.h) \
    $(wildcard include/config/arm/dma/mem/bufferable.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/32v6k.h) \
  include/linux/linkage.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/linkage.h \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/irqflags.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/hwcap.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/outercache.h \
    $(wildcard include/config/outer/cache/sync.h) \
    $(wildcard include/config/outer/cache.h) \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/arm/thumbee.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
    $(wildcard include/config/mmu.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/prefetch.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/processor.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/cache.h \
    $(wildcard include/config/arm/l1/cache/shift.h) \
    $(wildcard include/config/aeabi.h) \
  include/linux/delay.h \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /home/build/toolchain/prebuilt/linux-x86_64/toolchain/arm-unknown-linux-gnueabi-4.5.3-glibc/target-arm-unknown-linux-gnueabi/usr/lib/gcc/arm-unknown-linux-gnueabi/4.5.3/include/stdarg.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/dynamic_debug.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/div64.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/delay.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  arch/arm/mach-mv88de3100/include/mach/twsi_bus.h \
  arch/arm/mach-mv88de3100/include/mach/galois_generic.h \
  arch/arm/mach-mv88de3100/include/mach/galois_platform.h \
    $(wildcard include/config/mv88de3100/asic.h) \
  arch/arm/mach-mv88de3100/include/mach/freq.h \

arch/arm/mach-mv88de3100/modules/i2c/sw_i2c.o: $(deps_arch/arm/mach-mv88de3100/modules/i2c/sw_i2c.o)

$(deps_arch/arm/mach-mv88de3100/modules/i2c/sw_i2c.o):
