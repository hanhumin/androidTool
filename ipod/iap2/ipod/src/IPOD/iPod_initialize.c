/****************************************************************************
 *   FileName    : iPod_initialize.c
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
#include "iPod_initialize.h"
#include "iPod_interface.h"
#include <cutils/properties.h>
#include <utils/Log.h>

#include "porting_layer/iPod_OS.h"
#include "porting_layer/iPod_PAL.h"

#ifndef NULL
#define NULL 0
#endif


volatile unsigned long IPOD_RX_W_Ptr=0;// For filling receive buffer
volatile unsigned long IPOD_RX_W_Ptr2=0;// For filling receive buffer

unsigned char *pIPOD_RxBuff;
unsigned char *pIPOD_RxBuff2;

int gDispDataSize = -1;

#define LOG_TAG "IapService"

int IAPINIT_DEBUG	   = -1;
#define IAPINIT_DBUG(msg...)	if (IAPINIT_DEBUG == 1) { ALOGD( " " msg); }

/* UTF-8 Format */
//const IPOD_ACCINFO_TOKEN_VAL gIPOD_AccInfo =  	/* QAC : 9.2 */
IPOD_ACCINFO_TOKEN_VAL gIPOD_AccInfo;
#if 0
=  	/* QAC : 9.2 */
{
	(IPOD_ACCESSORY_NAME |IPOD_ACCESSORY_FIRMWARE_VERSION | IPOD_ACCESSORY_HARDWARE_VERSION
	| IPOD_ACCESSORY_MANUFACTURER | IPOD_ACCESSORY_MODEL_NUM | IPOD_ACCESSORY_RF_CERTIFICATIONS),
	0x06/*0x05*/,
	{'T','E','L','E','C','H','I','P','S',0x00 ,0x00 }, /* Name Info */
	{1, 0, 0 }, NULL,						     /* Firmware Version + padding */
	{1, 3, 0 }, NULL,						    /* Hardware Version + padding */
	{'T','E','L','E','C','H','I','P','S',0x00 ,0x00 }, /* manufacturer Name info */


	{'T','C','C','M','O','D','E','L',0x00 ,0x00 }, /* Model Number */
	{'S','E','R','I','A','L',0x00 ,0x00 }, /* Serial Number */


	/* See Apples's MFi Accessory Testing Specification */
	//(IPOD_RF_CERT_CLASS1|IPOD_RF_CERT_CLASS2|IPOD_RF_CERT_CLASS4) /* RF certifications*/
	0x00,
};
#endif

const IPOD_ACC_PREF_SETTING_FORMAT gIPOD_Pref[IPOD_PREF_MAX_CNT] =
{
	/*{IPOD_PREFERENCE_CLASSID, 			SettingID, 	RestoreOnExit},*/
	{IPOD_VIDEO_OUT_SETTING, 				0x01, 		RESTORE_ORG_SETTING}, /*Enable Apple device video output based on the other video preferences(NTSC/PAL,screen configuration, etc)*/
	{IPOD_SCREEEN_CONFIGURATION, 			0x01, 		RESTORE_ORG_SETTING}, /*0x00: Fill entire screen, 0x01:fit to screen edge*/
	{IPOD_VIDEO_SIGINAL_FORMAT, 			0x00, 		RESTORE_ORG_SETTING}, /*0x00: NTSC*/
	//-{IPOD_LINE_OUT_USAGE, 					0x01, 		RESTORE_ORG_SETTING}, /*0x01: Used*/
	{IPOD_VIDEO_OUT_CONNECTION,				0x01, 		RESTORE_ORG_SETTING}, /*0x00:None, 0x01:Composite, 0x02:S-Video, 0x03:Component*/
	//-{IPOD_CLOSED_CAPTIONING, 				0x01, 		RESTORE_ORG_SETTING}, /*0x00:Off, 0x01:On*/
	{IPOD_VIDEO_MONITOR_ASPECT_RATIO, 		0x00, 		RESTORE_ORG_SETTING}, /*0x00:4:3, 0x01:16:9*/
	//{IPOD_SUBTITLES, 						0x00, 		RESTORE_ORG_SETTING}, /*0x00:Subtitles off, 0x01:Subtitle On*/
	//{IPOD_VIDEO_ALTERNATE_AUDIO_CHANNEL, 	0x00, 		RESTORE_ORG_SETTING}, /*Alternate audio off*/
	//-{IPOD_PAUSE_ON_POWER_REMOVAL, 			0x00, 		RESTORE_ORG_SETTING}, /*0x00:Pause off, 0x01:Pause on*/
	//{IPOD_VOICEOVER_PREFERENCE, 0x00, RESTORE_ORG_SETTING},/*0x00:VoiceOver off, 0x01:VoiceOver on*/
	//{IPOD_ASSISTIVETOUCH, 0x00, RESTORE_ORG_SETTING}/*0x00:Off, 0x01:On*/
};

