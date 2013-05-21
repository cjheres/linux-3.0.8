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

#ifndef _ERRCODE_AUDIO_H_
#define _ERRCODE_AUDIO_H_

#define E_AUDIO( code ) ( E_ERR | E_AUDIO_BASE | ( (code) & 0xFFFF ) )

/* error code or audio module */
#define E_AUDIO_OK				E_SUC

/* audio generic error code */
#define E_AUDIO_GEN_START					(0x0000)

/* audio cfg error code */
#define E_AUDIO_CFG_START					(0x0100)
#define E_AUDIO_INVALID_OUTPUT_MODE			(E_AUDIO(E_AUDIO_CFG_START+0x00))
#define E_AUDIO_INVALID_LFE_MODE			(E_AUDIO(E_AUDIO_CFG_START+0x01))
#define E_AUDIO_INVALID_SPDIF_MODE			(E_AUDIO(E_AUDIO_CFG_START+0x02))
#define E_AUDIO_INVALID_STEREO_MODE			(E_AUDIO(E_AUDIO_CFG_START+0x03))
#define E_AUDIO_INVALID_DUAL_MONO_MODE		(E_AUDIO(E_AUDIO_CFG_START+0x04))
#define E_AUDIO_INVALID_PCM_WORD_SIZE		(E_AUDIO(E_AUDIO_CFG_START+0x05))
#define E_AUDIO_INVALID_PCM_SCL_FACTOR		(E_AUDIO(E_AUDIO_CFG_START+0x06))
#define E_AUDIO_INVALID_COMPR_MODE			(E_AUDIO(E_AUDIO_CFG_START+0x07))
#define E_AUDIO_INVALID_DRC_CUT				(E_AUDIO(E_AUDIO_CFG_START+0x08))
#define E_AUDIO_INVALID_DRC_BOOST			(E_AUDIO(E_AUDIO_CFG_START+0x09))
#define E_AUDIO_INVALID_DIALNORM_MODE		(E_AUDIO(E_AUDIO_CFG_START+0x0a))
#define E_AUDIO_INVALID_DIALNORM_LEVEL		(E_AUDIO(E_AUDIO_CFG_START+0x0b))
#define E_AUDIO_INVALID_KARAOKE_MODE		(E_AUDIO(E_AUDIO_CFG_START+0x0c))

/* audio cfg error code */
#define E_AUDIO_CMD_START					(0x0200)
#define E_AUDIO_INVALID_CMD					(E_AUDIO(E_AUDIO_DBG_START+0x00))

/* audio debug/test error code */
#define E_AUDIO_DBG_START					(0x0300)
#define E_AUDIO_OPENFILE					(E_AUDIO(E_AUDIO_DBG_START+0x00))
#define E_AUDIO_UNKNOWN_INPUTFILE			(E_AUDIO(E_AUDIO_DBG_START+0x01))
#define E_AUDIO_DISPLAY_USAGE_ONLY			(E_AUDIO(E_AUDIO_DBG_START+0x02))
#define E_AUDIO_INVALID_COMMAND_LINE		(E_AUDIO(E_AUDIO_DBG_START+0x04))
#define E_AUDIO_INVALID_FILE_FORMAT			(E_AUDIO(E_AUDIO_DBG_START+0x05))
#define E_AUDIO_INVALID_NOUTPUT_CHANS		(E_AUDIO(E_AUDIO_DBG_START+0x06))
#define E_AUDIO_ILLEGAL_CHAN_ROUTE_ARRAY	(E_AUDIO(E_AUDIO_DBG_START+0x07))
#define E_AUDIO_INVALID_DBGINFO_ARG			(E_AUDIO(E_AUDIO_DBG_START+0x08))

#endif //_ERRCODE_AUDIO_H_
