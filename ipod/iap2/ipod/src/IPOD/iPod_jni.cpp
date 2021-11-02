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
jobject obj_MediaItemListUpdatePlayListContents = NULL;
jobject obj_MediaLibraryUpdatePlayListContents = NULL;
jobject obj_NowPlayingPlaybackQueueContents = NULL;
jobject obj_MediaItemNowPlayingPlaybackQueueContents = NULL;

jobject obj_OnPcmDataReceiver = NULL;
jobject obj_OnAudioSampleRate = NULL;
//jobject obj_OnPlayBackPosition = NULL;
jobject obj_MediaLibraryUpdatePlayList = NULL;
jobject obj_MediaLibraryInformation = NULL;
jobject obj_NowPlayingArtwork = NULL;
jobject obj_IpodStart = NULL;
jobject obj_NowPlayingPlaybackAttributes = NULL;// registerPlaybackAttributesCallback 
jobject obj_NowPlayingMediaItemAttributes = NULL;
//jobject obj_OnPlayBackStatus = NULL;
jobject obj_MediaLibraryUpdate = NULL;

//jobject obj_OnMediaItemTile = NULL;
//jobject obj_OnMediaItemArtist = NULL;
jobject obj_OnMediaItemAlumTile = NULL;
jobject obj_OnMediaItemAlumArtist = NULL;
jclass gClazzMediaLibraryInformation = NULL;
jclass gClazzMediaItem = NULL;
jclass gClazzMediaPlaylist = NULL;

//jobject obj_OnDuration=NULL; 
int g_ipod_staus = 0;//0,默认，没有连接iphone，1，连接没播放，2，连接并播放
int g_ipod_plugin_staus = 0;//0,默认，没有连接iphone，1，连接没播放，2，连接并播放


sp<IIpodService>   gIpodService = NULL;

JavaVM* g_ipod_jm;



JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void *reserved) 
{
    JNIEnv* env = NULL;
	
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        ALOGE(" %s ERROR: GetEnv failed\n",__FUNCTION__);
    }else{
    //    env->DeleteGlobalRef(gClazzIap2);
    // env->DeleteLocalRef(gClazzMediaLibraryInformation);
    if(obj_OnMediaItemAlumArtist!=NULL)
	 env->DeleteGlobalRef(obj_OnMediaItemAlumArtist);
	if(obj_OnMediaItemAlumTile!=NULL)
    env->DeleteGlobalRef(obj_OnMediaItemAlumTile);
	if(obj_NowPlayingArtwork!=NULL)
 	env->DeleteGlobalRef(obj_NowPlayingArtwork);
	if(obj_NowPlayingPlaybackAttributes!=NULL)
 	env->DeleteGlobalRef(obj_NowPlayingPlaybackAttributes);
	if(obj_NowPlayingMediaItemAttributes!=NULL)
 	 env->DeleteGlobalRef(obj_NowPlayingMediaItemAttributes);
	if(obj_MediaLibraryInformation!=NULL)
 	 env->DeleteGlobalRef(obj_MediaLibraryInformation);
	if(obj_MediaLibraryUpdate!=NULL)
    env->DeleteGlobalRef(obj_MediaLibraryUpdate);
	if(obj_MediaLibraryUpdatePlayList!=NULL)
   env->DeleteGlobalRef(obj_MediaLibraryUpdatePlayList);
	if(obj_OnAudioSampleRate!=NULL)
   env->DeleteGlobalRef(obj_OnAudioSampleRate);
	if(obj_OnPcmDataReceiver!=NULL)
   env->DeleteGlobalRef(obj_OnPcmDataReceiver);
	if(gClazzMediaLibraryInformation!=NULL)
   env->DeleteGlobalRef(gClazzMediaLibraryInformation);	
   if(gClazzMediaItem!=NULL)
   env->DeleteGlobalRef(gClazzMediaItem);	
   if(gClazzMediaPlaylist != NULL)
   	env->DeleteGlobalRef(gClazzMediaPlaylist);
   if(obj_NowPlayingPlaybackQueueContents!=NULL)
   	env->DeleteGlobalRef(obj_NowPlayingPlaybackQueueContents);

   if(obj_MediaLibraryUpdatePlayListContents!=NULL)
   	env->DeleteGlobalRef(obj_MediaLibraryUpdatePlayListContents);

   if(obj_MediaItemNowPlayingPlaybackQueueContents)
	 	env->DeleteGlobalRef(obj_MediaItemNowPlayingPlaybackQueueContents);
  if(obj_MediaItemListUpdatePlayListContents)
 	env->DeleteGlobalRef(obj_MediaItemListUpdatePlayListContents);
  if(obj_IpodStart!=NULL)   
    env->DeleteGlobalRef(obj_IpodStart);
	
    }
}