IPOD_PLAY_INFO gIPOD_PlayInfo[IPOD_PLAY_INFO_MAX_CNT];// = {0};

IPOD_SDK_PROTOCOL_TOKEN gSDKProtocolToken;
IPOD_BUNDLE_SEED_ID_PREF_TOKEN gBundleSeedIDPrefToken;
IPOD_EA_PROTOCOL_METADATA_TOKEN gEaProtocolMetaToken = {0,0,0};

/*Telechips Test App*/
static unsigned char strIPODProtocolString[] = "com.telechips.externalaccessoryapptest001\0";

/**************************************************************************
*	Function : IPOD_ConvertSingleToMultiLingo
*
*	Description : This functin make pParam->IdentifyToken.ML_Lingo member variable.
*
*	Parameter :
*		- pParam : iPod initial parameter
*
*  	Return : none
*
**************************************************************************/
MLingoType IPOD_ConvertSingleToMultiLingo(const IPOD_INIT_PARAM *pParam)
{
	MLingoType lLingo = 0x00;
	unsigned long Add_Lingo;
	long i;

	for (i=0; i<(long)(pParam->IdentifyToken.LingoCnt); i++)
	{
		switch (pParam->IdentifyToken.Lingos[i])
		{
			case(LINGO_GENERAL):
				Add_Lingo = ML_GENERAL;
				break;
			case(LINGO_MICROPHONE):
				Add_Lingo = ML_MICROPHONE;
				break;
			case(LINGO_SIMPLE_REMOTE):
				Add_Lingo = ML_SIMPLE_REMOTE;
				break;
			case(LINGO_DISPLAY_REMOTE):
				Add_Lingo = ML_DISPLAY_REMOTE;
				break;
			case(LINGO_EXTENDED_INTERFACE):
				Add_Lingo = ML_REMOTE_UI_EXTENDED;
				break;
			case(LINGO_RF_TUNER):
				Add_Lingo = ML_RF_TRANSMITTER;
				break;
			case(LINGO_DIGITAL_AUDIO):
				Add_Lingo = ML_DIGITAL_AUDIO;
				break;
			case(LINGO_STORAGE):
				Add_Lingo = ML_STORAGE;
				break;
			default:
				Add_Lingo = 0;
				break;
		}

		lLingo |= Add_Lingo;
	}

//	return (ML_GENERAL | Lingo);
	return (lLingo);
}

void IPOD_API_InitDebug(void)
{
    //if(IAPINIT_DEBUG == -1)
    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.debug.init", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPINIT_DEBUG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPINIT_DEBUG = 0;
                break;
        }
    }    
}

/**************************************************************************
	Function : IPOD_API_USB_InitChannelParam

 	Description : It initialize the iPod channels.

	Parameter :
		pParam : iPod channel param
		Hid_Handle : HID handle
		I2C_Handle : I2C handle
		EventThreadID : Event thread ID
		I2C_ChannelNum : I2C Channel Number

  	Return : IPOD_ERROR

**************************************************************************/
IPOD_ERROR IPOD_API_USB_InitChannelParam(
                                                                        pIPOD_INIT_CHANNER_PARAM pParam, 
                                                                        void *Hid_Handle,
                                                                        void *I2C_Handle, 
                                                                        unsigned long EventThreadID, 
                                                                        unsigned long I2C_ChannelNum, 
                                                                        void *hGXPDevice
                                                                        )
{
	IPOD_ERROR iPod_err = IPOD_NO_ERROR;
	return iPod_err;
}

