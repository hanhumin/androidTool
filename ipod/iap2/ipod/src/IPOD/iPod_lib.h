/****************************************************************************
 *   FileName    : iPod_lib.h
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


#ifndef _IPOD_LIB_H_
#define _IPOD_LIB_H_

#include "iPod_error.h"
#if defined(__cplusplus)
extern "C"
{
#endif


	/*************************************************************************/
	/*Lingo ID define                                                                                                      */
	/*************************************************************************/
	typedef unsigned char		SLingoType;

#define SL_GENERAL					0x00
#define SL_MICROPHONE				0x01
#define SL_SIMPLE_REMOTE			0x02
#define SL_DISPLAY_REMOTE			0x03
#define SL_REMOTE_UI_EXTENDED		0x04

	typedef unsigned int		MLingoType;

#define ML_GENERAL 					0x0001
#define ML_MICROPHONE				0x0002
#define ML_SIMPLE_REMOTE 			0x0004
#define ML_DISPLAY_REMOTE			0x0008
#define ML_REMOTE_UI_EXTENDED		0x0010
#define ML_ACCESSORY_POWER		0x0020
#define ML_USB_HOST_CONTROL		0x0040
#define ML_RF_TRANSMITTER			0x0080
#define ML_ACCESSORY_EQ			0x0100
#define ML_DIGITAL_AUDIO			0x0400
#define ML_STORAGE					0x1000

	typedef enum
	{
		LINGO_GENERAL					= 0x00U,
		LINGO_MICROPHONE				= 0x01U,
		LINGO_SIMPLE_REMOTE			= 0x02U,
		LINGO_DISPLAY_REMOTE			= 0x03U,
		LINGO_EXTENDED_INTERFACE		= 0x04U,
		LINGO_ACCESSORY_POWER		= 0x05U,
		LINGO_USB_HOST_CONTROL		= 0x06U,
		LINGO_RF_TUNER				= 0x07U,
		LINGO_ACCESSORY_EQUALIZER	= 0x08U,
		LINGO_SPORTS					= 0x09U,
		LINGO_DIGITAL_AUDIO			= 0x0AU,
		LINGO_STORAGE					= 0x0CU,
		LINGO_IPOD_OUT				=0x0DU,
		LINGO_LOCATION				= 0x0EU,
		LINGO_MAX						= 0x0FU
	} IPOD_LINGOS;

	/*************************************************************************/
	/* Common Define and Enum												 */
	/*************************************************************************/


// Connection mode iAP over USB
	typedef enum{
		TWO_WIRE_CONNECTION,
		ONE_WIRE_CONNECTION,
		CONNECTION_MODE_MAX
	} IAP_CONNECTION_MODE;

	typedef enum
	{
		IPOD_DISCONNECTED,
		IPOD_CONNECTED
	} IPOD_PORT_STATE;

	typedef enum
	{
		IPOD_ANALOG_AUDIO,
		IPOD_DIGITAL_AUDIO
	} IPOD_INIT_TYPE;

	typedef enum
	{
		IPOD_USB_CONNECT    = 0x00000001,
		IPOD_BT_CONNECT     = 0x00000002,
		IPOD_MAX_CONNECT    = 0x7FFFFFFF
	} IPOD_CONNECTION_TYPE;

	typedef enum
	{
		IPOD_RX_INTERRUPT, 	/*Nucleus*/
		IPOD_RX_POLLING		/*Andorid,WinCE*/
	} IPOD_RX_TYPE;
	
	typedef enum
	{
		IPOD_IDPS_NOT_SUPPORT = 0x00000000,
		IPOD_IDPS_SUPPORT = 0x00000001,
		IPOD_MAX_DEFAULT_CONNECTION = 0x7FFFFFFF
	} IPOD_DEFAULT_CONNECTION;


// This power is from iPod
#define LOW_POWER_ONLY (0x00<<2)
#define INTERMITTENT_HIGH_POWER (0x01<<2)
#define CONSTANT_HIGH_POWER (0x03<<2)

#define NO_AUTHENTICATION 0x00
#define AUTHENTICATION_IMMEDIATELY 0x02

#define IPOD_MAX_LINGO_LEN 20

#define IPOD_FUNC_ENABLE 0x01
#define IPOD_FUNC_DISABLE 0x00

#define INTERNAL_EX_FLAG 0x01
#define TRANSID_IGNOR_EX_FLAG 0x02

#define IPOD_SEND_ORG 0x00
#define IPOD_SEND_ADD_HEADER 0x01
#define IPOD_SEND_SKIP 0x02

#define IPOD_TIMEOUT_FLAG_USE_ORIGINAL_TIMEOUT_VAL 0x01

#define IPOD_RECORD_SIZE_MAX 500 //Do not use.
#define IPOD_UTF8DECODE_STRINGBUFSIZE (1024)

#define IPOD_MAX_ARTWORK	6

	typedef enum
	{
		IPOD_AUTH_ING,
		IPOD_AUTH_FINISHED
	} IPOD_AUTH_STATUS, *pIPOD_AUTH_STATUS;

	typedef enum
	{
		IPOD_DATA_SINGLE_SESSION,

		IPOD_DATA_MULTI_SESSION,
		IPOD_DATA_MULTI_SESSION_END,

		IPOD_MULTI_RESPONSE,
		IPOD_MULTI_RESPONSE_END,

		IPOD_DATA_IDLE
	} IPOD_DATA_STATUS;

	typedef enum
	{
		ONCE_EXECUTED_IDENTIFYDEVICELINGOES,
		TWICE_EXECUTED_IDENTIFYDEVICELINGOES
	} IPOD_IDENTIFYDEVICELINGOES_STATUS;

	typedef enum
	{
		IPOD_SUPPORT_AUDIO = (0x01<<0),
		IPOD_SUPPORT_VIDEO = (0x01<<1),
		IPOD_SUPPORT_IPODOUT = (0x01<<2)
	} IPOD_SUPPORT_MODE;

	typedef enum
	{
		IPOD_CURRENT_AUDIO_MODE = 1,
		IPOD_CURRENT_VIDEO_MODE,
		IPOD_CURRENT_OUT_MODE
	} IPOD_CURRENT_MODE;

	typedef enum
	{
		IPOD_STATUS_STOPPED,
		IPOD_STATUS_FFW_SEEK_STARTED,
		IPOD_STATUS_REW_SEEK_STARTED,
		IPOD_STATUS_FFW_REW_SEEK_STOPPED,
		IPOD_STATUS_PLAYING,
		IPOD_STATUS_PAUSED,

		IPOD_PLAYBACK_STATUS_MAX = 0x7FFFFFFFU

	} IPOD_PLAYBACK_STATUS; /*used Extended or Display Remote. Play status values*/

	typedef enum
	{
		/*****************************************************/
		/* Extended Remote Events                            */
		/*****************************************************/
		IPOD_APP_EVT_STOPPED 							= 0x00000000,
		IPOD_APP_EVT_TRACKCHANGED 						= 0x00000001,
		IPOD_APP_EVT_FFSEEKSTOP 						= 0x00000002,
		IPOD_APP_EVT_REWSEEKSTOP 						= 0x00000003,
		IPOD_APP_EVT_TRACKPOSITION 						= 0x00000004,
		IPOD_APP_EVT_CHAPTERINDEX 						= 0x00000005,
		IPOD_APP_EVT_PLAYBACK_STATUS_EXT 				= 0x00000006,
		IPOD_APP_EVT_TRACKTIME_OFFSET 					= 0x00000007,
		IPOD_APP_EVT_CHAPTER_TIME_OFFSET_MSEC  	 		= 0x00000008,
		IPOD_APP_EVT_CHAPTER_TIME_OFFSET_SEC  			= 0x00000009,
		IPOD_APP_EVT_TRACK_UNIQUE_ID  					= 0x0000000A,
		IPOD_APP_EVT_TRACK_PLAYBACK_MODE 				= 0x0000000B,
		IPOD_APP_EVT_TRACK_LYRICS_READY 				= 0x0000000C,
		IPOD_APP_EVT_TRACK_CAPABILITIES_CHANGED 		= 0x0000000D,
		IPOD_APP_EVT_PLAYBACK_ENGINE_CONTNENTS_CHANGED  = 0x0000000E,
		

		/*****************************************************/
		/* Display Remote Events                             */
		/*****************************************************/
		IPOD_APP_DR_EVT_TRACK_POSITION 					= 0x00000010,
		IPOD_APP_DR_EVT_TRACK_INDEX 					= 0x00000011,
		IPOD_APP_DR_EVT_CHAPTER_INFO 					= 0x00000012,
		IPOD_APP_DR_EVT_PLAY_STATUS 					= 0x00000013,
		IPOD_APP_DR_EVT_MUTE_UI_VOL 					= 0x00000014,
		IPOD_APP_DR_EVT_POWER_BATTERY 					= 0x00000015,
		IPOD_APP_DR_EVT_EQALIZER_STATE 					= 0x00000016,
		IPOD_APP_DR_EVT_SHUFFLE 						= 0x00000017,
		IPOD_APP_DR_EVT_REPEAT 							= 0x00000018,
		IPOD_APP_DR_EVT_DATE_TIME 						= 0x00000019,
		IPOD_APP_DR_EVT_ALARM 							= 0x0000001A,
		IPOD_APP_DR_EVT_BACKLIGHT 						= 0x0000001B,
		IPOD_APP_DR_EVT_HOLD_SWITCH 					= 0x0000001C,
		IPOD_APP_DR_EVT_SOUND_CHECK 					= 0x0000001D,
		IPOD_APP_DR_EVT_AUDIOBOOK 						= 0x0000001E,
		IPOD_APP_DR_EVT_TRACK_POSITION_IN_SECONDS 		= 0x0000001F,
		IPOD_APP_DR_EVT_MUTE_UI_ABSOLUTE_VOL 			= 0x00000020,
		IPOD_APP_DR_EVT_TRACK_CAPABILITIES 				= 0x00000021,
		IPOD_APP_DR_EVT_PLAYBACK_ENGINE_CONTENTS_CHANGED = 0x00000022,


		/*****************************************************/
		/* General Events          			                   */
		/*****************************************************/
		IPOD_APP_NOTI_TYPE_FLOW_CONTROL					= 0x00000030,
		IPOD_APP_NOTI_TYPE_RADIO_TAGGING				= 0x00000031,
		IPOD_APP_NOTI_TYPE_CAMERA_NOTIFICATIONS			= 0x00000032,
		IPOD_APP_NOTI_TYPE_CHARGING_INFO				= 0x00000033,
		IPOD_APP_NOTI_TYPE_DATABASE_CHAGNED				= 0x00000034,
		IPOD_APP_NOTI_TYPE_NOWPLAYINGAPPBUNDLENAME		= 0x00000035,
		IPOD_APP_NOTI_TYPE_SESSIONSPACEAVAILABLE		= 0x00000036,
		IPOD_APP_NOTI_TYPE_COMMAND_COMPLETE				= 0x00000037,
		IPOD_APP_NOTI_TYPE_BLUETOOTHCONNECTIONSTATUS 	= 0x00000038,
		IPOD_APP_NOTI_TYPE_NOWPLAYINGAPPDISPLAYNAME   	= 0x00000039,
		IPOD_APP_NOTI_TYPE_ASSISTIVETOUCHSTATUS   		= 0x0000003A,

		/*****************************************************/
		/* Timeout Events          			                   */
		/*****************************************************/
		IPOD_SVC_REQ_TIMEOUT_PLAY = 0x000000D1,
		IPOD_SVC_REQ_TIMEOUT_PAUSE = 0x000000D2,
	        
		/*****************************************************/
		/* General Events          			                   */
		/*****************************************************/
		IPOD_APP_REQUEST_IDENTIFY					= 0x000000E1,        
	        
		/*****************************************************/
		/* External Accessory Protocol                             */
		/*****************************************************/
		IPOD_APP_EAP_OPENSESSION = 0x00000101,
		IPOD_APP_EAP_CLOSESESSION = 0x00000102,
		IPOD_APP_EAP_IPODDATATRANSFER = 0x00000103,
		IPOD_APP_EAP_TEST1 = 0x00000104,
		IPOD_APP_EAP_TEST2 = 0x00000105,

		/*****************************************************/
		/* Digital Audio Event Protocol                       */
		/*****************************************************/
		IPOD_APP_EVT_CHANGE_SAMPLEFREQ 					= 0x00000110,

		IPOD_APP_EVT_CATEGORIZED_DATABASE_RECORD = 0x00000111,
		IPOD_APP_IPODNOTIFICATION = 0x00000112,
		
		IPOD_APP_EVT_MAX= 0x000001FF,
	} IPOD_EVENT_ID;

