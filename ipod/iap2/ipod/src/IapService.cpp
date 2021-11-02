/****************************************************************************
 *   FileName    : IapService.cpp
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
#define LOG_TAG "IapService"

#include <binder/IServiceManager.h>
#include <utils/Log.h>
#include <binder/IPCThreadState.h>
#include <utils/threads.h>
#include <cutils/properties.h>

#include "IapService.h"
#include "iPodConfig.h"
#include "iPod_lib.h"
#include "iPod_sample.h"
#include "iPod_initialize.h"
#include "iPod_interface.h"
#include "iPod_OS.h"
#include "iPod_CP.h"
#include "iPod_PAL.h"
#include "iPod_HID.h"
#ifdef USE_TIMER
#include "iPod_Timer.h"
#endif

#include "iap_audio_hw.h"

#include <utils/misc.h>
#include <utils/String8.h>
#include <utils/String16.h>
#include <gui/BitTube.h>
#include <pthread.h>

int IAPSVC_DEBUG	   = -1;
#define IAPSVC_DBUG(msg...)	if (IAPSVC_DEBUG == 1) { ALOGD( " " msg); }

int IAPSVC_DDEBUG	   = -1;
#define IAPSVC_DDBUG(msg...)	if (IAPSVC_DDEBUG == 1) { ALOGD( " " msg); }

int IAPSVC_DEFLOG	   = 1;
#define IAPSVC_DEFLOG(msg...)	if (IAPSVC_DEFLOG == 1) { ALOGD( " " msg); }

#define TRUE 1
#define FLASE 0

int gNextPlayPause = 1;

int NEXT_PREVIOUS_CMD_FLAG = 0;

namespace android {

#define IS_CMDOK(x)	((x) == (IPOD_NO_ERROR) )
#define IS_CMDERROR(x)	( (x) != (IPOD_NO_ERROR) )

#define IPOD_INIT_RETRY_CNT 10
#define IPOD_INIT_TIMEOUT (60000000000) // 60 s

int noise_avoid_cnt=0;
IapService *nIapSvc = NULL;
unsigned char *gStackLargeBuff=NULL;

static const ipod_interface_t *sIpodStackInterface = NULL;
char HID_DEV[] = "/dev/hidraw0";

char *IPOD_GetSvcVersion(void)
{
    return (char *)"160711.1";
}

void IPOD_SetIapSvc(IapService *iapSvc)
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    nIapSvc = iapSvc;
}

PlayCmdCode IPOD_GetNextTrack_Cmd()
{
    PlayCmdCode cmd = NEXTTRACK;
    if(NEXT_PREVIOUS_CMD_FLAG)
        cmd = NEXT;
    return cmd;
}

PlayCmdCode IPOD_GetPreviousTrack_Cmd()
{
    PlayCmdCode cmd = PREVIOUSTRACK;
    if(NEXT_PREVIOUS_CMD_FLAG)
        cmd = PREVIOUS;
    return cmd;
}

void IPOD_Mem_StackLargeBuff(void)
{
    int size = IPOD_PAL_GetTotalBufSize();
    IAPSVC_DBUG("%d: %s() size %d", __LINE__, __FUNCTION__,size);
    gStackLargeBuff = (unsigned char *)IPOD_MemAlloc(size);
}

void IPOD_Free_StackLargeBuff(void)
{
    IAPSVC_DBUG("%d: %s()", __LINE__, __FUNCTION__);
    IPOD_MemDealloc(gStackLargeBuff);
    gStackLargeBuff = NULL;
}

IPOD_ERROR IPOD_Cb_OpenDataSessionForProtocol(
                                                                                    unsigned long SessionID, 
                                                                                    unsigned long nProtocolIndex 
                                                                                    )
{
    IPOD_ERROR ret = IPOD_UNKNOWN_ERROR;
    IAPSVC_DEFLOG("%d: %s() SessionID %ld protocolIndex %ld",
                                                                                        __LINE__, 
                                                                                        __FUNCTION__,
                                                                                        SessionID,
                                                                                        nProtocolIndex
                                                                                        );
    if(nIapSvc != NULL){
        ret = (IPOD_ERROR)nIapSvc->notifyEapOpenData(SessionID, nProtocolIndex);
    }
    return ret;
}

IPOD_ERROR IPOD_Cb_CloseDataSession(unsigned long SessionID )
{
    IAPSVC_DEFLOG("%d: %s() SessionID %ld", __LINE__, __FUNCTION__,SessionID);
    if(nIapSvc != NULL){
        nIapSvc->CloseDataSession(SessionID);
    }
    return IPOD_NO_ERROR;
}

IPOD_ERROR IPOD_Cb_iPodDataTransfer(
                                                                        unsigned long usSessionID, 
                                                                        unsigned char *pData, 
                                                                        unsigned long nDataSize 
                                                                        )
{
    Parcel noti;
    IAPSVC_DEFLOG("%d: %s() sess %ld dataSize %ld", 
                                                                            __LINE__, 
                                                                            __FUNCTION__,
                                                                            usSessionID,
                                                                            nDataSize
                                                                            );
    if(nIapSvc != NULL){
        noti.writeInt32((int)nDataSize);
        noti.writeInt32((int)nDataSize);
        noti.write(pData,nDataSize);        
        nIapSvc->notifyObjEap(
                                            IPOD_APP_EAP_IPODDATATRANSFER, 
                                            usSessionID, 
                                            0, 
                                            &noti,
                                            usSessionID
                                            );
    }
    return IPOD_NO_ERROR;
}

IPOD_ERROR IPOD_Cb_CheckEapSessionId(unsigned long sessionID,IPOD_ERROR *result)
{
    IPOD_ERROR ret = IPOD_NO_ERROR;
    if(nIapSvc != NULL){
        if(nIapSvc->mEapSessionList.size() > 0){
            if(nIapSvc->IsValidEapSessionId(sessionID) == false)
                ret =  IPOD_BAD_PARAMETER;
        }
        IAPSVC_DBUG("%d: %s() session %ld ret 0x%x",__LINE__,__FUNCTION__,sessionID,ret);
    }
    *result = ret;
    return ret;
}

IPOD_ERROR IPOD_Cb_NotifyString(unsigned int notiType,unsigned char *pStr,int strLen )
{
    Parcel data;
    String8 str8 = String8("");
    if(strLen > 0)
        str8 = String8((char const*)pStr);
    //IAPSVC_DBUG("%d: %s() [len %d](%s) ", __LINE__, __FUNCTION__,strLen,str8.string());
    data.writeString16(String16(str8));

    if(nIapSvc != NULL)
        nIapSvc->notifyObj(IPOD_APP_IPODNOTIFICATION,notiType, 0, &data);
    //if(pStr)
    //    free(pStr);
    return IPOD_NO_ERROR;
}

IPOD_ERROR IPOD_Cb_Test1(unsigned long value )
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    //if(nIapSvc != NULL)
    //    nIapSvc->notify(IPOD_APP_EAP_TEST1, value, 0, 0);    
    return IPOD_NO_ERROR;
}

IPOD_ERROR IPOD_Cb_Test2(unsigned long value,unsigned char *pData )
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    //if(nIapSvc != NULL)
    //    nIapSvc->notify(IPOD_APP_EAP_TEST2, value, 0, 0);    
    return IPOD_NO_ERROR;
}


void IPOD_Cb_ReqIndentify(void)
{
    ALOGE("\x1b[1;31m %s() \x1b[1;0m ", __FUNCTION__);
    if(nIapSvc != NULL){
        nIapSvc->notifyByName(IPOD_APP_REQUEST_IDENTIFY,0, 0, 0,String8("ipod-connect"));
        ALOGI("%d: %s() set disconnect because Request identify", __LINE__, __FUNCTION__);
        nIapSvc->SetSvcState(IAP_SVC_STATE_DISCONNECT);
        nIapSvc->setDisconnected(false);
    }
}

void iPOD_Cb_SetFullBandwidth(void)
{
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    if(nIapSvc != NULL)
        nIapSvc->playAudio();
}

void *iPOD_Cb_Alloc_mem(unsigned long size)
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(gStackLargeBuff == NULL){
        IPOD_Mem_StackLargeBuff();
    }
    return (void *)gStackLargeBuff;
}

void iPOD_Cb_Free(void *pBuff)
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
}

void IPOD_Cb_GetDetectStatus(void *pStatus)
{
    bool ret = false;
    IPOD_ERROR Err = IPOD_CHECKDEVICE_ERROR;
    IPOD_ERROR *pErr = (IPOD_ERROR*)pStatus;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(nIapSvc != NULL){
        ret = nIapSvc->GetUsbConnected();
        if(ret == true){
            Err = IPOD_NO_ERROR;
        }
    }
    *pErr = Err;
}

int IPOD_Cb_CheckTimeForGetRepeat(void)
{
    int ret = -1;
    if(nIapSvc != NULL){
        bool retBool = nIapSvc->IsValidPlayInfoTimeSchedule(nIapSvc->IPOD_TIME_SCHEDULE_REPEAT);
        if(retBool == true)
            ret = 0;
    }
    return ret;
}

void IPOD_Cb_SetLastAccessTimeForGetRepeat(void)
{
    if(nIapSvc != NULL){
        nIapSvc->UpdatePlayInfoTimeSchedule(nIapSvc->IPOD_TIME_SCHEDULE_REPEAT);
    }
}

int IPOD_Cb_CheckTimeForGetShuffle(void)
{
    int ret = -1;
    if(nIapSvc != NULL){
        bool retBool = nIapSvc->IsValidPlayInfoTimeSchedule(nIapSvc->IPOD_TIME_SCHEDULE_SHUFFLE);
        if(retBool == true)
            ret = 0;
    }
    return ret;
}

void IPOD_Cb_SetLastAccessTimeForGetShuffle(void)
{
    if(nIapSvc != NULL){
        int64_t localNewTime = (int64_t)systemTime();
        nIapSvc->UpdatePlayInfoTimeSchedule(nIapSvc->IPOD_TIME_SCHEDULE_SHUFFLE);
    }
}

ipod_callbacks_t sIpodCallbacks = {
    sizeof(sIpodCallbacks),
    IPOD_Cb_OpenDataSessionForProtocol,
    IPOD_Cb_CloseDataSession,
    IPOD_Cb_iPodDataTransfer,
    IPOD_Cb_CheckEapSessionId,
    IPOD_Cb_NotifyString,
    IPOD_Cb_Test1,
    IPOD_Cb_Test2,
    IPOD_Cb_ReqIndentify,
    iPOD_Cb_SetFullBandwidth,
    iPOD_Cb_Alloc_mem,
    iPOD_Cb_Free,
    IPOD_Cb_GetDetectStatus,
    IPOD_Cb_CheckTimeForGetRepeat,
    IPOD_Cb_SetLastAccessTimeForGetRepeat,
    IPOD_Cb_CheckTimeForGetShuffle,
    IPOD_Cb_SetLastAccessTimeForGetShuffle
};

void IPOD_InitIpodInterface()
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(sIpodStackInterface != NULL)
        sIpodStackInterface->init(&sIpodCallbacks);
}

void IPOD_Timer_Cb_Cancel(void *pData,unsigned int event,unsigned int type ,unsigned int sendNoti,unsigned int cmdFrom)
{
    #ifdef USE_TIMER
    IAPSVC_DBUG("%s[%d]  timeout event %d type %s",
                                                        __FUNCTION__,
                                                        __LINE__,
                                                        event,
                                                        iPod_Timer_GetNameString(type));
    #endif
    return;
}

void IPOD_Timer_Cb_Expired(void *pData,unsigned int event,unsigned int type ,unsigned int sendNoti,unsigned int cmdFrom)
{
    #ifdef USE_TIMER
    ALOGI("%s[%d]  timeout event %d type %s",
                                                        __FUNCTION__,
                                                        __LINE__,
                                                        event,
                                                        iPod_Timer_GetNameString(type));
    switch(type){
        case IPOD_TM_TYPE_PLAYING:
            if(nIapSvc != NULL){
                nIapSvc->SetSvcLocalPlayStatus(IPOD_DR_UNKNOWN);
                if(pData != NULL)
                {
                    iPodTimerNode_t *pTimerInfo = (iPodTimerNode_t *)pData;
                    if(pTimerInfo->sendNoti){
                        ALOGI("%s[%d]  sendNoti %s",
                                                        __FUNCTION__,
                                                        __LINE__,
                                                        iPod_Timer_GetNameString(type));                        
                        nIapSvc->notify(IPOD_SVC_REQ_TIMEOUT_PLAY, 0, 0, 0);
                    }
                }
            }

            break;
        case IPOD_TM_TYPE_PAUSE:
            if(nIapSvc != NULL)
                nIapSvc->SetSvcLocalPlayStatus(IPOD_DR_UNKNOWN);
            break;
        case IPOD_TM_TYPE_FF:
            //if(nIapSvc != NULL)
            //    nIapSvc->SetSvcLocalFFRewStatus(false);
            break;
        case IPOD_TM_TYPE_REW:
            //if(nIapSvc != NULL)
            //    nIapSvc->SetSvcLocalFFRewStatus(false);
            break;
        case IPOD_TM_TYPE_ENDFFREW:
            if(nIapSvc != NULL)
                nIapSvc->SetSvcLocalPlayStatus(IPOD_DR_UNKNOWN, type);
            break;
    }
    #endif
    return;
}


CategoryType IPOD_GetUpperCategory( 
                                                                    CategoryType eCurrentType,
                                                                    CategoryType eRootType
                                                                    )
{
    CategoryType eUpperCategory = IPOD_INVALID_CATEGORY;
    switch((int)eRootType){
        case IPOD_PLAYLIST:
            if(eCurrentType == IPOD_TRACK) eUpperCategory = IPOD_PLAYLIST;
            else{
                IAPSVC_DBUG("%d: %s() eCurrentType %d eRootType %d", 
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            eCurrentType,
                                                                                            eRootType
                                                                                            );
            }
            break;
        case IPOD_ALBUM:
            if(eCurrentType == IPOD_TRACK) eUpperCategory = IPOD_ALBUM;
            else{
                IAPSVC_DBUG("%d: %s() eCurrentType %d eRootType %d", 
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            eCurrentType,
                                                                                            eRootType
                                                                                            );
            }
            break;
        case IPOD_ARTIST:
            if(eCurrentType == IPOD_ALBUM) eUpperCategory = IPOD_ARTIST;
            else if(eCurrentType == IPOD_TRACK) eUpperCategory = IPOD_ALBUM;
            else{
                IAPSVC_DBUG("%d: %s() eCurrentType %d eRootType %d", 
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            eCurrentType,
                                                                                            eRootType
                                                                                            );
            }
            break;
        case IPOD_COMPOSER:
            if(eCurrentType == IPOD_ALBUM) eUpperCategory = IPOD_COMPOSER;
            else if(eCurrentType == IPOD_TRACK) eUpperCategory = IPOD_ALBUM;            
            else{
                IAPSVC_DBUG("%d: %s() eCurrentType %d eRootType %d", 
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            eCurrentType,
                                                                                            eRootType
                                                                                            );
            }
            break;
        case IPOD_PODCAST:
            if(eCurrentType == IPOD_TRACK) eUpperCategory = IPOD_PODCAST;
            else{
                IAPSVC_DBUG("%d: %s() eCurrentType %d eRootType %d", 
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            eCurrentType,
                                                                                            eRootType
                                                                                            );
            }
            break;
        case IPOD_GENRE:
            if(eCurrentType == IPOD_ARTIST) eUpperCategory = IPOD_GENRE;
            else if(eCurrentType == IPOD_ALBUM) eUpperCategory = IPOD_ARTIST;
            else if(eCurrentType == IPOD_TRACK) eUpperCategory = IPOD_ALBUM;     
            else{
                IAPSVC_DBUG("%d: %s() eCurrentType %d eRootType %d", 
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            eCurrentType,
                                                                                            eRootType
                                                                                            );
            }
            break;
        default:
            IAPSVC_DBUG("%d: %s() eCurrentType %d eRootType %d", 
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            eCurrentType,
                                                                                            eRootType
                                                                                            );
            break;
    }

    return eUpperCategory;
}

CategoryType IPOD_GetSubCategory( 
                                                                CategoryType eCurrentType,
                                                                CategoryType eRootType
                                                                )
{
    CategoryType eSubCategory = IPOD_INVALID_CATEGORY;

    switch((int)eRootType){
        case IPOD_PLAYLIST: eSubCategory = IPOD_TRACK; break;
        case IPOD_ALBUM: eSubCategory = IPOD_TRACK; break;
        case IPOD_ARTIST: 
            switch((int)eCurrentType){
                case IPOD_ARTIST: eSubCategory = IPOD_ALBUM; break;
                case IPOD_ALBUM: eSubCategory = IPOD_TRACK; break;
                default:
                    IAPSVC_DBUG("%d: %s() eCurrentType %d eRootType %d", 
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            eCurrentType,
                                                                                            eRootType
                                                                                            );
                    break;                
            }
            break;
        case IPOD_COMPOSER:
            switch((int)eCurrentType){
                case IPOD_COMPOSER: eSubCategory = IPOD_ALBUM; break;
                case IPOD_ALBUM: eSubCategory = IPOD_TRACK; break;
                default:
                    IAPSVC_DBUG("%d: %s() eCurrentType %d eRootType %d", 
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            eCurrentType,
                                                                                            eRootType
                                                                                            );
                    break;                
            }
            break;
        case IPOD_PODCAST: eSubCategory = IPOD_TRACK; break;
        case IPOD_GENRE:
            switch((int)eCurrentType){
                case IPOD_GENRE: eSubCategory = IPOD_ARTIST; break;
                case IPOD_ARTIST: eSubCategory = IPOD_ALBUM; break;
                case IPOD_ALBUM: eSubCategory = IPOD_TRACK; break;
                default:
                    IAPSVC_DBUG("%d: %s() eCurrentType %d eRootType %d", 
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            eCurrentType,
                                                                                            eRootType
                                                                                            );
                    break;                
            }
            break;
        default:
            IAPSVC_DBUG("%d: %s() eCurrentType %d eRootType %d", 
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            eCurrentType,
                                                                                            eRootType
                                                                                            );
            break;            
    }
    return eSubCategory;
}

static void *ipod_thread_test(void *param)
{   
    int Policy = 0;
    pthread_attr_t attr;
    struct sched_param param1;
    if (pthread_attr_getschedparam(&attr, &param1) != 0) {
            ALOGE("%d: %s() error pthread_attr_getschedparam", __LINE__, __FUNCTION__);
    } 
    ALOGE("%d: %s() priority %d", __LINE__, __FUNCTION__,param1.sched_priority);

    if(pthread_getschedparam(pthread_self(), &Policy, &param1) == 0){
        ALOGE("%d: %s() priority %d policy %d", __LINE__, __FUNCTION__,param1.sched_priority,Policy);
    }
    ALOGE("%d: %s() error pthread_getschedparam", __LINE__, __FUNCTION__);
    while(1){
    }
    return 0;
}

IapService::IapService()
    :BnIapService()
{
    short *pBuff = NULL;

    if(IPOD_Check_WithIap2()){
        property_set(PROP_IAP1_SUPPORT_ONLY,"f");
        property_set(PROP_IAP2_SUPPORT,"t");        
    }else{
        property_set(PROP_IAP1_SUPPORT_ONLY,"t");
        property_set(PROP_IAP2_SUPPORT,"f");  
    }

    InitDebug();
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    iPodControlState = IPOD_CONTROL_STATE_NONE;
    piPodLibApiHandle = 0;
    fUsedAsync = true;
    gLastEventTime = 0;

    gAudioBuff = (short *)malloc(sizeof(short)*AUDIO_CHUNK_BYTES);
    if(gAudioBuff == NULL){
        ALOGE("%d: %s() malloc failed", __LINE__, __FUNCTION__);
    }
    IPOD_Mem_StackLargeBuff();
    
    mIapCommandThread = NULL;
    mIapEventThread = NULL;
    mIapAudioThread = NULL;

    mIapTimer = NULL;
    startTimerThread();
    SetUsbConnected(false);
    mStatusManager = new IapStatusManager(this,mIapTimer); 
    SetSvcLocalFFRewStatus(false);
    
    startCommandThread();
    startAudioThread();

    IPOD_SetIapSvc(this);
    sIpodStackInterface = iPod_get_ipod_interface();
    IPOD_InitIpodInterface();
    IPOD_API_EAProtocolTokenInit();
    IPOD_Malloc_iPodBufInit();
}

IapService::~IapService()
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    mStatusManager = NULL;
    stopTimerThread();
    stopCommandThread();
    stopEventThread();
    stopAudioThread();
    if(gAudioBuff)
        free(gAudioBuff);
    IPOD_Free_StackLargeBuff();
}

void IapService::SetUsbConnected(bool flag)
{
    IAPSVC_DEFLOG("%s() %s", __FUNCTION__,(flag == true)? "USB cable is connected":"USB cable is disconnected");
    mUsbConnected = flag;
}

bool IapService::GetUsbConnected(void)
{
    return mUsbConnected;
}

void IapService::SetSvcState(int state)
{
    if((int)mSvcState != state){
        IAPSVC_DEFLOG("%s() change %d -> %s", __FUNCTION__,
                                    mSvcState,
                                    (state == IAP_SVC_STATE_DISCONNECT)? "disconnect 0":
                                    (state == IAP_SVC_STATE_HW_CONNECT)?"USB cable is connected 1":
                                    (state == IAP_SVC_STATE_CONNECT_SUCCESS)?"iAP is connected 2":
                                    (state == IAP_SVC_STATE_CONNECT_FAIL)? "iAP connection is failed 3":
                                        "Unknow state");
    }
    mSvcState = state;
}

int IapService::GetSvcState(void)
{
    return mSvcState;
}

void IapService::InitDebug(void)
{
    //if(IAPSVC_DEBUG == -1)
    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.next.previous", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                NEXT_PREVIOUS_CMD_FLAG = 1;
                break;
            case 'f':
            case 'F':
            default:
                NEXT_PREVIOUS_CMD_FLAG = 0;
                break;
        }
    }  
    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.debug.svc", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPSVC_DEBUG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPSVC_DEBUG = 0;
                break;
        }
    }    
    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.debug.svc.data", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPSVC_DDEBUG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPSVC_DDEBUG = 0;
                break;
        }
    }    
    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.debug.svc.def", iapDebug, "t");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPSVC_DEFLOG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPSVC_DEFLOG = 0;
                break;
        }
    }  
    IPOD_Sample_InitDebug();
    IPOD_API_InitDebug();
    IPOD_OS_InitDebug();
    IPOD_HID_InitDebug();
    IPOD_CP_InitDebug();
    IPOD_PAL_InitDebug();
    #ifdef USE_TIMER
    iPod_Time_InitDebug();
    #endif
}

bool IapService::IsValidEapSessionId(unsigned long sessionID)
{
    bool ret = false;
    Mutex::Autolock lock(mEaSessionLock); 
    ssize_t index = mEapSessionList.indexOfKey(sessionID);
    if (index >= 0) {
        ret = true;
    }
    return ret;
}

void IapService::ClearEapSessionList(void)
{
    Mutex::Autolock lock(mEaSessionLock);
    mEapSessionList.clear();
}

void IapService::ClearReadySessionList(void)
{
    Mutex::Autolock lock(mReadySessioLock);
    mReadySessionList.clear();
}

bool IapService::IsEmptyReadySessionList(void)
{
    Mutex::Autolock lock(mReadySessioLock);
    const int listCnt = mReadySessionList.size();
    if(listCnt > 0){
        return false;
    }
    return true;
}

bool IapService::AddReadySessionList(unsigned long SessionID, 
                                                                unsigned long nProtocolIndex)
{
    Mutex::Autolock lock(mReadySessioLock);
    ssize_t index = mReadySessionList.indexOfKey(nProtocolIndex);
    IAPSVC_DBUG("%d: %s() session %ld protocol %ld index %d ", 
                                __LINE__, __FUNCTION__,SessionID,nProtocolIndex,(int)index);
    if(index >= 0)
        mReadySessionList.removeItem(nProtocolIndex);
    mReadySessionList.add(nProtocolIndex,SessionID);
    return true;
}

bool IapService::RemoveReadySessionListBySessionId(unsigned long SessionID)
{
    Mutex::Autolock lock(mReadySessioLock);
    const int listCnt = mReadySessionList.size();
    int index;
    for (index=0; index<listCnt; index++) {
        int sessionId = mReadySessionList.valueAt(index);
        if(sessionId == (int)SessionID){
           break; 
        }
    }
    IAPSVC_DBUG("%d: %s() session %ld index %d listCnt %d", 
                                __LINE__, __FUNCTION__,SessionID,index,listCnt);
    if(index != listCnt){
        int ret = mReadySessionList.removeItemsAt((size_t)index,1);
        int listSize = mReadySessionList.size();
        IAPSVC_DBUG("%d: %s() session %ld index %d ret %d listCnt %d", 
                                __LINE__, __FUNCTION__,SessionID,index,ret,listSize);

    }
    return true;
}

bool IapService::RemoveReadySessionListByProtocolId(unsigned long nProtocolIndex)
{
    Mutex::Autolock lock(mReadySessioLock);
    ssize_t index = mReadySessionList.indexOfKey(nProtocolIndex);
    IAPSVC_DBUG("%d: %s() protocol %ld index %d ", 
                                __LINE__, __FUNCTION__,nProtocolIndex,(int)index);
    if(index >= 0)
        mReadySessionList.removeItem(nProtocolIndex);
    return true;
}

bool IapService::ProcCheckReadySession(int nProtocolIndex)
{
    ssize_t indexReadyList;
    unsigned long sessionId;
    ssize_t indexProtocol;
    ssize_t indexListener;
    {
        Mutex::Autolock lock(mReadySessioLock);
        indexReadyList= mReadySessionList.indexOfKey(nProtocolIndex);
        IAPSVC_DBUG("%d: %s() protocol %d indexReadyList %d ", 
                                    __LINE__, __FUNCTION__,nProtocolIndex,(int)indexReadyList);
        if(indexReadyList >= 0)
        {
            sessionId = (unsigned long )mReadySessionList.valueAt(indexReadyList);
        }
    }
    if(indexReadyList >= 0)
    {
        indexProtocol = mEapProtocolList.indexOfKey(nProtocolIndex);
        if(indexProtocol >= 0){
            {
                Mutex::Autolock lock(mListenerLock);    
                String8 listenerKey = mEapProtocolList.valueAt(indexProtocol);  
                indexListener = mListenerList.indexOfKey(listenerKey);
            }
            if(indexListener >= 0){
                sp<IIapServiceNotify> listener = mListenerList.valueAt(indexListener);

                //RemoveReadySessionListByProtocolId(nProtocolIndex);
                notifyEapOpenData(sessionId,nProtocolIndex);
            }
        }
    }
    return true;
}


int IapService::CloseDataSession(unsigned long SessionID )
{
    IAPSVC_DEFLOG("%d: %s() SessionID %ld", __LINE__, __FUNCTION__,SessionID);
    ssize_t index;
    
    notifyEap(IPOD_APP_EAP_CLOSESESSION, SessionID, 0, 0,SessionID);
    {
        RemoveReadySessionListBySessionId(SessionID);
    }
    {
        Mutex::Autolock lock(mEaSessionLock); 
        index = mEapSessionList.indexOfKey(SessionID);
        IAPSVC_DBUG("%d: %s() index %ld", __LINE__, __FUNCTION__,index);
        if(index >= 0)
            mEapSessionList.removeItem(SessionID);
        IAPSVC_DBUG("%d: %s() remove SessionID %ld listSize %d", 
                                                                        __LINE__, 
                                                                        __FUNCTION__,
                                                                        SessionID,
                                                                        mEapSessionList.size()
                                                                        );
    }
    return (int)IPOD_NO_ERROR;
}

int IapService::notifyEapOpenData(
                                                                unsigned long SessionID, 
                                                                unsigned long nProtocolIndex ) 
{
    int i;
    IAPSVC_DEFLOG("%d: %s() SessionID %ld protocolIndex %ld",
                                                                                        __LINE__, 
                                                                                        __FUNCTION__,
                                                                                        SessionID,
                                                                                        nProtocolIndex
                                                                                        );
    ssize_t index = mEapProtocolList.indexOfKey(nProtocolIndex);
    if (index < 0) {
        IAPSVC_DEFLOG("%d: %s() can not find the proper Eap List",
                                                                                        __LINE__, 
                                                                                        __FUNCTION__
                                                                                        );
        return IPOD_BAD_PARAMETER;
    }
    IAPSVC_DBUG("%d: %s() protocolIndex %ld",__LINE__,__FUNCTION__,index);
    String8 listenerKey = mEapProtocolList.valueAt(index);    
    {
        Mutex::Autolock lock(mListenerLock);    
        index = mListenerList.indexOfKey(listenerKey);
        AddReadySessionList(SessionID,nProtocolIndex);

        if (index < 0) {
            IAPSVC_DBUG("%d: %s() can not find the proper Listener",
                                                                                            __LINE__, 
                                                                                            __FUNCTION__
                                                                                            );
            return IPOD_BAD_PARAMETER;
        }
        IAPSVC_DBUG("%d: %s() listener Index %ld",__LINE__,__FUNCTION__,index);
        sp<IIapServiceNotify> listener = mListenerList.valueAt(index);

        mEapSessionList.add(SessionID,listener);
        IAPSVC_DBUG("%d: %s() mEapSessionList add SessionID %ld listSize %d", 
                                                                            __LINE__, 
                                                                            __FUNCTION__,
                                                                            SessionID,
                                                                            mEapSessionList.size()
                                                                            );
    }
    notifyEap(
                IPOD_APP_EAP_OPENSESSION,
                SessionID,
                nProtocolIndex,
                0,
                SessionID
                );
    return IPOD_NO_ERROR;
}

bool IapService::IsValidTimeSchedule(int64_t newTime)
{
    int64_t lastTime = GetLastEventTime();
    int64_t diff = newTime - lastTime;
    
    //if(nanoseconds_to_seconds(nsecs_t(diff)) >= 1)
    if(diff  >= 1020000000){
        SetLastEventTime(newTime);
        return true;
    }

    //if( diff >= 800000000)
    {
        IAPSVC_DBUG("%d: %s()  diff(%lld)", __LINE__, __FUNCTION__,diff);
        usleep((1100000000 - diff)/1000);
         int64_t localNewTime = (int64_t)systemTime();
         diff = localNewTime - lastTime;
         IAPSVC_DBUG("%d: %s()  diff(%lld)", __LINE__, __FUNCTION__,diff);
         if(diff  >= 1020000000){
            SetLastEventTime(localNewTime);
            return true;
        }
    }
    
    IAPSVC_DBUG("%d: %s()  diff(%lld) second(%lld)", 
                                                                        __LINE__, 
                                                                        __FUNCTION__,
                                                                        diff,
                                                                        nanoseconds_to_seconds(nsecs_t(diff)) 
                                                                        );
    return false;
}

bool IapService::UpdatePlayInfoTimeSchedule(int eventType)
{
    int64_t localNewTime = (int64_t)systemTime();
    SetLastEventPlayInfoTime(eventType,localNewTime);
    return true;
}

bool IapService::IsValidPlayInfoTimeSchedule(int eventType)
{
    if(eventType >= IPOD_TIME_SCHEDULE_MAX)
        return true;
    int64_t lastTime = GetLastEventPlayInfoTime(eventType);
    int64_t newTime = (int64_t)systemTime();
    int64_t diff = newTime - lastTime;
    int64_t checkTime = 1500000000; // 1.5 s
    int64_t remainTime = 0;
    int64_t intervalTime = 10000000; // 10 ms

    //if(nanoseconds_to_seconds(nsecs_t(diff)) >= 1)
    if(diff  >= checkTime){
        SetLastEventPlayInfoTime(eventType,newTime);
        return true;
    }

    //if( diff >= 800000000)
    {
        IAPSVC_DBUG("%d: %s()  diff(%lld)", __LINE__, __FUNCTION__,diff);
        {
            int i;
            int cnt = (checkTime / intervalTime) + 10;
            remainTime = checkTime - diff;
            for(i = 0; i <cnt && remainTime > 0; i++){
                int64_t sleepTime = remainTime;
                if(remainTime > intervalTime)
                    sleepTime = intervalTime;
                remainTime -= sleepTime;
                usleep(sleepTime/1000);
                if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) != IPOD_CONNECTED){
                    return false;
                }
            }
        }
        //usleep((checkTime - diff)/1000);
         int64_t localNewTime = (int64_t)systemTime();
         ALOGI("%d: %s()  diff(%lld) end(%lld)", __LINE__, __FUNCTION__,diff, localNewTime - lastTime);
         diff = localNewTime - lastTime;

         if(diff  >= checkTime){
            SetLastEventPlayInfoTime(eventType,localNewTime);
            return true;
        }
    }
    
    IAPSVC_DBUG("%d: %s()  end diff(%lld) second(%lld)", 
                                                                    __LINE__, 
                                                                    __FUNCTION__,
                                                                    diff,
                                                                    nanoseconds_to_seconds(nsecs_t(diff)) 
                                                                    );
    return false;
}

void *IapService::GetIPodHandle(void)
{
    return piPodLibApiHandle;
}

void IapService::SetIPodHandle(void *pHandle)
{
    piPodLibApiHandle = pHandle;
}

bool IapService::IsUsedAsync(void)
{
    //return false;
    return fUsedAsync;
}

int IapService::GetSvcLocalPlayStatus(void)
{
    int playStatus =IPOD_DR_UNKNOWN;
    if(mStatusManager != NULL)
        playStatus = mStatusManager->GetLocalPlayStatus();
    return playStatus;
}

void IapService::SetSvcLocalPlayStatus(int status)
{
    if(mStatusManager != NULL)
        mStatusManager->SetLocalPlayStatus(status,0,0);
}

void IapService::SetSvcLocalPlayStatus(int status,int type)
{
    int expectNextStatus = -1;
    if(mStatusManager != NULL){
        expectNextStatus = mStatusManager->GetWantedNextAction();
        mStatusManager->SetLocalPlayStatus(status,0,0);
        if(expectNextStatus == IPOD_DR_PLAYBACK_PAUSED){
            IPOD_DR_PLAY_STATUS playStatus = IPOD_DR_UNKNOWN;

            Sample_GetDRPlayStatus(&playStatus);
            if(playStatus == IPOD_DR_PLAYING){
                if(SendNextAction(PLAYPAUSE)){
                    mStatusManager->SetLocalPlayStatus(IPOD_DR_PLAYBACK_PAUSED,0,0);
                }
            }
        }else if(expectNextStatus == IPOD_DR_PLAYING){
            IPOD_DR_PLAY_STATUS playStatus = IPOD_DR_UNKNOWN;

            Sample_GetDRPlayStatus(&playStatus);
            if(playStatus == IPOD_DR_PLAYBACK_PAUSED){
                if(SendNextAction(PLAYPAUSE)){
                    mStatusManager->SetLocalPlayStatus(IPOD_DR_PLAYING,0,0);
                }
            }
        }
    }
}

void IapService::SetSvcLocalPlayStatus(int status,int sendNoti,int cmdFrom)
{
    if(mStatusManager != NULL)
        mStatusManager->SetLocalPlayStatus(status,sendNoti,cmdFrom);
}

bool IapService::GetSvcLocalFFRewStatus(void)
{
    bool ffrewMode =IPOD_DR_UNKNOWN;
    if(mStatusManager != NULL)
        ffrewMode = mStatusManager->GetLocalFFRewStatus();
    return ffrewMode;
}

void IapService::SetSvcLocalFFRewStatus(bool status)
{
    if(mStatusManager != NULL)
        mStatusManager->SetLocalFFRewStatus(status);
}

int IapService::GetSvcApplePlayStatus(void)
{
    int playStatus =IPOD_DR_UNKNOWN;
    if(mStatusManager != NULL)
        playStatus = mStatusManager->GetApplePlayStatus();
    return playStatus;
}

void IapService::SetSvcApplePlayStatus(int status)
{
    if(mStatusManager != NULL)
        mStatusManager->SetApplePlayStatus(status);
    IPOD_Sample_SetDRPlayStatus(GetIPodHandle(),(IPOD_DR_PLAY_STATUS)status);
}

bool IapService::GetSvcAppleFFRewStatus(void)
{
    bool status = false;
    if(mStatusManager != NULL)
        status = mStatusManager->GetAppleFFRewStatus();
    return status;
}

void IapService::SetSvcAppleFFRewStatus(bool status)
{
    if(mStatusManager != NULL)
        mStatusManager->SetAppleFFRewStatus(status);
}


void IapService::SetSvcWantedNextAction(int status)
{
    if(mStatusManager != NULL)
        mStatusManager->SetWantedNextAction(status);
}

bool IapService::SendNextAction(int type)
{
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    IAPSVC_DEFLOG("%d: %s() PlayControl Type %d ", __LINE__, __FUNCTION__,type);
    eCMDErrorCode= IPOD_PlayControl( GetIPodHandle(), (PlayCmdCode)type );
    if(IPOD_NO_ERROR == eCMDErrorCode){
        return true;
    }else{
        ALOGE(" %s  > IPOD_PlayControl() Failed(0x%x)",__FUNCTION__,eCMDErrorCode);
    }
    return false;
}

void IapService::InitVariable(const Parcel *data)
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    int dispDataSize = data->readInt32();

    {
        String8 accName = String8(data->readString16());
        String8 manufacturer = String8(data->readString16());
        String8 modelName = String8(data->readString16());
        int fwVerMajor = data->readInt32();
        int fwVerMinor = data->readInt32();
        int fwVerRevision = data->readInt32();
        int hwVerMajor = data->readInt32();
        int hwVerMinor = data->readInt32();
        int hwVerRevision = data->readInt32();
        int gRepeatState = data->readInt32();
        int gShuffleMode = data->readInt32();
        gNextPlayPause = data->readInt32();

        IPOD_API_SetAccInfoInit();
        IPOD_API_SetAccName(accName.string(),accName.size());
        IPOD_API_SetMenufacturer(manufacturer.string(),manufacturer.size());
        IPOD_API_SetModelNumber(modelName.string(),modelName.size());
        IPOD_API_SetFirmwareVerInAccInfo(fwVerMajor,fwVerMinor,fwVerRevision);
        IPOD_API_SetHardwareVerInAccInfo(hwVerMajor,hwVerMinor,hwVerRevision);
        setRepeatState(gRepeatState);
        setShuffleMode(gShuffleMode);

        ALOGI(" Name %s manufacturer %s model %s f [%d:%d:%d] s [%d:%d:%d] g [%d:%d:%d]", 
                                                                                            accName.string(), 
                                                                                            manufacturer.string(),
                                                                                            modelName.string(),
                                                                                            fwVerMajor,
                                                                                            fwVerMinor,
                                                                                            fwVerRevision,
                                                                                            hwVerMajor,
                                                                                            hwVerMinor,
                                                                                            hwVerRevision,
                                                                                            gRepeatState,
                                                                                            gShuffleMode,
                                                                                            gNextPlayPause
                                                                                            );

        int eaProtocolCount = data->readInt32();

        IPOD_API_SetEAProtocolTokenInit();
        if(eaProtocolCount > 0){
            int i;
            int eaProtocolIndex = 1;
            IPOD_ERROR ret = IPOD_NO_ERROR;
            for(i = 0; i < eaProtocolCount;i++){
                String8 eaProtocolString = String8(data->readString16());

                IAPSVC_DBUG(" EAP %s", eaProtocolString.string());
                
                ret = IPOD_API_AddEAProtocol(
                                                        (char const *)eaProtocolString.string(),
                                                        eaProtocolString.length()
                                                        );
                if(ret != IPOD_NO_ERROR){
                    ALOGE("%d: %s() add EAP ret = 0x%x", __LINE__, __FUNCTION__,ret);
                }
            }
            int bundleSeedIdStringExit = data->readInt32();
            if(bundleSeedIdStringExit != 0){
                String8 bundleSeedIdString = String8(data->readString16());
                IAPSVC_DBUG(" bundleSeed %s", bundleSeedIdString.string());
                ret = IPOD_API_SetBundleSeedIDString(
                                                            (char const*)bundleSeedIdString.string(),
                                                            bundleSeedIdString.length()
                                                            );
                if(ret != IPOD_NO_ERROR){
                    ALOGE("%d: %s() Set bundleSeed ret = 0x%x", __LINE__, __FUNCTION__,ret);
                }
            }

            //IPOD_API_SetEAProtocolMeta(1,0);
        }


    }
    
    IAPSVC_DBUG("%d: %s() iPodControlState = %d dispDataSize  %d", 
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            iPodControlState,
                                                                                            dispDataSize
                                                                                            );
    IPOD_API_SetDisplayDataSize(dispDataSize);
}

bool IapService::CheckElapsedTime(int64_t startTime)
{
    bool ret = false;
    int64_t currentTime =  (int64_t)systemTime();
    if((currentTime - startTime) >= IPOD_INIT_TIMEOUT)
    {
        ret = true;
        ALOGI(" %s ElapsedTime error 0x%x",__FUNCTION__, IPOD_INIT_TIMEOUT_ERROR);
    }
    return ret;
}

bool IapService::ProcInit(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    unsigned long InitRetry;
    IPOD_INIT_PARAM stDeviceInitParam;
    IPOD_LINGO_INFO_DATA stLingoInfoData;
    unsigned long CheckOption=1;
    IPOD_DEFAULT_CONNECTION IsSupportIDPS;
    int isExistHid = 0;    
    int startIDPSFailCnt = 0;
    int isWithStorage = 1;
    int64_t startTime = 0;

    IAPSVC_DEFLOG("%d: %s() isExistHid %d", __LINE__, __FUNCTION__,isExistHid);

    ClearEapSessionList();
    ClearReadySessionList();
    startTime = (int64_t)systemTime();

    for(InitRetry = 0; InitRetry<IPOD_INIT_RETRY_CNT; InitRetry++){

        isExistHid = IsExistHid();
        if(!isExistHid){
            eCMDErrorCode = IPOD_DISCONNECT_ERROR;
            ALOGI(" %s(%d) cannot find %s.",__FUNCTION__,__LINE__,HID_DEV);
            break;
        }
        if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) != IPOD_CONNECTED){
            eCMDErrorCode = IPOD_DISCONNECT_ERROR;
            ALOGI(" %s(%d) GetDetectStatus invalid.",__FUNCTION__,__LINE__);
            break;
        }
        if(CheckElapsedTime(startTime)){
            eCMDErrorCode = IPOD_INIT_TIMEOUT_ERROR;
            break;
        }  
        ALOGI(" %s(%d) IPOD_CheckiPodStartIDPS.",__FUNCTION__,__LINE__);
        if(IPOD_IDPS_TIMEOUT == eCMDErrorCode){
            CheckOption = 0;
            eCMDErrorCode = IPOD_NO_ERROR;
        }
        else{
            eCMDErrorCode = IPOD_CheckiPodStartIDPS((void *)GetIPodHandle());
        }

        if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) != IPOD_CONNECTED){
            eCMDErrorCode = IPOD_DISCONNECT_ERROR;
            ALOGI(" %s(%d) GetDetectStatus invalid.",__FUNCTION__,__LINE__);
            break;
        }
        if(CheckElapsedTime(startTime)){
            eCMDErrorCode = IPOD_INIT_TIMEOUT_ERROR;
            break;
        }        
        if(IPOD_NO_ERROR == eCMDErrorCode){
            IAPSVC_DBUG(" %s(%d) ",__FUNCTION__,__LINE__);

            IPOD_API_USB_InitParam(GetIPodHandle(), 
                                                        &stDeviceInitParam, 
                                                        &stLingoInfoData, 
                                                        isWithStorage
                                                        );

            registrerEapSessionId((void *)&stDeviceInitParam);
            
            if(1 == CheckOption){
                ALOGI(" %s(%d) ",__FUNCTION__,__LINE__);
                IPOD_GetiPodOptionForLingo(GetIPodHandle(), &stDeviceInitParam);
            }
            if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) != IPOD_CONNECTED){
                eCMDErrorCode = IPOD_DISCONNECT_ERROR;
                ALOGI(" %s(%d) GetDetectStatus invalid.",__FUNCTION__,__LINE__);
                break;
            }
            if(CheckElapsedTime(startTime)){
                eCMDErrorCode = IPOD_INIT_TIMEOUT_ERROR;
                break;
            }     
            ALOGI(" %s(%d) ",__FUNCTION__,__LINE__);
            IPOD_GetCheckIDPSSupport(GetIPodHandle(), &IsSupportIDPS);
            ALOGI(" %s(%d) IsSupportIDPS(%d)",
                                                            __FUNCTION__,
                                                            __LINE__,
                                                            IsSupportIDPS
                                                            );
            if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) != IPOD_CONNECTED){
                eCMDErrorCode = IPOD_DISCONNECT_ERROR;
                ALOGI(" %s(%d) GetDetectStatus invalid.",__FUNCTION__,__LINE__);
                break;
            }
            if(CheckElapsedTime(startTime)){
                eCMDErrorCode = IPOD_INIT_TIMEOUT_ERROR;
                break;
            }  
            //playAudio();//
            //initAudio();
            if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) != IPOD_CONNECTED){
                eCMDErrorCode = IPOD_DISCONNECT_ERROR;
                ALOGI(" %s(%d) GetDetectStatus invalid.",__FUNCTION__,__LINE__);
                break;
            }
            if(CheckElapsedTime(startTime)){
                eCMDErrorCode = IPOD_INIT_TIMEOUT_ERROR;
                break;
            }  
            ALOGI(" %s(%d) start IdenAuth .",__FUNCTION__,__LINE__);
            eCMDErrorCode = IPOD_IdentifyAuth_Process(
                                                                    GetIPodHandle(), 
                                                                    &stDeviceInitParam 
                                                                    );
            ALOGI(" %s(%d) IdentifyAuth (0x%x).",__FUNCTION__,__LINE__,eCMDErrorCode);
            if((IPOD_COMMAND_TIMEOUT == eCMDErrorCode) 
                || (IPOD_COMMAND_FAILED == eCMDErrorCode) 
                //||(IPOD_IO_CP_ERROR == eCMDErrorCode) 
                //|| (IPOD_IDPS_TIMEOUT == eCMDErrorCode) 
                || (IPOD_IDPS_RETRY_IDPS == eCMDErrorCode)
                || (IPOD_IDPS_RETRY_STARTIDPS  == eCMDErrorCode)
                || (IPOD_AUTH_INFO_DATA_ERROR == eCMDErrorCode)
                || (IPOD_AUTH_INFO_TIMEOUT_ERROR == eCMDErrorCode)){
                IPOD_ClearIDPSStatus(GetIPodHandle());
                IPOD_SetReStartIDPS(GetIPodHandle());
                continue;
            }else if(IPOD_AUTH_INFO_ERROR == eCMDErrorCode){
                ALOGI(" %s(%d) with Storage Lingo (%d).",__FUNCTION__,__LINE__,isWithStorage);
                // Nano 1G/2G doesn't support the Storage Lingo
                if(isWithStorage != 0){                    
                    isWithStorage = 0;
                    IPOD_ClearIDPSStatus(GetIPodHandle());
                    continue;
                }
                break;
            }else if(	IPOD_AUTH_SIG_ERROR == eCMDErrorCode
                || IPOD_AUTH_CP_ERROR == eCMDErrorCode
                || 	IPOD_AUTH_CP_INIT_ERROR == eCMDErrorCode
                || 	IPOD_AUTH_CERTIFICATE_READ_ERROR == eCMDErrorCode
                || IPOD_IO_CP_ERROR == eCMDErrorCode){
                ALOGI(" %s(%d) cp error.",__FUNCTION__,__LINE__);
                IPOD_CP_Clear();
                break;
            }else if(IPOD_IDPS_TIMEOUT == eCMDErrorCode) {
            		continue;
            }else{
                break;
            }
        }
        else
        {
            ALOGE(" %s(%d)  IPOD_CheckiPodStartIDPS. result (0x%x)",
                                                                                __FUNCTION__,
                                                                                __LINE__,
                                                                                eCMDErrorCode
                                                                                );
            if(IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) 
                == IPOD_DISCONNECTED){
                break;
            }
            if(CheckElapsedTime(startTime)){
                eCMDErrorCode = IPOD_INIT_TIMEOUT_ERROR;
                break;
            }  
            if(eCMDErrorCode == IPOD_COMMAND_TIMEOUT){
                startIDPSFailCnt++;
            }
        }
    }
    if ( IS_CMDOK(eCMDErrorCode) ){
        long nCurrentIndex = 0;
        long nCount = 0;
        ALOGI(" %s(%d) iPod init success",__FUNCTION__,__LINE__);

        IPOD_Sample_CheckPlayStatus(GetIPodHandle());
        SetSvcLocalFFRewStatus(false);
        if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) != IPOD_CONNECTED){
            eCMDErrorCode = IPOD_DISCONNECT_ERROR;
            ALOGI(" %s(%d) GetDetectStatus invalid.",__FUNCTION__,__LINE__);
        }else{
            ALOGI(" %s(%d) ",__FUNCTION__,__LINE__);
            if(IPOD_Sample_DB_Init(GetIPodHandle())>0)
            {
                void *ExtInfo = IPOD_GetExtInfo(GetIPodHandle());
                pIPOD_PLAY_INFO pInfo  = (pIPOD_PLAY_INFO)ExtInfo;
                IAPSVC_DBUG(" %s(%d) ",__FUNCTION__,__LINE__);
                iPodControlState = IPOD_CONTROL_STATE_INITIALIZED;
                IsSuccess = true;

                pInfo->mRootCategory = (CategoryType)-1;
                pInfo->mCurrentCategory = (CategoryType)-1;
                IAPSVC_DBUG(" %s(%d) IsSuccess = %d",__FUNCTION__,__LINE__,IsSuccess);
            }else{
                ALOGE(" %s(%d) IPOD_Sample_DB_Init failed",__FUNCTION__,__LINE__);
            }
        }
    }

    reply->writeInt32(IsSuccess);
    if(IsSuccess)
    {
        long ActualLen = 0;
        long BufLen = 1024;
        unsigned char *iPodNameStr = (unsigned char *)malloc( BufLen );

        if(iPodNameStr){
            memset(iPodNameStr,0x00,BufLen);
            eCMDErrorCode = IPOD_RequestiPodName(
                                                                        GetIPodHandle(),
                                                                        iPodNameStr,
                                                                        BufLen,
                                                                        &ActualLen
                                                                        );
            if(eCMDErrorCode == IPOD_NO_ERROR){
                String16 str16 = String16((const char*)iPodNameStr);
                IAPSVC_DEFLOG(" %s(%d) iPodName %s",__FUNCTION__,__LINE__,iPodNameStr);
                reply->writeInt32(ActualLen);
                reply->writeString16(str16);
            }
            else{
                reply->writeInt32(0);
                ALOGE(" %s IPOD_RequestiPodName() > Failed(0x%x).",__FUNCTION__, eCMDErrorCode);
            }

            if(iPodNameStr)
                free(iPodNameStr);
        }else{
            reply->writeInt32(0);
        }
    }else{
        reply->writeInt32(0);
    }

    if(IsSuccess == false && startIDPSFailCnt == IPOD_INIT_RETRY_CNT){
        reply->writeInt32(1);
        ALOGE(" %s(%d) there is no response for the StartIDPS",__FUNCTION__,__LINE__);
    }else{
        reply->writeInt32(0);
    }

    IAPSVC_DEFLOG(" %s(%d) IsSuccess %d ",__FUNCTION__,__LINE__,IsSuccess);
    IPOD_PAL_SetInitializingFlag(0);
    return IsSuccess;
}

bool IapService::Initialize(const Parcel *data)
{
    bool ret = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    
    IAPSVC_DBUG("%d: %s() iPodControlState =%d", __LINE__, __FUNCTION__,iPodControlState);

    InitVariable(data);

    {
        Mutex::Autolock lock(mLibStackOsLock);
        if ( IPOD_CONTROL_STATE_NONE == iPodControlState ){
            unsigned long iPodLibApiHandle = 0;
            startEventThread();
            ALOGI(" %s(%d) libVer is %s, SvcVer is %s",__FUNCTION__,__LINE__,IPOD_GetLibVer(),IPOD_GetSvcVersion());
            eCMDErrorCode = IPOD_API_Init(&iPodLibApiHandle, HID_DEV, 0,IPOD_USB_CONNECT,IPOD_RX_POLLING);
            ALOGI(" %s(%d) IPOD_API_Init (0x%x).",__FUNCTION__,__LINE__,eCMDErrorCode);
            if(eCMDErrorCode == IPOD_NO_ERROR){
                SetIPodHandle((void *)iPodLibApiHandle);
                if((void *)iPodLibApiHandle != NULL){
                    eCMDErrorCode = IPOD_NO_ERROR;
                    ret= true;
                    if(GetUsbConnected() == false){
                        IPOD_SetDisconnectionStatus(GetIPodHandle());
                    }
                }
                else{
                    eCMDErrorCode = IPOD_INVALID_HANDLE;
                    ALOGE(" %s(%d) iPodLibApiHandle (0x%lx).",__FUNCTION__,__LINE__,iPodLibApiHandle);
                }            
            }else{
                ALOGE(" %s(%d) IPOD_API_Init (0x%x).",__FUNCTION__,__LINE__,eCMDErrorCode);
            }
        }
    }

    return ret;
}


void IapService::AfterInit(bool ret)
{
    if(ret == false){
        IAPSVC_DEFLOG("%d: %s()  ret = %d", __LINE__, __FUNCTION__,ret);
        if(GetSvcState() != IAP_SVC_STATE_DISCONNECT)
            SetSvcState(IAP_SVC_STATE_CONNECT_FAIL);
        iPodControlState = IPOD_CONTROL_STATE_NONE;
        Deinitialize(false);   
        IPOD_PAL_SetInitializingFlag(0);
    }else{
        SetSvcState(IAP_SVC_STATE_CONNECT_SUCCESS);
    }
}

bool IapService::InitPreparation(const Parcel *data)
{
    bool ret = false;
    bool result = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    int dataPos = data->dataPosition();
    InitDebug();
    SetUsbConnected(true);
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    SetSvcLocalPlayStatus(IPOD_DR_PLAYBACK_STOPPED);
    ret = Initialize(data);
    IAPSVC_DBUG("%d: %s() ret %d ", __LINE__, __FUNCTION__,ret);
    data->setDataPosition(dataPos);

    if(IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) != IPOD_CONNECTED){
        ret = false;
        ALOGI(" %s(%d) GetDetectStatus invalid.",__FUNCTION__,__LINE__);
    }
    return ret;
}

bool IapService::Init(const Parcel *data)
{
    bool ret = false;
    int classId = data->readInt32();
    int methodId = data->readInt32();
    IPOD_PAL_SetInitializingFlag(1);
    SetSvcState(IAP_SVC_STATE_HW_CONNECT);
    ret = InitPreparation(data);
    if(ret == true){
        if(IsUsedAsync())
            ret = mIapCommandThread->sendCommand(IAP_CMD_INIT,data,NULL,true);
        else{
            Parcel reply;
            ret = ProcInit(data,&reply);
        }
    }
    
    AfterInit(ret);

    return ret;
}

bool IapService::Deinitialize(bool fCausedByHw)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    if(fCausedByHw)
        SetUsbConnected(false);
    #ifdef USE_TIMER
    if(mIapTimer != NULL)
        iPod_Timer_CancelTimerEntry(mIapTimer,IPOD_TM_TYPE_ALL);
    #endif
    ClearEapSessionList();
    ClearReadySessionList();
    setDisconnected(fCausedByHw);
    stopEventThread();

    {
        Mutex::Autolock lock(mLibStackOsLock);
        iPodControlState = IPOD_CONTROL_STATE_NONE;
        ALOGI("%d: %s() 1 ", __LINE__, __FUNCTION__);
		IPOD_StopDataThread(GetIPodHandle());
		IPOD_StopControlThread(GetIPodHandle());
        eCMDErrorCode = IPOD_API_DeInit(GetIPodHandle());
        ALOGI("%d: %s() 2 ", __LINE__, __FUNCTION__);
        if ( IS_FAIL(eCMDErrorCode) ){
            IsSuccess = false;
        }else{
            IsSuccess = true;
        }
        SetIPodHandle(NULL);
    }
    stopAudio();
    setInitAfterAudio();
    IAPSVC_DEFLOG("%d: %s() end ", __LINE__, __FUNCTION__);
    return IsSuccess;
}

bool IapService::GetIapState(Parcel *reply)
{
    bool ret = false;
    IAPSVC_DEFLOG("%d: %s()  ", __LINE__, __FUNCTION__);

    if ( IsInitialized() ){
        if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED)
        {
            if(IsExistHid())
               ret = true;
            else
                IAPSVC_DEFLOG("%d: %s()  IPOD_HID_IsExistHid fail", __LINE__, __FUNCTION__);
        }else{
            IAPSVC_DEFLOG("%d: %s()  IPOD_GetDetectStatus fail", __LINE__, __FUNCTION__);
        }
    }else{
        IAPSVC_DEFLOG("%d: %s() IsInitialized fail ", __LINE__, __FUNCTION__);
    }
    reply->writeInt32(ret);
    
    return true;
}

int IapService::IsExistHid(void)
{
    int isExistHid = IPOD_HID_IsExistHid(HID_DEV);
    if(isExistHid == 0){
        SetSvcState(IAP_SVC_STATE_DISCONNECT);
    }
    return isExistHid;
}

bool IapService::GetIapSvcState(Parcel *reply)
{
    bool ret = false;
    unsigned int svcState = GetSvcState();
    int isExistHid = IsExistHid();
    IAPSVC_DEFLOG("%d: %s()  svcState %d isExistHid %d", __LINE__, __FUNCTION__,svcState,isExistHid);
    if(isExistHid == 0){
        svcState = IAP_SVC_STATE_DISCONNECT;
    }

    reply->writeInt32(svcState);

    return true;
}

bool IapService::ProcDeinit()
{
    IAPSVC_DBUG("%d: %s()  ", __LINE__, __FUNCTION__);
    mIapCommandThread->WaitUntilCmdFinish();
    return Deinitialize(true);
}

bool IapService::Deinit()
{
    bool ret;
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    SetSvcLocalPlayStatus(IPOD_DR_PLAYBACK_STOPPED);
    //if(IsUsedAsync())
    //    ret = mIapCommandThread->sendCommand(IAP_CMD_DEINIT,0,0,0,NULL,false);
    //else
    {
        ret = ProcDeinit();
    }
    
    return ret;
}

bool IapService::ProcTerminate()
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    int ret = 0;    
    
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    IPOD_DR_PLAY_STATUS playStatus;
    if(Sample_GetDRPlayStatus((void *)&playStatus))
    {
        if (IPOD_DR_PLAYING == playStatus)
        {
            //pauseAudio();
            IsSuccess = DoEndFfRew();
            ret = IPOD_PlayControl( GetIPodHandle(), PLAYPAUSE );
        }
    }

    return Deinitialize(true);
}

bool IapService::Terminate()
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                    IAP_CMD_TERMINATE,
                                                                    (const Parcel *)NULL,
                                                                    NULL,
                                                                    false
                                                                    );
    }else
        ret = ProcTerminate();
    return ret;
}

bool IapService::ProcDoPlay(Parcel *reply,int sendNoti,int cmdFrom)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    IAPSVC_DEFLOG("%d: %s() start IsFFREWMode(%d)", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        GetSvcLocalFFRewStatus()
                                                                        );

    if ( IsInitialized() )
    {
    	IPOD_DR_PLAY_STATUS playStatus;
    	unsigned long NewTrackInfoStatus;

        IsSuccess = DoEndFfRew();
        
        if(Sample_GetDRPlayStatus((void *)&playStatus))
    	{
    	    IAPSVC_DEFLOG("%d: %s() playStatus(%d) ",
                                                                    __LINE__,
                                                                    __FUNCTION__,
                                                                    playStatus
                                                                    );
            if (IPOD_DR_PLAYING == playStatus){
                IsSuccess = true;
            }else{
                if(GetSvcLocalPlayStatus() == IPOD_DR_END_FAST_FORWARD_OR_REWIND){
                        IAPSVC_DBUG("%s() wait end FF or Rew",__FUNCTION__);
                        SetSvcWantedNextAction(IPOD_DR_PLAYING);
                        IsSuccess = true;
                }else{
                    PlayCmdCode cmd = PLAYPAUSE;
                    if(cmdFrom == IAP_CMD_NEWPLAY)
                        cmd = NEWPLAY;
                    eCMDErrorCode = IPOD_PlayControl( GetIPodHandle(), cmd);

                    if ( IS_CMDOK(eCMDErrorCode) ){
                        IsSuccess = true;
                        SetSvcLocalPlayStatus(IPOD_DR_PLAYING,sendNoti,cmdFrom);
                        //IPOD_Sample_SetDRPlayStatus(GetIPodHandle(), IPOD_DR_PLAYING);
                    }else{
                        ALOGE(" %s  > IPOD_PlayControl() PlayPause Failed(0x%x)",
                                                                                            __FUNCTION__,
                                                                                            eCMDErrorCode
                                                                                            );
                    }
                }
            }
    	}
    	else{
    		ALOGE(" %s Invalid state", __FUNCTION__);
    	}
    }	
    else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    IAPSVC_DBUG("%d: %s() end eCMDErrorCode (%d)", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        eCMDErrorCode
                                                                        );
    reply->writeInt32(IsSuccess);
    return IsSuccess;
}

bool IapService::ProcPlay(Parcel *reply)
{
    bool IsSuccess = false;
    IAPSVC_DEFLOG("%d: %s() start IsFFREWMode(%d)", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        GetSvcLocalFFRewStatus()
                                                                        );
    IsSuccess = ProcDoPlay(reply,1,IAP_CMD_PLAY);
    return IsSuccess;
}

bool IapService::Play(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_PLAY,data,NULL,true);
    else{
        Parcel reply;
        ret = ProcPlay(&reply);
    }
    return ret;    
}

bool IapService::PlayInBrwsList()
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        
    }
    return true;
}

bool IapService::ProcNewPlay(Parcel *reply)
{
    bool IsSuccess = false;
    IAPSVC_DEFLOG("%d: %s() start IsFFREWMode(%d)", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        GetSvcLocalFFRewStatus()
                                                                        );
    IsSuccess = ProcDoPlay(reply,1,IAP_CMD_NEWPLAY);
    return IsSuccess;
}

bool IapService::NewPlay(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_NEWPLAY,data,NULL,true);
    else{
        Parcel reply;
        ret = ProcNewPlay(&reply);
    }
    return ret;    
}

bool IapService::ProcNewPause(Parcel *reply)
{
    bool IsSuccess = false;
    IAPSVC_DEFLOG("%d: %s() start IsFFREWMode(%d)", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        GetSvcLocalFFRewStatus()
                                                                        );
    IsSuccess = ProcDoPause(reply,1,IAP_CMD_NEWPAUSE);
    return IsSuccess;
}

bool IapService::NewPause(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_NEWPAUSE,data,NULL,true);
    else{
        Parcel reply;
        ret = ProcNewPause(&reply);
    }
    return ret;    
}

bool IapService::ProcDoPause(Parcel *reply,int sendNoti,int cmdFrom)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    int ret = 0;
    IAPSVC_DEFLOG("%d: %s() IsFFREWMode(%d)", 
                                                                __LINE__,
                                                                __FUNCTION__,
                                                                GetSvcLocalFFRewStatus()
                                                                );
    if ( IsInitialized() ) {
        IPOD_DR_PLAY_STATUS playStatus;
        IsSuccess = DoEndFfRew();

        if(Sample_GetDRPlayStatus((void *)&playStatus))
            eCMDErrorCode = IPOD_NO_ERROR;
        else
            eCMDErrorCode = IPOD_UNKNOWN_ERROR;
        IAPSVC_DEFLOG("%d: %s() eCMDErrorCode = %d playStatus = %d", 
                                                                            __LINE__, 
                                                                            __FUNCTION__,
                                                                            eCMDErrorCode,playStatus
                                                                            );
        if( IS_CMDOK(eCMDErrorCode) ){
            if (IPOD_DR_PLAYING == playStatus){						
                PlayCmdCode cmd = PLAYPAUSE;
                if(cmdFrom == IAP_CMD_NEWPAUSE)
                    cmd = NEWPAUSE;
                eCMDErrorCode = IPOD_PlayControl( GetIPodHandle(), cmd);
                if ( IS_CMDOK(eCMDErrorCode) ){
                    IsSuccess = true;		
                    SetSvcLocalPlayStatus(IPOD_DR_PLAYBACK_PAUSED);
                    //IPOD_Sample_SetDRPlayStatus(GetIPodHandle(), IPOD_DR_PLAYBACK_PAUSED);
                }
                else{
                    ALOGE(" %s  > IPOD_PlayControl() PlayPause Failed(0x%x)",
                                                                                        __FUNCTION__,
                                                                                        eCMDErrorCode
                                                                                        );
                }
            }else if(IPOD_DR_END_FAST_FORWARD_OR_REWIND == playStatus){
                IAPSVC_DBUG("%s() wait end FF or Rew",__FUNCTION__);
                SetSvcWantedNextAction(IPOD_DR_PLAYBACK_PAUSED);
                IsSuccess = true;
            }else{
                IsSuccess = true;
            }
        }
        else{
            ALOGE(" %s  > IPOD_GetPlayStatus() Failed(0x%x)",__FUNCTION__, eCMDErrorCode);
        }
    }	
    else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    IAPSVC_DBUG("%d: %s()  end", __LINE__, __FUNCTION__);
    reply->writeInt32(IsSuccess);
    return IsSuccess;
}

bool IapService::ProcPause(Parcel *reply)
{
    bool IsSuccess = false;
    IAPSVC_DEFLOG("%d: %s() start IsFFREWMode(%d)", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        GetSvcLocalFFRewStatus()
                                                                        );
    IsSuccess = ProcDoPause(reply,1,IAP_CMD_PAUSE);
    return IsSuccess;
}

bool IapService::Pause(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_PAUSE,data,NULL,true);
    else{
        Parcel reply;
        ret = ProcPause(&reply);
    }
    return ret;    
}

bool IapService::ProcNext(Parcel *reply)
{
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    return NextPreviousControl(NEXT,reply);
}

bool IapService::Next(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_NEXT,data,NULL,true);
    else{
        Parcel reply;
        ret = ProcNext(&reply);
    }
    return ret;        
}

bool IapService::ProcPrevious(Parcel *reply)
{
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    return NextPreviousControl(PREVIOUS,reply);
}

bool IapService::Previous(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_PREVIOUS,data,NULL,true);
    else{
        Parcel reply;
        ret = ProcPrevious(&reply);
    }
    return ret;    
}

bool IapService::NextPreviousControl(int type,Parcel *reply)
{
    bool IsSuccess = false;
    bool ret = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    PlayCmdCode cmdCode = (PlayCmdCode)type;

    if ( IsInitialized() ){
        IsSuccess = DoEndFfRew();

        IAPSVC_DEFLOG("%d: %s() type %d ", __LINE__, __FUNCTION__,type);

        eCMDErrorCode = IPOD_PlayControl( GetIPodHandle(), (PlayCmdCode)type);
        if ( IS_CMDOK(eCMDErrorCode) ){
            IPOD_DR_PLAY_STATUS playStatus;
            
            if( Sample_GetDRPlayStatus((void *)&playStatus)){
                if(GetSvcLocalPlayStatus() == IPOD_DR_END_FAST_FORWARD_OR_REWIND){
                        IAPSVC_DEFLOG("%s() wait end FF or Rew",__FUNCTION__);
                        SetSvcWantedNextAction(IPOD_DR_PLAYING);
                        IsSuccess = true;
                }else{
                    if(playStatus != IPOD_DR_PLAYING){
                        eCMDErrorCode = IPOD_PlayControl( GetIPodHandle(), PLAYPAUSE);
                        if ( IS_CMDOK(eCMDErrorCode) ){
                            IsSuccess = true;
                            SetSvcLocalPlayStatus(IPOD_DR_PLAYING);
                            //IPOD_Sample_SetDRPlayStatus(GetIPodHandle(), IPOD_DR_PLAYING);
                        }else{
                            ALOGE(" %s  > IPOD_PlayControl() PlayPause Failed(0x%x)",
                                                                                                __FUNCTION__,
                                                                                                eCMDErrorCode
                                                                                                );
                        }
                    }else if(playStatus == IPOD_DR_PLAYING){
                        IsSuccess = true;
                    }
                }
            }else{
            }
        }
        else{
            ALOGE(" %s > IPOD_PlayControl() NextPrevious type %d Failed %d",__FUNCTION__,type,eCMDErrorCode);
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    reply->writeInt32(IsSuccess);
    return IsSuccess;
}

bool IapService::ProcNextTrack(Parcel *reply)
{
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    return NextPreviousControl(NEXTTRACK,reply);
}

bool IapService::NextTrack(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_NEXT_TRACK,data,NULL,true);
    else{
        Parcel reply;
        ret = ProcNextTrack(&reply);
    }
    return ret;        
}

bool IapService::ProcPreviousTrack(Parcel *reply)
{
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    return NextPreviousControl(PREVIOUSTRACK,reply);
}

bool IapService::PreviousTrack(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_PREVIOUS_TRACK,data,NULL,true);
    else{
        Parcel reply;
        ret = ProcPreviousTrack(&reply);
    }
    return ret;    
}


bool IapService::FfRewControl(int type,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    int ret = 0;
    if ( IsInitialized() ) {
        IPOD_DR_PLAY_STATUS playStatus;
        IsSuccess = DoEndFfRew();

        //if(Sample_GetDRPlayStatus( (void *)&playStatus))
        {
            //if (IPOD_DR_PLAYING == playStatus)
            {
                ret = IPOD_PlayControl( GetIPodHandle(), (PlayCmdCode)type );
                if ( IS_CMDOK(ret) ){
                    if(type == STARTFF)
                        SetSvcLocalPlayStatus(IPOD_DR_FAST_FORWARD);
                    else
                        SetSvcLocalPlayStatus(IPOD_DR_FAST_REWIND);
                    SetSvcLocalFFRewStatus(true);
                    IsSuccess = true;
                }else{
                    ALOGE(" %s  > IPOD_PlayControl() FfRew Type %d Failed(0x%x)",
                                                                                        __FUNCTION__,
                                                                                        type,
                                                                                        eCMDErrorCode
                                                                                        );
                }
            }
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    reply->writeInt32(IsSuccess);
    return IsSuccess;
}

bool IapService::ProcFF(Parcel *reply)
{
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    return FfRewControl(STARTFF,reply);
}

bool IapService::FF(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_FF,data,NULL,true);
    else{
        Parcel reply;
        ret = ProcFF(&reply);
    }
    return ret;    
}

bool IapService::ProcRew(Parcel *reply)
{
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    return FfRewControl(STARTREW,reply);
}

bool IapService::Rew(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_REW,data,NULL,true);
    else{
        Parcel reply;
        ret = ProcRew(&reply);
    }
    return ret;    
}

bool IapService::DoEndFfRew()
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    if ( IsInitialized() ) {
        if ( GetSvcLocalFFRewStatus() ){
            IAPSVC_DEFLOG("%s() send ENDFFREW", __FUNCTION__);
            eCMDErrorCode = IPOD_PlayControl( GetIPodHandle(), ENDFFREW );
            if ( IS_CMDOK(eCMDErrorCode) ){
                IsSuccess = true;
                SetSvcLocalFFRewStatus(false);
                SetSvcLocalPlayStatus(IPOD_DR_END_FAST_FORWARD_OR_REWIND);
            }else{
                ALOGE(" %s  > IPOD_PlayControl() ENDFFREW Failed(0x%x)",
                                                                                    __FUNCTION__,
                                                                                    eCMDErrorCode
                                                                                    );
            }
            IAPSVC_DBUG("%d: %s() IsFFREWMode(%d)", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        GetSvcLocalFFRewStatus()
                                                                        );
    	}
    }
    return IsSuccess;
}

bool IapService::ProcEndFfRew(Parcel *reply)
{
    IAPSVC_DEFLOG("%d: %s() statue %d", __LINE__, __FUNCTION__,GetSvcLocalFFRewStatus());
    bool IsSuccess = false;

    if ( IsInitialized() ) {
        IsSuccess = DoEndFfRew();
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    reply->writeInt32(IsSuccess);
    return IsSuccess;
}

bool IapService::EndFfRew(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_ENDFFREW,data,NULL,true);
    else{
        Parcel reply;
        ret = ProcEndFfRew(&reply);
    }
    return ret;    
}

bool IapService::ProcGetCurrentTrackIndex(Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    long nCurrentIndex = -1;

    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {

        eCMDErrorCode = (IPOD_ERROR)UtilGetCurrentTrackIndex(&nCurrentIndex);
        if ( IS_CMDOK(eCMDErrorCode) ){
            IsSuccess = true;
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    IAPSVC_DEFLOG("%d: %s()  nCurrentIndex = %ld", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        nCurrentIndex
                                                                        );
    reply->writeInt32(IsSuccess);
    reply->writeInt64(nCurrentIndex);
    return IsSuccess;
}

bool IapService::GetCurrentTrackIndex(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(
                                                                    IAP_CMD_GET_CURRENT_TRACKINDEX,
                                                                    data,
                                                                    reply,
                                                                    true
                                                                    );
    else
        ret = ProcGetCurrentTrackIndex(reply);
    return ret;    
}

bool IapService::ProcGetTotalPlayTracks(Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    long nTotalTrackNum = 0;

    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);

    if ( IsInitialized() ) {

        eCMDErrorCode = IPOD_GetNumPlayingTracks( 
                                                                            GetIPodHandle(),
                                                                            &nTotalTrackNum 
                                                                            );
        if ( IS_CMDOK(eCMDErrorCode) ){
            IsSuccess = true;
        }
        else{
            ALOGE(" %s IPOD_GetNumPlayingTracks() > Failed(0x%x).",
                                                                            __FUNCTION__,
                                                                            eCMDErrorCode
                                                                            );
        }

    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    IAPSVC_DEFLOG("%d: %s() eCMDErrorCode 0x%x", __LINE__, __FUNCTION__,eCMDErrorCode);
    reply->writeInt32(IsSuccess);
    reply->writeInt64(nTotalTrackNum);
    return IsSuccess;
}

bool IapService::GetTotalPlayTracks(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GET_TOTAL_TRACK,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetTotalPlayTracks(reply);
    return ret;    
}

bool IapService::ProcGetTitle(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    long nCurrentIndex = 0;
    String8 result = String8("");
    long nIndex = data->readInt64();
    IAPSVC_DBUG("%d: %s() nIndex(%ld)", __LINE__, __FUNCTION__,nIndex);

    if ( IsInitialized() ) {
        long nStringLen = 0;
        unsigned char strName[UI_MAX_STRING_LEN] = {0,};

        if ( Util_IsInvalidIndex(nIndex) ){
            eCMDErrorCode = (IPOD_ERROR)UtilGetCurrentTrackIndex(&nCurrentIndex);
            if ( IS_CMDOK(eCMDErrorCode) ){
                nIndex = (long)nCurrentIndex;
            }
        }

        if(nIndex >= 0){
            if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED)
                eCMDErrorCode = IPOD_GetIndexedPlayingTrackTitle(
                                                                                GetIPodHandle(),
                                                                                nIndex,
                                                                                &strName[0],
                                                                                UI_MAX_STRING_LEN,
                                                                                &nStringLen
                                                                                );
            else
                eCMDErrorCode = IPOD_DISCONNECT_ERROR;
            
            if ( IS_CMDOK(eCMDErrorCode) ){
                IsSuccess = true;
                result = String8((const char*)strName);
                IAPSVC_DEFLOG("%d: %s() index[%ld]  str [%s] ", 
                                                                                    __LINE__, 
                                                                                    __FUNCTION__,
                                                                                    nIndex,
                                                                                    result.string()
                                                                                    );
            }
            else{
                ALOGE(" %s IPOD_GetIndexedPlayingTrackTitle() > Failed(0x%x). nIndex(%ld) nIndex(%ld)",
                                                                                    __FUNCTION__,
                                                                                    eCMDErrorCode,
                                                                                    nIndex,nIndex
                                                                                    );
            }
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    reply->writeInt32(IsSuccess);

    if(IsSuccess == true){
        reply->writeInt64(nIndex);
        reply->writeString16(String16(result));    
    }
    return result;
}

bool IapService::GetTitle(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_GET_TITLE,data,reply,true);
    else
        ret = ProcGetTitle(data,reply);
    return ret;    
}
    
bool IapService::ProcGetArtist(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    long nCurrentIndex = 0;
    long nIndex = data->readInt64();
    String8 result = String8("");
    IAPSVC_DBUG("%d: %s() nIndex(%ld) ", __LINE__, __FUNCTION__,nIndex);

    if ( IsInitialized() ) {
        long nStringLen = 0;
        unsigned char strName[UI_MAX_STRING_LEN] = {0,};

        if ( Util_IsInvalidIndex(nIndex) ){
             eCMDErrorCode = (IPOD_ERROR)UtilGetCurrentTrackIndex(&nCurrentIndex);
            if ( IS_CMDOK(eCMDErrorCode) ){
                nIndex = (int)nCurrentIndex;
            }
        }
        if(nIndex >= 0){
            if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED)
                eCMDErrorCode = IPOD_GetIndexedPlayingTrackArtistName(
                                                                                            GetIPodHandle(),
                                                                                            nIndex,
                                                                                            &strName[0],
                                                                                            UI_MAX_STRING_LEN,
                                                                                            &nStringLen 
                                                                                            );
            else
                eCMDErrorCode = IPOD_DISCONNECT_ERROR;
            
            if ( IS_CMDOK(eCMDErrorCode) ){
                IsSuccess = true;
                result = String8((const char*)strName);
                IAPSVC_DEFLOG("%d: %s() index[%ld]  str [%s] ", 
                                                                            __LINE__,
                                                                            __FUNCTION__,
                                                                            nIndex,
                                                                            result.string()
                                                                            );
            }
            else{
                ALOGE(" %s IPOD_GetIndexedPlayingTrackArtistName() > Failed(0x%x).nIndex(%ld) nIndex(%ld)",
                                                                                        __FUNCTION__,
                                                                                        eCMDErrorCode,
                                                                                        nIndex,
                                                                                        nIndex
                                                                                        );
            }
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }   
    reply->writeInt32(IsSuccess);

    if(IsSuccess == true){
        reply->writeInt64(nIndex);
        reply->writeString16(String16(result));    
    }
    return IsSuccess;
}

bool IapService::GetArtist(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GET_ARTIST,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetArtist(data,reply);
    return ret;    
}

bool IapService::ProcGetAlbum(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    long nCurrentIndex = 0;
    long nIndex = data->readInt64();
    String8 result = String8("");
    IAPSVC_DBUG("%d: %s() nIndex(%ld)", __LINE__, __FUNCTION__,nIndex);

    if ( IsInitialized() ) {
        long nStringLen = 0;
        unsigned char strName[UI_MAX_STRING_LEN] = {0,};

        if ( Util_IsInvalidIndex(nIndex) ){
             eCMDErrorCode = (IPOD_ERROR)UtilGetCurrentTrackIndex(&nCurrentIndex);
            if ( IS_CMDOK(eCMDErrorCode) ){
                nIndex = (long)nCurrentIndex;
            }
        }

        if(nIndex >= 0){
            if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED)
                eCMDErrorCode = IPOD_GetIndexedPlayingTrackAblumName(
                                                                                            GetIPodHandle(),
                                                                                            nIndex,
                                                                                            &strName[0],
                                                                                            UI_MAX_STRING_LEN,
                                                                                            &nStringLen 
                                                                                            );
            else
                eCMDErrorCode = IPOD_DISCONNECT_ERROR;

            if ( IS_CMDOK(eCMDErrorCode) ){
                IsSuccess = true;
                IAPSVC_DBUG("%d: %s() nStringLen(%ld)", 
                                                                                __LINE__, 
                                                                                __FUNCTION__,
                                                                                nStringLen
                                                                                );
                if(nStringLen <= DUMMY_LEN){
                    result = String8("Unknown Album");
                }else{
                    result = String8((const char*)strName);
                }
                IAPSVC_DEFLOG("%d: %s() index[%ld]  str [%s] ", 
                                                                                    __LINE__, 
                                                                                    __FUNCTION__,
                                                                                    nIndex,
                                                                                    result.string()
                                                                                    );
            }
            else{
                ALOGE(" %s IPOD_GetIndexedPlayingTrackAblumName() > Failed(0x%x). nIndex(%ld) nIndex(%ld)",
                                                                                    __FUNCTION__,
                                                                                    eCMDErrorCode,
                                                                                    nIndex,
                                                                                    nIndex
                                                                                    );
            }
        }
    }  else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }  
    reply->writeInt32(IsSuccess);

    if(IsSuccess == true){
        reply->writeInt64(nIndex);
        reply->writeString16(String16(result));    
    }  
    return IsSuccess;
}

bool IapService::GetAlbum(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s()", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GET_ALBUM,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetAlbum(data,reply);
    return ret;    
}

bool IapService::ProcGetGenre(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    long nCurrentIndex = 0;
    long nIndex = data->readInt64();
    String8 result = String8("");
    IAPSVC_DBUG("%d: %s() nIndex(%ld)", __LINE__, __FUNCTION__,nIndex);
    if ( IsInitialized() ) {
        long nStringLen = 0;
        unsigned char strName[UI_MAX_STRING_LEN] = {0,};

        if ( Util_IsInvalidIndex(nIndex) ){
             eCMDErrorCode = (IPOD_ERROR)UtilGetCurrentTrackIndex(&nCurrentIndex);
            if ( IS_CMDOK(eCMDErrorCode) ){
                nIndex = nCurrentIndex;
            }
        }

        if(nIndex >= 0){
            eCMDErrorCode = IPOD_GetGenre(
                                                                    GetIPodHandle(),
                                                                    nIndex,
                                                                    &strName[0],
                                                                    UI_MAX_STRING_LEN,
                                                                    &nStringLen 
                                                                    );
            if ( IS_CMDOK(eCMDErrorCode) ){
                IsSuccess = true;
                result = String8((const char*)strName);
                IAPSVC_DEFLOG("%d: %s() index[%ld]  str [%s] ", 
                                                                                __LINE__,
                                                                                __FUNCTION__,
                                                                                nIndex,
                                                                                result.string()
                                                                                );
            }
            else{
                ALOGE(" %s IPOD_GetGenre() > Failed(0x%x). nIndex(%ld) nIndex(%ld)",
                                                                                __FUNCTION__,
                                                                                eCMDErrorCode,
                                                                                nIndex,
                                                                                nIndex
                                                                                );
            }
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    reply->writeInt32(IsSuccess);

    if(IsSuccess == true){
        reply->writeInt64(nIndex);
        reply->writeString16(String16(result));    
    }
    return IsSuccess;
}

bool IapService::GetGenre(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_GET_GENRE,data,reply,true);
    else
        ret = ProcGetGenre(data,reply);
    return ret;    
}

bool IapService::ProcCheckAlbumArtFormats(void)
{
    bool ret = false;
    IPOD_ERROR Err;
    long ActualLen;
    long FrameID;
    ArtworkFormat pformat[IPOD_MAX_ARTWORK];

    Err = IPOD_DR_GetArtworkFormats(GetIPodHandle(), &pformat[0], IPOD_MAX_ARTWORK, &ActualLen);

    IAPSVC_DBUG("%d: %s() Err(0x%x) ActualLen(%ld)", __LINE__, __FUNCTION__,Err,ActualLen);
    if (IPOD_NO_ERROR == Err)
    {
        for (FrameID=0; FrameID<ActualLen; FrameID++)
        {
            IAPSVC_DBUG(" format %d id(%d) w(%d) h(%d)", 
                                                                pformat[FrameID].PixelFormat,
                                                                pformat[FrameID].FormatID,
                                                                pformat[FrameID].ImageWidth,
                                                                pformat[FrameID].ImageHeight
                                                                );
        }
        ret = true;
    }

    return ret;
}

bool IapService::ProcGetAlbumArtworkParams(long index,void * pParam)
{
    bool ret = false;
    int result = 0;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    pIPOD_DR_TRACK_INFO_PARAM pTrackParam = (pIPOD_DR_TRACK_INFO_PARAM)pParam;
    pTrackParam->m_TrackIndex = index;
    pTrackParam->m_ChapterIndex = 0;
    pTrackParam->m_type = IPOD_DR_TRACK_INFO_CAPS;
    eCMDErrorCode = IPOD_DR_GetIndexedPlayingTrackInfo(GetIPodHandle(), pTrackParam);

    if(eCMDErrorCode == IPOD_NO_ERROR){
        uint32_t bitMaskArtwork = (1<<2);
        IAPSVC_DBUG("%d: %s() m_TrackCaps(0x%lx)", __LINE__, __FUNCTION__,
                                        pTrackParam->m_TrackInfo.m_capInfo.m_TrackCaps);
        if((pTrackParam->m_TrackInfo.m_capInfo.m_TrackCaps & bitMaskArtwork) == bitMaskArtwork){
            pTrackParam->m_type = IPOD_DR_TRACK_INFO_ARTWORK_CNT;
            eCMDErrorCode = IPOD_DR_GetIndexedPlayingTrackInfo(GetIPodHandle(), pTrackParam);

            if(eCMDErrorCode == IPOD_NO_ERROR){
                
                IAPSVC_DBUG("%d: %s() artwork cound(%ld)", __LINE__, __FUNCTION__,
                                                pTrackParam->m_TrackInfo.m_Artwork.m_ActualCnt);
                if(pTrackParam->m_TrackInfo.m_Artwork.m_ActualCnt > 0){
                    ret= true;
                }
            }
        }
    }
    return ret;
}

bool IapService::ProcNewGetAlbumArtwork(long index,int formatId,int timeOffset,int imageSize,Parcel *reply)
{
    uint8_t *pRetPayload = NULL;
    bool IsSuccess = false;
    int RcvCnt;
    uint8_t *retPayload = NULL;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    unsigned char *buff = IPOD_GetTxBuffer(GetIPodHandle()); /*Please use this function to get a Txbuffer */
    int retPayloadLen = 0;
    int rxBuffSize = 32;
    unsigned char *buffRx = (unsigned char *)malloc(rxBuffSize);
    unsigned char *buffImage = NULL;//(unsigned char *)malloc(imageSize);
    int totalSize = 19 + imageSize;//240*240*2
    int packetCnt = 0;
    int packetSize = 1014;
    int fullPacketCnt = 0;
    uint8_t lingo = LINGO_DISPLAY_REMOTE;
    uint16_t command = 0x18;
    uint16_t retCommand = 0x19;
    int pos = 0;
    int receiveCnt = 0;
    uint8_t errCode = 0;
    int packetIndex = 0;
    int pixelFormat = 0;
    int width = 0;
    int height = 0;
    int topLeftX = 0;
    int topLeftY = 0;
    int bottomRightX = 0;
    int bottomRightY = 0;
    int rawSize = 0;
    int realTotalSize = 0;
    int imageDataOffset = 0;
    int remainSize = imageSize;
    
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    eCMDErrorCode = IPOD_USER_EnterCriticalSection(GetIPodHandle());
    
    // 115219 - 1014 = 114205 // 1
    // 114205/1012 = 112.85... // 112
    // 114205 - (1012*112) = 114205 - 113344 = 861 // 1
    
    if(totalSize > packetSize)
        packetCnt++;
    totalSize -= packetSize;
    fullPacketCnt = totalSize/ (packetSize - 2);
    packetCnt += fullPacketCnt;
    if((totalSize - (fullPacketCnt * (packetSize - 2))) > 0)
        packetCnt++;

    receiveCnt = packetCnt;

    IAPSVC_DBUG("%d: %s() predicted packet count %d", __LINE__, __FUNCTION__,receiveCnt);
    
    if (IPOD_NO_ERROR == eCMDErrorCode){
        IPOD_CMD_PARAM CmdParam;
        int cnt = 0;
        buff[cnt++] = (unsigned char)lingo; 
        if(lingo != LINGO_EXTENDED_INTERFACE){
            buff[cnt++] = (unsigned char)command;
        }else{
            buff[cnt++] = 0;
            buff[cnt++] = (unsigned char)command;
        }

        cnt += PutInt(&buff[cnt], index);
        cnt += PutShort(&buff[cnt], formatId);
        cnt += PutInt(&buff[cnt], timeOffset);
        
        CmdParam.m_iPod_Handle = GetIPodHandle();
        CmdParam.m_CurCmd = command;
        CmdParam.m_SendCmdBuf = buff;
        CmdParam.m_SendCmdBufSize = cnt; /*buffer size of User added*/
        CmdParam.m_TimeOut = IPOD_Callback_GetWaitTime();
        CmdParam.m_ResponseBuf = buffRx;
        CmdParam.m_ResponseBufSize = rxBuffSize;

        if(receiveCnt>1){
            CmdParam.m_Flag = IPOD_MULTI_RESPONSE;
        } else{
            CmdParam.m_Flag = IPOD_DATA_SINGLE_SESSION;
        }
        for(RcvCnt=0;remainSize > 0 ;RcvCnt++){
            if(RcvCnt!=0){
                CmdParam.m_SendFunc = IPOD_SEND_SKIP;
                CmdParam.m_SendCmdBufSize = 0;
                CmdParam.m_TimeOut = IPOD_Callback_GetWaitTime();
            }else{
                CmdParam.m_SendFunc = IPOD_SEND_ADD_HEADER;
            }
            eCMDErrorCode = IPOD_USER_FUNC_SendCmd_RcvResponse(
                                                                                                    &CmdParam, 
                                                                                                    lingo, 
                                                                                                    retCommand
                                                                                                    ); 

            retPayloadLen = CmdParam.m_ResponseBufSize;
            if(eCMDErrorCode == IPOD_COMMAND_ACCEPTED){
                int pos = 0;
                int imageData = 0;
                if(RcvCnt!=0){
                    pos += GetShort(&CmdParam.m_ResponseBuf[pos],&packetIndex);
                    imageData = (retPayloadLen - pos);
                    if((imageDataOffset + imageData) <= realTotalSize){
                        memcpy(&buffImage[imageDataOffset],&CmdParam.m_ResponseBuf[pos],imageData);
                    }else{
                        ALOGE("%d: %s() invalid size",__LINE__, 
                                                                        __FUNCTION__
                                                                        );
                        break;
                    }
                    imageDataOffset += imageData;
                    remainSize -= imageData;
                    if(remainSize == 0){
                        IAPSVC_DBUG("%d: %s() receive all data",__LINE__, 
                                                                        __FUNCTION__
                                                                        );
                        IsSuccess = true;
                    }
                    IAPSVC_DDBUG("%d: %s() imageDataOffset(%d) rcvSize(%d) remainSize(%d)",__LINE__, 
                                                                        __FUNCTION__,
                                                                        imageDataOffset,
                                                                        imageData,
                                                                        remainSize
                                                                        );
                }else{
                    pos += GetShort(&CmdParam.m_ResponseBuf[pos],&packetIndex);
                    pixelFormat = CmdParam.m_ResponseBuf[pos++];
                    pos += GetShort(&CmdParam.m_ResponseBuf[pos],&width);
                    pos += GetShort(&CmdParam.m_ResponseBuf[pos],&height);
                    pos += GetShort(&CmdParam.m_ResponseBuf[pos],&topLeftX);
                    pos += GetShort(&CmdParam.m_ResponseBuf[pos],&topLeftY);
                    pos += GetShort(&CmdParam.m_ResponseBuf[pos],&bottomRightX);
                    pos += GetShort(&CmdParam.m_ResponseBuf[pos],&bottomRightY);
                    pos += GetInt(&CmdParam.m_ResponseBuf[pos], &rawSize);
                    realTotalSize = height * rawSize;
                    remainSize = realTotalSize;
                    buffImage = (unsigned char *)malloc(realTotalSize);
                    if(buffImage == NULL){
                        ALOGE("%d: %s() can not alloc memory realTotalSize %d",__LINE__, 
                                                                        __FUNCTION__,realTotalSize
                                                                        );
                        break;
                    }
                    imageData = (retPayloadLen - pos);
                    memcpy(&buffImage[imageDataOffset],&CmdParam.m_ResponseBuf[pos],imageData);
                    imageDataOffset += imageData;
                    remainSize -= imageData;
                    IAPSVC_DDBUG("%d: %s() imageDataSize(%d) rcvSize(%d) remainSize(%d)",__LINE__, 
                                                                        __FUNCTION__,
                                                                        realTotalSize,
                                                                        imageData,
                                                                        remainSize
                                                                        );

                }
            }else{
                uint8_t errCode = 0;
                uint16_t checkID = 0;    
                //reply->writeInt32(IsSuccess);
                if(retPayloadLen >= 3){
                    errCode = CmdParam.m_ResponseBuf[0];
                    checkID = (CmdParam.m_ResponseBuf[1]<<8) | CmdParam.m_ResponseBuf[2];
                    IAPSVC_DBUG("%d: %s() errCode [0x%x] Id[0x%x] ", 
                                                                        __LINE__, 
                                                                        __FUNCTION__,
                                                                        errCode,
                                                                        checkID
                                                                        );
                }
                IAPSVC_DBUG("%d: %s() eCMDErrorCode [0x%x] ",__LINE__, 
                                                                        __FUNCTION__,
                                                                        eCMDErrorCode
                                                                        );
                break;
            }
        } 

        CmdParam.m_SendCmdBufSize = 0;
        CmdParam.m_Flag = IPOD_MULTI_RESPONSE_END;
        CmdParam.m_SendFunc = IPOD_SEND_ADD_HEADER;
        eCMDErrorCode = IPOD_USER_FUNC_SendCmd_RcvResponse(&CmdParam, 0, 0);

        IAPSVC_DBUG("%d: %s() IsSuccess(%d) rcvCnt(%d) realTotalSize [%d] remainSize[%d] eCMDErrorCode 0x%x",__LINE__, 
                                                                        __FUNCTION__,
                                                                        IsSuccess,
                                                                        RcvCnt,
                                                                        realTotalSize,
                                                                        remainSize,
                                                                        eCMDErrorCode
                                                                        );

        if(IsSuccess == true){
            reply->writeInt32(true);
            reply->writeInt64(index);
            reply->writeInt32(width);
            reply->writeInt32(height);
            reply->writeInt32(realTotalSize);
            reply->writeInt32(realTotalSize);
            reply->write(buffImage,realTotalSize);
        }else{
            reply->writeInt32(false);
            reply->writeInt64(index);
            reply->writeInt32(0);
            reply->writeInt32(0);
            reply->writeInt32(0);
        }
    }

    if(buffRx != NULL)
        free(buffRx);

    if(buffImage != NULL)
        free(buffImage);
    
    IPOD_USER_ExitCriticalSection(GetIPodHandle());
    return IsSuccess;

}

bool IapService::ProcSearchAlbumArtworkFormat(void *pParam,int *formatId,int *artworkIndex,int *imageSize)
{
    bool ret = false;
    int i,j;
    pIPOD_PLAY_INFO pPlayInfo = IPOD_Sample_Get_AudioPlayInfo(GetIPodHandle());
    pIPOD_ARTWORK_INFO pArtworkInfo = &pPlayInfo->ArtworkInfo;
    pIPOD_DR_TRACK_INFO_PARAM  pTrackParam= (pIPOD_DR_TRACK_INFO_PARAM)pParam;
    pIPOD_DR_TRACK_ARTWORK pArtworkList = &pTrackParam->m_TrackInfo.m_Artwork;
    unsigned long displaysize, MaxDisplaysize;
    unsigned long currentDisplaysize = 0;
    int index = -1;
    int artworkIdx = 0;
    int size = 0;
    MaxDisplaysize = pArtworkInfo->MaxDisplaySize;
    IAPSVC_DBUG("%d: %s() BasicFormatID[0x%lx] MaxDisplaysize %ld", 
                            __LINE__, __FUNCTION__,pArtworkInfo->BasicFormatID,MaxDisplaysize);
    for(i = 0;i < (int)pArtworkInfo->MaxFormatCount ;i++){
        // if formatId is valid and is not a basic format
        IAPSVC_DDBUG("%d: %s() formatId[0x%lx] size %ld", 
                                        __LINE__, __FUNCTION__,
                                        pArtworkInfo->CapableFormatID[i],
                                        pArtworkInfo->CapableSize[i]
                                        );
        if(pArtworkInfo->CapableFormatID[i] != (unsigned long)(-1))// && pArtworkInfo->CapableFormatID[i] != pArtworkInfo->BasicFormatID )
        {
            if(pArtworkInfo->CapableSize[i] > currentDisplaysize ){
                // If there is a album artwork
                for(j = 0; j < (int)pArtworkList->m_ActualCnt ;j++){
                    if((unsigned long)pArtworkList->m_ArtworkCnt[j].m_FormatID == pArtworkInfo->CapableFormatID[i]){
                        if(pArtworkList->m_ArtworkCnt[j].m_ArtworkCnt > 0){
                            currentDisplaysize = pArtworkInfo->CapableSize[i];
                            index = i;
                            artworkIdx = j;
                            size = pArtworkInfo->CapableSize[i];
                            IAPSVC_DDBUG("%d: %s() index[%d] artworkIdx[%d] size %d", 
                                        __LINE__, __FUNCTION__,
                                        index,
                                        artworkIdx,
                                        size
                                        );
                        }
                    }
                }     
            }            
        }
    }

    if(index >= 0){
        *formatId = (int)pArtworkInfo->CapableFormatID[index];
        *artworkIndex = artworkIdx;
        *imageSize = size;
        IAPSVC_DBUG("%d: %s() final index[%d] artworkIdx[%d] size %d", 
                                        __LINE__, __FUNCTION__,
                                        index,
                                        artworkIdx,
                                        size
                                        );
        ret = true;
    }
    return ret;
}

bool IapService::ProcGetArtworkTimes(long index,int formatId,int artworkIndex,int *times)
{
    bool IsSuccess = false;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    int retPayloadLen = 0;
    uint8_t *pRetPayload = NULL;
    uint8_t lingo = LINGO_DISPLAY_REMOTE;
    uint16_t command = 0x1F;
    uint16_t retCommand = 0x20;
    int payloadLen = 10;
    uint8_t payload[10];
    uint8_t errCode = 0;
    int pos = 0;
    int retTimes = 0;
    IAPSVC_DEFLOG("%d: %s() final index[%ld] formatId[%d] artworkIdx[%d]", 
                                        __LINE__, __FUNCTION__,
                                        index,
                                        formatId,
                                        artworkIndex
                                        );
    
    pos += PutInt(&payload[pos], (int)index);
    pos += PutShort(&payload[pos], formatId);
    pos += PutShort(&payload[pos], 0);
    pos += PutShort(&payload[pos], 1);
    //Dump((const char *)payload, payloadLen);
    
    pRetPayload = SendPacketedCmd(lingo, 
                                                    command, 
                                                    retCommand, 
                                                    payload, 
                                                    payloadLen, 
                                                    (void *)&eCMDErrorCode, 
                                                    &retPayloadLen);
    IAPSVC_DBUG("%d: %s() errCode [0x%x] ", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                eCMDErrorCode
                                                                );
        
    if(eCMDErrorCode == IPOD_COMMAND_ACCEPTED){
        if(retPayloadLen >= 4){
            IsSuccess = true;
            pos = 0;
            pos += GetInt(&pRetPayload[pos], &retTimes);
            *times = retTimes;
        }
    }else{
        uint8_t errCode = 0;
        uint16_t checkID = 0;    
        if(retPayloadLen >= 3){
            errCode = pRetPayload[0];
            checkID = (pRetPayload[1]<<8) | pRetPayload[2];
            IAPSVC_DBUG("%d: %s() errCode [0x%x] Id[0x%x] ", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                errCode,
                                                                checkID
                                                                );
        }
        IAPSVC_DBUG("%d: %s() eCMDErrorCode [0x%x] ",__LINE__, 
                                                                __FUNCTION__,
                                                                eCMDErrorCode
                                                                );
    }


    if(pRetPayload){
        free(pRetPayload);
    }
    return IsSuccess;
}

bool IapService::ProcCheckAlbumArtwork(long index,Parcel *reply)
{
    bool ret = false;
    int listCount = IPOD_MAX_ARTWORK;
    int formatId = 0;
    int artworkIndex = 0;
    int artworkTimes = 0;
    int imageSize = 0;
    IPOD_DR_TRACK_INFO_PARAM TrackParam;
            
    ret = ProcGetAlbumArtworkParams(index,&TrackParam);
    if(ret == true){
        ret = ProcSearchAlbumArtworkFormat(&TrackParam, &formatId,&artworkIndex,&imageSize);
        IAPSVC_DBUG("%d: %s() formatId[%d] artworkIndex[%d] size %d", 
                                __LINE__, __FUNCTION__,
                                formatId,
                                artworkIndex,
                                imageSize
                                );
        if(ret == true){
            ret = ProcGetArtworkTimes(index, formatId, artworkIndex,&artworkTimes);
            if(ret == true){
                ret = ProcNewGetAlbumArtwork(index, formatId, artworkTimes, imageSize,reply);
            }
        }
    }

    return ret;
}

bool IapService::ProcGetSmallArtworkFormatId(long index,unsigned long *pNewFormatId)
{
    bool ret = false;
    int listCount = IPOD_MAX_ARTWORK;
    int formatId = 0;
    int artworkIndex = 0;
    int artworkTimes = 0;
    int imageSize = 0;
    IPOD_DR_TRACK_INFO_PARAM TrackParam;
            
    ret = ProcGetAlbumArtworkParams(index,&TrackParam);
    if(ret == true){
        ret = ProcSearchAlbumArtworkFormat(&TrackParam, &formatId,&artworkIndex,&imageSize);
        IAPSVC_DBUG("%d: %s() formatId[%d] artworkIndex[%d] size %d", 
                                __LINE__, __FUNCTION__,
                                formatId,
                                artworkIndex,
                                imageSize
                                );
        if(ret == true){
            *pNewFormatId = (unsigned long)formatId;
            IAPSVC_DBUG("%d: %s() formatId[0x%x]", 
                            __LINE__, __FUNCTION__,
                            formatId
                            );
        }
    }
    return ret;
}

bool IapService::ProcGetSmallArtwork(
                                                                    long nIndex,
                                                                    int bufSize,
                                                                    void *pData,
                                                                    Parcel *reply
                                                                    )
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    unsigned long newFormatId = 0;
    unsigned long originFormatId = 0;
    unsigned long nActualLen;
    pArtworkData pArtData = (pArtworkData)pData;
    pIPOD_PLAY_INFO pPlayInfo = IPOD_Sample_Get_AudioPlayInfo(GetIPodHandle());
    pIPOD_ARTWORK_INFO pArtworkInfo = &pPlayInfo->ArtworkInfo;
    originFormatId = pArtworkInfo->BasicFormatID;

    IAPSVC_DBUG("%d: %s() nIndex %ld ",__LINE__, __FUNCTION__,nIndex);
    
    if(ProcGetSmallArtworkFormatId(nIndex, &newFormatId)){
        IAPSVC_DBUG("%d: %s() originFormatId[0x%lx] newFormatId[0x%lx]", 
                                __LINE__, __FUNCTION__,
                                originFormatId,
                                newFormatId
                                );
        pArtworkInfo->BasicFormatID = newFormatId;
        eCMDErrorCode = IPOD_DR_GetAlbumArt( 
                                                                        GetIPodHandle(),
                                                                        nIndex,
                                                                        pArtData,
                                                                        bufSize,
                                                                        &nActualLen
                                                                        );
        IAPSVC_DBUG("%d: %s() eCMDErrorCode(0x%x) nActualLen %ld",
                                                                                                __LINE__,
                                                                                                __FUNCTION__,
                                                                                                eCMDErrorCode,
                                                                                                nActualLen
                                                                                                );
        pArtworkInfo->BasicFormatID = originFormatId;
        switch (eCMDErrorCode) {
            case 17:	// test
            case IPOD_NO_ERROR:
                IsSuccess = true;
                reply->writeInt32(true);
                reply->writeInt64(nIndex);
                reply->writeInt32((int)pArtData->sWidth);
                reply->writeInt32((int)pArtData->sHeight);
                reply->writeInt32((int)nActualLen);
                reply->writeInt32((int)nActualLen);
                reply->write(pArtData->pImageData,nActualLen);
                IAPSVC_DBUG("%d: %s() [ALBUMART] pixelformat=0x%x, w=%d, h=%d, x1=%d, y1=%d, y2=%d, y2=%d, RowSize=%ld, TotalSize=%ld ActualLen %ld", 
                                   __LINE__, __FUNCTION__,
                			pArtData->cPixelFormat, pArtData->sWidth, pArtData->sHeight, pArtData->sTopLeftX, pArtData->sTopLeftY, pArtData->sBottomRightX,
                			pArtData->sBottomRightY, pArtData->uiRowSize, pArtData->uiTotalSize,nActualLen);
                if(pArtData->uiTotalSize !=  (nActualLen - 1)){
                    IAPSVC_DBUG("%d: %s() [ALBUMART] wskim size error wskim ", __LINE__, __FUNCTION__);
                }
                break;
            case IPOD_NO_DATA:
                reply->writeInt32(false);
                 reply->writeInt64(nIndex);
                reply->writeInt32(0);
                reply->writeInt32(0);
                reply->writeInt32(0);
                IAPSVC_DBUG("%d: %s() IPOD_DR_GetAlbumArt() > No Album Art",
                                                                                                                        __LINE__,
                                                                                                                        __FUNCTION__
                                                                                                                        );
                break;
            default:
                ALOGE("%d: %s() IPOD_DR_GetAlbumArt() > Failed(0x%x) nIndex %ld",
                                                                                                                        __LINE__,
                                                                                                                        __FUNCTION__,
                                                                                                                        eCMDErrorCode,
                                                                                                                        nIndex
                                                                                                                        );
                reply->writeInt32(false);
                reply->writeInt64(nIndex);
                break;
        }
    }else{
        reply->writeInt32(false);
        reply->writeInt64(nIndex);    
    }
    return IsSuccess;
}

bool IapService::ProcGetAlbumArt(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    int bufSize = 0;
    unsigned char *pBuf = NULL;
    long nIndex = data->readInt64();
    pIPOD_PLAY_INFO pPlayInfo = IPOD_Sample_Get_AudioPlayInfo(GetIPodHandle());
    pIPOD_ARTWORK_INFO pArtworkInfo = &pPlayInfo->ArtworkInfo;
    bufSize = pArtworkInfo->MaxDisplaySize;
    bufSize += (bufSize/10);
    pBuf = (unsigned char *)malloc(bufSize);

    IAPSVC_DEFLOG("%d: %s() nIndex(%ld)", __LINE__, __FUNCTION__,nIndex);
    if(pBuf == NULL)
        ALOGE("%d: %s()  pBuf == NULL", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        if ( Util_IsInvalidIndex(nIndex) ){
            long nCurrentIndex = 0;
             eCMDErrorCode = (IPOD_ERROR)UtilGetCurrentTrackIndex(&nCurrentIndex);
            if ( IS_CMDOK(eCMDErrorCode) ){
                nIndex = (int)nCurrentIndex;
            }
            else{
                nIndex = 0;
            }
            IAPSVC_DBUG("%d: %s() nIndex(%ld) nIndex(%ld)",
                                                                                __LINE__,
                                                                                __FUNCTION__,
                                                                                nIndex,
                                                                                nCurrentIndex
                                                                                );
        }
        ArtworkData ArtData;
        unsigned long nActualLen = 0;
        ArtData.pImageData=(unsigned char *)pBuf;
        #if 0
        //IsSuccess = ProcCheckAlbumArtwork(nIndex,reply);
        IsSuccess = ProcGetSmallArtwork(nIndex, bufSize, (void *)&ArtData, reply);
        #else
        //REVIEWME@20100201, Test iPod library updated for IDPS, review me
        eCMDErrorCode = IPOD_DR_GetAlbumArt( 
                                                                        GetIPodHandle(),
                                                                        nIndex,
                                                                        &ArtData,
                                                                        bufSize,
                                                                        &nActualLen
                                                                        );
        IAPSVC_DBUG("%d: %s() eCMDErrorCode(0x%x) nActualLen %ld nIndex %ld",
                                                                                                __LINE__,
                                                                                                __FUNCTION__,
                                                                                                eCMDErrorCode,
                                                                                                nActualLen,
                                                                                                nIndex
                                                                                                );

        switch (eCMDErrorCode) {
            case 17:	// test
            case IPOD_NO_ERROR:
                IsSuccess = true;
                reply->writeInt32(true);
                reply->writeInt64(nIndex);
                reply->writeInt32((int)ArtData.sWidth);
                reply->writeInt32((int)ArtData.sHeight);
                reply->writeInt32((int)nActualLen);
                reply->writeInt32((int)nActualLen);
                reply->write(ArtData.pImageData,nActualLen);
                IAPSVC_DEFLOG("%d: %s() [ALBUMART] pixelformat=0x%x, w=%d, h=%d, x1=%d, y1=%d, y2=%d, y2=%d, RowSize=%ld, TotalSize=%ld ActualLen %ld", 
                                   __LINE__, __FUNCTION__,
                			ArtData.cPixelFormat, ArtData.sWidth, ArtData.sHeight, ArtData.sTopLeftX, ArtData.sTopLeftY, ArtData.sBottomRightX,
                			ArtData.sBottomRightY, ArtData.uiRowSize, ArtData.uiTotalSize,nActualLen);
                if(ArtData.uiTotalSize != (nActualLen - 1)){
                    ALOGE("%d: %s() [ALBUMART wskim ] nIndex %ld pixelformat=0x%x, w=%d, h=%d, x1=%d, y1=%d, y2=%d, y2=%d, RowSize=%ld, TotalSize=%ld ActualLen %ld", 
                                   __LINE__, __FUNCTION__,nIndex,
                			ArtData.cPixelFormat, ArtData.sWidth, ArtData.sHeight, ArtData.sTopLeftX, ArtData.sTopLeftY, ArtData.sBottomRightX,
                			ArtData.sBottomRightY, ArtData.uiRowSize, ArtData.uiTotalSize,nActualLen);
                    //IPOD_HID_TestAlbumartLog();
                }
                IAPSVC_DBUG("%d: %s() Parcel size(%d) %x %x %x %x", 
                                                                            __LINE__, 
                                                                            __FUNCTION__,
                                                                            reply->dataSize(),
                                                                            pBuf[0],pBuf[1],pBuf[2],pBuf[3]
                                                                            );
                break;
            case IPOD_NO_DATA:
                #if 0
                IsSuccess = ProcGetSmallArtwork(nIndex, bufSize, (void *)&ArtData, reply);
                #else
                reply->writeInt32(false);
                 reply->writeInt64(nIndex);
                reply->writeInt32(0);
                reply->writeInt32(0);
                reply->writeInt32(0);
                IAPSVC_DBUG("%d: %s() IPOD_DR_GetAlbumArt() > No Album Art",
                                                                                                                        __LINE__,
                                                                                                                        __FUNCTION__
                                                                                                                        );
                #endif
                break;
            default:
                ALOGE("%d: %s() IPOD_DR_GetAlbumArt() > Failed(0x%x) nIndex %ld",
                                                                                                                        __LINE__,
                                                                                                                        __FUNCTION__,
                                                                                                                        eCMDErrorCode,
                                                                                                                        nIndex
                                                                                                                        );
                reply->writeInt32(false);
                reply->writeInt64(nIndex);
                break;
        }
        #endif
    }
    else{
        ALOGE("%d: %s() Invalid state or invalid message.",__LINE__, __FUNCTION__);
    }
    IAPSVC_DBUG("%d: %s() end", __LINE__, __FUNCTION__);
    if(pBuf)
        free(pBuf);
    
    return IsSuccess;
}

bool IapService::GetAlbumArt(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    //if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_GET_ALBUMART,data,reply,true);
    //else
    //    ret = ProcGetAlbumArt(nIndex,reply);
    return ret;    
}

bool IapService::GetArtworkFormats(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GET_ARTWORK_FORMATS,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetArtworkFormats(data,reply);
    return ret;    
}

bool IapService::ProcGetArtworkFormats(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR Err = IPOD_NO_ERROR;
    ArtworkFormat pformat[IPOD_MAX_ARTWORK];
    long ActualLen = 0;
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        Err = IPOD_GetArtworkFormats(GetIPodHandle(), &pformat[0], IPOD_MAX_ARTWORK, &ActualLen);
        if(Err == IPOD_NO_ERROR){
            int cnt = 0;
            IsSuccess = true;
            reply->writeInt32(true);
            reply->writeInt32(ActualLen);
            IAPSVC_DBUG("%d: %s()  ActualLen %ld", __LINE__, __FUNCTION__,ActualLen);
            if(ActualLen > 0){
                for (cnt=0; cnt<ActualLen; cnt++)
                {
                    reply->writeInt32(pformat[cnt].FormatID);
                    reply->writeInt32(pformat[cnt].PixelFormat);
                    reply->writeInt32(pformat[cnt].ImageWidth);
                    reply->writeInt32(pformat[cnt].ImageHeight);
                }
            }
        }else{
            ALOGE("%s() %d: IPOD_GetArtworkFormats err 0x%x.", __FUNCTION__,__LINE__,Err);
            reply->writeInt32(false);
            reply->writeInt32(0);
        }
    }else{
        ALOGE("%d: %s() Invalid state or invalid message.",__LINE__, __FUNCTION__);
        reply->writeInt32(false);
        reply->writeInt32(0);
    }
    return IsSuccess;
}

bool IapService::GetArtworkCount(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GET_ARTWORK_COUNT,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetArtworkCount(data,reply);
    return ret;    
}

bool IapService::ProcGetArtworkCount(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR Err = IPOD_NO_ERROR;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        int cnt = 0;
        Info_Indexed_Track stTrackInfo;
        long ArtworkCount;
        long nIndex = data->readInt64();
        IAPSVC_DEFLOG("%d: %s() nIndex  %ld", __LINE__, __FUNCTION__,nIndex );
        
        Err = IPOD_GetIndexedPlayingTrackInfo(GetIPodHandle(), 
        										ARTWORKCOUNT, 
        										&stTrackInfo, 
        										nIndex, 
        										0/*chapterIndex*/, 
        										IPOD_MAX_ARTWORK, 
        										&ArtworkCount);
        if(Err == IPOD_NO_ERROR){
            IsSuccess = true;
            reply->writeInt32(true);
            reply->writeInt32(ArtworkCount);
            if(ArtworkCount > 0){
                for (cnt=0; cnt<ArtworkCount; cnt++)
                {
                    reply->writeInt32(stTrackInfo.curformatInfo.FormatID[cnt]);
                    reply->writeInt32(stTrackInfo.curformatInfo.CountOfImage[cnt]);
                }
            }
        }else{
            ALOGE("%s() %d: IPOD_GetIndexedPlayingTrackInfo err 0x%x.", __FUNCTION__,__LINE__,Err);
            reply->writeInt32(false);
            reply->writeInt32(0);
        }
    }else{
        ALOGE("%d: %s() Invalid state or invalid message.",__LINE__, __FUNCTION__);
        reply->writeInt32(false);
        reply->writeInt32(0);
    }
    return IsSuccess;
}

bool IapService::GetArtworkTimes(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GET_ARTWORK_TIMES,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetArtworkTimes(data,reply);
    return ret;    
}

						
bool IapService::ProcGetArtworkTimes(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR Err = IPOD_NO_ERROR;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        int cnt = 0;
        long nIndex = data->readInt64();
        int formatId = data->readInt32();
        int artworkIndex = data->readInt32();
        int artworkCount = data->readInt32();
         unsigned long outputData[IPOD_MAX_ARTWORK];
        if(artworkCount > IPOD_MAX_ARTWORK)
            artworkCount = IPOD_MAX_ARTWORK;

        IAPSVC_DEFLOG("%d: %s() nIndex %ld ID %d index %d count %d", 
                                                            __LINE__, 
                                                            __FUNCTION__,
                                                            nIndex,
                                                            formatId,
                                                            artworkIndex,
                                                            artworkCount
                                                            );
        
        Err = IPOD_GetTrackArtworkTimes(GetIPodHandle(), 
        									nIndex, 
        									formatId, 
        									artworkIndex, 
        									artworkCount, 
        									outputData, 
        									IPOD_MAX_ARTWORK);
        if(Err == IPOD_NO_ERROR){
            IsSuccess = true;
            reply->writeInt32(true);
            reply->writeInt32(artworkCount);
            for(cnt =0 ; cnt < artworkCount;cnt++){
                reply->writeInt32(outputData[cnt]);
            }
        }else{
            ALOGE("%s() %d: IPOD_GetTrackArtworkTimes err 0x%x.", __FUNCTION__,__LINE__,Err);
            reply->writeInt32(false);
            reply->writeInt32(0);
        }
    }else{
        ALOGE("%d: %s() Invalid state or invalid message.",__LINE__, __FUNCTION__);
        reply->writeInt32(false);
        reply->writeInt32(0);
    }
    return IsSuccess;
}

bool IapService::ProcGetArtworkSizeWithFormatID(unsigned long formatId,unsigned long *pImageSize)
{
    bool ret = false;
    int i;
    pIPOD_PLAY_INFO pPlayInfo = IPOD_Sample_Get_AudioPlayInfo(GetIPodHandle());
    pIPOD_ARTWORK_INFO pArtworkInfo = &pPlayInfo->ArtworkInfo;
    IAPSVC_DBUG("%d: %s() BasicFormatID[0x%lx] MaxFormatCount %ld ", 
                            __LINE__, __FUNCTION__,pArtworkInfo->BasicFormatID,pArtworkInfo->MaxFormatCount);
    for(i = 0;i < (int)pArtworkInfo->MaxFormatCount ;i++){
        // if formatId is valid and is not a basic format
        IAPSVC_DBUG("%d: %s() formatId[0x%lx] size %ld", 
                                        __LINE__, __FUNCTION__,
                                        pArtworkInfo->CapableFormatID[i],
                                        pArtworkInfo->CapableSize[i]
                                        );
        if(pArtworkInfo->CapableFormatID[i] == formatId)// && pArtworkInfo->CapableFormatID[i] != pArtworkInfo->BasicFormatID )
        {
            *pImageSize = pArtworkInfo->CapableSize[i];
            ret = true;
            break;
        }
    }

    return ret;
}

bool IapService::GetArtworkData(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GET_ARTWORK_DATA,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetArtworkData(data,reply);
    return ret;    
}

bool IapService::ProcGetArtworkData(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR Err = IPOD_NO_ERROR;
    if ( IsInitialized() ) {
        int cnt = 0;
        ArtworkData ArtData;
        unsigned long bufSize = 0;
        unsigned long nActualLen = 0;
        long nIndex = data->readInt64();
        int formatId = data->readInt32();
        int artworkTimes = data->readInt32();
        int width = data->readInt32();
        int height = data->readInt32();
        int pixelFormat = data->readInt32();
        unsigned char *pBuf = NULL;
        int pixelLen = 2;

        switch(pixelFormat){
            case RGB565_LITTLE_ENDIAN_16BPP:
            case RGB565_BIG_ENDIAN_16BPP:
                pixelLen = 2;
                break;
            case MONOCHROME_2BPP:
                pixelLen = 1;
                break;
            default:
                pixelLen = 0;
                break;
        }
        
        bufSize = width*height*pixelLen;
        IAPSVC_DEFLOG("%d: %s() nIndex %ld formatId %d artworkTimes %d w %d h %d p 0x%x size %ld", 
                            __LINE__, __FUNCTION__,nIndex,formatId,artworkTimes,width,height,pixelFormat,bufSize);
        
        if(bufSize > 0 && bufSize <= IPOD_RXBUFSIZE){
            pBuf = (unsigned char *)malloc(bufSize);
            ArtData.pImageData=(unsigned char *)pBuf;
            Err = IPOD_GetTrackArtworkData(GetIPodHandle(), 
            											nIndex, 
            											formatId, 
            											artworkTimes, 
            											&ArtData,
            											bufSize,
            											&nActualLen);
            if(Err == IPOD_NO_ERROR){
                nActualLen = ArtData.uiTotalSize;
                IAPSVC_DBUG("%d: %s() actualLen %ld ", 
                            __LINE__, __FUNCTION__,nActualLen);
                IsSuccess = true;
                reply->writeInt32(IsSuccess);
                reply->writeInt64(nIndex);
                reply->writeInt32((int)ArtData.sWidth);
                reply->writeInt32((int)ArtData.sHeight);
                reply->writeInt32((int)nActualLen);
                reply->writeInt32((int)nActualLen);
                reply->write(ArtData.pImageData,nActualLen);
            }else{
                ALOGE("%s() %d: IPOD_GetTrackArtworkData err 0x%x.", __FUNCTION__,__LINE__,Err);
                reply->writeInt32(false);
                reply->writeInt32(0);
            }
        }else{
            ALOGE("%d: %s() invalid size w %d h %d pixel 0x%x",__LINE__, __FUNCTION__,width,height,pixelFormat);
            reply->writeInt32(false);
            reply->writeInt32(0);
        }        
        
    }else{
        ALOGE("%d: %s() Invalid state or invalid message.",__LINE__, __FUNCTION__);
        reply->writeInt32(false);
        reply->writeInt32(0);
    }
    return IsSuccess;
}

bool IapService::ProcGetDeviceDisplayInfo(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    long width,height;
    IPOD_PIXEL_FORMAT format;
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
       
    width = 0;
    height = 0;

    if ( IsInitialized() ) {
       
        eCMDErrorCode = IPOD_GetColorDisplayLimits( GetIPodHandle(), &format, &width, &height );
        if ( IS_CMDOK(eCMDErrorCode) )
        {
            IsSuccess = true;
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    reply->writeInt32(IsSuccess);
    reply->writeInt64(width);
    reply->writeInt64(height);
    reply->writeInt32(format);
    return IsSuccess;
}

bool IapService::GetDeviceDisplayInfo(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_GET_DEVDSP_INFO,data,reply,true);
    else
        ret = ProcGetDeviceDisplayInfo(data,reply);
    return ret;    
}

bool IapService::ProcSetDeviceDisplayImage(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    
    long width = data->readInt64();
    long height = data->readInt64();
    int actualLen = data->readInt32();
    IAPSVC_DEFLOG("%d: %s()  w %ld h %ld len %d", __LINE__, __FUNCTION__,width,height,actualLen);
    unsigned char *pImage = (unsigned char *)IPOD_MemAlloc(actualLen);
    if(pImage){
        if(data->read(pImage,actualLen) == 0){
            if ( IsInitialized() ) {
                eCMDErrorCode=IPOD_SetDisplayImage( GetIPodHandle(), 
                                                                    IPOD_PIXEL_RGB_565_LITTLE,
                                                                    width,
                                                                    height,
                                                                    (unsigned char *)pImage);
                if ( IS_CMDOK(eCMDErrorCode)) 
                {
                    IsSuccess = true;
                }
                else
                {
                    ALOGE("%d: %s()  IPOD_SetDisplayImage() Failed > eCMDErrorCode=0x%x", 
                                                                                                        __LINE__,
                                                                                                        __FUNCTION__,
                                                                                                        eCMDErrorCode
                                                                                                        );

                }
            }else{
                ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
            }
        }
    }

    IPOD_MemDealloc((void *)pImage);
    reply->writeInt32(IsSuccess);
    return IsSuccess;
}
    
bool IapService::SetDeviceDisplayImage(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s()", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_SET_DEVDSP_IMAGE,data,reply,true);
    else
        ret = ProcSetDeviceDisplayImage(data,reply);
    return ret;    
}

bool IapService::ProcSetTrackPosition(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    IPOD_DR_SETSTATEINFO_PARAM settingInfo;
    long pos_ms = data->readInt64();
    IAPSVC_DEFLOG("%d: %s() pos_ms %ld", __LINE__, __FUNCTION__,pos_ms);

    memset(&settingInfo,0,sizeof(IPOD_DR_SETSTATEINFO_PARAM));
    if ( IsInitialized() ) {
        settingInfo.m_InfoType = IPOD_DR_STATUS_TRACK_POS_MS;
        settingInfo.m_data.m_TrackPos_ms = (unsigned long)pos_ms;
        
        eCMDErrorCode = IPOD_DR_SetiPodStateInfo(GetIPodHandle(),&settingInfo);
        if ( IS_CMDOK(eCMDErrorCode)) 
        {
            IPOD_DR_PLAY_STATUS playStatus;
            if (gNextPlayPause == FALSE) {
                DoEndFfRew();
                IsSuccess = true;
            } else {
                IsSuccess = DoEndFfRew();
                if(Sample_GetDRPlayStatus((void *)&playStatus)){
                    IAPSVC_DEFLOG("%d: %s() playStatus %d localPlayStatus %d", 
                                                                            __LINE__, 
                                                                            __FUNCTION__,
                                                                            playStatus,
                                                                            GetSvcLocalPlayStatus()
                                                                            );

                    if(playStatus != IPOD_DR_PLAYING){
                        if(GetSvcLocalPlayStatus() == IPOD_DR_END_FAST_FORWARD_OR_REWIND){
                            IAPSVC_DBUG("%s() wait end FF or Rew",__FUNCTION__);
                            SetSvcWantedNextAction(IPOD_DR_PLAYING);
                            IsSuccess = true;
                        }else{
                            eCMDErrorCode = IPOD_PlayControl( GetIPodHandle(), PLAYPAUSE);
                            if ( IS_CMDOK(eCMDErrorCode) ){
                                IsSuccess = true;
                                SetSvcLocalPlayStatus(IPOD_DR_PLAYING);
                                //IPOD_Sample_SetDRPlayStatus(GetIPodHandle(), IPOD_DR_PLAYING);
                            }else{
                                ALOGE(" %s  > IPOD_PlayControl() PlayPause Failed(0x%x)",
                                                                                                    __FUNCTION__,
                                                                                                    eCMDErrorCode
                                                                                                    );
                            }      
                        }
                    }else{
                        IsSuccess = true;
                    }
                }
            }
        }
        else
        {
            ALOGE("%d: %s()  IPOD_DR_SetiPodStateInfo() Failed > eCMDErrorCode=0x%x", 
                                                                                                                __LINE__,
                                                                                                                __FUNCTION__,
                                                                                                                eCMDErrorCode
                                                                                                                );
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }        

    if(reply != NULL)
        reply->writeInt32(IsSuccess);
    
    return IsSuccess;    
}

bool IapService::SetTrackPosition(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_SET_POSITION,data,NULL,true);
    else
        ret = ProcSetTrackPosition(data,NULL);
    return ret;    
}

bool IapService::ProcDatabaseInit(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    long nCount = -1;
    int64_t newTime = (int64_t)systemTime();
    int nCategory = data->readInt32();
    
    IAPSVC_DEFLOG("%d: %s()  nCategory %d", __LINE__, __FUNCTION__,nCategory);
    if(nCategory == -1)
        IAPSVC_DBUG("%d: %s() invalid category %d", __LINE__, __FUNCTION__,nCategory);

    /*
    if(IsValidTimeSchedule(newTime) == false)
    {
        IAPSVC_DBUG("%d: %s()  ATS Time Error.", __LINE__, __FUNCTION__);
    }
    else 
    */
    if ( IsInitialized() ) {
        CategoryType eCategory = (CategoryType)nCategory;

        eCMDErrorCode = IPOD_ResetDBSelection( GetIPodHandle() );
        if ( IS_CMDOK(eCMDErrorCode) ){
            eCMDErrorCode = IPOD_GetNumberCategorizedDBRecords( 
                                                                                            GetIPodHandle(),
                                                                                            eCategory,
                                                                                            &nCount 
                                                                                            );
            if ( IS_CMDOK(eCMDErrorCode) ){
                pIPOD_PLAY_INFO pInfo  = (pIPOD_PLAY_INFO)IPOD_GetExtInfo(GetIPodHandle());
                pInfo->mRootCategory = (CategoryType)nCategory;
                pInfo->mCurrentCategory = (CategoryType)nCategory;                

                SetLastEventTime(newTime);
                IsSuccess = true;
            }
            else{
                ALOGE("%d: %s()  IPOD_GetNumberCategorizedDBRecords() > Failed(0x%x).", 
                                                                                                __LINE__,
                                                                                                __FUNCTION__,
                                                                                                eCMDErrorCode
                                                                                                );
            }
        }
        else{
            ALOGE("%d: %s()  IPOD_ResetDBSelection() > Failed(0x%x).",
                                                                                    __LINE__,
                                                                                    __FUNCTION__,
                                                                                    eCMDErrorCode
                                                                                    );	
        }
    }
    else{
        ALOGE("%d: %s()   Invalid state or invalid message. iPodControlState(%d)", 
                                                                                    __LINE__,
                                                                                    __FUNCTION__,
                                                                                    iPodControlState
                                                                                    );
    }  
    reply->writeInt32(IsSuccess);
    if(IsSuccess == true){
        reply->writeInt32(nCategory);
        reply->writeInt64(nCount);
    }else{
        reply->writeInt32(-1);
        reply->writeInt64(0);
    }
    IAPSVC_DBUG("%d: %s() category(%d) nCount(%ld)", 
                                                                            __LINE__,
                                                                            __FUNCTION__,
                                                                            nCategory,
                                                                            nCount
                                                                            );
    return IsSuccess;
;
}

bool IapService::DatabaseInit(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_DATABASE_INIT,data,reply,true);
    else
        ret = ProcDatabaseInit(data,reply);
    return ret;    
}

bool IapService::ProcDatabaseSelectAll(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    long nCount = 0;
    
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);

    int64_t newTime = (int64_t)systemTime();
    #if 0
    if(IsValidTimeSchedule(newTime) == false)
    {
        IAPSVC_DBUG("%d: %s()  ATS Time Error.", __LINE__, __FUNCTION__);
    }
    else if ( IsInitialized() ) {
    #else
    if ( IsInitialized() ) {
    #endif
        pIPOD_PLAY_INFO pInfo  = (pIPOD_PLAY_INFO)IPOD_GetExtInfo(GetIPodHandle());
        CategoryType eCategory = pInfo->mCurrentCategory;
                
        if ( IPOD_TRACK == eCategory ){
            IsSuccess = DoEndFfRew();

            eCMDErrorCode = IPOD_PlayControl(GetIPodHandle(), STOP);
            if ( IS_CMDERROR(eCMDErrorCode) ){
                ALOGE("%d: %s() IPOD_PlayControl(STOP) > Falied(%d).", 
                                                                                            __LINE__,
                                                                                            __FUNCTION__,
                                                                                            eCMDErrorCode
                                                                                            );
            }
            else{
                eCMDErrorCode = IPOD_SelectDBRecord(GetIPodHandle(), eCategory, 0);
                if ( IS_CMDERROR(eCMDErrorCode) ){
                    ALOGE("%d: %s() IPOD_SelectDBRecord > Falied(%d) eCategory %d.",
                                                                                                __LINE__,
                                                                                                __FUNCTION__,
                                                                                                eCMDErrorCode,
                                                                                                eCategory
                                                                                                );
                }
                else{
                    eCMDErrorCode = IPOD_GetNumberCategorizedDBRecords( 
                                                                                                    GetIPodHandle(),
                                                                                                    eCategory,
                                                                                                    &nCount 
                                                                                                    );
                    if ( IS_CMDOK(eCMDErrorCode) ){
                        IsSuccess = true;
                        SetLastEventTime(newTime);
                    }
                    else{
                        ALOGE("%d: %s() IPOD_GetNumberCategorizedDBRecords > Falied(%d) eCategory %d.",
                                                                                                    __LINE__,
                                                                                                    __FUNCTION__,
                                                                                                    eCMDErrorCode,
                                                                                                    eCategory
                                                                                                    );
                    }
                }
            }
        }
        else{
            IAPSVC_DBUG("%d: %s() Cate(%d) ", __LINE__, __FUNCTION__,eCategory);
            eCategory = IPOD_GetSubCategory( eCategory,pInfo->mRootCategory);
            if(eCategory == -1){
                IAPSVC_DBUG("%d: %s() invalid category mRootCategory (%d).", 
                                                                                                        __LINE__,
                                                                                                        __FUNCTION__,
                                                                                                        pInfo->mRootCategory
                                                                                                        );
            }else{
                eCMDErrorCode = IPOD_GetNumberCategorizedDBRecords( 
                                                                                                    GetIPodHandle(),
                                                                                                    eCategory,
                                                                                                    &nCount 
                                                                                                    );
                if ( IS_CMDOK(eCMDErrorCode) ){
                    IsSuccess = true;
                    SetLastEventTime(newTime);
                    pInfo->mCurrentCategory = eCategory;    
                    reply->writeInt32(true);
                    reply->writeInt32(eCategory);
                    reply->writeInt64(nCount);
                }
                else{
                    ALOGE("%d: %s() IPOD_GetNumberCategorizedDBRecords > Falied(%d) eCategory %d..", 
                                                                                                __LINE__,
                                                                                                __FUNCTION__,
                                                                                                eCMDErrorCode,
                                                                                                eCategory
                                                                                                );
                }
            }
        }
    }
    else{
        ALOGE("%d: %s()  Invalid state or invalid message.", __LINE__, __FUNCTION__);
    }    

    if(IsSuccess == false){
        reply->writeInt32(false);
        reply->writeInt32(-1);
        reply->writeInt64(0);        
    }
    return IsSuccess;
}

bool IapService::DatabaseSelectAll(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_DATABASE_SELECT_ALL,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }
    else
        ret = ProcDatabaseSelectAll(data,reply);
    return ret;    
}

bool IapService::ProcDatabaseSelect(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    long nCount = 0;
    long nIndex = data->readInt64();
    
    IAPSVC_DEFLOG("%d: %s() nIndex(%ld) IsFFREWMode(%d)",
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            nIndex,
                                                                                            GetSvcLocalFFRewStatus()
                                                                                            );

    int64_t newTime = (int64_t)systemTime();
    #if 0
    if(IsValidTimeSchedule(newTime) == false)
    {
        IAPSVC_DBUG("%d: %s()  ATS Time Error.", __LINE__, __FUNCTION__);
    }
    else if ( IsInitialized() ) {
    #else
    if ( IsInitialized() ) {
    #endif
        pIPOD_PLAY_INFO pInfo  = (pIPOD_PLAY_INFO)IPOD_GetExtInfo(GetIPodHandle());
        CategoryType eCategory = pInfo->mCurrentCategory;        
        if(eCategory == -1)
            IAPSVC_DBUG("%d: %s() invalid category %d", __LINE__, __FUNCTION__,eCategory);        
        IsSuccess = DoEndFfRew();

        #if 0 
        if ( IPOD_TRACK == eCategory || IPOD_AUDIOBOOK == eCategory){
            eCMDErrorCode = IPOD_PlayControl( GetIPodHandle(), STOP);
            if ( IS_CMDERROR(eCMDErrorCode) ){
                IAPSVC_DBUG("%d: %s() IPOD_PlayControl(STOP) > Falied(%d).", __LINE__, __FUNCTION__, eCMDErrorCode);
            }
        }
        #endif
        if ( IPOD_TRACK == eCategory)
        {
            /* this command is deprecated. it is used for iPod nano 1G. */
            eCMDErrorCode = IPOD_PlayCurrentSelection(GetIPodHandle(), nIndex); 
            IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
            if ( IS_CMDERROR(eCMDErrorCode) ){
                ALOGE("%d: %s() IPOD_PlayCurrentSelection > Falied(0x%x) nIndex %ld.", 
                                                                                    __LINE__, 
                                                                                    __FUNCTION__, 
                                                                                    eCMDErrorCode,
                                                                                    nIndex
                                                                                    );
            }
        }
        else
        {
            eCMDErrorCode = IPOD_SelectDBRecord(GetIPodHandle(), eCategory, nIndex);
            IAPSVC_DBUG("%d: %s() zIndex(%ld) ", __LINE__, __FUNCTION__,nIndex);
             if ( IS_CMDERROR(eCMDErrorCode) ){
                ALOGE("%d: %s() IPOD_SelectDBRecord > Falied(0x%x) eCategory %d nIndex %ld.", 
                                                                                    __LINE__, 
                                                                                    __FUNCTION__, 
                                                                                    eCMDErrorCode,
                                                                                    eCategory,
                                                                                    nIndex
                                                                                    );
            }
        }

        if ( IS_CMDERROR(eCMDErrorCode) ){

        }
        else{
            if ( IPOD_TRACK == eCategory || IPOD_AUDIOBOOK == eCategory){
                IPOD_DR_PLAY_STATUS playStatus = IPOD_DR_PLAYBACK_STOPPED;
                if(Sample_GetDRPlayStatus( (void *)&playStatus))
                {
                    IAPSVC_DBUG("%d: %s() playStatus(%d) ",
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        playStatus
                                                                        );
                    if (IPOD_DR_PLAYING != playStatus){
                        SetSvcLocalPlayStatus(IPOD_DR_PLAYING);
                        //IPOD_Sample_SetDRPlayStatus(GetIPodHandle(), IPOD_DR_PLAYING);
                    }
                }
                IsSuccess = true;
                reply->writeInt32(true);
                reply->writeInt32(-1);
                reply->writeInt64(0);
                reply->writeInt64(nIndex);
                IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
            }
            else{
                IAPSVC_DBUG("%d: %s() Cate(%d) ", __LINE__, __FUNCTION__,eCategory);
                eCategory = IPOD_GetSubCategory( eCategory,pInfo->mRootCategory);
                IAPSVC_DBUG("%d: %s() subCate(%d) ", __LINE__, __FUNCTION__,eCategory);
                if(eCategory == -1){
                    ALOGE("%d: %s() invalid category mRootCategory (%d) .", 
                                                                                                    __LINE__,
                                                                                                    __FUNCTION__,
                                                                                                    pInfo->mRootCategory
                                                                                                    );
                }else{
                    eCMDErrorCode = IPOD_GetNumberCategorizedDBRecords( 
                                                                                                GetIPodHandle(),
                                                                                                eCategory,
                                                                                                &nCount 
                                                                                                );
                    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
                    if ( IS_CMDOK(eCMDErrorCode) ){
                        IsSuccess = true;
                        pInfo->mCurrentCategory = eCategory;    
                        reply->writeInt32(true);
                        reply->writeInt32(eCategory);
                        reply->writeInt64(nCount);
                        reply->writeInt64(nIndex);
                        IAPSVC_DBUG("%d: %s() new category(%d) newTotalCount(%ld)", 
                                                                                                            __LINE__,
                                                                                                            __FUNCTION__,
                                                                                                            eCategory,
                                                                                                            nCount
                                                                                                            );
                    }
                    else{
                        ALOGE("%d: %s() IPOD_GetNumberCategorizedDBRecords > Falied(%d) eCategory %d.",
                                                                                                        __LINE__,
                                                                                                        __FUNCTION__,
                                                                                                        eCMDErrorCode,
                                                                                                        eCategory
                                                                                                        );
                    }
                }
            }
            SetLastEventTime(newTime);
        }
    }
    else{
        IAPSVC_DBUG("%d: %s()  > Invalid state or invalid message.", __LINE__, __FUNCTION__);
    }
    if(IsSuccess == false){
        reply->writeInt32(false);
        reply->writeInt32(-1);
        reply->writeInt64(0);        
        reply->writeInt64(0);
    }    
    IAPSVC_DBUG("%d: %s() IsSuccess (%d)", __LINE__, __FUNCTION__,IsSuccess);
    return IsSuccess;
}

bool IapService::DatabaseSelect(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_DATABASE_SELECT,data,reply,true);
    else
        ret = ProcDatabaseSelect(data,reply);
    return ret;    
}

bool IapService::ProcDatabaseMoveUpper(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    long nCount = 0;
    
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);

    int64_t newTime = (int64_t)systemTime();
    #if 0
    if(IsValidTimeSchedule(newTime) == false)
    {
        IAPSVC_DBUG("%d: %s()  ATS Time Error.", __LINE__, __FUNCTION__);
    }
    else if ( IsInitialized() ) {
    #else
    if ( IsInitialized() ) {
    #endif
        pIPOD_PLAY_INFO pInfo  = (pIPOD_PLAY_INFO)IPOD_GetExtInfo(GetIPodHandle());
        CategoryType eCategory = pInfo->mCurrentCategory;               
        CategoryType eRootCategory = (CategoryType)pInfo->mRootCategory;
        long zIndex = (-1);
        if(eCategory == -1){
            IAPSVC_DBUG("%d: %s() invalid category %d", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        eCategory
                                                                        );
        }
        IAPSVC_DBUG("%d: %s()  cate %d rootCate %d", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        eCategory,
                                                                        eRootCategory
                                                                        );
        if(eCategory != eRootCategory){
            eCategory = IPOD_GetUpperCategory( eCategory ,eRootCategory);
            if(eCategory == -1){
                IAPSVC_DBUG("%d: %s() upperCate %d", __LINE__, __FUNCTION__,eCategory);
            }
        }

        if ( IPOD_ALL == eCategory )  {
            ALOGE("%d: %s() IPOD_API_GetUpperCategory > Invalid category(%d).",
                                                                                                            __LINE__,
                                                                                                            __FUNCTION__,
                                                                                                            eCategory
                                                                                                            );
        }
        else if(eCategory == -1){
            ALOGE("%d: %s() upperCate %d", __LINE__, __FUNCTION__,eCategory);
        }else{
            IAPSVC_DBUG("%d: %s() eCategory %d ", __LINE__, __FUNCTION__,eCategory);
            eCMDErrorCode = IPOD_SelectDBRecord(GetIPodHandle(), eCategory, zIndex);

            if ( IS_CMDERROR(eCMDErrorCode) ){
                ALOGE("%d: %s() IPOD_SelectDBRecord > Falied(%d) category %d index %ld.", 
                                                                                        __LINE__,
                                                                                        __FUNCTION__,
                                                                                        eCMDErrorCode,
                                                                                        eCategory,
                                                                                        zIndex
                                                                                        );
            }
            else{
                IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
                eCMDErrorCode = IPOD_GetNumberCategorizedDBRecords( 
                                                                                            GetIPodHandle(),
                                                                                            eCategory,
                                                                                            &nCount 
                                                                                            );
                if ( IS_CMDOK(eCMDErrorCode) ){
                    CategoryType eNewCategory;
                    IAPSVC_DBUG("%d: %s() eCategory %d  totalCount %ld ",
                                                                                                __LINE__,
                                                                                                __FUNCTION__,
                                                                                                eCategory,
                                                                                                nCount
                                                                                                );
                    IsSuccess = true;
                    pInfo->mCurrentCategory = eCategory;    
                    eNewCategory = IPOD_GetUpperCategory( eCategory ,eRootCategory);
                    if(eNewCategory == -1)
                        IAPSVC_DBUG("%d: %s() eNewCategory %d", 
                                                                                            __LINE__,
                                                                                            __FUNCTION__,
                                                                                            eNewCategory
                                                                                            );
                    reply->writeInt32(true);
                    reply->writeInt32(eCategory);
                    reply->writeInt64(nCount);
                    reply->writeInt32(eNewCategory);
                    SetLastEventTime(newTime);
                }
                else{
                    ALOGE("%d: %s() IPOD_GetNumberCategorizedDBRecords > Falied(%d) eCategory %d.", 
                                                                                                __LINE__,
                                                                                                __FUNCTION__,
                                                                                                eCMDErrorCode,
                                                                                                eCategory
                                                                                                );
                }
            }						
        }
    }
    else{
        ALOGE("%d: %s() Invalid state or invalid message.", __LINE__, __FUNCTION__);
    }
    if(IsSuccess == false){
        reply->writeInt32(false);
        reply->writeInt32(-1);
        reply->writeInt64(0);       
        reply->writeInt32(-1);
    }                        
    return IsSuccess;
}

bool IapService::DatabaseMoveUpper(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_DATABASE_MOVE_UPPER,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    else
        ret = ProcDatabaseMoveUpper(data,reply);
    return ret;    
}

bool IapService::ProcDatabaseGetNames(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    IPOD_RECORD_TYPE *pDatabaseDBRecord;
    long nDatabaseMaxCountRequested = 0;
    long nDatabaseDBRecordNum = 0;
    long nRetCount = 0;
    int category = data->readInt32();
    long zStartIndex = data->readInt64();
    long nCount = data->readInt64();
    IAPSVC_DEFLOG("%d: %s() startIndex %ld  count %ld ", 
                                                                            __LINE__,
                                                                            __FUNCTION__,
                                                                            zStartIndex,
                                                                            nCount
                                                                            );

    if ( IsInitialized() ) {
        //pIPOD_PLAY_INFO pInfo  = (pIPOD_PLAY_INFO)IPOD_GetExtInfo(GetIPodHandle());
        CategoryType eCategory = (CategoryType)category; //pInfo->mCurrentCategory;            
        
        if ( nCount ){
            int bufSize =  nCount*sizeof(IPOD_RECORD_TYPE);
            pDatabaseDBRecord = (IPOD_RECORD_TYPE *)malloc(bufSize);
            if(pDatabaseDBRecord != NULL){
                memset(pDatabaseDBRecord,0x00,bufSize);
                nDatabaseMaxCountRequested = nCount;
                nDatabaseDBRecordNum = nCount;

                IAPSVC_DBUG("%d: %s() IPOD_RetrieveCategorizedDatabaseRecords", __LINE__, __FUNCTION__);
                eCMDErrorCode = IPOD_RetrieveCategorizedDatabaseRecords( GetIPodHandle(), 
                                                                        eCategory, 
                                                                        zStartIndex, 
                                                                        nCount,
                                                                        pDatabaseDBRecord 
                                                                        );

                IAPSVC_DBUG("%d: %s() eCMDErrorCode %d", __LINE__, __FUNCTION__,eCMDErrorCode);
                if ( IS_CMDOK(eCMDErrorCode) ){
                    int i;
                    if(IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) != IPOD_CONNECTED){
                        ALOGE("%d: %s() IPOD_GetDetectStatus() > disconnected ", __LINE__, __FUNCTION__);
                        eCMDErrorCode = IPOD_DISCONNECT_ERROR;
                    }
                    for(i =0; i < nCount;i++){
                        if(pDatabaseDBRecord[i].DBindex != (zStartIndex + i)){
                            ALOGE("%s() wanted(%ld) index[%d]DBindex(%ld) ",
                                                                                __FUNCTION__,
                                                                                (zStartIndex + i),
                                                                                i,
                                                                                pDatabaseDBRecord[i].DBindex);
                            eCMDErrorCode = IPOD_UNKNOWN_ERROR;
                            break;
                        }
                        if(pDatabaseDBRecord[i].ActualLen > (long)IPOD_GetStringBufferSize()){
                            ALOGE("%s() ActualLen(%ld) >= max(%ld) ",
                                                                                __FUNCTION__,
                                                                                pDatabaseDBRecord[i].ActualLen,
                                                                                IPOD_GetStringBufferSize());
                            eCMDErrorCode = IPOD_UNKNOWN_ERROR;
                            break;
                        }
                    }
                    if(IS_CMDERROR(eCMDErrorCode)){
                        ALOGE("%s() IPOD_RetrieveCategorizedDatabaseRecords() > invalid ", __FUNCTION__);
                    }
                }

                if(IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) != IPOD_CONNECTED){
                    ALOGE("%d: %s() IPOD_GetDetectStatus() > disconnected ", __LINE__, __FUNCTION__);
                } else if ( IS_CMDOK(eCMDErrorCode) ){
                    long index;
                    IsSuccess = true;
                    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
                    reply->writeInt32(true);
                    reply->writeInt32(category);
                    reply->writeInt64(zStartIndex);
                    reply->writeInt64(nCount);
                    for(index = 0;index < nCount;index++){
                        IPOD_RECORD_TYPE *pDBRecord = &pDatabaseDBRecord[index];
                        #if 1
                        IAPSVC_DBUG("%d: %s() index[%ld] dbIndex[%ld] len[%ld]", 
                                                                                __LINE__, 
                                                                                __FUNCTION__,
                                                                                index,
                                                                                pDatabaseDBRecord[index].DBindex,
                                                                                pDatabaseDBRecord[index].ActualLen
                                                                                );
                        #endif
                        if(pDatabaseDBRecord[index].ActualLen <= (long)IPOD_GetStringBufferSize()  
                            && pDatabaseDBRecord[index].ActualLen != 0)
                        {
                            String8 name((const char*)pDatabaseDBRecord[index].Data);
                            reply->writeString16(String16(name));
                            nRetCount++;
                            IAPSVC_DBUG("%d: %s() str [%s] nRetCount[%ld]", 
                                                                                                __LINE__,
                                                                                                __FUNCTION__,
                                                                                                name.string(),
                                                                                                nRetCount
                                                                                                );

                        }else if(pDatabaseDBRecord[index].ActualLen 
                                        >  (long)IPOD_GetStringBufferSize())
                        {
                            String8 name("");
                            reply->writeString16(String16(name));
                            nRetCount++;
                            ALOGE("%d: %s() category %d start %ld pDatabaseDBRecord[index %ld].ActualLen (%ld)",
                                                                                __LINE__,
                                                                                __FUNCTION__,
                                                                                eCategory,
                                                                                zStartIndex,
                                                                                index,
                                                                                pDatabaseDBRecord[index].ActualLen
                                                                                );
                        }else if(pDatabaseDBRecord[index].ActualLen == 0){
                            String8 name("");
                            reply->writeString16(String16(name));
                            nRetCount++;
                            ALOGE("%d: %s() category %d  start %ld pDatabaseDBRecord[index %ld].ActualLen", 
                                                                                                            __LINE__,
                                                                                                            __FUNCTION__,
                                                                                                            eCategory,
                                                                                                            zStartIndex,
                                                                                                            index
                                                                                                            );
                        }
                    }
                    IAPSVC_DBUG("%d: %s() nRetCount(%ld) ", __LINE__, __FUNCTION__,nRetCount);
                }else{
                    ALOGE("%d: %s() IPOD_RetrieveCategorizedDatabaseRecords() > Failed(0x%x) eCategory %d",
                                                                                                                __LINE__,
                                                                                                                __FUNCTION__,
                                                                                                                eCMDErrorCode,
                                                                                                                eCategory
                                                                                                                );
                }
                if(pDatabaseDBRecord)
                    free(pDatabaseDBRecord);
            }
            else
            {
                ALOGE("%d: %s()  malloc failed.", __LINE__, __FUNCTION__);
            }
        }
    }
    else{
        ALOGE("%d: %s()  Invalid state or invalid message.", __LINE__, __FUNCTION__);
    }

    if(IsSuccess == false){
        reply->writeInt32(true);
        reply->writeInt64(0);
    }
    
    return IsSuccess;
}

bool IapService::DatabaseGetNames(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(
                                                                    IAP_CMD_DATABASE_GET_NAMES,
                                                                    data,
                                                                    reply,
                                                                    true
                                                                    );
    else
        ret = ProcDatabaseGetNames(data,reply);
    return ret;    
}

bool IapService::ExtFunc1()
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    return true;
}

bool IapService::ExtFunc2()
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    return true;
}

bool IapService::ExtFunc3()
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    return true;
}

bool IapService::ExtFunc4()
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    return true;
}

bool IapService::ExtFunc5()
{
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    return true;
}

bool IapService::ProcGetDrPlayStatus(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    long nIndex = -1;
    IPOD_DR_PLAY_STATUS status = IPOD_DR_PLAYBACK_PAUSED;
    if ( IsInitialized() ) {
        ret = Sample_GetDRPlayStatus((void *)&status);
        reply->writeInt32(ret);
        if(IPOD_Sample_GetCurrentIndex(GetIPodHandle(),&nIndex) == IPOD_NO_ERROR)
            reply->writeInt64(nIndex);
        else
            reply->writeInt64(-1);
        reply->writeInt32(status);
    }else{
        ALOGE("%d: %s()  Invalid state or invalid message.", __LINE__, __FUNCTION__);
        reply->writeInt32(false);
    }
    return ret;
}

bool IapService::GetDrPlayStatus(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                            IAP_CMD_GET_DR_PLAYSTATUS,
                                                                            data,
                                                                            reply,
                                                                            true
                                                                            );
    }else
        ret = ProcGetDrPlayStatus(data,reply);
    return ret;    
}

bool IapService::Util_IsInvalidIndex(long index)
{
    bool ret = false;
    if( index < 0 || index == 0x7FFFFFFF)
        ret = true;
    return ret;
}

int IapService::UtilGetCurrentTrackIndex(long *currentIndex)
{
    long nCurrentIndex = 0;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    
    if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED)
        eCMDErrorCode = IPOD_GetCurrentPlayingTrackIndex( 
                                                                                GetIPodHandle(),
                                                                                &nCurrentIndex 
                                                                                );
    else
        eCMDErrorCode = IPOD_DISCONNECT_ERROR;

    IAPSVC_DBUG("%s() IPOD_GetCurrentPlayingTrackIndex() > eCMDErrorCode 0x%x nCurrentIndex=%ld.", 
                                                                                                            __FUNCTION__,
                                                                                                            eCMDErrorCode,
                                                                                                            nCurrentIndex
                                                                                                            );
    
    if ( IS_CMDOK(eCMDErrorCode) ){
        *currentIndex = nCurrentIndex;
        IPOD_Sample_SetCurrentIndex(GetIPodHandle(),nCurrentIndex);
        if(Util_IsInvalidIndex(nCurrentIndex)){
            ALOGE("%s() IPOD_GetCurrentPlayingTrackIndexe CMDErrorCode 0x%x nIndex (%ld) error",  __FUNCTION__,eCMDErrorCode,nCurrentIndex);
            eCMDErrorCode = IPOD_UNKNOWN_ERROR;
        }
    }else{
        ALOGE("%s() IPOD_GetCurrentPlayingTrackIndexe failed CMDErrorCode 0x%x nIndex (%ld) error",  __FUNCTION__,eCMDErrorCode,nCurrentIndex);
    }
    return eCMDErrorCode;
}

int IapService::UtilGetDrPlayStatus(void *pData)
{
    IPOD_DR_PLAYSTATUS *playStatus = (IPOD_DR_PLAYSTATUS *)pData;
    IPOD_ERROR ret = IPOD_UNKNOWN_ERROR;
    if ( IsInitialized() ) {
        if(IsValidPlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_PLAYINFO)){
            if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED){
                ret = IPOD_DR_GetPlayStatus(GetIPodHandle(), playStatus);
                IAPSVC_DBUG("%d: %s() ret = %d status(%d), %ld/%ld ms", 
                                                                            __LINE__,
                                                                            __FUNCTION__,
                                                                            ret, 
                                                                            playStatus->m_Status,
                                                                            playStatus->m_TrackPos_ms,
                                                                            playStatus->m_TrackTotalLen_ms
                                                                            );    
                if(IS_CMDOK(ret)){
                    SetSvcApplePlayStatus(playStatus->m_Status);
                    IPOD_Sample_SetDuration(GetIPodHandle(),playStatus->m_TrackTotalLen_ms);
                }else{
                    ALOGE("%d: %s()  IPOD_DR_GetPlayStatus error 0x%x.", __LINE__, __FUNCTION__,ret);
                }
            }else{
                ALOGE("%d: %s()  USB disconnected.", __LINE__, __FUNCTION__);
            }
            UpdatePlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_PLAYINFO);
        }else{
            ALOGE("%d: %s() ATS Time fail.", __LINE__, __FUNCTION__);
        }
    }else{
        ret = IPOD_INVALID_HANDLE;
        ALOGE("%d: %s()  Invalid state or invalid message.", __LINE__, __FUNCTION__);
    }

    return (int)ret;
}

bool IapService::ProcGetPlayStatusInfo(const Parcel *data,Parcel *reply)
{
    IPOD_ERROR ret = IPOD_NO_ERROR;
    IPOD_DR_PLAYSTATUS playStatus;
    if ( IsInitialized() ) {
        ret = (IPOD_ERROR)UtilGetDrPlayStatus(&playStatus);
        IAPSVC_DEFLOG("%d: %s() ret = %d status(%d), %ld/%ld ms", 
                                                                            __LINE__,
                                                                            __FUNCTION__,
                                                                            ret, 
                                                                            playStatus.m_Status,
                                                                            playStatus.m_TrackPos_ms,
                                                                            playStatus.m_TrackTotalLen_ms
                                                                            );    
        if(ret == IPOD_NO_ERROR){
            reply->writeInt32(true);
            reply->writeInt64(playStatus.m_TrackIndex);
            reply->writeInt32(playStatus.m_Status);
            reply->writeInt64(playStatus.m_TrackTotalLen_ms);
            reply->writeInt64(playStatus.m_TrackPos_ms);
            return true;
        }else{
            reply->writeInt32(false);
            reply->writeInt64(0);
            reply->writeInt32(0);
            reply->writeInt64(0);
            reply->writeInt64(0);
        }
    }else{
        ALOGE("%d: %s()  Invalid state or invalid message.", __LINE__, __FUNCTION__);
        reply->writeInt32(false);
    }
    return false;
}

bool IapService::GetPlayStatusInfo(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GET_PLAYSTATUS_INFO,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetPlayStatusInfo(data,reply);
    return ret;    
}

bool IapService::ProcToggleRepeat(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IPOD_ERROR Err;
    CurrentRepeatState repeat;
    if(IsValidPlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_REPEAT) == true){
        Err = IPOD_GetRepeat(GetIPodHandle(), &repeat);
        UpdatePlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_REPEAT);
    }
    IAPSVC_DEFLOG("%d %s repeat(0x%x)",__LINE__,__FUNCTION__,repeat );
    
    if (IPOD_NO_ERROR == ret)
    {
        switch(repeat){
            case REPEATONETRACK:
                repeat = REPEATALLTRACKS;
                break;
            case REPEATALLTRACKS:
                repeat = REPEATONETRACK;
                {
                    ShuffleMode shuffle;
                    if(IsValidPlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_SHUFFLE) == true){
                        Err = IPOD_GetShuffle(GetIPodHandle(), &shuffle);
                        UpdatePlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_SHUFFLE);

                        if ( IS_CMDOK(Err) ){
                            if(shuffle == SHUFFLETRACKS){
                                Err = IPOD_SetShuffle(
                                                                GetIPodHandle(),
                                                                SHUFFLEOFF,
                                                                RESTORE_ORG_SETTING
                                                                );
                            }
                        }
                    }
                }                
                break;
            default:
                repeat = REPEATALLTRACKS;
                break;
        }
        Err = IPOD_SetRepeat(GetIPodHandle(), repeat, RESTORE_ORG_SETTING);
        if(Err == IPOD_NO_ERROR)
            ret = true;
    }

    if(reply != NULL)
        reply->writeInt32(ret);
    return ret;
}

bool IapService::ToggleRepeat(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_TOGGLE_REPEAT,
                                                                        data,
                                                                        NULL,
                                                                        true
                                                                        );
    }else
        ret = ProcToggleRepeat(data,NULL);
    return ret;    
}

bool IapService::ProcToggleShuffle(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IPOD_ERROR Err = IPOD_UNKNOWN_ERROR;
    ShuffleMode shuffle;
    if(IsValidPlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_SHUFFLE) == true){
        Err = IPOD_GetShuffle(GetIPodHandle(), &shuffle);
        UpdatePlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_SHUFFLE);
    }

    IAPSVC_DEFLOG("%d %s shuffle(0x%x)",__LINE__,__FUNCTION__,shuffle );    
    if (IPOD_NO_ERROR == ret)
    {
        switch(shuffle){
            case SHUFFLEOFF:
                shuffle = SHUFFLETRACKS;
                {
                    CurrentRepeatState repeat;
                    if(IsValidPlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_REPEAT) == true){
                        Err = IPOD_GetRepeat(GetIPodHandle(), &repeat);
                        UpdatePlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_REPEAT);
                        if ( IS_CMDOK(Err) ){
                            if(repeat == REPEATONETRACK){
                                Err = IPOD_SetRepeat(
                                                                    GetIPodHandle(),
                                                                    REPEATALLTRACKS,
                                                                    RESTORE_ORG_SETTING
                                                                    );
                            }
                        }
                    }

                }                break;
            case SHUFFLETRACKS:
                shuffle = SHUFFLEOFF;
                break;
            default:
                shuffle = SHUFFLEOFF;
                break;            
        }
        Err = IPOD_SetShuffle(GetIPodHandle(), shuffle, RESTORE_ORG_SETTING);
        if(Err == IPOD_NO_ERROR)
            ret = true;
    }
    if(reply != NULL)
        reply->writeInt32(ret);
    return ret;
}

bool IapService::ToggleShuffle(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_TOGGLE_SHUFFLE,
                                                                        data,
                                                                        NULL,
                                                                        true
                                                                        );
    }else
        ret = ProcToggleShuffle(data,NULL);
    return ret;    
}

bool IapService::ProcGetTrackInfos(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    long nIndex = -1;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    
     IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);

    if ( IsInitialized() ) {

        if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED)
            eCMDErrorCode = IPOD_Sample_GetCurrentIndex(GetIPodHandle(), &nIndex);
        else
            eCMDErrorCode = IPOD_DISCONNECT_ERROR;
            
         if(Util_IsInvalidIndex(nIndex)){
            long nCurrentIndex = 0;
            eCMDErrorCode = (IPOD_ERROR)UtilGetCurrentTrackIndex(&nCurrentIndex);
            
            if ( IS_CMDOK(eCMDErrorCode) ){
                nIndex = nCurrentIndex;
            }else{
                reply->writeInt32(false);
                return ret;
            }
        }

        long nStringLen = 0;
        unsigned char strName[UI_MAX_STRING_LEN] = {0,};
        String16 title = String16("");
        String16 artist = String16("");
        String16 album = String16("");
        
        if(nIndex >= 0){
            int i;
            bool fCheckSumError = true;
            for(i =0 ; i < 3 && fCheckSumError == true;i++){
                fCheckSumError = false;
                if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED)
                    eCMDErrorCode = IPOD_GetIndexedPlayingTrackTitle(
                                                                                    GetIPodHandle(),
                                                                                    nIndex,
                                                                                    &strName[0],
                                                                                    UI_MAX_STRING_LEN,
                                                                                    &nStringLen
                                                                                    );
                else{
                    eCMDErrorCode = IPOD_DISCONNECT_ERROR;
                    break;
                }

                if(eCMDErrorCode == IPOD_IO_CHECKSUM_ERROR)
                    fCheckSumError = true;                
                
                if ( IS_CMDOK(eCMDErrorCode) ){
                    title = String16(String8((const char*)strName));
                    IAPSVC_DEFLOG("%d: %s() index[%ld] title %s", 
                                                        __LINE__, 
                                                        __FUNCTION__,
                                                        nIndex,
                                                        String8(title).string()
                                                        );
                }
                else{
                    ALOGE(" %s IPOD_GetIndexedPlayingTrackTitle() > Failed(0x%x). nIndex(%ld) ",
                                                                                                        __FUNCTION__,
                                                                                                        eCMDErrorCode,nIndex
                                                                                                        );
                }

                if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED)
                    eCMDErrorCode = IPOD_GetIndexedPlayingTrackArtistName(
                                                                                                GetIPodHandle(),
                                                                                                nIndex,
                                                                                                &strName[0],
                                                                                                UI_MAX_STRING_LEN,
                                                                                                &nStringLen 
                                                                                                );
                else{
                    eCMDErrorCode = IPOD_DISCONNECT_ERROR;
                    break;
                }

                if(eCMDErrorCode == IPOD_IO_CHECKSUM_ERROR)
                    fCheckSumError = true;                
                
                if ( IS_CMDOK(eCMDErrorCode) ){
                    artist = String16(String8((const char*)strName));
                    IAPSVC_DEFLOG("%d: %s() index[%ld] artist %s ",
                                                            __LINE__, 
                                                            __FUNCTION__,
                                                            nIndex,
                                                            String8(artist).string()
                                                            );
                }
                else{
                    ALOGE(" %s IPOD_GetIndexedPlayingTrackArtistName() > Failed(0x%x). nIndex(%ld) ",
                                                                                                        __FUNCTION__,
                                                                                                        eCMDErrorCode,
                                                                                                        nIndex
                                                                                                        );
                }        

                if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED)
                    eCMDErrorCode = IPOD_GetIndexedPlayingTrackAblumName(
                                                                                                GetIPodHandle(),
                                                                                                nIndex,
                                                                                                &strName[0],
                                                                                                UI_MAX_STRING_LEN,
                                                                                                &nStringLen 
                                                                                                );
                else{
                    eCMDErrorCode = IPOD_DISCONNECT_ERROR;
                    break;
                }

                if(eCMDErrorCode == IPOD_IO_CHECKSUM_ERROR)
                    fCheckSumError = true;                

                if ( IS_CMDOK(eCMDErrorCode) ){
                    if(nStringLen <= DUMMY_LEN){
                        album = String16("Unknown Album");
                    }else{
                        album = String16(String8((const char*)strName));
                    }
                    IAPSVC_DEFLOG("%d: %s() index[%ld] album %s", 
                                                        __LINE__, 
                                                        __FUNCTION__,
                                                        nIndex,
                                                        String8(album).string()
                                                        );
                }
                else{
                    ALOGE(" %s IPOD_GetIndexedPlayingTrackAblumName() > Failed(0x%x). nIndex(%ld) ",
                                                                                                                    __FUNCTION__,
                                                                                                                    eCMDErrorCode,
                                                                                                                    nIndex
                                                                                                                    );
                }     
                if(fCheckSumError){
                    //ALOGE(" \x1b[1;31m %s checksum erro retry [%d] \x1b[1;0m ",__FUNCTION__,i);
                    ALOGE(" %s checksum erro retry [%d] ",__FUNCTION__,i);
                }
            }
        }
        
        reply->writeInt32(true);
        reply->writeInt64(nIndex);
        reply->writeString16(title);
        reply->writeString16(artist);
        reply->writeString16(album);

        long nTotalTrackNum = 0;
        if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED)
            eCMDErrorCode = IPOD_GetNumPlayingTracks( GetIPodHandle(), &nTotalTrackNum );
        else
            eCMDErrorCode = IPOD_DISCONNECT_ERROR;
            
        if ( IS_CMDOK(eCMDErrorCode) ){
            reply->writeInt64(nTotalTrackNum);
        }
        else{
            reply->writeInt64(0);
            ALOGE(" %s IPOD_GetNumPlayingTracks() > Failed(0x%x).",
                                                                                        __FUNCTION__,
                                                                                        eCMDErrorCode
                                                                                        );
        }

        IPOD_ERROR status = IPOD_UNKNOWN_ERROR;
        IPOD_DR_PLAYSTATUS playStatus;
        status = (IPOD_ERROR)UtilGetDrPlayStatus(&playStatus);
        IAPSVC_DEFLOG("%d: %s() ret = %d status(%d), %ld/%ld ms", 
                                                                            __LINE__,
                                                                            __FUNCTION__,
                                                                            ret, 
                                                                            playStatus.m_Status,
                                                                            playStatus.m_TrackPos_ms,
                                                                            playStatus.m_TrackTotalLen_ms
                                                                            );    
        
        if(status == IPOD_NO_ERROR){
            reply->writeInt64(playStatus.m_TrackPos_ms);
            reply->writeInt64(playStatus.m_TrackTotalLen_ms);
        }else{
            unsigned long dutation = 0;
            unsigned long position = 0;
            IPOD_Sample_GetDuration(GetIPodHandle(),&dutation);
            IPOD_Sample_GetPosition(GetIPodHandle(),&position);
            reply->writeInt64(position);
            reply->writeInt64(dutation);    
        }
    }else{
        ALOGE("%d: %s()  Invalid state or invalid message.", __LINE__, __FUNCTION__);
        reply->writeInt32(false);
        reply->writeInt64(nIndex);
    }
    ret = true;
     IAPSVC_DEFLOG("%d: %s() end nIndex %ld", __LINE__, __FUNCTION__,nIndex);
    return ret;
}

bool IapService::GetTrackInfos(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                    IAP_CMD_GET_TRACKINFO,
                                                                    data,
                                                                    reply,
                                                                    true
                                                                    );
    }else
        ret = ProcGetTrackInfos(data,reply);
    return ret;    
}

bool IapService::ProcGetTrackMetas(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    bool result = true;
    IPOD_ERROR Err;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    long nIndex = data->readInt64();
    IAPSVC_DEFLOG("%d: %s() nIndex (%ld)", __LINE__, __FUNCTION__,nIndex);
    if ( IsInitialized() ) {
        long nStringLen = 0;
        unsigned char strName[UI_MAX_STRING_LEN] = {0,};
        String16 title = String16("");
        String16 artist = String16("");
        String16 album = String16("");
        
        if(nIndex >= 0){
            if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED)
		
                eCMDErrorCode = IPOD_GetIndexedPlayingTrackTitle(
                                                                                GetIPodHandle(),
                                                                                nIndex,
                                                                                &strName[0],
                                                                                UI_MAX_STRING_LEN,
                                                                                &nStringLen
                                                                                );
            else
                eCMDErrorCode = IPOD_DISCONNECT_ERROR;
            
            if ( IS_CMDOK(eCMDErrorCode) ){
                title = String16((const char*)strName);
                IAPSVC_DEFLOG("%d: %s() index[%ld] title %s ", 
                                            __LINE__, 
                                            __FUNCTION__,
                                            nIndex,
                                            String8(title).string()
                                            );
            }
            else{
                ALOGE(" %s IPOD_GetIndexedPlayingTrackTitle() > Failed(0x%x). nIndex(%ld) ",
                                                                                                            __FUNCTION__,
                                                                                                            eCMDErrorCode,
                                                                                                            nIndex
                                                                                                            );
                result= false;
            }

            if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED)
                eCMDErrorCode = IPOD_GetIndexedPlayingTrackArtistName(
                                                                                            GetIPodHandle(),
                                                                                            nIndex,
                                                                                            &strName[0],
                                                                                            UI_MAX_STRING_LEN,
                                                                                            &nStringLen 
                                                                                            );
            else
                eCMDErrorCode = IPOD_DISCONNECT_ERROR;
            
            if ( IS_CMDOK(eCMDErrorCode) ){
                artist = String16((const char*)strName);
                IAPSVC_DEFLOG("%d: %s() index[%ld] artist %s", 
                                                        __LINE__, 
                                                        __FUNCTION__,
                                                        nIndex,
                                                        String8(artist).string()
                                                        );
            }
            else{
                ALOGE(" %s IPOD_GetIndexedPlayingTrackArtistName() > Failed(0x%x). nIndex(%ld) ",
                                                                                                                        __FUNCTION__,
                                                                                                                        eCMDErrorCode,
                                                                                                                        nIndex
                                                                                                                        );     
                result= false;
            }        

            if( IPOD_GetDetectStatus(IPOD_GetExtInfo(GetIPodHandle())) == IPOD_CONNECTED)
                eCMDErrorCode = IPOD_GetIndexedPlayingTrackAblumName(
                                                                                            GetIPodHandle(),
                                                                                            nIndex,
                                                                                            &strName[0],
                                                                                            UI_MAX_STRING_LEN,
                                                                                            &nStringLen 
                                                                                            );
            else
                eCMDErrorCode = IPOD_DISCONNECT_ERROR;

            if ( IS_CMDOK(eCMDErrorCode) ){
                if(nStringLen <= DUMMY_LEN){
                    album = String16("Unknown album");
                }else{
                    album = String16((const char*)strName);
                }
                IAPSVC_DEFLOG("%d: %s() index[%ld] album %s", 
                                                        __LINE__, 
                                                        __FUNCTION__,
                                                        nIndex,
                                                        String8(album).string()
                                                        );
            }
            else{
                ALOGE(" %s IPOD_GetIndexedPlayingTrackAblumName() > Failed(0x%x). nIndex(%ld) ",
                                                                                                        __FUNCTION__,
                                                                                                        eCMDErrorCode,
                                                                                                        nIndex
                                                                                                        );
                result= false;
            }        
        }
        
        if(result == true){
            reply->writeInt32(true);
            reply->writeInt64(nIndex);
            reply->writeString16(title);
            reply->writeString16(artist);
            reply->writeString16(album);
        }else{
            reply->writeInt32(false);
            reply->writeInt64(nIndex);
        }
    }else{
        ALOGE("%d: %s()  Invalid state or invalid message.", __LINE__, __FUNCTION__);
        reply->writeInt32(false);
        reply->writeInt64(nIndex);
    }
     IAPSVC_DBUG("%d: %s() end", __LINE__, __FUNCTION__);
    return ret;
}

bool IapService::GetTrackMetas(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_GET_TRACKMETA,data,reply,true);
    else
        ret = ProcGetTrackMetas(data,reply);
    return ret;    
}

bool IapService::ProcDisconnected()
{
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    IPOD_DR_PLAY_STATUS playStatus;

    IPOD_SetDisconnectionStatus(GetIPodHandle());
    pauseAudio();
   
    return true;
}

bool IapService::setDisconnected(bool fCausedByHw)
{
    bool ret = false;
    IAPSVC_DEFLOG("%d: %s() fCausedByHw %d ", __LINE__, __FUNCTION__,fCausedByHw);
    SetUsbConnected(false);
    MsgFlush();
    //if(IsUsedAsync())
    //    ret = mIapCommandThread->sendCommand(IAP_CMD_DISCONNECTED,NULL,false);
    //else
        ret = ProcDisconnected();
    stopAudio();
    IAPSVC_DEFLOG("%d: %s() end", __LINE__, __FUNCTION__);
    return ret;
}

bool IapService::Disconnected()
{
    bool ret = false;
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    SetSvcState(IAP_SVC_STATE_DISCONNECT);
    setDisconnected(true);
    return ret;
}

bool IapService::ProcSetCurrentPlayingTrack(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    long nIndex = data->readInt64();

    IAPSVC_DEFLOG("%d: %s() nIndex %ld ", __LINE__, __FUNCTION__,nIndex);
    if ( IsInitialized() ) {

        eCMDErrorCode = IPOD_SetCurrentPlayingTrack( GetIPodHandle(), nIndex );
        if ( IS_CMDOK(eCMDErrorCode) ){
            IPOD_DR_PLAY_STATUS playStatus = IPOD_DR_PLAYBACK_STOPPED;
            if(Sample_GetDRPlayStatus((void *)&playStatus))
            {
                IAPSVC_DBUG("%d: %s() playStatus(%d) ",
                                                                    __LINE__,
                                                                    __FUNCTION__,
                                                                    playStatus
                                                                    );
                if (IPOD_DR_PLAYING != playStatus){
                    SetSvcLocalPlayStatus(IPOD_DR_PLAYING);
                    //IPOD_Sample_SetDRPlayStatus(GetIPodHandle(), IPOD_DR_PLAYING);
                }
            }            
            IsSuccess = true;
        }
        else{
            ALOGE(" %s IPOD_SetCurrentPlayingTrack() > Failed(0x%x) nIndex %ld.",
                                                                                __FUNCTION__, 
                                                                                eCMDErrorCode,
                                                                                nIndex
                                                                                );
        }

    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    if(reply != NULL)
        reply->writeInt32(IsSuccess);
    return IsSuccess;
}

bool IapService::SetCurrentPlayingTrack(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                            IAP_CMD_SET_CURRENT_PLAYINGTRACK,
                                                                            data,
                                                                            NULL,
                                                                            true
                                                                            );
    }else
        ret = ProcSetCurrentPlayingTrack(data,NULL);
    return ret;    
}


bool IapService::ProcSetRepeat(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    CurrentRepeatState mode = (CurrentRepeatState)data->readInt32();

    IAPSVC_DEFLOG("%d: %s() mode %d", __LINE__, __FUNCTION__,mode);
    if ( IsInitialized() && mode >= REPEATOFF && mode <= REPEATALLTRACKS) {

        eCMDErrorCode = IPOD_SetRepeat(GetIPodHandle(), mode, RESTORE_ORG_SETTING);
        if(eCMDErrorCode == IPOD_NO_ERROR){
            IsSuccess = true;
        }
        else{
            ALOGE(" %s IPOD_SetRepeat() > Failed(0x%x).",__FUNCTION__, eCMDErrorCode);
        }

    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    if(reply != NULL)
        reply->writeInt32(IsSuccess);
    return IsSuccess;
}

bool IapService::SetRepeat(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s()", __LINE__, __FUNCTION__);
    
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_SET_REPEAT,data,NULL,true);
    else
        ret = ProcSetRepeat(data,NULL);
    return ret;    
}

bool IapService::ProcGetRepeat(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    CurrentRepeatState mode = REPEATOFF;
    
    if ( IsInitialized() ) {      

        if(IsValidPlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_REPEAT) == true){
            eCMDErrorCode = IPOD_GetRepeat(GetIPodHandle(), &mode);
            UpdatePlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_REPEAT);
            if(eCMDErrorCode == IPOD_NO_ERROR){
                IsSuccess = true;
                IAPSVC_DBUG("%d: %s() mode %d ", __LINE__, __FUNCTION__,mode);
            }
            else{
                ALOGE(" %s IPOD_GetRepeat() > Failed(0x%x).",__FUNCTION__, eCMDErrorCode);
            }
        }

    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    reply->writeInt32(IsSuccess);
    if(IsSuccess == false){
        reply->writeInt32(0);
    }else{
        reply->writeInt32(mode);
    }
    return IsSuccess;
}

bool IapService::GetRepeat(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_GET_REPEAT,data,reply,true);
    else
        ret = ProcGetRepeat(data,reply);
    return ret;    
}

bool IapService::ProcSetShuffle(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    ShuffleMode mode = (ShuffleMode)data->readInt32();
    IAPSVC_DEFLOG("%d: %s() mode %d ", __LINE__, __FUNCTION__,mode);
    if ( IsInitialized() && mode >= SHUFFLEOFF && mode <= SHUFFLEALBUMS) {

        eCMDErrorCode = IPOD_SetShuffle(GetIPodHandle(), mode, RESTORE_ORG_SETTING);
        if(eCMDErrorCode == IPOD_NO_ERROR){
            IsSuccess = true;
        }
        else{
            ALOGE(" %s IPOD_SetShuffle() > Failed(0x%x) mode %d.",__FUNCTION__, eCMDErrorCode,mode);
        }

    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    if(reply != NULL)
        reply->writeInt32(IsSuccess);
    return IsSuccess;
}

bool IapService::SetShuffle(const Parcel *data)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s()", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_SET_SHUFFLE,data,NULL,true);
    else
        ret = ProcSetShuffle(data,NULL);
    return ret;    
}

bool IapService::ProcGetShuffle(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    ShuffleMode mode = SHUFFLEOFF;

    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        if(IsValidPlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_SHUFFLE) == true){
            eCMDErrorCode = IPOD_GetShuffle(GetIPodHandle(), &mode);
            UpdatePlayInfoTimeSchedule(IPOD_TIME_SCHEDULE_SHUFFLE);
            if(eCMDErrorCode == IPOD_NO_ERROR){
                IsSuccess = true;
                IAPSVC_DEFLOG("%d: %s() mode %d", __LINE__, __FUNCTION__,mode);
            }
            else{
                ALOGE(" %s IPOD_GetShuffle() > Failed(0x%x).",__FUNCTION__, eCMDErrorCode);
            }
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    reply->writeInt32(IsSuccess);
    if(IsSuccess == false){
        reply->writeInt32(0);
    }else{
        reply->writeInt32(mode);
    }
    return IsSuccess;
}

bool IapService::GetShuffle(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_GET_SHUFFLE,data,reply,true);
    else
        ret = ProcGetShuffle(data,reply);
    return ret;    
}

bool IapService::ProcGetIndexedPlayingInfo(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    TrackInformationType type = (TrackInformationType) data->readInt32();
    long trackIndex = data->readInt64();
    long chapterIndex = data->readInt64();    

    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized()  && type == CAPAINFO) {
        Info_Indexed_Track info;
        long actualLen = 0;
        int bufLen = 1024;
        char * InfoString;
        InfoString = (char *)malloc( bufLen );
        info.InfoString = InfoString;

        eCMDErrorCode = IPOD_GetIndexedPlayingTrackInfo(
                                                                                    GetIPodHandle(),  
                                                                                    type, 
                                                                                    &info, 
                                                                                    trackIndex, 
                                                                                    chapterIndex, 
                                                                                    bufLen, 
                                                                                    &actualLen
                                                                                    );
        if(eCMDErrorCode == IPOD_NO_ERROR){
            IsSuccess = true;
            reply->writeInt32(IsSuccess);
            switch(type){
                case CAPAINFO:
                    reply->writeInt32(info.Capability);
                    reply->writeInt64(info.TotalLength);
                    reply->writeInt32(info.ChapterCount);
                    IAPSVC_DEFLOG("%d: %s()  cap 0x%lx len %ld", __LINE__, __FUNCTION__,info.Capability,info.TotalLength);
                    break;
                default:
                    break;
            }
        }
        else{
            ALOGE(" %s IPOD_GetIndexedPlayingTrackInfo() > Failed(0x%x) trackIndex %ld.",
                                                                                    __FUNCTION__, 
                                                                                    eCMDErrorCode,
                                                                                    trackIndex);
        }

        if(InfoString)
            free(InfoString);

    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    if(IsSuccess == false){
        switch(type){
            case CAPAINFO:
                reply->writeInt32(IsSuccess);
                reply->writeInt32(0);
                reply->writeInt64(0);
                reply->writeInt32(0);
                break;
            default:
                reply->writeInt32(IsSuccess);
                break;                
        }
    }
    return IsSuccess;
}

bool IapService::GetIndexedPlayingInfo(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(
                                                                IAP_CMD_GET_INDEXEDPLAYINGINFO,
                                                                data,
                                                                reply,
                                                                true
                                                                );
    else
        ret = ProcGetIndexedPlayingInfo(data,reply);
    return ret;    
}

bool IapService::ProcGetModelNum(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;

    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        long model_num = 0;
        long BufLen = 1024;
        long ActualLen = 0;
        unsigned char *ModelStr = (unsigned char *)malloc( BufLen );
        if(ModelStr){
            memset(ModelStr,0x00,BufLen);
            eCMDErrorCode = IPOD_RequestiPodModelNum(
                                                                                GetIPodHandle(),
                                                                                &model_num, 
                                                                                ModelStr, 
                                                                                BufLen, 
                                                                                &ActualLen
                                                                                );
            if(eCMDErrorCode == IPOD_NO_ERROR){
                String8 modelNum8 = String8((const char*)ModelStr);
                String16 modelNum16 = String16(modelNum8);
                String8 modelNum16_8 = String8(modelNum16);
                IsSuccess = true;
                reply->writeInt32(IsSuccess);
                reply->writeInt64(model_num);
                reply->writeString16(String16(modelNum8));
                IAPSVC_DEFLOG("%d: %s() ModelStr %s modelNum16_8 %s", 
                                                                                __LINE__, 
                                                                                __FUNCTION__,
                                                                                modelNum8.string(),
                                                                                modelNum16_8.string()
                                                                                );
            }
            else{
                ALOGE(" %s IPOD_RequestiPodModelNum() > Failed(0x%x).",__FUNCTION__, eCMDErrorCode);
            }

            if(ModelStr)
                free(ModelStr);
        }

    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    if(IsSuccess == false){
        reply->writeInt32(IsSuccess);
        reply->writeInt64(0);
        reply->writeString16(String16(""));
    }
    return IsSuccess;
}

bool IapService::GetModelNum(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GET_MODEL_NUM,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetModelNum(data,reply);
    return ret;    
}

bool IapService::ProcGetProtocolVersion(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;

    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        long major = 0;
        long minor = 0;
        
        eCMDErrorCode = IPOD_RequestProtocolVersion(GetIPodHandle(),&major, &minor);
        if(eCMDErrorCode == IPOD_NO_ERROR){
            IsSuccess = true;
            reply->writeInt32(IsSuccess);
            reply->writeInt64(major);
            reply->writeInt64(minor);
            IAPSVC_DEFLOG("%d: %s() ver[%ld:%ld] ", __LINE__, __FUNCTION__,major,minor);
        }
        else{
            ALOGE(" %s IPOD_RequestProtocolVersion() > Failed(0x%x).",
                                                                        __FUNCTION__,
                                                                        eCMDErrorCode
                                                                        );
        }

    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    if(IsSuccess == false){
        reply->writeInt32(IsSuccess);
        reply->writeInt64(0);
        reply->writeInt64(0);
    }
    return IsSuccess;
}

bool IapService::GetProtocolVersion(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                    IAP_CMD_GET_PROTOCOL_VESION,
                                                                    data,
                                                                    reply,
                                                                    true
                                                                    );
    }else
        ret = ProcGetProtocolVersion(data,reply);
    return ret;    
}

bool IapService::ProcGetIpodSerialNumber(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;

    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        long ActualLen = 0;
        long BufLen = 1024;
        unsigned char *SerialStr = (unsigned char *)malloc( BufLen );

        if(SerialStr){
            memset(SerialStr,0x00,BufLen);

            eCMDErrorCode = IPOD_RequestiPodSerialNum(
                                                                    GetIPodHandle(),
                                                                    SerialStr,
                                                                    BufLen,
                                                                    &ActualLen
                                                                    );
            if(eCMDErrorCode == IPOD_NO_ERROR){
                IsSuccess = true;
                reply->writeInt32(IsSuccess);
                reply->writeString16(String16((const char*)SerialStr));
                IAPSVC_DEFLOG("%d: %s() SerialStr %s ActualLen %ld", 
                                                                                        __LINE__,
                                                                                        __FUNCTION__,
                                                                                        SerialStr,
                                                                                        ActualLen
                                                                                        );
            }
            else{
                ALOGE(" %s IPOD_RequestiPodSerialNum() > Failed(0x%x).",
                                                                                __FUNCTION__,
                                                                                eCMDErrorCode
                                                                                );
            }

            if(SerialStr)
                free(SerialStr);
        }

    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    if(IsSuccess == false){
        reply->writeInt32(IsSuccess);
        reply->writeInt64(0);
        reply->writeInt64(0);
    }
    return IsSuccess;
}

bool IapService::GetIpodSerialNumber(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                            IAP_CMD_GET_SERIAL_NUM,
                                                            data,
                                                            reply,
                                                            true
                                                            );
    }else
        ret = ProcGetIpodSerialNumber(data,reply);
    return ret;    
}

bool IapService::ProcGetIpodSoftwareVersion(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;

    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        long BufLen = 1024;
        unsigned char major = 0;
        unsigned char minor = 0;
        unsigned char revision = 0;
        char buf[32];
        int bufIndex = 0;
        memset(buf,0x00,sizeof(buf));
        eCMDErrorCode = IPOD_RequestiPodSoftwareVersion(
                                                                            GetIPodHandle(),
                                                                            &major,
                                                                            &minor,
                                                                            &revision
                                                                            );
        if(eCMDErrorCode == IPOD_NO_ERROR){
            bufIndex = sprintf(&buf[0],"v%d.%d.%d",major,minor,revision);

            IsSuccess = true;
            reply->writeInt32(IsSuccess);
            reply->writeString16(String16(buf));
            IAPSVC_DEFLOG("%d: %s() buf %s ", __LINE__, __FUNCTION__,buf);
        }
        else{
            ALOGE(" %s IPOD_RequestiPodSoftwareVersion() > Failed(0x%x).",
                                                                            __FUNCTION__,
                                                                            eCMDErrorCode
                                                                            );
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    if(IsSuccess == false){
        reply->writeInt32(IsSuccess);
        reply->writeString16(String16(String8("")));
    }
    return IsSuccess;
}

bool IapService::GetIpodSoftwareVersion(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_GET_SOFTWARE_VERSION,data,reply,true);
    else
        ret = ProcGetIpodSoftwareVersion(data,reply);
    return ret;    
}

bool IapService::ProcGetIpodName(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;

    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        long ActualLen = 0;
        long BufLen = 1024;
        unsigned char *iPodNameStr = (unsigned char *)malloc( BufLen );

        if(iPodNameStr){
            memset(iPodNameStr,0x00,BufLen);
            eCMDErrorCode = IPOD_RequestiPodName(
                                                                        GetIPodHandle(),
                                                                        iPodNameStr,
                                                                        BufLen,
                                                                        &ActualLen
                                                                        );
            if(eCMDErrorCode == IPOD_NO_ERROR){
                IsSuccess = true;
                reply->writeInt32(IsSuccess);
                reply->writeString16(String16((const char*)iPodNameStr));
            }
            else{
                ALOGE(" %s ProcGetIpodName() > Failed(0x%x).",__FUNCTION__, eCMDErrorCode);
            }

            if(iPodNameStr)
                free(iPodNameStr);
        }

    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    if(IsSuccess == false){
        reply->writeInt32(IsSuccess);
        reply->writeInt64(0);
        reply->writeInt64(0);
    }
    return IsSuccess;
}

bool IapService::GetIpodName(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_GET_IPOD_NAME,data,reply,true);
    else
        ret = ProcGetIpodName(data,reply);
    return ret;    
}

bool IapService::ProcResponseFailByListenerName(
                                                                                        IAP_CMD_DATA_TYPE *cmd,
                                                                                        IAP_RSP_DATA_TYPE *rsp,
                                                                                        const Parcel *cmdData,
                                                                                        Parcel *replyData,
                                                                                        String8 *pName8)

{
    int wrapCmd = cmdData->readInt32();
    String8 listenerName = cmdData->readString8();
    IAP_CMD_DATA_TYPE cmdNew;
    pName8->setTo(listenerName);
    cmdNew.what = wrapCmd;// & ~IAP_CMD_MASK;
    IAPSVC_DBUG("%d: %s() wrapCmd %d %s",__LINE__,__FUNCTION__,cmdNew.what,listenerName.string());
    rsp->what = wrapCmd & ~IAP_CMD_MASK;
    rsp->what |= IAP_RSP_MASK;    
    return false;
}

bool IapService::ProcResponseFailByProtocolName(
                                                                                            const Parcel *data,
                                                                                            Parcel *reply,
                                                                                            String8 *pName8)
{
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    String8 appName;
    String8 protocolName;
    int param = 1;
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    protocolName = String8(data->readString8());

    if(protocolName.size() > 0){
        pName8->setTo(protocolName);
    }
    return false;
}


bool IapService::ProcResponseFailBySessionId(const Parcel *data,Parcel *reply,unsigned long *pSessionId)
{
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;
    unsigned long sessionId = data->readInt64();
    ssize_t index = mEapSessionList.indexOfKey(sessionId);
    IAPSVC_DEFLOG("%d: %s() sessionId %ld index %d ", __LINE__, __FUNCTION__,sessionId,index);

    if(index >= 0){
        *pSessionId = sessionId;
    }
    return false;
}

bool IapService::ProcAccessoryDataTransfer(const Parcel *data,Parcel *reply,unsigned long *pSessionId)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;

    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        unsigned long sessionId = data->readInt64();
        ssize_t index = mEapSessionList.indexOfKey(sessionId);
        if(index >= 0){
            int DataLen = data->readInt32();
            unsigned char *pData = (unsigned char *)malloc( DataLen );
            *pSessionId = sessionId;
            IAPSVC_DEFLOG("%d: %s() dataLen %d ", __LINE__, __FUNCTION__,DataLen);
            
            if(pData){
                if(data->read(pData,DataLen) == 0){       
                    eCMDErrorCode = IPOD_Sample_Transfer_Accessory_to_iOSApp(
                                                                                                            GetIPodHandle(),
                                                                                                            pData,
                                                                                                            DataLen,
                                                                                                            sessionId
                                                                                                            );

                    if(eCMDErrorCode == IPOD_NO_ERROR){
                        IsSuccess = true;
                        reply->writeInt32(IsSuccess);
                    }
                    else{
                        ALOGE(" %s IPOD_Sample_Transfer_Accessory_to_iOSApp() > Failed(0x%x) sess %ld len %d.",
                                                                                                            __FUNCTION__,
                                                                                                            eCMDErrorCode,
                                                                                                            sessionId,
                                                                                                            DataLen
                                                                                                            );
                    }

                }

                if(pData)
                    free(pData);
            }
        }else{
            ALOGE(" %s Invalid session Id %d.",__FUNCTION__,(int)sessionId);
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }

    if(IsSuccess == false){
        reply->writeInt32(IsSuccess);
    }
    return IsSuccess;
}

bool IapService::AccessoryDataTransfer(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    unsigned long sessionId = 0;
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_ACCESSORYDATATRANSFER,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcAccessoryDataTransfer(data,reply,&sessionId);
    return ret;    
}

bool IapService::ProcApplicationLaunch(const Parcel *data,Parcel *reply,String8 *pName8)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;

    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        const String8 appName = String8(data->readString16());
        const String8 protocolName = String8(data->readString16());
        int param = 1;
        if(data->dataPosition() < data->dataSize()){
            param = data->readInt32();
            IAPSVC_DEFLOG("%d: %s() param %d", __LINE__, __FUNCTION__,param);
        }

        IAPSVC_DEFLOG(" appName %s %d ", appName.string(),appName.length());
        IAPSVC_DEFLOG(" protocolName %s %d",protocolName.string(),protocolName.length());
        if(param != 1 && param != 2){
            param = 1;
        }
        if(protocolName.size() > 0){
            pName8->setTo(protocolName);
        }
        if(appName.size() > 0){
            const char *cAppName = appName.string();
            int dataLen = strlen(cAppName);
            eCMDErrorCode = IPOD_RequestApplicationLaunch(
                                                                            GetIPodHandle(),
                                                                            (unsigned char *)cAppName,
                                                                            dataLen + 1,
                                                                            param);
            if(eCMDErrorCode == IPOD_NO_ERROR){
                IsSuccess = true;
            }
            else{
                ALOGE(" %s IPOD_RequestApplicationLaunch() > Failed(0x%x) [%s].",
                                                                                        __FUNCTION__,
                                                                                        eCMDErrorCode,
                                                                                        cAppName
                                                                                        );
            }
        }

    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    if(reply != NULL){
        reply->writeInt32(IsSuccess);    
        reply->writeInt32(eCMDErrorCode);
    }
    return IsSuccess;
}

bool IapService::ApplicationLaunch(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    String8 protocolName;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_APPLICATIONLAUNCH,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcApplicationLaunch(data,reply,&protocolName);
    return ret;    
}

bool IapService::ProcSendPackedLingoCmd(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;

    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        IPOD_ERROR ret = IPOD_NO_ERROR;
        IPOD_CMD_PARAM CmdParam;
        unsigned char *buff = IPOD_GetTxBuffer(GetIPodHandle()); /*Please use this function to get a Txbuffer */
        unsigned char *pData = NULL;
        unsigned long DataSize = 0;
        ret = IPOD_USER_EnterCriticalSection(GetIPodHandle());
        IPOD_MemSet((void *)&CmdParam, 0x00, sizeof(IPOD_CMD_PARAM));

        if (IPOD_NO_ERROR == ret)
        {
            if (NULL != buff)
            {
                int lingo = data->readInt32();
                int command = data->readInt32();
                int retCommand = data->readInt32();
                int DataLen = data->readInt32();
                unsigned char *pData = NULL;
                int rxBuffSize = 32;
                unsigned char *buffRx = (unsigned char *)malloc(rxBuffSize);
                IAPSVC_DBUG("%d: %s() lingo 0x%x cmd 0x%x ret = 0x%x", 
                                                                                __LINE__,
                                                                                __FUNCTION__,
                                                                                lingo,
                                                                                command,
                                                                                retCommand
                                                                                );
                if(DataLen > 0)
                    pData = (unsigned char *)malloc( DataLen);
                if(pData || DataLen == 0){
                    if(data->read(pData,DataLen) == 0){      
                        int cnt = 0;
                        buff[cnt++] = (unsigned char)lingo; 
                        if(lingo != LINGO_EXTENDED_INTERFACE){
                            buff[cnt++] = (unsigned char)command;
                        }else{
                            buff[cnt++] = 0;
                            buff[cnt++] = (unsigned char)command;
                        }
                        if(DataLen > 0)
                            memcpy(&buff[cnt],pData,DataLen);
                        CmdParam.m_iPod_Handle = GetIPodHandle();
                        CmdParam.m_CurCmd = command;
                        CmdParam.m_SendCmdBuf = buff;
                        CmdParam.m_SendCmdBufSize = DataLen + cnt; /*buffer size of User added*/
                        CmdParam.m_TimeOut = IPOD_Callback_GetWaitTime();
                        CmdParam.m_SendFunc = IPOD_SEND_ADD_HEADER;
                        CmdParam.m_Flag = IPOD_DATA_SINGLE_SESSION;
                        CmdParam.m_ResponseBuf = buffRx;
                        CmdParam.m_ResponseBufSize = rxBuffSize;                        
                        //Dump((const char *)buff,DataLen + 2);

                        ret= IPOD_USER_FUNC_SendCmd_RcvResponse(
                                                                            &CmdParam, 
                                                                            lingo, 
                                                                            retCommand
                                                                            );
                        IAPSVC_DBUG("%d: %s() ret = 0x%x", 
                                                                                __LINE__,
                                                                                __FUNCTION__,
                                                                                ret
                                                                                );
                    }

                    if (ret == IPOD_COMMAND_ACCEPTED)
                    {
                        IsSuccess = true;
                        DataSize = CmdParam.m_ResponseBufSize;
                        reply->writeInt32(IsSuccess);
                        reply->writeInt32((int)DataSize);
                        reply->writeInt32((int)DataSize);
                        reply->write(CmdParam.m_ResponseBuf,DataSize);    
                        //iPOD_DumpMemory(CmdParam.m_ResponseBuf,(int)DataSize);
                    }else{
                        reply->writeInt32(IsSuccess);
                        reply->writeInt32((int)0);
                        ALOGE(" %s lingo 0x%x cmd 0x%x rsp 0x%x ",
                                                                                            __FUNCTION__,
                                                                                            lingo,
                                                                                            command,
                                                                                            retCommand
                                                                                            );
                    }

                    if(pData != NULL)
                        free(pData);
                    if(buffRx != NULL)
                        free(buffRx);
                }
            }else{
                ALOGE(" %s Invalid tx buffer.",__FUNCTION__);
            }
            IPOD_USER_ExitCriticalSection(GetIPodHandle());
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }

    return IsSuccess;
}

bool IapService::SendPackedLingoCmd(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_SEND_PACKEDLINGOCMD,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcSendPackedLingoCmd(data,reply);
    return ret;    
}

bool IapService::ProcMuteControl(bool flag)
{
    bool IsSuccess = true;

    IAPSVC_DEFLOG("%d: %s() flag %d", __LINE__, __FUNCTION__,flag);
    muteAudio(flag);
    return IsSuccess;
}

bool IapService::MuteControl(bool flag)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    //if(IsUsedAsync())
    //    ret = mIapCommandThread->sendCommand(IAP_CMD_MUTECONTROL,data,reply,true);
    //else
        ret = ProcMuteControl(flag);
    return ret;    
}

void IapService::registrerEapSessionId(void *pData)
{
    int i;
    pIPOD_INIT_PARAM pParam = (pIPOD_INIT_PARAM)pData;
    int protocolCount = (int)pParam->m_SDKProtocolToken.Total_ProtocolCnt;
    IAPSVC_DBUG("%d: %s() total protocol count %ld eap session count %d",
                                                            __LINE__, 
                                                            __FUNCTION__,
                                                            pParam->m_SDKProtocolToken.Total_ProtocolCnt,
                                                            mEapProtocolList.size()
                                                            );
    if(pParam->m_SDKProtocolToken.Total_ProtocolCnt == 0)
        return;

    if(mEapProtocolList.size() > 0)
        return;
    
    for(i = 0; i < protocolCount ; i++){
        IPOD_SDK_PROTOCOL_DATA *pData = &pParam->m_SDKProtocolToken.m_protocolTokenData[i];
        int index = (int)pData->m_protocolIndex;
        String8 protocoString = String8((char const*)pData->m_pProtocolString);
        IAPSVC_DBUG("  protocol index %d name [%s] ", index,protocoString.string());
        mEapProtocolList.add(index, protocoString);    
    }
}

uint8_t *IapService::SendPacketedCmd(uint8_t lingo,
                                                            uint16_t command,
                                                            uint16_t retCommand,
                                                            uint8_t *payload,
                                                            int payloadLen,
                                                            void *pCMDErrorCode,
                                                            int *pRetPayloadLen
                                                            )
{
    int RcvCnt;
    uint8_t *retPayload = NULL;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    eCMDErrorCode = IPOD_USER_EnterCriticalSection(GetIPodHandle());
    unsigned char *buff = IPOD_GetTxBuffer(GetIPodHandle()); /*Please use this function to get a Txbuffer */
    int rxBuffSize = 32;
    unsigned char *buffRx = (unsigned char *)malloc(rxBuffSize); /*Please use this function to get a Txbuffer */
    
    if (IPOD_NO_ERROR == eCMDErrorCode){
        IPOD_CMD_PARAM CmdParam;
        int cnt = 0;
        buff[cnt++] = (unsigned char)lingo; 
        if(lingo != LINGO_EXTENDED_INTERFACE){
            buff[cnt++] = (unsigned char)command;
        }else{
            buff[cnt++] = 0;
            buff[cnt++] = (unsigned char)command;
        }
        if(payloadLen > 0)
            memcpy(&buff[cnt],payload,payloadLen);
        CmdParam.m_iPod_Handle = GetIPodHandle();
        CmdParam.m_CurCmd = command;
        CmdParam.m_SendCmdBuf = buff;
        CmdParam.m_SendCmdBufSize = payloadLen + cnt; /*buffer size of User added*/
        CmdParam.m_TimeOut = IPOD_Callback_GetWaitTime();
        CmdParam.m_SendFunc = IPOD_SEND_ADD_HEADER;
        CmdParam.m_Flag = IPOD_DATA_SINGLE_SESSION;
        CmdParam.m_ResponseBuf = buffRx;
        CmdParam.m_ResponseBufSize = rxBuffSize;
         IAPSVC_DBUG("%d: %s() eCMDErrorCode 0x%x lingo 0x%x command 0x%x", 
                                                                                                                __LINE__, 
                                                                                                                __FUNCTION__,
                                                                                                                eCMDErrorCode,
                                                                                                                lingo,
                                                                                                                command);

        eCMDErrorCode = IPOD_USER_FUNC_SendCmd_RcvResponse(
                                                                                                    &CmdParam, 
                                                                                                    lingo, 
                                                                                                    retCommand
                                                                                                    ); 

         IAPSVC_DBUG("%d: %s() eCMDErrorCode 0x%x retPayloadLen %ld %p %p", 
                                                                        __LINE__, __FUNCTION__,
                                                                        eCMDErrorCode,
                                                                        CmdParam.m_ResponseBufSize,
                                                                        CmdParam.m_ResponseBuf,
                                                                        buffRx
                                                                        );

        *pRetPayloadLen = 0;
        if (eCMDErrorCode == IPOD_COMMAND_ACCEPTED)
        {
            int retPayloadLen = CmdParam.m_ResponseBufSize;
            if(retPayloadLen > 0){
                retPayload = (unsigned char *)malloc(retPayloadLen);
                IAPSVC_DBUG("%d: %s() retPayload %p", __LINE__, __FUNCTION__,retPayload);
                if(retPayload != NULL){
                    memcpy(retPayload,CmdParam.m_ResponseBuf,retPayloadLen);
                    *pRetPayloadLen = retPayloadLen;
                }else{
                    ALOGE("%d: %s() ", __LINE__, __FUNCTION__);
                }
            }

            //iPOD_DumpMemory(CmdParam.m_ResponseBuf,(int)DataSize);
        }else{
            ALOGE("%d: %s() eCMDErrorCode 0x%x lingo 0x%x cmd 0x%x retCmd 0x%x",
                                                                                                                    __LINE__,
                                                                                                                    __FUNCTION__,
                                                                                                                    eCMDErrorCode,
                                                                                                                    lingo,
                                                                                                                    command,
                                                                                                                    retCommand);
        }        
    }
    if(buffRx != NULL)
        free(buffRx);
    IPOD_USER_ExitCriticalSection(GetIPodHandle());
    *(IPOD_ERROR *)pCMDErrorCode = eCMDErrorCode;
    return retPayload;
}

void IapService::AddTrackInfos(Parcel *itemPool,
                                                Parcel *trackInfoPool,
                                                int itemCnt,
                                                int lastIndex
                                                )
{
    if(lastIndex >= 0){
        trackInfoPool->writeInt32(lastIndex);
        trackInfoPool->writeInt32(itemCnt);
        trackInfoPool->appendFrom(itemPool,0,itemPool->dataSize());
        IAPSVC_DBUG("%d: %s() lastIndex [%d] itemCnt[%d] ", 
                                                            __LINE__, 
                                                            __FUNCTION__,
                                                            lastIndex,
                                                            itemCnt
                                                            );
        itemPool->setDataSize(0);

    }
}

void IapService::AddTrackInfosUid(Parcel *itemPool,
                                                Parcel *trackInfoPool,
                                                int itemCnt,
                                                long long lastUid
                                                )
{
    if(lastUid >= 0){
        trackInfoPool->writeInt64(lastUid);
        trackInfoPool->writeInt32(itemCnt);
        trackInfoPool->appendFrom(itemPool,0,itemPool->dataSize());
        IAPSVC_DBUG("%d: %s() lastUid [%lld] itemCnt[%d] ", 
                                                            __LINE__, 
                                                            __FUNCTION__,
                                                            lastUid,
                                                            itemCnt
                                                            );
        itemPool->setDataSize(0);

    }
}

bool IapService::SendPacketedCmdForTrackInfo(Parcel *reply,
                                                            uint8_t lingo,
                                                            uint16_t command,
                                                            uint16_t retCommand,
                                                            uint8_t *payload,
                                                            int payloadLen,
                                                            void *pCMDErrorCode,
                                                            int receiveCnt,
                                                            bool isUidTrackInfo,
                                                            long long trackUID
                                                            )
{
    bool IsSuccess = false;
    int RcvCnt;
    uint8_t *retPayload = NULL;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    eCMDErrorCode = IPOD_USER_EnterCriticalSection(GetIPodHandle());
    unsigned char *buff = IPOD_GetTxBuffer(GetIPodHandle()); /*Please use this function to get a Txbuffer */
    int retPayloadLen;
    Parcel trackInfoPool;
    Parcel itemPool;
    int lastIndex = -1;
    int currentIndex = -1;
    long long lastUID = -1;
    long long currentUID = -1;
    int itemCnt = 0;
    int trackCnt = 0;
    int rxBuffSize = 32;
    unsigned char *buffRx = (unsigned char *)malloc(rxBuffSize);
    
    if (IPOD_NO_ERROR == eCMDErrorCode){
        IPOD_CMD_PARAM CmdParam;
        int cnt = 0;
        buff[cnt++] = (unsigned char)lingo; 
        if(lingo != LINGO_EXTENDED_INTERFACE){
            buff[cnt++] = (unsigned char)command;
        }else{
            buff[cnt++] = 0;
            buff[cnt++] = (unsigned char)command;
        }
        if(payloadLen > 0)
            memcpy(&buff[cnt],payload,payloadLen);
        CmdParam.m_iPod_Handle = GetIPodHandle();
        CmdParam.m_CurCmd = command;
        CmdParam.m_SendCmdBuf = buff;
        CmdParam.m_SendCmdBufSize = payloadLen + cnt; /*buffer size of User added*/
        CmdParam.m_TimeOut = IPOD_Callback_GetWaitTime();
        CmdParam.m_ResponseBuf = buffRx;
        CmdParam.m_ResponseBufSize = rxBuffSize;

        if(receiveCnt>1){
            CmdParam.m_Flag = IPOD_MULTI_RESPONSE;
        } else{
            CmdParam.m_Flag = IPOD_DATA_SINGLE_SESSION;
        }
        for(RcvCnt=0;RcvCnt<receiveCnt;RcvCnt++){
            if(RcvCnt!=0){
                CmdParam.m_SendFunc = IPOD_SEND_SKIP;
                CmdParam.m_SendCmdBufSize = 0;
                CmdParam.m_TimeOut = IPOD_Callback_GetWaitTime();
            }else{
                CmdParam.m_SendFunc = IPOD_SEND_ADD_HEADER;
            }
            eCMDErrorCode = IPOD_USER_FUNC_SendCmd_RcvResponse(
                                                                                                    &CmdParam, 
                                                                                                    lingo, 
                                                                                                    retCommand
                                                                                                    ); 

            retPayloadLen = CmdParam.m_ResponseBufSize;
            if(eCMDErrorCode == IPOD_COMMAND_ACCEPTED){
                int pos = 0;
                //reply->writeInt32(IsSuccess);
                if(isUidTrackInfo){
                    pos += CheckUIDofTrackInfo(CmdParam.m_ResponseBuf, retPayloadLen,&currentUID);

                    if(currentUID != lastUID){
                        AddTrackInfosUid(&itemPool,&trackInfoPool, itemCnt, lastUID);
                        itemCnt = 0;
                        lastUID = currentUID;
                        trackCnt++;
                    }

                }else{
                    pos += CheckIndexofTrackInfo(CmdParam.m_ResponseBuf, retPayloadLen,&currentIndex);

                    if(currentIndex != lastIndex){
                        AddTrackInfos(&itemPool,&trackInfoPool, itemCnt, lastIndex);
                        itemCnt = 0;
                        lastIndex = currentIndex;
                        trackCnt++;
                    }
                }
                

                ParseTrackInformation(&CmdParam.m_ResponseBuf[pos], retPayloadLen - pos,&itemPool,isUidTrackInfo);
                itemCnt++;
                //Dump((const char *)pRetPayload,retPayloadLen);
            }else{
                uint8_t errCode = 0;
                uint16_t checkID = 0;    
                //reply->writeInt32(IsSuccess);
                if(retPayloadLen >= 3){
                    errCode = CmdParam.m_ResponseBuf[0];
                    checkID = (CmdParam.m_ResponseBuf[1]<<8) | CmdParam.m_ResponseBuf[2];
                    IAPSVC_DBUG("%d: %s() errCode [0x%x] Id[0x%x] ", 
                                                                        __LINE__, 
                                                                        __FUNCTION__,
                                                                        errCode,
                                                                        checkID
                                                                        );
                }
                IAPSVC_DBUG("%d: %s() eCMDErrorCode [0x%x] ",__LINE__, 
                                                                        __FUNCTION__,
                                                                        eCMDErrorCode
                                                                        );
                break;
            }
        } 

        CmdParam.m_SendCmdBufSize = 0;
        CmdParam.m_Flag = IPOD_MULTI_RESPONSE_END;
        CmdParam.m_SendFunc = IPOD_SEND_ADD_HEADER;
        eCMDErrorCode = IPOD_USER_FUNC_SendCmd_RcvResponse(&CmdParam, 0, 0);

        if(RcvCnt ==  receiveCnt){
            if(isUidTrackInfo)
                AddTrackInfosUid(&itemPool,&trackInfoPool, itemCnt, lastUID);
            else
                AddTrackInfos(&itemPool,&trackInfoPool, itemCnt, lastIndex);
            reply->writeInt32(1);
            reply->writeInt32(trackCnt);
            reply->appendFrom(&trackInfoPool,0,trackInfoPool.dataSize());
        }else{
            reply->writeInt32(0);
        }
    }

    if(buffRx != NULL)
        free(buffRx);
    
    IPOD_USER_ExitCriticalSection(GetIPodHandle());
    *(IPOD_ERROR *)pCMDErrorCode = eCMDErrorCode;
    return retPayload;
}

bool IapService::GetNowPlayingAppBundleName(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GETNOWPLAYINGAPPBUNDLENAME,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetNowPlayingAppBundleName(data,reply);
    return ret;    
}

bool IapService::ProcGetNowPlayingAppBundleName(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    int retPayloadLen = 0;
    uint8_t *pRetPayload = NULL;
    uint8_t lingo = LINGO_GENERAL;
    uint16_t command = 0x65;
    uint16_t retCommand = 0x66;
    int payloadLen = 0;
    uint8_t errCode = 0;
    uint16_t checkID = 0;    
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    pRetPayload = SendPacketedCmd(lingo, 
                                                    command, 
                                                    retCommand, 
                                                    NULL, 
                                                    payloadLen, 
                                                    (void *)&eCMDErrorCode, 
                                                    &retPayloadLen);
    if(eCMDErrorCode == IPOD_COMMAND_ACCEPTED){
        String8 name;
        IsSuccess = true;
        reply->writeInt32(IsSuccess);
        name = String8((char const*)pRetPayload);
        reply->writeString16(String16(name));
        IAPSVC_DEFLOG("%d: %s() name [%s] ", 
                                                                            __LINE__, 
                                                                            __FUNCTION__,
                                                                            name.string()
                                                                            );
    }else{
        reply->writeInt32(IsSuccess);
        if(retPayloadLen >= 2){
            errCode = pRetPayload[0];
            checkID = pRetPayload[1];
        }
        IAPSVC_DBUG("%d: %s() eCMDErrorCode [0x%x]  errCode [0x%x] Id[0x%x] ", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                eCMDErrorCode,
                                                                errCode,
                                                                checkID
                                                                );
    }

    if(pRetPayload){
        free(pRetPayload);
    }
    return IsSuccess;
}

bool IapService::ResumeIpod(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_PLAYCONTROL,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcResumeIpod(data,reply);
    return ret;    
}

bool IapService::ProcResumeIpod(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    int retPayloadLen = 0;
    uint8_t *pRetPayload = NULL;
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    uint8_t lingo = LINGO_EXTENDED_INTERFACE;
    uint16_t command = 0x29;
    uint16_t retCommand = 0x01;
    uint8_t payload[1];
    int payloadLen = 1;
    uint8_t errCode = 0;
    uint16_t checkID = 0;
    payload[0] = RESUMEIPOD;
    pRetPayload = SendPacketedCmd(lingo, 
                                                    command, 
                                                    retCommand, 
                                                    payload, 
                                                    payloadLen, 
                                                    (void *)&eCMDErrorCode, 
                                                    &retPayloadLen);
    if(eCMDErrorCode == IPOD_COMMAND_ACCEPTED){
        IsSuccess = true;
        reply->writeInt32(IsSuccess);
        SetSvcLocalPlayStatus(IPOD_DR_UNKNOWN);
        //IPOD_Sample_SetDRPlayStatus(GetIPodHandle(),IPOD_DR_UNKNOWN);
    }else{
        reply->writeInt32(IsSuccess);
    }
    if(retPayloadLen >= 3){
        errCode = pRetPayload[0];
        checkID = (pRetPayload[1]<<8) | pRetPayload[2];
    }
    IAPSVC_DBUG("%d: %s() errCode [0x%x] Id[0x%x] ", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                errCode,
                                                                checkID
                                                                );

    if(pRetPayload){
        free(pRetPayload);
    }
    return IsSuccess;
}

int IapService::PutShort(uint8_t *buf,int value)
{
    int cnt = 0;
    buf[cnt++] = (value >> 8);
    buf[cnt++] = (value & 0xFF);
    return cnt;
}

int IapService::PutInt(uint8_t *buf,int value)
{
    int cnt = 0;
    buf[cnt++] = (value >> 24);
    buf[cnt++] = (value >> 16);
    buf[cnt++] = (value >> 8);
    buf[cnt++] = (value & 0xFF);
    return cnt;
}

int IapService::PutLong(uint8_t *buf,long long value)
{
    int cnt = 0;
    buf[cnt++] = (value >> 56);
    buf[cnt++] = (value >> 48);
    buf[cnt++] = (value >> 40);
    buf[cnt++] = (value >> 32);    
    buf[cnt++] = (value >> 24);
    buf[cnt++] = (value >> 16);
    buf[cnt++] = (value >> 8);
    buf[cnt++] = (value & 0xFF);
    return cnt;
}

int IapService::GetShort(uint8_t *buf,int *value)
{
    int cnt = 0;
    int data = 0;
    data |= buf[cnt++];data <<= 8;
    data |= buf[cnt++];
    *value = data;
    //IAPSVC_DBUG(" %s() data %d 0x%x",  __FUNCTION__,data,data);
    return cnt;
}

int IapService::GetInt(uint8_t *buf,int *value)
{
    int cnt = 0;
    int data = 0;
    data = buf[cnt++];data <<= 8;
    data |= buf[cnt++];data <<= 8;
    data |= buf[cnt++];data <<= 8;
    data |= buf[cnt++];
    *value = data;
    //IAPSVC_DBUG(" %s() data %d 0x%x",  __FUNCTION__,data,data);
    return cnt;
}

int IapService::GetLong(uint8_t *buf,long long *value)
{
    int cnt = 0;
    long long data = 0;
    data = buf[cnt++];data <<= 8;
    data |= buf[cnt++];data <<= 8;
    data |= buf[cnt++];data <<= 8;
    data |= buf[cnt++];data <<= 8;
    data |= buf[cnt++];data <<= 8;
    data |= buf[cnt++];data <<= 8;
    data |= buf[cnt++];data <<= 8;
    data |= buf[cnt++];
    *value = data;
    //IAPSVC_DBUG(" %s() data %lld 0x%llx",  __FUNCTION__,data,data);
    return cnt;
}

int IapService::CheckBitMask(uint32_t bitMask)
{
    int ret = 0;
    int i;
    bool fFound = false;
    uint32_t mask = 1;
    int cnt = sizeof(uint32_t)*8;
    for(i = 0;i < cnt;i++){
        if(((bitMask >> i) & mask) == mask){
            ret++;
        }
    }

    return ret;
}

int IapService::CheckIndexofTrackInfo(uint8_t *pPayload,int payloadLen,int *trackIndex)
{
    int cnt = 0;    
    int index = 0;
    if(payloadLen >= 4)
        cnt += GetInt(&pPayload[cnt], &index);
    *trackIndex = index;
    return cnt;
}

int IapService::CheckUIDofTrackInfo(uint8_t *pPayload,int payloadLen,long long *trackIndex)
{
    int cnt = 0;    
    long long index = 0;
    if(payloadLen >= 8)
        cnt += GetLong(&pPayload[cnt], &index);
    *trackIndex = index;
    return cnt;
}

bool IapService::ParseTrackInformation(uint8_t *pPayload,int payloadLen,Parcel *reply,bool isUidTrackInfo)
{
    int type = -1;
    int cnt = 0;
    int data32 = 0;
    long long data64 = 0;
    int index = 0;
    String8 str;

    /*
    if(isUidTrackInfo){
        cnt += GetLong(&pPayload[cnt], &data64);
        reply->writeInt64(data64);
    }else{
        cnt += GetInt(&pPayload[cnt], &index);
        reply->writeInt32(index);
    }
    */
    type =  pPayload[cnt++];
    reply->writeInt32(type);
    IAPSVC_DBUG("%d: %s() type %d", __LINE__, __FUNCTION__,type);
    switch(type){
        case IPOD_TRACKINFO_CAPABILITIES:// bytes 4
            if((payloadLen - cnt) < 4){
                reply->writeInt32(0);
                goto exit;
            }
            cnt += GetInt(&pPayload[cnt], &data32);
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_TRACKNAME: // UTF8
        case IPOD_TRACKINFO_ARTISTNAME: // UTF8
        case IPOD_TRACKINFO_ALBUMNAME: // UTF8
        case IPOD_TRACKINFO_GENRENAME: // UTF8
        case IPOD_TRACKINFO_COMPOSERNAME: // UTF8
            if((payloadLen  - cnt) < 1){
                reply->writeString16(String16(""));
                goto exit;
            }
            str = String8((char const *)&pPayload[cnt]);
            reply->writeString16(String16(str));
            cnt += str.size();
            break;
        case IPOD_TRACKINFO_TOTALTRACKDURATION: // bytes 4
            if((payloadLen - cnt) < 4){
                reply->writeInt32(0);
                goto exit;
            }
            cnt += GetInt(&pPayload[cnt], &data32);
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_ITUNESUID: // bytes 8
            if((payloadLen - cnt) < 8){
                reply->writeInt64(0);
                goto exit;
            }
            cnt += GetLong(&pPayload[cnt], &data64);
            reply->writeInt64(data64);
            break;            
        case IPOD_TRACKINFO_CHAPTERCOUNT: // bytes 2
            if((payloadLen - cnt) < 2){
                reply->writeInt32(0);
                goto exit;
            }
            cnt += GetShort(&pPayload[cnt], &data32);
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_CHAPTERTIMES:
            if((payloadLen - cnt) == 0)
            {
                reply->writeInt32(0);
                reply->writeInt32(0);
            }
            else if((payloadLen - cnt) < 6){
                reply->writeInt32(0);
                reply->writeInt32(0);
                goto exit;
            }else{
                cnt += GetShort(&pPayload[cnt], &data32); // bytes 2
                reply->writeInt32(data32);
                cnt += GetInt(&pPayload[cnt], &data32); // bytes 4
                reply->writeInt32(data32);
            }
            break;
        case IPOD_TRACKINFO_CHAPTERNAME:
            if((payloadLen - cnt) == 0)
            {
                reply->writeInt32(0);
                reply->writeString16(String16(""));
            }
            else if((payloadLen - cnt) < 3){
                reply->writeInt32(0);
                reply->writeString16(String16(""));
                goto exit;
            }else{
                cnt += GetShort(&pPayload[cnt], &data32); // bytes 2
                reply->writeInt32(data32);
                str = String8((char const *)&pPayload[cnt]);
                reply->writeString16(String16(str)); // UTF8
                cnt += str.size();
            }
            break;
        case IPOD_TRACKINFO_LYRICS:
            if((payloadLen - cnt) < 5){
                reply->writeInt32(0);
                reply->writeInt32(0);
                reply->writeString16(String16(""));
                goto exit;
            }
            cnt += GetShort(&pPayload[cnt], &data32); // bytes 2
            reply->writeInt32(data32);
            cnt += GetShort(&pPayload[cnt], &data32); // bytes 2
            reply->writeInt32(data32);     
            str = String8((char const *)&pPayload[cnt]);
            reply->writeString16(String16(str)); // UTF8
            cnt += str.size(); 
            break;
        case IPOD_TRACKINFO_DESCRIPTION:
            if((payloadLen - cnt) < 1){
                reply->writeString16(String16(""));
                goto exit;
            }
            str = String8((char const *)&pPayload[cnt]);
            reply->writeString16(String16(str)); // UTF8
            cnt += str.size(); 
            break;
        case IPOD_TRACKINFO_ALBUMTRACKINDEX:
            if((payloadLen - cnt) < 2){
                reply->writeInt32(0);
                goto exit;
            }
            cnt += GetShort(&pPayload[cnt], &data32); // bytes 2
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_DISCSETALBUMINDEX:
            if((payloadLen - cnt) < 2){
                reply->writeInt32(0);
                goto exit;
            }
            cnt += GetShort(&pPayload[cnt], &data32); // bytes 2
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_PLAYCOUNT:
            if((payloadLen - cnt) < 4){
                reply->writeInt32(0);
                goto exit;
            }
            cnt += GetInt(&pPayload[cnt], &data32); // bytes 4
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_SKIPCOUNT:
            if((payloadLen - cnt) < 4){
                reply->writeInt32(0);
                goto exit;
            }
            cnt += GetInt(&pPayload[cnt], &data32); // bytes 4
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_PODCASERELEASEDATE:
        case IPOD_TRACKINFO_LASTPLAYED:
            if((payloadLen - cnt) < 7){
                reply->writeInt32(0);
                reply->writeInt32(0);
                reply->writeInt32(0);
                reply->writeInt32(0);
                reply->writeInt32(0);
                reply->writeInt32(0);
                goto exit;
            }
            reply->writeInt32(pPayload[cnt++]); // second
            reply->writeInt32(pPayload[cnt++]); // minute
            reply->writeInt32(pPayload[cnt++]);// hour
            reply->writeInt32(pPayload[cnt++]); // day
            reply->writeInt32(pPayload[cnt++]); // month
            cnt += GetShort(&pPayload[cnt], &data32); // year , bytes 2
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_YEAR:
            if((payloadLen - cnt) < 2){
                reply->writeInt32(0);
                goto exit;
            }
            cnt += GetShort(&pPayload[cnt], &data32); // bytes 2
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_STARRATING:
            if((payloadLen - cnt) < 1){
                reply->writeInt32(0);
                goto exit;
            }
            reply->writeInt32(pPayload[cnt++]); 
            break;
        case IPOD_TRACKINFO_SERIESNAME:
            if((payloadLen - cnt) < 1){
                reply->writeString16(String16(""));
                goto exit;
            }
            str = String8((char const *)&pPayload[cnt]);
            reply->writeString16(String16(str)); // UTF8
            cnt += str.size(); 
            break;
        case IPOD_TRACKINFO_SEASONNUMBER:
            if((payloadLen - cnt) < 2){
                reply->writeInt32(0);
                goto exit;
            }
            cnt += GetShort(&pPayload[cnt], &data32); // bytes 2
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_TRACKVOLUMEADJUST:
            if((payloadLen - cnt) < 1){
                reply->writeInt32(0);
                goto exit;
            }
            reply->writeInt32(pPayload[cnt++]); // bytes 1
            break;
        case IPOD_TRACKINFO_TRACKEQPRESET:
            if((payloadLen - cnt) < 2){
                reply->writeInt32(0);
                goto exit;
            }
            cnt += GetShort(&pPayload[cnt], &data32); // bytes 2
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_DATARATE:
            if((payloadLen - cnt) < 4){
                reply->writeInt32(0);
                goto exit;
            }
            cnt += GetInt(&pPayload[cnt], &data32); // bytes 4
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_BOOKMARKOFFSET:
            if((payloadLen - cnt) < 4){
                reply->writeInt32(0);
                goto exit;
            }
            cnt += GetInt(&pPayload[cnt], &data32); // bytes 4
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_STARTSTOPOFFSET:
            if((payloadLen - cnt) < 8){
                reply->writeInt32(0);
                reply->writeInt32(0);
                goto exit;
            }
            cnt += GetInt(&pPayload[cnt], &data32); // bytes 4
            reply->writeInt32(data32);
            cnt += GetInt(&pPayload[cnt], &data32); // bytes 4
            reply->writeInt32(data32);
            break;
        case IPOD_TRACKINFO_RESERVED:
            break;
    }
    return true;
exit:
    ALOGE("%d: %s() index %d type %d payloadLen %d", __LINE__, __FUNCTION__,index,type,payloadLen);
    return false;
}

bool IapService::GetUIDTrackInfo(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GETUIDTRACKINFO,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetUIDTrackInfo(data,reply);
    return ret;    
}

bool IapService::ProcGetUIDTrackInfo(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    uint8_t lingo = LINGO_EXTENDED_INTERFACE;
    uint16_t command = 0x3E;
    uint16_t retCommand = 0x3F;
    uint8_t payload[12];
    int payloadLen = 0;
    uint8_t errCode = 0;
    uint16_t checkID = 0;
    long long trackUID = data->readInt64();
    uint32_t typeBitmaxk = data->readInt32();
    int i,pos;
    int bitmaskCount = CheckBitMask(typeBitmaxk);

    IAPSVC_DEFLOG("%d: %s() UID %lld", __LINE__, __FUNCTION__,trackUID);

    payloadLen += PutLong(&payload[payloadLen],trackUID);
    memcpy(&payload[payloadLen], &typeBitmaxk,4);
    pos = 0;
    for(i =0;i < 4;i++){
        if(payload[payloadLen + i] != 0)
            pos = i;
    }
    payloadLen += (pos + 1);
    
    IsSuccess = SendPacketedCmdForTrackInfo(reply,
                                                lingo, 
                                                command, 
                                                retCommand, 
                                                payload, 
                                                payloadLen, 
                                                (void *)&eCMDErrorCode, 
                                                bitmaskCount,
                                                true,
                                                trackUID);

    return IsSuccess;
}

bool IapService::ProcGetTrackInfo(const Parcel *data,Parcel *reply,bool fIsPB)
{
    bool IsSuccess = false;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    uint8_t lingo = LINGO_EXTENDED_INTERFACE;
    uint16_t command = 0;
    uint16_t retCommand = 0;
    uint8_t payload[12];
    int payloadLen = 0;
    int startIndex = data->readInt32();
    int trackCount = data->readInt32();
    uint32_t typeBitmaxk = data->readInt32();
    int i,pos;
    int bitCount = CheckBitMask(typeBitmaxk);
    int receiveCnt = bitCount*trackCount;
    IAPSVC_DEFLOG("%d: %s() receiveCnt %d start(%d) count(%d) bits(%d)",
                                        __LINE__, __FUNCTION__,receiveCnt,startIndex,trackCount,bitCount);

    if(fIsPB){
        command = 0x42;
        retCommand = 0x43;        
    }else{
        command = 0x40;
        retCommand = 0x41;          
    }

    payloadLen += PutInt(&payload[payloadLen],startIndex);
    payloadLen += PutInt(&payload[payloadLen],trackCount);
    memcpy(&payload[payloadLen], &typeBitmaxk,4);
    pos = 0;
    for(i =0;i < 4;i++){
        if(payload[payloadLen + i] != 0)
            pos = i;
    }
    payloadLen += (pos + 1);
    
    IsSuccess = SendPacketedCmdForTrackInfo(reply,
                                                    lingo, 
                                                    command, 
                                                    retCommand, 
                                                    payload, 
                                                    payloadLen, 
                                                    (void *)&eCMDErrorCode, 
                                                    receiveCnt,
                                                    false,
                                                    0);

    IAPSVC_DBUG("%d: %s() receiveCnt %d ", __LINE__, __FUNCTION__,receiveCnt);
    return IsSuccess;
}

bool IapService::GetPBTrackInfo(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GETPBTRACKINFO,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetPBTrackInfo(data,reply);
    return ret;    
}

bool IapService::ProcGetPBTrackInfo(const Parcel *data,Parcel *reply)
{
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    return ProcGetTrackInfo(data,reply,TRUE);
}

bool IapService::GetDBTrackInfo(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GETDBTRACKINFO,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetDBTrackInfo(data,reply);
    return ret;    
}

bool IapService::ProcGetDBTrackInfo(const Parcel *data,Parcel *reply)
{
    IAPSVC_DEFLOG("%d: %s() ", __LINE__, __FUNCTION__);
    return ProcGetTrackInfo(data,reply,FALSE);
}

bool IapService::GetIpodCaps(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GETIPODCAPS,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetIpodCaps(data,reply);
    return ret;    
}

bool IapService::ProcGetIpodCaps(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    int retPayloadLen = 0;
    uint8_t *pRetPayload = NULL;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    uint8_t lingo = LINGO_STORAGE;
    uint16_t command = 0x01;
    uint16_t retCommand = 0x02;
    uint8_t payload[1];
    int payloadLen = 0;
    uint8_t errCode = 0;
    int pos = 0;
    unsigned long maxPayload = 0;
    
    pRetPayload = SendPacketedCmd(lingo, 
                                                    command, 
                                                    retCommand, 
                                                    payload, 
                                                    payloadLen, 
                                                    (void *)&eCMDErrorCode, 
                                                    &retPayloadLen);

    IAPSVC_DBUG("%d: %s() errCode [0x%x] ", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                eCMDErrorCode
                                                                );

        
    if(eCMDErrorCode == IPOD_COMMAND_ACCEPTED){
        IsSuccess = true;
        reply->writeInt32(IsSuccess);
    }else{
        reply->writeInt32(IsSuccess);
    }
    if(retPayloadLen >= 22 && IsSuccess == true){
        long long totalSpace = 0;
        int maxFileSize = 0;
        int maxWriteSize = 0;
        char majorVersion = 0;
        char minorVersion = 0;
        
        pos += GetLong(&pRetPayload[pos], &totalSpace);
        pos += GetInt(&pRetPayload[pos], &maxFileSize);
        pos += GetShort(&pRetPayload[pos], &maxWriteSize);
        pos += 6; // reserved(6bytes)
        majorVersion = pRetPayload[pos++];
        minorVersion = pRetPayload[pos++];
        maxWriteSize = (maxWriteSize - 9); // (header )< start 1, length 3, lingo id 1, commad 1, transID 2>, tail< checksum 1 >
        
        reply->writeInt64(totalSpace);
        reply->writeInt32(maxFileSize);
        reply->writeInt32(maxWriteSize);
        reply->writeInt32(majorVersion);
        reply->writeInt32(minorVersion);

        
        IAPSVC_DEFLOG("%d: %s()  total[%lld] maxFile[%d] maxWrite[%d] ver[%d-%d]", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                totalSpace,
                                                                maxFileSize,
                                                                maxWriteSize,
                                                                majorVersion,
                                                                minorVersion
                                                                );
    }

    if(pRetPayload){
        free(pRetPayload);
    }
    return IsSuccess;
}

bool IapService::GetIpodFreeSpace(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_GETIPODFREESPACE,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcGetIpodFreeSpace(data,reply);
    return ret;    
}

bool IapService::ProcGetIpodFreeSpace(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    int retPayloadLen = 0;
    uint8_t *pRetPayload = NULL;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    uint8_t lingo = LINGO_STORAGE;
    uint16_t command = 0x10;
    uint16_t retCommand = 0x11;
    uint8_t payload[1];
    int payloadLen = 0;
    uint8_t errCode = 0;
    int pos = 0;
    pRetPayload = SendPacketedCmd(lingo, 
                                                    command, 
                                                    retCommand, 
                                                    payload, 
                                                    payloadLen, 
                                                    (void *)&eCMDErrorCode, 
                                                    &retPayloadLen);

    IAPSVC_DBUG("%d: %s() errCode [0x%x] ", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                eCMDErrorCode
                                                                );

    if(eCMDErrorCode == IPOD_COMMAND_ACCEPTED){
        IsSuccess = true;
        reply->writeInt32(IsSuccess);
    }else{
        reply->writeInt32(IsSuccess);
    }
    if(retPayloadLen >= 8){
        long long freeSpace = 0;
        pos += GetLong(pRetPayload, &freeSpace);
        reply->writeInt64(freeSpace);
        IAPSVC_DEFLOG("%d: %s()  freeSpace[%lld]", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                freeSpace
                                                                );
    }

    if(pRetPayload){
        free(pRetPayload);
    }
    return IsSuccess;
}

bool IapService::OpenIpodFeatureFile(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_OPENIPODFEATUREFILE,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcOpenIpodFeatureFile(data,reply);
    return ret;    
}

bool IapService::ProcOpenIpodFeatureFile(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    int retPayloadLen = 0;
    uint8_t *pRetPayload = NULL;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    uint8_t lingo = LINGO_STORAGE;
    uint16_t command = 0x12;
    uint16_t retCommand = 0x04;
    uint8_t payload[6];
    int payloadLen = 1;
    uint8_t errCode = 0;
    int pos = 0;
    uint8_t featureType = 0x01; // Radio tagging
    uint32_t fileOptionMask = 0;
    payload[pos++] = featureType;
    PutInt(&payload[pos], fileOptionMask);
    
    pRetPayload = SendPacketedCmd(lingo, 
                                                    command, 
                                                    retCommand, 
                                                    payload, 
                                                    payloadLen, 
                                                    (void *)&eCMDErrorCode, 
                                                    &retPayloadLen);
    IAPSVC_DBUG("%d: %s() errCode [0x%x] ", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                eCMDErrorCode
                                                                );
        
    if(eCMDErrorCode == IPOD_COMMAND_ACCEPTED){
        IsSuccess = true;
        reply->writeInt32(IsSuccess);
    }else{
        reply->writeInt32(IsSuccess);
    }
    if(retPayloadLen >= 1){
        uint8_t handle = 0;
        handle = pRetPayload[0];
        reply->writeInt32(handle);
        IAPSVC_DEFLOG("%d: %s()  handle[%d]", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                handle
                                                                );
    }

    if(pRetPayload){
        free(pRetPayload);
    }
    return IsSuccess;
}

bool IapService::WriteIpodFileData(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_WRITEIPODFILEDATA,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcWriteIpodFileData(data,reply);
    return ret;    
}

bool IapService::ProcWriteIpodFileData(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    int retPayloadLen = 0;
    uint8_t *pRetPayload = NULL;
    uint8_t lingo = LINGO_STORAGE;
    uint16_t command = 0x07;
    uint16_t retCommand = 0x00;
    uint8_t *payload = NULL;
    int payloadLen = 5;
    uint8_t errCode = 0;
    int pos = 0;
    uint32_t offset = 0;
    uint8_t handle = 0;
    uint32_t writeLen = 0;
    offset = data->readInt32();
    handle = data->readInt32(); 
    writeLen = data->readInt32();
    IAPSVC_DEFLOG("%d: %s() offset[%d] handle[%d] dataLen[%d]",
                                                                            __LINE__, __FUNCTION__,
                                                                            offset,
                                                                            handle,
                                                                            writeLen
                                                                            );
    payloadLen += writeLen;
    if(writeLen > 0){
        payload = (uint8_t *)malloc( writeLen + 4  + 1);
        if(payload != NULL){
            pos += PutInt(payload,offset);
            payload[pos++] = handle;
            if(data->read(&payload[pos],writeLen) == 0){ 
                    pRetPayload = SendPacketedCmd(lingo, 
                                                    command, 
                                                    retCommand, 
                                                    payload, 
                                                    payloadLen, 
                                                    (void *)&eCMDErrorCode, 
                                                    &retPayloadLen);
            }else{
                ALOGE("%d: %s() read error from parcel", 
                                                                __LINE__, 
                                                                __FUNCTION__
                                                                );
            }
        }else{
            ALOGE("%d: %s() malloc error", 
                                                                __LINE__, 
                                                                __FUNCTION__
                                                                );
        }
    }else{
        ALOGE("%d: %s() invalid data length [%d]", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                writeLen
                                                                );
    }
    ALOGE("%d: %s() errCode [0x%x] ", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                eCMDErrorCode
                                                                );
    
    if(eCMDErrorCode == IPOD_COMMAND_ACCEPTED){
        IsSuccess = true;
        reply->writeInt32(IsSuccess);
    }else{
        reply->writeInt32(IsSuccess);
    }
    if(retPayloadLen >= 3){
        uint8_t status = 0;
        uint8_t id = 0;
        uint8_t handle = 0;
        status = pRetPayload[0];
        id = pRetPayload[1];
        handle = pRetPayload[2];
        reply->writeInt32(status);
        reply->writeInt32(id);
        reply->writeInt32(handle);
        IAPSVC_DEFLOG("%d: %s()  status[0x%x] id[%d] handle[%d]", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                status,
                                                                id,
                                                                handle
                                                                );
    }


    if(pRetPayload){
        free(pRetPayload);
    }
    return IsSuccess;
}

bool IapService::CloseIpodFile(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_CLOSEIPODFILE,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        ret = ProcCloseIpodFile(data,reply);
    return ret;    
}

bool IapService::ProcCloseIpodFile(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR eCMDErrorCode = IPOD_NO_ERROR;
    int retPayloadLen = 0;
    uint8_t *pRetPayload = NULL;
    
    uint8_t lingo = LINGO_STORAGE;
    uint16_t command = 0x08;
    uint16_t retCommand = 0x00;
    uint8_t payload[1];
    int payloadLen = 1;
    uint8_t errCode = 0;
    int pos = 0;
    uint8_t handle = 0;
    handle = data->readInt32();
    IAPSVC_DBUG("%d: %s() handle %d", __LINE__, __FUNCTION__,handle);
    payload[0] = handle;
    
    pRetPayload = SendPacketedCmd(lingo, 
                                                    command, 
                                                    retCommand, 
                                                    payload, 
                                                    payloadLen, 
                                                    (void *)&eCMDErrorCode, 
                                                    &retPayloadLen);
    IAPSVC_DBUG("%d: %s() errCode [0x%x] ", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                eCMDErrorCode
                                                                );
        
    if(eCMDErrorCode == IPOD_COMMAND_ACCEPTED){
        IsSuccess = true;
        reply->writeInt32(IsSuccess);
    }else{
        reply->writeInt32(IsSuccess);
    }
    if(retPayloadLen >= 3){
        uint8_t status = 0;
        uint8_t id = 0;
        uint8_t handle = 0;
        status = pRetPayload[0];
        id = pRetPayload[1];
        handle = pRetPayload[2];
        reply->writeInt32(status);
        reply->writeInt32(id);
        reply->writeInt32(handle);
        IAPSVC_DEFLOG("%d: %s()  status[0x%x] id[%d] handle[%d]", 
                                                                __LINE__, 
                                                                __FUNCTION__,
                                                                status,
                                                                id,
                                                                handle
                                                                );
    }

    if(pRetPayload){
        free(pRetPayload);
    }
    return IsSuccess;
}

bool IapService::ProcExtraCmd(const Parcel *data,Parcel *reply)
{
    bool IsSuccess = false;
    IPOD_ERROR	eCMDErrorCode = IPOD_NO_ERROR;

    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        IPOD_ERROR ret = IPOD_NO_ERROR;

        int commandID = data->readInt32();
        switch(commandID){
            case IAP_EXT_CMD_GETNOWPLAYINGAPPBUNDLENAME:
                IsSuccess = ProcGetNowPlayingAppBundleName(data,reply);
                break;
            case IAP_EXT_CMD_PLAYCONTROL:
                IsSuccess = ProcResumeIpod(data,reply);                
                break;
            case IAP_EXT_CMD_GETUIDTRACKINFO:
                IsSuccess = ProcGetUIDTrackInfo(data,reply); 
                break;
            case IAP_EXT_CMD_GETDBTRACKINFO:
                IsSuccess = ProcGetDBTrackInfo(data,reply);
                break;
            case IAP_EXT_CMD_GETPBTRACKINFO:
                IsSuccess = ProcGetPBTrackInfo(data,reply);
                break;
            case IAP_EXT_CMD_GETIPODCAPS:
                IsSuccess = ProcGetIpodCaps(data,reply);
                break;
            case IAP_EXT_CMD_GETIPODFREESPACE:
                IsSuccess = ProcGetIpodFreeSpace(data,reply);
                break;
            case IAP_EXT_CMD_OPENIPODFEATUREFILE:
                IsSuccess = ProcOpenIpodFeatureFile(data,reply);
                break;
            case IAP_EXT_CMD_WRITEIPODFILEDATA:
                IsSuccess = ProcWriteIpodFileData(data,reply);
                break;
            case IAP_EXT_CMD_CLOSEIPODFILE:
                IsSuccess = ProcCloseIpodFile(data,reply);
                break;
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    
    return IsSuccess;
}

bool IapService::ExtraCmd(const Parcel *data,Parcel *reply)
{
    #if 1
    bool IsSuccess = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if ( IsInitialized() ) {
        IPOD_ERROR ret = IPOD_NO_ERROR;

        int commandID = data->readInt32();
        switch(commandID){
            case IAP_CMD_GETNOWPLAYINGAPPBUNDLENAME:
                IsSuccess = GetNowPlayingAppBundleName(data,reply);
                break;
            case IAP_CMD_PLAYCONTROL:
                IsSuccess = ResumeIpod(data,reply);                
                break;
            case IAP_CMD_GETUIDTRACKINFO:
                IsSuccess = GetUIDTrackInfo(data,reply); 
                break;
            case IAP_CMD_GETDBTRACKINFO:
                IsSuccess = GetDBTrackInfo(data,reply);
                break;
            case IAP_CMD_GETPBTRACKINFO:
                IsSuccess = GetPBTrackInfo(data,reply);
                break;
            case IAP_CMD_GETIPODCAPS:
                IsSuccess = GetIpodCaps(data,reply);
                break;
            case IAP_CMD_GETIPODFREESPACE:
                IsSuccess = GetIpodFreeSpace(data,reply);
                break;
            case IAP_CMD_OPENIPODFEATUREFILE:
                IsSuccess = OpenIpodFeatureFile(data,reply);
                break;
            case IAP_CMD_WRITEIPODFILEDATA:
                IsSuccess = WriteIpodFileData(data,reply);
                break;
            case IAP_CMD_CLOSEIPODFILE:
                IsSuccess = CloseIpodFile(data,reply);
                break;
            case IAP_CMD_GET_ARTWORK_FORMATS:
                IsSuccess = GetArtworkFormats(data,reply);
                break;
            case IAP_CMD_GET_ARTWORK_COUNT:
                IsSuccess = GetArtworkCount(data,reply);
                break;
            case IAP_CMD_GET_ARTWORK_TIMES:
                IsSuccess = GetArtworkTimes(data,reply);
                break;
            case IAP_CMD_GET_ARTWORK_DATA:
                IsSuccess = GetArtworkData(data,reply);
                break;
            default:
                break;
        }
    }else{
        ALOGE(" %s Invalid state or invalid message.",__FUNCTION__);
    }
    #else
    bool IsSuccess = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);    
    if(IsUsedAsync()){
        IsSuccess = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_EXTRA_COMMAND,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }else
        IsSuccess = ProcExtraCmd(data,reply);
    #endif
    return IsSuccess;        
}

bool IapService::ProcCallCb(const sp<IIapServiceCallback>& cbFunc ,Parcel *reply)
{
    IAPSVC_DBUG("%s [%d] ", __FUNCTION__, __LINE__);

    if(cbFunc != NULL && IInterface::asBinder(cbFunc)/*cbFunc->asBinder()*/->isBinderAlive()){

        cbFunc->onRspCb(reply);       
        //cbFunc->asBinder()->remoteBinder();
    }else{
        ALOGE("%s [%d] ", __FUNCTION__, __LINE__);
    }
    return true;
}

bool IapService::ProcAsyncCmd1(int cmdID,const sp<IIapServiceCallback>& cbFunc ,const Parcel *data,Parcel *reply)
{
    Parcel reply1;
    IAPSVC_DBUG("%s [%d] ", __FUNCTION__, __LINE__);
    if(data != NULL){
        int arg0 = data->readInt32();
        int arg1 = data->readInt32();
        int arg2 = data->readInt32();
        int arg3 = data->readInt32();
        //ALOGE("%s [%d]  [%d : %d : %d  : %d] ", __FUNCTION__, __LINE__,arg0,arg1,arg2,arg3);
    }
    if(cbFunc != NULL && IInterface::asBinder(cbFunc)/*cbFunc->asBinder()*/->isBinderAlive()){
        reply1.writeInt32(10);
        reply1.writeInt32(9);
        reply1.writeInt32(8);
        reply1.writeInt32(1);
        cbFunc->onRspCb(&reply1);       
        //cbFunc->asBinder()->remoteBinder();
    }else{
        ALOGE("%s [%d] ", __FUNCTION__, __LINE__);
    }
    return true;
}

bool IapService::AsyncCmd1(int cmdID,const sp<IIapServiceCallback>& cbFunc ,const Parcel *data,Parcel *reply)
{
    bool ret = true;

    if(cmdID == IAP_CMD_INIT){
        IPOD_PAL_SetInitializingFlag(1);
        ret = InitPreparation(data);
    }

    IAPSVC_DBUG("%s [%d] cmdID %d %p", __FUNCTION__, __LINE__,cmdID - IAP_CMD_MASK,data);
    if(ret == true){
        if(IsUsedAsync()){
            ret = mIapCommandThread->sendCommandCbFunc(
                                                                            IAP_CMD_ASYNC_COMMAND,
                                                                            data,
                                                                            reply,
                                                                            true,
                                                                            cmdID,
                                                                            cbFunc
                                                                            );
        }//else
            //ret = ProcAsyncCmd1(cmdID,cbFunc,data,reply);

        if(cmdID == IAP_CMD_INIT){
            AfterInit(ret);
        }
    }

    return ret;
}

bool IapService::ProcSyncCmd1(int cmdID,const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAP_CMD_DATA_TYPE cmd;
    IAP_RSP_DATA_TYPE rsp;
    unsigned long eapSessionId = 0;
    String8 protocolName;
    
    cmd.what = cmdID;
    cmd.data = data;
    cmd.reply = reply;
    if(mIapCommandThread != NULL){
        cmd.msgFlushSig = mIapCommandThread->getMsgFlushSig();
    }
    mIapCommandThread->SetLastSyncReqCmd(cmdID);
    ret = ProcExcuteCmd(cmdID,&cmd, &rsp, reply,&eapSessionId,&protocolName,FALSE);
    mIapCommandThread->SetLastSyncReqRsp(cmdID);

    return ret;
}


bool IapService::SyncCmd1(int cmdID,const Parcel *data,Parcel *reply)
{
    bool ret = false;

    IAPSVC_DBUG("%s [%d] ", __FUNCTION__, __LINE__);
    /*
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_SYNC_COMMAND,
                                                                        data,
                                                                        reply,
                                                                        false
                                                                        );
    }else
    */
        ret = ProcSyncCmd1(cmdID,data,reply);

    return ret;
}

bool IapService::WrapCommand(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                        IAP_CMD_WRAP_COMMAND,
                                                                        data,
                                                                        reply,
                                                                        true
                                                                        );
    }
    return ret;    
}

bool IapService::ProcEapPlay(const Parcel *data,Parcel *reply,String8 *pName8)
{
    const String8 protocolName = data->readString8();
    bool IsSuccess = false;
    
    if(protocolName.size() > 0){
        IAPSVC_DEFLOG(" %s listener Name %s",__FUNCTION__, protocolName.string());
        pName8->setTo(protocolName);
    }
    IAPSVC_DEFLOG("%d: %s() start IsFFREWMode(%d)", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        GetSvcLocalFFRewStatus()
                                                                        );
    IsSuccess = ProcDoPlay(reply,0,IAP_CMD_EAP_PLAY);
    return IsSuccess;
}

bool IapService::EapPlay(const Parcel *data)
{
    bool ret = false;
    String8 Name;
    Parcel reply;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_EAP_PLAY,data,NULL,true);
    else{
        ret = ProcEapPlay(data,&reply,&Name);
    }
    return ret;    
}

bool IapService::ProcEapPause(const Parcel *data,Parcel *reply,String8 *pName8)
{
    const String8 protocolName = data->readString8();

    if(protocolName.size() > 0){
        IAPSVC_DEFLOG(" %s listener Name %s",__FUNCTION__, protocolName.string());
        pName8->setTo(protocolName);
    }
    return ProcPause(reply);
}

bool IapService::EapPause(const Parcel *data)
{
    bool ret = false;
    String8 Name;
    Parcel reply;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync())
        ret = mIapCommandThread->sendCommand(IAP_CMD_EAP_PAUSE,data,NULL,true);
    else{
        ret = ProcEapPause(data,&reply,&Name);
    }
    return ret;    
}

bool IapService::ProcEapGetDrPlayStatus(const Parcel *data,Parcel *reply,String8 *pName8)
{
    const String8 protocolName = data->readString8();
    bool ret = false;
    long nIndex = -1;
    IPOD_DR_PLAY_STATUS status = IPOD_DR_PLAYBACK_PAUSED;
    
    if(protocolName.size() > 0){
        IAPSVC_DEFLOG(" %s listener Name %s",__FUNCTION__, protocolName.string());
        pName8->setTo(protocolName);
    }

    if ( IsInitialized() ) {
        ret = Sample_GetDRPlayStatus((void *)&status);
        reply->writeInt32(ret);
        reply->writeInt32(status);
    }else{
        ALOGE("%d: %s()  Invalid state or invalid message.", __LINE__, __FUNCTION__);
        reply->writeInt32(false);
    }
    return ret;
}

bool IapService::EapGetDrPlayStatus(const Parcel *data,Parcel *reply)
{
    bool ret = false;
    String8 Name;
    
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if(IsUsedAsync()){
        ret = mIapCommandThread->sendCommand(
                                                                            IAP_CMD_EAP_GET_DR_PLAYSTATUS,
                                                                            data,
                                                                            reply,
                                                                            true
                                                                            );
    }else{
        ret = ProcEapGetDrPlayStatus(data,reply,&Name);
    }
    return ret;    
}

bool IapService::Sample_GetDRPlayStatus(void *playStatus)
{
    bool ret = false;
    IPOD_ERROR error = IPOD_UNKNOWN_ERROR;
    IPOD_DR_PLAY_STATUS status;
    #if 1
    status = (IPOD_DR_PLAY_STATUS)GetSvcLocalPlayStatus();
    #else
    if(IS_CMDOK(IPOD_Sample_GetDRPlayStatus(
                                                                GetIPodHandle(),
                                                                &status))
                                                                )
    #endif
    {
        IAPSVC_DBUG("%d: %s() status(%d)", 
                                                                __LINE__,
                                                                __FUNCTION__,
                                                                status//*(IPOD_DR_PLAY_STATUS *)playStatus
                                                                );
        if(status == IPOD_DR_UNKNOWN){
            
            IPOD_DR_PLAYSTATUS tmpPlayStatus;
            if ( IsInitialized() ) {
                error = (IPOD_ERROR)UtilGetDrPlayStatus(&tmpPlayStatus);
                IAPSVC_DEFLOG("%d: %s() ret = %d status(%d), %ld/%ld ms", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        ret, 
                                                                        tmpPlayStatus.m_Status,
                                                                        tmpPlayStatus.m_TrackPos_ms,
                                                                        tmpPlayStatus.m_TrackTotalLen_ms
                                                                        );    
                if(error == IPOD_NO_ERROR){
                    status = tmpPlayStatus.m_Status;
                    *(IPOD_DR_PLAY_STATUS *)playStatus = status;
                    //IPOD_Sample_SetDRPlayStatus(GetIPodHandle(),status);
                    ret = true;
                }else{
                    ALOGE("%d: %s() UtilGetDrPlayStatus fail", __LINE__, __FUNCTION__);     
                }
            }else{
                ALOGE("%d: %s() IsInitialized false ", __LINE__, __FUNCTION__);     
            }
        }else{
            *(IPOD_DR_PLAY_STATUS *)playStatus = status;
            ret = true;
        }
    }
    return ret;
}

void IapService::startCommandThread()
{
    Mutex::Autolock lock(mCommandLock);
    if(mIapCommandThread == NULL){        
        mIapCommandThread =  new IapCommandThread(this);
        mIapCommandThread->run("IapCommandThread",PRIORITY_FOREGROUND);// PRIORITY_NORMAL);
        IAPSVC_DBUG("%d: %s()", __LINE__, __FUNCTION__);        
    }
}

void IapService::stopCommandThread()
{ 
    Mutex::Autolock lock(mCommandLock);
    IAPSVC_DBUG("%d: %s() start ", __LINE__, __FUNCTION__);
    if(mIapCommandThread != NULL)
    {
        sp<IapCommandThread> commandThread;
        IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
        commandThread = mIapCommandThread;
        commandThread->CommandDeinit();
        mIapCommandThread= NULL;
        IAPSVC_DBUG("%d: %s() before requestExitAndWait", __LINE__, __FUNCTION__);
        if (commandThread != 0) {
            IAPSVC_DBUG("%d: %s() req exit ", __LINE__, __FUNCTION__);
            commandThread->requestExit();
            IAPSVC_DBUG("%d: %s() wait exit ", __LINE__, __FUNCTION__);
            commandThread->requestExitAndWait();
        }
        IAPSVC_DBUG("%d: %s() after clear", __LINE__, __FUNCTION__);
    }
    IAPSVC_DBUG("%d: %s() end", __LINE__, __FUNCTION__);
}

void IapService::startEventThread()
{
    Mutex::Autolock lock(mEventLock);
    if(mIapEventThread == NULL){        
        mIapEventThread =  new IapEventThread(this);
        mIapEventThread->run("IapEventThread", PRIORITY_NORMAL);
        IAPSVC_DBUG("%d: %s()", __LINE__, __FUNCTION__);
    }
}

void IapService::stopEventThread()
{ 
    Mutex::Autolock lock(mEventLock);
    IAPSVC_DBUG("%d: %s() start ", __LINE__, __FUNCTION__);
    if(mIapEventThread != NULL)
    {
        sp<IapEventThread> eventThread;
        IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
        eventThread = mIapEventThread;
        mIapEventThread= NULL;
        IAPSVC_DBUG("%d: %s() before requestExitAndWait", __LINE__, __FUNCTION__);
        if (eventThread != 0) {
            IAPSVC_DBUG("%d: %s() req exit ", __LINE__, __FUNCTION__);
            eventThread->requestExit();
            IAPSVC_DBUG("%d: %s() wait exit ", __LINE__, __FUNCTION__);
            eventThread->requestExitAndWait();
        }
        IAPSVC_DBUG("%d: %s() after clear", __LINE__, __FUNCTION__);
    }
    IAPSVC_DBUG("%d: %s() end", __LINE__, __FUNCTION__);
}

void IapService::startTimerThread()
{
    #ifdef USE_TIMER
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    mIapTimer = iPod_Timer_CreateThread();
    IAPSVC_DBUG("%d: %s() end ", __LINE__, __FUNCTION__);
    #endif
}

void IapService::stopTimerThread()
{
    #ifdef USE_TIMER
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    iPod_Timer_Destroy(mIapTimer);
    IAPSVC_DBUG("%d: %s() end ", __LINE__, __FUNCTION__);
    #endif
}

void IapService::startAudioThread()
{
    Mutex::Autolock lock(mAudioLock);
    if(mIapAudioThread == NULL){
        mIapAudioThread = new IapAudioThread(this);
        mIapAudioThread->AudioSetBuffer(gAudioBuff);
        mIapAudioThread->run("IapAudioThread", ANDROID_PRIORITY_AUDIO);
        pauseAudio();
    }

    IAPSVC_DBUG("%d: %s()", __LINE__, __FUNCTION__);
}


void IapService::stopAudioThread()
{
    Mutex::Autolock lock(mAudioLock);
    IAPSVC_DBUG("%d: %s() start ", __LINE__, __FUNCTION__);
    if(mIapAudioThread != NULL)
    {
        sp<IapAudioThread> audioThread;
        IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
        audioThread = mIapAudioThread;
        mIapAudioThread = NULL;
        if (audioThread != 0) {
            IAPSVC_DBUG("%d: %s() req exit ", __LINE__, __FUNCTION__);
            audioThread->requestExit();
            IAPSVC_DBUG("%d: %s() wait exit ", __LINE__, __FUNCTION__);
            audioThread->requestExitAndWait();
        }
        IAPSVC_DBUG("%d: %s() after clear", __LINE__, __FUNCTION__);
    }  
    IAPSVC_DBUG("%d: %s() end ", __LINE__, __FUNCTION__);
}

void IapService::initAudio()
{
    if(mIapAudioThread!= NULL){
        int buffSize = 0;
        IAPSVC_DBUG("%d: %s() start ", __LINE__, __FUNCTION__);
        mIapAudioThread->AudioInitFunc(&buffSize);
        IAPSVC_DBUG("%d: %s() end", __LINE__, __FUNCTION__);
    }
}

void IapService::setInitAfterAudio()
{
    if(mIapAudioThread!= NULL){
        IAPSVC_DBUG("%d: %s()", __LINE__, __FUNCTION__);
        mIapAudioThread->AudioResetFirst();
        IAPSVC_DBUG("%d: %s()", __LINE__, __FUNCTION__);
    }
}

void IapService::stopAudio()
{
    IAPSVC_DEFLOG("%d: %s() start ", __LINE__, __FUNCTION__);
    if(mIapAudioThread != NULL)
    {
            mIapAudioThread->AudioDeinit();
    }
    IAPSVC_DEFLOG("%d: %s() end ", __LINE__, __FUNCTION__);
}

void IapService::playAudio()
{
    if(mIapAudioThread!= NULL){
        IAPSVC_DEFLOG("%d: %s() start ", __LINE__, __FUNCTION__);
        mIapAudioThread->AudioStart();
        IAPSVC_DEFLOG("%d: %s() end", __LINE__, __FUNCTION__);
    }
}

void IapService::pauseAudio()
{
    if(mIapAudioThread!= NULL){
        IAPSVC_DEFLOG("%d: %s()", __LINE__, __FUNCTION__);
        mIapAudioThread->AudioPause();
        IAPSVC_DEFLOG("%d: %s()", __LINE__, __FUNCTION__);
    }
}

void IapService::muteAudio(bool flag)
{
    if(mIapAudioThread!= NULL){
        IAPSVC_DEFLOG("%d: %s() flag %d", __LINE__, __FUNCTION__,flag);
        mIapAudioThread->AudioMute(flag);
        IAPSVC_DEFLOG("%d: %s()", __LINE__, __FUNCTION__);
    }
}

status_t IapService::setListener(const sp<IIapServiceNotify>& listener,String8 listenerName)
{
    sp<IIapServiceNotify> mListener = listener;
	IInterface::asBinder(mListener)->linkToDeath(this);
  //  mListener->asBinder()->linkToDeath(this);
    IAPSVC_DEFLOG("%d: %s() add listenerName %s %p", 
                                                                    __LINE__,
                                                                    __FUNCTION__,
                                                                    listenerName.string(),
                                                                    IInterface::asBinder(mListener).get()
                                                                   /* mListener->asBinder().get()*/
                                                                    );
    {
        Mutex::Autolock lock(mListenerLock);
        ssize_t index = mListenerList.indexOfKey(listenerName);
        if(index >= 0){
            sp<IIapServiceNotify> listener1 = NULL;
   
            listener1 = mListenerList.valueAt(index);
            
            ALOGI("%d: %s() remove old listener index %d list size %d [%s]", 
                                                                                                    __LINE__,
                                                                                                    __FUNCTION__,
                                                                                                    index,
                                                                                                    mListenerList.size(),
                                                                                                    listenerName.string()
                                                                                                    );
            if(IInterface::asBinder(listener)/*listener->asBinder() */== IInterface::asBinder(listener1)/*listener1->asBinder()*/){
                IAPSVC_DBUG("%d: %s() same listener", __LINE__, __FUNCTION__);
            }
            listener1->notify(0xF0003,0, 0,0);
            mListenerList.removeItem(listenerName);
        }
        #if 1
        mListenerList.add(listenerName, mListener);
        mListener->notify(0xF0002,0, 0,0);
        IAPSVC_DBUG("%d: %s() new listener mListenerList.size() %d", 
                                                                                                    __LINE__,
                                                                                                    __FUNCTION__,
                                                                                                    mListenerList.size()
                                                                                                    );
        #else
        if ( index < 0) {
            IAPSVC_DBUG("%d: %s() add listener index %d", __LINE__, __FUNCTION__,index);
            mListenerList.add(listenerName, mListener);
        } else {
            IAPSVC_DBUG("%d: %s() replace listener (index %d )", __LINE__, __FUNCTION__,index);
            mListenerList.replaceValueFor(listenerName, mListener);
        }
        #endif
    }

    int i;
    const char *cListenerName = listenerName.string();
    int listenerNameLan = strlen(cListenerName);

    IAPSVC_DBUG("listenerNameLan %d mEapProtocolList.size() %d",listenerNameLan,mEapProtocolList.size() );

    if(IsEmptyReadySessionList() == false){ 
        for(i = 0; i < (int)mEapProtocolList.size(); i++)
        {
            String8 protocolName = nIapSvc->mEapProtocolList.valueAt(i);
             const char *cProtocolName = protocolName.string();
             int protocolNameLen = strlen(cProtocolName);
             IAPSVC_DBUG("protocolNameLen %d  %s",protocolNameLen,cProtocolName );
             if(listenerNameLan == protocolNameLen){
                if(strncmp(cListenerName, cProtocolName,listenerNameLan) == 0){
                    int keyProtocolIndex = mEapProtocolList.keyAt(i);
                    IAPSVC_DBUG("%d: %s() add listenerName %s protocol Index %d", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        listenerName.string(),
                                                                        keyProtocolIndex
                                                                        );

                    mIapCommandThread->sendCommandArg1(
                                                                    IAP_CMD_CHK_READYSESSION,
                                                                    keyProtocolIndex,
                                                                    true
                                                                    );
                }
            }
        }
    }
    IAPSVC_DBUG("%d: %s() end", __LINE__, __FUNCTION__);
    return NO_ERROR;
}

status_t IapService::removeListener(String8 listenerName)
{
    return removeListener(listenerName,true);
}

status_t IapService::removeListener(String8 listenerName,bool notiFlag)
{
    IAPSVC_DEFLOG("%d: %s() remove listenerName %s", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        listenerName.string()
                                                                        );
   {
        Mutex::Autolock lock(mListenerLock);
        ssize_t index = mListenerList.indexOfKey(listenerName);
        if ( index >= 0) {
            sp<IIapServiceNotify> listener = NULL;
            listener = mListenerList.valueAt(index);
            if(notiFlag){
				IInterface::asBinder(listener)->unlinkToDeath(this);
               // listener->asBinder()->unlinkToDeath(this);
                listener->notify(0xF0003,0, 0,0);
            }
            mListenerList.removeItem(listenerName);
        }
    }
        
    return NO_ERROR;
}

void IapService::binderDied(const wp<IBinder>& who) 
{
    int i;
    String8 diedName;
    bool fFound = false;
    ALOGE("binderDied() %p, tid %d, calling tid %d", who.unsafe_get(), gettid(),
            IPCThreadState::self()->getCallingPid());
    {
        Mutex::Autolock lock(mListenerLock);
        for(i = 0; i < (int)mListenerList.size();i++){
            sp<IIapServiceNotify> listener = mListenerList.valueAt(i);
            if(who == IInterface::asBinder(listener)/*listener->asBinder()*/){
                diedName = mListenerList.keyAt(i);
                ALOGE("      died listener index  %d(%s)", 
                                                                        (int)i,
                                                                        diedName.string()
                                                                        );
                fFound = true;
            }
        }
    }
    if(fFound== true){
        int strLen = strlen(diedName.string());
        removeListener(diedName,false);
        if(strncmp("ipod-connect",diedName.string(),strLen) == 0){
            ALOGI(" send intent to start iAP1 connect app");
            //system("am broadcast -a com.telechips.android.iap1.recvstart");
        }
    }
}

void IapService::notifyObjEapByName(int what, int arg1, int arg2,Parcel *obj,String8 name) 
{
    int i;
    sp<IIapServiceNotify> listener = NULL;
    IAPSVC_DBUG("%d: %s() what %d name %s ", __LINE__, __FUNCTION__,what,name.string());
    {
        Mutex::Autolock lock(mListenerLock);
        ssize_t index = mListenerList.indexOfKey(name);
        if (index < 0) {
            ALOGE(" %s name %s ", __FUNCTION__,name.string());
            return;
        }
    
        listener = mListenerList.valueAt(index);
        listener->notifyObj(what, arg1, arg2,obj);
    }    
}

void IapService::notifyObjEap(int what, int arg1, int arg2,Parcel *obj,int sessionId) 
{
    int i;
    Mutex::Autolock lock(mEaSessionLock); 
    IAPSVC_DBUG("%d: %s() what %d ", __LINE__, __FUNCTION__,what);
    ssize_t index = mEapSessionList.indexOfKey(sessionId);
    if (index < 0) {
        IAPSVC_DBUG("%d: %s() can not file the proper listener for the session  %d ", __LINE__, __FUNCTION__,sessionId);
        return;
    }
    
    sp<IIapServiceNotify> listener = mEapSessionList.valueAt(index);
    listener->notifyObj(what, arg1, arg2,obj);
}

void IapService::notifyEap(int what, int arg1, int arg2,int arg3,int sessionId) 
{
    Mutex::Autolock lock(mEaSessionLock); 
    IAPSVC_DBUG("%d: %s() what %d ", __LINE__, __FUNCTION__,what);
    ssize_t index = mEapSessionList.indexOfKey(sessionId);
    if (index < 0) {
        IAPSVC_DBUG("%d: %s() can not file the proper listener for the session  %d ", __LINE__, __FUNCTION__,sessionId);
        return;
    }
    
    sp<IIapServiceNotify> listener = mEapSessionList.valueAt(index);
    listener->notify(what, arg1, arg2,arg3);
}	

void IapService::notifyObjConnect(int what, int arg1, int arg2,Parcel *obj) 
{
    int index = -1;
    IAPSVC_DBUG("%d: %s() what 0x%x ", __LINE__, __FUNCTION__,what );
    switch(what){
        case IAP_RSP_INIT:
            if(arg1 == 0)
                AfterInit(false);
            break;
        default:
            break;
    }
    {
        Mutex::Autolock lock(mListenerLock);
        index = mListenerList.indexOfKey(String8("ipod-connect"));
        IAPSVC_DBUG("%d: %s() what 0x%x ", __LINE__, __FUNCTION__,what );
        if (index < 0) {
            ALOGE("%d: %s() can not find the ipod-connect", __LINE__, __FUNCTION__);
            return;
        }
        sp<IIapServiceNotify> listener = mListenerList.valueAt(index);
        listener->notifyObj(what, arg1, arg2,obj);
    }
}

void IapService::notifyObjTag(int what, int arg1, int arg2,Parcel *obj) 
{
    int index = -1;
    {
        Mutex::Autolock lock(mListenerLock);
        index = mListenerList.indexOfKey(String8("tag-listener"));
        IAPSVC_DBUG("%d: %s() what 0x%x ", __LINE__, __FUNCTION__,what );
        if (index < 0) {
            ALOGE("%d: %s() can not find the tag-listener", __LINE__, __FUNCTION__);
            return;
        }
        sp<IIapServiceNotify> listener = mListenerList.valueAt(index);
        listener->notifyObj(what, arg1, arg2,obj);
    }
}

void IapService::notifyByName(int what, int arg1, int arg2,int arg3,String8 listenerName) 
{
    int index = -1;
    {
        Mutex::Autolock lock(mListenerLock);    
        index = mListenerList.indexOfKey(listenerName);
        IAPSVC_DBUG("%d: %s() listener-name(%s) what 0x%x ", __LINE__, __FUNCTION__,listenerName.string(),what );
        if (index < 0) {
            ALOGE("%d: %s() can not find the listener-name(%s)", __LINE__, __FUNCTION__,listenerName.string());
            return;
        }
        sp<IIapServiceNotify> listener = mListenerList.valueAt(index);
        listener->notify(what, arg1, arg2,arg3);
    }
}

void IapService::notifyObjByName(int what, int arg1, int arg2,Parcel *obj,String8 listenerName) 
{
    int index = -1;
    {
        Mutex::Autolock lock(mListenerLock);    
        index = mListenerList.indexOfKey(listenerName);
        IAPSVC_DBUG("%d: %s() listener-name(%s) what 0x%x ", __LINE__, __FUNCTION__,listenerName.string(),what );
        if (index < 0) {
            ALOGE("%d: %s() can not find the listener-name(%s)", __LINE__, __FUNCTION__,listenerName.string());
            return;
        }
        sp<IIapServiceNotify> listener = mListenerList.valueAt(index);
        listener->notifyObj(what, arg1, arg2,obj);
    }
}

void IapService::notifyObj(int what, int arg1, int arg2,Parcel *obj) 
{
    int index = -1;
    switch(what){
        case IAP_RSP_INIT:
            if(arg1 == 0)
                AfterInit(false);
            break;
        case IPOD_APP_IPODNOTIFICATION:
            if(arg1 == IPOD_NOTI_TYPE_NOWPLAYINGAPPBUNDLENAME)
            {
                if ( IsInitialized() ) {
                    IAPSVC_DBUG("%d: %s() set drPlayStatus to unknown ", __LINE__, __FUNCTION__ );
                    SetSvcLocalPlayStatus(IPOD_DR_UNKNOWN);
                    //IPOD_Sample_SetDRPlayStatus(GetIPodHandle(),IPOD_DR_UNKNOWN);
                }
            }
            break;
        default:
            break;
    }
    {
        Mutex::Autolock lock(mListenerLock);    
        index = mListenerList.indexOfKey(String8("ipod-listener"));
        IAPSVC_DBUG("%d: %s() what 0x%x ", __LINE__, __FUNCTION__,what );
        if (index < 0) {
            ALOGE("%d: %s() can not find the ipod-listener", __LINE__, __FUNCTION__);
            return;
        }
        sp<IIapServiceNotify> listener = mListenerList.valueAt(index);
        listener->notifyObj(what, arg1, arg2,obj);
    }
}

void IapService::notify(int what, int arg1, int arg2,int arg3) 
{
    int index = -1;    
    int fNotify = 1;
    
    switch(what){
        case IPOD_APP_EVT_TRACKPOSITION:
        case IPOD_APP_DR_EVT_TRACK_POSITION:
            IPOD_Sample_SetPostion(GetIPodHandle(),arg1);
            break;
        case IPOD_APP_DR_EVT_TRACK_INDEX:
            IPOD_Sample_SetCurrentIndex(GetIPodHandle(),arg1);
            break;
        case IPOD_APP_EVT_FFSEEKSTOP:
            SetSvcAppleFFRewStatus(false);
            break;
        case IPOD_APP_EVT_REWSEEKSTOP:
            SetSvcAppleFFRewStatus(false);
            break;
        case IPOD_APP_EVT_CHANGE_SAMPLEFREQ:
            if(mIapAudioThread!= NULL){
                IAPSVC_DBUG("%d: %s() AudioSetSampleRate start ", __LINE__, __FUNCTION__);
                mIapAudioThread->AudioSetSampleRate(arg1);
                IAPSVC_DBUG("%d: %s() AudioSetSampleRate end", __LINE__, __FUNCTION__);
            }                
            break;
        case IPOD_APP_DR_EVT_PLAY_STATUS:{
            
            IPOD_DR_PLAY_STATUS playStatus = (IPOD_DR_PLAY_STATUS)GetSvcLocalPlayStatus();;

            //Sample_GetDRPlayStatus(&playStatus);
            SetSvcApplePlayStatus(arg1);
            
            //Param 1 : Play status
            switch(arg1)
            {
                case(IPOD_DR_PLAYBACK_STOPPED):
                    IAPSVC_DEFLOG(" >> Playstatus = IPOD_DR_PLAYBACK_STOPPED %d",playStatus);
                    //pauseAudio();
                    break;

                case(IPOD_DR_PLAYING):
                    IAPSVC_DEFLOG(" >> Playstatus = IPOD_DR_PLAYING %d",playStatus);
                    //playAudio();
                    break;

                case(IPOD_DR_PLAYBACK_PAUSED):
                    IAPSVC_DEFLOG(" >> Playstatus = IPOD_DR_PLAYBACK_PAUSED %d",playStatus);
                    //pauseAudio();
                    break;
                    
                case(IPOD_DR_FAST_FORWARD):
                    IAPSVC_DEFLOG(" >> Playstatus = IPOD_DR_FAST_FORWARD %d",playStatus);
                    //playAudio();
                    break;

                case(IPOD_DR_FAST_REWIND):
                    IAPSVC_DEFLOG(" >> Playstatus = IPOD_DR_FAST_REWIND %d",playStatus);
                    //playAudio();
                    break;

                case(IPOD_DR_END_FAST_FORWARD_OR_REWIND):
                    break;

                default:
                    break;
            }                        
            break;}
        case IPOD_APP_DR_EVT_SHUFFLE:
            IPOD_Sample_SetShuffle(GetIPodHandle(),arg1);
            break;
        case IPOD_APP_DR_EVT_REPEAT:
            IPOD_Sample_SetRepeat(GetIPodHandle(), arg1);
            break;                
        default:
            break;
    }
    if(fNotify){
        Mutex::Autolock lock(mListenerLock);    
        index = mListenerList.indexOfKey(String8("ipod-listener"));
        IAPSVC_DBUG("%d: %s() what 0x%x ", __LINE__, __FUNCTION__,what );
        if (index < 0) {
            ALOGE("%d: %s() can not find the ipod-listener", __LINE__, __FUNCTION__);
            return;
        }
        sp<IIapServiceNotify> listener = mListenerList.valueAt(index);
        listener->notify(what, arg1, arg2,arg3);            
    }

}	

bool IapService::MsgFlush(void) 
{
    if(mIapCommandThread != NULL){
        IAPSVC_DBUG("%d: %s() flushSig 0x%x ", 
                                                    __LINE__, 
                                                    __FUNCTION__,
                                                    mIapCommandThread->getMsgFlushSig());
        mIapCommandThread->incMsgFlushSig();
        return true;
    }
    return false;
}

void IapService::Dump(const char *buff,int length)
{
    int i;
    int j;
    char *buf = NULL;

    if (IAPSVC_DEBUG != 1)
        return;

    buf = (char *)malloc(length*20);

    if (length>0) {
        for (i=0;i<length;i+=16) {
            int index = 0;
            index += sprintf(&buf[index],"DUMP %04X: ",i);
            for (j=0;j<16;j++) {
                if ((i+j)<length) {
                    index += sprintf(&buf[index]," %02X",buff[i+j]);
                } else {
                    index += sprintf(&buf[index],"   ");
                }
            }
            index += sprintf(&buf[index]," : ");
            for (j=0;j<16;j++) {
                if ((i+j)<length) {
                    index += sprintf(&buf[index],"%c",(buff[i+j]>' ')?buff[i+j]:'.');
                } else {
                    break;
                    //IAPHID_DBUG("  ");
                }
            }
            IAPSVC_DBUG(" %s",buf);
        }
    }
    free(buf);
}


bool IapService::ProcDispatchCmd(IAP_CMD_DATA_TYPE *cmd,const Parcel *cmdData,Parcel *replyData,void *pData)
{
    bool ret = false;
    
    switch(cmd->what){
        case IAP_CMD_INIT:
            ret = ProcInit(cmdData,replyData);
            break;
        case IAP_CMD_DEINIT:
            ret = ProcDeinit();
            break;
        case IAP_CMD_TERMINATE:
            ret = ProcTerminate();
            break;
        case IAP_CMD_PLAY:
            ret = ProcPlay(replyData);
            break;
        case IAP_CMD_PAUSE:
            ret = ProcPause(replyData);
            break;
        case IAP_CMD_NEXT_TRACK:
            ret = ProcNextTrack(replyData);
            break;
        case IAP_CMD_PREVIOUS_TRACK:
            ret = ProcPreviousTrack(replyData);
            break;
        case IAP_CMD_FF:
            ret = ProcFF(replyData);
            break;
        case IAP_CMD_REW:
            ret = ProcRew(replyData);
            break;
        case IAP_CMD_GET_CURRENT_TRACKINDEX:
            ret = ProcGetCurrentTrackIndex(replyData);
            break;
        case IAP_CMD_GET_TOTAL_TRACK:
            ret = ProcGetTotalPlayTracks(replyData);
            break;
        case IAP_CMD_GET_TITLE:
            ret = ProcGetTitle(cmdData, replyData);
            break;
        case IAP_CMD_GET_ARTIST :
            ret = ProcGetArtist(cmdData, replyData);
            break;
        case IAP_CMD_GET_ALBUM :
            ret = ProcGetAlbum(cmdData, replyData);
            break;
        case IAP_CMD_GET_GENRE:
            ret = ProcGetGenre(cmdData, replyData);
            break;
        case IAP_CMD_GET_ALBUMART :
            ret = ProcGetAlbumArt(cmdData, replyData);
            break;
        case IAP_CMD_GET_DEVDSP_INFO:
            ret = ProcGetDeviceDisplayInfo(cmdData, replyData);
            break;
        case IAP_CMD_SET_DEVDSP_IMAGE:
            ret = ProcSetDeviceDisplayImage(cmdData, replyData);
            break;
        case IAP_CMD_DATABASE_INIT :
            ret = ProcDatabaseInit(cmdData, replyData);
            break;
        case IAP_CMD_DATABASE_SELECT_ALL :
            ret = ProcDatabaseSelectAll(cmdData,replyData);
            break;
        case IAP_CMD_DATABASE_SELECT :
            ret = ProcDatabaseSelect(cmdData, replyData);
            break;
        case IAP_CMD_DATABASE_MOVE_UPPER :
            ret = ProcDatabaseMoveUpper(cmdData,replyData);
            break;
        case IAP_CMD_DATABASE_GET_NAMES :
            ret = ProcDatabaseGetNames(cmdData, replyData);
            break;
        case IAP_CMD_GET_DR_PLAYSTATUS :
            ret = ProcGetDrPlayStatus(cmdData,replyData);
            break;
        case IAP_CMD_GET_PLAYSTATUS_INFO :
            ret = ProcGetPlayStatusInfo(cmdData,replyData);
            break;
        case IAP_CMD_GET_TRACKINFO :
            ret = ProcGetTrackInfos(cmdData,replyData);
            break;
        case IAP_CMD_GET_TRACKMETA:
            ret = ProcGetTrackMetas(cmdData,replyData);
            break;
        case IAP_CMD_TOGGLE_REPEAT :
            ret = ProcToggleRepeat(cmdData,replyData);
            break;
        case IAP_CMD_TOGGLE_SHUFFLE :
            ret = ProcToggleShuffle(cmdData,replyData);
            break;
        case IAP_CMD_SET_POSITION :
            ret = ProcSetTrackPosition(cmdData,replyData);
            break;
        case IAP_CMD_DISCONNECTED :
            ret = ProcDisconnected();
            break;                    
        case IAP_CMD_SET_CURRENT_PLAYINGTRACK:
            ret = ProcSetCurrentPlayingTrack(cmdData,replyData);
            break;
        case IAP_CMD_SET_REPEAT:
            ret = ProcSetRepeat(cmdData,replyData);
            break;
        case IAP_CMD_GET_REPEAT:
            ret = ProcGetRepeat(cmdData, replyData);
            break;
        case IAP_CMD_SET_SHUFFLE:
            ret = ProcSetShuffle(cmdData,replyData);
            break;
        case IAP_CMD_GET_SHUFFLE:
            ret = ProcGetShuffle(cmdData, replyData);
            break;
        case IAP_CMD_GET_INDEXEDPLAYINGINFO:
            ret = ProcGetIndexedPlayingInfo(cmdData, replyData);
            break;
        case IAP_CMD_GET_MODEL_NUM:
            ret = ProcGetModelNum(cmdData, replyData);
            break;
        case IAP_CMD_GET_PROTOCOL_VESION:
            ret = ProcGetProtocolVersion(cmdData, replyData);
            break;
        case IAP_CMD_GET_SOFTWARE_VERSION:
            ret = ProcGetIpodSoftwareVersion(cmdData, replyData);
            break;
        case IAP_CMD_GET_SERIAL_NUM:
            ret = ProcGetIpodSerialNumber(cmdData, replyData);
            break;
        case IAP_CMD_GET_IPOD_NAME:
            ret = ProcGetIpodName(cmdData, replyData);
            break; 
        case IAP_CMD_ENDFFREW:
            ret = ProcEndFfRew(replyData);
            break;
        case IAP_CMD_SEND_PACKEDLINGOCMD:
            ret = ProcSendPackedLingoCmd(cmdData,replyData);
            break;
        case IAP_CMD_ACCESSORYDATATRANSFER:
            ret = ProcAccessoryDataTransfer(cmdData, replyData,(unsigned long *)pData);
            break;
        case IAP_CMD_APPLICATIONLAUNCH:
            ret = ProcApplicationLaunch(cmdData, replyData,(String8 *)pData);
            break;
        case IAP_CMD_EXTRA_COMMAND:
            ret = ProcExtraCmd(cmdData, replyData);
            break;
        case IAP_CMD_CHK_READYSESSION:
            ret = ProcCheckReadySession(cmd->argInt);
            break;
        case IAP_CMD_GETNOWPLAYINGAPPBUNDLENAME:
            ret = ProcGetNowPlayingAppBundleName(cmdData, replyData);
            break;
        case IAP_CMD_PLAYCONTROL:
            ret = ProcResumeIpod(cmdData, replyData);
            break;
        case IAP_CMD_GETUIDTRACKINFO:
            ret = ProcGetUIDTrackInfo(cmdData, replyData);
            break;
        case IAP_CMD_GETDBTRACKINFO:
            ret = ProcGetDBTrackInfo(cmdData, replyData);
            break;
        case IAP_CMD_GETPBTRACKINFO:
            ret = ProcGetPBTrackInfo(cmdData, replyData);
            break;
        case IAP_CMD_GETIPODCAPS:
            ret = ProcGetIpodCaps(cmdData, replyData);
            break;
        case IAP_CMD_GETIPODFREESPACE:
            ret = ProcGetIpodFreeSpace(cmdData, replyData);
            break;
        case IAP_CMD_OPENIPODFEATUREFILE:
            ret = ProcOpenIpodFeatureFile(cmdData, replyData);
            break;
        case IAP_CMD_WRITEIPODFILEDATA:
            ret = ProcWriteIpodFileData(cmdData, replyData);
            break;
        case IAP_CMD_CLOSEIPODFILE:
            ret = ProcCloseIpodFile(cmdData, replyData);
            break;
        case IAP_CMD_GET_IAP_STATE:
            ret = GetIapState(replyData);
            break;
        case IAP_CMD_GET_ARTWORK_FORMATS:
            ret = ProcGetArtworkFormats(cmdData, replyData);
            break;
        case IAP_CMD_GET_ARTWORK_COUNT:
            ret = ProcGetArtworkCount(cmdData, replyData);
            break;
        case IAP_CMD_GET_ARTWORK_TIMES:
            ret = ProcGetArtworkTimes(cmdData, replyData);
            break;
        case IAP_CMD_GET_ARTWORK_DATA:
            ret = ProcGetArtworkData(cmdData, replyData);
            break;
        case IAP_CMD_EAP_PLAY:
            ret = ProcEapPlay(cmdData, replyData,(String8 *)pData);
            break;
        case IAP_CMD_EAP_PAUSE:
            ret = ProcEapPause(cmdData, replyData,(String8 *)pData);
            break;
        case IAP_CMD_EAP_GET_DR_PLAYSTATUS:
            ret = ProcEapGetDrPlayStatus(cmdData, replyData,(String8 *)pData);
            break;
        case IAP_CMD_GET_IAP_SVC_STATE:
            ret = GetIapSvcState(replyData);
            break;
        case IAP_CMD_NEWPLAY:
            ret = ProcNewPlay(replyData);
            break;
        case IAP_CMD_NEWPAUSE:
            ret = ProcNewPause(replyData);
            break;
        case IAP_CMD_NEXT:
            ret = ProcNext(replyData);
            break;
        case IAP_CMD_PREVIOUS:
            ret = ProcPrevious(replyData);
            break;
        /*
        case IAP_CMD_MUTECONTROL:
            ret = mIapService->ProcMuteControl(cmdData, replyData);
            break;
        */
    }
    return ret;
}

bool IapService::ProcExcuteCmd(int cmdID,
                                                            IAP_CMD_DATA_TYPE *cmd,
                                                            IAP_RSP_DATA_TYPE *rsp,
                                                            Parcel *replyData,
                                                            unsigned long *eapSessionId,
                                                            String8 *protocolName,
                                                            bool asyncCmd
                                                            )
{
    bool ret = false;
    bool isInvalidState = false;

    IAP_CMD_DATA_TYPE cmdNew;
    const Parcel *cmdData = cmd->data;

    if(replyData != NULL){
        if(cmdData != NULL){
            IAPSVC_DBUG("%s[%d] cmd 0x%x %d ",  __FUNCTION__,
                                                                            __LINE__, 
                                                                            cmdID - IAP_CMD_MASK,
                                                                            cmdID - IAP_CMD_MASK
                                                                            );
        }
    }else{
        ALOGE("%s[%d] cmd 0x%x %d msgCnt %ld replyData == NULL",        
                                                            __FUNCTION__,
                                                            __LINE__, 
                                                            cmdID - IAP_CMD_MASK,
                                                            cmdID - IAP_CMD_MASK,
                                                            cmd->argLong1
                                                            );
        return ret;
    } 

    rsp->what = cmdID & ~IAP_CMD_MASK;
    rsp->what |= IAP_RSP_MASK;    
    if(mIapCommandThread != NULL){
        if(asyncCmd == TRUE)
            mIapCommandThread->SetLastMsgCount(cmd->argLong1);
    }else{
        ALOGE("%s[%d] cmd 0x%x %d msgCnt %ld mIapCommandThread == NULL",        
                                                            __FUNCTION__,
                                                            __LINE__, 
                                                            cmdID - IAP_CMD_MASK,
                                                            cmdID - IAP_CMD_MASK,
                                                            cmd->argLong1
                                                            );
        isInvalidState = true;
    }

    if(cmdID != IAP_CMD_INIT && cmdID != IAP_CMD_DEINIT && cmdID != IAP_CMD_DISCONNECTED){
        if(mIapCommandThread->IsInvalidState(cmd->what,cmd->msgFlushSig)){
            IAPSVC_DEFLOG("%d: %s() continue",__LINE__,__FUNCTION__);
            isInvalidState = true;
        }
    }

    cmdNew.what= cmdID;
    if(isInvalidState == false){
        switch(cmdID){
            case IAP_CMD_ACCESSORYDATATRANSFER:
                rsp->ret = ProcDispatchCmd(&cmdNew,cmdData,replyData,(void *)eapSessionId);
                break;
            case IAP_CMD_APPLICATIONLAUNCH:
            case IAP_CMD_EAP_PLAY:
            case IAP_CMD_EAP_PAUSE:
            case IAP_CMD_EAP_GET_DR_PLAYSTATUS:
                rsp->ret = ProcDispatchCmd(&cmdNew,cmdData,replyData,(void *)protocolName);
                break;
            case IAP_CMD_WRAP_COMMAND:
                {
                    int wrapCmd = cmdData->readInt32();
                    String8 listenerName = cmdData->readString8();
                    
                    cmdNew.what = wrapCmd;// & ~IAP_CMD_MASK;
                    IAPSVC_DBUG("%d: %s() wrapCmd %d %s",__LINE__,__FUNCTION__,cmdNew.what,listenerName.string());
                    rsp->ret = ProcDispatchCmd(&cmdNew,cmdData,replyData,NULL);
                    rsp->what = wrapCmd & ~IAP_CMD_MASK;
                    rsp->what |= IAP_RSP_MASK;    
                }
                break;
            default:
                rsp->ret = ProcDispatchCmd(&cmdNew,cmdData,replyData,NULL);
                break;
        }
    }else{
        IAPSVC_DEFLOG("%d: %s() return false cmd.what %d msgCnt %ld", 
                                                                        __LINE__, 
                                                                        __FUNCTION__,
                                                                        cmd->what - IAP_CMD_MASK,
                                                                        cmd->argLong1
                                                                        );
        switch(cmd->what){
            case IAP_CMD_ACCESSORYDATATRANSFER:
                rsp->ret = ProcResponseFailBySessionId(cmdData, replyData,eapSessionId);
                break;     
            case IAP_CMD_APPLICATIONLAUNCH:
                String8(cmdData->readString16());
            case IAP_CMD_EAP_PLAY:
            case IAP_CMD_EAP_PAUSE:
            case IAP_CMD_EAP_GET_DR_PLAYSTATUS:
                rsp->ret = ProcResponseFailByProtocolName(cmdData, replyData,protocolName);
                break;
            case IAP_CMD_WRAP_COMMAND:
                rsp->ret = ProcResponseFailByListenerName(cmd,rsp,cmdData, replyData,protocolName);
                break;
            case IAP_CMD_ASYNC_COMMAND:
                rsp->what = cmd->argInt2 & ~IAP_CMD_MASK;
                rsp->what |= IAP_RSP_MASK;    
                IAPSVC_DEFLOG("%s() return false async command real cmd %d", 
                                                                    __FUNCTION__,
                                                                    cmd->argInt2
                                                                    );
                rsp->ret = false;
                break;
            default:
                rsp->ret = false;
                break;
        }
        replyData->writeInt32(0);
    }

    //mIapCommandThread->SetLastReqRsp(rsp->what);
    ret = true;

    return ret;
}


void IapService::ProcParseCmdWithCb(IAP_CMD_DATA_TYPE *cmd)
{
    bool ret = false;
    const sp<IIapServiceCallback>& cbFunc = cmd->cbFunc;
    IAP_RSP_DATA_TYPE rsp;
    Parcel replyAsync;
    Parcel *replyData = cmd->reply;
    int cmdID = cmd->argInt2;
    unsigned long eapSessionId = 0;
    String8 protocolName;
    
    if(cmd->async)
        replyData = &replyAsync;
    ret = ProcExcuteCmd(cmdID,cmd, &rsp, replyData,&eapSessionId,&protocolName,TRUE);
    mIapCommandThread->SetLastReqRsp(rsp.what);

    if(ret == false){
        ALOGE("%s[%d] what %d msgCnt %ld ", __FUNCTION__,__LINE__,
                                                                    cmd->what - IAP_CMD_MASK,
                                                                    cmd->argLong1);
    }else{
        if( cmd->cbFunc != NULL){
            ProcCallCb(cmd->cbFunc,replyData);
        }else{
            ALOGE("%s [%d] ", __FUNCTION__, __LINE__);
        }                
    }

    if( cmd->cbFunc != NULL)
        cmd->cbFunc = NULL;
}

IapService::IapStatusManager::IapStatusManager(IapService *iapService,void *timer)
        : mIapService(iapService) , 
        mCurrentIndex(-1), 
        mLocalPlayStatus(IPOD_DR_PLAYBACK_STOPPED),
        mApplePlayStatus(IPOD_DR_PLAYBACK_STOPPED),
        mLocalFFRewStatus(false),
        mAppleFFRewStatus(false),
        mExpectNextStatus(IPOD_DR_PLAYBACK_STOPPED),
        mIapTimer(timer)
{
    
}

void IapService::IapStatusManager::ClearAll(void)
{
}

void IapService::IapStatusManager::SetWantedNextAction(int action)
{
    IAPSVC_DBUG("%s() action %d", __FUNCTION__,action);
    mExpectNextStatus = action;
}

int IapService::IapStatusManager::GetWantedNextAction(void)
{
    return mExpectNextStatus;
}


void IapService::IapStatusManager::SetExpectNextStatus(int status,int sendNoti,unsigned int cmdFrom)
{
    unsigned int timerType = (unsigned int)(-1);
    time_t delaySec = 0;
    long delayMsec = 0;
    mExpectNextStatus = status;
    IAPSVC_DBUG("%s() status %d", __FUNCTION__,status);
    #ifdef USE_TIMER
    switch(status){
        case IPOD_DR_PLAYING:
            timerType = IPOD_TM_TYPE_PLAYING;
            delaySec = 3;
            break;
        case IPOD_DR_PLAYBACK_PAUSED:
            timerType = IPOD_TM_TYPE_PAUSE;
            delaySec = 3;
            break;
        case IPOD_DR_FAST_FORWARD:
            timerType = IPOD_TM_TYPE_FF;
            delaySec = 3;
            break;
        case IPOD_DR_FAST_REWIND:
            timerType = IPOD_TM_TYPE_REW;
            delaySec = 3;
            break;
        case IPOD_DR_END_FAST_FORWARD_OR_REWIND:
            timerType = IPOD_TM_TYPE_ENDFFREW;
            delaySec = 3;
            break;
    }

    if((int)timerType >= 0){
        iPod_Timer_CreateTimerEntry(mIapTimer, 
                                                            timerType, 
                                                            delaySec, 
                                                            delayMsec, 
                                                            IPOD_Timer_Cb_Cancel, 
                                                            IPOD_Timer_Cb_Expired, 
                                                            NULL,
                                                            sendNoti,
                                                            cmdFrom);
    }
    #endif
}

int IapService::IapStatusManager::GetExpectNextStatus(void)
{
    return mExpectNextStatus;
}

void IapService::IapStatusManager::ClearExpectNextStatus(void)
{
    mExpectNextStatus = -1;
}

void IapService::IapStatusManager::SetLocalPlayStatus(int status,int sendNoti,unsigned int cmdFrom)
{
    SetLocalPlayStatus(status, true,sendNoti,cmdFrom);
}

void IapService::IapStatusManager::SetLocalPlayStatus(int status,bool setExpected,int sendNoti,unsigned int cmdFrom)
{
    IAPSVC_DBUG("%s() status %d", __FUNCTION__,status);
    mLocalPlayStatus = status;
    if(setExpected)
        SetExpectNextStatus(status,sendNoti,cmdFrom);
}

int IapService::IapStatusManager::GetLocalPlayStatus(void)
{
    return mLocalPlayStatus;
}

void IapService::IapStatusManager::SetApplePlayStatus(int status)
{
    bool updateLocalStatus = true;;
    int localPlayStatus = GetLocalPlayStatus();
    int nextAction = GetWantedNextAction();
    IAPSVC_DEFLOG("%s() status %d expected status %d localPlayStatus %d", 
                                                                            __FUNCTION__,
                                                                            status,
                                                                            GetExpectNextStatus(),
                                                                            localPlayStatus
                                                                            );
    mApplePlayStatus = status;
    ClearExpectNextStatus();
    switch(status){
        case IPOD_DR_PLAYING:
            #ifdef USE_TIMER
            iPod_Timer_CancelTimerEntry(mIapTimer, IPOD_TM_TYPE_PLAYING|IPOD_TM_TYPE_ENDFFREW);
            #endif
            IAPSVC_DBUG("%s() localStatus %d wantedNextAction %d", 
                                                                            __FUNCTION__,
                                                                            localPlayStatus,
                                                                            nextAction
                                                                            );
            if(localPlayStatus == IPOD_DR_END_FAST_FORWARD_OR_REWIND &&
                nextAction == IPOD_DR_PLAYBACK_PAUSED ){
                if(mIapService->SendNextAction(PLAYPAUSE)){
                    SetLocalPlayStatus(IPOD_DR_PLAYBACK_PAUSED,0,0);
                    updateLocalStatus  = false;
                }
            }
            break;
        case IPOD_DR_PLAYBACK_PAUSED:
            #ifdef USE_TIMER
            iPod_Timer_CancelTimerEntry(mIapTimer, IPOD_TM_TYPE_PAUSE|IPOD_TM_TYPE_ENDFFREW);
            #endif
            IAPSVC_DBUG("%s() localStatus %d wantedNextAction %d", 
                                                                            __FUNCTION__,
                                                                            localPlayStatus,
                                                                            nextAction
                                                                            );
            if(localPlayStatus == IPOD_DR_END_FAST_FORWARD_OR_REWIND &&
                nextAction == IPOD_DR_PLAYING ){
                if(mIapService->SendNextAction(PLAYPAUSE)){
                    SetLocalPlayStatus(IPOD_DR_PLAYING,0,0);
                    updateLocalStatus  = false;
                }
            }
                
            break;
        case IPOD_DR_FAST_FORWARD:
            #ifdef USE_TIMER
            iPod_Timer_CancelTimerEntry(mIapTimer, IPOD_TM_TYPE_FF);
            #endif
            SetAppleFFRewStatus(true);
            break;
        case IPOD_DR_FAST_REWIND:
            #ifdef USE_TIMER
            iPod_Timer_CancelTimerEntry(mIapTimer, IPOD_TM_TYPE_REW);
            #endif
            SetAppleFFRewStatus(true);
            break;
        default:
            break;
    }
    if(updateLocalStatus)
        SetLocalPlayStatus(status,false,0,0);
}

int IapService::IapStatusManager::GetApplePlayStatus(void)
{
    return mApplePlayStatus;
}

void IapService::IapStatusManager::SetLocalFFRewStatus(bool status)
{
    IAPSVC_DBUG("%s() status %d", __FUNCTION__,status);
    mLocalFFRewStatus = status;
}

bool IapService::IapStatusManager::GetLocalFFRewStatus(void)
{
    return mLocalFFRewStatus;
}

void IapService::IapStatusManager::SetAppleFFRewStatus(bool status)
{
    bool localStatus = GetLocalFFRewStatus();
    IAPSVC_DBUG("%s() status %d localStatus %d", __FUNCTION__,status,localStatus);
    mAppleFFRewStatus = status;
    if(localStatus == true && status == false){
        IAPSVC_DBUG("%s() send EndFFRew", __FUNCTION__);
        if(mIapService->SendNextAction(ENDFFREW)){
            SetLocalFFRewStatus(false);
        }
    }
}

bool IapService::IapStatusManager::GetAppleFFRewStatus(void)
{
    return mAppleFFRewStatus;
}

void IapService::IapCommandThread::incMsgFlushSig(void)
{
    Mutex::Autolock lock(mMsgSyncSigLock);
    mMsgFlushSig++;
}

unsigned int IapService::IapCommandThread::getMsgFlushSig(void)
{
    Mutex::Autolock lock(mMsgSyncSigLock);
    return mMsgFlushSig;
}

long IapService::IapCommandThread::getMsgCount(void)
{
    return mMsgCount++;
}

void IapService::IapCommandThread::requestExit()
{
    // must be in this order to avoid a race condition
    Thread::requestExit();
    mCommandTreadExit = true;
}

bool IapService::IapCommandThread::waitResponse()
{
    ssize_t res = 0;
    IAP_RSP_DATA_TYPE rsp;
    //ALOGE(" waitResponse res %d",(int)res);
    res = readResponse((void *)&rsp, 1);
    //ALOGE(" readResponse res %d",(int)res);
    if (res == 0) {
        //ALOGE(" waitResponse: read res %d what %d",(int)res,rsp.what);
        res = waitForRsp();
        //ALOGE(" waitResponse: waitForEvent res %d",(int)res);
        if (res != NO_ERROR)
            return false;
        // here we're guaranteed to have an event
        res = readResponse((void *)&rsp, 1);
        //ALOGE(" waitResponse: readMessage2 res %d",(int)res);
    }
    return rsp.ret;
}

bool IapService::IapCommandThread::sendCommandMessage(IAP_CMD_DATA_TYPE *pCmd)
{
    #if 1
    long msgCount = getMsgCount();
    IAPSVC_DBUG("%d: %s() >> command sendMsg  what %d async %d msgCount %ld ", 
                                                                                                    __LINE__, 
                                                                                                    __FUNCTION__,
                                                                                                    pCmd->what,
                                                                                                    pCmd->async,
                                                                                                    msgCount
                                                                                                    );
    #endif
    {
        IPOD_ERROR retval = IPOD_NO_ERROR;
        size_t res = 0;
        //Mutex::Autolock lock(mCommandLock);
        pCmd->msgFlushSig = getMsgFlushSig();
        pCmd->argLong1 = msgCount;
        if(pCmd->what == IAP_CMD_ASYNC_COMMAND){
            mLastSendCmd = pCmd->argInt2;
        }else{
            mLastSendCmd = pCmd->what;
        }

        #ifdef IAP_USE_TCC_QUEUE
        retval = IPOD_OS_SendToMessageQueue(QueueCmd,pCmd,sizeof(IAP_CMD_DATA_TYPE));
        if(retval != IPOD_NO_ERROR){
            ALOGE(" %s: sendObjects failed ~~! res %d  lastCmd %d notifyRsp %d currentMsgCnt %ld lastMsgCnt %ld",
                                                                            __FUNCTION__,
                                                                            (int)retval,
                                                                            mLastReqCmd - IAP_CMD_MASK,
                                                                            mLastReqRsp - IAP_RSP_MASK,
                                                                            msgCount,
                                                                            mLastMsgCount
                                                                            );
            return false;
        }
        #else
        res = BitTube::sendObjects(mCommand, pCmd, 1);
        if(res != 1){
            ALOGE(" %s: sendObjects failed ~~! res %d lastCmd %d notifyRsp %d currentMsgCnt %ld lastMsgCnt %ld",
                                                                            __FUNCTION__,
                                                                            (int)res,
                                                                            mLastReqCmd - IAP_CMD_MASK,
                                                                            mLastReqRsp - IAP_RSP_MASK,
                                                                            msgCount,
                                                                            mLastMsgCount
                                                                            );
            return false;
        }
        #endif

    }
    if(pCmd->async == false){
        return waitResponse();
    }
    else
    {
        if(pCmd->reply != NULL)
            pCmd->reply->writeInt32(true);
    }

    return true;
}

bool IapService::IapCommandThread::sendCommandArg1(
                                                                                    int what,
                                                                                    int data,
                                                                                    bool async
                                                                                    )
{
    bool ret;
    IAP_CMD_DATA_TYPE *cmd 
        = (IAP_CMD_DATA_TYPE *)IPOD_MemAlloc(sizeof(IAP_CMD_DATA_TYPE));
    memset(cmd,0x00,sizeof(IAP_CMD_DATA_TYPE));
    cmd->what = what;
    cmd->argInt = data;
    cmd->async = async;
    cmd->cbFunc= NULL;
    
    ret = sendCommandMessage(cmd);
    IPOD_MemDealloc(cmd);
    return ret;
}

bool IapService::IapCommandThread::sendCommandCbFunc(
                                                                                    int what,
                                                                                    const Parcel *data,
                                                                                    Parcel *reply,
                                                                                    bool async,
                                                                                    int cmdID,
                                                                                    const sp<IIapServiceCallback>& cbFunc
                                                                                    )
{
    bool ret;
    IAP_CMD_DATA_TYPE *cmd 
        = (IAP_CMD_DATA_TYPE *)IPOD_MemAlloc(sizeof(IAP_CMD_DATA_TYPE));
    memset(cmd,0x00,sizeof(IAP_CMD_DATA_TYPE));
    cmd->what = what;
    cmd->reply = reply;
    cmd->argInt2 = cmdID;
    cmd->async = async;
    cmd->cbFunc = cbFunc;
    if(data != NULL && data->dataSize() > 0){
        int pos = data->dataPosition();
        Parcel *msgParcel = new Parcel();
        msgParcel->appendFrom(data, pos, data->dataSize() - pos);
        msgParcel->setDataPosition(0);
        cmd->data = msgParcel;
        //ALOGE(" %s: data %p",__FUNCTION__,cmd->data);
    }else{
        cmd->data = data;
    }
    ret = sendCommandMessage(cmd);
    IPOD_MemDealloc(cmd);
    return ret;
}


bool IapService::IapCommandThread::sendCommand(
                                                                                    int what,
                                                                                    const Parcel *data,
                                                                                    Parcel *reply,
                                                                                    bool async
                                                                                    )
{
    bool ret;
    IAP_CMD_DATA_TYPE *cmd 
        = (IAP_CMD_DATA_TYPE *)IPOD_MemAlloc(sizeof(IAP_CMD_DATA_TYPE));
    memset(cmd,0x00,sizeof(IAP_CMD_DATA_TYPE));
    cmd->what = what;
    cmd->reply = reply;
    cmd->async = async;
    cmd->cbFunc= NULL;
    if(data != NULL && data->dataSize() > 0){
        int pos = data->dataPosition();
        Parcel *msgParcel = new Parcel();
        msgParcel->appendFrom(data, pos, data->dataSize() - pos);
        msgParcel->setDataPosition(0);
        cmd->data = msgParcel;
        //ALOGE(" %s: data %p",__FUNCTION__,cmd->data);
    }else{
        cmd->data = data;
    }
    ret = sendCommandMessage(cmd);
    IPOD_MemDealloc(cmd);
    return ret;
}

ssize_t IapService::IapCommandThread::readCommand(void *msg, size_t numEvents)
{
    #ifdef IAP_USE_TCC_QUEUE
    ssize_t ret = 0;
    IPOD_ERROR retval = IPOD_NO_ERROR;
    unsigned long readSize = 0;
    unsigned long bufSize = sizeof(IAP_CMD_DATA_TYPE)*numEvents;
    unsigned long timeout = (unsigned long)(-1);
    retval = IPOD_OS_ReadMessageQueue(QueueCmd,msg,bufSize,&readSize,timeout,NULL);
    if(retval == IPOD_NO_ERROR){
        if(readSize == bufSize)
            ret = 1;
        else
            ALOGE(" %s: readSize %d",__FUNCTION__,(int)readSize);
    }else{
        ALOGE(" %s: retval 0x%x",__FUNCTION__,(int)retval);
    }
    return ret;
    #else
    //Mutex::Autolock lock(mCommandLock);
    return BitTube::recvObjects(mCommand, (IAP_CMD_DATA_TYPE *)msg, numEvents);
    #endif
}

bool IapService::IapCommandThread::sendResponse(IAP_RSP_DATA_TYPE *rsp)
{
    //IAPSVC_DBUG("%d: %s() >> response sendMsg ", __LINE__, __FUNCTION__);
    size_t res = 0;
    #ifdef IAP_USE_TCC_QUEUE
    IPOD_ERROR retval;
    retval = IPOD_OS_SendToMessageQueue(QueueRsp,rsp,sizeof(IAP_RSP_DATA_TYPE));
    if(retval != IPOD_NO_ERROR){
        ALOGE(" %s: sendObjects failed ~~! res %d ",__FUNCTION__,(int)retval);
        return false;
    }
    #else
    res = BitTube::sendObjects(mResponse, rsp, 1);
    if(res != 1)
        ALOGE(" %s: sendObjects res %d",__FUNCTION__,(int)res);
    #endif
    return true;
}

ssize_t IapService::IapCommandThread::readResponse(void *msg, size_t numEvents)
{
    //IAPSVC_DBUG("%d: %s() numEvents %d", __LINE__, __FUNCTION__,(int)numEvents);
    #ifdef IAP_USE_TCC_QUEUE
    ssize_t ret = 0;
    IPOD_ERROR retval = IPOD_NO_ERROR;
    unsigned long readSize = 0;
    unsigned long bufSize = sizeof(IAP_RSP_DATA_TYPE)*numEvents;
    unsigned long timeout = (unsigned long)(-1);
    retval = IPOD_OS_ReadMessageQueue(QueueRsp,msg,bufSize,&readSize,timeout,NULL);
    if(retval == IPOD_NO_ERROR){
        if(readSize == bufSize)
            ret = 1;
        else
            ALOGE(" %s: readSize %d",__FUNCTION__,(int)readSize);
    }else{
        ALOGE(" %s: retval 0x%x",__FUNCTION__,(int)retval);
    }
    return ret;
    #else
    return BitTube::recvObjects(mResponse, (IAP_RSP_DATA_TYPE *)msg, numEvents);
    #endif
}


int IapService::IapCommandThread::getCmdFd() const
{
    return mCommand->getFd();
}

sp<Looper> IapService::IapCommandThread::getCmdLooper() const
{
    Mutex::Autolock _l(mLooperLock);
    if (mLooper == 0) {
        //ALOGE(" %s: make new looper",__FUNCTION__);
        mLooper = new Looper(true);
        mLooper->addFd(getCmdFd(), getCmdFd(), ALOOPER_EVENT_INPUT, NULL, NULL);
    }
    return mLooper;
}

status_t IapService::IapCommandThread::waitForCmd() const
{
    #ifdef IAP_USE_TCC_QUEUE
    return (-1);
    #else
    const int fd = getCmdFd();
    sp<Looper> looper(getCmdLooper());

    int events;
    int32_t result;
    
    //ALOGE(" %s: wait start ~~~~~~~~~~",__FUNCTION__);
    do {
        result = looper->pollOnce(-1, NULL, &events, NULL);
        if (result == ALOOPER_POLL_ERROR) {
            //ALOGE(" %s (errno=%d)", __FUNCTION__,errno);
            result = -EPIPE; // unknown error, so we make up one
            break;
        }
        if (events & ALOOPER_EVENT_HANGUP) {
            // the other-side has died
            //ALOGE("%s error HANGUP", __FUNCTION__);
            result = -EPIPE; // unknown error, so we make up one
            break;
        }
    } while (result != fd);
    //ALOGE(" %s: wait end ~~~~~~~~~~  status_t(NO_ERROR) %d  result %d fd %d",__FUNCTION__, status_t(NO_ERROR) ,result,fd);
    return  (result == fd) ? status_t(NO_ERROR) : result;
    #endif
}

status_t IapService::IapCommandThread::wakeCmd() const
{
    sp<Looper> looper(getCmdLooper());
    looper->wake();
    return NO_ERROR;
}

int IapService::IapCommandThread::getRspFd() const
{
    return mResponse->getFd();
}

sp<Looper> IapService::IapCommandThread::getRspLooper() const
{
    Mutex::Autolock _l(mLooperRspLock);
    if (mLooperRsp == 0) {
        //ALOGE(" %s: make new looper",__FUNCTION__);
        mLooperRsp = new Looper(true);
        mLooperRsp->addFd(getRspFd(), getRspFd(), ALOOPER_EVENT_INPUT, NULL, NULL);
    }
    return mLooperRsp;
}

status_t IapService::IapCommandThread::waitForRsp() const
{
    #ifdef IAP_USE_TCC_QUEUE
    return (-1);
    #else
    const int fd = getRspFd();
    sp<Looper> looper(getRspLooper());

    int events;
    int32_t result;
    //ALOGE(" %s: wait start ~~~~~~~~~~",__FUNCTION__);
    do {
        result = looper->pollOnce(-1, NULL, &events, NULL);
        if (result == ALOOPER_POLL_ERROR) {
            //ALOGE(" %s (errno=%d)", __FUNCTION__,errno);
            result = -EPIPE; // unknown error, so we make up one
            break;
        }
        if (events & ALOOPER_EVENT_HANGUP) {
            // the other-side has died
            //ALOGE("%s error HANGUP", __FUNCTION__);
            result = -EPIPE; // unknown error, so we make up one
            break;
        }
    } while (result != fd);
    //ALOGE(" %s: wait end ~~~~~~~~~~",__FUNCTION__);
    return  (result == fd) ? status_t(NO_ERROR) : result;
    #endif
}

status_t IapService::IapCommandThread::wakeRsp() const
{
    sp<Looper> looper(getRspLooper());
    looper->wake();
    return NO_ERROR;
}

bool IapService::IapCommandThread::IsInvalidState(int what,unsigned int msgFlushSig)
{
    bool ret = false;
    if(mIapService->IsInitialized() == false){
        IAPSVC_DEFLOG("%d: %s() IsInitialized == false ", 
                                                                    __LINE__, 
                                                                    __FUNCTION__
                                                                    );
        return true;
    }
        
    if(mIapService->GetIPodHandle() != NULL){
        if(IPOD_GetDetectStatus(
            IPOD_GetExtInfo(mIapService->GetIPodHandle())) != IPOD_CONNECTED)
        {
            IAPSVC_DEFLOG("%d: %s() cmd.what %d ", 
                                                                        __LINE__, 
                                                                        __FUNCTION__,
                                                                        what - IAP_CMD_MASK
                                                                        );
            return true;
        }
    }else{
        IAPSVC_DEFLOG("%d: %s() GetIPodHandle == NULL ", 
                                                                    __LINE__, 
                                                                    __FUNCTION__
                                                                    );
        return true;
    }

    if(ret == false){
        unsigned int flushSig = getMsgFlushSig();
        if(flushSig != msgFlushSig){
            IAPSVC_DEFLOG("%d: %s() flushSig 0x%x cmd.SyncSig =0x%x ", 
                                                                        __LINE__, 
                                                                        __FUNCTION__,
                                                                        flushSig,
                                                                        msgFlushSig
                                                                        );
            return true;
        }
    }
    return ret;
}

void IapService::IapCommandThread::WaitUntilCmdFinish()
{
    IAPSVC_DEFLOG("%d: %s() mLastSendCmd %d lastReqCmd %d lastRsp %d syncCmd %d syncRsp %d mLastMsgCount %ld mMsgCount %ld",
                                                                            __LINE__,
                                                                            __FUNCTION__,
                                                                            mLastSendCmd - IAP_CMD_MASK,
                                                                            mLastReqCmd - IAP_CMD_MASK,
                                                                            mLastReqRsp - IAP_RSP_MASK,
                                                                            mLastSyncReqCmd - IAP_CMD_MASK,
                                                                            mLastSyncReqRsp -  IAP_CMD_MASK,
                                                                            mLastMsgCount,
                                                                            mMsgCount
                                                                            );
    {
        Mutex::Autolock lock(mCmdProcessLock);
    }
}

void IapService::IapCommandThread::CommandInit()
{
    mMsgCount = 0;
    #ifdef IAP_USE_TCC_QUEUE
    void *tmp = NULL;
    IAPSVC_DBUG("%d: %s() continue",__LINE__,__FUNCTION__);
     
    IPOD_OS_CreateMessageQueue(&QueueCmd,&QueueCmd,(unsigned char *)"QueueCmd");
    IPOD_OS_CreateMessageQueue(&QueueRsp,&QueueRsp,(unsigned char *)"QueueRsp");
    #endif
}

void IapService::IapCommandThread::CommandDeinit()
{
    #ifdef IAP_USE_TCC_QUEUE
    void *tmp = NULL;
    IAPSVC_DBUG("%d: %s() continue",__LINE__,__FUNCTION__);
    //IPOD_OS_SensSig();
    IPOD_OS_CloseMessageQueue(QueueCmd,QueueCmd);
    QueueCmd = NULL;
    IPOD_OS_CloseMessageQueue(QueueRsp,QueueRsp);
    QueueRsp = NULL;
    IAPSVC_DBUG("%d: %s() end",__LINE__,__FUNCTION__);
    #endif
}

bool IapService::IapCommandThread::DispatchCmd(IAP_CMD_DATA_TYPE *cmd,const Parcel *cmdData,Parcel *replyData,void *pData)
{
    bool ret = false;

    if(mIapService != NULL){
        ret = mIapService->ProcDispatchCmd(cmd, cmdData,replyData,pData);
    }

    return ret;
}

void IapService::IapCommandThread::SetLastMsgCount(long count)
{
    mLastMsgCount = count;
}

void IapService::IapCommandThread::SetLastReqRsp(int rsp)
{
    mLastReqRsp = rsp;
}

void IapService::IapCommandThread::SetLastSyncReqCmd(int cmd)
{
    mLastSyncReqCmd = cmd;
}

void IapService::IapCommandThread::SetLastSyncReqRsp(int rsp)
{
    mLastSyncReqRsp = rsp;
}

void IapService::IapCommandThread::ParseCmdWithCb(IAP_CMD_DATA_TYPE *cmd)
{
    if(mIapService != NULL){
        mIapService->ProcParseCmdWithCb(cmd);
    }
}

void IapService::IapCommandThread::ParseCmd(IAP_CMD_DATA_TYPE *cmd)
{
    bool ret = false;
    IAP_RSP_DATA_TYPE rsp;
    unsigned long eapSessionId = 0;
    String8 protocolName;
    int cmdID = cmd->what;
    Parcel replyAsync;
    Parcel *replyData = cmd->reply;
    if(cmd->async)
        replyData = &replyAsync;
    
    ret = mIapService->ProcExcuteCmd( cmdID,cmd, &rsp, replyData, &eapSessionId, &protocolName,TRUE);

    if(ret == false){
        ALOGE("%s[%d] what %d msgCnt %ld ", __FUNCTION__,__LINE__,
                                                                    cmd->what, cmd->argLong1);
    }else{
        if(cmd->async == true){
            switch(cmd->what){
                case IAP_CMD_INIT:
                    mIapService->notifyObjConnect(rsp.what, rsp.ret, 0, replyData);
                    break;
                case IAP_CMD_ACCESSORYDATATRANSFER:
                    mIapService->notifyObjEap(rsp.what, rsp.ret, 0, replyData,eapSessionId);
                    break;
                case IAP_CMD_APPLICATIONLAUNCH:
                case IAP_CMD_EAP_PLAY:
                case IAP_CMD_EAP_PAUSE:
                case IAP_CMD_EAP_GET_DR_PLAYSTATUS:
                    mIapService->notifyObjEapByName(rsp.what, rsp.ret, 0, replyData,protocolName);
                    break;
                case IAP_CMD_CHK_READYSESSION:
                    break;
                case IAP_CMD_GETIPODCAPS:
                case IAP_CMD_GETIPODFREESPACE:
                case IAP_CMD_OPENIPODFEATUREFILE:
                case IAP_CMD_WRITEIPODFILEDATA:
                case IAP_CMD_CLOSEIPODFILE:
                    mIapService->notifyObjTag(rsp.what, rsp.ret, 0, replyData);
                    break;
                case IAP_CMD_WRAP_COMMAND:
                    mIapService->notifyObjByName(rsp.what, rsp.ret, 0, replyData, protocolName);
                    break;
                default:
                    mIapService->notifyObj(rsp.what, rsp.ret, 0, replyData);
                    break;
            }
        }else{
            sendResponse(&rsp);
        }
    }
}

int IapService::IapCommandThread::CommandRun()
{
    status_t res;
    IAP_CMD_DATA_TYPE cmd;
    //int startPosCmd = 0;

    //ALOGE(" CommandRun: start ~~~~~~~~~~");
    while(mCommandTreadExit == false){
        //usleep(100*1000);
        memset(&cmd,0x00,sizeof(IAP_CMD_DATA_TYPE));
        res = readCommand((void *)&cmd, 1);
        //ALOGE(" CommandRun: readCommand res %d",res);
        if (res == 0) {
            //ALOGE(" CommandRun: read res %d ",res);
            res = waitForCmd();
            //ALOGE(" CommandRun: waitForEvent res %d",res);
            if (res != NO_ERROR){
                ALOGE(" CommandRun: waitForCmd res %d",res);
                continue;
            }
            // here we're guaranteed to have an event
            res = readCommand((void *)&cmd, 1);
            if(res != 1)
                ALOGE(" CommandRun: readCommand2 res %d",res);
        }else{
            //usleep(100*1000);
        }
        if(res == 1)
            mLastReqCmd = cmd.what;

        #if 0
        if(IsInvalidState(cmd.what,cmd.msgFlushSig) && cmd.what != IAP_CMD_INIT){
            IAPSVC_DBUG("%d: %s() continue",__LINE__,__FUNCTION__);
        }else
        #endif
        {
            int reqCmd = cmd.what;
            if(cmd.what == IAP_CMD_ASYNC_COMMAND){
                reqCmd = cmd.argInt2;
                mLastReqCmd = reqCmd;
            }
            //if((reqCmd != 0 && mIapService->IsInitialized()) || (reqCmd == IAP_CMD_INIT))
            if(reqCmd != 0)
            {
                long argLong1 = cmd.argLong1;
                IAPSVC_DDBUG("%d: %s() start cmd.what %d cmdData msgCnt %ld", 
                                                                                        __LINE__, 
                                                                                        __FUNCTION__,
                                                                                        cmd.what - IAP_CMD_MASK,
                                                                                        argLong1
                                                                                        );
                Mutex::Autolock lock(mCmdProcessLock);
                if(cmd.what == IAP_CMD_ASYNC_COMMAND){
                    ParseCmdWithCb(&cmd);
                }else{
                    ParseCmd(&cmd);
                }
                IAPSVC_DDBUG("%d: %s() end cmd.what %d cmdData msgCnt %ld", 
                                                                                        __LINE__, 
                                                                                        __FUNCTION__,
                                                                                        cmd.what - IAP_CMD_MASK,
                                                                                        argLong1
                                                                                        );
            }else{
                long argLong1 = cmd.argLong1;
                IAPSVC_DBUG("%d: %s() cmd.what %d cmdData msgCnt %ld", 
                                                                                        __LINE__, 
                                                                                        __FUNCTION__,
                                                                                        cmd.what - IAP_CMD_MASK,
                                                                                        argLong1
                                                                                        );
            }
        }
        //IAPSVC_DBUG("%d: %s()" ,__LINE__,__FUNCTION__);
        if(cmd.data != NULL){
            delete cmd.data;
        }
        if (res <= 0) {
            //return -1;
        }        
        //IAPSVC_DBUG("%d: %s()" ,__LINE__,__FUNCTION__);
    }
    //ALOGE(" CommandRun: finish ~~~~~~~~~~");
    return 0;
}

void IapService::IapEventThread::requestExit()
{
    // must be in this order to avoid a race condition
    Thread::requestExit();
    mEventTreadExit = true;
}

int IapService::IapEventThread::EventRun()
{
    EVENTPROCESSOR_STATE iPodEventState = IPOD_STATE_INITIALIZED;
    bool IsSuccess = false;
    IPOD_DR_PLAY_STATUS Rd_playStatus;

    void *EventQueueRead = NULL;
    IPOD_EVT_msg EventMsg;
    unsigned long ReadSize;
    unsigned long flags;
    IPOD_ERROR err;
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);

    while ( IPOD_STATE_TERMINATED != iPodEventState && mEventTreadExit == false) {
		
        if(EventQueueRead == NULL)
        {
            EventQueueRead = IPOD_GetEventMsgReadQueue(mIapService->GetIPodHandle());
            usleep(100*1000);
            if(EventQueueRead != NULL)
                IAPSVC_DBUG("%d: %s() EventQueueInit", __LINE__, __FUNCTION__);
        }
        else
        {
            err = IPOD_OS_ReadMessageQueue(
                                                                    EventQueueRead,
                                                                    &EventMsg,
                                                                    sizeof(IPOD_EVT_msg),
                                                                    &ReadSize,
                                                                    10,
                                                                    &flags
                                                                    );
            if(err == IPOD_NO_ERROR)
            {
                IsSuccess = false;
                //MsgToUI.wParam = 0;
                //MsgToUI.lParam = 0;
                //IAPSVC_DBUG("%d: %s()  EventId = 0x%x", __LINE__, __FUNCTION__,EventMsg.EventID);
                switch( EventMsg.EventID ){
                    case IPOD_APP_EVT_STOPPED:
                        IAPSVC_DEFLOG("> IPOD_APP_EVT_STOPPED > param1 =0x%lx, param2=0x%lx", 
                        							EventMsg.Param1, EventMsg.Param2);
                        break;

                    case IPOD_APP_EVT_TRACKCHANGED:
                        IAPSVC_DEFLOG("> IPOD_APP_EVT_TRACKCHANGED > param1 =0x%lx, param2=0x%lx p3 0x%lx",  
                        							EventMsg.Param1, EventMsg.Param2, EventMsg.Param3);
                        //param1 : changed index
                        mIapService->notify(IPOD_APP_EVT_TRACKCHANGED,EventMsg.Param1,0,0);
                        break;
                        
                    case IPOD_APP_DR_EVT_TRACK_INDEX:
                        IAPSVC_DEFLOG("> IPOD_APP_DR_EVT_TRACK_INDEX > param1 =0x%lx, param2=0x%lx p3 0x%lx",  
                        							EventMsg.Param1, EventMsg.Param2, EventMsg.Param3);
                        //param1 : changed index
                        mIapService->notify(IPOD_APP_DR_EVT_TRACK_INDEX,EventMsg.Param1,0,0);
                        break;

                    case IPOD_APP_EVT_FFSEEKSTOP:
                        IAPSVC_DEFLOG("> IPOD_APP_EVT_FFSEEKSTOP > param1 =0x%lx, param2=0x%lx",  
                        							EventMsg.Param1, EventMsg.Param2);

                        mIapService->notify(IPOD_APP_EVT_FFSEEKSTOP,0,0,0);
                        break;
                    case IPOD_APP_EVT_REWSEEKSTOP:
                        IAPSVC_DEFLOG("> IPOD_APP_EVT_REWSEEKSTOP > param1 =0x%lx, param2=0x%lx",  
                        							EventMsg.Param1, EventMsg.Param2);

                        mIapService->notify(IPOD_APP_EVT_REWSEEKSTOP,0,0,0);
                        break;

                    case IPOD_APP_EVT_TRACKPOSITION:
                        IAPSVC_DEFLOG("> IPOD_APP_EVT_TRACKPOSITION > param1 =0x%lx, param2=0x%lx",  
                        							EventMsg.Param1, EventMsg.Param2);
                        //Param1 : track position ms
                        mIapService->notify(IPOD_APP_EVT_TRACKPOSITION,EventMsg.Param1,0,0);
                        break;
                        
                    case IPOD_APP_DR_EVT_TRACK_POSITION:
                        IAPSVC_DEFLOG("> IPOD_APP_DR_EVT_TRACK_POSITION > param1 =0x%lx, param2=0x%lx",  
                        							EventMsg.Param1, EventMsg.Param2);
                        //Param1 : track position ms
                        mIapService->notify(IPOD_APP_DR_EVT_TRACK_POSITION,EventMsg.Param1,0,0);
                        break;

                    case IPOD_APP_EVT_CHANGE_SAMPLEFREQ:
                        IAPSVC_DEFLOG("> IPOD_APP_EVT_CHANGE_SAMPLEFREQ > param1 =%ld, param2=0x%lx",  
                        							EventMsg.Param1, EventMsg.Param2);

                        //Param 1 : Sampling Frequency
                        mIapService->notify(IPOD_APP_EVT_CHANGE_SAMPLEFREQ,EventMsg.Param1,0,0);
                        break;

                    case IPOD_APP_EVT_CATEGORIZED_DATABASE_RECORD:
                        IAPSVC_DBUG("> IPOD_APP_EVT_CATEGORIZED_DATABASE_RECORD > param1 =0x%lx, param2=0x%lx",  
                        							EventMsg.Param1, EventMsg.Param2);
                        break;


                    case IPOD_APP_DR_EVT_CHAPTER_INFO:
                        IAPSVC_DBUG("> IPOD_APP_DR_EVT_CHAPTER_INFO > current index=0x%lx, chapter index=0x%lx p3 0x%lx", 
                        							EventMsg.Param1, EventMsg.Param2, EventMsg.Param3);
                        //Param1 : Current index
                        //Param2 : Chapter count
                        //Param3 : Chapter index
                        break;

                    case IPOD_APP_DR_EVT_PLAY_STATUS:
                        IAPSVC_DEFLOG("> IPOD_APP_DR_EVT_PLAY_STATUS > Playstatus=0x%lx ", 
                        							EventMsg.Param1);
                        //Param 1 : Play status
                        Rd_playStatus = (IPOD_DR_PLAY_STATUS)EventMsg.Param1;
                        mIapService->notify(IPOD_APP_DR_EVT_PLAY_STATUS,Rd_playStatus,0,0);
                        switch(Rd_playStatus)
                        {
                            case(IPOD_DR_PLAYBACK_STOPPED):
                                break;

                            case(IPOD_DR_PLAYING):
                                break;

                            case(IPOD_DR_PLAYBACK_PAUSED):
                                break;

                            case(IPOD_DR_FAST_FORWARD):
                                break;

                            case(IPOD_DR_FAST_REWIND):
                                break;

                            case(IPOD_DR_END_FAST_FORWARD_OR_REWIND):
                                break;

                            default:
                                break;
                        }                        
                        break;

                    case IPOD_APP_DR_EVT_MUTE_UI_VOL:
                        IAPSVC_DBUG("> IPOD_APP_DR_EVT_MUTE_UI_VOL > MuteState=0x%lx, UIvolLevel=0x%lx", 
                        							EventMsg.Param1, EventMsg.Param2);
                        //Param 1 : Mute State
                        //Param 2 : UI Volume Level
                        break;

                    case IPOD_APP_DR_EVT_POWER_BATTERY:
                        IAPSVC_DBUG("> IPOD_APP_DR_EVT_POWER_BATTERY > PowerStatus=0x%lx, BatteryLevel=0x%lx", 
                        							EventMsg.Param1, EventMsg.Param2);
                        //Param 1 : Poewr State
                        //Param 2 : Battery Level
                        break;

                    case IPOD_APP_DR_EVT_EQALIZER_STATE:
                        IAPSVC_DBUG("> IPOD_APP_DR_EVT_EQALIZER_STATE > EqStatus=0x%lx ", 
                        							EventMsg.Param1);
                        //Param 1 : EQ state
                        break;

                    case IPOD_APP_DR_EVT_SHUFFLE:
                        IAPSVC_DEFLOG("> IPOD_APP_DR_EVT_SHUFFLE > ShuffleStatus=0x%lx ", 
                        							EventMsg.Param1);
                        //Param 1 : Shuffle status
                        mIapService->notify(IPOD_APP_DR_EVT_SHUFFLE,EventMsg.Param1,0,0);
                        break;

                    case IPOD_APP_DR_EVT_REPEAT:
                        IAPSVC_DEFLOG("> IPOD_APP_DR_EVT_REPEAT > RepeatStatus=0x%lx ", 
                        							EventMsg.Param1);
                        //Param 1 : Repeat status
                        mIapService->notify(IPOD_APP_DR_EVT_REPEAT,EventMsg.Param1,0,0);
                        break;

                    case IPOD_APP_DR_EVT_DATE_TIME:
                        IAPSVC_DBUG("> IPOD_APP_DR_EVT_DATE_TIME > Hour=0x%lx, Minute=0x%lx", 
                        							EventMsg.Param1, EventMsg.Param2);
                        //Param 1 : Year
                        //Param 2 : Month
                        //Param 3 : Day
                        //Param 4 : Hour
                        //Param 5 : Minute
                        break;

                    case IPOD_APP_DR_EVT_ALARM:
                        IAPSVC_DBUG("> IPOD_APP_DR_EVT_ALARM > AlarmStatus=0x%lx, Hour(0x0000FF00) + Min(0x000000FF)=0x%lx", 
                        							EventMsg.Param1, EventMsg.Param2);
                        //Param 1 : Alarm Status
                        //Param 2 : Alarm Hour
                        //Param 3 : Alarm Min
                        break;

                    case IPOD_APP_DR_EVT_BACKLIGHT:
                        IAPSVC_DBUG("> IPOD_APP_DR_EVT_BACKLIGHT > BackLight=0x%lx ", 
                        							EventMsg.Param1);
                        //Param 1 : Backlight
                        break;

                    case IPOD_APP_DR_EVT_HOLD_SWITCH:
                        IAPSVC_DBUG("> IPOD_APP_DR_EVT_HOLD_SWITCH > Holdswitch=0x%lx ", 
                        							EventMsg.Param1);
                        //Param 1 : Hold switch
                        break;

                    case IPOD_APP_DR_EVT_SOUND_CHECK:
                        IAPSVC_DBUG("> IPOD_APP_DR_EVT_SOUND_CHECK > SoundCheck=0x%lx ", 
                        							EventMsg.Param1);
                        //Param 1 : Sound check
                        break;

                    case IPOD_APP_DR_EVT_AUDIOBOOK:
                        IAPSVC_DBUG("> IPOD_APP_DR_EVT_AUDIOBOOK > AudioBook=0x%lx ", 
                        							EventMsg.Param1);
                        //Param 1 : Audiobook
                        break;

                    case IPOD_APP_DR_EVT_TRACK_POSITION_IN_SECONDS:
                        //IAPSVC_DBUG("> IPOD_APP_DR_EVT_TRACK_POSITION_IN_SECONDS > TimePosition(ms)=0x%lx ", 
                        //							EventMsg.Param1);
                        //Param 1: Track Posiotion
                        break;

                    case IPOD_APP_DR_EVT_MUTE_UI_ABSOLUTE_VOL:
                        IAPSVC_DBUG("> IPOD_APP_DR_EVT_ALARM > MuteState=0x%lx, (long) UIVolume(0x0000FF00) + AbsoluteVolume(0x000000FF)=0x%lx", 
                        							EventMsg.Param1, EventMsg.Param2);
                        //Param 1 : MuteState
                        //Param 2 : UIvolLevel
                        //Param 3 : AbsoluteVol
                        break;

                    case IPOD_APP_DR_EVT_TRACK_CAPABILITIES:
                        IAPSVC_DEFLOG("> IPOD_APP_DR_EVT_TRACK_CAPABILITIES > TrackCapabilitiesBits=0x%lx ", 
                        							EventMsg.Param1);
                        //Pararm 1 : Track capabilities
                        mIapService->notify(IPOD_APP_DR_EVT_TRACK_CAPABILITIES,EventMsg.Param1,0,0);
                        break;
                    case IPOD_APP_DR_EVT_PLAYBACK_ENGINE_CONTENTS_CHANGED:
                        IAPSVC_DBUG("> IPOD_APP_DR_EVT_PLAYBACK_ENGINE_CONTENTS_CHANGED > Number of tracks in new playlists=0x%lx ", 
                        							EventMsg.Param1);
                        //Param 1: NumOfTracksInNewPlayList
                        break;

                    case IPOD_APP_NOTI_TYPE_NOWPLAYINGAPPBUNDLENAME:{
                        unsigned char *ptr;
                        unsigned long strlen = 0;
                        Parcel data;
                        String8 str8 = String8("");
                        ptr = (unsigned char *)EventMsg.Param1;
                        strlen = EventMsg.Param2;
                        if(strlen > 0)
                            str8 = String8((char const*)ptr);
                        ALOGI("%d: %s() NowPlayingBundleName [len %ld](%s) ", __LINE__, __FUNCTION__,strlen,str8.string());
                        data.writeString16(String16(str8));

                        if(nIapSvc != NULL){
                            //nIapSvc->notifyObj(IPOD_APP_NOTI_TYPE_NOWPLAYINGAPPBUNDLENAME,0, 0, &data);
                            nIapSvc->notifyObj(IPOD_APP_IPODNOTIFICATION,IPOD_NOTI_TYPE_NOWPLAYINGAPPBUNDLENAME, 0, &data);
                        }
                        break;
                        }
                    case IPOD_APP_NOTI_TYPE_NOWPLAYINGAPPDISPLAYNAME:{
                        unsigned char *ptr;
                        unsigned long strlen = 0;
                        Parcel data;
                        String8 str8 = String8("");
                        ptr = (unsigned char *)EventMsg.Param1;
                        strlen = EventMsg.Param2;
                        if(strlen > 0)
                            str8 = String8((char const*)ptr);
                        IAPSVC_DEFLOG("%d: %s() NowPlayingDisplayName [len %ld](%s) ", __LINE__, __FUNCTION__,strlen,str8.string());
                        data.writeString16(String16(str8));

                        if(nIapSvc != NULL){
                            //nIapSvc->notifyObj(IPOD_APP_NOTI_TYPE_NOWPLAYINGAPPDISPLAYNAME,0, 0, &data);
                            nIapSvc->notifyObj(IPOD_APP_IPODNOTIFICATION,IPOD_NOTI_TYPE_NOWPLAYINGAPPDISPLAYNAME, 0, &data);
                        }
                        break;
                        }

                    default:
                        IAPSVC_DBUG("> Invalid message %d 0x%x",EventMsg.EventID,EventMsg.EventID);
                        usleep(100*1000);
                        IsSuccess = false;
                        break;
                }			

            }else{
                //IAPSVC_DBUG(" %d: %s() IPOD_OS_ReadMessageQueue error(0x%x) ", __LINE__, __FUNCTION__,err);
            }
            
        }
    }
		
    return 0;
}


IapService::IapAudioThread::IapAudioThread(IapService *iapService)
        : Thread(false), mIapService(iapService) , mPaused(true), 
        mWaitPaused(false),mAudioTreadExit(false),g_sample_rate_change(false)
{ 
    IAPSVC_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    //g_iPhoneAudioThdExit = 0;

    g_sample_rate = 44100;
    g_channels = 2;
    gPlaybackHandle= NULL;
    gCaptureHandle= NULL;
    mIsAudioRunning = false;
    g_firstAfterInit = true;
}

void IapService::IapAudioThread::requestExit()
{
    // must be in this order to avoid a race condition
    Thread::requestExit();
    mAudioTreadExit = true;
    {
        AutoMutex _l(mAudioPauseLock);
        IAPSVC_DBUG("%d: %s() mAudioTreadExit %d send signal ",
                                                                                        __LINE__,
                                                                                        __FUNCTION__,
                                                                                        mAudioTreadExit
                                                                                        );
        mAudioThreadRunCond.signal();
    }
}

bool IapService::IapAudioThread::AudioResetFirst()
{
    IAPSVC_DBUG("%d: %s()  mPaused(%d) mWaitPaused(%d)\n",
                                                                                    __LINE__,
                                                                                    __FUNCTION__,
                                                                                    mPaused,
                                                                                    mWaitPaused
                                                                                    );
    {
        AutoMutex _l(mAudioPauseLock);
        g_firstAfterInit = true;
    }
    return true;
}


int IapService::IapAudioThread::AudioInitFunc(int *pBufSize)
{
    bool result = true;
    int ret;
    //int readBytes = 0;
    uint32_t sampleRate = 0;
    size_t frameCount = 0;
    int channels = 2;
//    int buffSize = AUDIO_BUFFER_MAX_SIZE;

    IAPSVC_DBUG("%d: %s() g_sample_rate %d ", __LINE__, __FUNCTION__,g_sample_rate);

    sampleRate = g_sample_rate;
    channels = g_channels;

    /*------------------------------------------------------------
     * Capture Audio
     *------------------------------------------------------------*/

#ifdef USE_DIRECT_AUDIO_HAL
    {
        Mutex::Autolock lock(mCaptureLock);
        IapAudioHw* captureHandle;
        if (gCaptureHandle != NULL) {
            captureHandle = (IapAudioHw*)gCaptureHandle;
            captureHandle->close();
            delete captureHandle;
            gCaptureHandle = NULL;
        }
        captureHandle = new IapAudioHw(1);
        gCaptureHandle = (void*) captureHandle;

        captureHandle->setSampleRate(sampleRate);
        captureHandle->setChannelCount(channels);
        captureHandle->setFrameSize(channels*2);

        ret = captureHandle->open(16384, 10000, 20000);


        if(ret < 0 ){
            ALOGE("captureHandle->open failed\n");
            ret = 2;
            goto error_return;
            //return 2;
        }

        gBufferSize = AUDIO_CHUNK_BYTES;
    }

#else
    {
        Mutex::Autolock lock(mCaptureLock);    
        if (gCaptureHandle != NULL) {
            gCaptureHandle->stop();
            delete gCaptureHandle;
            gCaptureHandle = NULL;
        }
        gCaptureHandle = new AudioRecord();

        ret = gCaptureHandle->getMinFrameCount(&frameCount, 
                        sampleRate, 
                        AUDIO_FORMAT_PCM_16_BIT, 
                        (channels == 2) ? AUDIO_CHANNEL_IN_STEREO : AUDIO_CHANNEL_IN_MONO
                        );



        if(ret < 0 || frameCount > AUDIO_IN_FRAME_MAX_SIZE){
            ALOGE("%d: %s()  getMinFrameCount ret(%d) frameCount(%d) channels(%d)\n", 
                                                                                                                    __LINE__,
                                                                                                                    __FUNCTION__,
                                                                                                                    ret,
                                                                                                                    frameCount,
                                                                                                                    channels
                                                                                                                    );
        }else{
            IAPSVC_DBUG("%d: %s()  getMinFrameCount (%d)\n", __LINE__, __FUNCTION__, frameCount);
            frameCount = AUDIO_IN_FRAME_MAX_SIZE;
            gBufferSize = AUDIO_CHUNK_BYTES;
        }

        ret = gCaptureHandle->set(AUDIO_SOURCE_MIC,
                    sampleRate,
                    AUDIO_FORMAT_PCM_16_BIT,
                    (channels == 2) ? AUDIO_CHANNEL_IN_STEREO : AUDIO_CHANNEL_IN_MONO,
                    frameCount,
                    0);
        
        IAPSVC_DBUG("%d: %s()  set()...  ret[%d]  gBufferSize[%d] channels(%d) sample(%d)\n", 
                                                                                        __LINE__,
                                                                                        __FUNCTION__,
                                                                                        ret, 
                                                                                        gBufferSize,
                                                                                        channels,
                                                                                        sampleRate
                                                                                        );

        if (gCaptureHandle->initCheck() != 0) {
            ALOGE("~~~~ AudioTrack->initCheck failed");
            ret = 2;
            goto error_return;
            //return 2;
        }
    }
#endif /* USE_DIRECT_AUDIO_HAL */
    
    *pBufSize= gBufferSize;


    /*------------------------------------------------------------
     * Playback Audio
     *------------------------------------------------------------*/
    {
        Mutex::Autolock lock(mPlaybackLock);

        if (gPlaybackHandle != NULL) {
            gPlaybackHandle->stop();
            gPlaybackHandle->flush();
            gPlaybackHandle.clear();
            gPlaybackHandle = NULL;
        }    

        if (gPlaybackHandle == NULL) {
            gBufferSize = AUDIO_CHUNK_BYTES;
            gPlaybackHandle = new AudioTrack();

            ret = gPlaybackHandle->getMinFrameCount(&frameCount, AUDIO_STREAM_MUSIC, sampleRate);

            if(ret < 0 || frameCount > AUDIO_OUT_FRAME_MAX_SIZE){
                ALOGE("%d: %s()  getMinFrameCount (%d) channels(%d)\n", 
                                                                                            __LINE__,
                                                                                            __FUNCTION__,
                                                                                            ret,
                                                                                            channels
                                                                                            );
            }else{
                IAPSVC_DBUG("%d: %s()  getMinFrameCount (%d)\n", 
                                                                                    __LINE__,
                                                                                    __FUNCTION__,
                                                                                    frameCount
                                                                                    );
                frameCount = AUDIO_OUT_FRAME_MAX_SIZE;
                gBufferSize = AUDIO_CHUNK_BYTES;
            }
            
            ret = gPlaybackHandle->set(AUDIO_STREAM_MUSIC,  //TODO parametrize this
                                sampleRate,
                                AUDIO_FORMAT_PCM_16_BIT,
                                (channels == 2) ? AUDIO_CHANNEL_OUT_STEREO : AUDIO_CHANNEL_OUT_MONO,
                                frameCount,
                                AUDIO_OUTPUT_FLAG_NONE);

            if (gPlaybackHandle->initCheck() != 0) {
                ALOGE("%d: %s()  AudioTrack->initCheck failed... ret[%d]", 
                                                                                                    __LINE__,
                                                                                                    __FUNCTION__,
                                                                                                    ret
                                                                                                    );
                gPlaybackHandle.clear();
                gPlaybackHandle = NULL;
                ret = 1;
                //return 1;
                goto error_return;
            }
            IAPSVC_DBUG("%d: %s()  set() ret[%d] gBufSize[%d]  Samplerate[%d]\n", 
                                                                                                                __LINE__,
                                                                                                                __FUNCTION__,
                                                                                                                ret,
                                                                                                                frameCount, 
                                                                                                                sampleRate
                                                                                                                );
        }
        else {
            gPlaybackHandle->stop();
            gPlaybackHandle->flush();
            ret = gPlaybackHandle->setSampleRate(sampleRate);
            IAPSVC_DBUG("%d: %s()  setSampleRate() ret[%d]  gBufSize[%d]  sample_rate[%d]\n",
                                                                                                                    __LINE__,
                                                                                                                    __FUNCTION__,
                                                                                                                    ret, 
                                                                                                                    gBufferSize, 
                                                                                                                    sampleRate
                                                                                                                    );
        }
    }

     IAPSVC_DBUG("%d: %s() OK  ret[%d]\n", 
                                                            __LINE__,
                                                            __FUNCTION__,
                                                            ret
                                                            );
    
    return 0;    
error_return:
    AudioDeinit();
    return ret;
}


int IapService::IapAudioThread::AudioInit(int *pBufSize)
{
    int ret;
    ret = AudioInitFunc(pBufSize);
    if(ret != 0){
        ALOGE(" AudioInit failed ret  %d",ret);
        return ret;
    }
    {
#ifndef USE_DIRECT_AUDIO_HAL
        Mutex::Autolock lock(mCaptureLock);    
        if(gCaptureHandle)
            gCaptureHandle->start();
#endif /* USE_DIRECT_AUDIO_HAL */
    }
    {
        Mutex::Autolock lock(mPlaybackLock);    
        if(gPlaybackHandle != NULL)
            gPlaybackHandle->start();
    }    

     IAPSVC_DBUG("%d: %s() OK ret[%d]\n", 
                                                            __LINE__,
                                                            __FUNCTION__,
                                                            ret
                                                            );
    
    return 0;
}

void IapService::IapAudioThread::AudioDeinit()
{
    {
        Mutex::Autolock lock(mPlaybackLock);
        if (gPlaybackHandle != NULL) {
            //if(gPlaybackHandle->muted())
            //    gPlaybackHandle->mute(false);
            //if(gPlaybackHandle->stopped() == false)
                gPlaybackHandle->stop();
            gPlaybackHandle->flush();        
            gPlaybackHandle.clear();
            gPlaybackHandle = NULL;
        }
    }
    {
        Mutex::Autolock lock(mCaptureLock);
        if (gCaptureHandle != NULL) {
#ifdef USE_DIRECT_AUDIO_HAL
            IapAudioHw* captureHandle;
            captureHandle = (IapAudioHw*)gCaptureHandle;
            captureHandle->close();
            delete captureHandle;
            gCaptureHandle = NULL;
#else
            if(gCaptureHandle->stopped() == false)
                gCaptureHandle->stop();
            delete gCaptureHandle;
            gCaptureHandle = NULL;
#endif /* USE_DIRECT_AUDIO_HAL */
        }    
    }
}

void IapService::IapAudioThread::AudioSetSampleRate(uint32_t sampleRate)
{
    int fChange = 0;
    IPOD_DR_PLAY_STATUS playStatus;

    //playStatus = IPOD_DR_PLAYBACK_STOPPED;
    //ret = mIapService->Sample_GetDRPlayStatus(&playStatus);
    playStatus = (IPOD_DR_PLAY_STATUS)mIapService->GetSvcLocalPlayStatus();
    
    IAPSVC_DBUG("%d: %s()  old(%d) new(%d) playStatus(%d)\n", 
                                                                        __LINE__,
                                                                        __FUNCTION__,
                                                                        g_sample_rate,
                                                                        sampleRate,
                                                                        playStatus
                                                                        );
    {
        AutoMutex _l(mAudioPauseLock);
        if(g_sample_rate != sampleRate){
            fChange = 1;
        }
        g_sample_rate = sampleRate;

        if(fChange)
        {
            IAPSVC_DBUG("%d: %s() set sample rate change  \n", __LINE__, __FUNCTION__);
            g_sample_rate_change = true;

            if(IsRunning())
            {
                mPaused = true;
                //mWaitPaused = false;
            }
            IAPSVC_DBUG("%d: %s() set sample rate change true  mPaused(%d)\n", 
                                                                                                    __LINE__,
                                                                                                    __FUNCTION__,
                                                                                                    mPaused
                                                                                                    );
        }
    }
}

void IapService::IapAudioThread::AudioSetChannels(uint32_t channels)
{
    IAPSVC_DBUG("%d: %s()  \n", __LINE__, __FUNCTION__);
    {
        AutoMutex _l(mAudioPauseLock);  
        g_channels = channels;
    }        
}

void IapService::IapAudioThread::AudioSetBuffer(short * pPcmBuff)
{
    IAPSVC_DBUG("%d: %s()  \n", __LINE__, __FUNCTION__);
    gPcmBuff = pPcmBuff;
}

bool IapService::IapAudioThread::IsRunning()
{
    AutoMutex _l(mAudioPauseLock);
    return mIsAudioRunning;
}

bool IapService::IapAudioThread::AudioStart()
{
    IAPSVC_DEFLOG("%d: %s()  mPaused(%d) mWaitPaused(%d)\n", 
                                                                                    __LINE__,
                                                                                    __FUNCTION__,
                                                                                    mPaused,
                                                                                    mWaitPaused
                                                                                    );
    {
        AutoMutex _l(mAudioPauseLock);
        mIsAudioRunning = true;
        mAudioThreadRunCond.signal();
    }
    return true;
}

bool IapService::IapAudioThread::AudioFirstStart()
{
    IAPSVC_DBUG("%d: %s()  mPaused(%d) mWaitPaused(%d)\n", 
                                                                                        __LINE__,
                                                                                        __FUNCTION__,
                                                                                        mPaused,
                                                                                        mWaitPaused
                                                                                        );
    {
        AutoMutex _l(mAudioPauseLock);
        mIsAudioRunning = true;
        mAudioThreadRunCond.signal();
    }
    return true;
}

void IapService::IapAudioThread::AudioPause()
{
    AutoMutex _l(mAudioPauseLock);
    IAPSVC_DEFLOG("%d: %s()  mPaused(%d) mWaitPaused(%d)\n",
                                                                                        __LINE__,
                                                                                        __FUNCTION__,
                                                                                        mPaused,
                                                                                        mWaitPaused
                                                                                        );
    if(mPaused == false)
    {
        mIsAudioRunning = false;
        mPaused = true;
        mWaitPaused = true;

        IAPSVC_DBUG("%d: %s()  wait until AudioRun mPaused(%d) mWaitPaused(%d) \n", 
                                                                                    __LINE__,
                                                                                    __FUNCTION__,
                                                                                    mPaused,
                                                                                    mWaitPaused
                                                                                    );
        switch(g_sample_rate)
        {
            case 5500:
            case 6000:
            case 7333:
                noise_avoid_cnt = 2;
                break;
            case 8000:
            case 11025:
                noise_avoid_cnt = 3;
                break;
            case 16000:
            case 22050:
            case 24000:
                noise_avoid_cnt = 5;
                break;
            case 32000:
            case 44100:
            case 48000:
            defualt:
                noise_avoid_cnt = 10;
                break;
        }
        mAudioPausedCond.wait(mAudioPauseLock);
        IAPSVC_DBUG("%d: %s()  AudioRun stoped\n", __LINE__, __FUNCTION__);
    }
}

bool IapService::IapAudioThread::AudioMute(bool flag)
{
    IAPSVC_DEFLOG("%d: %s()  mute(%d) \n", __LINE__, __FUNCTION__,flag);
    {
        AutoMutex _l(mPlaybackLock);
        if(gPlaybackHandle != NULL){
            //gPlaybackHandle->mute(flag);
            mMuteFlag = flag;
        }
    }
    return true;
}


int IapService::IapAudioThread::AudioRun()
{
    int ret;
    int readBytes = 0;
    int buffSize = AUDIO_CHUNK_BYTES;
    int readCnt = 0;
    IAPSVC_DBUG("%d: %s()  mPaused(%d ) mWaitPaused(%d)\n", 
                                                                                    __LINE__,
                                                                                    __FUNCTION__,
                                                                                    mPaused,
                                                                                    mWaitPaused
                                                                                    );
    {
        sleep(2);

        while ( !mAudioTreadExit ) {
            {
                AutoMutex _l(mAudioPauseLock);

                if (mPaused) {
                    IAPSVC_DEFLOG("%d: %s() paused process WaitPaused(%d) sample_change(%d) mAudioTreadExit(%d)\n", 
                                                                        __LINE__, 
                                                                        __FUNCTION__,
                                                                        mWaitPaused,
                                                                        g_sample_rate_change,
                                                                        mAudioTreadExit
                                                                        );
                    if (mWaitPaused){
                        mWaitPaused = false;
                        IAPSVC_DBUG("%d: %s()  Send Pause cond\n", 
                                                                                __LINE__,
                                                                                __FUNCTION__
                                                                                );
                        mAudioPausedCond.signal();
                    }

                    if(g_sample_rate_change){
                        IAPSVC_DBUG("%d: %s()  g_sample_rate_change clear\n", 
                                                                                                    __LINE__,
                                                                                                    __FUNCTION__
                                                                                                    );
                    }else{
                        IAPSVC_DBUG("%d: %s()  wait audioRun start signal\n",
                                                                                            __LINE__,
                                                                                            __FUNCTION__
                                                                                            );
                        {
                            Mutex::Autolock lock(mPlaybackLock);
                            if(gPlaybackHandle != NULL)
                                gPlaybackHandle->pause();
                        }

                        mAudioThreadRunCond.wait(mAudioPauseLock);
                        // caller will check for exitPending()
                        IAPSVC_DBUG("%d: %s()  receive signal (%d) mPaused(%d)\n", 
                                                                                                __LINE__,
                                                                                                __FUNCTION__,
                                                                                                mWaitPaused,
                                                                                                mPaused
                                                                                                );
                    }
                    
                    if(mAudioTreadExit){
                        IAPSVC_DBUG("%d: %s()  audio threa signal exit mAudioTreadExit(%d)  \n", 
                                                                                                __LINE__,
                                                                                                __FUNCTION__,
                                                                                                mAudioTreadExit
                                                                                                );
                        break;
                    }
                    mPaused = false;
                        
                    if(g_sample_rate_change || gPlaybackHandle == NULL 
                        || gCaptureHandle == NULL || g_firstAfterInit )
                    {
                        ret = AudioInit(&buffSize);
                        if(ret == 1 || ret == 2){
                            ALOGE(" %s AudioInit failed ret  %d",__FUNCTION__,ret);
                            mPaused = true;
                            continue;
                        }else{
                            if(g_firstAfterInit)
                                g_firstAfterInit = false;
                            if(g_sample_rate_change)
                                g_sample_rate_change = false;
                            IAPSVC_DBUG("%d: %s() paused process WaitPaused(%d) mPaused(%d) \n", 
                                                                                                            __LINE__, 
                                                                                                            __FUNCTION__,
                                                                                                            mWaitPaused,
                                                                                                            mPaused
                                                                                                            );     
                        }
                    }else{
                        {
                            Mutex::Autolock lock(mPlaybackLock);
                            if(gPlaybackHandle != NULL){
                                gPlaybackHandle->flush();
                                gPlaybackHandle->start();
                            }
                        }
                    }
                    
                    IAPSVC_DEFLOG("%d: %s()  audioRun start\n",
                                                                                            __LINE__,
                                                                                            __FUNCTION__
                                                                                            );
                }
            }

            readBytes = 0;
            {
                Mutex::Autolock lock(mCaptureLock);
#ifdef USE_DIRECT_AUDIO_HAL
                if(gCaptureHandle)
                {
                    IapAudioHw* captureHandle;
                    captureHandle = (IapAudioHw*)gCaptureHandle;
                    readBytes = captureHandle->read(gPcmBuff, buffSize);
                }
                else
                {
                    ALOGE("gCaptureHandle is NULL");
                }
#else
                if(gCaptureHandle)
                    readBytes = gCaptureHandle->read(gPcmBuff, buffSize);
#endif /* USE_DIRECT_AUDIO_HAL */
            }
            readCnt++;
            if(readCnt % 100 ==0 )
                IAPSVC_DEFLOG("%s()  readCnt %d  read %d\n",__FUNCTION__,readCnt,readBytes);
            if (readBytes > 0) {
				if(noise_avoid_cnt-- > 0)
					memset(gPcmBuff, 0x00, buffSize);
                #ifdef AUDIO_OUTPUT_DUMP
                FILE *dump_out_fp = NULL;

                dump_out_fp= fopen("/data/dump_out.pcm", "a+");

                if(dump_out_fp == NULL)
                    ALOGE("cannot open /data/dump_out.pcm");

                if(dump_out_fp)
                {
                    fwrite(gPcmBuff, readBytes, 1, dump_out_fp);
                    fclose(dump_out_fp);
                }
                #endif                
                //IAPSVC_DBUG("%d: %s() readBytes[%d]\n",__LINE__,__FUNCTION__,readBytes);
                {
                    Mutex::Autolock lock(mPlaybackLock);
                    if(gPlaybackHandle != NULL){
                        ret = gPlaybackHandle->write(gPcmBuff, readBytes);
                        if (ret != readBytes)
                            ALOGE("~~~~  write: %d / %d writted...", ret, readBytes);
                        //IAPSVC_DBUG("%d: %s() write ret[%d]\n",__LINE__,__FUNCTION__,ret);
                    }
                }
            }
            else {
                ALOGE("~~~~ AudioRecord->read: %d readed...", readBytes);
                if(readBytes == -19 || readBytes == -16){
                    ALOGE("~~~~ AudioRecord->read: force stop");
                    mIsAudioRunning = false; 
                    mPaused= true;
                }
                usleep(10*1000);
            }

        }
    }

    AudioDeinit();

    IAPSVC_DBUG("%d: %s()  exit~~~\n", __LINE__, __FUNCTION__);

    mAudioTreadExit = 0;

    return 0;
}

};