void IPOD_API_SetAccInfoInit(void)
{
    int accInfoCnt = 0;
    int i;
    int infoBitCnt = sizeof(gIPOD_AccInfo.AccInfo)*8;
    unsigned long accInfo = 0;
    IPOD_API_InitDebug();
    
    IPOD_MemSet((void *)&gIPOD_AccInfo,0x00,sizeof(IPOD_ACCINFO_TOKEN_VAL));
    gIPOD_AccInfo.AccInfo = (IPOD_ACCESSORY_NAME |IPOD_ACCESSORY_FIRMWARE_VERSION 
                    | IPOD_ACCESSORY_HARDWARE_VERSION	| IPOD_ACCESSORY_MANUFACTURER 
                    | IPOD_ACCESSORY_MODEL_NUM | IPOD_ACCESSORY_RF_CERTIFICATIONS);
    accInfo = gIPOD_AccInfo.AccInfo;
    for(i = 0; i < infoBitCnt;i++){
        if(((accInfo >>i) & 1) != 0) accInfoCnt++;           
    }
    IAPINIT_DBUG("%s %d infoBitCnt %d accInfoCnt [%d]",__FUNCTION__,__LINE__,infoBitCnt,accInfoCnt);
    gIPOD_AccInfo.AccInfoCnt = accInfoCnt;
}

void IPOD_API_SetFirmwareVerInAccInfo(
                                                                                        int majorNum,
                                                                                        int minorNum,
                                                                                        int revisionNum
                                                                                        )
{
    IAPINIT_DBUG("%s %d [%d-%d-%d]",__FUNCTION__,__LINE__,majorNum,minorNum,revisionNum);
    gIPOD_AccInfo.Accessory_Firmware_Ver[0] = (majorNum & 0xFF);
    gIPOD_AccInfo.Accessory_Firmware_Ver[1] = (minorNum& 0xFF);
    gIPOD_AccInfo.Accessory_Firmware_Ver[2] = (revisionNum& 0xFF);
}

void IPOD_API_SetHardwareVerInAccInfo(
                                                                                        int majorNum,
                                                                                        int minorNum,
                                                                                        int revisionNum
                                                                                        )
{
    IAPINIT_DBUG("%s %d [%d-%d-%d]",__FUNCTION__,__LINE__,majorNum,minorNum,revisionNum);
    gIPOD_AccInfo.Accessory_Hardware_Ver[0] = (majorNum & 0xFF);
    gIPOD_AccInfo.Accessory_Hardware_Ver[1] = (minorNum& 0xFF);
    gIPOD_AccInfo.Accessory_Hardware_Ver[2] = (revisionNum& 0xFF);
}

void IPOD_API_SetAccName(char const *pData,int size)
{
    if(size < IPOD_MAX_INFO_STR){
        IAPINIT_DBUG("%s %d [%s(%d)]",__FUNCTION__,__LINE__,pData,size);
        IPOD_MemCopy((void *)&gIPOD_AccInfo.Accessory_Name_Info[0],(void *)pData,size);
    }
}

void IPOD_API_SetMenufacturer(char const *pData,int size)
{
    if(size < IPOD_MAX_INFO_STR){
        IAPINIT_DBUG("%s %d [%s(%d)]",__FUNCTION__,__LINE__,pData,size);
        IPOD_MemCopy((void *)&gIPOD_AccInfo.Accessory_manufacturer[0],(void *)pData,size);
    }
}

void IPOD_API_SetModelNumber(char const *pData,int size)
{
    if(size < IPOD_MAX_INFO_STR){
        IAPINIT_DBUG("%s %d [%s(%d)]",__FUNCTION__,__LINE__,pData,size);
        IPOD_MemCopy((void *)&gIPOD_AccInfo.Accessory_modelNum[0],(void *)pData,size);
    }
}

void IPOD_API_EAProtocolTokenInit(void)
{
    IPOD_MemSet((void *)&gSDKProtocolToken,0x00,sizeof(IPOD_SDK_PROTOCOL_TOKEN));
    IPOD_MemSet((void *)&gBundleSeedIDPrefToken,0x00,sizeof(IPOD_BUNDLE_SEED_ID_PREF_TOKEN));
}

void IPOD_API_SetEAProtocolTokenInit(void)
{
    int i,maxCnt = IPOD_SDK_PROTOCOL_MAX_CNT;
    if(gSDKProtocolToken.Total_ProtocolCnt > 0){
        for(i = 0; i < maxCnt;i++){
            if(gSDKProtocolToken.m_protocolTokenData[i].m_ProtocolStringSize > 0){
                if(gSDKProtocolToken.m_protocolTokenData[i].m_pProtocolString)
                    IPOD_MemDealloc((void *)gSDKProtocolToken.m_protocolTokenData[i].m_pProtocolString);
                gSDKProtocolToken.m_protocolTokenData[i].m_ProtocolStringSize = 0;
            }
            gSDKProtocolToken.m_protocolTokenData[i].m_protocolIndex = 0;
        }
        gSDKProtocolToken.Total_ProtocolCnt = 0;
    }
    IPOD_MemSet((void *)&gSDKProtocolToken,0x00,sizeof(IPOD_SDK_PROTOCOL_TOKEN));

    IPOD_MemSet((void *)&gBundleSeedIDPrefToken,0x00,sizeof(IPOD_BUNDLE_SEED_ID_PREF_TOKEN));
}

