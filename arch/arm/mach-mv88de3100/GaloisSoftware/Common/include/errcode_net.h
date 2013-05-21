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

///////////////////////////////////////////////////////////////////////////////
//! \file errcode_net.h
//! \brief Header file for ERROR CODE
//!
//! Puopose:
//!	Defines a bunch error codes
///////////////////////////////////////////////////////////////////////////////
#ifndef	_ERRCODE_NET_H_
#define	_ERRCODE_NET_H_


#define E_NET( code ) ( E_ERR | E_NET_BASE | ( (code) & 0xFFFF ) )

#define E_NET_GENERAL_START					(0x0000)
#define E_NET_PARAM_INVALID				(E_NET(E_NET_GENERAL_START|0x01))
#define E_NET_UNKNOWN_FAILURE				(E_NET(E_NET_GENERAL_START|0x02))
#define E_NET_URI_INVALID					(E_NET(E_NET_GENERAL_START|0x03))
#define E_NET_SESSION_MODE_INVALID		(E_NET(E_NET_GENERAL_START|0x04))
#define E_NET_UNREGISTERED_LISTENER			(E_NET(E_NET_GENERAL_START|0x05))
#define E_NET_INVALID_DATETIME_FORMAT		(E_NET(E_NET_GENERAL_START|0x06))




#define E_NET_NETWORK_START           (0x1020)
#define E_NET_NETWORK_NOT_CONNECTED			(E_NET(E_NET_NETWORK_START|0x00)) /*  */
#define E_NET_RESOURCE_UNAVAILALBE		(E_NET(E_NET_NETWORK_START|0x01))
#define E_NET_SESSION_OPENED_UNSUCCESSFULLY	(E_NET(E_NET_NETWORK_START|0x02))
#define E_NET_SESSION_INVALID			(E_NET(E_NET_NETWORK_START|0x03))
#define E_NET_SESSION_IS_NOT_DOWNLOADING	(E_NET(E_NET_NETWORK_START|0x04))
#define E_NET_SESSION_IS_NOT_PAUSED			(E_NET(E_NET_NETWORK_START|0x05))
#define E_NET_SESSION_SEEK_OFFSET_INVALID	(E_NET(E_NET_NETWORK_START|0x06))
#define E_NET_SESSION_SEEK_NOT_SUPPORTED	(E_NET(E_NET_NETWORK_START|0x07))
#define E_NET_SESSION_CONFIG_INVALID		(E_NET(E_NET_NETWORK_START|0x08))
#define E_NET_UNRECOGNIZED_DATETIME_FORMAT	(E_NET(E_NET_NETWORK_START|0x09))
#define E_NET_PROTOCOL_NOT_SUPPORTTED		(E_NET(E_NET_NETWORK_START|0x0a))
#define E_NET_UNKNOWN_DOWNLOAD_TYPE			(E_NET(E_NET_NETWORK_START|0x0b))
#define E_NET_SESSION_FAIL_TO_INITILIZE_CURLIB	(E_NET(E_NET_NETWORK_START|0x0c))
#define E_NET_SESSION_FAIL_TO_CONNECT_TO_SERVER (E_NET(E_NET_NETWORK_START|0x0d))
#define E_NET_SESSION_NOT_IN_CREATED_STATE	(E_NET(E_NET_NETWORK_START|0x0e))
#define E_NET_SESSION_NOT_IN_CONNECTED_OR_STOPPED_STATE (E_NET(E_NET_NETWORK_START|0x0f))
#define E_NET_SESSION_FAIL_TO_CREATE_TASK	(E_NET(E_NET_NETWORK_START|0x10))
#define E_NET_SESSION_NOT_IN_STARTED_STATE	(E_NET(E_NET_NETWORK_START|0x11))
#define E_NET_SESSION_NOT_IN_PAUSED_STATE	(E_NET(E_NET_NETWORK_START|0x12))
#define E_NET_SESSION_NOT_IN_STREAM_DOWNLOAD_MODE	(E_NET(E_NET_NETWORK_START|0x13))
#define E_NET_SESSION_STATE_ERROR                	(E_NET(E_NET_NETWORK_START|0x14))


