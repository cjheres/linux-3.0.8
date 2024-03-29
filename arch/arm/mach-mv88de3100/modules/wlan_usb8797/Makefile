# File: Makefile
#
# Copyright (C) 2008-2011, Marvell International Ltd.
#
# This software file (the "File") is distributed by Marvell International
# Ltd. under the terms of the GNU General Public License Version 2, June 1991
# (the "License").  You may use, redistribute and/or modify this File in
# accordance with the terms and conditions of the License, a copy of which
# is available by writing to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or on the
# worldwide web at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
#
# A copy of the GPL is available in file gpl-2.0.txt accompanying in this
# deliverables.
#
# THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE
# IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE
# ARE EXPRESSLY DISCLAIMED.  The License provides additional details about
# this warranty disclaimer.
#############################################################################
# Configuration Options
#############################################################################

# Debug Option
# DEBUG LEVEL n/1/2:
# n: NO DEBUG
# 1: Only PRINTM(MMSG,...), PRINTM(MFATAL,...), ...
# 2: All PRINTM()
CONFIG_DEBUG=1

# Proc debug file
CONFIG_PROC_DEBUG=y

# Enable STA mode support
CONFIG_STA_SUPPORT=y

# Enable uAP mode support
CONFIG_UAP_SUPPORT=y

# Enable WIFIDIRECT support
CONFIG_WIFI_DIRECT_SUPPORT=y

# Re-association in driver
CONFIG_REASSOCIATION=y

# Manufacturing firmware support
CONFIG_MFG_CMD_SUPPORT=y

# Big-endian platform
CONFIG_BIG_ENDIAN=n

# DFS testing support
CONFIG_DFS_TESTING_SUPPORT=y

#############################################################################
# Compiler Flags
#############################################################################
EXTRA_CFLAGS += -I$(srctree)/include
EXTRA_CFLAGS += -I$(srctree)/arch/arm/mach-mv88de3100/modules/wlan_usb8797/mlan
EXTRA_CFLAGS += -DLINUX

EXTRA_CFLAGS += -DFPNUM='"62"'

ifeq ($(CONFIG_DEBUG),1)
	EXTRA_CFLAGS += -DDEBUG_LEVEL1
endif

ifeq ($(CONFIG_DEBUG),2)
	EXTRA_CFLAGS += -DDEBUG_LEVEL1
	EXTRA_CFLAGS += -DDEBUG_LEVEL2
	DBG=	-dbg
endif

ifeq ($(CONFIG_PROC_DEBUG),y)
	EXTRA_CFLAGS += -DPROC_DEBUG
	export CONFIG_PROC_DEBUG
endif

ifeq ($(CONFIG_STA_SUPPORT),y)
	EXTRA_CFLAGS += -DSTA_SUPPORT
ifeq ($(CONFIG_REASSOCIATION),y)
	EXTRA_CFLAGS += -DREASSOCIATION
endif
endif

ifeq ($(CONFIG_UAP_SUPPORT),y)
	EXTRA_CFLAGS += -DUAP_SUPPORT
endif

ifeq ($(CONFIG_WIFI_DIRECT_SUPPORT),y)
	EXTRA_CFLAGS += -DWIFI_DIRECT_SUPPORT
endif

ifeq ($(CONFIG_MFG_CMD_SUPPORT),y)
	EXTRA_CFLAGS += -DMFG_CMD_SUPPORT
endif

ifeq ($(CONFIG_BIG_ENDIAN),y)
	EXTRA_CFLAGS += -DBIG_ENDIAN_SUPPORT
endif

ifeq ($(CONFIG_DFS_TESTING_SUPPORT),y)
	EXTRA_CFLAGS += -DDFS_TESTING_SUPPORT
endif

#############################################################################
# Make Targets
#############################################################################

MOALOBJS =	mlinux/moal_main.o \
		mlinux/moal_ioctl.o \
		mlinux/moal_shim.o

MLANOBJS =	mlan/mlan_shim.o mlan/mlan_init.o \
		mlan/mlan_txrx.o \
		mlan/mlan_cmdevt.o mlan/mlan_misc.o \
		mlan/mlan_cfp.o \
		mlan/mlan_module.o

MLANOBJS += mlan/mlan_wmm.o
MLANOBJS += mlan/mlan_usb.o
MLANOBJS += mlan/mlan_11n_aggr.o
MLANOBJS += mlan/mlan_11n_rxreorder.o
MLANOBJS += mlan/mlan_11n.o
MLANOBJS += mlan/mlan_11d.o
MLANOBJS += mlan/mlan_11h.o
ifeq ($(CONFIG_STA_SUPPORT),y)
MLANOBJS += mlan/mlan_meas.o
MLANOBJS += mlan/mlan_scan.o \
			mlan/mlan_sta_ioctl.o \
			mlan/mlan_sta_rx.o \
			mlan/mlan_sta_tx.o \
			mlan/mlan_sta_event.o \
			mlan/mlan_sta_cmd.o \
			mlan/mlan_sta_cmdresp.o \
			mlan/mlan_join.o
MOALOBJS += mlinux/moal_priv.o \
            mlinux/moal_wext.o
endif
ifeq ($(CONFIG_UAP_SUPPORT),y)
MLANOBJS += mlan/mlan_uap_ioctl.o
MLANOBJS += mlan/mlan_uap_cmdevent.o
MLANOBJS += mlan/mlan_uap_txrx.o
MOALOBJS += mlinux/moal_uap.o
MOALOBJS += mlinux/moal_uap_priv.o
MOALOBJS += mlinux/moal_uap_wext.o
MOALOBJS += mlinux/moal_wext.o
endif

ifdef CONFIG_PROC_FS
MOALOBJS += mlinux/moal_proc.o
ifeq ($(CONFIG_PROC_DEBUG),y)
MOALOBJS += mlinux/moal_debug.o
endif
endif

obj-m := usb97mlan.o
usb97mlan-objs := $(MLANOBJS)
MOALOBJS += mlinux/moal_usb.o
obj-m += usb8797.o
usb8797-objs := $(MOALOBJS)

# End of file