JNIEnv* getIPODJNIEnv() {  
	#if 0
	 JNIEnv* env = NULL;  
	JavaVM* vm = AndroidRuntime::getJavaVM();

    JavaVMAttachArgs args;
    char name[] = "getIPODJNIEnv  Thread";
    args.version = JNI_VERSION_1_6;
    args.name = NULL;
    args.group = NULL;
//	vm->AttachCurrentThread(&env, &args);
    g_ipod_jm->AttachCurrentThread(&env, &args);
	return env;

   #else
 
   JNIEnv* env = NULL;  
   //JavaVM* vm = AndroidRuntime::getJavaVM();
 //  switch (vm->GetEnv((void**)&env, JNI_VERSION_1_6))

	switch (g_ipod_jm->GetEnv((void**)&env, JNI_VERSION_1_6))
	   {
		   case JNI_OK:
		   //	LOGI("getIPODJNIEnv JNI_OK\n");
		   //	status = g_ipod_jm->AttachCurrentThread(&env, NULL);
		   break;
		   case JNI_EDETACHED:
		   	//LOGI("getIPODJNIEnv JNI_EDETACHED\n");
			   if (g_ipod_jm->AttachCurrentThread(&env, NULL)!=0)
			   {
			   }
			   break;
		   case JNI_EVERSION:
		   //	LOGI("getIPODJNIEnv JNI_EVERSION\n");
			   break;
			   default:
			   //		LOGI("getIPODJNIEnv default\n");
			   	break;
	   }

	
    return env;  
	#endif
} 
 void  initClass(JNIEnv *env)
{
	//ALOGD("	%s",__FUNCTION__);
	jclass clsMediaLibraryInformation = env->FindClass("com/autoipod/data/MediaLibraryInformation");
  
	if(!clsMediaLibraryInformation) {
		LOGI("initClass: failed to get com.autoipod.data.MediaLibraryInformation class reference ");
		//return;
	}
	else
		{
		  gClazzMediaLibraryInformation = (jclass)env->NewGlobalRef(clsMediaLibraryInformation);
		   gClazzMediaItem = (jclass)env->NewGlobalRef(clsMediaLibraryInformation);
		}

	jclass clsMediaItem = env->FindClass("com/autoipod/data/MediaItem");
  
	if(!clsMediaItem) {
		LOGI("initClass: failed to get com.autoipod.data.MediaItem class reference ");
		//return;
	}
	else
		{
		  
		   gClazzMediaItem = (jclass)env->NewGlobalRef(clsMediaItem);
		}

	jclass clsMediaPlaylist = env->FindClass("com/autoipod/data/MediaPlaylist");
  
	if(!clsMediaPlaylist) {
		LOGI("initClass: failed to get com.autoipod.data.MediaPlaylist class reference ");
	}
	else
		{
		  
		   gClazzMediaPlaylist= (jclass)env->NewGlobalRef(clsMediaPlaylist);
		}
	
	
	
	
}


jint JNI_OnLoad(JavaVM* vm, void* reserved)
{

    g_ipod_jm = vm;

    int status;
    int isAttached = 0;
    JNIEnv* env = NULL;
    status = g_ipod_jm->GetEnv((void**) &env, JNI_VERSION_1_6);
    if (status < 0)
    {
    	status = g_ipod_jm->AttachCurrentThread(&env, NULL);
    	if (status < 0)
    	{
    		//return JNI_VERSION_1_6;
    	}
		else
    	isAttached = 1;
    }
    initClass(env);
       
    return JNI_VERSION_1_6;


}



