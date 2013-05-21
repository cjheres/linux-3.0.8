/********************************************************************************
 * Marvell GPL License Option
 *
 * If you received this File from Marvell, you may opt to use, redistribute and/or
 * modify this File in accordance with the terms and conditions of the General
 * Public License Version 2, June 1991 (the "GPL License"), a copy of which is
 * available along with the File in the license.txt file or by writing to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or
 * on the worldwide web at http://www.gnu.org/licenses/gpl.txt.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY
 * DISCLAIMED.  The GPL License provides additional details about this warranty
 * disclaimer.
 ********************************************************************************/
#ifndef _SM_H_
#define _SM_H_

#include "com_type.h"

#define SM_ITCM_BASE	0x00000000
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
#define SM_ITCM_SIZE	0x00010000	// 64k
#else
#define SM_ITCM_SIZE	0x00008000	// 32k
#endif
#define SM_ITCM_END		(SM_ITCM_BASE + SM_ITCM_SIZE)
#define SM_ITCM_ALIAS_SIZE	0x00020000	// 128k

#define SM_DTCM_BASE	0x04000000
#define SM_DTCM_SIZE	0x00004000	// 16k
#define SM_DTCM_END		(SM_DTCM_BASE + SM_DTCM_SIZE)
#define SM_DTCM_ALIAS_SIZE	0x00020000	// 128k

#define SM_APBC_BASE	0x10000000
#define SM_APBC_SIZE	0x00010000	// 64k
#define SM_APBC_END		(SM_APBC_BASE + SM_APBC_SIZE)
#define SM_APBC_ALIAS_SIZE	0x00010000	// 64k

#define SM_SECM_BASE	0x40000000
#define SM_SECM_SIZE	0x00010000	//64k
#define SM_SECM_END		(SM_SECM_BASE + SM_SECM_SIZE)
#define SM_SECM_ALIAS_SIZE	0x00010000	// 64k

#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
#define SM_WOL_BASE		0x10010000
#define SM_WOL_SIZE		0x00001000	//4k
#define SM_WOL_END		(SM_WOL_BASE + SM_WOL_SIZE)
#define SM_WOL_ALIAS_SIZE	0x00001000	// 4k

#define SM_CEC_BASE		0x10011000
#define SM_CEC_SIZE		0x00001000	//4k
#define SM_CEC_END		(SM_CEC_BASE + SM_CEC_SIZE)
#define SM_CEC_ALIAS_SIZE	0x00001000	// 4k
#endif

#define SOC_ITCM_BASE	0xf7f80000
#define SOC_DTCM_BASE	0xf7fa0000
#define SOC_APBC_BASE	0xf7fc0000
#define SOC_SECM_BASE	0xf7fd0000
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
#define SOC_WOL_BASE	0xf7fE0000
#define SOC_CEC_BASE	0xf7fE1000
#endif

#define SOC_ITCM( addr )	( (INT)addr - SM_ITCM_BASE + SOC_ITCM_BASE )
#define SOC_DTCM( addr )	( (INT)addr - SM_DTCM_BASE + SOC_DTCM_BASE )
#define SOC_APBC( addr )	( (INT)addr - SM_APBC_BASE + SOC_APBC_BASE )
#define SOC_SECM( addr )	( (INT)addr - SM_SECM_BASE + SOC_SECM_BASE )
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
#define SOC_WOL( addr )	( (INT)addr - SM_WOL_BASE + SOC_WOL_BASE )
#define SOC_CEC( addr )	( (INT)addr - SM_CEC_BASE + SOC_CEC_BASE )
#endif

#if ( CPUINDEX == 2 )
#define SM_APBC( reg )         ( (INT32)(reg) )
#else
#define SM_APBC( reg )         ( (INT32)(reg) - SM_APBC_BASE + SOC_APBC_BASE )
#endif

#define MV_SM_READ_REG32( reg )		( *( (volatile  UINT32*)SM_APBC( reg ) ) )
#define MV_SM_READ_REG8( reg )		( *( (volatile UCHAR*)SM_APBC( reg ) ) )
#define MV_SM_WRITE_REG32( reg, value )	( *( (volatile UINT32*)SM_APBC( reg ) ) = (UINT32)( value ) )
#define MV_SM_WRITE_REG8( reg, value )	( *( (volatile UCHAR*)SM_APBC( reg ) ) = (UCHAR)( value ) )