IPOD_ERROR IPOD_API_AddEAProtocol(char const *pData,int size)
{
    int i;
    int index = -1;
    IPOD_ERROR iPod_err = IPOD_NO_ERROR;
    if(gSDKProtocolToken.Total_ProtocolCnt >= IPOD_SDK_PROTOCOL_MAX_CNT)
    {
        ALOGE("%s %d  error totalCnt %ld [%s]",
                                                            __FUNCTION__,
                                                            __LINE__,
                                                            gSDKProtocolToken.Total_ProtocolCnt,
                                                            pData
                                                            );
        return IPOD_OUT_OF_RESOURCE;
    }
    if(size <= 0){
        ALOGE("%s %d  size error %d [%s]",
                                                            __FUNCTION__,
                                                            __LINE__,
                                                            size,
                                                            pData
                                                            );
        return IPOD_BAD_PARAMETER;
    }
    if(iPod_err == IPOD_NO_ERROR){
        if(gSDKProtocolToken.Total_ProtocolCnt == 0){
            index = 0;
        }else{
            int maxCnt = IPOD_SDK_PROTOCOL_MAX_CNT;
            IAPINIT_DBUG("%s %d Total_ProtocolCnt %ld",
                                                                        __FUNCTION__,
                                                                        __LINE__,
                                                                        gSDKProtocolToken.Total_ProtocolCnt);
            for(i=0;i < maxCnt;i++){
                if(gSDKProtocolToken.m_protocolTokenData[i].m_protocolIndex == 0){
                    IAPINIT_DBUG("%s %d [%d] %s index (%d)",
                                                            __FUNCTION__,
                                                            __LINE__,
                                                            i,
                                                            gSDKProtocolToken.m_protocolTokenData[i].m_pProtocolString,
                                                            gSDKProtocolToken.m_protocolTokenData[i].m_protocolIndex
                                                            );
                    index = i;
                    break;
                }
            }
        }
        IAPINIT_DBUG("%s %d [%s(%d)] index %d",__FUNCTION__,__LINE__,pData,size,index);
        if(index < 0){
            ALOGE("%s %d  index error %d [%s]",
                                                            __FUNCTION__,
                                                            __LINE__,
                                                            index,
                                                            pData
                                                            );
            return IPOD_UNKNOWN_ERROR;
        }
        if(size > 0 && index >= 0){
            unsigned char *pProtocolStr = (unsigned char *)IPOD_MemAlloc(size + 1);
            if(pProtocolStr){
                IPOD_MemSet((void *)pProtocolStr,0x00,size + 1);
                IPOD_MemCopy((void *)pProtocolStr,(void *)pData,size);
                gSDKProtocolToken.m_protocolTokenData[index].m_protocolIndex = index + 1;
                gSDKProtocolToken.m_protocolTokenData[index].m_ProtocolStringSize = size + 1;
                gSDKProtocolToken.m_protocolTokenData[index].m_pProtocolString = pProtocolStr;
                gSDKProtocolToken.Total_ProtocolCnt++;
            }else{
                return IPOD_OUT_OF_RESOURCE;
            }            
        }
    }
    return iPod_err;
}

IPOD_ERROR IPOD_API_SetBundleSeedIDString(char const *pData,int size)
{
    IPOD_ERROR iPod_err = IPOD_NO_ERROR;
    if(size == (IPOD_BUNDLE_SEEDID_STRING_SIZE - 1)){
        IAPINIT_DBUG("%s %d [%s(%d)]",__FUNCTION__,__LINE__,pData,size);
        IPOD_MemCopy((void *)&gBundleSeedIDPrefToken.m_BundleSeedIDString[0],(void *)pData,size);
        gBundleSeedIDPrefToken.m_BundleSeedIDIsExist = 1;        
    }else{
        IAPINIT_DBUG("%s %d size error != 10 [%s(%d)]",__FUNCTION__,__LINE__,pData,size);
        iPod_err = IPOD_BAD_PARAMETER;
    }
    return iPod_err;
}

