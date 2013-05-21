include $(src)/mv88de3100.mk

# Global flags passed to gcc
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)
asflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

# SOC_OBJS defines low-level driver objects that should be
# compiled and linked into U-Boot.
GALOIS_FIRMWARE_DIR     = GaloisSoftware
SOC_OBJS	+= $(SOC_DHUB_DIR)/source/hal_dhub.o
SOC_OBJS	+= $(SOC_MISC_DIR)/source/galois_speed.o
SOC_OBJS	+= $(SOC_GPIO_DIR)/source/gpio.o
obj-y 	:= core.o irq.o time.o platform_devs.o pinmux.o mv88de3100.o

mv88de3100-objs	:= $(SOC_OBJS)

obj-$(CONFIG_MV88DE3100_DYNAMIC_FREQ) += freq.o
obj-$(CONFIG_MV88DE3100_ETHERNET) += geth.o
obj-$(CONFIG_SMP) += platsmp.o headsmp.o
obj-$(CONFIG_HOTPLUG_CPU) += hotplug.o
obj-$(CONFIG_CPU_FREQ) += cpufreq.o
obj-$(CONFIG_LOCAL_TIMERS) += localtimer.o
obj-$(CONFIG_MV88DE3100_SM) += sm.o
obj-$(CONFIG_PM) += pm.o sleep.o
obj-$(CONFIG_MV88DE3100_SPI) += modules/spi/
obj-$(CONFIG_MV88DE3100_I2C) += modules/i2c/
obj-$(CONFIG_MV88DE3100_GPU) += modules/gpu/
obj-$(CONFIG_MV88DE3100_GPU3D) += modules/gpu3D/
obj-$(CONFIG_MV88DE3100_IR) += modules/ir/
obj-$(CONFIG_MV88DE3100_SDIO_WLAN_8787) += modules/wlan_sd8787/
obj-$(CONFIG_MV88DE3100_SDIO_BT_8787) += modules/bt_sd8787/
obj-$(CONFIG_MV88DE3100_SDIO_WLAN_8786) += modules/wlan_sd8786/
obj-$(CONFIG_MV88DE3100_SDIO_WLAN_8797) += modules/wlan_sd8797/
obj-$(CONFIG_MV88DE3100_USB_WLAN_8797) += modules/wlan_usb8797/
obj-$(CONFIG_MV88DE3100_PXA3XX_NFC) += pxa3xx_nand.o
pxa3xx_nand-objs := pxa3xx_nand_debu.o
ifeq ($(CONFIG_MV88DE3100_NAND_RANDOMIZER), y)
	pxa3xx_nand-objs += prbs15.o nand_randomizer.o
endif
obj-$(CONFIG_MV88DE3100_SHM) += modules/shm/
obj-$(CONFIG_MV88DE3100_CC) += modules/cc/
obj-$(CONFIG_MV88DE3100_PE_MODULE) += modules/pe/
obj-$(CONFIG_MV88DE3100_PWM) += modules/pwm/
obj-$(CONFIG_MV88DE3100_GPIO) += modules/gpio/

clean:
	rm -f $(SOBJS) $(OBJS)