#define SM_SM_APB_ICTL_BASE		(SM_APBC_BASE)
#define SM_SM_APB_WDT0_BASE		(SM_APBC_BASE + 0x1000)
#define SM_SM_APB_WDT1_BASE		(SM_APBC_BASE + 0x2000)
#define SM_SM_APB_WDT2_BASE		(SM_APBC_BASE + 0x3000)
#define SM_SM_APB_TIMERS_BASE	(SM_APBC_BASE + 0x4000)
#define SM_SM_APB_COUNTER_BASE	(SM_APBC_BASE + 0x5000)
#define SM_SM_APB_SPI_BASE		(SM_APBC_BASE + 0x6000)
#define SM_SM_APB_I2C0_BASE		(SM_APBC_BASE + 0x7000)
#define SM_SM_APB_I2C1_BASE		(SM_APBC_BASE + 0x8000)
#define SM_SM_APB_UART0_BASE	(SM_APBC_BASE + 0x9000)
#define SM_SM_APB_UART1_BASE	(SM_APBC_BASE + 0xA000)
#define SM_SM_APB_UART2_BASE	(SM_APBC_BASE + 0xB000)
#define SM_SM_APB_GPIO_BASE		(SM_APBC_BASE + 0xC000)
#define SM_SM_SYS_CTRL_REG_BASE	(SM_APBC_BASE + 0xD000)
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
#define SM_SM_APB_ICTL1_BASE		(SM_APBC_BASE + 0xE000)
#define SM_SM_APB_ICTL2_BASE		(SM_APBC_BASE + 0xF000)
#endif

#define	LSb32SS_SM_ICTL_IRQ_SM_WDT0				0
#define	LSb32SS_SM_ICTL_IRQ_SM_WDT1				1
#define	LSb32SS_SM_ICTL_IRQ_SM_WDT2				2
#define	LSb32SS_SM_ICTL_IRQ_SM_TIMERS			3
#if	(BERLIN_CHIP_VERSION >= BERLIN_C_0)
#define	LSb32SS_SM_ICTL_IRQ_SM_GPIO_B			4
#else
#define	LSb32SS_SM_ICTL_IRQ_RTC					4
#endif
#define	LSb32SS_SM_ICTL_IRQ_SM_SPI				5
#define	LSb32SS_SM_ICTL_IRQ_SM_I2C0				6
#define	LSb32SS_SM_ICTL_IRQ_SM_I2C1				7
#define	LSb32SS_SM_ICTL_IRQ_SM_UART0			8
#define	LSb32SS_SM_ICTL_IRQ_SM_UART1			9
#define	LSb32SS_SM_ICTL_IRQ_SM_UART2			10
#define	LSb32SS_SM_ICTL_IRQ_SM_GPIO_A			11
#define	LSb32SS_SM_ICTL_IRQ_SM_ADC				12
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
#define	LSb32SS_SM_ICTL_IRQ_SOC2SM_SW			13
#define	LSb32SS_SM_ICTL_IRQ_SM_TSEN			14
#define	LSb32SS_SM_ICTL_IRQ_SM_WOL				15
#define	LSb32SS_SM_ICTL_IRQ_SM_CEC				16
#define	LSb32SS_SM_ICTL_IRQ_SM_CEC_FIFO			17
#define	LSb32SS_SM_ICTL_IRQ_SM_ETH				18
#define	LSb32SS_SM_ICTL_IRQ_SM_HPD				19
#define	LSb32SS_SM_ICTL_IRQ_SM_IHPD			20
#define	LSb32SS_SM_ICTL_IRQ_SM_HDMIRX_PWR		21
#define	LSb32SS_SM_ICTL_IRQ_SM_IHDMIRX_PWR		22
#endif

#define SM_MSG_SIZE			32
#define SM_MSG_BODY_SIZE	( SM_MSG_SIZE - sizeof(INT16) * 2 )
#define SM_MSGQ_TOTAL_SIZE	512
#define SM_MSGQ_HEADER_SIZE	SM_MSG_SIZE
#define SM_MSGQ_SIZE		( SM_MSGQ_TOTAL_SIZE - SM_MSGQ_HEADER_SIZE )
#define SM_MSGQ_MSG_COUNT	( SM_MSGQ_SIZE / SM_MSG_SIZE )

typedef struct
{
	INT16	m_iModuleID;
	INT16	m_iMsgLen;
	CHAR	m_pucMsgBody[SM_MSG_BODY_SIZE];
} MV_SM_Message;

