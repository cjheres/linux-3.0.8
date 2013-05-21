include $(srctree)/arch/arm/mach-mv88de3100/mv88de3100.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

GALOIS_FIRMWARE_DIR	= ../../GaloisSoftware

obj-$(CONFIG_MV88DE3100_I2C) += i2c.o
i2c-objs := i2c_bus.o sw_i2c.o
i2c-objs += $(SOC_APB_DIR)/source/i2c_master.o
obj-$(CONFIG_MV88DE3100_LINUX_I2C) += i2c-mv88de3100.o