//int gListenerCount = 0;
void* NoitfyThread(void* param) 
{

	sp<ProcessState> proc(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();
    ALOGI("ServiceManager: iapServer %p", sm.get());
    //JNITelechipsIap1Listener::instantiate();
	
	PcmServiceNotify::instantiate();
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
	return NULL;
}

/*

void get_iap1_service()
{
	 pthread_t read_thead;
			 pthread_create(&read_thead, NULL, NoitfyThread,
							  NULL);
}
*/
void get_iap1_service()
{
    if(gIpodService!=NULL)
		return ;
    bool makeNewService = false;
   // miAP1ClientLock.lock();
    if (gIpodService.get() == 0) {
        ALOGI(" get_iap1_service check %p ",gIpodService.get());
        makeNewService = true;
    }else if(/*gIpodService->asBinder()->isBinderAlive() */IInterface::asBinder(gIpodService)->isBinderAlive()== false){
        makeNewService= true;
        ALOGI(" get_iap1_service isBinderAlive is false");
    }
    
    if (makeNewService) {
        sp<IServiceManager> sm = defaultServiceManager();
        sp<IBinder> binder;
        do {
            binder = sm->getService(String16("IpodService"));
            if (binder != 0)
            	{
                break;
            	}
            ALOGE("IpodService not  published, waiting... ");
            usleep(500000); // 0.5 s
        } while(true);
		

       // sp<JNITelechipsIap1Listener> listenerApi= new JNITelechipsIap1Listener();
       // status_t ret = binder->linkToDeath(listenerApi);
		 sp<PcmServiceNotify> listenerPcm= new PcmServiceNotify();
          binder->linkToDeath(listenerPcm);
		   sp<PcmServiceNotify> listenerFilePcm= new PcmServiceNotify();
          binder->linkToDeath(listenerFilePcm);
        sp<IIpodService> service = interface_cast<IIpodService>(binder);
		gIpodService = service;
		char  cListenerName[13];
		
		//sprintf(cListenerName,"jniListener%d",1);
      //  service->setListener(listenerApi,String8(cListenerName));
		char*  pcmListenerName="PcmListener";
		char*  pcmFileListenerName="PcmFileListener";
		service->setPcmListener(listenerPcm,String8(pcmListenerName));
		service->setPcmFileListener(listenerFilePcm,String8(pcmFileListenerName));

		
        
       // ALOGI("%s new service ret %d %s",__FUNCTION__,ret,miAP1ListenerName.string());
    }
   // miAP1ClientLock.unlock();

    return ;
}


JNIEXPORT
void
JNICALL
							  
Java_com_autoipod_jni_Ipod_registerMediaItemListUpdatePlaylistContentsCallback(JNIEnv* env,
												   jobject thiz,jobject obj_register){
													
												   obj_MediaItemListUpdatePlayListContents = env->NewGlobalRef(obj_register);
														
												  } 


JNIEXPORT
void
JNICALL
							  
Java_com_autoipod_jni_Ipod_registerMediaLibraryUpdatePlaylistContentsCallback(JNIEnv* env,
												   jobject thiz,jobject obj_register){
													
												   obj_MediaLibraryUpdatePlayListContents = env->NewGlobalRef(obj_register);
														
												  } 


	
JNIEXPORT
void
JNICALL
							  
Java_com_autoipod_jni_Ipod_registerMediaItemNowPlayingPlaybackQueueContentsCallback(JNIEnv* env,
												   jobject thiz,jobject obj_register){
													
												   obj_MediaItemNowPlayingPlaybackQueueContents = env->NewGlobalRef(obj_register);
														
												  } 

JNIEXPORT
void
JNICALL
							  
Java_com_autoipod_jni_Ipod_registerNowPlayingPlaybackQueueContentsCallback(JNIEnv* env,
												   jobject thiz,jobject obj_register){
													
												   obj_NowPlayingPlaybackQueueContents = env->NewGlobalRef(obj_register);
														
												  } 


JNIEXPORT
void
JNICALL
	                          
Java_com_autoipod_jni_Ipod_reigsterOnPcmDataReceiveCallBack(JNIEnv* env,
												   jobject thiz,jobject obj_register){
												    
												   obj_OnPcmDataReceiver = env->NewGlobalRef(obj_register);
												 		
												  }	

JNIEXPORT
void
JNICALL
	                          
Java_com_autoipod_jni_Ipod_unReigsterOnPcmDataReceiveCallBack(JNIEnv* env,
												   jobject thiz){
												   if(obj_OnPcmDataReceiver!=NULL)
												    env->DeleteGlobalRef(obj_OnPcmDataReceiver);
												   obj_OnPcmDataReceiver = NULL;
												//   obj_OnPcmDataReceiver = env->NewGlobalRef(obj_register);
												 		
												  }	



JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_registerAudioInformationCallBack(JNIEnv* env,
												   jobject thiz,jobject obj_register){
												    
												   obj_OnAudioSampleRate = env->NewGlobalRef(obj_register);
												 //   env->DeleteGlobalRef(obj_OnAudioSampleRate);
												  
												 		
												  }	

JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_unRegisterAudioInformationCallBack(JNIEnv* env,
												   jobject thiz){
												     if(obj_OnAudioSampleRate!=NULL)
												    env->DeleteGlobalRef(obj_OnAudioSampleRate);
												   obj_OnAudioSampleRate = NULL;
												  // obj_OnAudioSampleRate = env->NewGlobalRef(obj_register);
												 		
												  }	


JNIEXPORT
jint
JNICALL
Java_com_autoipod_jni_Ipod_getIpodPluginStatus(JNIEnv* env,
												   jobject thiz){
												   return g_ipod_plugin_staus;												   
													//stopHid(g_sessionId);												 		
												  }	


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_setIpodPluginStatus(JNIEnv* env,
												   jobject thiz,jint status){
												   g_ipod_plugin_staus = status;
												  
												 		
												  }	


JNIEXPORT
jint
JNICALL
Java_com_autoipod_jni_Ipod_getIpodStatus(JNIEnv* env,
												   jobject thiz){
												   return g_ipod_staus;												   
													//stopHid(g_sessionId);												 		
												  }	


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_setIpodStatus(JNIEnv* env,
												   jobject thiz,jint status){
												   g_ipod_staus = status;
												  
												 		

}





JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_iAP2SetPQListContentTransferStartIndex(JNIEnv* env,
												   jobject thiz,jint index){
												 //  LOGI("%s  position =%d\n",__FUNCTION__,position);
												  // g_ipod_staus = status;
												   if(gIpodService!=NULL)
												 	gIpodService->setPQListContentTransferStartIndex(index);	
												  }	

JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_iAP2SetPlaybackQueueIndex(JNIEnv* env,
												   jobject thiz,jint index){
												 //  LOGI("%s  position =%d\n",__FUNCTION__,position);
												  // g_ipod_staus = status;
												   if(gIpodService!=NULL)
												 	gIpodService->setPlaybackQueueIndex(index);	
												  }	


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_setIpodPlayPosition(JNIEnv* env,
												   jobject thiz,jint position){
												 //  LOGI("%s  position =%d\n",__FUNCTION__,position);
												  // g_ipod_staus = status;
												   if(gIpodService!=NULL)
												 	gIpodService->setPlayPosition(position);	
												  }	



JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_stopHid(JNIEnv* env,
												   jobject thiz){
												    gIpodService->stopHid();
													//stopHid(g_sessionId);
												 		
												  }	




JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_stopUSBDeviceModeAudio(JNIEnv* env,
												   jobject thiz){
												  //  LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
													if(gIpodService!=NULL)
												    gIpodService->stopUSBDeviceModeAudio();//
												    //stopUSBDeviceModeAudio(g_sessionId);
												 		
												  }	
JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_playPause(JNIEnv* env,
												   jobject thiz,jint mod){
												   // LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
													if(mod == 0)//0 to stop
												  g_ipod_staus = 0;

														
												 if(gIpodService!=NULL)
												gIpodService->playPause(mod);
												  }	
JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_notifyMicRecord(JNIEnv* env,
												   jobject thiz,jint mod){
												    LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
													 if(gIpodService!=NULL)
												gIpodService->notifyMicRecord(mod);
												//if(mod == 0)//0 to stop
												//  g_ipod_staus = 0;
												
												   LOGI("%s  mod =%d\n",__FUNCTION__,mod);

														
												  }	
JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_random(JNIEnv* env,
												   jobject thiz){
												
											
												 if(gIpodService!=NULL)
												  gIpodService->random();

														
												  }	




JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_repeat(JNIEnv* env,
												   jobject thiz){
												
											
												 if(gIpodService!=NULL)
												  gIpodService->repeat();

														
												  }	


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_play(JNIEnv* env,
												   jobject thiz){
												
												g_ipod_staus = 2;
												 if(gIpodService!=NULL)
												  gIpodService->play();

														
												  }	

JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_send0Hid(JNIEnv* env,
												   jobject thiz){
												
											
												 if(gIpodService!=NULL)
												  gIpodService->send0Hid();

														
												  }	





JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_pause(JNIEnv* env,
												   jobject thiz){
												  // gIpodService->setPlayPosition(120);
												  g_ipod_staus = 1;
												   if(gIpodService!=NULL)
												gIpodService->pause();
												

														
												  }	


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_playNextTrack(JNIEnv* env,
												   jobject thiz){
												   g_ipod_staus = 2;
												    if(gIpodService!=NULL)
												gIpodService->playNextTrack();
												 		
												  }	
JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_playProvTrack(JNIEnv* env,
												   jobject thiz){
												   g_ipod_staus = 2;
												    if(gIpodService!=NULL)
												gIpodService->playProvTrack();
												 		
												  }	



JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_iAP2StopMediaLibraryInformation(JNIEnv* env,
												   jobject thiz){
												    if(gIpodService!=NULL)
												   gIpodService->StopMediaLibraryInformation();
												//iAP2StartNowPlayingUpdates(g_sessionId);
												 		
												  }	


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_iAP2StartMediaLibraryInformation(JNIEnv* env,
												   jobject thiz){
												LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
												    if(gIpodService!=NULL)

												    	{
												    	
														LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
												   gIpodService->StartMediaLibraryInformation();
												    	}
												//iAP2StartNowPlayingUpdates(g_sessionId);
												 		
												  }	


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_iAP2StartNowPlayingUpdates(JNIEnv* env,
												   jobject thiz){
												   
												   LOGI("%s :%s line=%d\n",__FILE__,__FUNCTION__,__LINE__);
												    if(gIpodService!=NULL)
												    	{
												    	
														LOGI("%s :%s line=%d\n",__FILE__,__FUNCTION__,__LINE__);
												   gIpodService->StartNowPlayingUpdates();
												    	}
												//iAP2StartNowPlayingUpdates(g_sessionId);
												 		
												  }	



JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_iAP2PlayMediaLibraryItems(JNIEnv* env,
												   jobject thiz,jint itemBytesSize,jbyteArray itemsPids,jint itemStartIndex,
												   jint mediaLibraryUidSize,jbyteArray mediaLibraryUid){
												   
											
	
	   int i;
											
	    if(gIpodService!=NULL)
	    {
	    	uint8_t * bufItemsPids = NULL;
			uint8_t * bufMediaLibrary = NULL;
			jbyte *bytesItemsPids;
			jbyte *bytesMediaLibrary;
			
	   //LOGI("jni_Ipod_iAP2StartMediaLibraryUpdates uid_size=%d\n",uid_size);
			
			  
			   bytesItemsPids = env->GetByteArrayElements(itemsPids,NULL);
			   bufItemsPids =(uint8_t *)malloc(itemBytesSize);				
				memset(bufItemsPids,0,itemBytesSize);
				//LOGI("itemBytesSize = %d\n",itemBytesSize);
				for(i=0;i<itemBytesSize;i++)
				{
					*(bufItemsPids+i)=( uint8_t)(*(bytesItemsPids+i));
					//LOGI("bytesItemsPids+%d =0x%02x\n",i,( uint8_t)(*(bytesItemsPids+i)));
					//LOGI("bufItemsPids+%d =0x%02x\n",i,( uint8_t)(*(bufItemsPids)));
				}
			  
			   bytesMediaLibrary = env->GetByteArrayElements(mediaLibraryUid,NULL);
			   bufMediaLibrary =(uint8_t *)malloc(mediaLibraryUidSize);				
				memset(bufMediaLibrary,0,mediaLibraryUidSize);
					//LOGI("mediaLibraryUidSize = %d\n",mediaLibraryUidSize);
				for(i=0;i<mediaLibraryUidSize;i++)
				{
					*(bufMediaLibrary+i)=( uint8_t)(*(bytesMediaLibrary+i));
					//	LOGI("bytesMediaLibrary+%d =0x%02x\n",i,( uint8_t)(*(bytesMediaLibrary+i)));
					//LOGI("*(bufMediaLibrary+%d) =0x%02x\n",i,( uint8_t)(*(bufMediaLibrary+i)));
				}
		   
			 //LOGI("jni_Ipod_iAP2PlayMediaLibraryItems 222 uid_size = %d\n",uid_size);
		 
		   gIpodService->PlayMediaLibraryItems(bufItemsPids,itemBytesSize,itemStartIndex,
	 	   bufMediaLibrary,mediaLibraryUidSize);

		 
		   	env->ReleaseByteArrayElements(mediaLibraryUid,bytesMediaLibrary,0);

		    free(bufMediaLibrary);
		  
		   	env->ReleaseByteArrayElements(itemsPids,bytesItemsPids,0);

		    free(bufItemsPids);
		   
	    }
												
												 		
 }	

JNIEXPORT
void
JNICALL
	Java_com_autoipod_jni_Ipod_iAP2PlayMediaLibrarySpecial(JNIEnv* env,jobject thiz,jint mediaLibraryUidSize,jbyteArray mediaLibraryUid,
						jboolean isPlayAll,jint startMediaItemPidSize,jbyteArray startMediaItemPid)
{
        if(gIpodService!=NULL)
		{
			unsigned char * bufMediaLibraryUid = NULL;
			unsigned char * bufMediaItemPid = NULL;
			jbyte *bytesMediaLibraryUid;
			jbyte *bytesMediaItemPid;
			int i;		 
			
			if(mediaLibraryUidSize > 0)
			{			  
			   bytesMediaLibraryUid = env->GetByteArrayElements(mediaLibraryUid,NULL);
			   bufMediaLibraryUid =(unsigned char *)malloc(mediaLibraryUidSize);				
				memset(bufMediaLibraryUid,0,mediaLibraryUidSize);
				for(i=0;i<mediaLibraryUidSize;i++)
				{
					*(bufMediaLibraryUid+i)=( uint8_t)(*(bytesMediaLibraryUid+i));
				}
			}
			if(startMediaItemPidSize > 0)
			{
			  
			   bytesMediaItemPid = env->GetByteArrayElements(startMediaItemPid,NULL);
			   bufMediaItemPid =(unsigned char *)malloc(startMediaItemPidSize);				
				memset(bufMediaItemPid,0,startMediaItemPidSize);
				for(i=0;i<startMediaItemPidSize;i++)
				{
					*(bufMediaItemPid+i)=( uint8_t)(*(bytesMediaItemPid+i));
				}
			}
			
			 gIpodService->PlayMediaLibrarySpecial(mediaLibraryUidSize,bufMediaLibraryUid,
			 									isPlayAll,startMediaItemPidSize,bufMediaItemPid);

			 if(startMediaItemPidSize>0)
			 {
			 	free(bufMediaItemPid);			 
			    env->ReleaseByteArrayElements(startMediaItemPid,bytesMediaItemPid,0);
			 }
			 if(mediaLibraryUidSize>0)
			 {
			 	free(bufMediaLibraryUid);			 
			    env->ReleaseByteArrayElements(mediaLibraryUid,bytesMediaLibraryUid,0);
			 }
		
		
			
		   
		}
}

JNIEXPORT
void
JNICALL
	Java_com_autoipod_jni_Ipod_iAP2PlayMediaLibraryCollection(JNIEnv* env,jobject thiz,jint pid_size,jbyteArray pid,
						jint type,jint index,jint mediaLibraryUidSize,jbyteArray mediaLibraryUid,
						jint startMediaItemPidSize,jbyteArray startMediaItemPid)
	{
        if(gIpodService!=NULL)
		{
			unsigned char * bufPid = NULL;
			unsigned char * bufMediaLibraryUid = NULL;
			unsigned char * bufMediaItemPid = NULL;
			jbyte *bytesPid;
			jbyte *bytesMediaLibraryUid;
			jbyte *bytesMediaItemPid;
			int i;		 
			if(pid_size > 0)
			{			  
			   bytesPid = env->GetByteArrayElements(pid,NULL);
			   bufPid =(unsigned char *)malloc(pid_size);				
				memset(bufPid,0,pid_size);
				for(i=0;i<pid_size;i++)
				{
					*(bufPid+i)=( uint8_t)(*(bytesPid+i));
				}
			}
			if(mediaLibraryUidSize > 0)
			{			  
			   bytesMediaLibraryUid = env->GetByteArrayElements(mediaLibraryUid,NULL);
			   bufMediaLibraryUid =(unsigned char *)malloc(mediaLibraryUidSize);				
				memset(bufMediaLibraryUid,0,mediaLibraryUidSize);
				for(i=0;i<mediaLibraryUidSize;i++)
				{
					*(bufMediaLibraryUid+i)=( uint8_t)(*(bytesMediaLibraryUid+i));
				}
			}
			if(startMediaItemPidSize > 0)
			{
			  
			   bytesMediaItemPid = env->GetByteArrayElements(startMediaItemPid,NULL);
			   bufMediaItemPid =(unsigned char *)malloc(startMediaItemPidSize);				
				memset(bufMediaItemPid,0,startMediaItemPidSize);
				for(i=0;i<startMediaItemPidSize;i++)
				{
					*(bufMediaItemPid+i)=( uint8_t)(*(bytesMediaItemPid+i));
				}
			}
			 gIpodService->PlayMediaLibraryCollection(pid_size,bufPid,type,index,
			 	mediaLibraryUidSize,bufMediaLibraryUid,startMediaItemPidSize,bufMediaItemPid);

			 if(startMediaItemPidSize>0)
			 {
			 	free(bufMediaItemPid);			 
			    env->ReleaseByteArrayElements(startMediaItemPid,bytesMediaItemPid,0);
			 }
			 if(mediaLibraryUidSize>0)
			 {
			 	free(bufMediaLibraryUid);			 
			    env->ReleaseByteArrayElements(mediaLibraryUid,bytesMediaLibraryUid,0);
			 }
			  if(pid_size>0)
			 {
			 	free(bufPid);			 
			    env->ReleaseByteArrayElements(pid,bytesPid,0);
			 }
		
		
			
		   
		}
}
                                                        


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_iAP2PlayMediaLibraryCurrentSelection(JNIEnv* env,
												   jobject thiz,jint uid_size,jbyteArray uid_data_array){
												   
											
	
	   int i;
											
		if(gIpodService!=NULL)
		{
			unsigned char * bufUid = NULL;
			unsigned char * bufRevision = NULL;
			jbyte *bytesUid;
			jbyte *bytesRevision;
			
	   LOGI("jni_Ipod_iAP2StartMediaLibraryUpdates uid_size=%d\n",uid_size);
			if(uid_size > 0)
			{
			  
			   bytesUid = env->GetByteArrayElements(uid_data_array,NULL);
			   bufUid =(unsigned char *)malloc(uid_size);				
				memset(bufUid,0,uid_size);
				for(i=0;i<uid_size;i++)
				{
					*(bufUid+i)=( uint8_t)(*(bytesUid+i));
				}
			}
		
			 LOGI("jni_Ipod_iAP2StartMediaLibraryUpdates 222 uid_size=%d\n",uid_size);
		   gIpodService->PlayMediaLibraryCurrentSelection(bufUid,uid_size);

		   if(uid_size > 0)
			{
			env->ReleaseByteArrayElements(uid_data_array,bytesUid,0);

			free(bufUid);
			}
		   
		}
												
														
 }	


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_iAP2StopMediaLibraryUpdates(JNIEnv* env,
												   jobject thiz,jint uid_size,jbyteArray uid_data_array){
												   
											
	
	   int i;
											
	    if(gIpodService!=NULL)
	    {
	    	unsigned char * bufUid = NULL;
			unsigned char * bufRevision = NULL;
			jbyte *bytesUid;
			jbyte *bytesRevision;
			
	   LOGI("jni_Ipod_iAP2StartMediaLibraryUpdates uid_size=%d\n",uid_size);
			if(uid_size > 0)
			{
			  
			   bytesUid = env->GetByteArrayElements(uid_data_array,NULL);
			   bufUid =(unsigned char *)malloc(uid_size);				
				memset(bufUid,0,uid_size);
				for(i=0;i<uid_size;i++)
				{
					*(bufUid+i)=( uint8_t)(*(bytesUid+i));
				}
		    }
		
			 LOGI("jni_Ipod_iAP2StartMediaLibraryUpdates 222 uid_size=%d\n",uid_size);
		   gIpodService->StopMediaLibraryUpdates(bufUid,uid_size);

		   if(uid_size > 0)
		   	{
		   	env->ReleaseByteArrayElements(uid_data_array,bytesUid,0);

		    free(bufUid);
		   	}
		   
	    }
												
												 		
 }	


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_iAP2StartMediaLibraryUpdates(JNIEnv* env,
												   jobject thiz,jint uid_size,jbyteArray uid_data_array,jint revision_size,jbyteArray revision_data_array){
												   
											
	
	   int i;
											
	    if(gIpodService!=NULL)
	    {
	    	unsigned char * bufUid = NULL;
			unsigned char * bufRevision = NULL;
			jbyte *bytesUid;
			jbyte *bytesRevision;
			
	   LOGI("jni_Ipod_iAP2StartMediaLibraryUpdates uid_size=%d\n",uid_size);
			if(uid_size > 0)
			{
			  
			   bytesUid = env->GetByteArrayElements(uid_data_array,NULL);
			   bufUid =(unsigned char *)malloc(uid_size);				
				memset(bufUid,0,uid_size);
				for(i=0;i<uid_size;i++)
				{
					*(bufUid+i)=( uint8_t)(*(bytesUid+i));
				}
		    }
			if(revision_size > 0)
			{
			  
			   bytesRevision = env->GetByteArrayElements(revision_data_array,NULL);
			   bufRevision =(unsigned char *)malloc(revision_size);				
				memset(bufRevision,0,revision_size);
				for(i=0;i<revision_size;i++)
				{
					*(bufRevision+i)=( uint8_t)(*(bytesRevision+i));
				}
		    }
			 LOGI("jni_Ipod_iAP2StartMediaLibraryUpdates 222 uid_size=%d\n",uid_size);
		   gIpodService->StartMediaLibraryUpdates(bufUid,uid_size,bufRevision,revision_size);

		   if(uid_size > 0)
		   	{
		   	env->ReleaseByteArrayElements(uid_data_array,bytesUid,0);

		    free(bufUid);
		   	}
		     if(revision_size > 0)
		   	{
		   	env->ReleaseByteArrayElements(revision_data_array,bytesRevision,0);

		    free(bufRevision);
		   	}
	    }
												
												 		
 }	







JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_iAP2StopNowPlayingUpdates(JNIEnv* env,
												   jobject thiz){
												   gIpodService->StopNowPlayingUpdates();
												//iAP2StopNowPlayingUpdates(g_sessionId);
												 		
												  }	
	
JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_registerMediaLibraryUpdatePlayListCallback(JNIEnv* env,
												   jobject thiz,jobject obj_register){
												   obj_MediaLibraryUpdatePlayList = env->NewGlobalRef(obj_register);
												   // env->DeleteGlobalRef(obj_MediaLibraryUpdatePlayList);
													 
												  } 

JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_registerMediaLibraryUpdateCallback(JNIEnv* env,
												   jobject thiz,jobject obj_register){
												   obj_MediaLibraryUpdate = env->NewGlobalRef(obj_register);
												   //env->DeleteGlobalRef(obj_MediaLibraryUpdate);
												  
														
												  } 

JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_registerMediaLibraryInformationCallback(JNIEnv* env,
												   jobject thiz,jobject obj_register){
												   obj_MediaLibraryInformation = env->NewGlobalRef(obj_register);
												  // env->DeleteGlobalRef(obj_MediaLibraryInformation);
												  
														
												  } 

JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_registerNowPlayingPlaybackAttributesCallback(JNIEnv* env,
												   jobject thiz,jobject obj_register){
												   obj_NowPlayingPlaybackAttributes = env->NewGlobalRef(obj_register);
												  // env->DeleteGlobalRef(obj_NowPlayingPlaybackAttributes);
												  
												  }	
JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_registerNowPlayingMediaItemAttributesCallback(JNIEnv* env,
												   jobject thiz,jobject obj_register){
												   obj_NowPlayingMediaItemAttributes = env->NewGlobalRef(obj_register);
												   // env->DeleteGlobalRef(obj_NowPlayingMediaItemAttributes);
													 	
												  }	

JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_registerMediaNowPlayingArtworkDataCallback(JNIEnv* env,
												   jobject thiz,jobject obj_register){
												   obj_NowPlayingArtwork  = env->NewGlobalRef(obj_register);
												  // env->DeleteGlobalRef(obj_NowPlayingArtwork);
												   	
												  }	


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_registerIpodStartCallback(JNIEnv* env,
												   jobject thiz,jobject obj_register){
												   obj_IpodStart  = env->NewGlobalRef(obj_register);
												  // env->DeleteGlobalRef(obj_NowPlayingArtwork);
												   	
												  }	












JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_reigsterMeidaItemAlumTileCallBack(JNIEnv* env,
												   jobject thiz,jobject obj_register){
												   obj_OnMediaItemAlumTile = env->NewGlobalRef(obj_register);
												  // env->DeleteGlobalRef(obj_OnMediaItemAlumTile);
												  		
												  }	


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_unReigsterMeidaItemAlumTileCallBack(JNIEnv* env,
												   jobject thiz){

												     if(obj_OnMediaItemAlumTile!=NULL)
												    env->DeleteGlobalRef(obj_OnMediaItemAlumTile);
												    obj_OnMediaItemAlumTile = NULL;
													
												//   obj_OnMediaItemAlumTile = env->NewGlobalRef(obj_register);
												 		
												  }	





JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_reigsterOnMediaItemAlumArtistCallBack(JNIEnv* env,
												   jobject thiz,jobject obj_register){
												   obj_OnMediaItemAlumArtist = env->NewGlobalRef(obj_register);
												   // env->DeleteGlobalRef(obj_OnMediaItemAlumArtist);
													
												 		
												  }	


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_unReigsterOnMediaItemAlumArtistCallBack(JNIEnv* env,
												   jobject thiz){

												      if(obj_OnMediaItemAlumArtist!=NULL)
												    env->DeleteGlobalRef(obj_OnMediaItemAlumArtist);
												    obj_OnMediaItemAlumArtist = NULL;
													
												  // obj_OnMediaItemAlumArtist = env->NewGlobalRef(obj_register);
												 		
												  }	


JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_stopIpod(JNIEnv* env,
												   jobject thiz){
												   g_ipod_staus = 1;
												   get_iap1_service();
													 // sleep(5);
												   gIpodService->stopIpod();
													  
	
}




JNIEXPORT
void
JNICALL
Java_com_autoipod_jni_Ipod_startIpod(JNIEnv* env,
												   jobject thiz){
												   g_ipod_staus = 1;
												   get_iap1_service();
													 // sleep(5);
												   gIpodService->startIpod();
													  
	
}




#if defined(__cplusplus)
}
#endif



