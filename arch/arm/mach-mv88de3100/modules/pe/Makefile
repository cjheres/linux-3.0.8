include $(srctree)/arch/arm/mach-mv88de3100/mv88de3100.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

ccflags-y	+= -DBERLIN_SINGLE_CPU

GALOIS_FIRMWARE_DIR = ../../GaloisSoftware
SHM_SOURCE_DIR	    = $(SHM_DIR)/source/linux/kernel

obj-$(CONFIG_MV88DE3100_PE_MODULE) += galois_pe.o
galois_pe-objs := pe_driver.o $(SOC_DHUB_DIR)/source/avio_dhub_drv.o pe_agent_driver.o