#define MAX_RETRY_DATA_TRANSFER_COUNT (10)

	typedef enum
	{
		IPOD_DATATRANSFER_NORMAL,
		IPOD_DATATRANSFER_TIMEOUT
	} IPOD_DATATRANSFER_TYPE;

	/****************************************************************************************************************/
	/* 0x00 General Lingo Command Define and Enum							 										*/
	/****************************************************************************************************************/


	typedef enum
	{
		IPOD_NOTI_TYPE_FLOW_CONTROL 				= 0x02U,
		IPOD_NOTI_TYPE_RADIO_TAGGING 				= 0x03U,
		IPOD_NOTI_TYPE_CAMERA_NOTIFICATIONS 		= 0x04U,
		IPOD_NOTI_TYPE_CHARGING_INFO				= 0x05U,
		IPOD_NOTI_TYPE_DATABASE_CHAGNED			= 0x09U,
		IPOD_NOTI_TYPE_NOWPLAYINGAPPBUNDLENAME 	= 0x0AU,
		IPOD_NOTI_TYPE_SESSIONSPACEAVAILABLE		= 0x0BU,
		IPOD_NOTI_TYPE_COMMAND_COMPLETE			= 0x0DU,
		IPOD_NOTI_TYPE_BLUETOOTHCONNECTIONSTATUS	= 0x11U,
		IPOD_NOTI_TYPE_NOWPLAYINGAPPDISPLAYNAME   = 0x13U,
		IPOD_NOTI_TYPE_ASSISTIVETOUCHSTATUS   = 0x14U,
	} IPOD_NOTI_TYPE; 

	typedef enum
	{
		IPOD_NOTI_BITMASK_RESERVED_BIT0							= (0x01U<<0),
		IPOD_NOTI_BITMASK_RESERVED_BIT1							= (0x01U<<1),
		IPOD_NOTI_BITMASK_FLOW_CONTROL							= (0x01U<<2),
		IPOD_NOTI_BITMASK_RADIO_TAGGING_STATUS					= (0x01U<<3),
		IPOD_NOTI_BITMASK_CAMERA_STATUS							= (0x01U<<4),
		IPOD_NOTI_BITMASK_CHARGING_INFO							= (0x01U<<5),
		IPOD_NOTI_BITMASK_RESERVED_BIT6							= (0x01U<<6),
		IPOD_NOTI_BITMASK_RESERVED_BIT7							= (0x01U<<7),
		IPOD_NOTI_BITMASK_RESERVED_BIT8							= (0x01U<<8),
		IPOD_NOTI_BITMASK_DATABASE_CHANGED							= (0x01U<<9),
		IPOD_NOTI_BITMASK_NOWPLAYINGFOCUSAPP_STATUS			= (0x01U<<10),
		IPOD_NOTI_BITMASK_SSESSIONSPACEAVAILABLE_STATUS		= (0x01U<<11),
		IPOD_NOTI_BITMASK_RESERVED_BIT12					= (0x01U<<12),
		IPOD_NOTI_BITMASK_COMMAND_COMPLETED					= (0x01U<<13),
		IPOD_NOTI_BITMASK_RESERVED_BIT14							= (0x01U<<14),
		IPOD_NOTI_BITMASK_STATUS_IPOD_OUT_MODE					= (0x01U<<15),
		IPOD_NOTI_BITMASK_RESERVED_BIT16						= (0x01U<<16),
		IPOD_NOTI_BITMASK_BT_CONNECTION_STATUS					= (0x01U<<17),
		IPOD_NOTI_BITMASK_RESERVED_BIT18						= (0x01U<<18),
		IPOD_NOTI_BITMASK_NOWPLAYINGAPPDISPLAYNAME_STATUS		= (0x01U<<19),
		IPOD_NOTI_BITMASK_ASSISTIVETOUCH_STATUS					= (0x01U<<20)
	} IPOD_NOTI_BITMASK_STATUS; /*Notification bitmask bits*/

	typedef enum
	{
		IPOD_NOTI_TAGGING_OPERATION_SUCCESSFUL				= 0x01U,
		IPOD_NOTI_TAGGING_OPERATION_FAILED						= 0x02U,
		IPOD_NOTI_INFORMATION_AVAILABLE_FOR_TAGGING 			= 0x03U,
		IPOD_NOTI_INFORMATION_NOT_AVAILABLE_FOR_TAGGING		= 0x04U
	} IPOD_NOTI_TAG_STATUS;

	typedef enum
	{
		IPOD_NOTI_CAMERA_APP_OFF		= 0x01U,
		IPOD_NOTI_PREVIEW					= 0x03U,
		IPOD_NOTI_RECODING 				= 0x04U
	} IPOD_NOTI_CAMERA_PAYLOAD;

	typedef enum
	{
		IPOD_NOTI_DATABASE_NOT_AVAILABLE		= 0x00U,
		IPOD_NOTI_DATABASE_AVAILABLE			= 0x01U
	} IPOD_NOTI_DATABASE_AVAILABLE_STATUS;

	typedef enum
	{
		IPOD_NOTI_COMMAND_SUCCESSFUL			= 0x00U,
		IPOD_NOTI_COMMAND_FAILED				= 0x01U,
		IPOD_NOTI_COMMAND_CANCELLED			= 0x02U
	} IPOD_NOTI_COMMAND_COMPLETION_STATUS;

	typedef enum
	{
		IPOD_NOTI_AVAILABLE_CURRENT			= 0x00U
	} IPOD_NOTI_CHARGING_INFO_TYPE;

	typedef enum
	{
		IPOD_NOTI_AVAILABLE_CURRENT_VALUE		= 0x00U
	} IPOD_NOTI_CHARGING_INFO_VALUE;

	typedef enum
	{
		IPOD_VIDEO_OUT_SETTING = 0x00,
		IPOD_SCREEEN_CONFIGURATION = 0x01,
		IPOD_VIDEO_SIGINAL_FORMAT= 0x02,
		IPOD_LINE_OUT_USAGE = 0x03,
		IPOD_VIDEO_OUT_CONNECTION = 0x08,
		IPOD_CLOSED_CAPTIONING = 0x09,
		IPOD_VIDEO_MONITOR_ASPECT_RATIO = 0x0A,
		IPOD_SUBTITLES = 0x0C,
		IPOD_VIDEO_ALTERNATE_AUDIO_CHANNEL =0x0D,
		IPOD_PAUSE_ON_POWER_REMOVAL = 0x0F,
		IPOD_VOICEOVER_PREFERENCE = 0x14,
		IPOD_ASSISTIVETOUCH	= 0x16
	} IPOD_PREFERENCE_CLASSID;/*Apple device preference class ID*/

	enum
	{
		IPOD_SETTING_OFF,
		IPOD_SETTING_ON
	};
	enum
	{
		IPOD_VIDEO_FULLSCREEN,
		IPOD_VIDEO_FIT_TO_SCREEN
	};
	enum
	{
		IPOD_VIDEO_NTSC,
		IPOD_VIDEO_PAL
	};
	enum
	{
		IPOD_VIDEO_OUT_NONE,
		IPOD_VIDEO_OUT_COMPOSITE,
		IPOD_VIDEO_OUT_SVIDEO,
		IPOD_VIDEO_OUT_COMPONENT
	};
	enum
	{
		IPOD_VIDEO_4BY3_RATIO,
		IPOD_VIDEO_16BY9_RATIO
	};
	enum
	{
		IPOD_VIDEO_SUBTITLES_OFF,
		IPOD_VIDEO_SUBTITLES_ON
	};
	enum
	{
		IPOD_VIDEO_ALTERNATE_AUDIO_CHANNEL_ON,
		IPOD_VIDEO_ALTERNATE_AUDIO_CHANNEL_OFF
	};
	enum
	{
		IPOD_PAUSE_ON_POWER_REMOVAL_PAUSE_OFF,
		IPOD_PAUSE_ON_POWER_REMOVAL_PAUSE_ON
	};
	enum
	{
		IPOD_VOICEOVER_PREFERENCE_VOICEOVER_OFF,
		IPOD_VOICEOVER_PREFERENCE_VOICEOVER_ON
	};
	enum
	{
		IPOD_ASSISTIVETOUCH_OFF,
		IPOD_ASSISTIVETOUCH_ON
	};

	typedef enum/*GetAccessoryInfo*/
	{
		IPOD_ACC_INFO_CAPA 					= 0x00, /*Required*/
		IPOD_ACC_NAME 						= 0x01, /*Required*/
		/* Deprecated	IPOD_ACC_MIN_FW_VER	= 0x02,*/ /*Optional*/
		IPOD_ACC_MIN_LINGO_VER 				= 0x03, /*Optional*/
		IPOD_ACC_FW_VER						= 0x04, /*Required*/
		IPOD_ACC_HW_VER						= 0x05, /*Required*/
		IPOD_ACC_MANUFACTURER				= 0x06, /*Required*/
		IPOD_ACC_MODEL_NUM					= 0x07, /*Required*/
		IPOD_ACC_SERIAL_NUM					= 0x08, /*Optional*/
		IPOD_ACC_INCOMING_MAX_PAYLOAD_SIZE 	= 0x09, /*Optional*/
		/*Reserved*/
		IPOD_ACC_STATUS_TYPES_SUPPORTED 	= 0x0B /*Optional*/
		/*Reserved*/
	} IPOD_ACCESSORY_INFO_TYPE_VALUES;/*Accessory Info Type values*/

	typedef unsigned long long		IPOD_ACCCAPS_TOCKEN_VAL;

#define IPOD_MAX_INFO_STR	64

// Accessory capabilities bit values //
#define AC_ANALOG_LINE_OUT		(0x00000001<<0)
#define AC_ANALOG_LINE_IN			(0x00000001<<1)
#define AC_ANALOG_VIDEO_OUT		(0x00000001<<2)
#define AC_USB_DIGITAL_AUDIO		(0x00000001<<4)
#define AC_SUPPORT_IPHONE_OS_APP	(0x00000001<<9)
#define AC_CHECK_IPOD_VOLUME		(0x00000001<<11)
#define AC_VOICEOVER				(0x00000001<<17)
#define AC_PLAYBACK_STATUS_NOTI		(0x00000001<<18)
#define AC_INTERLEAVED_TRANSFERS	(0x00000001<<19)
#define AC_AV_SYNC					(0x00000001<<21)
#define AC_ASSISTIVETOUCH			(0x00000001<<23)

	/*Need to modify*/
#define IPOD_ACCESSORY_CAP_INFO							(0x01<<0) /*Deprecated.*/
#define IPOD_ACCESSORY_NAME								(0x01<<1)
//#define IPOD_ACCESSORY_MIN_SUPPORT_IPOD_FIRMWARE_VER	(0x01<<2) /*Deprecated.*/
//#define IPOD_ACCESSORY_MIN_SUPPORT_LINGO_VER			(0x01<<3) /*Deprecated.*/
#define IPOD_ACCESSORY_FIRMWARE_VERSION					(0x01<<4)
#define IPOD_ACCESSORY_HARDWARE_VERSION					(0x01<<5)
#define IPOD_ACCESSORY_MANUFACTURER						(0x01<<6)
#define IPOD_ACCESSORY_MODEL_NUM						(0x01<<7)
#define IPOD_ACCESSORY_SERIAL_NUM						(0x01<<8)
#define IPOD_ACCESSORY_INCOMING_MAX_PAYLOAD_SIZE		(0x01<<9)

#define IPOD_ACCESSORY_STATUS							(0x01<<11)
//#define IPOD_ACCESSORY_ASYNC_PLAYBACK_STATUS_CHANGE	(0x01<<18)

	/* This is not defined on table 2-59. but It is used on Library definition*/
	/*Need to modify. need to separate GetAccessoryInfo and SetFIDTokenvalues*/

#define IPOD_ACCESSORY_RF_CERTIFICATIONS					(0x01<<12)
	/*Accssory RF certification declarations*/
#define IPOD_RF_CERT_CLASS1	(0x01<<0)
#define IPOD_RF_CERT_CLASS2	(0x01<<1)
#define IPOD_RF_CERT_CLASS3	(0x01<<2) /*It is reserved - Do not use this one*/
#define IPOD_RF_CERT_CLASS4	(0x01<<3)

#define IPOD_SDK_PROTOCOL_MAX_CNT 0x0D

#define IPOD_BUNDLE_SEEDID_STRING_SIZE 11

