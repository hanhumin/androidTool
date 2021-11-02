/****************************************************************************
 *   FileName    : iPod_notify.c
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
#include "iPod_error.h"
#include "iPod_lib.h"
#include "iPod_notify.h"
#include "iPod_interface.h"

#include "porting_layer/iPod_OS.h"
#include "porting_layer/iPod_PAL.h"

#ifndef NULL
#define NULL 0
#endif


#define LOG_TAG "IapService"


/**************************************************************************
*	Function : IPOD_Callback_EndAuthentication
*
*	Description : This function notify end of iPod authentication.
*
*	IMPORTANT NOTICE : Please don't use IPOD library API on callback function.
*
*	Parameter : 	iPod_Handle
			IsSuccess(1) - iPod authentication success
			IsSuccess(0) - iPod authentication fail
*
*  	Return : none
*
**************************************************************************/
void IPOD_Callback_EndAuthentication(void* iPod_Handle, unsigned long IsSuccess)
{
    if (IsSuccess == 1)/*iPod authentication success*/
    {
    }
    else/*iPod authentication fail*/
    {
    }
}

/**************************************************************************
*	Function : IPOD_Callback_iPodNotification
*
*	Description : This function updated all informatin of new contents.
*				callback function of iPod library.
*
*	IMPORTANT NOTICE : Please don't use IPOD library API on callback function.
*
*	Parameter :
*		- ExtInfo :
*
*  	Return : none
*
**************************************************************************/
long IPOD_Callback_iPodNotification(void* iPod_Handle, unsigned char NotiType, const unsigned char *pData)
{
    unsigned long WaitTime;
    unsigned short OverflowTransID;
    IPOD_NOTI_TAG_STATUS TagStatus;
    IPOD_NOTI_CAMERA_PAYLOAD CameraPayload;
    long ActualLen;
    unsigned char *AppString = NULL;
    unsigned short SessionID;
    IPOD_NOTI_DATABASE_AVAILABLE_STATUS DatabaseAvailableStatus;
    IPOD_NOTI_COMMAND_COMPLETION_STATUS CommandCompletionStatus;
    IPOD_NOTI_CHARGING_INFO_TYPE ChargingInfoType;
    IPOD_NOTI_CHARGING_INFO_VALUE ChargingInfoValue;
    void *ExtInfo = IPOD_GetExtInfo(iPod_Handle);
    pIPOD_PLAY_INFO pInfo = (pIPOD_PLAY_INFO)ExtInfo;
    switch (NotiType)
    {
        case IPOD_NOTI_TYPE_FLOW_CONTROL :
            pInfo->m_Noti_WaitTime = IPOD_Get_Oneword(&pData[0]);
            pInfo->m_Noti_OverflowTransID = IPOD_Get_Short(&pData[4]);
            /*TCC800X*/
            //DMP_MessageEvtUpdateiPodNotiFlowControl(pInfo->Disk);
            break;
        case IPOD_NOTI_TYPE_RADIO_TAGGING :
            TagStatus = pData[0];
            break;
        case IPOD_NOTI_TYPE_CAMERA_NOTIFICATIONS :
            CameraPayload = pData[0];
            break;
        case IPOD_NOTI_TYPE_CHARGING_INFO :
            ChargingInfoType = pData[0];
            ChargingInfoValue = IPOD_Get_Short(&pData[1]);
            break;
        case IPOD_NOTI_TYPE_NOWPLAYINGAPPBUNDLENAME:
            
            //ActualLen =  IPOD_UTF8Decoder(iPod_Handle, pData);
            ActualLen =  strlen((const char *)pData) + 1;
            //iPOD_DumpMemory(pData,ActualLen);
            if(ActualLen > 0){
                AppString = (unsigned char*)malloc(sizeof(unsigned char) * ActualLen);
                memcpy(AppString, &pData[0], ActualLen);
                //AppString[(int)ActualLen] = 0;

                IPOD_CBACK(ipod_stack_cbacks,
                                        iPODCbNotifyString_cb,
                                        IPOD_NOTI_TYPE_NOWPLAYINGAPPBUNDLENAME,
                                        AppString,
                                        ActualLen);
				free(AppString);
            }

            break;
        case IPOD_NOTI_TYPE_SESSIONSPACEAVAILABLE :
            SessionID = IPOD_Get_Short(&pData[0]);
            break;
        case IPOD_NOTI_TYPE_DATABASE_CHAGNED :
            DatabaseAvailableStatus = pData[0];
            break;
        case IPOD_NOTI_TYPE_COMMAND_COMPLETE :
            CommandCompletionStatus = pData[0];
            break;
        case IPOD_NOTI_TYPE_NOWPLAYINGAPPDISPLAYNAME:
            //ActualLen =  IPOD_UTF8Decoder(iPod_Handle, pData);
            ActualLen =  strlen((const char *)pData) + 1;
            if(ActualLen > 0){
                AppString = (unsigned char*)malloc(sizeof(unsigned char) * ActualLen);
                memcpy(AppString, &pData[0], ActualLen);
                //AppString[(int)ActualLen] = 0;
                IPOD_CBACK(ipod_stack_cbacks,
                                        iPODCbNotifyString_cb,
                                        IPOD_NOTI_TYPE_NOWPLAYINGAPPDISPLAYNAME,
                                        AppString,
                                        ActualLen);
            }
            break;
        default :
            break;
    }
    return 0;
}


