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
#include "usb.h"
#include <sys/stat.h>

//#include <jni.h>
//#include <linux/types.h>

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

#include "iAP2_utils.h"

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include "IpodService.h"

namespace android{


#if defined(__cplusplus)
extern "C"
{
#endif

int g_sessionId = -1;

int rb_recive_sync;

void* iAP2TransportWriteThread(void* param) 
{
	const uint8_t	kIap2PacketDetectData[] 	= { 0xFF, 0x55, 0x02, 0x00, 0xEE, 0x10 };
	//const uint8_t	kIap2PacketDetectData[] 	= { 0xFF, 0xf5, 0xf2, 0xf0, 0xEE, 0x1f };
	IPOD_HID_CONTEXT *handle = (IPOD_HID_CONTEXT *)param;
	rb_recive_sync = 0;

  while(!rb_recive_sync)
  	{
          
	  IPOD_ERROR error =  IPOD_HID_SendPacket(handle,(unsigned char *)kIap2PacketDetectData,6);
	   sleep(1);
	   
  	}
  return NULL;

 
}


void* iAP2TransportReadThread(void* param) 
{
	const uint8_t	HidRcvBuffer [1024] = {0};
	IPOD_HID_CONTEXT *handle = (IPOD_HID_CONTEXT *)param;
	int fd = open("/dev/usb/hidraw2-1:2.2", O_RDWR);

  while(!rb_recive_sync)
  	{
  	    //ALOGD("iAP2TransportReadThread \n");
		// ALOGD("g_handleg_handle->HIDReportCount =%d line:%d\n",handle->HIDReportCount,__LINE__);
		int ret=read(fd,(void *)HidRcvBuffer,1024);
	// int  ret=iPOD_HID_ReadHID(handle, HidRcvBuffer, 256);
	 // ALOGD("iAP2TransportReadThread ret:%d ",ret);
	  // ALOGD(" \n");
	 if(ret > 0)
	 	{
	 	 ALOGD(" iAP2TransportReadThread ret > 0 \n");
	 	int i;
	 	for(i=0;i<ret;i++)
	 		{
	 		  ALOGD(" iAP2TransportReadThread 0x%02x",HidRcvBuffer[i]);
			     ALOGD(" \n");
				 
			   
	 		}
		rb_recive_sync = 1;
		//break ;
	 	}
	  usleep(10000);
	
	 //  sleep(1);
  	}

  return NULL;
}




int main(int argc, char **argv){
#if 1
   // system("su");
	//system("mkdir /storage/emulated/test &");

	sp<ProcessState> proc(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();
    ALOGI("ServiceManager: iapServer  %p", sm.get());
    IpodService::instantiate();
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
	//IPOD_HID_CheckiPodDevice2();
	#else 
	ALOGD("main start \n");
	g_handle = (IPOD_HID_CONTEXT *)IPOD_HID_Init();
	IPOD_HID_CheckiPodDevice(g_handle);
	IPOD_HID_SetHIDChannel(g_handle,/*"/dev/hidraw0",*/-1);
	iAP2ClientStart();

		  ALOGD("main start 111 \n");
	 ALOGD("g_handleg_handle->HIDReportCount =%d line:%d\n",g_handle->HIDReportCount,__LINE__);
	//pthread_t write_thead;
	

	  // int ret = pthread_create(&write_thead, NULL, iAP2TransportReadThread,
		//		  g_handle);
	//	ALOGD("main start 111 ret=%d \n",ret);
	   
		 // pthread_t read_thead;
	   
	   
		//int  ret = pthread_create(&read_thead, NULL, iAP2TransportReadThreadSound,
				//  g_handle);

	 //  iAP2ClientStart();

	   
	  
    int index =5;
	// ALOGD("main start 2222 ret=%d \n",ret);
	/* while (1)
    {
      char c = getchar();
        if (c == 'q')
        {
			
			stopHid(g_sessionId);

		stopUSBDeviceModeAudio(g_sessionId);
		//stopHid(g_sessionId);
            return 0;            
        }
		
		else if(c=='p')
		{
		   playPause();
		}
		
		else if(c=='k')
		{
		   playNextTrack();
		}
		else if(c=='l')
		{
		   playProvTrack();
		}
		else if(c=='m')
			{
			  iAP2StartNowPlayingUpdates(g_sessionId);
			}
		else  if(c=='n')
			{
			 iAP2StopNowPlayingUpdates(g_sessionId);
			}
		else if(c=='t')
			{
			    index++;
			   SetNowPlayingInformation(-1,index,-1,g_sessionId);
			}
		else if(c=='r')
			{
			    index--;
			   SetNowPlayingInformation(-1,index,-1,g_sessionId);
			}
    }
	 */

     while(1)
     	{
     	  sleep(1);
     	}
		
	//usb_claim_interface();
#endif
return 0;
}
#if defined(__cplusplus)
};
#endif


}