IPOD_ERROR IPOD_API_SetEAProtocolMeta(int protocolIndex,int fMeta)
{
    IPOD_ERROR iPod_err = IPOD_NO_ERROR;
    gEaProtocolMetaToken.m_EAProtocolMetadataTokenIsExist = 0;
    gEaProtocolMetaToken.metadataType = 0;
    gEaProtocolMetaToken.protocolIndex = 0;
    if(protocolIndex > 0){
        gEaProtocolMetaToken.m_EAProtocolMetadataTokenIsExist = 1;
        gEaProtocolMetaToken.protocolIndex = (unsigned char)protocolIndex;
        if(fMeta != 0)
            gEaProtocolMetaToken.metadataType = 0x01;
    }
    return iPod_err;
}

/**************************************************************************
	Function : IPOD_API_USB_InitParam

 	Description : It initialize the iPod init parameter. ( Extended Lingo )

	Parameter :
		Handle : iPod handle
		pParam : init param for iPod
		connectionType : connection type (UART or USB)

  	Return : MOUNT_FAIL, MOUNT_SUCCESS

**************************************************************************/

IPOD_ERROR IPOD_API_USB_InitParam(
                                                                    void *Handle, 
                                                                    pIPOD_INIT_PARAM pParam, 
                                                                    const IPOD_LINGO_INFO_DATA *pLingo_InfoData, 
                                                                    int fWithStorage
                                                                    )
{
    int lingoCnt = 0;
    IPOD_MemSet(pParam, 0x00, sizeof(IPOD_INIT_PARAM));

    
    pParam->IdentifyToken.Lingos[lingoCnt++] = LINGO_GENERAL;
    pParam->IdentifyToken.Lingos[lingoCnt++] = LINGO_EXTENDED_INTERFACE; 
    pParam->IdentifyToken.Lingos[lingoCnt++] = LINGO_DIGITAL_AUDIO;
    pParam->IdentifyToken.Lingos[lingoCnt++] = LINGO_DISPLAY_REMOTE;
    if(fWithStorage != 0)
        pParam->IdentifyToken.Lingos[lingoCnt++] = LINGO_STORAGE;
    
    pParam->IdentifyToken.LingoCnt = lingoCnt;

    pParam->IdentifyToken.DeviceOption = (LOW_POWER_ONLY | AUTHENTICATION_IMMEDIATELY);
    pParam->IdentifyToken.ML_Lingo = IPOD_ConvertSingleToMultiLingo(pParam);
    pParam->IdentifyToken.EnterExtendedModeOnInit = IPOD_FUNC_ENABLE;
    pParam->pAccInfoToken = &gIPOD_AccInfo;
    
    if(gSDKProtocolToken.Total_ProtocolCnt > 0)
        pParam->AccesoryCapToken = (AC_SUPPORT_IPHONE_OS_APP|AC_USB_DIGITAL_AUDIO);
    else
        pParam->AccesoryCapToken = (AC_USB_DIGITAL_AUDIO);
    
    pParam->AccPrefToken.Total_PreferenceCnt = 0;
    IAPINIT_DBUG("%d: %s() add Total_ProtocolCnt = %ld",
                                                                                __LINE__, 
                                                                                __FUNCTION__,
                                                                                gSDKProtocolToken.Total_ProtocolCnt
                                                                                );
    if(gSDKProtocolToken.Total_ProtocolCnt > 0){
        int i;
        pParam->m_SDKProtocolToken.Total_ProtocolCnt = gSDKProtocolToken.Total_ProtocolCnt;
        for(i = 0;i < (int)gSDKProtocolToken.Total_ProtocolCnt; i++){
            pParam->m_SDKProtocolToken.m_protocolTokenData[i].m_protocolIndex = i + 1;
            pParam->m_SDKProtocolToken.m_protocolTokenData[i].m_ProtocolStringSize = 
            gSDKProtocolToken.m_protocolTokenData[i].m_ProtocolStringSize;
            pParam->m_SDKProtocolToken.m_protocolTokenData[i].m_pProtocolString = 
            gSDKProtocolToken.m_protocolTokenData[i].m_pProtocolString;
            
            IAPINIT_DBUG("[%d][(%d)%s]", 
                                    pParam->m_SDKProtocolToken.m_protocolTokenData[i].m_protocolIndex,
                                    pParam->m_SDKProtocolToken.m_protocolTokenData[i].m_ProtocolStringSize,
                                    pParam->m_SDKProtocolToken.m_protocolTokenData[i].m_pProtocolString
                                    );
            #if 0
            {
                int k;
                int j;
                char buf[256];
                int length = pParam->m_SDKProtocolToken.m_protocolTokenData[i].m_ProtocolStringSize;
                unsigned char *pucBuffer =pParam->m_SDKProtocolToken.m_protocolTokenData[i].m_pProtocolString;

                if (length>0) {
                    for (k=0;k<length;k+=16) {
                        int index = 0;
                        index += sprintf(&buf[index],"%04X: ",k);
                        for (j=0;j<16;j++) {
                            if ((k+j)<length) {
                                index += sprintf(&buf[index]," %02X",pucBuffer[k+j]);
                            } else {
                                index += sprintf(&buf[index],"   ");
                            }
                        }
                        index += sprintf(&buf[index]," : ");
                        for (j=0;j<16;j++) {
                            if ((k+j)<length) {
                                index += sprintf(&buf[index],"%c",(pucBuffer[k+j]>' ')?pucBuffer[k+j]:'.');
                            } else {
                                break;
                                //IAPHID_DBUG("  ");
                            }
                        }
                        IAPINIT_DBUG("%s",buf);
                    }
                }
            }
            #endif
        }
        IAPINIT_DBUG("%d: %s() add m_BundleSeedIDIsExist = %d", __LINE__, __FUNCTION__,gBundleSeedIDPrefToken.m_BundleSeedIDIsExist);
        if(gBundleSeedIDPrefToken.m_BundleSeedIDIsExist != 0){
            pParam->m_BundleSeedIDPrefToken.m_BundleSeedIDIsExist = 1;
            IPOD_MemCopy(
                                    &(pParam->m_BundleSeedIDPrefToken.m_BundleSeedIDString[0]),
                                    &(gBundleSeedIDPrefToken.m_BundleSeedIDString[0]),
                                    IPOD_BUNDLE_SEEDID_STRING_SIZE
                                    );
            IAPINIT_DBUG(" bundleSeedID [%s]", 
                                    pParam->m_BundleSeedIDPrefToken.m_BundleSeedIDString
                                    );
        }

        pParam->EAProtocolMetaToken.m_EAProtocolMetadataTokenIsExist = 0;
        if(gEaProtocolMetaToken.m_EAProtocolMetadataTokenIsExist != 0){
            pParam->EAProtocolMetaToken.m_EAProtocolMetadataTokenIsExist = 1;
            pParam->EAProtocolMetaToken.protocolIndex = gEaProtocolMetaToken.protocolIndex;
            pParam->EAProtocolMetaToken.metadataType = gEaProtocolMetaToken.metadataType;
            IAPINIT_DBUG(" Ea Meta Index [%d] 0x%x", 
                                    pParam->EAProtocolMetaToken.protocolIndex,
                                    pParam->EAProtocolMetaToken.metadataType
                                    );
        }
    }
#if 0
    // For SDKProtocolToken
    pParam->m_SDKProtocolToken.Total_ProtocolCnt = 1;
    pParam->m_SDKProtocolToken.m_protocolTokenData[0].m_protocolIndex = 1;
    /*Telechips Test App*/
    pParam->m_SDKProtocolToken.m_protocolTokenData[0].m_ProtocolStringSize= 42;
    pParam->m_SDKProtocolToken.m_protocolTokenData[0].m_pProtocolString = &strIPODProtocolString[0];
    // For BundleSeedIDPreToken
    pParam->m_BundleSeedIDPrefToken.m_BundleSeedIDIsExist = 1;
    IPOD_MemSet( &(pParam->m_BundleSeedIDPrefToken.m_BundleSeedIDString[0]), 0x00, IPOD_BUNDLE_SEEDID_STRING_SIZE );
    IPOD_MemCopy( &(pParam->m_BundleSeedIDPrefToken.m_BundleSeedIDString[0]), "AZ3V4GJQCV", sizeof("AZ3V4GJQCV") );
#endif
    return 0;
}

