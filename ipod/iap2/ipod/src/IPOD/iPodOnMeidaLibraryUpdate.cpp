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

void OnMediaLibraryUpdateUID(uint8_t  *data, u32 len){
	 if(obj_MediaLibraryUpdate==NULL)
	return;


	
	  uint8_t byteArray[len];	
	 LOGI("OnMediaLibraryUpdateUID() is invoked");
	JNIEnv *env = getIPODJNIEnv();
	  jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdate);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaLibraryUpdateUID", "(ILjava/lang/String;[B)V");
	  jbyteArray  jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放

	  	memcpy(byteArray,data,len);
	   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	   env->SetByteArrayRegion(jbarray, 0, len, jy);	
	   
	  jstring jUid =  (jstring)env->NewStringUTF((char *) data);
				 
	  env->CallVoidMethod(obj_MediaLibraryUpdate,getName_method,len,jUid,jbarray);
	    env->DeleteLocalRef(jbarray);
	   env->DeleteLocalRef(jUid);
	   env->DeleteLocalRef(obj_class);
	   
}




void OnMediaLibraryUpdateRevision(uint8_t  *data, u32 len, uint8_t  *dataUid,u32 uidLen){
	 if(obj_MediaLibraryUpdate==NULL)
	return;
	
	
	 LOGI("OnMediaLibraryUpdateRevision()  is invoked len=%d uidLen=%d\n",len,uidLen);
	JNIEnv *env = getIPODJNIEnv();
	//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	  jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdate);
	  jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaLibraryUpdateRevision", "(ILjava/lang/String;ILjava/lang/String;[B)V");

	 jbyteArray  jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放
	 uint8_t byteArray[len];	

	  	memcpy(byteArray,data,len);
	   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
	   env->SetByteArrayRegion(jbarray, 0, len, jy);	

	   
	//LOGI("OnMediaLibraryUpdateRevision 444\n");


	  // LOGI("OnMediaLibraryUpdateRevision 777\n");
	  jstring jRevision =  (jstring)env->NewStringUTF((char *) data);
	    jstring jMeidaLibraryUid =  (jstring)env->NewStringUTF((char *) dataUid);
	  env->CallVoidMethod(obj_MediaLibraryUpdate,getName_method,len,jRevision,uidLen,jMeidaLibraryUid,jbarray);
	  //  LOGI("OnMediaLibraryUpdateRevision 888\n");
	 // LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	 
	   env->DeleteLocalRef(jbarray);
	   env->DeleteLocalRef(jRevision);
	   // LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	   env->DeleteLocalRef(jMeidaLibraryUid);
	   // LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	   env->DeleteLocalRef(obj_class);
	 //   LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	   
}

void OnMediaPlayListDeleteUpdatePID(uint8_t *data, u32 len, uint8_t  *dataUid, u32 uidLen){
	  if(obj_MediaLibraryUpdate==NULL)
	   return;
	 
	   uint8_t byteArray[len];	 
	   JNIEnv *env = getIPODJNIEnv();
	   jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdate);
	   jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaPlayListDeleteUpdatePID", "(I[BILjava/lang/String;)V");
	   jbyteArray  jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放
 
	   memcpy(byteArray,data,len);
	  jbyte *jy=(jbyte*)byteArray;	//BYTE强制转换成Jbyte；
	  env->SetByteArrayRegion(jbarray, 0, len, jy);   

	 
	 jstring jMeidaLibraryUid =  (jstring)env->NewStringUTF((char *) dataUid);
	   env->CallVoidMethod(obj_MediaLibraryUpdate,getName_method,len,jbarray,uidLen,jMeidaLibraryUid);
	   env->DeleteLocalRef(jbarray);
	    env->DeleteLocalRef(jMeidaLibraryUid);
	   env->DeleteLocalRef(obj_class);
 }
 
 
 void OnMediaItemDeleteUpdatePID(uint8_t *data, u32 len, uint8_t  *dataUid, u32 uidLen){
	  if(obj_MediaLibraryUpdate==NULL)
	   return;
	 
	   uint8_t byteArray[len];	 
	   JNIEnv *env = getIPODJNIEnv();
	   jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdate);
	   jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaItemDeleteUpdatePID", "(I[BILjava/lang/String;)V");
	   jbyteArray  jbarray = env->NewByteArray(len);//建立jbarray数组//在heap里面申请， 不需要释放
 
	   memcpy(byteArray,data,len);
	  jbyte *jy=(jbyte*)byteArray;	//BYTE强制转换成Jbyte；
	  env->SetByteArrayRegion(jbarray, 0, len, jy);  
	  
	  
	   jstring jMeidaLibraryUid =  (jstring)env->NewStringUTF((char *) dataUid);
	   env->CallVoidMethod(obj_MediaLibraryUpdate,getName_method,len,jbarray,uidLen,jMeidaLibraryUid);
	   env->DeleteLocalRef(jbarray);
	    env->DeleteLocalRef(jMeidaLibraryUid);
	   env->DeleteLocalRef(obj_class);
 }


 
 void OnMediaLibraryReset(uint8_t  *dataUid, u32 uidLen){
 	LOGI("OnMediaLibraryReset invoke\n");
	 if(obj_MediaLibraryUpdate== NULL)
		 return ;
	 JNIEnv *env = getIPODJNIEnv();  
	   jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdate);
     
	    jstring jMeidaLibraryUid =  (jstring)env->NewStringUTF((char *) dataUid);
	   jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaLibraryReset", "(ILjava/lang/String;)V");
	   env->CallVoidMethod(obj_MediaLibraryUpdate,getName_method,uidLen,jMeidaLibraryUid);
	    env->DeleteLocalRef(jMeidaLibraryUid);
		env->DeleteLocalRef(obj_class);
 }
 void OnMediaLibraryUpdateProgress(int progress,uint8_t  *dataUid, u32 uidLen)
 {
	 if(obj_MediaLibraryUpdate== NULL)
		 return ;
	 JNIEnv *env = getIPODJNIEnv();  
	   jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdate);
 
	    jstring jMeidaLibraryUid =  (jstring)env->NewStringUTF((char *) dataUid);
		    
	   uint8_t byteArray[uidLen];	  
	   
	   jbyteArray  jbarray = env->NewByteArray(uidLen);//建立jbarray数组//在heap里面申请， 不需要释放
  	  memcpy(byteArray,dataUid,uidLen);
  	 jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
  	 env->SetByteArrayRegion(jbarray, 0, uidLen, jy);	
	   jmethodID getName_method = env->GetMethodID(obj_class, "OnMediaLibraryUpdateProgress", "(IILjava/lang/String;[B)V");
	   env->CallVoidMethod(obj_MediaLibraryUpdate,getName_method,progress,uidLen,jMeidaLibraryUid,jbarray);
	   env->DeleteLocalRef(jMeidaLibraryUid);
	     env->DeleteLocalRef(jbarray);
		env->DeleteLocalRef(obj_class);
 }
 
 void OnIsHidingRemoteItems(int status,uint8_t  *dataUid, u32 uidLen)
 {
	 if(obj_MediaLibraryUpdate== NULL)
		 return ;
	 JNIEnv *env = getIPODJNIEnv(); 
	 jstring jMeidaLibraryUid =  (jstring)env->NewStringUTF((char *) dataUid);
	   jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdate);
	   jboolean b = false;
	   if(status == 1)
		 b=true;
	    
	   uint8_t byteArray[uidLen];	  
	   
	   jbyteArray  jbarray = env->NewByteArray(uidLen);//建立jbarray数组//在heap里面申请， 不需要释放
  	  memcpy(byteArray,dataUid,uidLen);
  	 jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
  	 env->SetByteArrayRegion(jbarray, 0, uidLen, jy);	  
	   jmethodID getName_method = env->GetMethodID(obj_class, "OnIsHidingRemoteItems", "(ZILjava/lang/String;[B)V");
	   env->CallVoidMethod(obj_MediaLibraryUpdate,getName_method,b,uidLen,jMeidaLibraryUid,jbarray);
	   env->DeleteLocalRef(jMeidaLibraryUid);
	    env->DeleteLocalRef(jbarray); 
		env->DeleteLocalRef(obj_class);
 }
 
 void OnPlayAllSongsCapable(int status,uint8_t  *dataUid, u32 uidLen)
 {
	 if(obj_MediaLibraryUpdate== NULL)
		 return ;
	  JNIEnv *env = getIPODJNIEnv();
	 jstring jMeidaLibraryUid =  (jstring)env->NewStringUTF((char *) dataUid);
	  
	   jclass obj_class = env->GetObjectClass(obj_MediaLibraryUpdate);
	   jboolean b = false;
	   if(status == 1)
		 b=true;

	   
	   uint8_t byteArray[uidLen];	  
	   
	   jbyteArray  jbarray = env->NewByteArray(uidLen);//建立jbarray数组//在heap里面申请， 不需要释放
  	  memcpy(byteArray,dataUid,uidLen);
  	 jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
  	 env->SetByteArrayRegion(jbarray, 0, uidLen, jy);	  

	   jmethodID getName_method = env->GetMethodID(obj_class, "OnPlayAllSongsCapable", "(ZILjava/lang/String;[B)V");
	   env->CallVoidMethod(obj_MediaLibraryUpdate,getName_method,b,uidLen,jMeidaLibraryUid,jbarray);
	    env->DeleteLocalRef(jbarray);
	   env->DeleteLocalRef(jMeidaLibraryUid);
		env->DeleteLocalRef(obj_class);
 }