typedef struct
{
	INT32	m_iWrite;
	INT32	m_iRead;
	INT32	m_iWriteTotal;
	INT32	m_iReadTotal;
	CHAR	m_Padding[ SM_MSGQ_HEADER_SIZE - sizeof(INT32) * 4 ];
	CHAR	m_Queue[ SM_MSGQ_SIZE ];
} MV_SM_MsgQ;

#define SM_MSG_EXTRA_BUF_SIZE 494
#define SM_MSG_EXTRA_BUF_ADDR_OFF 512

//********************DTCM memory usage********************
// 1. bss & stack section of SM;
// 2. Shared memory for SM & SOC
// (DTCM can be accessed by both SM and SOC. Also, the data in DTCM can be saved even in standby state.)
// The layout of DTCM is like this:
// 0x04000000-: bss & stack
// ......
// 0x04003600-: Shared data area
// 0x04003800-: Message queue
#define SM_DTCM_SHARED_DATA_SIZE        512
#define SM_DTCM_SHARED_DATA_ADDR	( SM_DTCM_END - SM_MSGQ_TOTAL_SIZE * 4 - SM_DTCM_SHARED_DATA_SIZE)
#define SM_PINMUX_TABLE_OFFSET          SM_DTCM_SHARED_DATA_ADDR
#define SM_PINMUX_TABLE_SIZE            128

#define SM_CPU0_INPUT_QUEUE_ADDR	( SM_DTCM_END - SM_MSGQ_TOTAL_SIZE * 4 )
#define SM_CPU1_INPUT_QUEUE_ADDR	( SM_DTCM_END - SM_MSGQ_TOTAL_SIZE * 3 )//Not used, reserved
#define SM_CPU0_OUTPUT_QUEUE_ADDR	( SM_DTCM_END - SM_MSGQ_TOTAL_SIZE * 2 )
#define SM_CPU1_OUTPUT_QUEUE_ADDR	( SM_DTCM_END - SM_MSGQ_TOTAL_SIZE * 1 )//Not for this usage now, but for some msg shared between SM and other CPUs
//*********************************************************

#define SM_LOADER_STS_ADDR			( SM_DTCM_END - 8 )
//Below value will be saved in SM_LOADER_STS_ADDR, are used to pass to loader by sata driver
#define SM_LOADER_STS_COLDBOOT		(0xF)
#define SM_LOADER_STS_WARMBOOT		(0x0)
// define Msg
#define MV_SM_POWER_WARMDOWN_REQUEST	1//from Linux
#define	MV_SM_POWER_WARMDOWN_REQUEST_2	2
#define MV_SM_POWER_WARMUP_SOURCE_REQUEST   3//from Linux
#define MV_SM_POWER_WARMUP_REQUEST		4 //from Linux
#define MV_SM_POWER_WARMDOWN_TIME		11//from Linux
#define MV_SM_POWER_WARMUP_TIME			12//from Linux

#define MV_SM_POWER_WARMUP_0_RESP		(MV_SM_STATE_WAIT_WARMUP_0_RESP)
#define MV_SM_POWER_WARMDOWN_1_RESP		(MV_SM_STATE_WAIT_WARMDOWN_RESP)
#define MV_SM_POWER_WARMUP_1_RESP		(MV_SM_STATE_WAIT_WARMUP_RESP)
#define MV_SM_POWER_WARMDOWN_2_RESP		(MV_SM_STATE_WAIT_WARMDOWN_2_RESP)
#define MV_SM_POWER_WARMUP_2_RESP		(MV_SM_STATE_WAIT_WARMUP_2_RESP)
//reset the whole system
#define MV_SM_POWER_SYS_RESET		0xFF
#define	MV_SoC_STATE_COLDBOOT		0x21
#define MV_SM_POWER_FASTBOOT_ENABLE	0x0F
#define MV_SM_POWER_FASTBOOT_DISABLE	0x0E
//error detection
#define MV_SM_POWER_INSPECT_REQUEST	0xED

//sys module message
//Msg to tell SM: loader firmware is ready
#define MV_SM_Loader_FWready	0x1
#define MV_SM_Loader_Appready	0x2
#define MV_SM_Loader_Appexit	0x3
#define MV_SM_UART_Enable		0x4
#define MV_SM_UART_Disable		0x5
#define MV_SM_GPIO_SET			0x6
#define MV_SM_GPIO_GET			0x7