/**************************************************************************
*	Function : IPOD_Callback_GetReturnPlayInfo
*
*	Description : This function updated all informatin of new contents.
*				callback function of iPod library.
*
*	IMPORTANT NOTICE : Please don't use IPOD library API on callback function.
*
*	Parameter :
*		- ExtInfo :
*
*  	Return : none
*
**************************************************************************/
long IPOD_Callback_GetReturnPlayInfo(void *iPod_Handle, char *pCmdData)
{
	return 0;
}

/**************************************************************************
*	Function : IPOD_Callback_DisplayRemoteEvent
*
*	Description : This function updated all informatin of new contents.
*				callback function of iPod library.
*
*	IMPORTANT NOTICE : Please don't use IPOD library API on callback function.
*
*	Parameter :
*		- ExtInfo :
*
*  	Return : none
*
**************************************************************************/
long IPOD_Callback_DisplayRemoteEvent(unsigned char EventNum, const unsigned char *pData, void *iPod_Handle)
{	
	return 0;
}


/**************************************************************************
*	Function : IPOD_Callback_OpenDataSessionForProtocol
*
*	Description : iPhone OS application communicatin callback
*
*	Parameter : Mode
*
*  	Return : none
*
**************************************************************************/
IPOD_ERROR IPOD_Callback_OpenDataSessionForProtocol( void *iPod_Handle, unsigned long SessionID, unsigned long nProtocolIndex )
{
    /* Sample code */
    long ret = IPOD_NO_ERROR;
    void *ExtInfo = IPOD_GetExtInfo(iPod_Handle);
    pIPOD_PLAY_INFO pInfo = (pIPOD_PLAY_INFO)ExtInfo;

    if ( NULL == iPod_Handle )
    {
        ret = IPOD_BAD_PARAMETER;
    }
    else
    {
        IPOD_CBACK(ipod_stack_cbacks,openDataSessionForProtocol_cb,SessionID,nProtocolIndex);
    } 

    return ret;
}

/**************************************************************************
*	Function : IPOD_Callback_CloseDataSession
*
*	Description : iPhone OS application communicatin callback
*
*	Parameter : Mode
*
*  	Return : none
*
**************************************************************************/
IPOD_ERROR IPOD_Callback_CloseDataSession( void *iPod_Handle, unsigned long SessiongID )
{
    /* Sample code */
    long ret = IPOD_NO_ERROR;
    void *ExtInfo = IPOD_GetExtInfo(iPod_Handle);
    pIPOD_PLAY_INFO pInfo = (pIPOD_PLAY_INFO)ExtInfo;
    IPOD_ERROR checkRet = IPOD_NO_ERROR;
    IPOD_CBACK(ipod_stack_cbacks,isValidSessionId_cb,SessiongID, &checkRet);

    if ( NULL == iPod_Handle )
    {
        ret = IPOD_BAD_PARAMETER;
    }
    else if( checkRet != IPOD_NO_ERROR)
    {
        ret = IPOD_BAD_PARAMETER;
    }
    else
    {
        // Implement here
        //pInfo->m_iPod_OSApp_DataSesiongID = IPOD_NOT_ESTABLISED_SESSIONGID;
        //pInfo->m_iPod_OSApp_ProtocolIndex = 0;
        IPOD_CBACK(ipod_stack_cbacks,closeDataSession_cb,SessiongID);
    }

    return ret;
}

/**************************************************************************
*	Function : IPOD_Callback_iPodDataTransfer
*
*	Description : iPhone OS application communicatin callback
*
*	Parameter : Mode
*
*  	Return : none
*
**************************************************************************/
IPOD_ERROR IPOD_Callback_iPodDataTransfer( void *iPod_Handle, unsigned long usSessionID, unsigned char *pData, unsigned long nDataSize )
{
	/* Sample code */
	long ret = IPOD_NO_ERROR;
	void *ExtInfo = IPOD_GetExtInfo(iPod_Handle);
	pIPOD_PLAY_INFO pInfo = (pIPOD_PLAY_INFO)ExtInfo;
	IPOD_ERROR checkRet = IPOD_NO_ERROR;
	IPOD_CBACK(ipod_stack_cbacks,isValidSessionId_cb,usSessionID, &checkRet);
    
	if ( (NULL == iPod_Handle) || (NULL == pData) )
	{
		ret = IPOD_BAD_PARAMETER;
	}
	else if(checkRet != IPOD_NO_ERROR)
	{
		ret = IPOD_BAD_PARAMETER;
	}
	else
	{
		// Implement here
//		IPOD_optionLog(0x10, 0x00400000,"Session ID : 0x%x", usSessionID);
//		IPOD_optionLog(0x10, 0x00400000," rcv data : -->%s<-- ", pData);
		//BT Test
		IPOD_CBACK(ipod_stack_cbacks,iPodDataTransfer_cb,usSessionID,&pData[0],nDataSize);
#ifdef IPOD_BT_INCLUDE
		SendTestBufLen = nDataSize;
		IPOD_MemCopy(SendTestBuf, pData, SendTestBufLen);
		UI_BT_IPOD_SEND_TEST();
#endif
	}

	return ret;
}

void IPOD_Callback_RequestIdentify(void* iPod_Handle)
{
	/*
		need to reset process.
	*/
    IPOD_CBACK(ipod_stack_cbacks,iPodCbReqIdentify_cb);
    ALOGE(" %s Apple device requestIdentify",__FUNCTION__);
}

void IPOD_Callback_SetDiskHandle(void* iPod_Handle)
{
	; //Do nothing except nucleus OS.
}