#define IPOD_PREF_MAX_CNT 0x0D


	/****************************************************************************************************************/
	/* 0x02 Simple Remote Lingo Command Define and Enum						 										*/
	/****************************************************************************************************************/

	typedef enum
	{
		IPOD_SIMPLE_NO_BUTTONS				= (0x00UL),
		IPOD_SIMPLE_PLAY_PAUSE 				= (0x01UL),
		IPOD_SIMPLE_VOLUME_UP 				= (0x02UL),
		IPOD_SIMPLE_VOLUME_DOWN 			= (0x04UL),
		IPOD_SIMPLE_NEXT_TRACK 				= (0x08UL),
		IPOD_SIMPLE_PREVIOUS_TRACK 			= (0x10UL),
		IPOD_SIMPLE_NEXT_ALBUM 				= (0x20UL),
		IPOD_SIMPLE_PREVIOUS_ALBUM 			= (0x40UL),
		IPOD_SIMPLE_STOP 						= (0x80UL),

		IPOD_SIMPLE_PLAY_RESUME 				= (0x01UL<<8),
		IPOD_SIMPLE_PAUSE 					= (0x02UL<<8),
		IPOD_SIMPLE_MUTE_TOGGLE 				= (0x04UL<<8),
		IPOD_SIMPLE_NEXT_CHAPTER 			= (0x08UL<<8),
		IPOD_SIMPLE_PREVIOUS_CHAPTER 		= (0x10UL<<8),
		IPOD_SIMPLE_NEXT_PLAYLIST 			= (0x20UL<<8),
		IPOD_SIMPLE_PREVIOUS_PLAYLIST 		= (0x40UL<<8),
		IPOD_SIMPLE_SHUFFLE_SETTING_ADV 	= (0x80UL<<8),

		IPOD_SIMPLE_REPEAT_SETTING_ADV 		= (0x01UL<<16),
		IPOD_SIMPLE_POWER_ON 				= (0x02UL<<16),
		IPOD_SIMPLE_POWER_OFF 				= (0x04UL<<16),
		IPOD_SIMPLE_BACKLIGHT_FOR_30SEC 	= (0x08UL<<16),
		IPOD_SIMPLE_BEGIN_FAST_FORWARD 		= (0x10UL<<16),
		IPOD_SIMPLE_BEGIN_REWIND 			= (0x20UL<<16),
		IPOD_SIMPLE_MENU 						= (0x40UL<<16),
		IPOD_SIMPLE_SELECT 					= (0x80UL<<16),

		IPOD_SIMPLE_UP_ARROW 				= (0x01UL<<24),
		IPOD_SIMPLE_DOWN_ARROW 				= (0x02UL<<24),
		IPOD_SIMPLE_BACKLIGHT_OFF 			= (0x04UL<<24)

	} IPOD_SIMPLE_BUTTON_CTRL;/*Button states*/

	typedef enum
	{
		IPOD_IMG_PLAY_PAUSE 				= (0x01UL),
		IPOD_IMG_NEXT_IMG 				= (0x02UL),
		IPOD_IMG_PREVIOUS_IMG 			= (0x04UL),
		IPOD_IMG_STOP		 				= (0x08UL),
		IPOD_IMG_PLAY_RESUME				= (0x10UL),
		IPOD_IMG_PAUSE 					= (0x20UL),
		IPOD_IMG_SHUFFLE_ADV 			= (0x40UL),
		IPOD_IMG_REPEAT_ADV				= (0x80UL)
	} IPOD_IMG_BUTTON_CTRL;/*Deprecated.*/


	typedef enum
	{
		IPOD_VIDEO_PLAY_PAUSE 				= (0x01UL),
		IPOD_VIDEO_NEXT_VIDEO 				= (0x02UL),
		IPOD_VIDEO_PREVIOUS_VIDEO 			= (0x04UL),
		IPOD_VIDEO_STOP		 				= (0x08UL),
		IPOD_VIDEO_PLAY_RESUME				= (0x10UL),
		IPOD_VIDEO_PAUSE 						= (0x20UL),
		IPOD_VIDEO_BEGIN_FF 					= (0x40UL),
		IPOD_VIDEO_BEGIN_REW					= (0x80UL),

		IPOD_VIDEO_NEXT_CHAPTER 				= (0x01UL<<8),
		IPOD_VIDEO_PREVIOUS_CHAPTER 			= (0x02UL<<8),
		IPOD_VIDEO_NEXT_FRAME 				= (0x04UL<<8),
		IPOD_VIDEO_PREVIOUS_FRAME			= (0x08UL<<8)

	} IPOD_VIDEO_BUTTON_CTRL;/*Video-specific button values*/

	typedef enum
	{
		IPOD_AUDIO_PLAY_PAUSE 				= (0x01UL),
		IPOD_AUDIO_VOLUME_UP 				= (0x02UL),
		IPOD_AUDIO_VOLUME_DOWN 				= (0x04UL),
		IPOD_AUDIO_NEXT_TRACK 				= (0x08UL),
		IPOD_AUDIO_PREVIOUS_TRACK 			= (0x10UL),
		IPOD_AUDIO_NEXT_ALBUM 				= (0x20UL),
		IPOD_AUDIO_PREVIOUS_ALBUM 			= (0x40UL),
		IPOD_AUDIO_STOP 						= (0x80UL),

		IPOD_AUDIO_PLAY_RESUME 				= (0x01UL<<8),
		IPOD_AUDIO_PAUSE 						= (0x02UL<<8),
		IPOD_AUDIO_MUTE_TOGGLE 				= (0x04UL<<8),
		IPOD_AUDIO_NEXT_CHAPTER 				= (0x08UL<<8),
		IPOD_AUDIO_PREVIOUS_CHAPTER 		= (0x10UL<<8),
		IPOD_AUDIO_NEXT_PLAYLIST 			= (0x20UL<<8),
		IPOD_AUDIO_PREVIOUS_PLAYLIST 		= (0x40UL<<8),
		IPOD_AUDIO_SHUFFLE_SETTING_ADV 		= (0x80UL<<8),

		IPOD_AUDIO_REPEAT_SETTING_ADV 		= (0x01UL<<16),
		IPOD_AUDIO_BEGIN_FF 					= (0x02UL<<16),
		IPOD_AUDIO_BEGIN_REW 				= (0x04UL<<16),
		IPOD_AUDIO_RECORD 					= (0x08UL<<16)

	} IPOD_AUDIO_BUTTON_CTRL;/*Audio-specific button values*/

	/****************************************************************************************************************/
	/* 0x03 Display Remote Lingo Command Define and Enum															*/
	/****************************************************************************************************************/

	typedef enum
	{
		/* the new setting persists after accessory detach */
		UPDATE_NEW_SETTING,/*Do not save the original state.*/
		/* the original setting is restored on detach */
		RESTORE_ORG_SETTING /*Save the original state and restore it on exit.*/
	} RestoreOnExit;

	typedef enum
	{
		IPOD_DR_EVT_TRACK_POSITION = 0x00U,
		IPOD_DR_EVT_TRACK_INDEX = 0x01U,
		IPOD_DR_EVT_CHAPTER_INFO = 0x02U,
		IPOD_DR_EVT_PLAY_STATUS = 0x03U,
		IPOD_DR_EVT_MUTE_UI_VOL = 0x04U,
		IPOD_DR_EVT_POWER_BATTERY = 0x05U,
		IPOD_DR_EVT_EQALIZER_STATE = 0x06U,
		IPOD_DR_EVT_SHUFFLE = 0x07U,
		IPOD_DR_EVT_REPEAT = 0x08U,
		IPOD_DR_EVT_DATE_TIME = 0x09U,
		IPOD_DR_EVT_ALARM = 0x0AU,
		IPOD_DR_EVT_BACKLIGHT = 0x0BU,
		IPOD_DR_EVT_HOLD_SWITCH = 0x0CU,
		IPOD_DR_EVT_SOUND_CHECK = 0x0DU,
		IPOD_DR_EVT_AUDIOBOOK = 0x0EU,
		IPOD_DR_EVT_TRACK_POSITION_IN_SECONDS = 0x0FU,
		IPOD_DR_EVT_MUTE_UI_ABSOLUTE_VOL = 0x10U,
		IPOD_DR_EVT_TRACK_CAPABILITIES = 0x11U,
		IPOD_DR_EVT_PLAYBACK_ENGINE_CONTENTS_CHANGED = 0x12U,

		IPOD_DR_EVENT_MAX = 0x7FU

	} IPOD_REMOTE_EVENT_NOTI;/*Event notification data*/

	typedef enum/*Need to modify.*/
	{
		IPOD_DR_STATUS_TRACK_POS_MS = 0x00U,
		IPOD_DR_STATUS_PLAYBACK_INDEX = 0x01U,
		IPOD_DR_STATUS_CHAPTER_INFO = 0x02U,
		IPOD_DR_STATUS_PLAY_STATUS = 0x03U,
		IPOD_DR_STATUS_MUTE_AND_VOLUME = 0x04U,
		IPOD_DR_STATUS_POWER_AND_BATTERY = 0x05U,
		IPOD_DR_STATUS_EQ_SETTING = 0x06U,
		IPOD_DR_STATUS_SHUFFLE_SETTING = 0x07U,
		IPOD_DR_STATUS_REPEAT_SETTING = 0x08U,
		IPOD_DR_STATUS_DATA_AND_TIME = 0x09U,
		IPOD_DR_STATUS_ALARM_STATE_AND_DATE = 0x0AU,
		IPOD_DR_STATUS_BACKLIGHT = 0x0BU,
		IPOD_DR_STATUS_HOLD_SWITCH = 0x0CU,
		IPOD_DR_STATUS_SOUND_CHECK = 0x0DU,
		IPOD_DR_STATUS_AUDIO_BOOK_SPEED = 0x0EU,
		IPOD_DR_STATUS_TRACK_POS_S = 0x0FU,
		IPOD_DR_STATUS_MUTE_UI_ABSOLUTE_VOL = 0x10U,

		IPOD_STATUS_INFO_MAX = 0x7FU

	} IPOD_DR_STATUS_INFO;/*Event notification data*/

	typedef enum/*Need to modify.*/
	{
		IPOD_DR_EVT_SET_TRACK_TIME_POS_MS = (0x01<<0),
		IPOD_DR_EVT_SET_TRACK_PLAY_INDEX = (0x01<<1),
		IPOD_DR_EVT_SET_CHAPTER_INDEX = (0x01<<2),
		IPOD_DR_EVT_SET_PLAY_STATUS = (0x01<<3),
		IPOD_DR_EVT_SET_MUTE_UI_VOL = (0x01<<4),
		IPOD_DR_EVT_SET_POWER_BAT = (0x01<<5),
		IPOD_DR_EVT_SET_EQ_SETTING = (0x01<<6),
		IPOD_DR_EVT_SET_SHUFFLE_SETTING = (0x01<<7),
		IPOD_DR_EVT_SET_REPEAT_SETTING = (0x01<<8),
		IPOD_DR_EVT_SET_DATE_TIME_SETTING = (0x01<<9),
		IPOD_DR_EVT_SET_ALARM_SETTING = (0x01<<10),
		IPOD_DR_EVT_SET_BACKLIGHT_STATE = (0x01<<11),
		IPOD_DR_EVT_SET_HOLD_SWITCH = (0x01<<12),
		IPOD_DR_EVT_SET_SOUND_CHECK = (0x01<<13),
		IPOD_DR_EVT_SET_AUDIO_BOOK_SPEED = (0x01<<14),
		IPOD_DR_EVT_SET_TRACK_POS_S = (0x01<<15),
		IPOD_DR_EVT_SET_MUTE_UI_ABSOLUTEVOL = (0x01<<16),
		IPOD_DR_EVT_SET_TRACK_CAPABILITIES = (0x01<<17),
		IPOD_DR_EVT_SET_PLAYBACK_ENGINE_CONTENTS_CHANGED = (0x01<<18),

		IPOD_DR_EVT_SET_MAX = 0x7FFFFFFF

	} IPOD_REMOTE_EVENT_NOTI_SET;/*Event notification data*/



	typedef enum
	{
		IPOD_DR_PLAYBACK_STOPPED = 0x00U,
		IPOD_DR_PLAYING = 0x01U,
		IPOD_DR_PLAYBACK_PAUSED = 0x02U,
		IPOD_DR_FAST_FORWARD = 0x03U,
		IPOD_DR_FAST_REWIND = 0x04U,
		IPOD_DR_END_FAST_FORWARD_OR_REWIND =0x05U,

		IPOD_DR_UNKNOWN = 0x20U,

		IPOD_DR_MAX_PLAYSTATUS = 0x7FU

	} IPOD_DR_PLAY_STATUS;



	typedef enum{
		IPOD_DR_TRACK_INFO_CAPS = 0x00U,
		IPOD_DR_TRACK_INFO_CHAPTER_TIMENAME = 0x01U,
		IPOD_DR_TRACK_INFO_ARTIST_NAME = 0x02U,
		IPOD_DR_TRACK_INFO_ALBUM_NAME = 0x03U,
		IPOD_DR_TRACK_INFO_GENRE_NAME = 0x04U,
		IPOD_DR_TRACK_INFO_TRACK_TITLE = 0x05U,
		IPOD_DR_TRACK_INFO_COMPOSER_NAME = 0x06U,
		IPOD_DR_TRACK_INFO_LYRICS = 0x07U,
		IPOD_DR_TRACK_INFO_ARTWORK_CNT = 0x08U,

		IPOD_DR_TRACK_INFO_MAX = 0x7FU
	} IPOD_DR_TRACK_INFO_TYPE; /*Track information data*/




	/****************************************************************************************************************/
	/* 0x04 Extended Interface Lingo Command Define and Enum				 										*/
	/****************************************************************************************************************/
	typedef enum
	{
		EX_STOPPED = 0x02,
		EX_FFW_SEEK_STARTED = 0x05,
		EX_REW_SEEK_STARTED = 0x06,
		EX_FFWREW_SEEK_STOPPED = 0x07,
		EX_PLAYING = 0x0A,
		EX_PAUSED = 0x0B
	} IPOD_EX_PLAY_STATUS; /*Play status change notification */

	typedef enum
	{
		EX_AUDIO_TRACK = 0x00,
		EX_VIDEO_TRACK = 0x01
	} IPOD_EX_PLAY_MODE;

	typedef enum
	{
		IPOD_SLOW	= 0xFFU, /*Slower -1*/
		IPOD_NORMAL	= 0x00U, /*Normal*/
		IPOD_FAST	= 0x01U /*Faster +1*/
	} AudiobookSpeed;		/*Audiobook speed states*/
	typedef enum
	{
		CAPAINFO, /*Track capabilities and information */
		PODCASTNAME,
		RELEASEDATE,
		DESCRIPTION,
		SONGLYRICS,
		GENRE,
		COMPOSER,
		ARTWORKCOUNT
	} TrackInformationType;/*Track information type*/

	typedef enum
	{
		IPOD_INVALID_CATEGORY = -1,
		IPOD_ALL, /*Top-level*/
		IPOD_PLAYLIST,
		IPOD_ARTIST,
		IPOD_ALBUM,
		IPOD_GENRE,
		IPOD_TRACK,
		IPOD_COMPOSER,
		IPOD_AUDIOBOOK,
		IPOD_PODCAST,
		IPOD_NESTEDPLAYLIST,
		IPOD_GENIUSMIXES,/*Similar to playlists, but with the Genius feature*/
		IPOD_ITUNESU/*Similar to podcast*/
	} CategoryType;/*Database category types for commands*/