//Msg to tell Linux is ready to receive IR
#define MV_SM_IR_Linuxready     30
//Msg to send to Watchdog
#define MV_SM_WD_APP_START		0x12
#define MV_SM_WD_APP_EXIT		0x36 //When wdt is enabled, after app exits, SM should take charge
#define MV_SM_WD_APP_CONTINUE		0x63
#define MV_SM_WD_Kickoff		0x24
//Msg to send to Temp
#define MV_SM_TEMP_EN			0x03
#define MV_SM_TEMP_DISEN		0x0C
#define MV_SM_TEMP_SAMPLE		0xF0
#define MV_SM_TEMP_SAMPLE_ORG	0xF3//get original data
#define MV_SM_TEMP_SAMPLE_CES	0xFC//get Celsius value
#define MV_SM_TEMP_CPU_TYPE     0xCC
#define MV_SM_TEMP_CPU_TYPE_B0  0xB0
//CEC message
#define MV_SM_CEC_APPREADY		0x1
#define MV_SM_CEC_INFO			0x2
#define MV_SM_CEC_TASKSTOP		0x3//SM send to app, shows SM stops CEC task already

#define MV_SM_CEC_HDMICTRL_ONOFF 0x4 // SoC-->SM: User has enabled/disabled HDMI Control
#define MV_SM_CEC_APP_REQUESTED 0x5	 // SM-->SoC: TV requested to run this App while you were in standby.
#define MV_SM_CEC_NO_APP_REQUESTED 0x6	 // SM-->SoC: no app request, we started for another reason.
#define MV_SM_CEC_ACTIVE_SOURCE_EVENT 0x7   //SM-->SoC: Signal SoC CEC at <ACTIVE SOURCE> event occurred.
#define MV_SM_CEC_STREAM_PATH_EVENT 0x8  //SM-->SoC: Signal SoC CEC that a <SET STREAM PATH> event occurred.
#define MV_SM_CEC_UC_PRESSED_EVENT 0x9  //SM-->SoC: Signal SoC CEC that a <UC PRESSED> event occurred.
#define MV_SM_CEC_DISC_INSERTED  0xA  //SM-->SoC: Signal SoC CEC that SM booted SoC because a disk was inserted.
#define MV_SM_CEC_COLD_BOOT  0xB  //SM-->SoC: Signal SoC CEC that SM booted from COLD BOOT (AC ON).


//VPG DPMS message
#define MV_SM_VGA_SET_TIMEOUT_THRESHOLD  0x1
#define MV_SM_VGA_NOSIGNAL      0x2 //SM send to app. Told app that vga has no signal.
#define MV_SM_VGA_HASSIGNAL     0x3 //SM send to app. Told app that vga has signal.
#define MV_SM_VGA_GET_STATUS    0x4
#define MV_SM_VGA_ENABLE_WAKEUP 0x5 //app send to SM. Told SM to warmup SoC if vga has signal.
#define MV_SM_VGA_RESET_TIMER   0x6 //app send to SM. Told SM to reset timer for detecting dpms status

//INPUT message
#define MV_SM_INPUT_DISABLE     0x1
#define MV_SM_INPUT_ENABLE      0x2
#define MV_SM_INPUT_REQ_CUSTOM  0x3
#define MV_SM_INPUT_SEND_KEY    0x4

// These messages use MODULE_ID = MV_SM_ID_SYS. Handled on SM side by Misc.c.
//TouchKey message: EJECT touchpad has been pressed. (Response to MV_SM_FE_REQUEST_KEYSTATE below)
// In the MV_SM_FE_TOUCH_EJECT message, the first parameter is:
//		1 = an Eject keypress happened after the last time you polled,
//		0 = no Eject keypresses.
//
#define MV_SM_FE_TOUCH_EJECT	0xFE

// SYS (loader message) : load sequence is complete. (Sent by NavMgr when loader reaches "ready" state.)
#define MV_SM_FE_LOAD_COMPLETE	0xFD
// SYS (loader message) : eject complete (sent by NavMgr after eject sequence.)
#define MV_SM_FE_EJECT_COMPLETE 0xFC
// SoC side sends this message every 50 mssec or so, polling for an Eject key. If there was one, SM responds with MV_SM_FE_TOUCH_EJECT above.
#define MV_SM_FE_REQUEST_KEYSTATE 0xFB

// FIXME better not name as this
#define SONY_LED_ONOFF_CONTRL		0x5D
#define SONY_EJECT_LED_ONOFF_CONTRL	0x5E


