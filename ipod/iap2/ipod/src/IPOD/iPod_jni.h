
#ifndef IPOD_JNI_H
#define IPOD_JNI_H
#include <jni.h>



#define  TAG    "IPODJni"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/stat.h>
#include <linux/input.h>
#include <linux/hidraw.h>
#include <cutils/properties.h>
#include <utils/Log.h>
#include <sys/times.h>




#include "iPod_PAL.h"
#include "iPod_CP.h"
#include "iPod_OS.h"
#include "iPod_HID.h"
#include "iPod_interface.h"


#include "usbi.h"
#include "iAP2Client.h"


#include <nativehelper/JNIHelp.h>
#include <iPod_jni.h>
//iAP2ClientStart
#include "IIpodService.h"
#include "JNIIpodNotify.h"

#include <binder/IServiceManager.h>
#include <utils/Log.h>
#include <binder/IPCThreadState.h>
#include <utils/threads.h>
#include <cutils/properties.h>
#include "android_runtime/AndroidRuntime.h"
#include "iAP2_utils.h"


#include <utils/Log.h>


#if 1
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "IPODJni", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "IPODJni", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , "IPODJni", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "IPODJni", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , "IPODJni", __VA_ARGS__)
#else
#define LOGV(...)
#define LOGD(...)
#define LOGI(...)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG, __VA_ARGS__)
#endif



#if defined(__cplusplus)
extern "C"
{
#endif

extern sp<IIpodService>   gIpodService ;
JNIEnv* getIPODJNIEnv();
jint JNI_OnLoad(JavaVM* vm, void* reserved);

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void *reserved) ;


#if defined(__cplusplus)
};
#endif


#endif

