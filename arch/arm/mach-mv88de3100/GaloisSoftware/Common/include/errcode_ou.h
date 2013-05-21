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
#ifndef _OU_ERRCODE_H
#define _OU_ERRCODE_H

#define E_OU( code )	(E_ERR | E_OU_BASE | ( (code) & 0xFFFF) )

//	define errer codes for downloading file from internet
//	0x0000~0x00FF 
#define E_OU_GET_IMAGE_START 					(0x0000) 
#define E_OU_GET_IMAGE_INTERNET_START			(0x00)
#define E_OU_NETWORK_NOT_CONNECTED				(E_OU(E_OU_GET_IMAGE_INTERNET_START + 0x02)) 
#define	E_OU_NETWORK_SERVER_ERROR				(E_OU(E_OU_GET_IMAGE_INTERNET_START + 0x03)) 

//	define error codes for Image Tools
//	0x0100-0x1FF
#define E_OU_IMAGETOOL_START						(0x100)
#define E_OU_IMAGETOOL_SIGNATURE_VERIFY_FAILED		(E_OU(E_OU_IMAGETOOL_START + 0x01))
#define E_OU_IMAGETOOL_INVALID_IMAGE				(E_OU(E_OU_IMAGETOOL_START + 0x02))


//	define	file error, include NAND ioctrl error , file open error .... etc
#define	E_OU_FILE_CTRL_START					(0x200)
#define	E_OU_CAN_NOT_OPEN_VERSION_FILE			(E_OU(E_OU_FILE_CTRL_START + 0x08))
#define E_OU_NAND_IO							(E_OU(E_OU_FILE_CTRL_START + 0x0B))
#define E_OU_UNIIMAGE_IO						(E_OU(E_OU_FILE_CTRL_START + 0x0D))
#define E_OU_IMAGE_NOENT		(E_OU(E_OU_FILE_CTRL_START + 0x0E))
#define E_OU_PARTITION_TABLE					(E_OU(E_OU_FILE_CTRL_START + 0x0F))
#define E_OU_NO_UNIIMG							(E_OU(E_OU_FILE_CTRL_START + 0x10)

#define	E_OU_COMMOM_ERROR_START				(0x300)
#define	E_OU_COMMON_SYSTEM_ERROR			(E_OU(E_OU_COMMOM_ERROR_START + 0x01))
#define	E_OU_COMMAND_ERROR					(E_OU(E_OU_COMMOM_ERROR_START + 0x02))
#define E_OU_NAND_PART_NOT_FOUND	        (E_OU(E_OU_COMMOM_ERROR_START + 0x07))
#define E_OU_BUSY							(E_OU(E_OU_COMMOM_ERROR_START + 0x08))
#define E_OU_OUTOFSPACE						(E_OU(E_OU_COMMOM_ERROR_START + 0x09))

// these definition are not errors , just indicate a status of function return

#define	E_OU_RIGHT_RETURN_STATUS_START		(0x400)
#define	E_OU_RECEIVE_QUIT_MSG				(E_OU(E_OU_RIGHT_RETURN_STATUS_START + 0x01)) 	
#define	E_OU_NO_LATEST_IMAGE				(E_OU(E_OU_RIGHT_RETURN_STATUS_START + 0x02)) 
#define	E_OU_NO_LATEST_APP_IMAGE			(E_OU(E_OU_RIGHT_RETURN_STATUS_START + 0x03))
#define	E_OU_CANCELED						(E_OU(E_OU_RIGHT_RETURN_STATUS_START + 0x04))
#define E_OU_NONEEDTOOU						(E_OU(E_OU_RIGHT_RETURN_STATUS_START+0x05))


//	errors for the Image
#define	E_OU_INVALID_IMAGE_START			(0x500)
#define E_OU_INVALID_UNIIMAGE				(E_OU(E_OU_INVALID_IMAGE_START+0x0B))
#define E_OU_MISMATCH_UNIIMAGE				(E_OU(E_OU_INVALID_IMAGE_START+0x0C))

/*
 * errors for online image manager
 */
#define	E_OU_OUIMGMAN_START					(0x600)
#define E_OU_OUIMGMAN_TOOMANY				(E_OU(E_OU_OUIMGMAN_START+0x01))
#define E_OU_OUIMGMAN_NOENT					(E_OU(E_OU_OUIMGMAN_START+0x02))

#endif
