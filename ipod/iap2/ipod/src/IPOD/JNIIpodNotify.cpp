/*
**
** Copyright (C) 2013 Telechips Inc.
** Copyright 2008, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#define LOG_TAG "IIpodService"
#include <utils/Log.h>

#include <stdint.h>
#include <sys/types.h>
#include <binder/Parcel.h>
#include "IIpodService.h"
#include "JNIIpodNotify.h"
#include <unistd.h>
#include <utils/String8.h>
#include <cutils/properties.h>
#include <nativehelper/JNIHelp.h>
#include "iAP2_utils.h"
#include "android_runtime/AndroidRuntime.h"
#include <iPod_jni.h>

#include <utils/Log.h>

int IIAPSVC_DEBUG	   =  -1;
#define IIAPSVC_DBUG(msg...)	if (IIAPSVC_DEBUG == 1) { ALOGD( " " msg); }

namespace android {


JNITelechipsIap1Listener::JNITelechipsIap1Listener()
{
    ALOGI(" JNITelechipsIap1Listener ");
    serverDied = false;
}

JNITelechipsIap1Listener::~JNITelechipsIap1Listener()
{
  //  ALOGI(" ~JNITelechipsIap1Listener %s",miAP1ListenerName.string());
   // clearService();
}

void JNITelechipsIap1Listener::clearService(void)
{
    //miAP1ClientLock.lock();
   // ALOGI(" ~clearService  API this %p serverDied %d get(%p) %s",this,serverDied,giAP1Service.get(),miAP1ListenerName.string());
    //if(giAP1Service.get() != 0){
      //  status_t ret = giAP1Service->asBinder()->unlinkToDeath(this);
        //giAP1Service.clear();
        //ALOGI(" ~Check  ret %d  get(%p)",ret,giAP1Service.get());
   // }
    //miAP1ClientLock.unlock();
}

void JNITelechipsIap1Listener::notify(int msg, int ext1, int ext2,int ext3)
{
    //IAP1JNI_DBUG("JNITelechipsIap1Listener::notify");
    if((unsigned int)(msg & 0xF0000) != 0xF0000){
       // callByLibraryiAP1(msg,ext1,ext2,ext3);
    }else{
        switch(msg){
			case NOTIFY_CODE_MEDIA_PLAYBACK_STATUS:
				OnPlayBackStatus( ext1);
				break;
			case NOTIFY_CODE_MEDIA_PLAYBACK_POSITION:
				OnPlayBackPosition( ext1);
				break;
			case NOTIFY_CODE_DURATION:
				 OnDuration(ext1);
				break;
			case NOTIFY_CODE_SAMPLERATE: //audio sampelrate
			   OnUSBDeviceModeAudioInformation(ext1);
				break;
            case 0xF0001:
                ALOGI(" IapService died 222 ~~~~~~~~~~~~~~~~~~ ");
               // serverDied = true;
               // mIap1SvcReconnect = true;
               // clearService();
               // callByLibraryiAP1(msg,ext1,ext2,ext3);
                break;
            case 0xF0002:
                ALOGI(" Listener is registered tid %d",gettid());
                break;
            case 0xF0003:
                ALOGI(" Listener is removed tid %d callService",gettid());
               // clearService();
                break;
        }
    }
}


	void JNITelechipsIap1Listener::notifyRawByte(int msg,  int ext1, const unsigned char	*obj) 
	{
	
		
	
		switch(msg){
		case NOTIFY_CODE_MEDIA_PCM_DATA_RECEIVE:
			//ALOGD("%s %d",__FUNCTION__,__LINE__);
			OnPcmDataReceive(obj,ext1);
			break;
			}
		
	  
	}


void JNITelechipsIap1Listener::notifyRawString(int msg,  int ext1, const unsigned char  *obj) 
{

	

	switch(msg){

    case NOTIFY_CODE_MEDIA_TITLE:
		
		OnMediaItemTile(obj,ext1);
		break;
	case NOTIFY_CODE_MEDIA_ARTIST:
		
		OnMediaItemArtist(obj,ext1);
		break;
	case NOTIFY_CODE_MEDIA_ABLUM_TITLE:
		
		
		OnMediaItemAlumTitle(obj,ext1);
		break;
	case NOTIFY_CODE_MEDIA_ABLUM_ARTIST:
		
		OnMediaItemAlumArtist(obj,ext1);
		break;
	}
  
}


void JNITelechipsIap1Listener::notifyObj(int what,int arg1, int arg2,const Parcel *obj)
{
    //IAP1JNI_DBUG("JNITelechipsIap1Listener::notifyObj");
    if((unsigned int)(what & 0xF0000)  == 0xF0000){
        ALOGI(" IapService died ~~~~~~~~~~~~~~~~~~");
    }else if(((unsigned int)what&0x00100) == 0x00100){
      ALOGI("111 unsigned int)what&0x00100) == 0x00100");
        //callByLibraryObjiAP1(what,arg1,arg2,obj);
    }else{
        JNIEnv *env = android::AndroidRuntime::getJNIEnv();
        obj->setDataPosition(0);
        jclass IpodPlayerClazz = (jclass)obj->readInt32();
        jmethodID MethodRspID = (jmethodID)obj->readInt32();          
        jobject jParcel = createJavaParcelObject(env);
        Parcel* nativeParcel = parcelForJavaObject(env, jParcel);
        nativeParcel->appendFrom(obj,obj->dataPosition(),obj->dataAvail());
        nativeParcel->setDataPosition(0);
        
       // env->CallStaticVoidMethod(IpodPlayerClazz, MethodRspID, jParcel);                
        //env->DeleteLocalRef(jParcel);            
       // env->DeleteGlobalRef(IpodPlayerClazz);
    }
}

void JNITelechipsIap1Listener::binderDied(const wp<IBinder>& who)
{
    notify(0xF0001,0,0,0);
}
};
