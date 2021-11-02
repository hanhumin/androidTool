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



namespace android{


#if defined(__cplusplus)
extern "C"
{
#endif
/*
int rb_recive_sync;

void* iAP2TransportWriteThread(void* param) 
{
	const uint8_t	kIap2PacketDetectData[] 	= { 0xFF, 0x55, 0x02, 0x00, 0xEE, 0x10 };
	//const uint8_t	kIap2PacketDetectData[] 	= { 0xFF, 0xf5, 0xf2, 0xf0, 0xEE, 0x1f };
	IPOD_HID_CONTEXT *handle = param;
	rb_recive_sync = 0;

  while(!rb_recive_sync)
  	{
          
	     ALOGD("iAP2TransportWriteThread \n");
		  ALOGD("g_handleg_handle->HIDReportCount =%d line:%d\n",handle->HIDReportCount,__LINE__);
	  IPOD_ERROR error =  IPOD_HID_SendPacket(handle,kIap2PacketDetectData,6);
	    ALOGD("iAP2TransportWriteThread error =%d\n",error);
	   sleep(1);
	   
  	}
}


void* iAP2TransportReadThread(void* param) 
{
	const uint8_t	HidRcvBuffer [256];
	IPOD_HID_CONTEXT *handle = param;

  while(!rb_recive_sync)
  	{
  	    ALOGD("iAP2TransportReadThread \n");
		 ALOGD("g_handleg_handle->HIDReportCount =%d line:%d\n",handle->HIDReportCount,__LINE__);
	 int  ret=iPOD_HID_ReadHID(handle, HidRcvBuffer, 256);
	  ALOGD("iAP2TransportReadThread ret:%d \n",ret);
	 if(ret > 0)
	 	{
	 	int i;
	 	for(i=0;i<ret;i++)
	 		{
	 		   ALOGD(" 0x%02x",HidRcvBuffer[i]);
			     ALOGD(" \n");
				 
			   
	 		}
		rb_recive_sync = 1;
		//break ;
	 	}
	  sleep(1);
	
	 //  sleep(1);
  	}
  
}

*/

int main(int argc, char **argv){
	ALOGD("main start \n");
	system("mkdir /storage/emulated/test &");
	g_handle = (IPOD_HID_CONTEXT *)IPOD_HID_Init();
	IPOD_HID_CheckiPodDevice(g_handle);
	IPOD_HID_SetHIDChannel(g_handle,/*"/dev/hidraw0",*/-1);


	pthread_t write_thead;

	iAP2ClientStart();


	  /*int ret = pthread_create(&write_thead, NULL, iAP2TransportReadThread,
        		g_handle);
	  ALOGD("main start 111 ret=%d \n",ret);

		pthread_t read_thead;
	

	   ret = pthread_create(&read_thead, NULL, iAP2TransportWriteThread,
        		g_handle);
	   */

	// ALOGD("main start 2222 ret=%d \n",ret);

     while(1)
     	{
     	  sleep(1);
     	}
		
	//usb_claim_interface();

return 0;
}
#if defined(__cplusplus)
};
#endif


}