//add by qzhang
#define E_NET_SESSION_FAIL_TO_TRANSFERM_HTTPHEADER			(E_NET(E_NET_NETWORK_START|0x15))
#define E_NET_SESSION_FAIL_TO_INIT_COOKIE_PATH				(E_NET(E_NET_NETWORK_START|0x16))
#define E_NET_SESSION_FAIL_TO_OPEN_DOWNLOAD_FILE_TO_WRITE 	(E_NET(E_NET_NETWORK_START|0x17))
#define E_NET_SESSION_FAIL_TO_OPEN_UPLOAD_FILE_TO_READ 		(E_NET(E_NET_NETWORK_START|0x18))
#define E_NET_SESSION_INVALID_PARAM_IN_STREAM_DOWNLOAD_MODE	(E_NET(E_NET_NETWORK_START|0x19))
#define E_NET_SESSION_INVALID_PARAM_IN_STRING_SEND_MODE 	(E_NET(E_NET_NETWORK_START|0x1a))
#define E_NET_SESSION_INVALID_MODE 							(E_NET(E_NET_NETWORK_START|0x1b))



#define E_NET_CURL_START									(0x0700)
#define E_NET_CURL_INVALID_PARAM							(E_NET(E_NET_CURL_START+0x00))
#define E_NET_CURL_UNSUPPORTED_PROTOCOL						(E_NET(E_NET_CURL_START+0x01))
#define E_NET_CURL_FAILED_INIT								(E_NET(E_NET_CURL_START+0x02))
#define E_NET_CURL_INVALID_URL								(E_NET(E_NET_CURL_START+0x03))
#define E_NET_CURL_INTERNAL_ERROR							(E_NET(E_NET_CURL_START+0x04))
#define E_NET_CURL_FAIL_RESOLVE_PROXY						(E_NET(E_NET_CURL_START+0x05))
#define E_NET_CURL_FAIL_RESOLVE_HOST						(E_NET(E_NET_CURL_START+0x06))
#define E_NET_CURL_FAIL_CONNECT								(E_NET(E_NET_CURL_START+0x07))

#define E_NET_CURL_REMOTE_ACCESS_DENIED						(E_NET(E_NET_CURL_START+0x09))

#define E_NET_CURL_PARTIAL_FILE								(E_NET(E_NET_CURL_START+0x12))

#define E_NET_CURL_HTTP_RETURNED_ERROR						(E_NET(E_NET_CURL_START+0x16))
#define E_NET_CURL_WRITE_ERROR								(E_NET(E_NET_CURL_START+0x17))

#define E_NET_CURL_FAIL_UPLOAD								(E_NET(E_NET_CURL_START+0x19))
#define E_NET_CURL_READ_ERROR								(E_NET(E_NET_CURL_START+0x1a))
#define E_NET_CURL_OUT_OF_MEMORY							(E_NET(E_NET_CURL_START+0x1b))
#define E_NET_CURL_OPERATION_TIMEDOUT						(E_NET(E_NET_CURL_START+0x1c))

#define E_NET_CURL_RANGE_ERROR								(E_NET(E_NET_CURL_START+0x21))
#define E_NET_CURL_POST_ERROR								(E_NET(E_NET_CURL_START+0x22))
#define E_NET_CURL_SSL_CONNECT_ERROR						(E_NET(E_NET_CURL_START+0x23))
#define E_NET_CURL_BAD_DOWNLOAD_RESUME						(E_NET(E_NET_CURL_START+0x24))
#define E_NET_CURL_FILE_FAIL_READ_FILE						(E_NET(E_NET_CURL_START+0x25))

#define E_NET_CURL_FUNCTION_NOT_FOUND						(E_NET(E_NET_CURL_START+0x29))
#define E_NET_CURL_ABORTED_BY_CALLBACK						(E_NET(E_NET_CURL_START+0x2a))
#define E_NET_CURL_BAD_FUNCTION_ARGUMENT					(E_NET(E_NET_CURL_START+0x2b))

#define E_NET_CURL_INTERFACE_FAILED							(E_NET(E_NET_CURL_START+0x2d))

#define E_NET_CURL_TOO_MANY_REDIRECTS						(E_NET(E_NET_CURL_START+0x2f))