#define MEDIA_KIND IPOD_GENRE

#define ONE_BYTE_FORMAT 0x00
#define FOUR_BYTE_FORMAT 0x01

	typedef enum
	{
		DISABLENOTIFICATION = 0x00,  //Disable all change notification
		ENABLENOTIFICATION = 0x01	//Enable all change notification

	} OneByteSetPlayStatusNotification; /*One-byte status change event values*/

	typedef enum
	{
		IPOD_EVT_BASIC_PLAY_STATUS = (0x01<<0),
		IPOD_EVT_EXTENDED_PLAY_STATUS_CHANGES = (0x01<<1),
		IPOD_EVT_TRACK_INDEX = (0x01<<2),
		IPOD_EVT_TRACK_TIME_OFFSET_MSEC = (0x01<<3),
		IPOD_EVT_TRACK_TIME_OFFSET_SEC = (0x01<<4),
		IPOD_EVT_CHAPTER_INDEX = (0x01<<5),
		IPOD_EVT_CHAPTER_OFFSET_MSEC = (0x01<<6),
		IPOD_EVT_CHAPTER_OFFSET_SEC = (0x01<<7),
		IPOD_EVT_TRACK_UNIQUE_ID = (0x01<<8),
		IPOD_EVT_TRACK_MEDIA_TYPE = (0x01<<9),
		IPOD_EVT_TRACK_LYRICS_READY = (0x01<<10),
		IPOD_EVT_TRACK_CAPABILITIES_CHANGED = (0x01<<11),
		IPOD_EVT_TRACK_PLAYBACK_ENGINE_CONTENTS_CHANGED = (0x01<<12)
//	IPOD_EVT_MAX = 0x7FFFFFFF
	} FourByteSetPlayStatusNotification;/*Four-byte status change event mask bits*/
	typedef enum
	{
		STOPPED = 0x00000000U,
		TRACKCHANGED = 0x00000001U,
		FFSEEKSTOP = 0x00000002U,
		REWSEEKSTOP = 0x00000003U,
		TRACKPOSITION = 0x00000004U,
		CHAPTERINDEX = 0x00000005U,
		PLAYBACK_STATUS_EXT = 0x00000006U,  /* Extended play state changes */
		TRACKTIME_OFFSET = 0x00000007U,
		CHAPTER_TIME_OFFSET_MSEC = 0x00000008U,
		CHAPTER_TIME_OFFSET_SEC = 0x00000009U,
		TRACK_UNIQUE_ID = 0x0000000AU,
		TRACK_PLAYBACK_MODE = 0x0000000BU,
		TRACK_LYRICS_READY = 0x0000000CU,
		TRACK_CAPABILITIES_CHANGED = 0x0000000DU,
		PLAYBACK_ENGINE_CONTNENTS_CHANGED = 0x0000000EU,
		NOTIFICATION_MAX = 0x7FFFFFFFU
	} NotificationPlayStatus;/*Play status change notification codes*/

	typedef enum
	{
		IPOD_STOPPING = 0x00U,
		IPOD_PLAYING = 0x01U,
		IPOD_PAUSED = 0x02U,
		IPOD_STATUS_ERROR = 0xFFU	//Enable all change notification
	} IPOD_GETPLAYSTATUS; /*IPOD_GetPlayStatus*/

	typedef enum
	{
		PLAYPAUSE 		= 0x01U, /*Toggle play/pause 1.00*/
		STOP			= 0x02U,
		NEXTTRACK		= 0x03U, /*Deprecated; use Next(0x08) instead.*/
		PREVIOUSTRACK	= 0x04U, /*Deprecated; use Previous(0x09) instead.*/
		STARTFF			= 0x05U, /*1.00*/
		STARTREW		= 0x06U, /*1.00*/
		ENDFFREW		= 0x07U, /*1.00*/
		NEXT			= 0x08U, /*1.06*/
		PREVIOUS		= 0x09U, /*1.06*/
		NEWPLAY			= 0x0AU, /*1.13*/
		NEWPAUSE		= 0x0BU, /*1.13*/
		NEXTCHAPTER		= 0x0CU, /*Deprecated; use Next(0x08) instead.*/
		PREVIOUSCHAPTER	= 0x0DU,	  /*Deprecated; use Previous(0x09) instead.*/
		RESUMEIPOD                = 0x0EU /* 1.14*/
	} PlayCmdCode; /*Play control command codes*/

	typedef enum
	{
		SHUFFLEOFF,
		SHUFFLETRACKS,
		SHUFFLEALBUMS
	} ShuffleMode;/*Shuffle modes*/

	typedef enum
	{
		REPEATOFF,
		REPEATONETRACK,
		REPEATALLTRACKS
	} CurrentRepeatState;/*Repeat state values*/

	typedef enum /*Deprecated*/
	{
		SORTBYGENRE = 0x00U,
		SORTBYARTIST = 0x01U,
		SORTBYCOMPOSER = 0x02U,
		SORTBYALBUM = 0x03U,
		SORTBYNAME = 0x04U,
		SORTBYPLAYLIST = 0x05U,
		SORTBYRELEASEDATE = 0x06U,
		SORTBYDEFAULT = 0xFFU
	} SortOrder;

	typedef enum /*Need to modify*/
	{
		MONOCHROME_2BPP = 0x01,
		RGB565_LITTLE_ENDIAN_16BPP,
		RGB565_BIG_ENDIAN_16BPP
//	MAX_PIXEL_FORMAT = 0x7F
	} DisplayPixelFormat; /*Display pixel format codes*/

	typedef enum /*Need to modify*/
	{
		// 2bits per pixel
		IPOD_PIXEL_MONOCHROME = 0x01U,

		// RGB 565 color little-endian, 16bpp
		IPOD_PIXEL_RGB_565_LITTLE = 0x02U,

		// RGB 565 color big-endian, 16bpp
		IPOD_PIXEL_RGB_565_BIG = 0x03U,

		IPOD_PIXEL_MAX = 0x7FFFFFFFU

	} IPOD_PIXEL_FORMAT;/*Display pixel format codes*/

	typedef enum
	{
		DBSEL_AUDIO = 0x01,
		DBSEL_VIDEO = 0x02
	} IPOD_SEL_DB_TYPE;/*Hierarchy selection*/

	typedef enum
	{
		IPOD_VIDEO_AVAILABLE = (0x01<<0)
	} IPOD_VIDEO_OPTION;


	/****************************************************************************************************************/
	/* 0x0A Digital Audio Lingo Command Define and Enum						 										*/
	/****************************************************************************************************************/

	/****************************************************************************************************************/
	/* 0x0D iPod Out Lingo Command Define and Enum							 										*/
	/****************************************************************************************************************/
	typedef enum
	{
		IPOD_OUT_AVAILABLE = (0x01<<0)
	} IPOD_OUT_OPTION;
	
	typedef enum
	{
		STD_OPERATING_MODE,
		EXTENDED_INTERFACE_MODE,
		IPOD_OUT_MODE
	} IPOD_UI_MODE;


	typedef struct
	{
		unsigned char m_Mute;
		unsigned char m_Vol;
		unsigned char m_AbsoluteVol;
	} IPOD_DR_STATEINFO_MUTEVOL;

	typedef struct
	{
		unsigned short m_Year;
		unsigned char m_Month;
		unsigned char m_Day;
		unsigned char m_Hour;
		unsigned char m_Min;
	} IPOD_DR_DATETIME;

	typedef struct
	{
		unsigned char m_State;
		unsigned char m_Hour;
		unsigned char m_Min;
	} IPOD_DR_ALARM;

	typedef union tag_IPOD_DR_SETSTATEINFO_PARAM_DATA
	{
		unsigned long m_TrackPos_ms;
		unsigned long m_TrackIndex;
		unsigned short m_ChapterIndex;
		IPOD_DR_PLAY_STATUS m_PlayStatus;
		IPOD_DR_STATEINFO_MUTEVOL m_MuteVol;
		unsigned long m_EqIndex;
		unsigned char m_ShuffleState;
		unsigned char m_RepeatSetting;
		IPOD_DR_DATETIME m_DateTime;
		IPOD_DR_ALARM m_Alarm;
		unsigned char m_BackLight;
		unsigned char m_SoundCheck;
		unsigned char m_AudioBookSpeed;
		unsigned short m_TrackPos_s;
		IPOD_DR_STATEINFO_MUTEVOL  m_MuteUiAbsoluteVol;
	} IPOD_DR_SETSTATEINFO_PARAM_DATA, *pIPOD_DR_SETSTATEINFO_PARAM_DATA;

	typedef struct
	{
		IPOD_DR_STATUS_INFO m_InfoType;
		IPOD_DR_SETSTATEINFO_PARAM_DATA m_data;
		RestoreOnExit m_RestoreOpt;
	} IPOD_DR_SETSTATEINFO_PARAM, *pIPOD_DR_SETSTATEINFO_PARAM;

	typedef struct
	{
		IPOD_DR_PLAY_STATUS m_Status;
		unsigned long m_TrackIndex;
		unsigned long m_TrackTotalLen_ms;
		unsigned long m_TrackPos_ms;
	} IPOD_DR_PLAYSTATUS, *pIPOD_DR_PLAYSTATUS;

	typedef struct
	{
		unsigned long m_TrackCaps;
		unsigned long m_TrackTotalLen_ms;
		unsigned short m_ChapterCnt;
	} IPOD_DR_TRACK_CAP_INFO, *pIPOD_DR_TRACK_CAP_INFO;

	typedef struct
	{
		unsigned long m_ChapterTime;
		unsigned char *m_ChapterName;
		unsigned long m_ChapterNameBufSize;
		unsigned long m_ChapterNameActualSize;
	} IPOD_DR_TRACK_CHAPTER_INFO, *pIPOD_DR_TRACK_CHAPTER_INFO;

	typedef struct
	{
		unsigned char *m_String;
		unsigned long m_StringBufSize;
		unsigned long m_StringActualSize;
	} IPOD_DR_TRACK_STRING_DATA_INFO, *pIPOD_DR_TRACK_STRING_DATA_INFO;

	typedef struct
	{
		unsigned char m_PacketInfo;
		unsigned char m_padding;
		unsigned short m_PacketIndex;
		unsigned char *m_Lyrics;
		unsigned long m_LyricsBufSize;
		unsigned long m_LyricsActualSize;
	} IPOD_DR_TRACK_LYRICS, *pIPOD_DR_TRACK_LYRICS;

	typedef struct
	{
		unsigned short m_FormatID;
		unsigned short m_ArtworkCnt;
	} IPOD_DR_TRACK_ARTWORK_, *pIPOD_DR_TRACK_ARTWORK_;

	typedef struct
	{
		unsigned long m_ActualCnt;
		IPOD_DR_TRACK_ARTWORK_ m_ArtworkCnt[IPOD_MAX_ARTWORK];
	} IPOD_DR_TRACK_ARTWORK, *pIPOD_DR_TRACK_ARTWORK;

	typedef union
	{
		IPOD_DR_TRACK_CAP_INFO m_capInfo;
		IPOD_DR_TRACK_CHAPTER_INFO m_ChapterInfo;
		IPOD_DR_TRACK_STRING_DATA_INFO m_StringInfo;
		IPOD_DR_TRACK_LYRICS m_lyrics;
		IPOD_DR_TRACK_ARTWORK m_Artwork;
	} IPOD_DR_TRACK_INFO, *pIPOD_DR_TRACK_INFO;

	typedef struct
	{
		IPOD_DR_TRACK_INFO_TYPE m_type;
		unsigned char m_padding_1;
		unsigned short m_ChapterIndex;
		unsigned long m_TrackIndex;
		IPOD_DR_TRACK_INFO m_TrackInfo;
	} IPOD_DR_TRACK_INFO_PARAM, *pIPOD_DR_TRACK_INFO_PARAM;




	/*************************************************************************/
	/*Struct                                                                                                                    */
	/*************************************************************************/