void MediaLibraryUpdateMediaItem(uint8_t  *data, u32 dataLen,uint8_t  *uidData, u32 uidDataLen)
{
	//LOGI("%s line:%d \n",__FUNCTION__,__LINE__);

    if(obj_MediaLibraryUpdate == NULL)
		return;
	//LOGI("%s line:%d \n",__FUNCTION__,__LINE__);
   int i = 4;
   int param_len = 0;
   int id;
   int  duration;
   int value;
    uint8_t data_array[8];
	jmethodID getName_method;
	 JNIEnv *env = getIPODJNIEnv();	
	  // 	LOGI("%s line:%d id=%d\n",__FUNCTION__,__LINE__,id);
	jmethodID mediaItem_costruct = env->GetMethodID(gClazzMediaItem,"<init>","()V");
	jobject item_obj =env->NewObject(gClazzMediaItem,mediaItem_costruct);
	 jclass obj_class = env->GetObjectClass(item_obj);
	 //	LOGI("%s line:%d id=%d\n",__FUNCTION__,__LINE__,id);
  while(i<dataLen)
  {
    param_len =    ( data[ i ] << 8 ) | data[ i+1 ];
    id =  ( data[ i+2 ] << 8 ) | data[ i+3 ];
	uint8_t *s_char;
	//LOGI("%s line:%d id=%d\n",__FUNCTION__,__LINE__,id);
	   
	switch(id)
	{

	  case 0: //MediaItemPersistentIdentifier
	    {
	        int k;
			for(k=0;k<(param_len-4);k++)
			{
				data_array[k] = data[i+4+k];
			}
		//	LOGI("%s line:%d id=%d k=%d\n",__FUNCTION__,__LINE__,id,k);

			uint8_t byteArray[k];   
			jbyteArray	jbarray = env->NewByteArray(k);

			memcpy(byteArray,data_array,k);
			jbyte *jy=(jbyte*)byteArray; 
			env->SetByteArrayRegion(jbarray, 0, k, jy);  

			//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setMediaItemPid","([B)V");
			env->CallVoidMethod(item_obj,getName_method,jbarray);
			env->DeleteLocalRef(jbarray);
	  	}
			
	  // mIpodService->notifyOnMediaItemPID(data_array,8);
      break;
	  case 1:
	  	{
	//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	 
			uint8_t byteArray[param_len-4];
			memcpy(byteArray,&data[i+4],param_len-4);
			jstring juid =	(jstring)env->NewStringUTF((char *) byteArray);	
			//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setMediaItemTitle","(Ljava/lang/String;)V");
			env->CallVoidMethod(item_obj,getName_method,juid);
			 env->DeleteLocalRef(juid);
			
		}		  
		
	  	break;
	  case 2:
	  		{
	  	 value = data[ i+4 ];
		 value = value+1;
		 int valuebyte = 0x01;
		 int type = 0;
		 valuebyte = valuebyte << value;
		 
		 
		jmethodID setName_method;
		setName_method = env->GetMethodID(obj_class , "setMediaItemType","(I)V");
		getName_method = env->GetMethodID(obj_class , "getMediaItemType","()I");
		//	LOGI("%s line:%d valuebyte=%d\n",__FUNCTION__,__LINE__,valuebyte);
		 type = env->CallIntMethod(item_obj,getName_method);
		type = type|valuebyte;
		
		env->CallVoidMethod(item_obj,setName_method,type);
	  	}
		 //mIpodService->notifyOnPlayBackShuffleMode(value);
	  	break;
	  case 3:
	  	
		//LOGI("%s line:%d \n",__FUNCTION__,__LINE__);
	  		 value = data[ i+4 ];
		  //mIpodService->notifyMediaItemRating(value);
		  getName_method = env->GetMethodID(obj_class , "setMediaItemRating","(I)V");
		  env->CallVoidMethod(item_obj,getName_method,value);
		 //mIpodService->notifyMediaItemMediaType(value);
	  	break;
	  case 4:
	  	
	
	  	  duration = ( data[ i+4 ] << 24 ) | (data[ i+1+4 ]<<16)  | (data[ i+2 +4] <<8) | data[ i+3+4 ];
		// 	LOGI("%s line:%d duration=%d\n",__FUNCTION__,__LINE__,duration);
	    // mIpodService->notifyOnDuration(duration);
		    getName_method = env->GetMethodID(obj_class , "setMediaItemDuration","(I)V");
		  env->CallVoidMethod(item_obj,getName_method,duration);
	  	break;
	  case 5:
	  	{
	         int k;
			for(k=0;k<(param_len-4);k++)
			{
				data_array[k] = data[i+4+k];
			}


			uint8_t byteArray[k];   
			jbyteArray	jbarray = env->NewByteArray(k);

			memcpy(byteArray,data_array,k);
			jbyte *jy=(jbyte*)byteArray; 
			env->SetByteArrayRegion(jbarray, 0, k, jy);    		   


			//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setMediaItemmAlbumPid","([B)V");
			env->CallVoidMethod(item_obj,getName_method,jbarray);
			env->DeleteLocalRef(jbarray);
	  	}
	  // mIpodService->notifyOnMediaItemAlbumPID(data_array,8);
	  	
	  	break;
	  case 6:
	  	{
	
	  	
			//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			uint8_t byteArray[param_len-4];
			memcpy(byteArray,&data[i+4],param_len-4);
			jstring juid =	(jstring)env->NewStringUTF((char *) byteArray);	
			//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setMediaItemAlbumTitle","(Ljava/lang/String;)V");
			env->CallVoidMethod(item_obj,getName_method,juid);
			 env->DeleteLocalRef(juid);
			//free(s_char);
		}
		   // mIpodService->notifyOnMediaItemAlumTitle((uint8_t *)s_char,param_len-4);
		  
		  	 
	  	break;
	  case 7:
	  	
		value = 	 (data[ i+4] <<8) | data[ i+1+4 ];
		  // mIpodService->notifyMediaItemAlbumTrackNumber(value);
		   getName_method = env->GetMethodID(obj_class , "setMediaItemAlbumTrackNumber","(I)V");
		env->CallVoidMethod(item_obj,getName_method,value);
	  	break;
	  case 8:
	  		value = 	 (data[ i+4] <<8) | data[ i+1+4 ];
		   //mIpodService->notifyMediaItemAlbumTrackCount(value);
		     getName_method = env->GetMethodID(obj_class , "setMediaItemAlbumTrackCount","(I)V");
		env->CallVoidMethod(item_obj,getName_method,value);
		   
	  	break;
	  case 9:
	  	value = 	 (data[ i+4] <<8) | data[ i+1+4 ];
		  // mIpodService->notifyMediaItemAlbumDiscNumber(value);
		   getName_method = env->GetMethodID(obj_class , "setMediaItemAlbumDiscNumber","(I)V");
		env->CallVoidMethod(item_obj,getName_method,value);
	  	break;
	  case 10:
	  		value = 	 (data[ i+4] <<8) | data[ i+1+4 ];
		  // mIpodService->notifyMediaItemAlbumDiscCount(value);
		     getName_method = env->GetMethodID(obj_class , "setMediaItemAlbumDiscCount","(I)V");
		env->CallVoidMethod(item_obj,getName_method,value);
	  	break;
	  case 11:
	  	
		{
			int k;
	
			for(k=0;k<(param_len-4);k++)
			{
				data_array[k] = data[i+4+k];
			}


			uint8_t byteArray[k];   
			jbyteArray	jbarray = env->NewByteArray(k);

			memcpy(byteArray,data_array,k);
			jbyte *jy=(jbyte*)byteArray; 
			env->SetByteArrayRegion(jbarray, 0, k, jy);    		   


			//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setMediaItemArtistPid","([B)V");
			env->CallVoidMethod(item_obj,getName_method,jbarray);
			env->DeleteLocalRef(jbarray);
	  	}
	   //mIpodService->notifyOnMediaItemArtistPID(data_array,8);
	  	break;
	  case 12:

		 {
		 	uint8_t byteArray[param_len-4];
			memcpy(byteArray,&data[i+4],param_len-4);
			jstring juid =	(jstring)env->NewStringUTF((char *) byteArray);	
				//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setMediaItemArtist","(Ljava/lang/String;)V");
			env->CallVoidMethod(item_obj,getName_method,juid);
			 env->DeleteLocalRef(juid);
		}
			//mIpodService->notifyOnMediaItemArtist((uint8_t *)s_char,param_len-4);
		  
	  	break;
	  case 13:
	  		
		{
			int k;
			for(k=0;k<(param_len-4);k++)
			{
				data_array[k] = data[i+4+k];
			}


			uint8_t byteArray[k];   
			jbyteArray	jbarray = env->NewByteArray(k);

			memcpy(byteArray,data_array,k);
			jbyte *jy=(jbyte*)byteArray; 
			env->SetByteArrayRegion(jbarray, 0, k, jy);  
			//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setMediaItemAlbumArtistPid","([B)V");
			env->CallVoidMethod(item_obj,getName_method,jbarray);
			env->DeleteLocalRef(jbarray);
	  	}
	   //mIpodService->notifyOnMediaItemAlbumArtistPID(data_array,8);
	  	break;
	  case 14:
	  	{
	  	 uint8_t byteArray[param_len-4];
			memcpy(byteArray,&data[i+4],param_len-4);
			jstring juid =	(jstring)env->NewStringUTF((char *) byteArray);	
				//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setMediaItemAlbumArtist","(Ljava/lang/String;)V");
			env->CallVoidMethod(item_obj,getName_method,juid);
			 env->DeleteLocalRef(juid);
		}
			//mIpodService->notifyOnMediaItemAlumArtist((uint8_t *)s_char,param_len-4);
		   
	  	break;
	  case 15:	  	
			
		 {
			int k;
			for(k=0;k<(param_len-4);k++)
			{
				data_array[k] = data[i+4+k];
			}


			uint8_t byteArray[k];   
			jbyteArray	jbarray = env->NewByteArray(k);

			memcpy(byteArray,data_array,k);
			jbyte *jy=(jbyte*)byteArray; 
			env->SetByteArrayRegion(jbarray, 0, k, jy);  
				//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setMediaItemGenrePid","([B)V");
			env->CallVoidMethod(item_obj,getName_method,jbarray);
			env->DeleteLocalRef(jbarray);
	  	}
		//mIpodService->notifyOnMediaItemGenrePID(data_array,8);
	  	break;
	  case 16:
	  	{
	  	 uint8_t byteArray[param_len-4];
			memcpy(byteArray,&data[i+4],param_len-4);
			jstring juid =	(jstring)env->NewStringUTF((char *) byteArray);	
				//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setMediaItemGenre","(Ljava/lang/String;)V");
			env->CallVoidMethod(item_obj,getName_method,juid);
			 env->DeleteLocalRef(juid);
		}
			//mIpodService->notifyOnMediaItemGenre((uint8_t *)s_char,param_len-4);
		  
	  	break;
	  case 17:
	  	{
			int k;
			for(k=0;k<(param_len-4);k++)
			{
				data_array[k] = data[i+4+k];
			}


			uint8_t byteArray[k];   
			jbyteArray	jbarray = env->NewByteArray(k);

			memcpy(byteArray,data_array,k);
			jbyte *jy=(jbyte*)byteArray; 
			env->SetByteArrayRegion(jbarray, 0, k, jy);  
				//LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setMediaItemComposerPid","([B)V");
			env->CallVoidMethod(item_obj,getName_method,jbarray);
			env->DeleteLocalRef(jbarray);
	  	}
		//mIpodService->notifyOnMediaItemComposerPID(data_array,8);
	  	break;
	  case 18:
	  	 {
	  	 uint8_t byteArray[param_len-4];
			memcpy(byteArray,&data[i+4],param_len-4);
			jstring juid =	(jstring)env->NewStringUTF((char *) byteArray);	
			//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setMediaItemComposer","(Ljava/lang/String;)V");
			env->CallVoidMethod(item_obj,getName_method,juid);
			 env->DeleteLocalRef(juid);
		}
	  	break;
	  case 19:
	  	 {
	  	 	value = data[ i+4 ];
			 jboolean b = false;
	        if(value == 1)
		    b=true;
			 getName_method = env->GetMethodID(obj_class , "setIsPartOfCompilation","(Z)V");
		   env->CallVoidMethod(item_obj,getName_method,b);
	  	 }
		// mIpodService->notifyOnMediaItemIsPartOfCompilation(value);
		//	 mIpodService->notifyOnPBAppleMusicRadioAd(value);
	  	break;
	  case 20:
	  		
	  	break;
	  case 21:
		 {
	  	 	value = data[ i+4 ];
			 jboolean b = false;
	        if(value == 1)
		    b=true;
			 getName_method = env->GetMethodID(obj_class , "setLikeSupported","(Z)V");
		   env->CallVoidMethod(item_obj,getName_method,b);
	  	 }
			
		 //mIpodService->notifyOnMediaItemIsLikeSupported(value);
	  	break;
	  case 22:
	  	  {
	  	 	value = data[ i+4 ];
			 jboolean b = false;
	        if(value == 1)
		    b=true;
			 getName_method = env->GetMethodID(obj_class , "setBanSupported","(Z)V");
		   env->CallVoidMethod(item_obj,getName_method,b);
	  	 }
			
		// mIpodService->notifyOnMediaItemIsBeanSupported(value);
	  	break;
	  case 23:
	  	
		{
	  	 	value = data[ i+4 ];
			 jboolean b = false;
	        if(value == 1)
		    b=true;
			 getName_method = env->GetMethodID(obj_class , "setLiked","(Z)V");
		   env->CallVoidMethod(item_obj,getName_method,b);
	  	 }
			
		   
	//	mIpodService->notifyOnMediaItemIsLiked(value);
	  	break;
	  case 24:
	  	{
	  	 	value = data[ i+4 ];
			 jboolean b = false;
	        if(value == 1)
		    b=true;
			 getName_method = env->GetMethodID(obj_class , "setBaned","(Z)V");
		   env->CallVoidMethod(item_obj,getName_method,b);
	  	 }
			
		   
		//mIpodService->notifyOnMediaItemIsBanned(value);
	  	break;
	  case 25:
	  	
	  	{
	  	 	value = data[ i+4 ];
			 jboolean b = false;
	        if(value == 1)
		    b=true;
			 getName_method = env->GetMethodID(obj_class , "setResidentOnDevice","(Z)V");
		   env->CallVoidMethod(item_obj,getName_method,b);
	  	 }
		   
		//mIpodService->notifyOnMediaItemIsResidentOnDevice(value);
	  	break;
	  case 26:
	  		 value = data[ i+4 ];	
			  getName_method = env->GetMethodID(obj_class , "setArtWorkFileTransferId","(I)V");
		   env->CallVoidMethod(item_obj,getName_method,value);
		//mIpodService->notifyMediaItemArtWorkFileTransferID(value);
		//  mIpodService->notifyMediaItemRating(value);
		 
	  	break;
	  case 27:
	  	value = 	 (data[ i+4] <<8) | data[ i+1+4 ];
		  getName_method = env->GetMethodID(obj_class , "setChapterCount","(I)V");
		   env->CallVoidMethod(item_obj,getName_method,value);
		// mIpodService->notifyMediaItemChapterCount(value);
	  	break;
		
	}
    i+=param_len;
  }

	// LOGI("%s line:%d id=%d\n",__FUNCTION__,__LINE__,id);

	 jclass obj_update_class = env->GetObjectClass(obj_MediaLibraryUpdate);
	  jstring jMeidaLibraryUid =  (jstring)env->NewStringUTF((char *) uidData);
	  
	// LOGI("%s line:%d id=%d\n",__FUNCTION__,__LINE__,id);
	   
	   uint8_t byteArray[uidDataLen];	  
	   
	   jbyteArray  jbarray = env->NewByteArray(uidDataLen);//建立jbarray数组//在heap里面申请， 不需要释放
  	  memcpy(byteArray,uidData,uidDataLen);
  	 jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
  	 env->SetByteArrayRegion(jbarray, 0, uidDataLen, jy);	 

      getName_method = env->GetMethodID(obj_update_class , "onMediaItem","(Lcom/autoipod/data/MediaItem;ILjava/lang/String;[B)V");
	 //	LOGI("%s line:%d id=%d\n",__FUNCTION__,__LINE__,id);
	  env->CallVoidMethod(obj_MediaLibraryUpdate,getName_method,item_obj,uidDataLen,jMeidaLibraryUid,jbarray);
	  //	LOGI("%s line:%d id=%d\n",__FUNCTION__,__LINE__,id);
	   env->DeleteLocalRef(jbarray);
	  env->DeleteLocalRef(jMeidaLibraryUid);
	  env->DeleteLocalRef(obj_class);
	  env->DeleteLocalRef(item_obj);
	  env->DeleteLocalRef(obj_update_class);
	  
}

