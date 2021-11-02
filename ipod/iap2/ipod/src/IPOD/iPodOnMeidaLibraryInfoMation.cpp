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

typedef struct MediaLibraryInformation{
	uint8_t * name;//utf8
	int 	nameSize;
	uint8_t * uniqueueIdentifier; //utf8
	int 	uidSize;
	uint8_t type;
	//static int 	size;

	struct MediaLibraryInformation* pNext;
}Media_Library_Information;


void OnMediaLibraryInformation(uint8_t  *data, u32 dataLen){
	LOGI("OnMediaLibraryInformation 1 \n");
	
	 if(obj_MediaLibraryInformation==NULL)
 	  return;
	 
	 LOGI("OnMediaLibraryInformation 2 \n");
	 int i = 0;
	 int j = 6;
	  int param_len0 = 0;
	  int data_len1 = 0;
	 int param_len = 0;
	 int k1 = 0,k2=0,k3=0;
		 LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		 JNIEnv *env = getIPODJNIEnv();	
	   
		   jclass list_cls = env->FindClass("java/util/ArrayList");
		   jmethodID list_costruct = env->GetMethodID(list_cls , "<init>","()V");
	   
		   jobject list_obj = env->NewObject(list_cls , list_costruct);
			
		   jmethodID list_add  = env->GetMethodID(list_cls,"add","(Ljava/lang/Object;)Z");
		   LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		 // jclass info_cls = env->FindClass("com/autoipod/data/MediaLibraryInformation");
		  LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		  jmethodID info_costruct = env->GetMethodID(gClazzMediaLibraryInformation,"<init>","()V");
	LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		                                  
		  // jmethodID info_costruct = env->GetMethodID(info_cls , "<init>", "(III)V");

	   if(dataLen<10)
	   return ;

		LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		while(j<dataLen)
			{
			 param_len0 =  ( data[ j ] << 8 ) | data[ j+1 ];
		 i=4;
		 LOGI("%s line:%d\n",__FUNCTION__,__LINE__);

		 //start here
		 MediaLibraryInformation *pInfo = (MediaLibraryInformation *)malloc(sizeof(MediaLibraryInformation));
		 memset(pInfo,0,sizeof(MediaLibraryInformation));
		 pInfo->pNext = NULL;
		  while(i<param_len0)
		{
		LOGI("%s line:%d\n",__FUNCTION__,__LINE__);
		 param_len =  ( data[ j+i ] << 8 ) | data[ j+i+1 ];
		  if((data[j+i+2]==0)&& (data[j+i+3]==0 ))//MeidaLibraryName
		 	{
		 	  char * dataChar = NULL;
		 	 
			   dataChar = (char *)malloc(param_len);
			    
			   memcpy(dataChar,&data[j+i+4],param_len);
			   	LOGI("%s line:%d MeidaLibraryName=%s\n",__FUNCTION__,__LINE__,dataChar);
			   // mIpodService->notifyOnMediaLibraryName((uint8_t *)dataChar,param_len);
				pInfo->name = (uint8_t *)dataChar;
				pInfo->nameSize = param_len;
				//free(dataChar);
			  
			  k1++;
		 	}
		 else if((data[j+i+2]==0)&& (data[j+i+3]==1 ))//MeidaLibraryUniqueIdentifier
		 	{
		 	  char * dataCharId = NULL;
		 	 
			   dataCharId = (char *)malloc(param_len);
			   memcpy(dataCharId,&data[j+i+4],param_len);
			  // mIpodService->notifyOnMeidaLibraryUID((uint8_t *)dataCharId,param_len);
			   	LOGI("%s line:%d MeidaLibraryUniqueIdentifier=%s\n",__FUNCTION__,__LINE__,dataCharId);
				pInfo->uniqueueIdentifier = (uint8_t *)dataCharId;
				pInfo->uidSize = param_len;
			   // free(dataCharId);
			  // p->uniqueueIdentifier= dataCharId;
			  k2++;
		 	}
		   else if((data[j+i+2]==0)&& (data[j+i+3]==2 ))//type
		 	{
			 //  p->type = data[j+i+4];
			 LOGI("%s line:%d MediaLibraryInformation type=%d\n",__FUNCTION__,__LINE__,data[j+i+4]);
			 //mIpodService->notifyMediaLibraryType( data[j+i+4]);
			 pInfo->type= data[j+i+4];
			 //mIpodService->notifyMediaItemMediaType( data[j+i+4]);
			   k3++;
		 	}

		 i+=param_len;
		}
		   LOGI("%s line:%d\n",__FUNCTION__,__LINE__);

		   // jobject info_obj = env->NewObject(info_cls , info_costruct , pInfo->name,
			 // pInfo->uniqueueIdentifier,pInfo->type);
			   jobject info_obj =env->NewObject(gClazzMediaLibraryInformation,info_costruct);
		    jfieldID name_fid = env->GetFieldID( gClazzMediaLibraryInformation, "mediaLibraryName", "Ljava/lang/String;"); 
			
              jstring jname =  (jstring)env->NewStringUTF((char *) pInfo->name);
			   env->SetObjectField( info_obj, name_fid, jname);
			   env->DeleteLocalRef(jname);
			  jfieldID uid_fid = env->GetFieldID( gClazzMediaLibraryInformation, "mediaLibraryUID", "Ljava/lang/String;");   
			  
			  jstring juid =  (jstring)env->NewStringUTF((char *) pInfo->uniqueueIdentifier);
			   env->SetObjectField( info_obj, uid_fid, juid);
			   env->DeleteLocalRef(juid);

             env->SetObjectField( info_obj, uid_fid, (env)->NewStringUTF((char *) pInfo->uniqueueIdentifier));
			 jfieldID type_fid = env->GetFieldID( gClazzMediaLibraryInformation, "mediaLibraryType", "I");
            env->SetIntField( info_obj, type_fid, pInfo->type);
			
			if(pInfo->name!=NULL)
				free(pInfo->name);
				if(pInfo->name!=NULL)
				free(pInfo->uniqueueIdentifier);
				free(pInfo);
			
		  env->CallBooleanMethod(list_obj , list_add , info_obj);
		 
		  env->DeleteLocalRef(info_obj);
		  
		//  addMediaLibraryInformation(pInfo);
		 j+=param_len0;
	  }
	  
	  env->DeleteLocalRef(list_cls);
	 
	  jclass obj_class = env->GetObjectClass(obj_MediaLibraryInformation);
	  jmethodID getName_method = env->GetMethodID(obj_class , "OnMediaLibrary","(Ljava/util/ArrayList;)V");
	  env->CallVoidMethod(obj_MediaLibraryInformation,getName_method,list_obj);
	  env->DeleteLocalRef(list_obj);
	  env->DeleteLocalRef(obj_class);
}




#if defined(__cplusplus)
}
#endif



