/****************************************************************************
 *   FileName    : iPod_PAL.c
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

#include <utils/Log.h>
#include "iPod_PAL.h"
#include "iPod_OS.h"
#include "iPod_CP.h"
#include "iPod_HID.h"
#include "iPod_interface.h"
#include "iPod_notify.h"
#include <utils/Log.h>
#include <cutils/properties.h>

//#define IPOD_DEBUG
#ifndef NULL
#define NULL 0
#endif

#define LOG_TAG "IapService"

int IAPPAL_DEBUG = -1;
#define IAPPAL_DBUG(msg...)	if (IAPPAL_DEBUG == 1) { ALOGD( " " msg); }
#define IAPPAL_ERR(msg...)	if (IAPPAL_DEBUG == 1) { ALOGE( " " msg); }
int gfInitializing = 0;

void IPOD_PAL_SetInitializingFlag(int flag)
{
    gfInitializing = flag;
    ALOGD(" %s flag %d",__FUNCTION__,flag);
}

int IPOD_PAL_GetInitializingFlag()
{
    return gfInitializing;
}

void IPOD_PAL_InitDebug(void)
{
    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.debug.pal", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPPAL_DEBUG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPPAL_DEBUG = 0;
                break;
        }
    }
}

int IPOD_PAL_GetTotalBufSize(void)
{
    return IPOD_TOTALBUFSIZE;
}

unsigned long IPOD_PAL_SetCommandTimeout(void* iPod_Handle, unsigned long Lingo, unsigned long SendCmd, unsigned long TimeOutValue)
{
    unsigned long ret=0;
    /*Example*/
    switch(Lingo)
    {
        case LINGO_GENERAL:{
            switch(SendCmd){
                case 0x07: // RequestiPodName
                    ret = 3000; 
                    break;
                case 0x65: // GetNowPlayingApplicationBundleName /  iphone4 v7.0.6 / 1.8s
                    ret = 3000;
                    break;
                case 0x15:
                    ret = 30000; // RetAccessoryAuthenticationInfo / iPhone 3G // 20s
                    break;
            }
            break;
        }
        case LINGO_DISPLAY_REMOTE:{
            switch(SendCmd){
                case 0x12: // GetIndexedPlayingTrackInfo / iphone4 v7.0.6 / 1.6s
                    ret = 3000;
                    break;
                case 0x18: // GetTrackArtworkData /iphone4 v7.0.6 / 3.0s
                    ret = 5000;
                    break;
                case 0x7A: // GetIndexedPlayingTrackInfo /  iphone4 v7.0.6 / 1.6s
                    ret = 3000;
                    break;
            }
            break;
        }
        case LINGO_EXTENDED_INTERFACE:{
            switch(SendCmd)
            {
                case 0x0016: // ResetDBSelection / iphone4 v7.0.6 / 34s / Appde device off -> connect
                    ret = 40000;
                    break;
                //case 0x0017: // SelectDBRecord / iphone4 v7.0.6 / 54s / Appde device off -> connect
                //    ret = 40000;
                //    break;                    
                case 0x0018: // GetNumberCategorizedDBRecords / iphone5 v8 ? / ? > 3s / 
                    ret = 6000;
                    break;
                case 0x001A: // RetrieveCategorizedDatabaseRecords / iphone4 v7.0.6 / ? > 1.5s 
                    ret = 4000;
                    break;
                case 0x001E: // GetCurrentPlayingTrackIndex / iphone5 v8 ? / ? > 1.5s / 
                    ret = 6000;
                    break;             
                case 0x0029: // PlayControl / phone4 v7.0.6 / < 1.7s / 
                    ret = 4000;
                    break;
                case 0x0020: // GetIndexedPlayingTrackTitle  / iphone4 v7.0.6 / 1.7s
                    ret = 4000;
                    break;
                case 0x0028: // PlayCurrentSelection  / iphone4 v7.0.6 / 1.65s
                    ret= 4000;
                    break;
                case 0x0035:// GetNumPlayingTracks   / iphone4 v7.0.6 /  1.7s
                    ret = 4000; 
                    break;
                case 0x0037: // SetCurrentPlayingTack   / iphone4 v7.0.6 /  3.1s
                    ret = 5000;
                    break;
            }
            break;
        }
    }

    if(IPOD_PAL_GetInitializingFlag() != 0)
    {
        switch(Lingo)
        {
            case LINGO_EXTENDED_INTERFACE:{
                switch(SendCmd)
                {
                    case 0x000E: // GetArtworkFormats
                        ret = 30000;
                        break;
                    case 0x0017: // SelectDBRecord
                        ret = 30000;
                        break;
                    case 0x001C: // GetPlayStatus
                        ret = 10000;
                        break;
                    case 0x0018: // GetNumberCategorizedDBRecords
                        ret = 30000;
                        break;
                    case 0x001E: // GetCurrentPlayingTrackIndex
                        ret = 10000;
                        break;
                    case 0x002C: // GetShuffle
                        ret = 60000;
                        break;
                    case 0x002F: // GetRepeat / iphone4 v7.0.6 / 26s  / Appde device off -> connect
                        ret = 60000;
                        break;
                }
                break;
            }            
        }
    }

    if(ret > 0 && (TimeOutValue*50) > ret){
        ALOGD(" %s timeOutValue %ld ret %ld",__FUNCTION__,TimeOutValue,ret);
        ret = 0;
    }
    
    if(ret > 0)
        IPOD_SetTimeoutChangeFlag(iPod_Handle, 1);
    
    return ret;
}