// ---------- KEY(raptor front panel communication)  ------------------------------
//   message from raptor/cbutton at startup, for Sony to update Atmel IC registers after final MV software
#define MV_SM_KEY_TOUCH_REG_UPDATE 0x43
//
//  Following are control code types that may be in the MV_SM_KEY_TOUCH_REG_UPDATE:
#define	MV_SM_KEY_DIAG_DELTAS_AFTER_PRESS 1
#define MV_SM_KEY_DIAG_DELTAS_PERIODICALLY 2
#define MV_SM_KEY_DIAG_ATMEL_SOFT_RESET 4
#define MV_SM_KEY_DIAG_DUMP_ATMEL_REGISTERS 8

//  Raptor connect button pressed/released
#define MV_SM_KEY_CONNECT_PRESSED 0x41
#define	MV_SM_KEY_CONNECT_RELEASED 0x42


typedef enum
{
    MV_SM_DPMS_UNKNOWN,
    MV_SM_DPMS_OFF,
    MV_SM_DPMS_ON,
} MV_SM_DPMS_STATUS;

typedef enum
{
    MV_SM_ID_SYS = 1,
    MV_SM_ID_COMM,
    MV_SM_ID_IR,
    MV_SM_ID_KEY,
    MV_SM_ID_POWER,
    MV_SM_ID_WD,
    MV_SM_ID_TEMP,
    MV_SM_ID_VFD,
    MV_SM_ID_SPI,
    MV_SM_ID_I2C,
    MV_SM_ID_UART,
    MV_SM_ID_CEC,
    MV_SM_ID_LOGOBAR,
    MV_SM_ID_VGADPMS,
    MV_SM_ID_LED,
    MV_SM_ID_RTC,

#ifdef SONY_RELEASE					/* ir.c uses this in both beetle and raptor platforms. */
	MV_SM_ID_CONNECT_BUTTON,		/* so that ir.c can tell whether power cmd came from connect button or power key on remote. */

#ifdef SONY_PLATFORM_RAPTOR
	MV_SM_ID_TouchKey,				/* raptor touch panel for power/eject */
    MV_SM_ID_SoC_GPIO,				/* SM to SoC telling it to operate on GPIOs */
#endif //SONY_PLATFORM_RAPTOR
#endif //SONY_RELEASE
    /* add new module id here */
    MV_SM_ID_MAX,
}MV_SM_MODULE_ID;

#define MAX_MSG_TYPE (MV_SM_ID_MAX - 1)

//Information shared between SM/IR/MVPM Linux driver and GaloisSoftware
//SM driver ioctl cmd
#define SM_READ			0x1
#define SM_WRITE		0x2
#define SM_RDWR			0x3
#define SM_Enable_WaitQueue	0x1234
#define SM_Disable_WaitQueue	0x3456
//MVPM driver ioctl cmd
#define		CMD_BASE				0x00//send direct cmd
#define		CMD_PARA_BASE			0x10//send parameter to PowerManagement Module

#define		OFFSET_WARMDOWN			0
#define		OFFSET_WARMDOWN_ONTIME		1
#define		OFFSET_WARMUP_ONTIME		2
#define		OFFSET_WARMDOWN_2		3
#define		OFFSET_PVCOMPS_ONEWIRE	4
#define		OFFSET_CPUBusyLimit	0
#define		OFFSET_InputIdleLimit	1
#define		OFFSET_CPUTimerInv		2
#define		OFFSET_DetectTimerInv	3
#define		OFFSET_ENTER_LOW_SPEED_MODE	(0x100+1)
#define		OFFSET_ENTER_SLEEP_MODE		(0x100+2)
#define		OFFSET_PRINT_CLOCK_INFO		(0x100+3)
#define		OFFSET_ENTER_NORMAL_SPEED_MODE  (0x100+4)