IPOD_ERROR IPOD_API_InitChannelParam(pIPOD_INIT_CHANNER_PARAM pParam, void *pHandle, IPOD_CONNECTION_TYPE type, unsigned long driverID, unsigned long Block)
{
	IPOD_ERROR iPod_err = IPOD_NO_ERROR;
	pIPOD_PLAY_INFO pInfo;
	pInfo = (pIPOD_PLAY_INFO)IPOD_GetExtInfo(pHandle);
	pParam->ExtInfo = (unsigned long)pInfo;

	if (pParam->ExtInfo)
	{
		IPOD_InitBuff(&pParam->buff, pInfo->pBuffer, type, 0/*USB Block*/);
		IPOD_API_InitArtworkInfo(pInfo);
		pParam->DevHandleDes.DevNo_Handle = 0;
	}
	else
	{
		iPod_err = IPOD_MALLOC_ERROR;
	}

	return iPod_err;
}

/**************************************************************************
*	Function : IPOD_InitBuff
*
*	Description : This function initialize the iPod buffer.
*				usage : iPod initialization / authentication / UTF8 decode buffer / communication buffer
*						USB audio PCM buffer
*
*	Parameter :
*		- buffer
*
*  	Return : none
*
**************************************************************************/
void IPOD_InitBuff(pIPOD_BUF_PARAM buf_param, const unsigned char *buffer, IPOD_CONNECTION_TYPE type, unsigned int Block)	/* QAC : 16.7 */
{
	unsigned long lAddress;
	lAddress = (unsigned long)buffer;
	buf_param->CP_buffer = (unsigned char *)lAddress; // buffer for authentication IC. size : 50Kbytes
	buf_param->CP_buffer_size = IPOD_CPBUFSIZE;
	lAddress += IPOD_CPBUFSIZE;
//	pIPOD_TxBuff = (unsigned char *)lAddress; // size :IPOD_TXBUFSIZE
	buf_param->Tx_buffer = (unsigned char *)lAddress;
	buf_param->Tx_buffer_size = 	IPOD_TXBUFSIZE;
	lAddress += IPOD_TXBUFSIZE;

	if (Block == 0)
	{
		pIPOD_RxBuff = (unsigned char *)lAddress; // size : IPOD_RXBUFSIZE
		buf_param->Rx_buffer = (unsigned char *)lAddress;
		buf_param->Rx_buffer_size = 	IPOD_RXBUFSIZE;
		lAddress += IPOD_RXBUFSIZE;
	}
	else
	{
		pIPOD_RxBuff2 = (unsigned char *)lAddress; // size : IPOD_RXBUFSIZE
		buf_param->Rx_buffer = (unsigned char *)lAddress;
		buf_param->Rx_buffer_size = 	IPOD_RXBUFSIZE;
		lAddress += IPOD_RXBUFSIZE;
	}

//	IPOD_RX_W_Ptr = 0;
//	IPOD_RX_R_Ptr = 0;
//	pIPOD_RXData  = (unsigned char *)lAddress; // size :IPOD_RXBUFSIZE
	buf_param->RxData_buffer = (unsigned char *)lAddress;
	buf_param->RxData_buffer_size = 	IPOD_RXBUFSIZE;
	lAddress += IPOD_RXBUFSIZE;
	buf_param->UTF8DecodeBuffer = (unsigned char *)lAddress;
	buf_param->UTF8DecodeBuffer_size = 	IPOD_RXBUFSIZE;
	lAddress += IPOD_RXBUFSIZE;
	// Total size : IPOD_TXBUFSIZE + ( IPOD_RXBUFSIZE * 3 ) = about 194Kbytes
}

