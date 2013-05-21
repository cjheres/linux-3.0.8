include $(srctree)/arch/arm/mach-mv88de3100/mv88de3100.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

GALOIS_FIRMWARE_DIR	= ../../GaloisSoftware

obj-$(CONFIG_MV88DE3100_SPI) += spi.o
spi-objs := spi_bus.o
spi-objs += $(SOC_APB_DIR)/source/spi_master.o