#define E_NET_CURL_PEER_FAILED_VERIFICATION					(E_NET(E_NET_CURL_START+0x33))
#define E_NET_CURL_GOT_NOTHING								(E_NET(E_NET_CURL_START+0x34))
#define E_NET_CURL_SSL_ENGINE_NOTFOUND						(E_NET(E_NET_CURL_START+0x35))
#define E_NET_CURL_SSL_ENGINE_SETFAILED						(E_NET(E_NET_CURL_START+0x36))	
#define E_NET_CURL_SEND_ERROR								(E_NET(E_NET_CURL_START+0x37))	
#define E_NET_CURL_RECV_ERROR								(E_NET(E_NET_CURL_START+0x38))

#define E_NET_CURL_SSL_CERT_ERROR							(E_NET(E_NET_CURL_START+0x3a))
#define E_NET_CURL_SSL_CIPHER_ERROR							(E_NET(E_NET_CURL_START+0x3b))
#define E_NET_CURL_SSL_CACERT_ERROR							(E_NET(E_NET_CURL_START+0x3c))
#define E_NET_CURL_BAD_CONTENT_ENCODING						(E_NET(E_NET_CURL_START+0x3d))

#define E_NET_CURL_FILESIZE_EXCEEDED						(E_NET(E_NET_CURL_START+0x3f))
#define E_NET_CURL_USE_SSL_FAILED							(E_NET(E_NET_CURL_START+0x40))
#define E_NET_CURL_SEND_FAIL_REWIND							(E_NET(E_NET_CURL_START+0x41))
#define E_NET_CURL_SSL_ENGINE_FAIL_INIT						(E_NET(E_NET_CURL_START+0x42))
#define E_NET_CURL_LOGIN_DENIED								(E_NET(E_NET_CURL_START+0x43))

#define E_NET_CURL_REMOTE_DISK_FULL							(E_NET(E_NET_CURL_START+0x46))

#define E_NET_REMOTE_FILE_EXISTS							(E_NET(E_NET_CURL_START+0x49))

#define E_NET_CURL_SSL_CACERT_BADFILE						(E_NET(E_NET_CURL_START+0x4d))

#define E_NET_CURL_REMOTE_FILE_NOT_FOUND					(E_NET(E_NET_CURL_START+0x4e))

#define E_NET_CURL_SSL_SHUTDOWN_FAILED						(E_NET(E_NET_CURL_START+0x50))

#define E_NET_CURL_RECEIVE_STOP_CMD							(E_NET(E_NET_CURL_START+0x51))
#define E_NET_CURL_SEEK_SEEK_CMD							(E_NET(E_NET_CURL_START+0x52))
#define E_NET_CURL_RECEIVE_CLOSE_CMD						(E_NET(E_NET_CURL_START+0x53))