void IPOD_PAL_UsbAudioSetFullBandwidthInterface(unsigned long DevNo)
{
	/*Do nothing*/
    IPOD_CBACK(ipod_stack_cbacks,iPodCbSetFullBandwidth_cb);
}

void IPOD_PAL_UsbAudioSetCur(unsigned long DevNo, long bitrate)
{
	/*Do nothing*/
}


void IPOD_PAL_EventExtendedInterfaceLingo(void *iPod_Handle, char *pCmdData)
{
	IPOD_Event_GetReturnPlayInfo(iPod_Handle, pCmdData);
}

void IPOD_PAL_EventGeneralLingo(void *iPod_Handle, unsigned char EventNum, char *pCmdData, unsigned long length)
{
	IPOD_Event_GeneralEvent(iPod_Handle, EventNum, pCmdData, length);
}

void IPOD_PAL_EventDisplayRemoteLingo(void *iPod_Handle, unsigned char EventNum, char *pCmdData)
{
	IPOD_Event_DisplayRemoteEvent(iPod_Handle, EventNum, pCmdData);
}

void IPOD_PAL_EventDigitalAudioLingo(void *iPod_Handle, unsigned long bitrate, void *hEventQueueWrite, IPOD_EVENT_ID EventID)
{
	IPOD_EVT_msg EvtMsg;
	EvtMsg.EventID = EventID;
	EvtMsg.Param1 = bitrate;
	IPOD_OS_SendToMessageQueue(hEventQueueWrite, &EvtMsg, sizeof(EvtMsg)/4);
}

/**************************************************************************
*	Function : IPOD_Callback_GetWaitTimeMount
*
*	Description : This function returns mount command timeout value.
*
*	IMPORTANT NOTICE : Please don't use IPOD library API on callback function.
*
*	Parameter :
*
*  	Return : none
*
**************************************************************************/
unsigned long IPOD_Callback_GetWaitTimeMount(void)
{
	return IPOD_TIMEOUT_MOUNT_VALUE*5;
}

/**************************************************************************
*	Function : IPOD_Callback_GetWaitTimeMount
*
*	Description : This function returns command timeout value.
*
*	IMPORTANT NOTICE : Please don't use IPOD library API on callback function.
*
*	Parameter :
*
*  	Return : none
*
**************************************************************************/
unsigned long IPOD_Callback_GetWaitTime(void)
{
	return IPOD_TIMEOUT_VALUE*5;
}


unsigned long IPOD_Callback_DefaultTimeout(void)
{	
	return IPOD_COMMAND_TIMEOUT_VALUE;
}

unsigned long IPOD_Callback_AuthProcess_NoResponseTimeout(void)
{
	return IPOD_AUTHPROCESS_RESPONSE_TIMEOUT_S;
}
unsigned long IPOD_GetStringBufferSize(void)
{
	return IPOD_UTF8DECODE_STRINGBUFSIZE;
}
/**************************************************************************
*	Function : IPOD_SetDisconnectionStatus
*
*	Description : This function set the disconnection status to iPod thread
*
*	Parameter : Mode
*
*  	Return : none
*
**************************************************************************/
void IPOD_SetDisconnectionStatus(void *Handle)
{
	pIPOD_PLAY_INFO pInfo = (pIPOD_PLAY_INFO)IPOD_GetExtInfo(Handle);

	if (pInfo)
	{
		pInfo->DisconnecEvt = 1;
	}
}

/**************************************************************************
*	Function : IPOD_GetDetectStatus
*
*	Description : This function returns iPod connection status.
*
*	Parameter :
*
*  	Return : none
*
**************************************************************************/
IPOD_PORT_STATE IPOD_GetDetectStatus(void *ExtInfo)
{
	IPOD_PORT_STATE lRet = IPOD_DISCONNECTED;
	pIPOD_PLAY_INFO pInfo = (pIPOD_PLAY_INFO)ExtInfo;

	if (pInfo)
	{
		if (pInfo->DisconnecEvt == 1)
		{
			return IPOD_DISCONNECTED;
		}
		else
		{
			return IPOD_CONNECTED;
		}
	}
	else
	{
		return IPOD_INVALID_PARAM;
	}
}