void MediaLibraryUpdateMediaPlayList(uint8_t *data,int dataLen,uint8_t  *uidData, u32 uidDataLen)
{
	if(obj_MediaLibraryUpdate == NULL)
		  return;
   int i = 4;
   int param_len = 0;
   int id;
   uint8_t  *s_char;
   int index2;
   int count2;
   int  position ;
   int status;
   int value  = 0;
	uint8_t data_array[8];
	jmethodID getName_method;
	JNIEnv *env = getIPODJNIEnv();	

	jmethodID mediaPlaylist_costruct = env->GetMethodID(gClazzMediaPlaylist,"<init>","()V");
	jobject playlist_obj =env->NewObject(gClazzMediaPlaylist,mediaPlaylist_costruct);
	jclass obj_class = env->GetObjectClass(playlist_obj);
	 
  while(i<dataLen)
  {  
    param_len =	 ( data[ i ] << 8 ) | data[ i+1 ]; 
	id =  ( data[ i+2 ] << 8 ) | data[ i+3 ];
	LOGI("MediaLibraryUpdateMediaPlayList id=%d\n",id);
	switch(id)
	{
	  case 0: //MediaPlayListPersistentIdentifier
		{
			int k;
			for(k=0;k<(param_len-4);k++)
			{
				data_array[k] = data[i+4+k];
			}
			uint8_t byteArray[k];   
			jbyteArray	jbarray = env->NewByteArray(k);

			memcpy(byteArray,data_array,k);
			jbyte *jy=(jbyte*)byteArray; 
			env->SetByteArrayRegion(jbarray, 0, k, jy);  
			//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setPid","([B)V");
			env->CallVoidMethod(playlist_obj,getName_method,jbarray);
			env->DeleteLocalRef(jbarray);
		  }
	
	  break;	  
	  case 1://MediaPlayListName
	  	{
		LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
	 
			uint8_t byteArray[param_len-4];
			memcpy(byteArray,&data[i+4],param_len-4);
			jstring juid =	(jstring)env->NewStringUTF((char *) byteArray);	
				LOGI("%s line:%d name = %s\n",__FUNCTION__,__LINE__,byteArray);
			getName_method = env->GetMethodID(obj_class , "setName","(Ljava/lang/String;)V");
			env->CallVoidMethod(playlist_obj,getName_method,juid);
			 env->DeleteLocalRef(juid);
			
		}  	    
		break;		
	  case 2:	 // 	MediaPlayListParentPersistentIdentifier
	  	{
	  		int k;
			for(k=0;k<(param_len-4);k++)
			{
				data_array[k] = data[i+4+k];
			}
			uint8_t byteArray[k];   
			jbyteArray	jbarray = env->NewByteArray(k);

			memcpy(byteArray,data_array,k);
			jbyte *jy=(jbyte*)byteArray; 
			env->SetByteArrayRegion(jbarray, 0, k, jy);  
			//	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
			getName_method = env->GetMethodID(obj_class , "setParentPid","([B)V");
			env->CallVoidMethod(playlist_obj,getName_method,jbarray);
			env->DeleteLocalRef(jbarray);
		  }
		break;		
	  case 3:	//MediaPlayListlsGeniusMix  	
		 {
	  	 	value = data[ i+4 ];
			 jboolean b = false;
	        if(value == 1)
		    b=true;
			 getName_method = env->GetMethodID(obj_class , "setGeniusMix","(Z)V");
		   env->CallVoidMethod(playlist_obj,getName_method,b);
	  	 }
		break;		
	  case 4: //MediaPlaylistIsFolder
	      {
	  	 	value = data[ i+4 ];
			 jboolean b = false;
	        if(value == 1)
		    b=true;
			 getName_method = env->GetMethodID(obj_class , "setFolder","(Z)V");
		   env->CallVoidMethod(playlist_obj,getName_method,b);
	  	 }
		break;
	  case 5://MediaPlaylistContainedMediaItemsFileTransferIdentifier
	  	    value = data[ i+4 ];
		   getName_method = env->GetMethodID(obj_class , "setFileTransferID","(I)V");
		env->CallVoidMethod(playlist_obj,getName_method,value);
		break;
	  case 6://isApppeMusicRadioStation
	   {
	  	 	value = data[ i+4 ];
			 jboolean b = false;
	        if(value == 1)
		    b=true;
			 getName_method = env->GetMethodID(obj_class , "setAppMusicRadioStation","(Z)V");
		   env->CallVoidMethod(playlist_obj,getName_method,b);
	  	 }
		break;
	  case 7:
	  //	mIpodService->notifyOnMediaUpdatePlayListContentTransfer();
	  	//mIpodService->notifyOnMediaLibraryReset();	  
		break;
		
	}	
	i+=param_len;
  }

  
		  jstring juid =  (jstring)env->NewStringUTF((char *) uidData);
		getName_method = env->GetMethodID(obj_class , "setMediaLibraryUid","(Ljava/lang/String;)V");
				  env->CallVoidMethod(playlist_obj,getName_method,juid);
		

  
   jclass obj_update_class = env->GetObjectClass(obj_MediaLibraryUpdate);
   
	 uint8_t byteArray[uidDataLen]; 	
	 
	 jbyteArray  jbarray = env->NewByteArray(uidDataLen);//建立jbarray数组//在heap里面申请， 不需要释放
	memcpy(byteArray,uidData,uidDataLen);
   jbyte *jy=(jbyte*)byteArray;  //BYTE强制转换成Jbyte；
   env->SetByteArrayRegion(jbarray, 0, uidDataLen, jy);    


      getName_method = env->GetMethodID(obj_update_class , "onMediaPlaylist","(Lcom/autoipod/data/MediaPlaylist;ILjava/lang/String;[B)V");
	 
	  env->CallVoidMethod(obj_MediaLibraryUpdate,getName_method,playlist_obj,uidDataLen,juid,jbarray);
	   env->DeleteLocalRef(jbarray);
	  env->DeleteLocalRef(juid);
	  env->DeleteLocalRef(obj_class);
	  env->DeleteLocalRef(playlist_obj);
	  env->DeleteLocalRef(obj_update_class);
}


