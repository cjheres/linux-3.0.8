cmd_arch/arm/mach-mv88de3100/headsmp.o := arm-unknown-linux-gnueabi-gcc -Wp,-MD,arch/arm/mach-mv88de3100/.headsmp.o.d  -nostdinc -isystem /home/build/toolchain/prebuilt/linux-x86_64/toolchain/arm-unknown-linux-gnueabi-4.5.3-glibc/target-arm-unknown-linux-gnueabi/usr/lib/gcc/arm-unknown-linux-gnueabi/4.5.3/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-mv88de3100/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork  -D__LINUX_ARM_ARCH__=7 -march=armv7-a  -include asm/unified.h -msoft-float    -DBERLIN -DSoC_Galois -D__LINUX_KERNEL__ -D__CODE_LINK__=0 -DCPUINDEX=0 -DBERLIN_B_0=0x1000 -DBERLIN_C_0=0x2000 -DBERLIN_BG2=0x3000 -DBERLIN_BG2_Z2=0x3010 -DBERLIN_BG2_A0=0x3020 -DBERLIN_CHIP_VERSION=BERLIN_BG2_A0 -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/APB/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/Common/include/Firmware_Berlin_BG2_A0 -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/DHUB/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/MISC/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/PINMUX/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/DEV_INIT/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/GPIO/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Galois_SoC/MISC/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Common/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/OSAL/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/OSAL/include/CPU0 -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/modules/shm    -c -o arch/arm/mach-mv88de3100/headsmp.o arch/arm/mach-mv88de3100/headsmp.S

deps_arch/arm/mach-mv88de3100/headsmp.o := \
  arch/arm/mach-mv88de3100/headsmp.S \
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
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \

arch/arm/mach-mv88de3100/headsmp.o: $(deps_arch/arm/mach-mv88de3100/headsmp.o)

$(deps_arch/arm/mach-mv88de3100/headsmp.o):