#define HID_REPORT_MAX	100

	typedef struct
	{
		void *iPodHIDHandle;
		void *iPodBTHandle;
		void *iPodAuthHandle;
	} IPOD_IO_HANDLE;

	typedef struct tag_IPOD_CURIDX_FORMATINFO
	{
		short FormatID[IPOD_MAX_ARTWORK];
		short CountOfImage[IPOD_MAX_ARTWORK];
	} IPOD_CURIDX_FORMATINFO, pIPOD_CURIDX_FORMATINFO;


//Command:0x000D Return Information for Indexed Track : Track Information Structure
//Capability bit field : valid only Bit0 - Bit6
//Bit0 : 1 = Track is audiobook
//Bit1 : 1 = Track has chapters
//Bit2 : 1 = Track has album artwork
//Bit3 : 1 = Track has song lyrics
//Bit4 : 1 = Track is a podcast episode
//Bit5 : 1 = Track has release date
//Bit6 : 1 = Track has description
	typedef struct
	{
		/*response of CAPAINFO*/
		long Capability; //Track Capability bitfields
		long TotalLength; // Track total length in ms
		short ChapterCount;

		/*response of RELEASEDATE*/
		char Seconds; // 0-59
		char Minutes; // 0- 59
		char Hours; // 0-23
		char Day; // 0- 31
		char Month; // 1-12
		char Weekday; //0-6
		short Year; // 1- xxxx

		/*response of ARTWORKCOUNT*/
		IPOD_CURIDX_FORMATINFO curformatInfo;

		/*
			response of
				PODCASTNAME,	DESCRIPTION,	SONGLYRICS
				GENRE,			COMPOSER
			Unicode
		*/
		char * InfoString;
	} Info_Indexed_Track, *pInfo_Indexed_Track;

	typedef struct
	{
		short FormatID;
		DisplayPixelFormat PixelFormat;
		short ImageWidth;
		short ImageHeight;
	} ArtworkFormat, *pArtworkFormat;

	typedef struct
	{
		char	cPixelFormat;
		short sWidth;
		short sHeight;
		short sTopLeftX;
		short sTopLeftY;
		short sBottomRightX;
		short sBottomRightY;
		unsigned long uiRowSize;
		unsigned long uiTotalSize;
		unsigned char* pImageData;
	} ArtworkData, *pArtworkData;


	typedef struct tag_IPOD_IDENTIFY_TOKEN_VAL
	{
		// Lingo setting //
		unsigned long LingoCnt;
		unsigned char Lingos[IPOD_MAX_LINGO_LEN];
		MLingoType ML_Lingo;
		unsigned long DeviceOption;

		/*
			If the the LINGO_EXTENDED_INTERFACE set on Lingos parameter,
			Enter-extended-mode function called automatically.
			If user doesn't want to call the enter-extended-mode function,
			Please set the EnterExtendedModeOnInitFunc as "IPOD_FUNC_DISABLE"
		*/
		unsigned long EnterExtendedModeOnInit;
	} IPOD_IDENTIFY_TOKEN_VAL, *pIPOD_IDENTIFY_TOKEN_VAL;


	typedef struct tag_IPOD_ACCINFO_TOKEN_VAL
	{
		unsigned long AccInfo;
		unsigned long AccInfoCnt;
		unsigned char Accessory_Name_Info[IPOD_MAX_INFO_STR];  /* UTF-8 String*/
		unsigned char Accessory_Firmware_Ver[3];
		unsigned char Padding_1;
		unsigned char Accessory_Hardware_Ver[3];
		unsigned char Padding_2;
		unsigned char Accessory_manufacturer[IPOD_MAX_INFO_STR];  /* UTF-8 String*/
		unsigned char Accessory_modelNum[IPOD_MAX_INFO_STR];  /* UTF-8 String*/
		unsigned char Accessory_serialNum[IPOD_MAX_INFO_STR];  /* UTF-8 String*/
		unsigned short Accessory_incommingMaximumPayloadSize;
		unsigned char Padding_3;
		unsigned char Padding_4;
		unsigned long Accessory_RF_Certifications;
		unsigned long Accessory_status; /*Do not use*/

	} IPOD_ACCINFO_TOKEN_VAL, *pIPOD_ACCINFO_TOKEN_VAL;

	typedef struct tag_IPOD_ACC_PREF_SETTING_FORMAT
	{
		/*Please refer tothe */
		unsigned char m_ClassID;
		unsigned char m_SettingID;
		RestoreOnExit m_RestoreOnExit;
	} IPOD_ACC_PREF_SETTING_FORMAT, *pIPOD_ACC_PREF_SETTING_FORMAT;

	typedef struct tag_IPOD_ACCPREF_TOKEN_VAL
	{
		unsigned long Total_PreferenceCnt;
		IPOD_ACC_PREF_SETTING_FORMAT Pref[IPOD_PREF_MAX_CNT];

	} IPOD_ACCPREF_TOKEN_VAL, *pIPOD_ACCPREF_TOKEN_VAL;

	typedef struct tag_IPOD_SDK_PROTOCOL_DATA
	{
		unsigned char m_protocolIndex;
		unsigned char m_ProtocolStringSize;
		unsigned short m_padding_;
		unsigned char *m_pProtocolString;
	} IPOD_SDK_PROTOCOL_DATA, *pIPOD_SDK_PROTOCOL_DATA;

	typedef struct tag_IPOD_SDK_PROTOCOL_TOKEN
	{
		unsigned long Total_ProtocolCnt;
		IPOD_SDK_PROTOCOL_DATA m_protocolTokenData[IPOD_SDK_PROTOCOL_MAX_CNT];

	} IPOD_SDK_PROTOCOL_TOKEN, *pIPOD_SDK_PROTOCOL_TOKEN;

	typedef struct tag_IPOD_BUNDLE_SEED_ID_PREF_TOKEN
	{
		unsigned char m_BundleSeedIDString[IPOD_BUNDLE_SEEDID_STRING_SIZE];

		/* If user want to add Bundle Seed ID string, Please set the m_BundleSeedIDIsExist as '1'*/
		unsigned char m_BundleSeedIDIsExist;
	} IPOD_BUNDLE_SEED_ID_PREF_TOKEN, *pIPOD_BUNDLE_SEED_ID_PREF_TOKEN;
	typedef struct tag_IPOD_SCREEN_INFO_TOKEN
	{
		unsigned short totalScreenWidthInches;
		unsigned short totalScreenHeightInches;
		unsigned short totalScreenWidthPixels;
		unsigned short totalScreenHeightPixels;
		unsigned short iPodOutScreenWidthPixels;
		unsigned short iPodOutScreenHeightPixels;
		unsigned char screenFeatureMask;
		unsigned char screenGammaValue;
		unsigned char padding1;
		unsigned char padding2;
	} IPOD_SCREEN_INFO_TOKEN, *pIPOD_SCREEN_INFO_TOKEN;

	typedef struct tag_IPOD_EA_PROTOCOL_METADATA_TOKEN
	{
		unsigned char protocolIndex;
		unsigned char metadataType;

		/* If user want to add EA_PROTOCOL_METADATA_TOKEN, Please set the m_EAProtocolMetadataTokenIsExist as '1'*/
		unsigned char m_EAProtocolMetadataTokenIsExist;
	}IPOD_EA_PROTOCOL_METADATA_TOKEN, *pIPOD_EA_PROTOCOL_METADATA_TOKEN;





	typedef struct tag_IPOD_BUFF_VAL
	{
		unsigned char *CP_buffer;
		unsigned long CP_buffer_size;

		unsigned char *Tx_buffer;
		unsigned long Tx_buffer_size;

		unsigned char *Rx_buffer;
		unsigned long Rx_buffer_size;

		unsigned char *RxData_buffer;
		unsigned long RxData_buffer_size;

		unsigned char *UTF8DecodeBuffer;
		unsigned long UTF8DecodeBuffer_size;

	} IPOD_BUF_PARAM, *pIPOD_BUF_PARAM;

	typedef struct tag_IPOD_DEV_HANDLE_DES
	{
		unsigned long DevNo_Handle; //nucleus dev_no
	} IPOD_DEV_HANDLE_DES, *pIPOD_DEV_HANDLE_DES;

	typedef struct tag_IPOD_INIT_CHANNEL_PARAM
	{
		unsigned long ExtInfo;
		IPOD_BUF_PARAM buff;
		IPOD_DEV_HANDLE_DES DevHandleDes;
	} IPOD_INIT_CHANNER_PARAM, *pIPOD_INIT_CHANNER_PARAM;

	typedef struct tag_IPOD_INIT_PARAM
	{
		IPOD_IDENTIFY_TOKEN_VAL IdentifyToken; /*Required*/
		IPOD_ACCCAPS_TOCKEN_VAL AccesoryCapToken; /*Required*/
		pIPOD_ACCINFO_TOKEN_VAL pAccInfoToken; /*Required*/
		IPOD_ACCPREF_TOKEN_VAL AccPrefToken; /*Required*/
		IPOD_SDK_PROTOCOL_TOKEN m_SDKProtocolToken; /*Required*/
		IPOD_BUNDLE_SEED_ID_PREF_TOKEN m_BundleSeedIDPrefToken; /*Required*/
		//IPOD_SCREEN_INFO_TOKEN m_ScreenInfoToken; /*Deprecated*/
		IPOD_EA_PROTOCOL_METADATA_TOKEN EAProtocolMetaToken; /*Optional*/
	} IPOD_INIT_PARAM, *pIPOD_INIT_PARAM;

	typedef struct tag_IPOD_LINGO_VER
	{
		unsigned long Major;
		unsigned long Minor;

	} IPOD_LINGO_VER, *pIPOD_LINGO_VER;

	typedef union tag_IPOD_LINGO_INFO
	{
		unsigned long long LingoOption[LINGO_MAX];
		IPOD_LINGO_VER LingoVer[LINGO_MAX];
	} IPOD_LINGO_INFO, *pIPOD_LINGO_INFO;


	typedef struct tag_IPOD_LINGO_INFO_DATA
	{
		/*
			If the m_IsVersion is set as '1', Please access m_LingoInfo.LingoVer.
			If the m_IsVersion is set as '0', Please access m_LingoInfo.LingoOption.
		*/
		unsigned long m_IsVerison;
		IPOD_LINGO_INFO m_LingoInfo;
		IPOD_ERROR m_Lingo_result[LINGO_MAX];
	} IPOD_LINGO_INFO_DATA, *pIPOD_LINGO_INFO_DATA;

	typedef struct tag_IPOD_cmd_param
	{
		void *m_iPod_Handle;
		unsigned short m_TransID;

		/* IPOD_FLAG - USER setting */
		unsigned char m_Flag;

		/* Internal Ex_flag */
		unsigned char m_Ex_Flag;

		unsigned long m_CurCmd;

		unsigned long m_SendFunc;

		unsigned long m_TimeOutFlag;
		unsigned long m_TimeOut;

		unsigned char *m_SendCmdBuf;
		unsigned long m_SendCmdBufSize;

		unsigned long m_ExecutionTime;

		unsigned char *m_ResponseBuf;
		unsigned long m_ResponseBufSize;

	} IPOD_CMD_PARAM, *pIPOD_CMD_PARAM;

	typedef struct
	{
		long DBindex;
		long ActualLen;
		unsigned char Data[IPOD_UTF8DECODE_STRINGBUFSIZE];

	} IPOD_RECORD_TYPE, *PIPOD_RECORD_TYPE;




	/*************************************************************************/
	/*enum                                                                                                                    */
	/*************************************************************************/
	enum
	{
		IPOD_UFD_REMOVED,
		IPOD_UFD_INSERTED
	};

	typedef enum
	{
		IPOD_AUDIO_TRACK = 0x00,
		IPOD_VIDEO_TRACK = 0x01


	} IPOD_PLAYBACK_MODE;