/**************************************************************************
*	Function : IPOD_Alloc_communication_mem
*
*	Description : This function allocate the iPod communication buffer
*
*	Parameter : buffer size
*
*  	Return : void pointer of communication buffer
*
**************************************************************************/
static void *IPOD_Alloc_communication_mem(unsigned long size)
{
	unsigned char *Buff=NULL;
	Buff = IPOD_MemAlloc(size);
	return Buff;
}

void IPOD_Malloc_iPodBufInit(void)
{
    int  i = 0;
    gIPOD_PlayInfo[i].pBuffer = NULL;
}

/**************************************************************************
*	Function : IPOD_Malloc_iPodBuf
*
*	Description : This function allocate the iPod play info
*
*	Parameter : none
*
*  	Return : void pointer of play info
*
**************************************************************************/
void *IPOD_Malloc_iPodBuf(void)
{
    long i;
    void *Ret = NULL;
    unsigned char *Buff = NULL;
    #if 1
    i = 0;
    gIPOD_PlayInfo[i].m_Status = 1;

    if (gIPOD_PlayInfo[i].pBuffer == 0)
    {
#if 1
        if(ipod_stack_cbacks->iPodAlloc_communication_mem_cb){
            Buff = ipod_stack_cbacks->iPodAlloc_communication_mem_cb(IPOD_TOTALBUFSIZE);
        }
#else
        Buff = IPOD_Alloc_communication_mem(IPOD_TOTALBUFSIZE);
#endif
    }
    else
    {
        Buff = gIPOD_PlayInfo[i].pBuffer;
    }

    if (Buff == 0)
    {
        gIPOD_PlayInfo[i].m_Status = 0;
    }
    else
    {
        IPOD_MemSet(Buff,0x5A, IPOD_TOTALBUFSIZE);
        gIPOD_PlayInfo[i].pBuffer = Buff;
        Ret = (void *)&gIPOD_PlayInfo[i];
    }

    gIPOD_PlayInfo[i].DisconnecEvt = 0;
    #else

	for (i=0; i<IPOD_PLAY_INFO_MAX_CNT; i++)
	{
		if (gIPOD_PlayInfo[i].m_Status == 0)
		{
			gIPOD_PlayInfo[i].m_Status = 1;

			if (gIPOD_PlayInfo[i].pBuffer == 0)
			{
				#if 1
				if(ipod_stack_cbacks->iPodAlloc_communication_mem_cb){
					Buff = ipod_stack_cbacks->iPodAlloc_communication_mem_cb(IPOD_TOTALBUFSIZE);
				}
				#else
				Buff = IPOD_Alloc_communication_mem(IPOD_TOTALBUFSIZE);
				#endif

				if (Buff == 0)
				{
					/* Alloc ERROR */
					gIPOD_PlayInfo[i].m_Status = 0;
					break;
				}
				else
				{
					IPOD_MemSet(Buff,0x5A, IPOD_TOTALBUFSIZE);
					gIPOD_PlayInfo[i].pBuffer = Buff;
				}
			}
			else
			{
				Buff = gIPOD_PlayInfo[i].pBuffer;
			}

			gIPOD_PlayInfo[i].DisconnecEvt = 0;
			Ret = (void *)&gIPOD_PlayInfo[i];
			break;
		}
	}
    #endif
	return Ret;
}
/**************************************************************************
*	Function : IPOD_Free_iPodBuf
*
*	Description : This function deallocate the play information of iPod
*
*	Parameter : iPod_Handle
*
*  	Return : none.
*
**************************************************************************/
void IPOD_Free_iPodBuf(void *iPod_Handle)
{
	pIPOD_PLAY_INFO pInfo;
	pInfo = (pIPOD_PLAY_INFO)IPOD_GetExtInfo(iPod_Handle);

	if (pInfo != 0)
	{
		if (pInfo->pBuffer != 0)
		{
			#if 1
			IPOD_CBACK(ipod_stack_cbacks,
							iPodAlloc_communication_free_cb,
							pInfo->pBuffer);
			#else		
			IPOD_MemDealloc(pInfo->pBuffer);
			#endif
		}

		if (pInfo->m_Status != 0)
		{
			IPOD_MemSet((void *)pInfo, 0x00, sizeof(IPOD_PLAY_INFO));
		}
	}
}
		
