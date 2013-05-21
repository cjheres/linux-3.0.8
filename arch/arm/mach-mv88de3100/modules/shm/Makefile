include $(srctree)/arch/arm/mach-mv88de3100/mv88de3100.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

ifeq ($(CONFIG_MV88DE3100_SHM_DEBUG),y)
  DEBFLAGS = -O -g -DENABLE_DEBUG -DENABLE_DEBUG_OSAL # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

ccflags-y	+= $(DEBFLAGS)


GALOIS_FIRMWARE_DIR = ../../GaloisSoftware

obj-$(CONFIG_MV88DE3100_SHM) += galois_shm.o
galois_shm-objs := shm_driver.o shm_device.o memory_engine.o shm_api_kernel.o
