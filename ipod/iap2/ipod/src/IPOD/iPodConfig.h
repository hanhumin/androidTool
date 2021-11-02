/****************************************************************************
 *   FileName    : iPodConfig.h
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


#ifndef _IPODCONFIG_H_
#define _IPODCONFIG_H_


/* 'Select all' item added to browser list*/
#define BRWS_SELECTALL_INCLUDE

/* TEST FUNCTION */
//#define TIME_CHECK
#define TEST_CLEAR_BUFFER


//#define TEST_TERMINATE_THREAD

/* USER MESSAGE ID */

#define WM_MESSAGE_DETECTHID		0x7999		// Set this ID in .reg of HID device driver
// [HKEY_LOCAL_MACHINE\Drivers\USB\ClientDrivers\TCCHIDClient]
//	"ConnectionEventMsgID"=dword:7999			; WM_USER+xxx
#define WM_MESSAGE_DETECTAUDIO		0x7998		// Set this ID in .reg of Audio Class device driver

#define WM_APP (0x0)

// USER MESSAGE ID
#define WM_UI_MESSAGE_BASE	(WM_APP+0x0100)

#define WM_CMDCTRL_MSGBASE	(WM_APP+0x0200)

#define WM_EVENTCTRL_MSGBASE	(WM_APP+0x0400)

#define WM_IPODAUDIO_MSGBASE	(WM_APP+0x0800)

#define WM_IPOD_EX_MSGBASE      (WM_APP+0x1000)

typedef enum
{
	WM_UI_UPDATE_CURRENTTRACKINFO =	WM_UI_MESSAGE_BASE,
	WM_UI_UPDATE_CURRENTTRACKNUM,
	WM_UI_UPDATE_TRACKPLAYBACKPOSITION,
	WM_UI_UPDATE_FFSEEKSTOP,
	WM_UI_UPDATE_REWSEEKSTOP,
	WM_UI_UPDATE_SAMPLINGFREQUENCE,
	WM_UI_UPDATE_NONE,

	WM_UI_TIMEOUT_STOPPING_CMDTHREAD,
	WM_UI_MESSAGE_MAX = WM_CMDCTRL_MSGBASE
} WM_UI_MSGID;

typedef enum
{
	WM_CMDCTRL_RES_SUCCESS = WM_CMDCTRL_MSGBASE,
	WM_CMDCTRL_RES_FAIL,

	WM_CMDCTRL_INIT,
	WM_CMDCTRL_SEND_HANDLE,
	WM_CMDCTRL_RES_INIT,
	WM_CMDCTRL_STOP,
	WM_CMDCTRL_RES_STOP,
	WM_CMDCTRL_TERMINATE,
	WM_CMDCTRL_RES_TERMINATE,
	WM_CMDCTRL_CANCELCMD,
	WM_CMDCTRL_RES_CANCELCMD,

	WM_CMDCTRL_PLAY,
	WM_CMDCTRL_PLAY_IN_BRWSLIST,
	WM_CMDCTRL_RES_PLAY,
	WM_CMDCTRL_PAUSE,
	WM_CMDCTRL_RES_PAUSE,
	WM_CMDCTRL_NEXTTRACK,
	WM_CMDCTRL_RES_NEXTTRACK,
	WM_CMDCTRL_PREVIOUSTRACK,
	WM_CMDCTRL_RES_PREVIOUSTRACK,
	WM_CMDCTRL_FF,
	WM_CMDCTRL_RES_FF,
	WM_CMDCTRL_REW,
	WM_CMDCTRL_RES_REW,

	WM_CMDCTRL_GETCURRENTTRACKINDEX,
	WM_CMDCTRL_RES_GETCURRENTTRACKINDEX,
	WM_CMDCTRL_GETTOTALPLAYTRACKS,
	WM_CMDCTRL_RES_GETTOTALPLAYTRACKS,
	WM_CMDCTRL_GETTITLE,
	WM_CMDCTRL_RES_GETTITLE,
	WM_CMDCTRL_GETARTIST,
	WM_CMDCTRL_RES_GETARTIST,
	WM_CMDCTRL_GETALBUM,
	WM_CMDCTRL_RES_GETALBUM,
	WM_CMDCTRL_GETALBUMART,
	WM_CMDCTRL_RES_GETALBUMART,
	WM_CMDCTRL_INFORM_SAMPLINGFREQ_CHECKED,
	WM_CMDCTRL_RES_INFORM_SAMPLINGFREQ_CHECKED,

	WM_CMDCTRL_GET_DEVICEDISPLAYINFO,
	WM_CMDCTRL_RES_DEVICEDISPLAYINFO,
	WM_CMDCTRL_SET_DEVICEDISPLAYIMAGE,
	WM_CMDCTRL_RES_DEVICEDISPLAYIMAGE,

	WM_CMDCTRL_DATABASE_INIT,
	WM_CMDCTRL_DATABASE_RES_INIT,
	WM_CMDCTRL_DATABASE_SELECTALL,
	WM_CMDCTRL_DATABASE_RES_SELECTALL,
	WM_CMDCTRL_DATABASE_SELECT,
	WM_CMDCTRL_DATABASE_RES_SELECT,
	WM_CMDCTRL_DATABASE_MOVEUPPER,
	WM_CMDCTRL_DATABASE_RES_MOVEUPPER,
	WM_CMDCTRL_DATABASE_GETNAME,
	WM_CMDCTRL_DATABASE_RES_GETNAME,

	WM_CMDCTRL_EXT_FUNC1,
	WM_CMDCTRL_RES_EXT_FUNC1,
	WM_CMDCTRL_EXT_FUNC2,
	WM_CMDCTRL_RES_EXT_FUNC2,
	WM_CMDCTRL_EXT_FUNC3,
	WM_CMDCTRL_RES_EXT_FUNC3,
	WM_CMDCTRL_EXT_FUNC4,
	WM_CMDCTRL_RES_EXT_FUNC4,
	WM_CMDCTRL_EXT_FUNC5,
	WM_CMDCTRL_RES_EXT_FUNC5,

	WM_CMDCTRL_MAX = WM_EVENTCTRL_MSGBASE
} WM_CMDCTRL_MSGID;


