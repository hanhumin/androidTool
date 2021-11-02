/****************************************************************************
 *   FileName    : iPod_HID.c
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
#include "PcmServiceNotify.h"

#include <binder/IServiceManager.h>
#include <utils/Log.h>
#include <binder/IPCThreadState.h>
#include <utils/threads.h>
#include <cutils/properties.h>
#include "android_runtime/AndroidRuntime.h"
#include "iAP2_utils.h"



//

//using namespace CIPODJNIH;

using namespace android;


#if defined(__cplusplus)
extern "C"
{
#endif



  void OnNowPlayingMediaItemAttributes(uint8_t	*data, u32 dataLen){
   if(obj_NowPlayingMediaItemAttributes==NULL)
		return;
	  
		uint8_t byteArray[dataLen];   
		JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemRawData", "(I[B)V");
		jbyteArray	jbarray = env->NewByteArray(dataLen);//建立jbarray数组//在heap里面申请， 不需要释放
  
		memcpy(byteArray,data,dataLen);
	   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	   env->SetByteArrayRegion(jbarray, 0, dataLen, jy);	
	  
		env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,dataLen,jbarray);
		env->DeleteLocalRef(jbarray);
		env->DeleteLocalRef(obj_class);
  }
  
  void OnNowPlayingPlayBackAttributes(uint8_t  *data, u32 dataLen){
  	if(obj_NowPlayingPlaybackAttributes==NULL)
	return;

	
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnPlaybackAttributesRawData", "(I[B)V");
	  uint8_t byteArray[dataLen];	
	  jbyteArray  jbarray = env->NewByteArray(dataLen);//建立jbarray数组//在heap里面申请， 不需要释放

	  memcpy(byteArray,data,dataLen);
	 jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	 env->SetByteArrayRegion(jbarray, 0, dataLen, jy);	  

	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,dataLen,jbarray);
	  env->DeleteLocalRef(jbarray);
	  env->DeleteLocalRef(obj_class);
  }

				  
				  
  void OnMediaItemAlumTitle(uint8_t *data, u32 len){
	   if(obj_NowPlayingMediaItemAttributes==NULL)
	  return;
	   
	   LOGI("OnMediaItemAlumTitle() is invoked");
	  JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		
		jmethodID getName_method = env->GetMethodID(obj_class, "onMediaItemAlumTitle", "(ILjava/lang/String;)V");
		 
	  
	   jstring jTitle =  (jstring)env->NewStringUTF((char *) data);		
		env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,len,jTitle);
		  env->DeleteLocalRef(jTitle);
		 env->DeleteLocalRef(obj_class);
  }
				  
				  
  void OnMediaItemGenreComposer(uint8_t * data, u32 len){
	   if(obj_NowPlayingMediaItemAttributes==NULL)
	  return;
	   
	   LOGI("OnMediaItemGenreComposer() is invoked");
	  JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		
		jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemGenreComposer", "(ILjava/lang/String;)V");
		
		 jstring jGenre =  (jstring)env->NewStringUTF((char *) data); 			//将Jbyte 转换为jbarray数组
		env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,len,jGenre);
  env->DeleteLocalRef(jGenre);
 env->DeleteLocalRef(obj_class);
}
		  
		  
  void OnMediaItemGenre(uint8_t * data, u32 len){
	   if(obj_NowPlayingMediaItemAttributes==NULL)
	  return;
	   
	   LOGI("OnMediaItemGenre() is invoked");
	  JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		
		jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemGenre", "(ILjava/lang/String;)V");
		
		 jstring jGenre =  (jstring)env->NewStringUTF((char *) data); 		//将Jbyte 转换为jbarray数组
		env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,len,jGenre);
		  env->DeleteLocalRef(jGenre);
		 env->DeleteLocalRef(obj_class);
  }
  
  
  void OnMediaItemAlumArtist(uint8_t * data, u32 len){
	   if(obj_NowPlayingMediaItemAttributes==NULL)
	  return;
	   
	   LOGI("OnMediaItemAlumArtist() is invoked");
	  JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);

		jmethodID getName_method = env->GetMethodID(obj_class, "onMediaItemAlumArtist", "(ILjava/lang/String;)V");
			
			  jstring jArtist =  (jstring)env->NewStringUTF((char *) data); 		//将Jbyte 转换为jbarray数组

		
			//将Jbyte 转换为jbarray数组
		env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,len,jArtist);
		  env->DeleteLocalRef(jArtist);
		 env->DeleteLocalRef(obj_class);
  }
  
  
  
  void OnMediaItemArtist(uint8_t *data, u32 len){
	   if(obj_NowPlayingMediaItemAttributes==NULL)
	  return;
	   
	   LOGI("OnMediaItemArtist() is invoked data:%s",data);
	  JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		jmethodID getName_method = env->GetMethodID(obj_class, "onMediaItemAlumTitle", "(ILjava/lang/String;)V");
		
		  jstring jArtist =  (jstring)env->NewStringUTF((char *) data);			//将Jbyte 转换为jbarray数组
		env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,len,jArtist);
		  env->DeleteLocalRef(jArtist);
		 env->DeleteLocalRef(obj_class);
  }
  
 
  
  void OnMediaItemAlbumPID(uint8_t *data, u32 len){
	   if(obj_NowPlayingMediaItemAttributes==NULL)
		return;
	  
		uint8_t byteArray[len];   
		JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemAlbumPID", "(I[B)V");
		jbyteArray	jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放
  
		memcpy(byteArray,data,len);
	   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	   env->SetByteArrayRegion(jbarray, 0, len, jy);	
	  
		env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,len,jbarray);
		env->DeleteLocalRef(jbarray);
		env->DeleteLocalRef(obj_class);
  }
  
  void OnMediaItemComposerPID(uint8_t *data, u32 len){
	   if(obj_NowPlayingMediaItemAttributes==NULL)
		return;
	  
		uint8_t byteArray[len];   
		JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemComposerPID", "(I[B)V");
		jbyteArray	jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放
  
		memcpy(byteArray,data,len);
	   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	   env->SetByteArrayRegion(jbarray, 0, len, jy);	
	  
		env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,len,jbarray);
		env->DeleteLocalRef(jbarray);
		env->DeleteLocalRef(obj_class);
	  }
		  
  void OnMediaItemGenrePID(uint8_t *data, u32 len){
	   if(obj_NowPlayingMediaItemAttributes==NULL)
		return;
	  
		uint8_t byteArray[len];   
		JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemGenrePID", "(I[B)V");
		jbyteArray	jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放
  
		memcpy(byteArray,data,len);
	   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	   env->SetByteArrayRegion(jbarray, 0, len, jy);	
	  
		env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,len,jbarray);
		env->DeleteLocalRef(jbarray);
		env->DeleteLocalRef(obj_class);
  }
  
  void OnMediaItemAlbumArtistPID(uint8_t *data, u32 len){
	   if(obj_NowPlayingMediaItemAttributes==NULL)
		return;
	  
		uint8_t byteArray[len];   
		JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemAlbumArtistPID", "(I[B)V");
		jbyteArray	jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放
  
		memcpy(byteArray,data,len);
	   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	   env->SetByteArrayRegion(jbarray, 0, len, jy);	
	  
		env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,len,jbarray);
		env->DeleteLocalRef(jbarray);
		env->DeleteLocalRef(obj_class);
  }
  
  void OnMediaItemArtistPID(uint8_t *data, u32 len){
	   if(obj_NowPlayingMediaItemAttributes==NULL)
		return;
	  
		uint8_t byteArray[len];   
		JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemArtistPID", "(I[B)V");
		jbyteArray	jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放
  
		memcpy(byteArray,data,len);
	   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	   env->SetByteArrayRegion(jbarray, 0, len, jy);	
	  
		env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,len,jbarray);
		env->DeleteLocalRef(jbarray);
		env->DeleteLocalRef(obj_class);
  }
  
  void OnMediaItemPID(uint8_t *data, u32 len){
	   if(obj_NowPlayingMediaItemAttributes==NULL)
		return;
	  
		uint8_t byteArray[len];   
		JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemPID", "(I[B)V");
		jbyteArray	jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放
  
		memcpy(byteArray,data,len);
	   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	   env->SetByteArrayRegion(jbarray, 0, len, jy);	
	  
		env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,len,jbarray);
		env->DeleteLocalRef(jbarray);
		env->DeleteLocalRef(obj_class);
  }
  
  
  void OnMeidaLibraryUID(uint8_t  *data, u32 len){
	   if(obj_MediaLibraryInformation==NULL)
	  return;
	  
		uint8_t byteArray[len];   
	   LOGI("OnMeidaLibraryUID() is invoked");
	  JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_MediaLibraryInformation);
		jmethodID getName_method = env->GetMethodID(obj_class, "OnMeidaLibraryUID", "(I[B)V");
		jbyteArray	jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放
  
		memcpy(byteArray,data,len);
	   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	   env->SetByteArrayRegion(jbarray, 0, len, jy);	
		env->CallVoidMethod(obj_MediaLibraryInformation,getName_method,len,jbarray);
		 env->DeleteLocalRef(jbarray);
		 env->DeleteLocalRef(obj_class);
  }
  
  void OnMediaLibraryUParentpdatePlaylistID(uint8_t   *data, u32 len){
	   if(obj_MediaLibraryUpdatePlayList==NULL)
		return;
	  
		uint8_t byteArray[len];   
		JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdatePlayList);
		jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaLibraryUParentpdatePlaylistID", "(I[B)V");
		jbyteArray	jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放
  
		memcpy(byteArray,data,len);
	   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	   env->SetByteArrayRegion(jbarray, 0, len, jy);	
	  
		env->CallVoidMethod(obj_MediaLibraryUpdatePlayList,getName_method,len,jbarray);
		env->DeleteLocalRef(jbarray);
		env->DeleteLocalRef(obj_class);
  }
  void OnMediaLibraryUpdateParentPlaylistID(u8 *data,u32 len){
	   if(obj_MediaLibraryUpdatePlayList==NULL)
		return;
	  
		uint8_t byteArray[len];   
		JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdatePlayList);
		jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaLibraryUpdateParentPlaylistID", "(I[B)V");
		jbyteArray	jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放
  
		memcpy(byteArray,data,len);
	   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	   env->SetByteArrayRegion(jbarray, 0, len, jy);	
	  
		env->CallVoidMethod(obj_MediaLibraryUpdatePlayList,getName_method,len,jbarray);
		env->DeleteLocalRef(jbarray);
		env->DeleteLocalRef(obj_class);
  }
  
  void OnMediaLibraryUpdatePlaylistID(uint8_t *data, u32 len){
	   if(obj_MediaLibraryUpdatePlayList==NULL)
		return;
	  
		uint8_t byteArray[len];   
		JNIEnv *env = getIPODJNIEnv();
		jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdatePlayList);
		jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaLibraryUpdatePlaylistID", "(I[B)V");
		jbyteArray	jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放
  
		memcpy(byteArray,data,len);
	   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	   env->SetByteArrayRegion(jbarray, 0, len, jy);	
	  
  env->CallVoidMethod(obj_MediaLibraryUpdatePlayList,getName_method,len,jbarray);
  env->DeleteLocalRef(jbarray);
  env->DeleteLocalRef(obj_class);
}

void OnPBAppleMusicRadioStationMediaPlaylistID(uint8_t	*data, u32 len){
	if(obj_NowPlayingPlaybackAttributes==NULL)
	return;

	
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnPBAppleMusicRadioStationMediaPlaylistID", "(I[B)V");
	  uint8_t byteArray[len];	
	  jbyteArray  jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放

	  memcpy(byteArray,data,len);
	 jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	 env->SetByteArrayRegion(jbarray, 0, len, jy);	  

	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,len,jbarray);
	  env->DeleteLocalRef(jbarray);
	  env->DeleteLocalRef(obj_class);
}
	
void OnPcmDataReceive(uint8_t  *data, u32 len){
	 if(obj_OnPcmDataReceiver==NULL)
	  return;
	
	  uint8_t byteArray[len];	
	  JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_OnPcmDataReceiver);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnPcmDataReceive", "(I[B)V");
	  jbyteArray  jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放

	  memcpy(byteArray,data,len);
	 jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	 env->SetByteArrayRegion(jbarray, 0, len, jy);	  
	
	  env->CallVoidMethod(obj_OnPcmDataReceiver,getName_method,len,jbarray);
	  env->DeleteLocalRef(jbarray);
	  env->DeleteLocalRef(obj_class);
}
	
	
void OnPBAppleMusicRadioStationName(uint8_t *data, u32 len){
	 if(obj_NowPlayingPlaybackAttributes==NULL)
	return;
	
	  uint8_t byteArray[len];	
	 LOGI("OnPBAppleMusicRadioStationName() is invoked");
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnPBAppleMusicRadioStationName", "(ILjava/lang/String;)V");
	  jbyteArray  jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放

	 jstring jName =  (jstring)env->NewStringUTF((char *) data); 
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,len,jName);
	   env->DeleteLocalRef(jName);
	   env->DeleteLocalRef(obj_class);
	   
	}
	
void OnPBMediaLibraryUID(uint8_t	*data, u32 len){
	 if(obj_NowPlayingPlaybackAttributes==NULL)
	return;
	
	  uint8_t byteArray[len];	
	 LOGI("OnPBMediaLibraryUID() is invoked");
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnPBMediaLibraryUID", "(ILjava/lang/String;)V");
	  jbyteArray  jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放

	  jstring jUid =  (jstring)env->NewStringUTF((char *) data);
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,len,jUid);
	   env->DeleteLocalRef(jUid);
	   env->DeleteLocalRef(obj_class);
	   
}

void onPlayBackBundleID(uint8_t *data, u32 len){
	 if(obj_NowPlayingPlaybackAttributes==NULL)
	return;
	
	  uint8_t byteArray[len];	
	 LOGI("onPlayBackBundleID() is invoked");
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "onPlayBackBundleID", "(ILjava/lang/String;)V");
	  jbyteArray  jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放

	  jstring jUid =  (jstring)env->NewStringUTF((char *) data);  
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,len,jUid);
	   env->DeleteLocalRef(jUid);
	   env->DeleteLocalRef(obj_class);
	   
}


	
void OnMediaLibraryUpdatePlaylistName(uint8_t	*data, u32 len){
	 if(obj_MediaLibraryUpdatePlayList==NULL)
	return;
	
	  uint8_t byteArray[len];	
	 LOGI("OnMediaLibraryUpdatePlaylistName() is invoked");
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdatePlayList);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaLibraryUpdatePlaylistName", "(ILjava/lang/String;)V");

	 
	 jstring jName =  (jstring)env->NewStringUTF((char *) data);
				 
	  env->CallVoidMethod(obj_MediaLibraryUpdatePlayList,getName_method,len,jName);
	   env->DeleteLocalRef(jName);
	   env->DeleteLocalRef(obj_class);
	   
}

void OnPlayBackAppName(uint8_t	*data, u32 len){
	 if(obj_NowPlayingPlaybackAttributes==NULL)
	return;
	
	  uint8_t byteArray[len];	
	 LOGI("OnPlayBackAppName() is invoked");
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnPlayBackAppName", "(ILjava/lang/String;)V");
	  jbyteArray  jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放

	  jstring jName =  (jstring)env->NewStringUTF((char *) data);  
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,len,jName);
	   env->DeleteLocalRef(jName);
	   env->DeleteLocalRef(obj_class);
	   
}
	
	
	



void OnMediaLibraryName(uint8_t  *data, u32 len){
	 if(obj_MediaLibraryInformation==NULL)
	return;
	
	  uint8_t byteArray[len];	
	 LOGI("OnMediaLibraryName() is invoked\n");
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_MediaLibraryInformation);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaLibraryName", "(I[B)V");
	  jbyteArray  jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放

	  memcpy(byteArray,data,len);
	 jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	 env->SetByteArrayRegion(jbarray, 0, len, jy);	  
	  env->CallVoidMethod(obj_MediaLibraryInformation,getName_method,len,jbarray);
	   env->DeleteLocalRef(jbarray);
	   env->DeleteLocalRef(obj_class);
	   
}
	
void OnMediaItemTile(uint8_t  *data, u32 len){
	 if(obj_NowPlayingMediaItemAttributes==NULL)
	return;
	
	  uint8_t byteArray[len];	
	 LOGI("OnMediaItemTile() is invoked");
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "onMediaItemTile", "(ILjava/lang/String;)V");

	  
	  jstring jTitle =  (jstring)env->NewStringUTF((char *) data);
	 
	  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,len,jTitle);
	   env->DeleteLocalRef(jTitle);
	   env->DeleteLocalRef(obj_class);
	   
}
	
	//second
	void OnDuration(int duration)
	{
	if(obj_NowPlayingMediaItemAttributes == NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
	  
	  jmethodID getName_method = env->GetMethodID(obj_class, "onDuration", "(I)V");
             
	  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,duration);
	   env->DeleteLocalRef(obj_class);
}

void OnPlayBackRepeatMode(int status)
{
	if(obj_NowPlayingPlaybackAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnPlayBackRepeatMode", "(I)V");
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,status);
	   env->DeleteLocalRef(obj_class);
}


void OnMediaUpdatePlaylistIsRadioStation(int status)
{
	if(obj_MediaLibraryUpdatePlayList== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdatePlayList);
	  jboolean b = false;
	  if(status == 1)
		b=true;
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaUpdatePlaylistIsRadioStation", "(Z)V");
	  env->CallVoidMethod(obj_MediaLibraryUpdatePlayList,getName_method,b);
	   env->DeleteLocalRef(obj_class);

}

void OnNowPlayingPBQContentsFinish(uint8_t *data, u32 len)
{
	 if(obj_NowPlayingPlaybackQueueContents==NULL)
	   return;
	 
	 LOGI("OnNowPlayingPBQContentsFinish() is invoked data:%s\n",data);
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackQueueContents);
	  
	  jmethodID getName_method = env->GetMethodID(obj_class, "onFinish", "(Ljava/lang/String;)V");

	 
	 jstring jFileName =	(jstring)env->NewStringUTF((char *) data);	
	 
	  env->CallVoidMethod(obj_NowPlayingPlaybackQueueContents,getName_method,jFileName);
		env->DeleteLocalRef(jFileName);
	   env->DeleteLocalRef(obj_class);

}

void OnMediaItemListUpdatePlaylistContentsFinish(int fileName_len, const char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libraryUID)
{
	 if(obj_MediaItemListUpdatePlayListContents==NULL)
	   return;
	 
	 LOGI("OnMediaItemListUpdatePlaylistContentsStart() is invoked \n");
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_MediaItemListUpdatePlayListContents);
	  
	  jmethodID getName_method = env->GetMethodID(obj_class, "onFinish", "(Ljava/lang/String;I[BILjava/lang/String;)V");

	 
	 jstring jFileName =	(jstring)env->NewStringUTF( fileName);	

		
			jbyteArray	jbarrayPid = env->NewByteArray(playListPid_len);			
			jbyte *jy=(jbyte*)playListPid; 
			env->SetByteArrayRegion(jbarrayPid, 0, playListPid_len, jy);  

			
			jstring	jbarrayUid = (jstring)env->NewStringUTF((char *) libraryUID);	 		
		
				
	  env->CallVoidMethod(obj_MediaItemListUpdatePlayListContents,getName_method,jFileName,playListPid_len,jbarrayPid,libraryUID_len,libraryUID);
		env->DeleteLocalRef(jFileName);
		env->DeleteLocalRef(jbarrayPid);
		env->DeleteLocalRef(jbarrayUid);		
	   env->DeleteLocalRef(obj_class);

}


void OnMediaItemListUpdatePlaylistContentsStart( int fileName_len, const char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char	*libraryUID)
{
	 if(obj_MediaItemListUpdatePlayListContents==NULL)
	   return;
	 
	 LOGI("OnMediaItemListUpdatePlaylistContentsStart() is invoked \n");
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_MediaItemListUpdatePlayListContents);
	  
	  jmethodID getName_method = env->GetMethodID(obj_class, "onStart", "(Ljava/lang/String;I[BILjava/lang/String;)V");

	 
	 jstring jFileName =	(jstring)env->NewStringUTF( fileName);	

		
			jbyteArray	jbarrayPid = env->NewByteArray(playListPid_len);			
			jbyte *jy=(jbyte*)playListPid; 
			env->SetByteArrayRegion(jbarrayPid, 0, playListPid_len, jy);  

			jstring jbarrayUid =	(jstring)env->NewStringUTF( (char *)libraryUID);
				LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
				
	  env->CallVoidMethod(obj_MediaItemListUpdatePlayListContents,getName_method,jFileName,playListPid_len,jbarrayPid,libraryUID_len,libraryUID);
		env->DeleteLocalRef(jFileName);
		env->DeleteLocalRef(jbarrayPid);
		env->DeleteLocalRef(jbarrayUid);		
	   env->DeleteLocalRef(obj_class);

}

void OnMediaLibraryUpdatePlaylistContentsStart( int fileName_len, const char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libararyUID)
{
	 if(obj_MediaLibraryUpdatePlayListContents==NULL)
	   return;
	 
	 LOGI("OnMediaLibraryUpdatePlaylistContentsStart() is invoked fileName=%s fileName_len=%d\n",fileName,fileName_len);
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdatePlayListContents);
	 // LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	  jmethodID getName_method = env->GetMethodID(obj_class, "onStart", "(Ljava/lang/String;I[BILjava/lang/String;)V");
	  //LOGI("%s line:%d\n",__FUNCTION__,__LINE__);

	 
	 jstring jFileName =	(jstring)env->NewStringUTF( fileName);	

     	//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			jbyteArray	jbarrayPid = env->NewByteArray(playListPid_len);			
			jbyte *jy=(jbyte*)playListPid; 
			env->SetByteArrayRegion(jbarrayPid, 0, playListPid_len, jy);  
			//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			jstring jbarrayUid =    (jstring)env->NewStringUTF( (char *)libararyUID);  

			//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	  env->CallVoidMethod(obj_MediaLibraryUpdatePlayListContents,getName_method,jFileName,playListPid_len,jbarrayPid,libraryUID_len,jbarrayUid);
	 // LOGI("%s line:%d\n",__FUNCTION__,__LINE__);

		env->DeleteLocalRef(jFileName);
		//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		env->DeleteLocalRef(jbarrayPid);
		//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		env->DeleteLocalRef(jbarrayUid);	
		//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	   env->DeleteLocalRef(obj_class);
	   //LOGI("%s line:%d\n",__FUNCTION__,__LINE__);

}

void OnMediaLibraryUpdatePlaylistContentsFinish( int fileName_len, const char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libararyUID)
{
	LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);

	 if(obj_MediaLibraryUpdatePlayListContents==NULL)
	   return;
	 
	 LOGI("OnMediaLibraryUpdatePlaylistContentsFinish() is invoked \n");
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdatePlayListContents);
	  
	  jmethodID getName_method = env->GetMethodID(obj_class, "onFinish", "(Ljava/lang/String;I[BILjava/lang/String;)V");

	 
	 jstring jFileName =	(jstring)env->NewStringUTF( fileName);	

     	
			jbyteArray	jbarrayPid = env->NewByteArray(playListPid_len);			
			jbyte *jy=(jbyte*)playListPid; 
			env->SetByteArrayRegion(jbarrayPid, 0, playListPid_len, jy);  

			 jstring jbarrayUid =	(jstring)env->NewStringUTF( (char *)libararyUID);
				LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			
	 
	 
	  env->CallVoidMethod(obj_MediaLibraryUpdatePlayListContents,getName_method,jFileName,playListPid_len,jbarrayPid,libraryUID_len,jbarrayUid);
		env->DeleteLocalRef(jFileName);
		env->DeleteLocalRef(jbarrayPid);
		env->DeleteLocalRef(jbarrayUid);		
	   env->DeleteLocalRef(obj_class);

}

void OnMediaItemNowPlayingPBQContentsFinish(uint8_t *data, u32 len)
{
	 if(obj_NowPlayingPlaybackQueueContents==NULL)
	   return;
	 
	 LOGI("OnMediaItemNowPlayingPBQContentsFinish() is invoked data:%s\n",data);
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackQueueContents);
	  
	  jmethodID getName_method = env->GetMethodID(obj_class, "onFinish", "(Ljava/lang/String;)V");

	 
	 jstring jFileName =	(jstring)env->NewStringUTF((char *) data);	
	 
	  env->CallVoidMethod(obj_NowPlayingPlaybackQueueContents,getName_method,jFileName);
		env->DeleteLocalRef(jFileName);
	   env->DeleteLocalRef(obj_class);

}

void OnMediaItemNowPlayingPBQContentsStart(uint8_t *data, u32 len)
{
	 if(obj_NowPlayingPlaybackQueueContents==NULL)
	   return;
	 
	 LOGI("OnMediaItemArtist() is invoked data:%s\n",data);
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackQueueContents);
	  
	  jmethodID getName_method = env->GetMethodID(obj_class, "onStart", "(Ljava/lang/String;)V");

	 
	 jstring jFileName =	(jstring)env->NewStringUTF((char *) data);	
	 
	  env->CallVoidMethod(obj_NowPlayingPlaybackQueueContents,getName_method,jFileName);
		env->DeleteLocalRef(jFileName);
	   env->DeleteLocalRef(obj_class);

}

void OnNowPlayingPBQContentsStart(uint8_t *data, u32 len)
{
	 if(obj_NowPlayingPlaybackQueueContents==NULL)
	   return;
	 
	 LOGI("OnNowPlayingPBQContentsStart() is invoked data:%s\n",data);
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackQueueContents);
	  
	  jmethodID getName_method = env->GetMethodID(obj_class, "onStart", "(Ljava/lang/String;)V");

	 
	 jstring jFileName =	(jstring)env->NewStringUTF((char *) data);	
	 
	  env->CallVoidMethod(obj_NowPlayingPlaybackQueueContents,getName_method,jFileName);
		env->DeleteLocalRef(jFileName);
	   env->DeleteLocalRef(obj_class);

}
void OnMeidaNowPlayingArtworkStart()
{
	if(obj_NowPlayingArtwork== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingArtwork);
	 
	  jmethodID getName_method = env->GetMethodID(obj_class, "onDataStart", "()V");
	  env->CallVoidMethod(obj_NowPlayingArtwork,getName_method);
	   env->DeleteLocalRef(obj_class);
}


void OnIPODStartReady()
{
//	LOGI("%s %d\n",__FUNCTION__,__LINE__);

	if(obj_IpodStart== NULL)
		return ;
	
//	LOGI("%s %d\n",__FUNCTION__,__LINE__);
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_IpodStart);
	 
	 //LOGI("%s %d\n",__FUNCTION__,__LINE__);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnIPodStartReady", "()V");
	  env->CallVoidMethod(obj_IpodStart,getName_method);
	   env->DeleteLocalRef(obj_class);
}

void OnMeidaNowPlayingArtworkFinish()
{
	if(obj_NowPlayingArtwork== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingArtwork);
	 
	  jmethodID getName_method = env->GetMethodID(obj_class, "onDataFinish", "()V");
	  env->CallVoidMethod(obj_NowPlayingArtwork,getName_method);
	   env->DeleteLocalRef(obj_class);
}

void OnMediaUpdatePlaylistContentTransfer()
{
	if(obj_MediaLibraryUpdatePlayList== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdatePlayList);
	 
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaUpdatePlaylistContentTransfer", "()V");
	  env->CallVoidMethod(obj_MediaLibraryUpdatePlayList,getName_method);
	   env->DeleteLocalRef(obj_class);
}
void OnContainedMediaItemFsId(int status)
{
	if(obj_MediaLibraryUpdatePlayList== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdatePlayList);
	 
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnContainedMediaItemFsId", "(I)V");
	  env->CallVoidMethod(obj_MediaLibraryUpdatePlayList,getName_method,status);
	   env->DeleteLocalRef(obj_class);
}

void OnMediaUpdatePlaylistIsFolder(int status)
{
	if(obj_MediaLibraryUpdatePlayList== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdatePlayList);
	  jboolean b = false;
	  if(status == 1)
		b=true;
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaUpdatePlaylistIsFolder", "(Z)V");
	  env->CallVoidMethod(obj_MediaLibraryUpdatePlayList,getName_method,b);
	   env->DeleteLocalRef(obj_class);
}

void OnMediaUpdatePlaylistIsGeniusMix(int status)
{
	if(obj_MediaLibraryUpdatePlayList== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdatePlayList);
	  jboolean b = false;
	  if(status == 1)
		b=true;
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaUpdatePlaylistIsGeniusMix", "(Z)V");
	  env->CallVoidMethod(obj_MediaLibraryUpdatePlayList,getName_method,b);
	   env->DeleteLocalRef(obj_class);
}

void OnPlayBackQueueListAvail(int status)
{
	if(obj_NowPlayingPlaybackAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jboolean b = false;
	  if(status == 1)
		b=true;
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnSetElapsedTimeAvailable", "(Z)V");
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,b);
	   env->DeleteLocalRef(obj_class);
}

	
void OnSetElapsedTimeAvailable(int status)
{
	if(obj_NowPlayingPlaybackAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jboolean b = false;
	  if(status == 1)
	  	b=true;
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnSetElapsedTimeAvailable", "(Z)V");
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,b);
	   env->DeleteLocalRef(obj_class);
}



void OnMediaItemIsResidentOnDevice(int status)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
	  jboolean b = false;
	  if(status == 1)
		b=true;
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemIsResidentOnDevice", "(Z)V");
	  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,b);
	   env->DeleteLocalRef(obj_class);
}


void OnMediaItemIsBanned(int status)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
	  jboolean b = false;
	  if(status == 1)
		b=true;
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemIsBanned", "(Z)V");
	  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,b);
	   env->DeleteLocalRef(obj_class);
}

void OnMediaItemIsLiked(int status)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
	  jboolean b = false;
	  if(status == 1)
		b=true;
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemIsLiked", "(Z)V");
	  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,b);
	   env->DeleteLocalRef(obj_class);
}


void OnMediaItemIsBeanSupported(int status)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
	  jboolean b = false;
	  if(status == 1)
		b=true;
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemIsBeanSupported", "(Z)V");
	  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,b);
	   env->DeleteLocalRef(obj_class);
}

void OnMediaItemIsLikeSupported(int status)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
	  jboolean b = false;
	  if(status == 1)
		b=true;
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemIsLikeSupported", "(Z)V");
	  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,b);
	   env->DeleteLocalRef(obj_class);
}

void OnMediaItemIsPartOfCompilation(int status)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
	  jboolean b = false;
	  if(status == 1)
		b=true;
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemIsPartOfCompilation", "(Z)V");
	  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,b);
	   env->DeleteLocalRef(obj_class);
}

void OnPBAppleMusicRadioAd(int status)
{
	if(obj_NowPlayingPlaybackAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jboolean b = false;
	  if(status == 1)
	  	b=true;
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnPBAppleMusicRadioAd", "(Z)V");
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,b);
	   env->DeleteLocalRef(obj_class);
}

void OnPlayBackQueueListContentTransfer()
{

	if(obj_NowPlayingPlaybackAttributes== NULL)
		return ;
	LOGI("OnPlayBackQueueListContentTransfer \n");
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnPlayBackQueueListContentTransfer", "()V");
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method);
	   env->DeleteLocalRef(obj_class);
}

void OnPlayBackQueueListTransferID(int status)
{
	if(obj_NowPlayingPlaybackAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnPlayBackQueueListTransferID", "(I)V");
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,status);
	   env->DeleteLocalRef(obj_class);
}



void OnMediaItemArtWorkFileTransferID(int rating)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemArtWorkFileTransferID", "(I)V");
	  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,rating);
	   env->DeleteLocalRef(obj_class);
}

void OnMediaItemRating(int rating)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemRating", "(I)V");
	  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,rating);
	   env->DeleteLocalRef(obj_class);
}

void OnMediaItemAlbumDiscCount(int count)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
			return ;
		JNIEnv *env = getIPODJNIEnv();	
		  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemAlbumDiscCount", "(I)V");
		  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,count);
		   env->DeleteLocalRef(obj_class);
}

void OnMediaItemAlbumDiscNumber(int number)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
			return ;
		JNIEnv *env = getIPODJNIEnv();	
		  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemAlbumDiscNumber", "(I)V");
		  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,number);
		   env->DeleteLocalRef(obj_class);
}

void OnMediaItemChapterCount(int count)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
			return ;
		JNIEnv *env = getIPODJNIEnv();	
		  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemChapterCount", "(I)V");
		  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,count);
		   env->DeleteLocalRef(obj_class);
}

void OnMediaItemAlbumTrackCount(int count)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
			return ;
		JNIEnv *env = getIPODJNIEnv();	
		  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemAlbumTrackCount", "(I)V");
		  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,count);
		   env->DeleteLocalRef(obj_class);
}

void OnMediaItemAlbumTrackNumber(int number)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
			return ;
		JNIEnv *env = getIPODJNIEnv();	
		  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
		  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemAlbumTrackNumber", "(I)V");
		  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,number);
		   env->DeleteLocalRef(obj_class);
}

	
	
void OnMediaLibraryType(int type)
{
	if(obj_MediaLibraryInformation== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_MediaLibraryInformation);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaLibraryType", "(I)V");
	  env->CallVoidMethod(obj_MediaLibraryInformation,getName_method,type);
	   env->DeleteLocalRef(obj_class);
}

void OnMediaItemMediaType(int type)
{
	if(obj_NowPlayingMediaItemAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingMediaItemAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemMediaType", "(I)V");
	  env->CallVoidMethod(obj_NowPlayingMediaItemAttributes,getName_method,type);
	   env->DeleteLocalRef(obj_class);
}

void OnPlayBackShuffleMode(int status)
{
	if(obj_NowPlayingPlaybackAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnPlayBackShuffleMode", "(I)V");
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,status);
	   env->DeleteLocalRef(obj_class);
}

void OnPlayBackStatus(int status)
{
	if(obj_NowPlayingPlaybackAttributes== NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "onPlayBackStatus", "(I)V");
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,status);
	   env->DeleteLocalRef(obj_class);
}

void OnPlayBackQueueChapterIndex(int index)
{

	if(obj_NowPlayingPlaybackAttributes == NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "onPlayBackQueueChapterIndex", "(I)V");
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,index);
	   env->DeleteLocalRef(obj_class);
}

void OnPlayBackQueueCount(int index)
{

	if(obj_NowPlayingPlaybackAttributes == NULL)
		return ;
	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "onPlayBackQueueCount", "(I)V");
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,index);
	   env->DeleteLocalRef(obj_class);
}


void OnPlayBackQueueIndex(int index)
{

	

	if(obj_NowPlayingPlaybackAttributes == NULL)
		return ;

	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "onPlayBackQueueIndex", "(I)V");
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,index);
	   env->DeleteLocalRef(obj_class);
}

void OnPlayBackSpeed(int index)
{

	

	if(obj_NowPlayingPlaybackAttributes == NULL)
		return ;

	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnPlayBackSpeed", "(I)V");
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,index);
	   env->DeleteLocalRef(obj_class);
}



void OnPlayBackPosition(int position)
{

	if(obj_NowPlayingPlaybackAttributes == NULL)
		return ;

	JNIEnv *env = getIPODJNIEnv();	
	  jclass obj_class = env->GetObjectClass(obj_NowPlayingPlaybackAttributes);
	  jmethodID getName_method = env->GetMethodID(obj_class, "onPlayBackPosition", "(I)V");
	  env->CallVoidMethod(obj_NowPlayingPlaybackAttributes,getName_method,position);
	   env->DeleteLocalRef(obj_class);
}



void OnUSBDeviceModeAudioInformation(int sampleRate)
{
   
	LOGI("OnUSBDeviceModeAudioInformation() is invoked sampleRate=%d",sampleRate);
	if(obj_OnAudioSampleRate==NULL)
		return;
	
	JNIEnv *env = getIPODJNIEnv();	
	
	
	
	  jclass obj_class = env->GetObjectClass(obj_OnAudioSampleRate);
	  
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnAudioSampleRate", "(I)V");
       
	  env->CallVoidMethod(obj_OnAudioSampleRate,getName_method,sampleRate);
	  env->DeleteLocalRef(obj_class);
	
}





#if defined(__cplusplus)
}
#endif



