/****************************************************************************
 *   FileName    : DiPO_jni.cpp
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


#include <string.h>
#include "jni.h"
#include <nativehelper/JNIHelp.h>
#include "android_runtime/AndroidRuntime.h"

#include <utils/Log.h>
#include <binder/Parcel.h>
#include "android_os_Parcel.h"

#include "iAP2Client.h"
#include "iAP2Interface.h"

#include "iAP2Transport.h"
#include "iAP2_jni.h"
#define LOG_TAG "iAP2"

JavaVM *g_javaVM = NULL;
jobject g_callbackObject = NULL;

using namespace android;
using namespace IAP2JNIH;


static JNIEnv *gEnvIap2;
static jclass gClazzIap2;

static jmethodID gMethodIDIap2;
static jmethodID gMethodIDIap2Chk;
static jmethodID gMethodIDIap2RawData;
static jmethodID gMethodIDIap2TransportSendData;

jobject iap2_ep_object = NULL;
jobject iap2_sessCtrlDispatch_object = NULL;



//static jobject gInterfaceObjectIap2;
pid_t gTid;

static const char *classPathNameIap2 = "com/tcc/iap2server/Iap2Service";

#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus

#if 0

static void callByLibraryObj(int what,int arg1, int arg2,void *objParcel)
{ 
    #if 1
    JNIEnv * env;
    JavaVM *javaVM = g_javaVM;
    bool attached = false;
    Parcel *obj = (Parcel *)objParcel;
    pid_t currTid = gettid();
    switch (javaVM->GetEnv((void**)&env, JNI_VERSION_1_6))
    {
        case JNI_OK:
        break;
        case JNI_EDETACHED:
            if (javaVM->AttachCurrentThread(&env, NULL)!=0)
            {
                ALOGE("   %s what = 0x%x arg[ %d , %d] Could not attach current thread gTid(%d) tid(%d)",__FUNCTION__,what, arg1, arg2,gTid,currTid);
            }
            attached = true;
            break;
        case JNI_EVERSION:
            ALOGE("   %s what = 0x%x arg[ %d , %d] JNI_EVERSION gTid(%d) tid(%d)",__FUNCTION__,what, arg1, arg2,gTid,currTid);
            break;
    }

    if (obj && obj->dataSize() > 0) {
        jobject jParcel = createJavaParcelObject(env);
        if (jParcel != NULL) {
            Parcel* nativeParcel = parcelForJavaObject(env, jParcel);
            nativeParcel->setData(obj->data(), obj->dataSize());
            env->CallStaticVoidMethod(gClazzIap2, gMethodIDIap2,
                                                what, arg1, arg2, jParcel);

            env->DeleteLocalRef(jParcel);
            obj->freeData();
        }
    } else {
        env->CallStaticVoidMethod(gClazzIap2, gMethodIDIap2,
                                                what, arg1, arg2, NULL);
    }
    
    if (attached)
    {
        javaVM->DetachCurrentThread();
    }
    #else
    JNIEnv *env = gEnvIap2;//AndroidRuntime::getJNIEnv();
    //JNIEnv *env= AndroidRuntime::getJNIEnv();
    JavaVM *javaVM = g_javaVM;
    pid_t currTid = gettid();
    ALOGD("   %s what = 0x%x arg[ %d , %d] gTid(%d) tid(%d)",__FUNCTION__,what, arg1, arg2,gTid,currTid);
    
    if(gTid == currTid){
        env->CallStaticVoidMethod(gClazzIap2, gMethodIDIap2,what, arg1, arg2,obj);
    }else{
        if(javaVM->AttachCurrentThread(&env, NULL)== JNI_OK){
            Parcel *data = (Parcel *)obj;
            jobject jParcel = createJavaParcelObject(env);
            Parcel* nativeParcel = parcelForJavaObject(env, jParcel);
            
            if(obj != NULL){
                nativeParcel->appendFrom(data,0,data->dataSize());
            }
            nativeParcel->setDataPosition(0);
            
            env->CallStaticVoidMethod(gClazzIap2, gMethodIDIap2,what, arg1, arg2,jParcel);
            if(javaVM->DetachCurrentThread() != JNI_OK){
                ALOGE(" %s DetachCurrentThread failed",__FUNCTION__);
            }
        }else{
            ALOGE(" %s AttachCurrentThread failed",__FUNCTION__);
        }
    }

   #endif
    return;
}

static void callByLibraryObj2(int what,int arg1, int arg2,void *objParcel)
{ 
    JNIEnv * env;
    JavaVM *javaVM = g_javaVM;
    bool attached = false;
    Parcel *obj = (Parcel *)objParcel;
    int fileId = arg1;
    char *rawData = (char *)arg2;
    pid_t currTid = gettid();
    switch (javaVM->GetEnv((void**)&env, JNI_VERSION_1_6))
    {
        case JNI_OK:
        break;
        case JNI_EDETACHED:
            if (javaVM->AttachCurrentThread(&env, NULL)!=0)
            {
                ALOGE("   %s what = 0x%x arg[ %d , %p] Could not attach current thread gTid(%d) tid(%d)",__FUNCTION__,what, fileId, rawData,gTid,currTid);
            }
            attached = true;
            break;
        case JNI_EVERSION:
            ALOGE("   %s what = 0x%x arg[ %d , %p] JNI_EVERSION gTid(%d) tid(%d)",__FUNCTION__,what, fileId, rawData,gTid,currTid);
            break;
    }

    if (obj && obj->dataSize() > 0) {
        Parcel *pData = (Parcel *)objParcel;
        int fileID = -1;
        int len = -1;
        pData->setDataPosition(0);
        fileID = pData->readInt32();
        len = pData->readInt32();
        //ALOGE("%s : fileId %d len %d",__FUNCTION__,fileID,len);
        if(fileID >= 0 && len > 0){
            jbyteArray array = env->NewByteArray(len);
            if (!array) {  // OutOfMemoryError exception has already been thrown.
                ALOGE("%s : OutOfMemoryError is thrown.",__FUNCTION__);
            } else {
                jbyte* bytes = env->GetByteArrayElements(array, NULL);
                if (bytes != NULL) {
                    memcpy(bytes, rawData, len);
                    env->CallStaticVoidMethod(gClazzIap2, gMethodIDIap2RawData,
                                                what, fileID, array, NULL);
                    env->ReleaseByteArrayElements(array, bytes, 0);

                }
            }
            if (array) {
                env->DeleteLocalRef(array);
            }
        }
        pData->freeData();
    }
    
    if (attached)
    {
        javaVM->DetachCurrentThread();
    }
    return;
}

int notify(int what,int arg1, int arg2,void *obj)
{
    //ALOGD(" %s",__FUNCTION__);
    if(what == IAP2_NOTI_NOW_PLAYING_ARTWORK_RAW){
        callByLibraryObj2(what,arg1,arg2,obj);
    }else
        callByLibraryObj(what,arg1,arg2,obj);
    return 0;
}

static BOOL isConnected()
{
    #if 1
    JNIEnv * env;
    JavaVM *javaVM = g_javaVM;
    bool attached = false;
    pid_t currTid = gettid();
    BOOL ret = FALSE;
    switch (javaVM->GetEnv((void**)&env, JNI_VERSION_1_6))
    {
        case JNI_OK:
        break;
        case JNI_EDETACHED:
            if (javaVM->AttachCurrentThread(&env, NULL)!=0)
            {
                ALOGE("   %s Could not attach current thread gTid(%d) tid(%d)",__FUNCTION__,gTid,currTid);
            }
            attached = true;
            break;
        case JNI_EVERSION:
            ALOGE("   %s JNI_EVERSION gTid(%d) tid(%d)",__FUNCTION__,gTid,currTid);
            break;
    }

    ret = env->CallStaticBooleanMethod(gClazzIap2, gMethodIDIap2Chk);
   
    if (attached)
    {
        javaVM->DetachCurrentThread();
    }
    #else
    //JNIEnv *env = gEnvIap2;
    JNIEnv *env= AndroidRuntime::getJNIEnv();
    JavaVM *javaVM = g_javaVM;
    BOOL ret = FALSE;
    #if 1
    ret = env->CallStaticBooleanMethod(gClazzIap2, gMethodIDIap2Chk);
    #else
    if(javaVM->AttachCurrentThread(&env, NULL)== JNI_OK){
        
        ret = env->CallStaticBooleanMethod(gClazzIap2, gMethodIDIap2Chk);
        if(javaVM->DetachCurrentThread() != JNI_OK){
            ALOGE(" %s DetachCurrentThread failed",__FUNCTION__);
        }
    }else{
        ALOGE(" %s AttachCurrentThread failed",__FUNCTION__);
    }
    #endif
    #endif
    return ret;
}

static void initClassHelper(JNIEnv *env, const char *path)//, jobject *objptr)
{
    //ALOGD("   %s",__FUNCTION__);
    jclass cls = env->FindClass(path);

    if(!cls) {
        ALOGE("initClassHelper: failed to get %s class reference ", path);
        return;
    }

    gEnvIap2 = env;
    gClazzIap2 = (jclass)env->NewGlobalRef(cls);;

    jmethodID constr = env->GetStaticMethodID(cls, "postEventFromNative", "(IIILjava/lang/Object;)V");
    if(!constr) {
        ALOGE("initClassHelper: failed to get %s constructor postEventFromNative ", path);
        return;
    }

    gMethodIDIap2 = constr;

    constr = env->GetStaticMethodID(cls, "postEventFromNative2", "(II[BLjava/lang/Object;)V");
    if(!constr) {
        ALOGE("initClassHelper: failed to get %s constructor postEventFromNative2 ", path);
        return;
    }
    gMethodIDIap2RawData = constr;

    constr = env->GetStaticMethodID(cls, "isConnected", "()Z");
    if(!constr) {
        ALOGE("initClassHelper: failed to get %s constructor isConnected ", path);
        return;
    }

    gMethodIDIap2Chk = constr;

    constr = env->GetStaticMethodID(cls, "transportSendData", "([BI)V");
    if(!constr) {
        ALOGE("initClassHelper: failed to get %s constructor transportSendData ", path);
        return;
    }
    gMethodIDIap2TransportSendData = constr;
    
}

void *iAP2Jni_AttachedCurentThread(void *pEnv, int *pAttached)
{
    JNIEnv * env = (JNIEnv *)pEnv;
    JavaVM *javaVM = g_javaVM;
    bool attached = false;

    *pAttached = 0;
    switch (javaVM->GetEnv((void**)&env, JNI_VERSION_1_6))
    {
        case JNI_OK:
        break;
        case JNI_EDETACHED:
            //ALOGI("   %s call attachCurrentThread ",__FUNCTION__);
            if (javaVM->AttachCurrentThread(&env, NULL)!=0)
            {
                ALOGE("   %s error ",__FUNCTION__);
            }
            attached = true;
            break;
        case JNI_EVERSION:
            ALOGE("   %s version ",__FUNCTION__);
            break;
    }

    if(attached)
        *pAttached = 1;
    return env;
}

int iAP2Jni_ExternalDoWrite(void *pEnv,void *buffer,int len)
{
    JNIEnv * env = (JNIEnv *)pEnv;
    bool attached = false;

    if(pEnv == NULL)
    {
        ALOGE("%s : invalid env(NULL)",__FUNCTION__);
        return -2;
    }
    
    if( len > 0)
    {
        if(len > 0)
        {
            jbyteArray array = env->NewByteArray(len);
            if (!array) {  // OutOfMemoryError exception has already been thrown.
                ALOGE("%s : OutOfMemoryError is thrown.",__FUNCTION__);
            } 
            else 
            {
                jbyte* bytes = env->GetByteArrayElements(array, NULL);
                if (bytes != NULL) 
                {
                    memcpy(bytes,buffer,len);

                    if(len > 0)
                    {
                        //ALOGI("%s : len %d",__FUNCTION__,len);
                        env->CallStaticVoidMethod(gClazzIap2, gMethodIDIap2TransportSendData,array,len);
                    }
                    env->ReleaseByteArrayElements(array, bytes, 0);

                }
                if (array) {
                    env->DeleteLocalRef(array);
                }
            }
        }
    }

    return len;
}

void iAP2Jni_DetachedCurentThread(int attached)
{
    JavaVM *javaVM = g_javaVM;

    //ALOGI("   %s call ",__FUNCTION__);
    if (attached != 0)
    {
        //ALOGI("   %s call DetachCurrentThread ",__FUNCTION__);
        javaVM->DetachCurrentThread();
    }
}

JNIEXPORT void JNICALL Java_com_tcc_iap2server_Iap2Service_Iap2DebugTrigger(
                                                                                            JNIEnv* env, jobject thiz,int handle)
{
    ALOGD(" %s %d",__FUNCTION__,__LINE__);
    iAP2Client_DebugTrigger(handle);
}

JNIEXPORT int JNICALL Java_com_tcc_iap2server_Iap2Service_Iap2ExternalReadDone(
                                            JNIEnv* env, jobject thiz,int handle,jbyteArray pJavaBuffer, int size)
{
    int ret = 0;
    jboolean isCopy;
    jbyte *buffer = NULL;

    //ALOGD(" %s size %d ",__FUNCTION__,size);
    if(pJavaBuffer == NULL)
        return ret;
    buffer = env->GetByteArrayElements(pJavaBuffer, &isCopy);
    ret = iAP2ClientExternalReadDone(handle,(void *)buffer,size);
    env->ReleaseByteArrayElements(pJavaBuffer, buffer, 0);
    return ret;
}


JNIEXPORT void JNICALL Java_com_tcc_iap2server_Iap2Service_Iap2AudioInit(
                                                                                            JNIEnv* env, jobject thiz)
{
    ALOGD(" %s %d",__FUNCTION__,__LINE__);
    miAudioManager = iAP2AudioManager::Instance();
    miAudioManager->startThread();
}

JNIEXPORT void JNICALL Java_com_tcc_iap2server_Iap2Service_Iap2AudioDeinit(
                                                                                            JNIEnv* env, jobject thiz)
{
    ALOGD(" %s %d",__FUNCTION__,__LINE__);
    miAudioManager->stopThread();
}


JNIEXPORT void JNICALL Java_com_tcc_iap2server_Iap2Service_Iap2AudioPlay(
                                                                                            JNIEnv* env, jobject thiz)
{
    ALOGD(" %s %d ",__FUNCTION__,__LINE__);
    //miAudioManager->PlayAudio();
}

JNIEXPORT void JNICALL Java_com_tcc_iap2server_Iap2Service_Iap2AudioPause(
                                                                                            JNIEnv* env, jobject thiz)
{
    ALOGD(" %s %d",__FUNCTION__,__LINE__);
    miAudioManager->PauseAudio();
    miAudioManager->StopAudio();
}

JNIEXPORT void JNICALL Java_com_tcc_iap2server_Iap2Service_Iap2Init(
                                                                                            JNIEnv* env, jobject thiz)
{
    ALOGD(" %s %d tid(%d)",__FUNCTION__,__LINE__,gTid);
    g_callbackObject = env->NewGlobalRef(thiz);
}

JNIEXPORT int JNICALL Java_com_tcc_iap2server_Iap2Service_Iap2Main(
                                                                            JNIEnv* env, jobject thiz,jobject obj,jobject objCp,int transportType)
{
    int handle = 0;
    Parcel *data = parcelForJavaObject(env, obj);
    Parcel *dataCp = parcelForJavaObject(env, objCp);
    gTid = gettid();
    ALOGD(" %s %d ++++ gTid(%d) tid %d",__FUNCTION__,__LINE__,gTid,gettid());
    handle = iAP2ClientStart(notify,
                                            (void *)data,
                                            (void *)dataCp,
                                            isConnected,
                                            transportType,
                                            iAP2Jni_AttachedCurentThread,
                                            iAP2Jni_ExternalDoWrite,
                                            iAP2Jni_DetachedCurentThread
                                            );
    //ALOGD(" %s %d handle 0x%x",__FUNCTION__,__LINE__,handle);
    return handle;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2StopMain(
                                                                    JNIEnv* env, jobject thiz,int handle)
{
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    int ret = 0;
    ret = iAP2ClientStop(handle);	
    //ALOGD(" %s %d ----",__FUNCTION__,__LINE__);
    return (ret == 0);
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2TestEnable(
                                                                        JNIEnv* env, jobject thiz,int handle)
{
    int ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientTestEnable(handle);
    //ALOGD(" %s %d ----",__FUNCTION__,__LINE__);
    return (ret == 0);
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2TestDisable(
                                                                            JNIEnv* env, jobject thiz,int handle)
{
    int ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientTestDisable(handle);
    //ALOGD(" %s %d ----",__FUNCTION__,__LINE__);
    return (ret == 0);
}


JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2SendLocationData(
                                                                    JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientSendLocationData(handle,data);
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2SendPowerSourceUpdate(
                                                    JNIEnv* env, jobject thiz,int handle,int support,int current)
{
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientSendPowerSourceUpdate(handle,support,current);
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2SendVehicleStatus(
                                                        JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientSendVehicleStatusData(handle,data);
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2CopyCertificate(
                                                JNIEnv* env, jobject thiz,int handle,jobject outObj)
{
    Parcel *data = parcelForJavaObject(env, outObj);
    int ret = 0;
    ALOGD(" %s %d ++++ tid %d",__FUNCTION__,__LINE__,gettid());
    ret = iAP2ClientCopyCertificate(handle,data);
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    return (ret == 0);
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2CreateSignature(
                            JNIEnv* env, jobject thiz,int handle,jobject inObj,jobject outObj)
{
    int ret = 0;
    Parcel *data = parcelForJavaObject(env, inObj);
    Parcel *reply = parcelForJavaObject(env, outObj);
    ALOGD(" %s %d ++++ tid %d",__FUNCTION__,__LINE__,gettid());
    ret = iAP2ClientCreateSignature(handle,data,reply);
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    return (ret == 0);
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2GetCertificate(
                                                JNIEnv* env, jobject thiz,jobject outObj)
{
    Parcel *data = parcelForJavaObject(env, outObj);
    int ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientGetCertificate(data);
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    return (ret == 0);
}


JNIEXPORT int JNICALL Java_com_tcc_iap2server_Iap2Service_Iap2CpInit(
                                                                                JNIEnv* env, jobject thiz,jobject obj)
{
    int handle = 0;
    ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    handle = iAP2ClientCpInit();
    ALOGD(" %s %d handle 0x%x",__FUNCTION__,__LINE__,handle);
    return handle;
}

JNIEXPORT void JNICALL Java_com_tcc_iap2server_Iap2Service_Iap2CpDeinit(    
                                                                                    JNIEnv* env, jobject thiz,int handle)
{
    ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    iAP2ClientCpDeinit(handle);
    ALOGD(" %s %d handle 0x%x",__FUNCTION__,__LINE__,handle);
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2CopyCertificateWithoutLink(
                                                                    JNIEnv* env, jobject thiz,int handle,jobject outObj)
{
    Parcel *data = parcelForJavaObject(env, outObj);
    int ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientCopyCertificateWithoutLink(handle,data);
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    return (ret == 0);
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2CreateSignatureWithoutLink(
                                                JNIEnv* env, jobject thiz,int handle,jobject inObj,jobject outObj)
{
    int ret = 0;
    Parcel *data = parcelForJavaObject(env, inObj);
    Parcel *reply = parcelForJavaObject(env, outObj);
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientCreateSignatureWithoutLink(handle,data,reply);
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    return (ret == 0);
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2TelephonyStartCallState(
                                                                                    JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientTelephonyStartCallState(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2TelephonyStopCallState(
                                                                                        JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientTelephonyStopCallState(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2TelephonyStartUpdates(
                                                                                JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientTelephonyStartUpdates(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2TelephonyStopUpdates(
                                                                                    JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientTelephonyStopUpdates(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
    Java_com_tcc_iap2server_Iap2Service_Iap2CommunicationInitialCall(
                                                            JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientCommunicationInitialCall(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
    Java_com_tcc_iap2server_Iap2Service_Iap2CommunicationAcceptCall(
                                                            JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientCommunicationAcceptCall(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
    Java_com_tcc_iap2server_Iap2Service_Iap2CommunicationEndCall(
                                                            JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientCommunicationEndCall(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
    Java_com_tcc_iap2server_Iap2Service_Iap2CommunicationSwapCalls(
                                                            JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientCommunicationSwapCalls(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
    Java_com_tcc_iap2server_Iap2Service_Iap2CommunicationMergeCalls(
                                                            JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientCommunicationMergeCalls(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
    Java_com_tcc_iap2server_Iap2Service_Iap2CommunicationHoldStatusUpdate(
                                                            JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientCommunicationHoldStatusUpdate(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
    Java_com_tcc_iap2server_Iap2Service_Iap2CommunicationMuteStatusUpdate(
                                                            JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientCommunicationMuteStatusUpdate(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
    Java_com_tcc_iap2server_Iap2Service_Iap2CommunicationSendDTMF(
                                                            JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientCommunicationSendDTMF(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
    Java_com_tcc_iap2server_Iap2Service_Iap2CommunicationStartUpdateList(
                                                            JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientCommunicationStartUpdateList(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
    Java_com_tcc_iap2server_Iap2Service_Iap2CommunicationStopUpdateList(
                                                            JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientCommunicationStopUpdateList(handle);
    return ret;
}


JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2NowPlayingStart(
                                                                    JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientNowPlayingStart(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
    Java_com_tcc_iap2server_Iap2Service_Iap2NowPlayingStop(
                                                                    JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientNowPlayingStop(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
    Java_com_tcc_iap2server_Iap2Service_Iap2NowPlayingSetInfo(
                                                            JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientNowPlayingSetInfo(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2StartPowerUpdates(
                                                                        JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientStartPowerUpdates(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2SendBtCompInfo(
                                                                    JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientSendBtCompInfo(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2SendBtCompInfoUpdate(
                                                            JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientSendBtCompInfoUpdate(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2StartBtConnectUpdates(
                                                                                JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientStartBtConnectUpdates(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2StopBtConnectUpdates(
                                                                                    JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientStopBtConnectUpdates(handle);
    return ret;
}



JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2AppLaunch(
                                        JNIEnv* env, jobject thiz,int handle,jobject outObj)
{
    Parcel *data = parcelForJavaObject(env, outObj);
    BOOL ret = 0;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientAppLaunch(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2SendEaData(
                                                        JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientSendEaData(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2SendEaSessionStatus(
                                                        JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientSendEaSessionStatus(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2IdentificationUpdate(
                                                        JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientIndentificationUpdate(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2HidStart(
                                                        JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientHidStart(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2HidStop(
                                                        JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientHidStop(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2HidAccessoryReport(
                                                        JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientHidAccessoryReport(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2MLAStartInformation(
                                                        JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientMLAStartInformation(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2MLAStopInformation(
                                                        JNIEnv* env, jobject thiz,int handle)
{
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientMLAStopInformation(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2MLAStartUpdate(
                                                        JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientMLAStartUpdate(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2MLAStopUpdate(
                                                        JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientMLAStopUpdate(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2MLACurrentSelection(
                                                        JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientMLACurrentSelection(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2MLALibraryItems(
                                                        JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientMLALibraryItems(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2MLACollection(
                                                        JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientMLACollection(handle,data);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2MLASpecial(
                                                        JNIEnv* env, jobject thiz,int handle,jobject obj)
{
    Parcel *data = parcelForJavaObject(env, obj);
    BOOL ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientMLASpecial(handle,data);
    return ret;
}

JNIEXPORT 
    jint 
    JNICALL 
     Java_com_tcc_iap2server_Iap2Service_Iap2MLAGetMaxLibraryItemsCount(
                                                        JNIEnv* env, jobject thiz,int handle)
{
    int ret;
    //ALOGD(" %s %d ++++",__FUNCTION__,__LINE__);
    ret = iAP2ClientMLAGetMaxLibraryItemsCount(handle);
    return ret;
}

JNIEXPORT 
    jboolean 
    JNICALL
    Java_com_tcc_iap2server_Iap2Service_Iap2GetUsbInfo(
                                                         JNIEnv* env, jobject thiz,  jobject java_reply)
{
    Parcel *reply = NULL;
    if(java_reply == NULL){
        return false;
    }
    reply = parcelForJavaObject(env, java_reply);
    return iAP2ClientGetUsbInfo(reply);
}
#endif

JNIEXPORT 
    void 
    JNICALL
    Java_com_ipod_Iap2Service_registerIap2EP(JNIEnv* env, jobject thiz,  jobject obj_ep)
{
   iap2_ep_object = (jobject)env->NewGlobalRef(obj_ep);
}



JNIEXPORT 
	void 
	JNICALL
	Java_com_ipod_carlife_manager_iap2manager_Iap2Service_accessoryAuthenticationDoChallengeData(JNIEnv* env, jobject thiz,  jint dataLen,jbyteArray dataArray,jint session)
{

		
	   //把jbyteArray转C用BYTE数组	
			  
		 
		 
			   jbyte *jbarray = (jbyte *)malloc(dataLen * sizeof(jbyte));  
		 
		 
			   env->GetByteArrayRegion(dataArray,0,dataLen,jbarray);  
		 
		 
			   uint8_t *data = (uint8_t *)jbarray;//uint8_t 就是byte  
			   

	   AccessoryAuthenticationDoChallengeData(dataLen,data,session);
}


JNIEXPORT 
    void 
    JNICALL
    Java_com_ipod_carlife_manager_iap2manager_Iap2Service_accessoryAuthenticationSerialNumber(JNIEnv* env, jobject thiz,  jint session)
{
   AccessoryAuthenticationSerialNumber(session);
}



JNIEXPORT 
    void 
    JNICALL
    Java_com_ipod_carlife_manager_iap2manager_Iap2Service_registerSessCtrlDispatchMsg(JNIEnv* env, jobject thiz,  jobject obj_ep)
{
   iap2_sessCtrlDispatch_object = (jobject)env->NewGlobalRef(obj_ep);
}

JNIEXPORT 
    void 
    JNICALL
    Java_com_ipod_carlife_manager_iap2manager_Iap2Service_iAP2ClientStart (JNIEnv* env,
												   jobject thiz/*iAP2Notify_callback noti_cb,
                                    void *pData,
                                    void *pCpData,
                                    iAP2ChkConn_callback chkConn_cb,
                                    int transportType,
                                    iAP2ExtWriteAttached_callback attached_cb,
                                    iAP2ExtDoWrite_callback doWrite_cb,
                                    iAP2ExtWriteDetached_callback detached_cb
                                    */)
{
   iAP2ClientStart();
}





#ifdef __cplusplus
}
#endif	// __cplusplus

