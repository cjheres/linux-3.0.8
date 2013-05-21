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

////////////////////////////////////////////////////////////////////////////////
//! \file debug.h
//! \brief headfile of Galois Software debug module
//!			
//!	
//! Purpose:
//!	   Shared the Error code of the Galois System
//!
//!	Version, Date and Author :
//!		V 1.00,	        	FengLei Wang	
//!		V 1.10,	8/13/2008,	Fang Bao	, Add dbg_OutAddKey(), dbg_OutRemoveKey() and dbg_OutGetKWL()
//!										, Now the dynamic regulation of debug trace control is available.
//!		V 1.11,	8/14/2008,	Fang Bao	, Remove E_ALREADYEXIST from dbg_OutAddKey() return value
//!		V 1.12,	8/20/2008,	Fang Bao	, Add feature - dbg_Out to log is controlled by another KWL_ToLog when ENABLE_DEBUG_TOSYSMON is defined
//!										, Add dbg_OutToLogAddKey(), dbg_OutToLogRemoveKey(), dbg_OutToLogGetKWL(), kwlistIsIn_ToLog
//!										, Add DBG_OUT_TOLOG_CONSOLE in which all trace will be saved in log file by key word
//!										, Remove all code for ENABLE_DEBUG_MEMDUMP
//!										, Remove feature - ENABLE_DEBUG_MEMDUMP
//!		V 1.13,	9/17/2008,	Fang Bao	, Remove ENABLE_DEBUG_TOSYSMON, replace it by ENABLE_SYSMON
//!		V 1.14,	12/29/2008,	Fang Bao	, move Assert, now Assert is not depended on ENABLE_DEBUG
//!		V 1.15,	1/19/2009,	Fang Bao	, remove ASSERT_SUCCEEDED, add DISABLE_ASSERT for Assert control
//!		V 1.16,	1/21/2009,	Fang Bao	, remove dbg_Log and other dbg_Log code which is not used.
//!										, remove Assert
//!										, add ENABLE_DBGOUT and debug trace macros.
//!		V 1.17, 2/19/2008,	Fang Bao
//!										, Modify MV_XXX_DBG_Info()
//!		V 1.18,	8/26/2009,	Fang Bao	, Add dbg_Out in ISR support
//!		V 1.19,	2/22/2010,	Fang Bao	, Add Android support
//!
//!		
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "com_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ENABLE_DBGOUT

// debug message output keywords
#define DBG_LV1		"[LEVEL1]"
#define DBG_LV2		"[LEVEL2]"
#define DBG_LV3		"[LEVEL3]"
#define DBG_LV4		"[LEVEL4]"
#define DBG_LV5		"[LEVEL5]"
#define DBG_AUDIO	"[AUDIO]"
#define DBG_VIDEO	"[VIDEO]"
#define DBG_NAV		"[NAV]"
#define DBG_OSAL	"[OSAL]"
#define DBG_CC		"[CC]"
#define DBG_DVDSPU	"[DVDSPU]"
#define DBG_PEAGENT	"[PEAGENT]"
#define DBG_VDM     "[VDM]"
#define DBG_VBUF    "[VBUF]"
#define DBG_GFX 	"[GFX]"
#define DBG_MON		"[MON]"
#define	DBG_PE		"[PE]"
#define	DBG_ADM		"[ADM]"
#define	DBG_VDM		"[VDM]"
#define	DBG_DC		"[DC]"
#define	DBG_DRM		"[DRM]"
#define	DBG_DMX		"[DMX]"
#define	DBG_LOG		"[LOG]"