#define		CMD_WARMDOWN			(CMD_BASE+OFFSET_WARMDOWN)
#define		CMD_WARMDOWN_2			(CMD_BASE+OFFSET_WARMDOWN_2)
#define		CMD_WARMDOWN_ONTIME		(CMD_BASE+OFFSET_WARMDOWN_ONTIME)
#define		CMD_WARMUP_ONTIME		(CMD_BASE+OFFSET_WARMUP_ONTIME)
#define		CMD_PVCOMPS_ONEWIRE		(CMD_BASE+OFFSET_PVCOMPS_ONEWIRE)
#define		CMD_SET_CPUBusyLimit	(CMD_PARA_BASE+OFFSET_CPUBusyLimit)//when cpu usage > (CPUBusyLimit)%, the cpu is defined as busy and Berlin can not enter standby
#define		CMD_SET_InputIdleLimit	(CMD_PARA_BASE+OFFSET_InputIdleLimit)//If the input has no event in (InputIdleLimit)seconds,Berlin may enter standby, also, the cpu usage should be considered.
#define		CMD_SET_CPUTimerInv	(CMD_PARA_BASE+OFFSET_CPUTimerInv)
#define		CMD_SET_DetectTimerInv	(CMD_PARA_BASE+OFFSET_DetectTimerInv)
#define		CMD_ENTER_LOW_SPEED_MODE	(CMD_PARA_BASE+OFFSET_ENTER_LOW_SPEED_MODE)
#define		CMD_ENTER_SLEEP_MODE		(CMD_PARA_BASE+OFFSET_ENTER_SLEEP_MODE)
#define		CMD_PRINT_CLOCK_INFO		(CMD_PARA_BASE+OFFSET_PRINT_CLOCK_INFO)
#define		CMD_ENTER_NORMAL_SPEED_MODE (CMD_PARA_BASE+OFFSET_ENTER_NORMAL_SPEED_MODE)

typedef struct _MVPM_PVCOMPS_ONEWIRE_PARA
{
	int		VDDVol;//vdd voltage, unit:mV
	char	gpioport;//gpio port number
	int		res;//reserved
}ONEWIRE_PARA;

typedef enum _MV_SM_WARMUP_SOURCE_TYPE
{
    MV_SM_WARMUP_SOURCE_INVALID = -1,
    MV_SM_WARMUP_SOURCE_IR = 0,
    MV_SM_WARMUP_SOURCE_WIFI_BT,
    MV_SM_WARMUP_SOURCE_WOL,
    MV_SM_WARMUP_SOURCE_VGA,
    MV_SM_WARMUP_SOURCE_CEC,
    MV_SM_WARMUP_SOURCE_NUM,
} MV_SM_WARMUP_SOURCE_TYPE;

typedef enum _MV_SM_LED_STATE
{
	MV_SM_LED_OFF = 5,
	MV_SM_LED_ON,
	MV_SM_LED_BLINK,
	MV_SM_SAFETY_2,		// long off, then 2 short blinks, then repeat
	MV_SM_SAFETY_7,		// long off, then 7 blinks, then repeat
	MV_SM_LED_DIAG_1,	// MV diagnostic blink "21"
	MV_SM_LED_MAX
} MV_SM_LED_STATE;
	#define MV_SM_LED_MIN MV_SM_LED_OFF
/* For I2C comm */
#define SM_I2C_WRITE_BYTES	8
typedef enum _MV_SM_I2C_OPR
{
    MV_SM_I2C_BUS_INIT,
    MV_SM_I2C_OPR_INIT,
    MV_SM_I2C_READ_OPR,
    MV_SM_I2C_WRITE_OPR,
    MV_SM_I2C_OPR_TERM
} MV_SM_I2C_OPR;

typedef enum _MV_SM_I2C_ERR
{
    MV_SM_I2C_PCK_UNFNSH,
    MV_SM_I2C_PCK_FNSH_OK,
    MV_SM_I2C_INDEX_ERR,
    MV_SM_I2C_WRITE_ERR,
    MV_SM_I2C_UNDEF_ERR
} MV_SM_I2C_ERR;

typedef struct SM_I2C_Byte_Write_Msg_T
{
    unsigned char               msg_packet_id; /* packet id: every NVM edid update which include multiple write own a unique packet id*/
    unsigned char               i2c_rw; /* i2c read OR write message: 0-- i2c read; 1--i2c write*/
    unsigned char               master_id; /* TWS2, TWS3 are i2c buses on SM, and their ids on SM are 0, 1. Be careful when converting i2c bus id on Soc and on SM */
    unsigned char               addr_type;
    union
    {
        unsigned short              slave_addr; /* Currently only 0x50 is valid */
        unsigned short              error_type; /*It's used for notifying Soc when SM has errors during programming NVM edid through I2C */
    };
    unsigned short              sub_addr;
    union
    {
        unsigned short              iCount;
        unsigned short              errIndex;
    };
    unsigned short              totalBytes;
    unsigned char               data[SM_I2C_WRITE_BYTES];
} SM_I2C_Byte_Write_Msg;

#endif // #ifndef _SM_H_
