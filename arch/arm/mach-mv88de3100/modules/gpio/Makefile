include $(srctree)/arch/arm/mach-mv88de3100/mv88de3100.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

GALOIS_FIRMWARE_DIR = ../../GaloisSoftware

obj-$(CONFIG_MV88DE3100_GPIO) += kgpio.o
kgpio-objs := gpio_main.o

#ifeq ($(CONFIG_MV88DE3100_GPIO),m)
kgpio-objs += $(SOC_APB_DIR)/source/spi_master.o
#endif