//IPOD status
	typedef enum
	{
		IPOD_UNPLUGED = 0x00000000,
		IPOD_PLUGED = 0x00000001,
		IPOD_INITIALIZED = 0x00000002,
		IPOD_MOUNTED = 0x00000003,
		IPOD_UNMOUNTED = 0x00000004,
		IPOD_COMPLETED = 0x00000005,

		IPOD_STATUS_MAX= 0x7FFFFFFF
	} IPOD_STATE;

//Track Capabilities
	typedef enum
	{
		IPOD_CAPABILITIES_AUDIOBOOK = ((unsigned long)0x01<<0),
		IPOD_CAPABILITIES_CHAPTERS = ((unsigned long)0x01<<1),
		IPOD_CAPABILITIES_ALBUM_ARTWORK = ((unsigned long)0x01<<2),
		IPOD_CAPABILITIES_SONG_LYRICS = ((unsigned long)0x01<<3),
		IPOD_CAPABILITIES_PODCAST_EPISODE = ((unsigned long)0x01<<4),
		IPOD_CAPABILITIES_RELEASE_DATE = ((unsigned long)0x01<<5),
		IPOD_CAPABILITIES_DESCRIPTION = ((unsigned long)0x01<<6),
		IPOD_CAPABILITIES_CONTAINS_VIDEO = ((unsigned long)0x01<<7),
		IPOD_CAPABILITIES_CURRENTLY_QUEUED_TO_PLAY_AS_VIDEO = ((unsigned long)0x01<<8),
		IPOD_CAPABILITIES_CAPABLE_OF_GENERATING_GENIUS_PLAYLIST = ((unsigned long)0x01<<13),
		IPOD_CAPABILITIES_ITUNESU_EPISODE = ((unsigned long)0x01<<14)
	} IPOD_CAPABILITIES;

	typedef enum
	{
		MUSICAPP = 0,
		OTHERSAPP = 1
	}IPOD_APP_TYPE;
	
	typedef struct tag_IPOD_TRACK_CAPABILITIES_INFO
	{
		unsigned char m_Audiobook;
		unsigned char m_Chapter;
		unsigned char m_AlbumArtwork;
		unsigned char m_SongLyrics;
		unsigned char m_PodcastEpisode;
		unsigned char m_ReleaseDate;
		unsigned char m_Description;
		unsigned char m_ContainsVideo;
		unsigned char m_CurrentlyQueuedToPlayAsVideo;
		unsigned char m_CapableOfGeneratingGeniusPlaylist;
		unsigned char m_ItunseUEpisode;
		unsigned char m_padding;
	} IPOD_TRACK_CAPABILITIES_INFO, *pIPOD_TRACK_CAPABILITIES_INFO;

	typedef struct tag_IPOD_STATUS
	{
		IPOD_STATE ipod_state;
		unsigned long DevNo;
		/* 	void *iPod_Handle; */
		unsigned long SamplingFreq;
	} IPOD_BASIC_INFO, *pIPOD_INFO;


	typedef struct tag_IPOD_ARTWORK_INFO
	{
		unsigned long BasicFormatID;
		unsigned long CapableFormatID[IPOD_MAX_ARTWORK];
		unsigned long CapableSize[IPOD_MAX_ARTWORK];
		unsigned long MaxDisplaySize;
		unsigned long MaxFormatCount;
	} IPOD_ARTWORK_INFO, *pIPOD_ARTWORK_INFO;

