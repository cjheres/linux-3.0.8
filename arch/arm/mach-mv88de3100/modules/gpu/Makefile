include $(srctree)/arch/arm/mach-mv88de3100/mv88de3100.mk

ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)
#ccflags-y	+= -I$(GALOIS_FIRMWARE_DIR)/GFXDS/GfxLinux/inc/

obj-$(CONFIG_MV88DE3100_GPU) += gfx.o
gfx-objs := gfx_linux_device.o  gfx_linux_dispatch.o  gfx_linux_kernel.o