typedef enum
{
	WM_DISCNTDETECT_RES_SUCCESS = WM_CMDCTRL_MSGBASE,
	WM_DISCNTDETECT_RES_FAIL,

	WM_DISCNTDETECT_INIT,
	WM_DISCNTDETECT_TERMINATE,

	WM_DISCNTDETECT_EVENT,

	WM_DISCNTDETECT_MAX = WM_EVENTCTRL_MSGBASE

} WM_DISCNTDETECT_MSGID;

typedef enum
{
	WM_EVENTCTRL_TERMINATE =	WM_EVENTCTRL_MSGBASE,

	WM_EVENTCTRL_MAX = WM_IPODAUDIO_MSGBASE
} WM_EVENTCTRL_MSGID;

typedef enum
{
	WM_AUDIOCTRL_SET_FULLBAND_WIDTH =	WM_IPODAUDIO_MSGBASE,

	WM_AUDIOCTRL_MAX = WM_IPOD_EX_MSGBASE
} WM_AUDIOCTRL_MSGID;
// MACRO
#define IS_FAIL(x)	(x<0)
#define IS_SUCCESS(x) ( !IS_FAIL(x) )

#define IS_NULL(x) 	((void*)(x) == (void*)0)

//
#define UI_MAX_STRING_LEN	(1024)
#define IPOD_I2C_CH (0)

#define IPOD_EXT_LINGO_SUPPORT 0x00
#define IPOD_DR_LINGO_SUPPORT 0x01

// LOG
#define LV_NONE  0x00000000
#define LV_ERROR  0x00000001
#define LV_WARN   0x00000002
#define LV_LOG      0x00000004
#define LV_DEBUG  0x00000008
#define LV_TIME  0x00000010

//#define TLOG_LEVEL  (LV_ERROR|LV_WARN|LV_LOG|LV_DEBUG)
#define TLOG_LEVEL  (LV_ERROR|LV_WARN)
//#define TLOG_LEVEL  (LV_ERROR|LV_WARN)

#define CH_NONE				0x00000000
#define CH_UI  				0x00010000
#define CH_CMDCTL  			0x00020000
#define CH_EVTCTL  			0x00040000
#define CH_AUDCTL  			0x00080000
#define CH_DISCNTCTL		0x00100000
#define CH_INITCTL			0x00200000
#define CH_EXT_FUNC			0x00400000
#define CH_ALL  	0xFFFF0000

#define TLOG_CHANNEL (CH_UI|CH_CMDCTL|CH_EVTCTL|CH_AUDCTL|CH_DISCNTCTL)
//#define TLOG_CHANNEL (CH_UI|CH_CMDCTL|CH_AUDCTL)

#define L_CFG(channel, level)  (((TLOG_CHANNEL)&(channel))&&((TLOG_LEVEL)&(level)))


#define L_CHANNEL_SIGN(channel)	((channel&CH_UI) ? L"UI": ((channel&CH_CMDCTL) ? L"CMD": ((channel&CH_EVTCTL)? L"EVT": ((channel&CH_AUDCTL) ? L"AUD":L""))))
//#define L_LEVEL_SIGN(level)	((level&LV_ERROR) ? L"[E]": ((level&LV_WARN) ? L"[W]": ((level&LV_LOG)? L"[L]": ((level&LV_DEBUG) ? L"[D]":L""))))
#define L_LEVEL_SIGN(level)	((level&LV_ERROR) ? L"[E]": ((level&LV_WARN) ? L"[W]": ((level&LV_LOG)? L"[L]": ((level&LV_DEBUG) ? L"[D]":(level&LV_TIME)?L"[TIME]":L""))))
#if 0
#define TLOG(channel, level, string)	RETAILMSG(L_CFG(channel,level), (TEXT("[IPOD PLAYER | %s ]%s "),L_CHANNEL_SIGN(channel), L_LEVEL_SIGN(level)));	\
	RETAILMSG(L_CFG(channel,level), string)
#endif
/////////////////////////////////////////////////////////////////////////////////////////////
//
//  IPOD LIBRARY
//
#include "iPod_error.h"
#include "iPod_sample.h"

#endif