#define IPOD_NOT_ESTABLISED_SESSIONGID (0xFFFFFFFF)
#define IPOD_PLAY_INFO_MAX_CNT 6

	/*************************************************************************/
	/*Information General                                          */
	/*************************************************************************/

	typedef struct tag_IPOD_GEN_FLOWCONTROL_INFO
	{
		unsigned int GEN_WaitTimeMs;
		unsigned short GEN_OverflowTransID;
	} IPOD_GEN_FLOWCONTROL_INFO; /*0x02*/

	typedef struct tag_IPOD_GEN_RADIOTAGGING_INFO
	{
		unsigned char GEN_TagStatus;
	} IPOD_GEN_RADIOTAGGING_INFO;/*0x03*/

	typedef struct tag_IPOD_GEN_CAMERA_INFO
	{
		unsigned char GEN_CameraNoti;
	} IPOD_GEN_CAMERA_INFO;/*0x04*/

	typedef struct tag_IPOD_GEN_CHARGING_INFO
	{
		unsigned char GEN_ChargingInfoType;
		unsigned short GEN_ChargingInfoValue;
	} IPOD_GEN_CHARGING_INFO;/*0x05*/

	typedef struct tag_IPOD_GEN_DATABASE_CHANGED
	{
		unsigned char GEN_DatabaseChangedNoti;
	}IPOD_GEN_DATABASE_CHANGED;/*0x09*/
	
	typedef struct tag_IPOD_GEN_NOWPLAYINGAPP_BUNDLENAME_STATUS
	{
		unsigned char GEN_NowPlayingApplicationBundleName[128];
	} IPOD_GEN_NOWPLAYINGAPP_BUNDLENAME_STATUS; /*0x0A*/

	typedef struct tag_IPOD_GEN_SESSIONSPACE_AVAILALBE_STATUS
	{
		unsigned short GEN_SessionID;
	} IPOD_GEN_SESSIONSPACE_AVAILALBE_STATUS; /*0x0B*/

	typedef struct tag_IPOD_GEN_COMMAND_COMPLETE
	{
		unsigned char GEN_LingoID;
		unsigned short GEN_CmdID;
		unsigned char GEN_CommandCompletionStatus;
	} IPOD_GEN_COMMAND_COMPLETE;/*0x0D*/

	typedef struct tag_IPOD_GEN_IPODOUTMODE
	{
		unsigned char GEN_iPodOutModeStatus;
	} IPOD_GEN_IPODOUTMODE; /*0x0F*/

	typedef struct tag_IPOD_GEN_BLUETOOTH_CONNECTION_STATUS
	{
		unsigned char GEN_BTMacAddress[6];
		unsigned long long GEN_BTProfileID;
	} IPOD_GEN_BLUETOOTH_CONNECTION_STATUS;/*0x11*/

	typedef struct tag_IPOD_GEN_NOWPLAYINGAPP_DISPLAYNAME_STATUS
	{
		unsigned char GEN_NowPlayingApplicationDisplayName[128];
	} IPOD_GEN_NOWPLAYINGAPP_DISPLAYNAME_STATUS;/*0x13*/

	typedef struct tag_IPOD_GEN_ASSISTIVETOUCH_STATUS
	{
		unsigned char GEN_AssistiveTouchNoti;
	} IPOD_GEN_ASSISTIVETOUCH_STATUS;/*0x14*/



	/*************************************************************************/
	/*Information Display Remote                                             */
	/*************************************************************************/
	typedef struct tag_IPOD_DR_CHAPTER_INFO
	{
		unsigned long DR_CurrentPlayTrack;
		unsigned short DR_ChapterCountOfTrack;
		unsigned short DR_ChapterIndex;
	} IPOD_DR_CHAPTER_INFO;

	typedef struct tag_IPOD_DR_POWER_BATTERY_INFO
	{
		unsigned char DR_PowerState;
		unsigned char DR_BatteryLevel;
	} IPOD_DR_POWER_BATTERY_INFO;

	typedef struct tag_IPOD_DR_DATETIME_INFO
	{
		unsigned short DR_CurrentYear;
		unsigned char DR_CurrentMonth;
		unsigned char DR_CurrentDayOfTheMonth;
		unsigned char DR_CurrentHour;
		unsigned char DR_CurrentMinute;
	} IPOD_DR_DATETIME_INFO;

	typedef struct tag_IPOD_DR_VOL_INFO
	{
		unsigned char DR_MuteState;
		unsigned char DR_UIVolumeLevel;
		unsigned char DR_AbsoluteVolumeLevel;
	} IPOD_DR_VOL_INFO;

	/*************************************************************************/
	/*Information Extended interface                                         */
	/*************************************************************************/
	typedef struct tag_IPOD_EX_TRACK_CAPABILITIES_INFO
	{
		unsigned char EX_Audiobook;
		unsigned char EX_Chapter;
		unsigned char EX_AlbumArtwork;
		unsigned char EX_SongLyrics;
		unsigned char EX_PodcastEpisode;
		unsigned char EX_ReleaseDate;
		unsigned char EX_Description;
		unsigned char EX_ContainsVideo;
		unsigned char EX_CurrentlyQueuedToPlayAsVideo;
		unsigned char EX_CapableOfGeneratingGeniusPlaylist;
		unsigned char EX_ItunseUEpisode;
		unsigned char EX_padding;
	} IPOD_EX_TRACK_CAPABILITIES_INFO;


	typedef struct tag_IPOD_GENERAL_INFO
	{
		
		IPOD_GEN_FLOWCONTROL_INFO GEN_FlowControl;
		IPOD_GEN_RADIOTAGGING_INFO GEN_RadioTagging;
		IPOD_GEN_CAMERA_INFO GEN_CameraInfo;
		IPOD_GEN_CHARGING_INFO GEN_Charging;
		IPOD_GEN_DATABASE_CHANGED GEN_DatabaseChanged;
		IPOD_GEN_NOWPLAYINGAPP_BUNDLENAME_STATUS GEN_NowPlayingAppBundleName;
		IPOD_GEN_SESSIONSPACE_AVAILALBE_STATUS GEN_SessionSpaceAvailable;
		IPOD_GEN_COMMAND_COMPLETE GEN_CommandComplete;
		IPOD_GEN_IPODOUTMODE GEN_iPodOutModeStatus;
		IPOD_GEN_BLUETOOTH_CONNECTION_STATUS GEN_BTConnectionStatus;
		IPOD_GEN_NOWPLAYINGAPP_DISPLAYNAME_STATUS GEN_NowPlayingAppDisplayName;
		IPOD_GEN_ASSISTIVETOUCH_STATUS GEN_AssistiveTouch;
	} IPOD_GENERAL_INFO;

	typedef struct tag_IPOD_DISPLAY_REMOTE_INFO
	{
		unsigned long DR_TrackPosition;
		unsigned long DR_TrackIndex;
		IPOD_DR_CHAPTER_INFO DR_ChapterInfo;
		IPOD_DR_PLAY_STATUS DR_PlayStatus;
		IPOD_DR_POWER_BATTERY_INFO DR_PowerBattery;
		unsigned long DR_EqualizerState;
		unsigned char DR_Shuffle;
		unsigned char DR_Repeat;
		IPOD_DR_DATETIME_INFO DR_DateTime;
		unsigned char DR_Backlight;
		unsigned char DR_HoldSwitch;
		unsigned char DR_SoundCheck;
		unsigned char DR_AudioBookSpeed;
		unsigned short DR_TrackPositionInSecond;
		IPOD_DR_VOL_INFO DR_Volume;
		IPOD_CAPABILITIES DR_TrackCapabilities;
		unsigned long DR_NewPlaylistTrackNumber;
	} IPOD_DISPLAY_REMOTE_INFO;

	typedef struct tag_IPOD_EXTENDED_INTERFACE_INFO
	{
		/*Playback stopped*/
		unsigned int EX_TrackIndex;/*Track index*/
		/*Playback FFW seek stop*/
		/*Playback REW seek stop*/
		unsigned int EX_TrackTimeOffsetMs;/*Track time offset*/
		unsigned int EX_ChapterIndex;/*Chapter index*/
		IPOD_EX_PLAY_STATUS EX_PlaybackState;/*Playback status extended*/
		unsigned int EX_TrackTimeOffsetSec;/*Track time offset*/
		unsigned int EX_ChapterTimeOffsetMs;/*Chapter time offset(ms)*/
		unsigned int EX_ChapterTimerOffsetSec;/*Chapter time offset(s)*/
		unsigned long long EX_TrackUniqueID;/*Track unique identifier*/
		IPOD_EX_PLAY_MODE EX_TrackPlaybackMode;/*Track playback mode*/
		/*Track lyrics ready*/
		IPOD_EX_TRACK_CAPABILITIES_INFO EX_TrackCapabilities;/*Track capabilities changed*/
		unsigned int EX_NewPlaylistTrackNumber;/*Playback engine contents changed*/
	} IPOD_EXTENDED_INTERFACE_INFO;


	/*************************************************************************/
	/*Information for User													 */
	/*************************************************************************/
	typedef struct tag_IPOD_PLAYINFO
	{
		/*Notification Informataion*/
		IPOD_DISPLAY_REMOTE_INFO DisplayRemoteInfo;
		IPOD_EXTENDED_INTERFACE_INFO ExtendedInterfaceInfo;
		IPOD_GENERAL_INFO GeneralInfo;

		/* User can add up the member variables*/
		unsigned int m_Status;/* Do not modify - Status member*/
		unsigned int DisconnecEvt;/* If Application detect the disconnection of iPod, Please set the below member*/
		unsigned char m_SupportMode;/* Support Audio,Video,iPodOut Mode*/
		unsigned char *pBuffer;
		IPOD_ARTWORK_INFO ArtworkInfo;

		unsigned int TotalPlayNum; 
		unsigned int CurrentIdx;
		unsigned int TotalTime;
		unsigned int ShuffleStatus;
		unsigned int RepeatStatus;
		unsigned long PostionMs;
		unsigned int Disk;
		IPOD_BASIC_INFO Basic_Info;
		/*For iPod OS application */
		unsigned long m_iPod_OSApp_DataSesiongID;
		unsigned long m_iPod_OSApp_ProtocolIndex;
		/*For iPod Flow Control */
		unsigned int m_Noti_WaitTime;
		unsigned short m_Noti_OverflowTransID;
		CategoryType mRootCategory;
		CategoryType mCurrentCategory;        
	} IPOD_PLAY_INFO, *pIPOD_PLAY_INFO;




	/*************************************************************************/
	/*global Variable                                                                                                                 */
	/*************************************************************************/

	extern IPOD_ACCINFO_TOKEN_VAL gIPOD_AccInfo;
	extern const IPOD_ACC_PREF_SETTING_FORMAT gIPOD_Pref[IPOD_PREF_MAX_CNT];
	extern IPOD_PLAY_INFO gIPOD_PlayInfo[IPOD_PLAY_INFO_MAX_CNT];

	extern volatile unsigned long IPOD_RX_W_Ptr;	// For filling receive buffer
	extern volatile unsigned long IPOD_RX_W_Ptr2;	// For filling receive buffer

	extern unsigned char *pIPOD_RxBuff;			//rx buffer pointer
	extern unsigned char *pIPOD_RxBuff2;			//rx buffer pointer

	extern long IPOD_UFDStatusFlag;
	extern unsigned char media_kind[10][40];




	/********************************************************************************************************************/
	/* Functions                                                                                                        */
	/* (send-response) lingo command name                                                                               */
	/********************************************************************************************************************/

	/********************************************************************************************************************/
	/* Extended Interface Lingo                                                                                         */
	/********************************************************************************************************************/
	/*0x0002-0x0003*/	extern IPOD_ERROR IPOD_GetCurrentPlayingTrackChapterInfo(void *iPod_Handle, long *chapterIndex, long * chapterCount);
	/*0x0004-0x0001*/	extern IPOD_ERROR IPOD_SetCurrentPlayingTrackChapter(void *iPod_Handle, unsigned long index);
	/*0x0005-0x0006*/	extern IPOD_ERROR IPOD_GetCurrentPlayingTrackChapterPlayStatus(void *iPod_Handle, unsigned long Chapter_index, unsigned long *ChapterLen, unsigned long *ElapsedTime);
	/*0x0007-0x0008*/	extern IPOD_ERROR IPOD_GetCurrentPlayingTrackChapterName(void *iPod_Handle, unsigned long index, unsigned char *pChapterName, unsigned long BufLen, unsigned long *pActualLen);
	/*0x0009-0x000A*/	extern IPOD_ERROR IPOD_GetAudioBookSpeed(void *iPod_Handle,  unsigned char * speed);
	/*0x000B-0x0001*/	extern IPOD_ERROR IPOD_SetAudioBookSpeed(void *iPod_Handle, AudiobookSpeed speed , unsigned char GlobalSetting, unsigned char RestoreOnExit);
	/*0x000C-0x000D*/	extern IPOD_ERROR IPOD_GetIndexedPlayingTrackInfo(void *iPod_Handle, TrackInformationType type, pInfo_Indexed_Track pTrackInfo, unsigned long trackindex, unsigned long chapterindex, long BufLen, long *pActualLen);
	/*0x000E-0x000F*/	extern IPOD_ERROR IPOD_GetArtworkFormats(void *iPod_Handle, pArtworkFormat pArtFormatData, long FormatCnt, long *AcutalFormatCnt);
	/*0x0010-0x0011*/	extern IPOD_ERROR IPOD_GetTrackArtworkData(void *iPod_Handle, unsigned long trackIndex, short formatID, long timeOffset, ArtworkData *pData, unsigned long BufLen, unsigned long *ActualLen);
	/*0x0016-0x0001*/	extern IPOD_ERROR IPOD_ResetDBSelection(void *iPod_Handle);
	/*0x0017-0x0001*/	extern IPOD_ERROR IPOD_SelectDBRecord(void *iPod_Handle, CategoryType type, long index);
	/*0x0018-0x0019*/	extern IPOD_ERROR IPOD_GetNumberCategorizedDBRecords(void *iPod_Handle, CategoryType type, long *TotalListNum);
	/*0x001A-0x001B*/	extern IPOD_ERROR IPOD_RetrieveCategorizedDatabaseRecords(void *iPod_Handle, CategoryType type, long index, long num, IPOD_RECORD_TYPE *RecordBuf);
	/*0x001C-0x001D*/	extern IPOD_ERROR IPOD_GetPlayStatus(void *iPod_Handle, long *TotalTracklength, long *Curtime_ms, IPOD_GETPLAYSTATUS *PlayStatus);
	/*0x001E-0x001F*/	extern IPOD_ERROR IPOD_GetCurrentPlayingTrackIndex(void *iPod_Handle, long *Index);
	/*0x0020-0x0021*/	extern IPOD_ERROR IPOD_GetIndexedPlayingTrackTitle(void *iPod_Handle, long index, unsigned char *pTitle, long BufLen, long *ActualLen);
	/*0x0022-0x0023*/	extern IPOD_ERROR IPOD_GetIndexedPlayingTrackArtistName(void *iPod_Handle, long index, unsigned char *pArtist, long BufLen, long *ActualLen);
	/*0x0024-0x0025*/	extern IPOD_ERROR IPOD_GetIndexedPlayingTrackAblumName(void *iPod_Handle, long index, unsigned char *pAlbum, long BufLen, long *ActualLen);
	/*0x0026-0x0001*/	extern IPOD_ERROR IPOD_SetPlayStatusChangeNotification(void *iPod_Handle, unsigned long setting, unsigned long Extended);	
	/*0x0029-0x0001*/	extern IPOD_ERROR IPOD_PlayControl(void *iPod_Handle, PlayCmdCode cmdcode);
	/*0x002A-0x002B*/	extern IPOD_ERROR IPOD_GetTrackArtworkTimes(void *iPod_Handle, long trackindex, 
																					short formatID, short artworkIndex, short artworkCount,
																					unsigned long *outputData, unsigned long maxCount);
	/*0x002C-0x002D*/	extern IPOD_ERROR IPOD_GetShuffle(void *iPod_Handle, ShuffleMode *mode);
	/*0x002E-0x0001*/	extern IPOD_ERROR IPOD_SetShuffle(void *iPod_Handle, ShuffleMode mode, RestoreOnExit RestoreOn);
	/*0x002F-0x0030*/	extern IPOD_ERROR IPOD_GetRepeat(void *iPod_Handle, CurrentRepeatState *mode);
	/*0x0031-0x0001*/	extern IPOD_ERROR IPOD_SetRepeat(void *iPod_Handle, CurrentRepeatState mode, RestoreOnExit RestoreOn);	
	/*0x0032-0x0001*/	extern IPOD_ERROR IPOD_SetDisplayImage(void *iPod_Handle, IPOD_PIXEL_FORMAT format, long width, long height, unsigned char * buffer);
	/*0x0033-0x0034*/	extern IPOD_ERROR IPOD_GetMonoDisplayImageLimits(void *iPod_Handle, IPOD_PIXEL_FORMAT * format, long * width, long * height);
	/*0x0035-0x0036*/	extern IPOD_ERROR IPOD_GetNumPlayingTracks(void *iPod_Handle, long *TotalTrackNum);
	/*0x0037-0x0001*/	extern IPOD_ERROR IPOD_SetCurrentPlayingTrack(void *iPod_Handle, long index);
	/*0x0039-0x003A*/	extern IPOD_ERROR IPOD_GetColorDisplayLimits(void *iPod_Handle, IPOD_PIXEL_FORMAT *format, long *width, long *height);
	/*0x003B-0x0001*/	extern IPOD_ERROR IPOD_ResetDBSelectionHierarchy(void *iPod_Handle, IPOD_SEL_DB_TYPE DB_type);
 	/*0x000C-0x000D
 	  0x002A-0x002B
 	  0x0010-0x0011*/	extern IPOD_ERROR IPOD_GetAlbumArt(void *iPod_Handle, unsigned long index, pArtworkData pData,  unsigned long BufLen, unsigned long *pActualLen);
	/********************************************************************************************************************/
	/* Deprecated Extended Interface Lingo                                                                              */
	/********************************************************************************************************************/
	/*0x000F-0x0010*/	extern IPOD_ERROR IPOD_RequestProtocolVersion(void *iPod_Handle, long *major, long *minor);
	/*0x0028-0x0001*/	extern IPOD_ERROR IPOD_PlayCurrentSelection(void *iPod_Handle, long index);
	/*0x0038-0x0001*/	extern IPOD_ERROR IPOD_SelectSortDBRecord(void *iPod_Handle, CategoryType type, long index, SortOrder sort);

	//	extern IPOD_ERROR IPOD_GetVideoMediaKindCnt(void *iPod_Handle, long *pMedia_kind_record_cnt);
	//	extern IPOD_ERROR IPOD_SelectVedeoDBRecord(void *iPod_Handle, long Total_Media_kind_record_cnt, long index);

	/********************************************************************************************************************/
	/* General Lingo                                                                                                    */
	/********************************************************************************************************************/
	/*0x01*/		extern IPOD_ERROR IPOD_RequestIdentify(void *iPod_Handle, IPOD_LINGOS Lingo);
	/*0x06-0x02*/	extern IPOD_ERROR IPOD_ExitExtendedInterfaceMode(void *iPod_Handle);
	/*0x07-0x08*/	extern IPOD_ERROR IPOD_RequestiPodName(void *iPod_Handle, unsigned char *iPodNameStr, long BufLen, long *ActualLen);
	/*0x09-0x0A*/	extern IPOD_ERROR IPOD_RequestiPodSoftwareVersion(void *iPod_Handle, unsigned char* major, unsigned char* minor, unsigned char* revision);
	/*0x0B-0x0C*/	extern IPOD_ERROR IPOD_RequestiPodSerialNum(void *iPod_Handle, unsigned char *SerialStr, long BufLen, long *ActualLen);
	/*0x0F-0x10*/	extern IPOD_ERROR IPOD_RequestLingoProtocolVersion(void *iPod_Handle, long lingo, long *inform, long *major, long *minor);
	/*0x11-0x12*/	extern IPOD_ERROR IPOD_RequestTransportMaxPayloadSize(void *iPod_Handle, unsigned long *maxPayload);

	/*0x24-0x25*/	extern IPOD_ERROR IPOD_GetiPodOptions(void *iPod_Handle, unsigned char *Option);
	/*0x29-0x2A*/	extern IPOD_ERROR IPOD_GetiPodPreferences(void *iPod_Handle, IPOD_PREFERENCE_CLASSID ClassID, unsigned char *settingID);
	/*0x2B-0x02*/	extern IPOD_ERROR IPOD_SetiPodPreferences(void *iPod_Handle, IPOD_PREFERENCE_CLASSID ClassID, unsigned char settingID, RestoreOnExit RestoreOn);