void OnMediaLibraryUpdate(uint8_t  *data, u32 dataLen){



   if(dataLen<10)
	return ;

   int i = 0;
   int j = 6;
	
	
   int param_len = 0;
  int k1 = 0,k2=0,k3=0;
  int id = 0;
    	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		char * libUID = NULL;
		int uidSize = 0;

	 while(j<dataLen)
	{
	
	 param_len =  ( data[ j ] << 8 ) | data[ j+1 ];
	// LOGI("%s line:%d id=%d\n",__FUNCTION__,__LINE__,data[ j+3 ]);
	    if(data[ j +2]==0x00 && data[ j+3 ]==0x00)
	 	{
	    	
		 
		   libUID = (char *)malloc(param_len-4);
		  
		   memcpy(libUID,&data[j+4],param_len-4);
		  LOGI("OnMediaLibraryUpdateUID \n");
		   	//mIpodService->notifyOnMeidaLibraryUpdateUID((uint8_t *)libUID,param_len-4);
			OnMediaLibraryUpdateUID((uint8_t *)libUID,param_len-4);
			uidSize = param_len-4;
		//	LOGI("%s line:%d uid=%s\n",__FUNCTION__,__LINE__,libUID);
			//mIpodService->notifyOnMediaLibraryName((uint8_t *)dataChar,param_len);
		//	free(dataChar);
	
	 	}
	    else if(data[ j +2]==0x00&&data[ j+3 ]==0x01)
	 	{
	 		char * dataChar = NULL;
		 
		   dataChar = (char *)malloc(param_len-4);
		  
		   memcpy(dataChar,&data[j+4],param_len-4);
		 
		// LOGI("OnMediaLibraryUpdateRevision \n");
		   //	mIpodService->notifyOnMeidaLibraryUpdateRevision((uint8_t *)dataChar,param_len-4,libUID,uidSize);
			OnMediaLibraryUpdateRevision((uint8_t *)dataChar,param_len-4,(uint8_t  *)libUID,uidSize);
		//	LOGI("%s line:%d revision=%s\n",__FUNCTION__,__LINE__,dataChar);
			//mIpodService->notifyOnMediaLibraryName((uint8_t *)dataChar,param_len);
			free(dataChar);
	 	}
	    else if(data[ j +2]==0x00&&data[ j+3 ]==0x02)
	 	{
	 	
		LOGI("MediaLibraryUpdateMediaItem \n");
	 //	LOGI("%s line:%d \n",__FUNCTION__,__LINE__);
	 	  MediaLibraryUpdateMediaItem(&data[j],param_len,(uint8_t  *)libUID,uidSize);
	 	}
		else if(data[ j +2]==0x00&&data[ j+3 ]==0x03)
	 	{
	 	
		LOGI("MediaLibraryUpdateMediaPlayList \n");
	 	    MediaLibraryUpdateMediaPlayList(&data[j+i],param_len,(uint8_t  *)libUID,uidSize);
	 	}
		 else if(data[ j +2]==0x00&&data[ j+3 ]==0x04)
	 	{
	 		uint8_t data_array[8];
		for(int ids=0;ids<0;ids++)
			{
			  data_array[ids] = data[j+4+ids];
			}
		
		LOGI("OnMediaItemDeleteUpdatePID \n");
	   //  mIpodService->notifyOnMediaItemDeleteUpdatePID(data_array,8,libUID,uidSize);
		 OnMediaItemDeleteUpdatePID(data_array,8,(uint8_t  *)libUID,uidSize);
	 	} 
		else if(data[ j +2]==0x00&&data[ j+3 ]==0x05)
	 	{
	 	uint8_t data_array[8];
		for(int ids=0;ids<0;ids++)
			{
			  data_array[ids] = data[j+4+ids];
			}
		
		LOGI("OnMediaPlayListDeleteUpdatePID \n");
	    // mIpodService->notifyOnMediaPlayListDeleteUpdatePID(data_array,8,libUID,uidSize);
		 OnMediaPlayListDeleteUpdatePID(data_array,8,(uint8_t  *)libUID,uidSize);
	 	}
		else if(data[ j +2]==0x00&&data[ j+3 ]==0x06)
	 	{
	 	
		LOGI("OnMediaLibraryReset 00 \n");
	 	// mIpodService->notifyOnMediaLibraryReset(libUID,uidSize);
		 OnMediaLibraryReset((uint8_t  *)libUID,uidSize);
	 	}
		else if(data[ j +2]==0x00&&data[ j+3 ]==0x07)
	 	{
	 	
		LOGI("OnMediaLibraryUpdateProgress 00 \n");
	 	// mIpodService->notifyOnMediaLibraryUPdateProgress(data[j+4],libUID,uidSize);
		 OnMediaLibraryUpdateProgress(data[j+4],(uint8_t  *)libUID,uidSize);
	 	}  
		else if(data[ j +2]==0x00&&data[ j+3 ]==0x08)
	 	{
	 	
		LOGI("OnIsHidingRemoteItems 00 \n");
	 	  // mIpodService->notifyOnIsHidingRemoteItems(data[j+4],libUID,uidSize);
		   OnIsHidingRemoteItems((int)data[j+4],(uint8_t  *)libUID,uidSize);
	 	} 
		else if(data[ j +2]==0x00&&data[ j+3 ]==0x09)
	 	{
	 	
		LOGI("OnPlayAllSongsCapable 00 \n");
	    	//mIpodService->notifyOnPlayAllSongsCapable(data[j+4],libUID,uidSize);
			OnPlayAllSongsCapable(data[j+4],(uint8_t  *)libUID,uidSize);
	 	} 
	 

	 j+=param_len;
	}

	 if(libUID!=NULL)
	 	free(libUID);
 
   
}






#if defined(__cplusplus)
}
#endif