/**************************************************************************
*	Function : IPOD_Get_Oneword
*
*	Description : This function returns one word from iPod receive buffer.
*
*	Parameter :
*		- Data : data pointer
*
*  	Return : one word data
*
**************************************************************************/
unsigned long IPOD_Get_Oneword(const unsigned char *Data)
{
	unsigned long RetData = 0;

	if (Data != 0)
	{
		RetData = ((unsigned long)Data[0] << 24) & 0xFF000000U;
		RetData |= ((unsigned long)Data[1] << 16) & 0xFF0000U;
		RetData |= ((unsigned long)Data[2] << 8) & 0xFF00U;
		RetData |= (unsigned long)Data[3] & 0xFFU;
	}

	return RetData;
}

/**************************************************************************
*	Function : IPOD_Get_Short
*
*	Description : This function returns short size data from iPod receive buffer.
*
*	Parameter :
*		- Data : data pointer
*
*  	Return : short data
*
**************************************************************************/
unsigned short IPOD_Get_Short(const unsigned char *Data)
{
	unsigned short RetData = 0;

	if (Data !=0)
	{
		RetData = ((unsigned short)Data[0] << 8) & 0xFF00U;
		RetData |= (unsigned short)Data[1] & 0xFFU;
	}

	return RetData;
}

unsigned long IPOD_PAL_CancelIDPSCount(void)
{
	long CancelIPDSCnt = 30; 
	return CancelIPDSCnt; 
}

void IPOD_PrintLog(unsigned char *Log)
{
	IAPPAL_DBUG("[iPodLIB]%s",Log);
}
void IPOD_AlertLog(unsigned char *Log)
{
//	IAPPAL_ERR("%s",Log);
	IAPPAL_ERR("\x1b[1;31m %s\x1b[1;0m",Log);
}
void IPOD_AlertLog_type1(unsigned char *Log)
{
	IAPPAL_ERR("\x1b[1;31m %s\x1b[1;0m",Log);
}
void IPOD_optionLog(IPOD_LOG_TYPE Type, IPOD_LOG_PART Part, unsigned char *format, ...)
{
	/*
		it's used in Nucleus code.
		to use android, fix it.
	*/
	va_list arg_ptr;
	unsigned char tempStr[256];
	unsigned int str_len;

	va_start(arg_ptr, format);
	str_len = vsprintf(tempStr, format, arg_ptr);

	if(str_len > (255-4)) {
		tempStr[255-4] = 0;
		str_len = 256;
		IPOD_MemCopy(&tempStr[str_len-4], "\r\n", 4);
		
	}
	else

	{
		IPOD_MemCopy(&tempStr[str_len], "\r\n", 4);
	}

	if(Type & IPOD_ALERT)
	{
		IPOD_AlertLog(tempStr);
	}
	else if(Type & IPOD_ALERT1)
	{ 
		IPOD_AlertLog_type1(tempStr);
	}
	else{

		// Filter 
		if(
			(Part & IPOD_LINGO_DAUDIO)
		//	||(Part & IPOD_CMD_TASK_MSG)
		//	||(Part & IPOD_DATAPASING)
		//	||(Part & IPOD_ACCESSORY_INFO)
		//	||(Part & IPOD_LINGO_GEN)
		//	||(Part & IPOD_LINGO_EXT)
			||(Part & IPOD_AUTH)
		//	||(Part & IPOD_LINGO_SIMPLE)
		//	||(Part & IPOD_LINGO_DISPLAYR)
		//	||(Part & IPOD_INTERFACE)
			||(Part & IPOD_CP)
			||(Part & IPOD_TIMEOUT_CNT)
		//	||(Part & IPOD_HID_IO)
		//	||(Part & IPOD_CP_IO)
		//	||(Part & IPOD_EVENT_LOG)
			)
		{
			IPOD_AlertLog_type1(tempStr);
		}
		else if(
			(Part & IPOD_AUTH)
		//	||(Part & IPOD_DATAPASING)
		//	||(Part & IPOD_ACCESSORY_INFO)
			||(Part & IPOD_LINGO_GEN)
			||(Part & IPOD_LINGO_EXT)
			||(Part & IPOD_LINGO_DAUDIO)
			||(Part & IPOD_LINGO_SIMPLE)
			||(Part & IPOD_LINGO_DISPLAYR)
		//	||(Part & IPOD_INTERFACE)
		//	||(Part & IPOD_CP)
			||(Part & IPOD_TIMEOUT_CNT)
		//	||(Part & IPOD_HID_IO)
		//	||(Part & IPOD_CP_IO)
		//	||(Part & IPOD_EVENT_LOG)
		//	||(Part & IPOD_OS_APP_LOG)
		//	||(Part & IPOD_CMD_TASK)
		//	||(Part & IPOD_CMD_TASK_MSG)
		//	||(Part & IPOD_INIT_PROCESS_LOG)
			||(Part & IPOD_AUTH_INFO)
		//	||(Part & IPOD_INTERFACE_OS)
			||(Part & IPOD_OS_RES_LOG)
			)
		{
			IPOD_PrintLog(tempStr);
		}
		else
		{
			// No message 
			;
		}
	}
}
