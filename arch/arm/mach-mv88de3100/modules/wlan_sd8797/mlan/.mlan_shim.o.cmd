cmd_arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_shim.o := arm-unknown-linux-gnueabi-gcc -Wp,-MD,arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/.mlan_shim.o.d  -nostdinc -isystem /home/build/toolchain/prebuilt/linux-x86_64/toolchain/arm-unknown-linux-gnueabi-4.5.3-glibc/target-arm-unknown-linux-gnueabi/usr/lib/gcc/arm-unknown-linux-gnueabi/4.5.3/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-mv88de3100/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -I/home/build/kernel/linux-2.6.35.14/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan -DLINUX -DFPNUM='"62"' -DDEBUG_LEVEL1 -DPROC_DEBUG -DSTA_SUPPORT -DREASSOCIATION -DUAP_SUPPORT -DWFD_SUPPORT -DMFG_CMD_SUPPORT -DSDIO_MULTI_PORT_TX_AGGR -DSDIO_MULTI_PORT_RX_AGGR -DSDIO_SUSPEND_RESUME  -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(mlan_shim)"  -D"KBUILD_MODNAME=KBUILD_STR(97mlan)"  -c -o arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_shim.o arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_shim.c

deps_arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_shim.o := \
  arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_shim.c \
  arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_decl.h \
    $(wildcard include/config/change.h) \
    $(wildcard include/config/action/get.h) \
    $(wildcard include/config/action/set.h) \
    $(wildcard include/config/action/default.h) \
    $(wildcard include/config/action/max.h) \
  arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_ioctl.h \
    $(wildcard include/config/acs/mode.h) \
    $(wildcard include/config/manual.h) \
  arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_ieee.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_join.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_util.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_fw.h \
    $(wildcard include/config/timeshare.h) \
    $(wildcard include/config/extra/tlv/bytes.h) \
    $(wildcard include/config/5ghz.h) \
  arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_main.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_wmm.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_sdio.h \
    $(wildcard include/config/reg.h) \
  arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_uap.h \

arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_shim.o: $(deps_arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_shim.o)

$(deps_arch/arm/mach-mv88de3100/modules/wlan_sd8797/mlan/mlan_shim.o):
