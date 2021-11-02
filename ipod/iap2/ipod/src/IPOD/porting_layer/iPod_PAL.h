/****************************************************************************
 *   FileName    : iPod_PAL.h
 *   Description :
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-
distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall
constitute any express or implied warranty of any kind, including without limitation, any warranty
of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright
or other third party intellectual property right. No warranty is made, express or implied,
regarding the information's accuracy, completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of
or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************/


#ifndef _IPOD_PAL_H_
#define _IPOD_PAL_H_

#if defined(__cplusplus)
extern "C"
{
#endif
	/*
		OS
	*/
#include "../iPod_error.h"
#include "../iPod_lib.h"

#define IPOD_CPBUFSIZE (50*1024)
//#define IPOD_RXBUFSIZE ((128*128*2)+1024)
//#define IPOD_RXBUFSIZE ((240*240*2)+1024)
#define IPOD_RXBUFSIZE ((240*240*2)*10+1024)

#define IPOD_TXBUFSIZE 2048

#define USBH_AUDIO_PLAYBUFSIZE 0



#define IPOD_TOTALBUFSIZE	(IPOD_CPBUFSIZE+IPOD_TXBUFSIZE+(IPOD_RXBUFSIZE*2)+IPOD_UTF8DECODE_STRINGBUFSIZE+USBH_AUDIO_PLAYBUFSIZE)
#define IPOD_SYNC 0xFF					//packet sync flag
#define IPOD_START 0x55					//packet start flag

#define IPOD_TIMEOUT_MOUNT_VALUE 4 

#define IPOD_TIMEOUT_VALUE 10 // do not fix. 10 = 50ms (needed 5 times in IPOD_Callback_GetWaitTime)

// command timeout time, 1500(ms) = IPOD_TIMEOUT_VALUE(10*5) * IPOD_COMMAND_TIMEOUT_VALUE(30)
// IPOD_COMMAND_TIMEOUT_VALUE 30 : 1.5s
// IPOD_COMMAND_TIMEOUT_VALUE 60 : 3s
#define IPOD_COMMAND_TIMEOUT_VALUE 60 

#define IPOD_AUTHPROCESS_RESPONSE_TIMEOUT_S 75//unit (s), this value must set to 20(s) over.
 
#ifdef IPOD_KEEP_REPEAT_RANDOM
#define IPOD_RESTORE_ON_EXIT	UPDATE_NEW_SETTING
#else
#define IPOD_RESTORE_ON_EXIT	RESTORE_ORG_SETTING
#endif

#define IPOD_1S_TIME_CHECK 1000

typedef enum
{
	IPOD_ALERT = 0x00000001,
	IPOD_ALERT1 = 0x00000002,
	IPOD_LOG 	= 0x00000010,

	IPOD_TYPE_MAX    = 0x7fffffff
} IPOD_LOG_TYPE;

typedef enum
{
	IPOD_LINGO_GEN = 0x00000001,
	IPOD_LINGO_EXT = 0x00000002,
	IPOD_LINGO_DAUDIO = 0x00000004,
	IPOD_LINGO_SIMPLE = 0x00000008,
	IPOD_LINGO_DISPLAYR = 0x00000010,
	IPOD_LINGO_IPODOUT = 0x00000020,

	IPOD_AUTH = 0x00000200,
	IPOD_DATAPASING = 0x00000400,
	IPOD_INTERFACE = 0x00000800,
	IPOD_INTERFACE_OS = 0x00001000,
	IPOD_CP = 0x00002000,
	IPOD_CP_IO = 0x00004000,

	IPOD_CP_FUNC = 0x00008000,
	IPOD_CMD_TASK = 0x00010000,
	IPOD_CMD_TASK_MSG = 0x00020000,

	IPOD_HID_IO = 0x00040000,

	IPOD_TIMEOUT_CNT = 0x00080000,
	IPOD_ACCESSORY_INFO = 0x00100000,
	IPOD_EVENT_LOG		= 0x00200000,

	IPOD_OS_APP_LOG		= 0x00400000,
	IPOD_INIT_PROCESS_LOG = 0x00800000,

	IPOD_AUTH_INFO		= 0x01000000,
	IPOD_OS_RES_LOG = 0x02000000,
	IPOD_NULL_CHECK_LOG = 0x04000000,
	
	IPOD_PART_MAX = 0x7fffffff
} IPOD_LOG_PART;

    extern void IPOD_PAL_SetInitializingFlag(int flag);
    extern void IPOD_PAL_InitDebug(void);
    extern int IPOD_PAL_GetTotalBufSize(void);
	extern unsigned long IPOD_PAL_SetCommandTimeout(void* iPod_Handle, unsigned long Lingo, unsigned long SendCmd, unsigned long TimeOutValue);
	extern void IPOD_PAL_UsbAudioSetFullBandwidthInterface(unsigned long DevNo);
	extern void IPOD_PAL_UsbAudioSetCur(unsigned long DevNo, long bitrate);


	extern void IPOD_PAL_EventExtendedInterfaceLingo(void *iPod_Handle, char *pCmdData);
	extern void IPOD_PAL_EventGeneralLingo(void *iPod_Handle, unsigned char EventNum, char *pCmdData, unsigned long length);
	extern void IPOD_PAL_EventDisplayRemoteLingo(void *iPod_Handle, unsigned char EventNum, char *pCmdData);
	extern void IPOD_PAL_EventDigitalAudioLingo(void *iPod_Handle, unsigned long bitrate, void *hEventQueueWrite, IPOD_EVENT_ID EventID);

	extern unsigned long IPOD_Callback_GetWaitTimeMount(void);
	extern unsigned long IPOD_Callback_GetWaitTime(void);
	extern unsigned long IPOD_Callback_DefaultTimeout(void);
	extern unsigned long IPOD_Callback_AuthProcess_NoResponseTimeout(void);
	
	extern void IPOD_SetDisconnectionStatus(void *Handle);
	extern IPOD_PORT_STATE IPOD_GetDetectStatus(void *ExtInfo);

	extern unsigned long IPOD_Get_Oneword(const unsigned char *Data);
	extern unsigned short IPOD_Get_Short(const unsigned char *Data);

	extern void IPOD_PrintLog(unsigned char *Log);
	extern void IPOD_AlertLog(unsigned char *Log);
	extern void IPOD_AlertLog_type1(unsigned char *Log);
	extern void IPOD_optionLog(IPOD_LOG_TYPE Type, IPOD_LOG_PART Part, unsigned char *format, ...);
    extern unsigned long IPOD_GetStringBufferSize(void);
	extern unsigned long IPOD_PAL_CancelIDPSCount(void);

#if defined(__cplusplus)
};
#endif
#endif	/* _IPOD_PAL_H_ */