//	/*0x37-0x02*/	extern IPOD_ERROR IPOD_SetUIMode(void *iPod_Handle, unsigned char mode);
	/*0x42-0x02*/	extern IPOD_ERROR IPOD_AccessoryDataTransfer( void *iPod_Handle, unsigned long SessingID, unsigned char *pData, unsigned long nDataSize, IPOD_DATATRANSFER_TYPE iPodDataTransferType, unsigned long *RetryTransID, unsigned long *RetryDataCount);
	/*0x4B-0x4C*/	extern IPOD_ERROR IPOD_GetiPodOptionForLingo(void *iPod_Handle, pIPOD_INIT_PARAM pParam);
	/*0x4F-0x51*/	extern IPOD_ERROR IPOD_GetSupportedEventNotification(void *iPod_Handle, unsigned long long *eventNotiMask);

	/*0x49-0x02*/	extern IPOD_ERROR IPOD_SetEventNotification(void *iPod_Handle, unsigned long long eventNotiMask);
	/*0x4D-0x4E*/	extern IPOD_ERROR IPOD_GetEventNotification(void *iPod_Handle, unsigned long long *eventNotiMask);
	/*0x64-0x02*/	extern IPOD_ERROR IPOD_RequestApplicationLaunch(void* iPod_Handle, unsigned char *AppString, unsigned long Strlen, unsigned char option); //- option : set to 0x02. if this function returns IPOD_BAD_PARAMETER, repeat this function with set to 0x01 option.

	

	/*0x05-0x02 */	extern IPOD_ERROR IPOD_EnterExtendedInterfaceMode(void *iPod_Handle);
	/*0x0D-0x0E*/	extern IPOD_ERROR IPOD_RequestiPodModelNum(void *iPod_Handle, long *model_num, unsigned char *ModelStr, long BufLen, long *ActualLen);
	/*0x65-0x66*/	extern IPOD_ERROR IPOD_GetNowPlayingApplicationBundleName(void *iPod_Handle, unsigned char *AppBundleNameStr, long BufLen, long *ActualLen);

	/********************************************************************************************************************/
	/* Simple Remote Lingo                                                                                              */
	/********************************************************************************************************************/
	/*0x00*/		extern IPOD_ERROR IPOD_ContextButtonStatus(void *iPod_Handle, IPOD_SIMPLE_BUTTON_CTRL ButtonCtrl);
	/*0x02-0x01*/	extern IPOD_ERROR IPOD_ImageButtonStatus(void *iPod_Handle, IPOD_IMG_BUTTON_CTRL ButtonCtrl);
	/*0x03-0x01*/	extern IPOD_ERROR IPOD_VideoButtonStatus(void *iPod_Handle, IPOD_VIDEO_BUTTON_CTRL ButtonCtrl);
	/*0x04-0x01*/	extern IPOD_ERROR IPOD_AudioButtonStatus(void *iPod_Handle, IPOD_AUDIO_BUTTON_CTRL ButtonCtrl);
	/*0x0E-0x01*/	extern IPOD_ERROR IPOD_iPodOutButtonStatus(void *iPod_Handle, unsigned char ButtonCtrl);

	/********************************************************************************************************************/
	/* iPod Out Lingo                                                                                                   */
	/********************************************************************************************************************/
	/*0x01-0x02*/	extern IPOD_ERROR IPOD_GetiPodOutOptions(void *iPod_Handle, unsigned char enableOptions);
	/*0x03-0x00*/	extern IPOD_ERROR IPOD_SetiPodOutOptions(void *iPod_Handle, unsigned char enableOptions);
	/*0x04-0x00*/	extern IPOD_ERROR IPOD_AccessoryStateChangeEvent(void *iPod_Handle, unsigned char stateChange);



	/********************************************************************************************************************/
	/* Display Remote Lingo                                                                                             */
	/********************************************************************************************************************/
	/*0x01-0x02*/	extern IPOD_ERROR IPOD_DR_GetCurrentEQProfileIndex(void *iPod_Handle, unsigned long *Index);
	/*0x03-0x00*/	extern IPOD_ERROR IPOD_DR_SetCurrentEQProfileIndex(void *iPod_Handle, unsigned long Index, RestoreOnExit bRestoreOnExit);
	/*0x04-0x05*/	extern IPOD_ERROR IPOD_DR_GetNumEQProfiles(void *iPod_Handle, unsigned long *ProfileCnt);
	/*0x06-0x07*/	extern IPOD_ERROR IPOD_DR_GetIndexedEQProfileName(void *iPod_Handle, unsigned long Index, unsigned char *eqProfileName, long BufLen, long *pActualLen);
	/*0x08-0x00*/	extern IPOD_ERROR IPOD_DR_SetRemoteEventNotification(void *iPod_Handle, unsigned long EventMask);
	/*0x0A-0x0B*/	extern IPOD_ERROR IPOD_DR_GetRemoteEventStatus(void *iPod_Handle, unsigned long *EventStatus);
	/*0x0C-0x0D*/	extern IPOD_ERROR IPOD_DR_GetiPodStateInfo(void *iPod_Handle, IPOD_DR_STATUS_INFO InfoType);
	/*0x0E-0x00*/	extern IPOD_ERROR IPOD_DR_SetiPodStateInfo(void *iPod_Handle, pIPOD_DR_SETSTATEINFO_PARAM pStateParam);
	/*0x0F-0x10*/	extern IPOD_ERROR IPOD_DR_GetPlayStatus(void *iPod_Handle, pIPOD_DR_PLAYSTATUS pPlayStatus);
	/*0x11-0x00*/	extern IPOD_ERROR IPOD_DR_SetCurrentPlayingTrack(void *iPod_Handle, unsigned long TrackIndex);
	/*0x12-0x13*/	extern IPOD_ERROR IPOD_DR_GetIndexedPlayingTrackInfo(void *iPod_Handle, pIPOD_DR_TRACK_INFO_PARAM pTrackParam);
	/*0x14-0x15*/	extern IPOD_ERROR IPOD_DR_GetNumPlayingTracks(void *iPod_Handle, unsigned long *numPlayTracks);
	/*0x16-0x17*/	extern IPOD_ERROR IPOD_DR_GetArtworkFormats(void *iPod_Handle, pArtworkFormat pArtFormatData, long FormatCnt, long *AcutalFormatCnt);
	/*0x1A-0x1B*/	extern IPOD_ERROR IPOD_DR_GetPowerBatteryState(void *iPod_Handle, unsigned char *PowerState, unsigned char *BattLevel);
	/*0x1C-0x1D*/	extern IPOD_ERROR IPOD_DR_GetSoundCheckState(void *iPod_Handle, unsigned char *SndChkStatel);
	/*0x1E-0x00*/	extern IPOD_ERROR IPOD_DR_SetSoundCheckState(void *iPod_Handle, unsigned char SndChkState, RestoreOnExit RestoreOn);
	/*0x12-0x13
	  0x18-0x19
	  0x1F-0x20*/	extern IPOD_ERROR IPOD_DR_GetAlbumArt(void *iPod_Handle, unsigned long  index, pArtworkData pData, unsigned long BufLen, unsigned long *pActualLen);
	/********************************************************************************************************************/
	/* Digital Audio Lingo                                                                                              */
	/********************************************************************************************************************/
	/*0x00*/	extern IPOD_ERROR IPOD_AccessoryAck(void *iPod_Handle, unsigned short Info);
	/*0x05-0x01*/	extern IPOD_ERROR IPOD_SetVideoDelay(void *iPod_Handle, long Delay);

	/*complex type*/	
		extern IPOD_ERROR IPOD_API_Init(unsigned long *iPodLibApiHandle, void *driverID, unsigned long block, IPOD_CONNECTION_TYPE connection_type, IPOD_RX_TYPE RxType);
		extern IPOD_ERROR IPOD_API_DeInit(void *iPod_Handle);
		extern IPOD_ERROR IPOD_IdentifyAuth_Process(void *iPod_Handle, pIPOD_INIT_PARAM pParam);
		extern IPOD_ERROR IPOD_CheckiPodStartIDPS(void *iPod_Handle);
		extern IPOD_ERROR IPOD_ProcessiPodEvent(void *iPod_Handle);
		extern IPOD_ERROR IPOD_Check_iPod_status(void *iPod_Handle);
		extern IPOD_ERROR IPOD_GetInitType(void *iPod_Handle, IPOD_INIT_TYPE *type);
		extern void *IPOD_GetExtInfo(void *iPod_Handle);
		extern IPOD_ERROR IPOD_GetGenre(void *iPod_Handle, long index, unsigned char *pGener, long BufLen, long *pActualLen);
		extern IPOD_ERROR IPOD_CheckiPodOption(void *iPod_Handle, IPOD_LINGOS Lingo, pIPOD_LINGO_INFO pLingo_Info, unsigned long *IsVerion);
		extern IPOD_ERROR IPOD_GetVideoMediaKindCnt(void *iPod_Handle, long *pMedia_kind_record_cnt);
		extern IPOD_ERROR IPOD_SelectVedeoDBRecord(void *iPod_Handle, long Total_Media_kind_record_cnt, long index);
	 	
	/*not lingo*/		
		extern void IPOD_ResetBufferPtr(void *iPod_Handle);
		extern void IPOD_RX_Put_Buffer(unsigned char *data, unsigned long len);
		extern void *IPOD_GetEventMsgReadQueue(void *iPod_Handle);
		extern void IPOD_SetiPodIOHandle(void *iPod_Handle, void *piPodIOHandle);
		extern long IPOD_GetDefaultArtworkFormatID(void *iPod_Handle); //use in library. move this function to iPod Library.
		extern IPOD_ERROR IPOD_GetCheckIDPSSupport(void* iPod_Handle, IPOD_DEFAULT_CONNECTION *IDPS_support);  
		extern IPOD_ERROR IPOD_InitChannel(void *iPod_Handle, pIPOD_INIT_CHANNER_PARAM pChannelPrarm, unsigned long Block, IPOD_CONNECTION_TYPE connection_type, IPOD_RX_TYPE RxType);
		extern IPOD_ERROR IPOD_GetOptionForLingo(void *iPod_Handle, IPOD_LINGOS lingo, unsigned long long *OptionForLingo);
		extern IPOD_ERROR IPOD_CheckNewTrackInfoStatus(void *iPod_Handle, unsigned long *status);
		extern unsigned char *IPOD_GetTxBuffer(void *iPod_Handle);
		extern unsigned short IPOD_GetNextTransID(void *iPod_Handle);
		extern unsigned long IPOD_Get_IDPS_DataOffset(void *iPod_Handle);
		extern void *IPOD_GetExtInfo(void *iPod_Handle);
		extern void IPOD_InitVar(void *Res0, void *Res1, void *Res2, void *Res3, void *Res4);
		extern IPOD_ERROR IPOD_GetSupportMultiLingo(void* iPod_Handle, MLingoType *supportMultiLingo);
		extern IPOD_ERROR IPOD_IsNowPlayingAppNofitication(void* iPod_Handle, unsigned long *NowPlayingNotification);

		extern long IPOD_Event_GetReturnPlayInfo(void *iPod_Handle, signed char* CmdData);
		extern long IPOD_Event_DisplayRemoteEvent(void *iPod_Handle, unsigned char EventNum, signed char* pData);
		extern long IPOD_Event_GeneralEvent(void *iPod_Handle, unsigned char EventNum, signed char* pData, unsigned long length);

	/*user command*/
		extern IPOD_ERROR IPOD_USER_CMD_RcvResponse(void *iPod_Handle, unsigned short Lingo, unsigned short Snd_cmd, unsigned short Rcv_cmd, unsigned long TimeOut, unsigned short TransID);
		extern IPOD_ERROR IPOD_USER_FUNC_SendCmd_RcvResponse(pIPOD_CMD_PARAM pCmdParam, unsigned short Lingo, unsigned short Rcv_cmd);
		extern IPOD_ERROR IPOD_USER_EnterCriticalSection(void *iPod_Handle);
		extern IPOD_ERROR IPOD_USER_ExitCriticalSection(void *iPod_Handle);
		extern unsigned char* IPOD_GetLibVer(void);
		extern void IPOD_ClearIDPSStatus(void *iPod_Handle);
		extern void IPOD_SetReStartIDPS(void *iPod_Handle);
		extern void IPOD_StopDataThread(void *iPod_Handle);//IPOD_LIB_TASKINFO ThreadInfo);
		extern void IPOD_StopControlThread(void *iPod_Handle);//IPOD_LIB_TASKINFO ThreadInfo);
		extern void IPOD_SetTimeoutChangeFlag(void *iPod_Handle, unsigned long Flag);

		
#if defined(__cplusplus)
};
#endif


#endif /*  _IPOD_LIB_H_ */
/* End of File */