HRESULT dbg_CoreInit( void );
BOOL dbg_CoreIsInited( void );
HRESULT dbg_CoreDeinit( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    dbg_Out
//!
//! Description: ouput the debug trace in debug trace control
//!
//! Work Flow  :  
//!
//!
//! \return Return:		None
//!
//!						
////////////////////////////////////////////////////////////////////////////////
void dbg_Out( CHAR* fmt, ... );

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    dbg_OutInISR
//!
//! Description: (In ISR procedure)ouput the debug trace in debug trace control
//!
//! Work Flow  :  It is only available in eCos OS when system monitor is opened.
//!
//!
//! \return Return:		None
//!
//!						
////////////////////////////////////////////////////////////////////////////////
void dbg_OutInISR( CHAR* fmt, ... );

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    dbg_Out_ISRBuf_Transfer
//!
//! Description: Transfer the trace in ISR buffer to dbg_Out buffer
//!
//! Work Flow  :  It is called by system monitor periodically.
//!
//!
//! \return Return:		the byte size transferred
//!						E_FAIL
//!						
////////////////////////////////////////////////////////////////////////////////
HRESULT dbg_Out_ISRBuf_Transfer();

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    dbg_OutGetKWL
//!
//! Description: Get the pointer to the key string of debug trace control
//!
//! Work Flow  :  
//!
//!
//! \return Return:		the pointer to the key string of debug trace control
//!
//!						
////////////////////////////////////////////////////////////////////////////////
CHAR *dbg_OutGetKWL( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    dbg_OutRemoveKey
//!
//! Description: Remove the key from the key string of debug trace control
//!
//! Work Flow  :  
//!
//!
//! \return Return:		S_OK		|	OK
//! 					E_NOTEXIST	|	the key does not exist
//!
//!						
////////////////////////////////////////////////////////////////////////////////
HRESULT dbg_OutRemoveKey( CHAR* pcKWL );

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    dbg_OutAddKey
//!
//! Description: add the key into the key string of debug trace control
//!
//! Work Flow  :  
//!
//!
//! \return Return:		S_OK			|	OK
//! 					E_ALREADYEXIST	|	the key has already existed
//! 					E_FULL			|	the key string is full, please modify the DBG_OUT_CONSOLE_KWLEN_MAX
//!
//!						
////////////////////////////////////////////////////////////////////////////////
HRESULT dbg_OutAddKey( CHAR* pcKWL );

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    dbg_Out_SetLevel
//!
//! Description: Set level of debug trace control to screen
//!
//! \param uiLevel 	(IN): -- 
//!				 0 = Level (1 - 5) is disabled in debug trace control
//!				 1 = Level (1) is enabled in debug trace control
//!				 2 = Level (1 - 2) is enabled in debug trace control
//!				 3 = Level (1 - 3) is enabled in debug trace control
//!				 4 = Level (1 - 4) is enabled in debug trace control
//!				 5 = Level (1 - 5) is enabled in debug trace control
//!
//! Work Flow  :  
//!
//!
//! \return Return:		S_OK			|	OK
//! 					E_INVALIDARG	|	the parameter is out of range ( 0 - 5 )
//!
//!						
////////////////////////////////////////////////////////////////////////////////
HRESULT dbg_Out_SetLevel(UINT uiLevel);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    dbg_Out_GetLevel
//!
//! Description: Get level of debug trace control to screen
//!
//! \param ptr_uiLevel 	(OUT): -- 
//!				 0 = Level (1 - 5) is disabled in debug trace control
//!				 1 = Level (1) is enabled in debug trace control
//!				 2 = Level (1 - 2) is enabled in debug trace control
//!				 3 = Level (1 - 3) is enabled in debug trace control
//!				 4 = Level (1 - 4) is enabled in debug trace control
//!				 5 = Level (1 - 5) is enabled in debug trace control
//!
//!
//! \return Return:		S_OK			|	OK
//! 					E_INVALIDARG	|	the pointer is NULL
//!
//!						
////////////////////////////////////////////////////////////////////////////////
HRESULT dbg_Out_GetLevel(UINT *ptr_uiLevel);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    dbg_OutToLogGetKWL
//!
//! Description: Get the pointer to the key string of debug trace control to log
//!
//! Work Flow  :  
//!
//!
//! \return Return:		the pointer to the key string of debug trace control
//!
//!						
////////////////////////////////////////////////////////////////////////////////
CHAR *dbg_OutToLogGetKWL( void );

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    dbg_OutToLogRemoveKey
//!
//! Description: Remove the key from the key string of debug trace control to log
//!
//! Work Flow  :  
//!
//!
//! \return Return:		S_OK		|	OK
//! 					E_NOTEXIST	|	the key does not exist
//!
//!						
////////////////////////////////////////////////////////////////////////////////
HRESULT dbg_OutToLogRemoveKey( CHAR* pcKWL );

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    dbg_OutToLogAddKey
//!
//! Description: add the key into the key string of debug trace control to log
//!
//! Work Flow  :  
//!
//!
//! \return Return:		S_OK			|	OK
//! 					E_ALREADYEXIST	|	the key has already existed
//! 					E_FULL			|	the key string is full, please modify the DBG_OUT_CONSOLE_KWLEN_MAX
//!
//!						
////////////////////////////////////////////////////////////////////////////////
HRESULT dbg_OutToLogAddKey( CHAR* pcKWL );

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    dbg_OutToLog_SetLevel
//!
//! Description: Set level of debug trace control to log
//!
//! \param uiLevel 	(IN): -- 
//!				 0 = Level (1 - 5) is disabled in debug trace control
//!				 1 = Level (1) is enabled in debug trace control
//!				 2 = Level (1 - 2) is enabled in debug trace control
//!				 3 = Level (1 - 3) is enabled in debug trace control
//!				 4 = Level (1 - 4) is enabled in debug trace control
//!				 5 = Level (1 - 5) is enabled in debug trace control
//!
//!
//! \return Return:		S_OK			|	OK
//! 					E_INVALIDARG	|	the parameter is out of range ( 0 - 5 )
//!
//!						
////////////////////////////////////////////////////////////////////////////////
HRESULT dbg_OutToLog_SetLevel(UINT uiLevel);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    dbg_OutToLog_GetLevel
//!
//! Description: Get level of debug trace control to log
//!
//! \param ptr_uiLevel 	(IN): -- 
//!				 0 = Level (1 - 5) is disabled in debug trace control
//!				 1 = Level (1) is enabled in debug trace control
//!				 2 = Level (1 - 2) is enabled in debug trace control
//!				 3 = Level (1 - 3) is enabled in debug trace control
//!				 4 = Level (1 - 4) is enabled in debug trace control
//!				 5 = Level (1 - 5) is enabled in debug trace control
//!
//!
//! \return Return:		S_OK			|	OK
//! 					E_INVALIDARG	|	the pointer is NULL
//!
//!						
////////////////////////////////////////////////////////////////////////////////
HRESULT dbg_OutToLog_GetLevel(UINT *ptr_uiLevel);

	#ifdef ENABLE_SYSMON
	
	void dbg_OutToLog_Start(HANDLE hLogBuf);
	
	void dbg_OutToLog_Stop();
	
	#else
	
	#define dbg_OutToLog_Start( ... )
	
	#define dbg_OutToLog_Stop( ... )
	
	#endif

#else
#define dbg_CoreInit()					S_OK
#define dbg_CoreIsInited()				S_FALSE
#define dbg_CoreDeinit()				S_OK

#define dbg_Out( ... )
#define dbg_OutInISR( ... )	
#define dbg_Out_ISRBuf_Transfer()		S_OK
#define dbg_OutGetKWL( ... ) 			NULL
#define dbg_OutRemoveKey( ... )			S_OK
#define dbg_OutAddKey( ... )    		S_OK
#define dbg_OutToLogGetKWL( ... ) 		NULL
#define dbg_OutToLogRemoveKey( ... )	S_OK
#define dbg_OutToLogAddKey( ... )    	S_OK

#define dbg_OutToLog_Start( ... )
#define dbg_OutToLog_Stop( ... )

#define dbg_Out_SetLevel( ... )			S_OK
#define dbg_Out_GetLevel( ... )			S_OK

#define dbg_OutToLog_SetLevel( ... )	S_OK
#define dbg_OutToLog_GetLevel( ... )	S_OK

#endif	// ENABLE_DBGOUT

#ifdef ANDROID
void dbg_Out_Log( CHAR* fmt, ... );
#else
#define dbg_Out_Log( ... )
#endif

////////////////////////////////////////////////////////////////////////////////
//	ASSERT defination

#if defined( __LINUX__ )
void os_dump_stack();
#endif
void t_Assert( const CHAR* pucFile, UINT uiLine, UINT bIn );

#ifdef DISABLE_ASSERT
#define MV_ASSERT( a )
#else
#ifdef __LINUX_KERNEL__
#define MV_ASSERT( a )					BUG_ON(!(a))
#else
#define MV_ASSERT( a )					t_Assert( __FILE__, __LINE__, (UINT)( a ) )
#endif
#endif

typedef struct os_task_sched_param
{
    int policy;
    int priority;
}task_sched_param_t;

#if defined( __LINUX__ )
int os_set_task_prio(task_sched_param_t *new_param, task_sched_param_t *old_param);
CHAR* os_get_systime_string();

#else
__inline static int os_set_task_prio(task_sched_param_t *new_param, task_sched_param_t *old_param)
{return 0;}
__inline static CHAR* os_get_systime_string() {return NULL;}
#endif

////////////////////////////////////////////////////////////////////////////////
//	debug trace defination
//
//	The parameter s = the string of debug trace
//	The parameter x = the error code should be set for warning or error

#ifdef ENABLE_DBGOUT

	// Module = Navigator

	#define	MV_NAV_DBG_Info(...)				dbg_Out(DBG_NAV DBG_LV5 " " __VA_ARGS__)
	#define	MV_NAV_DBG_Warning(x, s, ...)		dbg_Out(DBG_NAV DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_NAV_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_NAV DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}
	// Module = Audio

	#define	MV_AUDIO_DBG_Info(...)				dbg_Out(DBG_AUDIO DBG_LV5 " " __VA_ARGS__)
	#define	MV_AUDIO_DBG_Warning(x, s, ...)		dbg_Out(DBG_AUDIO DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_AUDIO_DBG_Error(x, s, ...)		{ \
												dbg_Out(DBG_AUDIO DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}

	// Module = Video

	#define	MV_VIDEO_DBG_Info(...)				dbg_Out(DBG_VIDEO DBG_LV5 " " __VA_ARGS__)
	#define	MV_VIDEO_DBG_Warning(x, s, ...)		dbg_Out(DBG_VIDEO DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_VIDEO_DBG_Error(x, s, ...)		{ \
												dbg_Out(DBG_VIDEO DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}

	// Module = ADM

	#define	MV_ADM_DBG_Info(...)				dbg_Out(DBG_ADM DBG_LV5 " " __VA_ARGS__)
	#define	MV_ADM_DBG_Warning(x, s, ...)		dbg_Out(DBG_ADM DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_ADM_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_ADM DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}

	// Module = VDM

	#define	MV_VDM_DBG_Info(...)				dbg_Out(DBG_VDM DBG_LV5 " " __VA_ARGS__)
	#define	MV_VDM_DBG_Warning(x, s, ...)		dbg_Out(DBG_VDM DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_VDM_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_VDM DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}

	// Module = GFX

	#define	MV_GFX_DBG_Info(...)				dbg_Out(DBG_GFX DBG_LV5 " " __VA_ARGS__)
	#define	MV_GFX_DBG_Warning(x, s, ...)		dbg_Out(DBG_GFX DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_GFX_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_GFX DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}

	// Module = DC

	#define	MV_DC_DBG_Info(...)					dbg_Out(DBG_DC DBG_LV5 " " __VA_ARGS__)
	#define	MV_DC_DBG_Warning(x, s, ...)		dbg_Out(DBG_DC DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_DC_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_DC DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}
	// Module = DRM

	#define	MV_DRM_DBG_Info(...)				dbg_Out(DBG_DRM DBG_LV5 " " __VA_ARGS__)
	#define	MV_DRM_DBG_Warning(x, s, ...)		dbg_Out(DBG_DRM DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_DRM_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_DRM DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}
	// Module = DMX

	#define	MV_DMX_DBG_Info(...)				dbg_Out(DBG_DMX DBG_LV5 " " __VA_ARGS__)
	#define	MV_DMX_DBG_Warning(x, s, ...)		dbg_Out(DBG_DMX DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
	#define	MV_DMX_DBG_Error(x, s, ...)			{ \
												dbg_Out(DBG_DMX DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); \
												MV_ASSERT(0); \
												}
#else

	// Module = Navigator

	#define	MV_NAV_DBG_Info(...)				
	#define	MV_NAV_DBG_Warning(x, s, ...)		
	#define	MV_NAV_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }
	
	// Module = Audio

	#define	MV_AUDIO_DBG_Info(...)				
	#define	MV_AUDIO_DBG_Warning(x, s, ...)		
	#define	MV_AUDIO_DBG_Error(x, s, ...)		{ MV_ASSERT(x == S_OK); }

	// Module = Video

	#define	MV_VIDEO_DBG_Info(...)			
	#define	MV_VIDEO_DBG_Warning(x, s, ...)		
	#define	MV_VIDEO_DBG_Error(x, s, ...)		{ MV_ASSERT(x == S_OK); }	

	// Module = ADM

	#define	MV_ADM_DBG_Info(...)			
	#define	MV_ADM_DBG_Warning(x, s, ...)		
	#define	MV_ADM_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }	

	// Module = VDM

	#define	MV_VDM_DBG_Info(...)			
	#define	MV_VDM_DBG_Warning(x, s, ...)		
	#define	MV_VDM_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }	

	// Module = GFX

	#define	MV_GFX_DBG_Info(...)			
	#define	MV_GFX_DBG_Warning(x, s, ...)		
	#define	MV_GFX_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }	

	// Module = DC

	#define	MV_DC_DBG_Info(...)			
	#define	MV_DC_DBG_Warning(x, s, ...)		
	#define	MV_DC_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }	

	// Module = DRM

	#define	MV_DRM_DBG_Info(...)			
	#define	MV_DRM_DBG_Warning(x, s, ...)		
	#define	MV_DRM_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }	
	
	// Module = DMX

	#define	MV_DMX_DBG_Info(...)			
	#define	MV_DMX_DBG_Warning(x, s, ...)		
	#define	MV_DMX_DBG_Error(x, s, ...)			{ MV_ASSERT(x == S_OK); }	
	
		
#endif	// ENABLE_DBGOUT

#define DBG_MAX_TASK_COUNT	64
typedef struct
{
	INT		m_iID;
	INT		m_iPriority;
	INT64	m_iPeriod;
	INT		m_iSwitches_all;
	INT		m_iSwitches_period;
} tTask_Data;

typedef struct
{
	INT		m_iTask_Cnt;
	BOOL	m_bTotal_Task_Number_Overflow;
	INT64	m_iPeriodStartTime;
	INT64	m_iPeriodEndTime;
	tTask_Data	m_Task_Data[DBG_MAX_TASK_COUNT];
} tTask_Statistics;

extern tTask_Statistics Task_Stat;

HRESULT MV_DBG_CPU_Loading_Start_Count( void);
HRESULT MV_DBG_CPU_Loading_Stop_Count( void );
HRESULT MV_DBG_CPU_Loading_Output( void );
HRESULT MV_DBG_CPU_Loading_Stop_Count_And_Output( void );

#ifdef __cplusplus
}
#endif

#endif	// _DEBUG_H_