#if 0
typedef enum {
  CURLE_OK = 0,
  CURLE_UNSUPPORTED_PROTOCOL, /* 1 */E_NET_CURL_UNSUPPORTED_PROTOCOL
  CURLE_FAILED_INIT,             /* 2 */E_NET_CURL_FAILED_INIT
  CURLE_URL_MALFORMAT,           /* 3 */ E_NET_SESSION_INVALID_URL
  CURLE_OBSOLETE4,               /* 4 - NOT USED */
  CURLE_COULDNT_RESOLVE_PROXY,   /* 5 */E_NET_CURL_FAIL_RESOLVE_PROXY
  CURLE_COULDNT_RESOLVE_HOST,    /* 6 */E_NET_CURL_FAIL_RESOLVE_HOST
  CURLE_COULDNT_CONNECT,         /* 7 */E_NET_CURL_FAIL_CONNECT
  CURLE_FTP_WEIRD_SERVER_REPLY,  /* 8 */E_NET_CURL_INTERNEL_ERROR
  CURLE_REMOTE_ACCESS_DENIED,    /* 9 a service was denied by the server
                                    due to lack of access - when login fails
                                    this is not returned. */
	E_NET_CURL_REMOTE_ACCESS_DENIED
  CURLE_OBSOLETE10,              /* 10 - NOT USED */
  CURLE_FTP_WEIRD_PASS_REPLY,    /* 11 */ E_NET_CURL_INTERNEL_ERROR
  CURLE_OBSOLETE12,              /* 12 - NOT USED */
  CURLE_FTP_WEIRD_PASV_REPLY,    /* 13 */ E_NET_CURL_INTERNEL_ERROR
  CURLE_FTP_WEIRD_227_FORMAT,    /* 14 */ E_NET_CURL_INTERNEL_ERROR
  CURLE_FTP_CANT_GET_HOST,       /* 15 */ E_NET_CURL_INTERNEL_ERROR
  CURLE_OBSOLETE16,              /* 16 - NOT USED */
  CURLE_FTP_COULDNT_SET_TYPE,    /* 17 */ E_NET_CURL_INTERNEL_ERROR
  CURLE_PARTIAL_FILE,            /* 18 */ E_NET_CURL_PARTIAL_FILE
  CURLE_FTP_COULDNT_RETR_FILE,   /* 19 */ E_NET_CURL_INTERNEL_ERROR
  CURLE_OBSOLETE20,              /* 20 - NOT USED */
  CURLE_QUOTE_ERROR,             /* 21 - quote command failure */E_NET_CURL_INTERNEL_ERROR
  CURLE_HTTP_RETURNED_ERROR,     /* 22 */E_NET_CURL_HTTP_RETURNED_ERROR
  CURLE_WRITE_ERROR,             /* 23 */ E_NET_CURL_WRITE_ERROR
  CURLE_OBSOLETE24,              /* 24 - NOT USED */
  CURLE_UPLOAD_FAILED,           /* 25 - failed upload "command" */E_NET_CURL_FAIL_UPLOAD
  CURLE_READ_ERROR,              /* 26 - could open/read from file */E_NET_CURL_READ_ERROR
  CURLE_OUT_OF_MEMORY,           /* 27 */E_NET_CURL_OUT_OF_MEMORY
  /* Note: CURLE_OUT_OF_MEMORY may sometimes indicate a conversion error
           instead of a memory allocation error if CURL_DOES_CONVERSIONS
           is defined
  */
  CURLE_OPERATION_TIMEDOUT,      /* 28 - the timeout time was reached */E_NET_CURL_OPERATION_TIMEDOUT
  CURLE_OBSOLETE29,              /* 29 - NOT USED */
  CURLE_FTP_PORT_FAILED,         /* 30 - FTP PORT operation failed */E_NET_CURL_INTERNEL_ERROR
  CURLE_FTP_COULDNT_USE_REST,    /* 31 - the REST command failed */E_NET_CURL_INTERNEL_ERROR
  CURLE_OBSOLETE32,              /* 32 - NOT USED */
  CURLE_RANGE_ERROR,             /* 33 - RANGE "command" didn't work */E_NET_CURL_RANGE_ERROR
  CURLE_HTTP_POST_ERROR,         /* 34 */E_NET_CURL_POST_ERROR
  CURLE_SSL_CONNECT_ERROR,       /* 35 - wrong when connecting with SSL */E_NET_CURL_SSL_CONNECT_ERROR
  CURLE_BAD_DOWNLOAD_RESUME,     /* 36 - couldn't resume download */E_NET_CURL_BAD_DOWNLOAD_RESUME
  CURLE_FILE_COULDNT_READ_FILE,  /* 37 */E_NET_CURL_FILE_FAIL_READ_FILE
  CURLE_LDAP_CANNOT_BIND,        /* 38 */ E_NET_CURL_INTERNEL_ERROR
  CURLE_LDAP_SEARCH_FAILED,      /* 39 */ E_NET_CURL_INTERNEL_ERROR
  CURLE_OBSOLETE40,              /* 40 - NOT USED */
  CURLE_FUNCTION_NOT_FOUND,      /* 41 */E_NET_CURL_FUNCTION_NOT_FOUND
  CURLE_ABORTED_BY_CALLBACK,     /* 42 */E_NET_CURL_ABORTED_BY_CALLBACK
  CURLE_BAD_FUNCTION_ARGUMENT,   /* 43 */E_NET_CURL_BAD_FUNCTION_ARGUMENT
  CURLE_OBSOLETE44,              /* 44 - NOT USED */
  CURLE_INTERFACE_FAILED,        /* 45 - CURLOPT_INTERFACE failed */E_NET_CURL_INTERFACE_FAILED
  CURLE_OBSOLETE46,              /* 46 - NOT USED */
  CURLE_TOO_MANY_REDIRECTS ,     /* 47 - catch endless re-direct loops */E_NET_CURL_TOO_MANY_REDIRECTS
  CURLE_UNKNOWN_TELNET_OPTION,   /* 48 - User specified an unknown option */E_NET_CURL_INTERNEL_ERROR
  CURLE_TELNET_OPTION_SYNTAX ,   /* 49 - Malformed telnet option */E_NET_CURL_INTERNEL_ERROR
  CURLE_OBSOLETE50,              /* 50 - NOT USED */
  CURLE_PEER_FAILED_VERIFICATION, /* 51 - peer's certificate or fingerprint wasn't verified fine */E_NET_CURL_PEER_FAILED_VERIFICATION
  CURLE_GOT_NOTHING,             /* 52 - when this is a specific error */E_NET_CURL_GOT_NOTHING
  CURLE_SSL_ENGINE_NOTFOUND,     /* 53 - SSL crypto engine not found */E_NET_CURL_SSL_ENGINE_NOTFOUND
  CURLE_SSL_ENGINE_SETFAILED,    /* 54 - can not set SSL crypto engine as default */E_NET_CURL_SSL_ENGINE_SETFAILED
  CURLE_SEND_ERROR,              /* 55 - failed sending network data */E_NET_CURL_SEND_ERROR
  CURLE_RECV_ERROR,              /* 56 - failure in receiving network data */E_NET_CURL_RECV_ERROR
  CURLE_OBSOLETE57,              /* 57 - NOT IN USE */
  CURLE_SSL_CERTPROBLEM,         /* 58 - problem with the local certificate */E_NET_CURL_SSL_CERT_ERROR
  CURLE_SSL_CIPHER,              /* 59 - couldn't use specified cipher */E_NET_CURL_SSL_CIPHER_ERROR
  CURLE_SSL_CACERT,              /* 60 - problem with the CA cert (path?) */E_NET_CURL_SSL_CACERT_ERROR
  CURLE_BAD_CONTENT_ENCODING,    /* 61 - Unrecognized transfer encoding */E_NET_CURL_BAD_CONTENT_ENCODING,
  CURLE_LDAP_INVALID_URL,        /* 62 - Invalid LDAP URL */E_NET_CURL_INTERNEL_ERROR
  CURLE_FILESIZE_EXCEEDED,       /* 63 - Maximum file size exceeded */E_NET_CURL_FILESIZE_EXCEEDED
  CURLE_USE_SSL_FAILED,          /* 64 - Requested FTP SSL level failed */E_NET_CURL_USE_SSL_FAILED
  CURLE_SEND_FAIL_REWIND,        /* 65 - Sending the data requires a rewind  that failed */E_NET_CURL_SEND_FAIL_REWIND
  CURLE_SSL_ENGINE_INITFAILED,   /* 66 - failed to initialise ENGINE */E_NET_CURL_SSL_ENGINE_FAIL_INIT
  CURLE_LOGIN_DENIED,            /* 67 - user, password or similar was not  accepted and we failed to login */E_NET_CURL_LOGIN_DENIED
  CURLE_TFTP_NOTFOUND,           /* 68 - file not found on server */E_NET_CURL_INTERNEL_ERROR
  CURLE_TFTP_PERM,               /* 69 - permission problem on server */E_NET_CURL_INTERNEL_ERROR
  CURLE_REMOTE_DISK_FULL,        /* 70 - out of disk space on server */E_NET_CURL_REMOTE_DISK_FULL
  CURLE_TFTP_ILLEGAL,            /* 71 - Illegal TFTP operation */E_NET_CURL_INTERNEL_ERROR
  CURLE_TFTP_UNKNOWNID,          /* 72 - Unknown transfer ID */E_NET_CURL_INTERNEL_ERROR
  CURLE_REMOTE_FILE_EXISTS,      /* 73 - File already exists */E_NET_REMOTE_FILE_EXISTS
  CURLE_TFTP_NOSUCHUSER,         /* 74 - No such user */E_NET_CURL_INTERNEL_ERROR
  CURLE_CONV_FAILED,             /* 75 - conversion failed */E_NET_CURL_INTERNEL_ERROR
  CURLE_CONV_REQD,               /* 76 - caller must register conversion
  									callbacks using curl_easy_setopt options   									
  									CURLOPT_CONV_FROM_NETWORK_FUNCTION,
  									CURLOPT_CONV_TO_NETWORK_FUNCTION, and
  									CURLOPT_CONV_FROM_UTF8_FUNCTION */E_NET_CURL_INTERNEL_ERROR

  CURLE_SSL_CACERT_BADFILE,      /* 77 - could not load CACERT file, missing  or wrong format */E_NET_CURL_SSL_CACERT_BADFILE
  CURLE_REMOTE_FILE_NOT_FOUND,   /* 78 - remote file not found */E_NET_CURL_REMOTE_FILE_NOT_FOUND
  CURLE_SSH,                     /* 79 - error from the SSH layer, somewhat
                                    			generic so the error message will be of
                                   			 interest when this has happened */E_NET_CURL_INTERNEL_ERROR
  CURLE_SSL_SHUTDOWN_FAILED,     /* 80 - Failed to shut down the SSL connection */E_NET_CURL_INTERNEL_ERROR
  //ADD BY QZHANG 
  CURLE_MV_STOPPED,				 /* 81 - receive stop cmd when process */E_NET_CURL_RECEIVE_STOP_CMD
  //tluo add   
  CURLE_MV_SEEK,                  /* 82 - receive seek cmd when process */E_NET_CURL_SEEK_SEEK_CMD

  CURLE_MV_CLOSE,                 /*  83 - when caller close the http process*/E_NET_CURL_RECEIVE_CLOSE_CMD
                
  CURL_LAST /* never use! */
} CURLcode;
#endif

