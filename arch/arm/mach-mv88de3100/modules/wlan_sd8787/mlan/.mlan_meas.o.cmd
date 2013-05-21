cmd_arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_meas.o := arm-unknown-linux-gnueabi-gcc -Wp,-MD,arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/.mlan_meas.o.d  -nostdinc -isystem /home/build/toolchain/prebuilt/linux-x86_64/toolchain/arm-unknown-linux-gnueabi-4.5.3-glibc/target-arm-unknown-linux-gnueabi/usr/lib/gcc/arm-unknown-linux-gnueabi/4.5.3/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-mv88de3100/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -I/home/build/kernel/linux-2.6.35.14/include -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan -DLINUX -I/home/build/kernel/linux-2.6.35.14/arch/arm/mach-mv88de3100/GaloisSoftware/Drivers/Board/platform_setting/ -DFPNUM='"66"' -DDEBUG_LEVEL1 -DPROC_DEBUG -DSTA_SUPPORT -DREASSOCIATION -DSTA_WEXT -DUAP_SUPPORT -DUAP_WEXT -DWIFI_DIRECT_SUPPORT -DWIFI_DISPLAY_SUPPORT -DMFG_CMD_SUPPORT -DSDIO_MULTI_PORT_TX_AGGR -DSDIO_MULTI_PORT_RX_AGGR -DSDIO_SUSPEND_RESUME -DDFS_TESTING_SUPPORT  -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(mlan_meas)"  -D"KBUILD_MODNAME=KBUILD_STR(87mlan)"  -c -o arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_meas.o arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_meas.c

deps_arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_meas.o := \
  arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_meas.c \
  arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_decl.h \
    $(wildcard include/config/change.h) \
    $(wildcard include/config/action/get.h) \
    $(wildcard include/config/action/set.h) \
    $(wildcard include/config/action/default.h) \
    $(wildcard include/config/action/max.h) \
  arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_ioctl.h \
    $(wildcard include/config/acs/mode.h) \
    $(wildcard include/config/manual.h) \
    $(wildcard include/config/5g.h) \
    $(wildcard include/config/2g.h) \
  arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_ieee.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_join.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_util.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_fw.h \
    $(wildcard include/config/extra/tlv/bytes.h) \
    $(wildcard include/config/5ghz.h) \
  arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_main.h \
  arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_meas.h \

arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_meas.o: $(deps_arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_meas.o)

$(deps_arch/arm/mach-mv88de3100/modules/wlan_sd8787/mlan/mlan_meas.o):
