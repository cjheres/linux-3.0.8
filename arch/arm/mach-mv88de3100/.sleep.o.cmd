cmd_arch/arm/mach-mv88de3100/sleep.o := arm-unknown-linux-gnueabi-gcc -Wp,-MD,arch/arm/mach-mv88de3100/.sleep.o.d  -nostdinc -isystem /home/build/toolchain/prebuilt/linux-x86_64/toolchain/arm-unknown-linux-gnueabi-4.5.3-glibc/target-arm-unknown-linux-gnueabi/usr/lib/gcc/arm-unknown-linux-gnueabi/4.5.3/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-mv88de3100/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork  -D__LINUX_ARM_ARCH__=7 -march=armv7-a  -include asm/unified.h -msoft-float    -DBERLIN -DSoC_Galois -D__LINUX_KERNEL__ -D__CODE_LINK__=0 -DCPUINDEX=0 -DBERLIN_B_0=0x1000 -DBERLIN_C_0=0x2000 -DBERLIN_BG2=0x3000 -DBERLIN_BG2_Z2=0x3010 -DBERLIN_BG2_A0=0x3020 -DBERLIN_CHIP_VERSION=BERLIN_BG2_A0 -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/APB/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include/Firmware_Berlin_BG2_A0 -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/DHUB/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/MISC/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/PINMUX/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/DEV_INIT/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/GPIO/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/MISC/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Common/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/OSAL/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/OSAL/include/CPU0 -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/modules/shm    -c -o arch/arm/mach-mv88de3100/sleep.o arch/arm/mach-mv88de3100/sleep.S

deps_arch/arm/mach-mv88de3100/sleep.o := \
  arch/arm/mach-mv88de3100/sleep.S \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/linkage.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/assembler.h \
    $(wildcard include/config/cpu/feroceon.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/smp.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/hwcap.h \
  /home/build/kernel/linux-2.6.35.14/arch/arm/include/asm/system.h \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/arch/has/barriers.h) \
    $(wildcard include/config/arm/dma/mem/bufferable.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/32v6k.h) \

arch/arm/mach-mv88de3100/sleep.o: $(deps_arch/arm/mach-mv88de3100/sleep.o)

$(deps_arch/arm/mach-mv88de3100/sleep.o):
