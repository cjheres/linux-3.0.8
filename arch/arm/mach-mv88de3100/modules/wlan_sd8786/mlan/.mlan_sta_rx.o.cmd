cmd_arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_sta_rx.o := arm-unknown-linux-gnueabi-gcc -Wp,-MD,arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/.mlan_sta_rx.o.d  -nostdinc -isystem /home/build/toolchain/prebuilt/linux-x86_64/toolchain/arm-unknown-linux-gnueabi-4.5.3-glibc/target-arm-unknown-linux-gnueabi/usr/lib/gcc/arm-unknown-linux-gnueabi/4.5.3/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-mv88de3100/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -I/home/build/kernel/linux-2.6.35.14/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan -DLINUX -DFPNUM='"55"' -DDEBUG_LEVEL1 -DPROC_DEBUG -DREASSOCIATION -DMFG_CMD_SUPPORT  -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(mlan_sta_rx)"  -D"KBUILD_MODNAME=KBUILD_STR(86mlan)"  -c -o arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_sta_rx.o arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_sta_rx.c

deps_arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_sta_rx.o := \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_sta_rx.c \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_decl.h \
    $(wildcard include/config/change.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/atomic.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/generic/atomic64.h) \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
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
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/hwcap.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/outercache.h \
    $(wildcard include/config/outer/cache/sync.h) \
    $(wildcard include/config/outer/cache.h) \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/atomic-long.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_ioctl.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_ieee.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_11d.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_join.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_scan.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_util.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_fw.h \
    $(wildcard include/config/timeshare.h) \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_main.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_rx.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_11n_aggr.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_11n_rxreorder.h \

arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_sta_rx.o: $(deps_arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_sta_rx.o)

$(deps_arch/arm/mach-mv88de3100/modules/wlan_sd8786/mlan/mlan_sta_rx.o):
