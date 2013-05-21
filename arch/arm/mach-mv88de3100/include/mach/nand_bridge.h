#ifndef __NAND_BIRDGE__
#define __NAND_BRIDGE__
#include "api_dhub.h"
#include "pBridge.h"
enum {
	READ_DATA_CHANNEL_ID = PBSemaMap_dHubSemID_dHubCh0_intr,
	WRITE_DATA_CHANNEL_ID = PBSemaMap_dHubSemID_dHubCh1_intr,
	DESCRIPTOR_CHANNEL_ID = PBSemaMap_dHubSemID_dHubCh2_intr,
	NFC_DEV_CTL_CHANNEL_ID = PBSemaMap_dHubSemID_dHubCh3_intr,
	SPI_DEV_CTL_CHANNEL_ID = PBSemaMap_dHubSemID_dHubCh4_intr
};
#define READ_DATA_CHAN_CMD_BASE		(0)
#define READ_DATA_CHAN_CMD_SIZE		(2 << 3)
#define READ_DATA_CHAN_DATA_BASE	(READ_DATA_CHAN_CMD_BASE + READ_DATA_CHAN_CMD_SIZE)
#define READ_DATA_CHAN_DATA_SIZE	(32 << 3)

#define WRITE_DATA_CHAN_CMD_BASE	(READ_DATA_CHAN_DATA_BASE + READ_DATA_CHAN_DATA_SIZE)
#define WRITE_DATA_CHAN_CMD_SIZE	(2 << 3)
#define WRITE_DATA_CHAN_DATA_BASE	(WRITE_DATA_CHAN_CMD_BASE + WRITE_DATA_CHAN_CMD_SIZE)
#define WRITE_DATA_CHAN_DATA_SIZE	(32 << 3)

#define DESCRIPTOR_CHAN_CMD_BASE	(WRITE_DATA_CHAN_DATA_BASE + WRITE_DATA_CHAN_DATA_SIZE)
#define DESCRIPTOR_CHAN_CMD_SIZE	(2 << 3)
#define DESCRIPTOR_CHAN_DATA_BASE	(DESCRIPTOR_CHAN_CMD_BASE + DESCRIPTOR_CHAN_CMD_SIZE)
#define DESCRIPTOR_CHAN_DATA_SIZE	(32 << 3)

#define NFC_DEV_CTL_CHAN_CMD_BASE	(DESCRIPTOR_CHAN_DATA_BASE + DESCRIPTOR_CHAN_DATA_SIZE)
#define NFC_DEV_CTL_CHAN_CMD_SIZE	(2 << 3)
#define NFC_DEV_CTL_CHAN_DATA_BASE	(NFC_DEV_CTL_CHAN_CMD_BASE + NFC_DEV_CTL_CHAN_CMD_SIZE)
#define NFC_DEV_CTL_CHAN_DATA_SIZE	(32 << 3)

#define SPI_DEV_CTL_CHAN_CMD_BASE	(NFC_DEV_CTL_CHAN_DATA_BASE + NFC_DEV_CTL_CHAN_DATA_SIZE)
#define SPI_DEV_CTL_CHAN_CMD_SIZE	(2 << 3)
#define SPI_DEV_CTL_CHAN_DATA_BASE	(SPI_DEV_CTL_CHAN_CMD_BASE + SPI_DEV_CTL_CHAN_CMD_SIZE)
#define SPI_DEV_CTL_CHAN_DATA_SIZE	(32 << 3)
#endif