/**************************************************************************
*	Function : IPOD_API_GetDisplayDataSize
*
*	Description : This functin get display data size
*
*	Parameter : none.
*
*  	Return : Display data size.
*
**************************************************************************/
static unsigned long IPOD_API_GetDisplayDataSize(void)
{
    unsigned long lDisplayDataSize = 0;

    //lDisplayDataSize = 240*240*2;//(64*1024);
    //lDisplayDataSize = 120*120*2;//(64*1024);
    if(gDispDataSize >= (80*80)  && gDispDataSize < (240*240))
        lDisplayDataSize = gDispDataSize* 2;
    else
        lDisplayDataSize = 240*240*2;//(64*1024);

    return lDisplayDataSize;
}

void IPOD_API_SetDisplayDataSize(int dataSize)
{
    if(dataSize >= (80*80)  && dataSize < (240*240))
        gDispDataSize = dataSize;
}


/**************************************************************************
*	Function : IPOD_API_InitArtworkInfo
*
*	Description : This functin set display limit
*
*	Parameter :
*		- pPlayInfo
*
*  	Return : none.
*
**************************************************************************/
void IPOD_API_InitArtworkInfo(pIPOD_PLAY_INFO pPlayInfo)
{
	pIPOD_ARTWORK_INFO pArtworkInfo = &(pPlayInfo->ArtworkInfo);
	unsigned long DisplayDataSize;
	DisplayDataSize = IPOD_API_GetDisplayDataSize();

	if (DisplayDataSize > (IPOD_RXBUFSIZE -20))
	{
		DisplayDataSize = (IPOD_RXBUFSIZE -20);
	}

	pArtworkInfo->MaxDisplaySize = DisplayDataSize;
	pArtworkInfo->BasicFormatID = -1;
	IPOD_MemSet(pArtworkInfo->CapableFormatID, -1, sizeof(unsigned long)*IPOD_MAX_ARTWORK);
	pArtworkInfo->MaxFormatCount= 0;
}


