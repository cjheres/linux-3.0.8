cmd_arch/arm/mach-mv88de3100/localtimer.o := arm-unknown-linux-gnueabi-gcc -Wp,-MD,arch/arm/mach-mv88de3100/.localtimer.o.d  -nostdinc -isystem /home/build/toolchain/prebuilt/linux-x86_64/toolchain/arm-unknown-linux-gnueabi-4.5.3-glibc/target-arm-unknown-linux-gnueabi/usr/lib/gcc/arm-unknown-linux-gnueabi/4.5.3/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-mv88de3100/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DBERLIN -DSoC_Galois -D__LINUX_KERNEL__ -D__CODE_LINK__=0 -DCPUINDEX=0 -DBERLIN_B_0=0x1000 -DBERLIN_C_0=0x2000 -DBERLIN_BG2=0x3000 -DBERLIN_BG2_Z2=0x3010 -DBERLIN_BG2_A0=0x3020 -DBERLIN_CHIP_VERSION=BERLIN_BG2_A0 -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/APB/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include/Firmware_Berlin_BG2_A0 -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/DHUB/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/MISC/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/PINMUX/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/DEV_INIT/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/GPIO/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/MISC/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Common/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/OSAL/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/OSAL/include/CPU0 -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/modules/shm   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(localtimer)"  -D"KBUILD_MODNAME=KBUILD_STR(localtimer)"  -c -o arch/arm/mach-mv88de3100/localtimer.o arch/arm/mach-mv88de3100/localtimer.c

deps_arch/arm/mach-mv88de3100/localtimer.o := \
  arch/arm/mach-mv88de3100/localtimer.c \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  include/linux/smp.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/use/generic/smp/helpers.h) \
    $(wildcard include/config/debug/preempt.h) \
  include/linux/errno.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
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
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/posix_types.h \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/prefetch.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/processor.h \
    $(wildcard include/config/mmu.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/hwcap.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/cache.h \
    $(wildcard include/config/arm/l1/cache/shift.h) \
    $(wildcard include/config/aeabi.h) \
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
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/irqflags.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/outercache.h \
    $(wildcard include/config/outer/cache/sync.h) \
    $(wildcard include/config/outer/cache.h) \
  include/asm-generic/cmpxchg-local.h \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
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
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/bitops.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
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
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/string.h \
  include/linux/preempt.h \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/arm/thumbee.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/smp.h \
  arch/arm/mach-mv88de3100/include/mach/smp.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/hardware/gic.h \
  include/linux/clockchips.h \
    $(wildcard include/config/generic/clockevents/build.h) \
    $(wildcard include/config/generic/clockevents.h) \
  include/linux/clocksource.h \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/clocksource/watchdog.h) \
    $(wildcard include/config/generic/time/vsyscall.h) \
  include/linux/timex.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/seqlock.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/spinlock_types.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/rwlock_types.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/spinlock.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/inline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/atomic.h \
    $(wildcard include/config/generic/atomic64.h) \
  include/asm-generic/atomic-long.h \
  include/linux/math64.h \
  include/linux/param.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/timex.h \
  arch/arm/mach-mv88de3100/include/mach/timex.h \
  arch/arm/mach-mv88de3100/include/mach/galois_platform.h \
    $(wildcard include/config/mv88de3100/asic.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/io.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/memory.h \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/thumb2/kernel.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/dram/size.h) \
    $(wildcard include/config/dram/base.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/discontigmem.h) \
  include/linux/const.h \
  arch/arm/mach-mv88de3100/include/mach/memory.h \
    $(wildcard include/config/mv88de3100/cpu0mem/start.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/sizes.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  arch/arm/mach-mv88de3100/include/mach/io.h \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/smp_twd.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/localtimer.h \
    $(wildcard include/config/local/timers.h) \
    $(wildcard include/config/have/arm/twd.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/smp_twd.h \
  arch/arm/mach-mv88de3100/include/mach/irqs.h \

arch/arm/mach-mv88de3100/localtimer.o: $(deps_arch/arm/mach-mv88de3100/localtimer.o)

$(deps_arch/arm/mach-mv88de3100/localtimer.o):