//fifo error code
#define E_NET_FIFO_START                                            			 (0x0500)
#define E_NET_FIFO_FIFOISNULL												(E_NET(E_NET_FIFO_START+0x00))
#define E_NET_FIFO_CMDISNULL													(E_NET(E_NET_FIFO_START+0x01))
#define E_NET_FIFO_FIFOSIZE_OUTOF_SCOPE										(E_NET(E_NET_FIFO_START+0x02))
#define E_NET_FIFO_CANNT_ALLOCBUFF											(E_NET(E_NET_FIFO_START+0x03))
#define E_NET_FIFO_NOT_INIT													(E_NET(E_NET_FIFO_START+0x04))
#define E_NET_FIFO_FULL														(E_NET(E_NET_FIFO_START+0x05))
#define E_NET_FIFO_EMPTY														(E_NET(E_NET_FIFO_START+0x06))




#define E_NET_SSL_START						(0x0050)
#define E_NET_SSL_SERVER_AUTHENTICATION_FAIL	(E_NET(E_NET_SSL_START|0x00))


#define E_NET_CMD_START						(0x0040)
#define E_NET_CMD_NOTSUPPORT	(E_NET(E_NET_CMD_START|0x00))


#define E_NET_THREAD_START						(0x0100)
#define E_NET_CANNT_CREATE_THREAD	(E_NET(E_NET_THREAD_START|0x00))



#define E_NET_METHOD_START                    (0x0200)
#define E_NET_ERROR_UNSUPPORT_METHOD          (E_NET(E_NET_METHOD_START|0x00))
#define E_NET_ERROR_INVOKE_METHOD               (E_NET(E_NET_METHOD_START|0x01))



#define E_NET_HEADER_START					(0x0030)
#define E_NET_NO_SPECIFIED_HEADER			(E_NET(E_NET_HEADER_START|0x00))
#define E_NET_HTTPHEADER_INVALID			(E_NET(E_NET_HEADER_START|0x01))
#define E_NET_FAIL_TO_CREATE_STD_MAP		(E_NET(E_NET_HEADER_START|0x02))

#endif

