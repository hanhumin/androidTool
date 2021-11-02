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
#include <string.h>
#define LOG_TAG "IapService"

int IAPHID_DEBUG = 1;
#define IAPHID_DBUG(msg...)	if (IAPHID_DEBUG == 1) { ALOGD( msg); }

int IAPHIDD_DEBUG = 1;
#define IAPHIDD_DBUG(msg...)	if (IAPHIDD_DEBUG == 1) { ALOGD( msg); }

int IAPHIDD2_DEBUG = 1;
#define IAPHIDD2_DBUG(msg...)	if (IAPHIDD2_DEBUG == 1) { ALOGD(msg); }

int Ipod_Lingo_type = -1;
int Ipod_Lingo_cmd = -1;
int Ipod_Last_transId = 0;

#define APPLE_VID		0x05ac
#define APPLE_PID_MASK		0x1200
#define CFG_iUI_IDPS			6
#define CFG_iUI_NONIDPS			4

#define HID_REPORT_MAX	100

#define HID_DIRECTION_INPUT		0x80
#define HID_DIRECTION_OUTPUT		0x90

#define LOCAL_USB_AUDIO 0x01
#define LOCAL_USB_HID 0x02
#define LOCAL_USB_AUDIO_HID (LOCAL_USB_AUDIO | LOCAL_USB_HID)
 IPOD_HID_CONTEXT *g_handle;

// kernel/sound/usb/card.c  static struct usb_driver usb_audio_driver .name
const char *usb_audio_driver_name = "snd-usb-audio"; 
// kernel/drivers/hid/usbhid/hid-core.c static struct usb_driver hid_driver .name
const char *usb_hid_driver_name = "usbhid"; 
void IPOD_HID_checkFd();

int gHidrawFd = -1;
void *IPOD_MemAlloc(int length)
{
    return malloc(length);
}

int IPOD_MemDealloc(void *pMem)
{
    if (pMem)
    {
        free(pMem);
    }

    return 0;
}
void IPOD_MemSet(void *pBuffer, unsigned char ucVal, int length)
{
    memset(pBuffer,ucVal,length);
}

int IPOD_MemCopy(void *pDest,void *pSrc, int length)
{
    int retval=0;

    if (pDest && pSrc && length)
    {
        (void)memcpy(pDest,pSrc,length);
        retval=length;
    }

    return retval;
}

int IPOD_HID_ConvertStr2Int(char c){
    int value = 0;
    if(c >= '0' && c <= '9'){
        value = c - '0';
    }else if(c >= 'a' && c <= 'f'){
        value = c - 'a' + 10;
    }else{
        value = -1;
    }
    return value;
}

void IPOD_HID_InitDebug(void)
{
    //if(IAPHID_DEBUG == -1)
    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.debug.hid", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPHID_DEBUG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPHID_DEBUG = 0;
                break;
        }
    }
    //if(IAPHIDD_DEBUG == -1)
    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.debug.hiddata", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPHIDD_DEBUG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPHIDD_DEBUG = 0;
                break;
        }
    }    

    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.debug.hiddump", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPHIDD2_DEBUG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPHIDD2_DEBUG = 0;
                break;
        }
        if(IAPHIDD2_DEBUG){
            int value = 0;
            property_get("persist.iap.debug.hidlingo", iapDebug, "-1");
            value = IPOD_HID_ConvertStr2Int(iapDebug[1]);
            if(value >= 0)
                value += IPOD_HID_ConvertStr2Int(iapDebug[0]) * 16;
            else
                value = (-1);
            Ipod_Lingo_type = value;
            ALOGD(" %s Ipod_Lingo_type %d",__FUNCTION__,Ipod_Lingo_type);

            value = 0;
            property_get("persist.iap.debug.hidcmd", iapDebug, "00");
            value = IPOD_HID_ConvertStr2Int(iapDebug[1]);
            if(value >= 0)
                value += IPOD_HID_ConvertStr2Int(iapDebug[0]) * 16;
            else
                value = (-1);
            Ipod_Lingo_cmd = value;
            ALOGD(" %s Ipod_Lingo_cmd %d",__FUNCTION__,Ipod_Lingo_cmd);

        }
    }   

}

void *IPOD_HID_Init(void)
{
    void *retval=0;
    IPOD_HID_CONTEXT *piPodContext=(IPOD_HID_CONTEXT*)IPOD_MemAlloc(sizeof(IPOD_HID_CONTEXT));

    if (piPodContext)
    {
        IPOD_MemSet(piPodContext,0,sizeof(IPOD_HID_CONTEXT));
        piPodContext->fd_hidraw = -1;
        gHidrawFd = -1;
        retval=(void *)piPodContext;
    }
    IPOD_HID_InitDebug();
    return retval;
}

void IPOD_HID_Deinit(void *piPodHIDHandle)
{
    IAPHID_DBUG(" %d %s start",__LINE__,__FUNCTION__);
    IPOD_HID_CONTEXT *piPodContext=(IPOD_HID_CONTEXT *)piPodHIDHandle;
    iPOD_HID_CloseiPodDevice(piPodContext);
    IPOD_MemDealloc(piPodContext);
    IAPHID_DBUG(" %d %s end",__LINE__,__FUNCTION__);
}

int IPOD_HID_IsExistHid(char *file_name)
{
    int return_stat;
    struct stat sb;
    return_stat = stat(file_name,&sb);
    if(return_stat < 0)
    {
        return 0;
    }
    return 1;
}
int IPOD_HID_CheckiPodDevice2()
{
int retval=-1;
void *handle = 0;
int cfg_num, iface_num,num_interface;
int ret = 0;
int i,j;
int fHasHidAudioClass = 0;
char dname[32] = {0};
//IPOD_HID_CONTEXT *piPodContext=piPodPALHandle;
//piPodContext->usb_dev_handle=0;


        
//usb_set_debug(255);
usb_init();

for( i = 0 ; i < 5 && handle == 0; i++)    
{
	int fidbus = usb_find_busses();
	ALOGD(" %s .fidbus=%d\n",__FUNCTION__,fidbus);
	int fiddev = usb_find_devices();
	ALOGD(" %s .fiddev=%d\n",__FUNCTION__,fiddev);
	handle = iPOD_HID_FindiPodDevice(&cfg_num, &iface_num,&num_interface);
	
}

#if 1
if ( handle == 0)
{
	ALOGD(" %s Could not find device(HID).\n",__FUNCTION__);
}
else
{

	ALOGD("cfg_num=%d, iface_num=%d\n", cfg_num, iface_num);
	// Check the interfaces in the current configuration
	for(j = 0;j< num_interface;j++){
		ret = usb_get_driver_np(handle, j, dname, 31);
		ALOGD("usb_get_driver_np ret=%d dname %s\n", ret,dname);
		if(ret == 0){
			unsigned int lenAudio = strlen(usb_audio_driver_name);
			unsigned int lenHid = strlen(usb_hid_driver_name);
			if(strlen(dname) == lenAudio 
				&& memcmp(dname,usb_audio_driver_name,lenAudio) == 0){
				fHasHidAudioClass |= LOCAL_USB_AUDIO;
			}else if(strlen(dname) == lenHid 
				&& memcmp(dname,usb_hid_driver_name,lenHid) == 0){
				fHasHidAudioClass |= LOCAL_USB_HID;
			}
		}
	}
	
          ALOGD("usb_get_driver_np fHasHidAudioClass= 0x%x\n",fHasHidAudioClass);

	// If the current configuration is invalid, set the valid configuration.
	if ( fHasHidAudioClass !=  LOCAL_USB_AUDIO_HID)
	{
		usb_detach_kernel_driver_np(handle, 0);
		ret = usb_set_configuration(handle, cfg_num); 
	}
	
	


	if ( ret )
	{
	
	    usb_dev_handle *pHandle = (usb_dev_handle *)handle;
	    if(pHandle != NULL)
	        	ALOGD(" %s usb_close : %d.\n",__FUNCTION__,pHandle->fd);
		ALOGD("Could not set configuration\n");
		usb_close(handle);
	
	}
	else
	{

		//IPOD_HID_CONTEXT *piPodContext=piPodPALHandle;
		//retval=0;
		ALOGD("hidraw handle : %p.\n",handle);
		//piPodContext->usb_dev_handle=0;
		usb_close(handle);
		
	}
	
}



#endif

return retval;
}
int IPOD_HID_CheckiPodDevice(void *piPodPALHandle)
{
	int retval=-1;
	void *handle = 0;
	int cfg_num, iface_num,num_interface;
	int ret = 0;
	int i,j;
	int fHasHidAudioClass = 0;
	char dname[32] = {0};
	IPOD_HID_CONTEXT *piPodContext=piPodPALHandle;
	piPodContext->usb_dev_handle=0;

            
	//usb_set_debug(255);
	usb_init();

	for( i = 0 ; i < 5 && handle == 0; i++)    
	{
		int fidbus = usb_find_busses();
		int fiddev = usb_find_devices();
		handle = iPOD_HID_FindiPodDevice(&cfg_num, &iface_num,&num_interface);
		
	}

#if 1
	if ( handle == 0)
	{
		ALOGD(" %s Could not find device(HID).\n",__FUNCTION__);
	}
	else
	{
	
		ALOGD("cfg_num=%d, iface_num=%d\n", cfg_num, iface_num);
		// Check the interfaces in the current configuration
		for(j = 0;j< num_interface;j++){
			ret = usb_get_driver_np(handle, j, dname, 31);
			ALOGD("usb_get_driver_np ret=%d dname %s\n", ret,dname);
			if(ret == 0){
				unsigned int lenAudio = strlen(usb_audio_driver_name);
				unsigned int lenHid = strlen(usb_hid_driver_name);
				if(strlen(dname) == lenAudio 
					&& memcmp(dname,usb_audio_driver_name,lenAudio) == 0){
					fHasHidAudioClass |= LOCAL_USB_AUDIO;
				}else if(strlen(dname) == lenHid 
					&& memcmp(dname,usb_hid_driver_name,lenHid) == 0){
					fHasHidAudioClass |= LOCAL_USB_HID;
				}
			}
		}
		
              ALOGD("usb_get_driver_np fHasHidAudioClass= 0x%x\n",fHasHidAudioClass);

		// If the current configuration is invalid, set the valid configuration.
		if ( fHasHidAudioClass !=  LOCAL_USB_AUDIO_HID)
		{
			usb_detach_kernel_driver_np(handle, 0);
			ret = usb_set_configuration(handle, cfg_num); 
		}
		
		

	
		if ( ret )
		{
		
		    usb_dev_handle *pHandle = (usb_dev_handle *)handle;
		    if(pHandle != NULL)
    	        	ALOGD(" %s usb_close : %d.\n",__FUNCTION__,pHandle->fd);
			ALOGD("Could not set configuration\n");
			usb_close(handle);
		
		}
		else
		{
	
			IPOD_HID_CONTEXT *piPodContext=piPodPALHandle;
			retval=0;
			ALOGD("hidraw handle : %p.\n",handle);
			piPodContext->usb_dev_handle=0;
			usb_close(handle);
			
		}
		
	}
	
	
#endif

	return retval;
}

void iPOD_HID_CloseiPodDevice(void* piPodPALHandle)
{
	IPOD_HID_CONTEXT *piPodContext=piPodPALHandle;

	if (piPodContext)
	{
	    usb_dev_handle *handle = (usb_dev_handle *)piPodContext->usb_dev_handle;
	    if(handle != NULL)
	        	ALOGD(" %s usb_close : %d.\n",__FUNCTION__,handle->fd);
	    if(piPodContext->usb_dev_handle)
        		usb_close(piPodContext->usb_dev_handle);
		piPodContext->usb_dev_handle=NULL;
		IAPHID_DBUG("%s usb is closed.\n",__FUNCTION__);
	}
	else
	{
		IAPHID_DBUG("Fail to close usb.\n");
	}
}

void *iPOD_HID_FindiPodDevice(int *cfg_num, int *iface_num,int *num_interface)
{
	usb_dev_handle *handle = NULL;
	struct usb_bus *bus;
	struct usb_device *dev;
	int ncfg, niface, i, j;
	struct usb_config_descriptor *config;
	struct usb_interface *interface;
	int found = 0;
	*cfg_num = 1;
	*iface_num = 1;
	*num_interface = 1;

	for ( bus=usb_get_busses() ; bus ; bus=bus->next )
	{
		for ( dev=bus->devices ; dev ; dev=dev->next )
		{
			ALOGD("Vid=%x, Pid=%x \n", dev->descriptor.idVendor, dev->descriptor.idProduct);

			if ( dev->descriptor.idVendor == APPLE_VID &&
			        ((dev->descriptor.idProduct & APPLE_PID_MASK) == APPLE_PID_MASK) )
			{
				#if 1
				/* TODO: find iPod USB Interface Configuration number */
				ncfg = dev->descriptor.bNumConfigurations;
				config = dev->config;
				ALOGD("Check max configurataion number = %d.\n",ncfg);
				for ( i=0 ; i<ncfg ; i++ )
				{
					//if ( config[i].iConfiguration == CFG_iUI_IDPS || config[i].iConfiguration == CFG_iUI_NONIDPS)
					//{
						*cfg_num = config[i].bConfigurationValue;
						niface = config[i].bNumInterfaces;
						ALOGD(" [%d] bConfigurationValue = %d niface %d.\n",i,config[i].bConfigurationValue,niface);
						interface = config[i].interface;

						for ( j=0 ; j<niface ; j++ )
						{
						    ALOGD("  VendorID[0x%x] PID[0x%x] InterfaceClass = %d subClass %d interfaceNum %d.\n",
                                                                                                                        dev->descriptor.idVendor,
                                                                                                                        dev->descriptor.idProduct, 
                                                                                                                        interface[j].altsetting[0].bInterfaceClass,
                            IOCTL_USB_GETDRIVER                                                                                            interface[j].altsetting[0].bInterfaceSubClass,
                                                                                                                        interface[j].altsetting[0].bInterfaceNumber
                                                                                                                        );
							if ( interface[j].altsetting[0].bInterfaceClass
							        == USB_CLASS_HID )
							{
								*iface_num = interface[j].altsetting[0].bInterfaceNumber;
								found = 1;
								*num_interface = niface;                                
								ALOGD("Check selected nConfigurataion = %d, nInterface = %d altSetNum %d .\n",i, j,interface[j].num_altsetting);
								break;
							}
						} /* end of for (niface) */
					//}
					//else
					//{
					//	IAPHID_DBUG("Could not fine iConfiguration.\n");
					//}

					if ( found ) break;
				} /* end of for (ncfg) */
				#else
				found = 1;
				#endif
				if ( found )
				{
					if ( !(handle = usb_open(dev)) )
					{
						ALOGD("  %s Could not open usb device.\n",__FUNCTION__);
						return NULL;
					}
					ALOGD(" %s usb_open handle  fd %d \n",__FUNCTION__,handle->fd);
					#if 1
					{
						int result  = 0;
						#if 1
						int extraCurrent = 1600;
						#else
						int extraCurrent = 500;
						switch(dev->descriptor.idProduct){
							// Apple device Pid which is descripted in http://www.linux-usb.org/usb.ids
							case 0x129A: // iPad
							case 0x129F: // iPad 2
							case 0x12A2: // iPad 2 (3G; 64GB)
							case 0x12A6: // iPad 3 (3G, 16 GB)
							case 0x12A9: // iPad 2
								extraCurrent = 1600;
								break;
							default:
								extraCurrent = 500;
								break;
						}
						#endif
						result = usb_control_msg(handle, 0x40,0x40,500,extraCurrent,NULL,0,100000);
						ALOGD(" usb_control_msg result:%d\n",result);
					}
					#endif

					break;
				}
			} /* end of if (iphone) */
		} /* end of for (dev) */

		if ( handle != NULL ) break;
	} /* end of for (bus) */

	return handle;
}

static int iPOD_ReadHIDReportDescriptor(unsigned char *pucBuf,unsigned char ucLength) {
	int retval=0;
	int i;
	if (ucLength<4) {
		for (i=0;i<ucLength;i++) {
			retval|=(*(pucBuf+i)<<(i*8));
		}
	}
	return retval;
}

int IPOD_HID_SetHIDChannel(IPOD_HID_CONTEXT *piPodHIDContext,/*const char *path,*/ unsigned int block) {
	int retval=-1;
	int retryCnt = 50;
	int retry = 0;
	int hidnum;
	char *path = (char *) malloc(20);
	for(retry = 0; retry < retryCnt ; retry++){
		for( hidnum = 0;hidnum <5;hidnum++)
		{
			
			  memset(path,0,20);
			  sprintf(path,"/dev/hidraw%d",hidnum);
			
		    retval=IPOD_SetHIDChannel(piPodHIDContext,path);
		    if(retval==0)
			 break;
		}
		if(retval == 0){
			if(retry > 0)
				ALOGD(" %s retry[%d , %d ms].\n",__FUNCTION__,retry,retry*10);
			break;
		}else{
			IPOD_ERROR Err = IPOD_NO_ERROR;
			IPOD_CBACK(ipod_stack_cbacks,iPodCetDetectStatus_cb,&Err);
			if(Err != IPOD_NO_ERROR){
				break;
			}
			ALOGD( "dev open error is ");
			usleep(10*1000);
		}
	}
	ALOGD( "dev open error is ");
	if (retval==0) {
		int length=0;
		unsigned char *pReportDescriptorInfo;

		retval=-1;
		
		pReportDescriptorInfo=iPOD_HID_GetHIDReportDescriptorInfo(piPodHIDContext,&length);

		if (length && pReportDescriptorInfo) {
			
			// parse report descriptor
			
			int i;
			
			int HIDReportCount=0;
			
			for (i=0;i<length;i++) {
				unsigned char ucSize=pReportDescriptorInfo[i] & 0x03;
				unsigned char ucTag=pReportDescriptorInfo[i] & 0xFC;
				switch (ucTag) {
					#if 0
					case 0x74:	// Report size
						piPodHIDContext->HIDReportSize=(unsigned char)iPOD_ReadHIDReportDescriptor(pReportDescriptorInfo+i+1,ucSize);
						IAPHID_DBUG("Report size=%d,%d\n",piPodHIDContext->HIDReportSize,*(pReportDescriptorInfo+i+1));
						break;
					#endif
					case 0x84:	// Report ID
						piPodHIDContext->HIDReport[HIDReportCount].ID=(unsigned char)iPOD_ReadHIDReportDescriptor(pReportDescriptorInfo+i+1,ucSize);
						break;
					case 0x94:	// Report count
						piPodHIDContext->HIDReport[HIDReportCount].length=(unsigned char)iPOD_ReadHIDReportDescriptor(pReportDescriptorInfo+i+1,ucSize);
						break;
					case 0x80:	// INPUT
						piPodHIDContext->HIDReport[HIDReportCount].direction=HID_DIRECTION_INPUT;
						HIDReportCount++;
						break;
					case 0x90:	// OUTPUT
						piPodHIDContext->HIDReport[HIDReportCount].direction=HID_DIRECTION_OUTPUT;
						HIDReportCount++;
						break;
					default:
						break;
				}
				i+=ucSize;
			}
			piPodHIDContext->HIDReportCount=HIDReportCount;
//			IAPHID_DBUG("Report Size = %d, Report count = %d\n",piPodHandle->HIDReportSize,HIDReportCount);
			for (i=0;i<HIDReportCount;i++) {
				ALOGD("%03d: HID Report %03d:%03d %s\n",i,piPodHIDContext->HIDReport[i].ID,piPodHIDContext->HIDReport[i].length,(piPodHIDContext->HIDReport[i].direction==0x80)?"IN":"OUT");
			}
			retval=0;
		    }

	}
	else
	{
		ALOGD("iPOD_HID_SetHIDChannel ERROR!!!!!!!!!!!!\n");
	}
	return retval;
}

int IPOD_SetHIDChannel(void *piPodPALHandle,const char *path)
{
	int retval=-1;
	int fd;
	int i, ret, desc_size = 0;
	IPOD_HID_CONTEXT *piPodContext=piPodPALHandle;
    ALOGE("%s hidraw %p \n" ,__FUNCTION__,&piPodContext->fd_hidraw);
    IPOD_HID_checkFd();
    
        if(piPodContext->fd_hidraw >= 0)
        {
            ALOGE("%s path %s hidraw %d\n", __FUNCTION__,path,piPodContext->fd_hidraw);
        }
    //chmod(path, 0666);
	fd = open(path, O_RDWR);
	if ( fd < 0 )
	{
		ALOGE("cannot open device handle %p %s.\n",piPodContext,path);
	}
	else
	{
		struct hidraw_devinfo info;
		gHidrawFd = fd;
		ALOGE(" %s open fd %d.\n",__FUNCTION__,fd);
		memset(&info, 0x0, sizeof(struct hidraw_devinfo));
		/* Get Raw Info */
		ret = ioctl(fd, HIDIOCGRAWINFO, &info);

		if ( ret < 0 )
		{
			ALOGE(" %s HIDIOCGRAWINFO\n",__FUNCTION__);
			close(fd);
			gHidrawFd = -1;
		}
		else
		{
			ALOGD("%s Raw Info: fd %d\n",__FUNCTION__,fd);
			ALOGD("\tbustype: %d\n", info.bustype);
			ALOGD("\tvendor: 0x%04x\n", info.vendor);
			ALOGD("\tproduct: 0x%04x\n", info.product);

			if ( (info.vendor != APPLE_VID)
			        || ((info.product & APPLE_PID_MASK) != APPLE_PID_MASK) )
			{
				IAPHID_DBUG(" %s is not iphone device. fd  %d\n", path,fd);
				close(fd);
				gHidrawFd = -1;
				ALOGE(" %s close fd %d.\n",__FUNCTION__,fd);
			}
			else
			{
				piPodContext->fd_hidraw=fd;
				retval=0;
			}
		}
	}
	ALOGI(" %s fd_hidraw %d\n",__FUNCTION__,piPodContext->fd_hidraw);

	return retval;
}

unsigned char *iPOD_HID_GetHIDReportDescriptorInfo(void *piPodPALHandle,int *pLength)
{
	IPOD_HID_CONTEXT *piPodContext=piPodPALHandle;
	struct hidraw_report_descriptor rpt_desc;
	int desc_size=0;
	unsigned char *retval=0;
	int ret;

	if (pLength && piPodContext && (piPodContext->fd_hidraw>=0))
	{
		*pLength=0;
		memset(&rpt_desc, 0x0, sizeof(struct hidraw_report_descriptor));
		/* Get Report Descriptor Size */
		ret = ioctl(piPodContext->fd_hidraw, HIDIOCGRDESCSIZE, &desc_size);

		if ( ret < 0 )
		{
			ALOGD(" %s HIDIOCGRDESCSIZE\n",__FUNCTION__);
		}
		else
		{
			ALOGD("Report Descriptor Size: %d\n", desc_size);
		}

		/* Get Report Descriptor */
		rpt_desc.size = desc_size;
		ret = ioctl(piPodContext->fd_hidraw, HIDIOCGRDESC, &rpt_desc);

		if ( ret < 0 )
		{
			ALOGD("%s HIDIOCGRDESC\n",__FUNCTION__);
		}
		else
		{
			/* TODO: parse rpt_desc */
			unsigned int i;
			ALOGD("Report Descriptor:\n");

			
			retval=rpt_desc.value;
			*pLength=rpt_desc.size;
		} 
	}

	return retval;
}

void IPOD_HID_checkFd()
{
    if(gHidrawFd >= 0){
        ALOGD(" x1b[1;31m %s [wskim] gHidrawFd %d\x1b[1;0m ",__FUNCTION__,gHidrawFd);
        close(gHidrawFd);
    }
    gHidrawFd = -1;
}

void IPOD_HID_CloseHIDChannel(void *piPodPALHandle)
{
#if 1 //system exception (jellybean)
	IPOD_HID_CONTEXT *piPodContext=piPodPALHandle;


	if (piPodContext)
	{
		ALOGD(" %s : fd %d\n",__FUNCTION__,piPodContext->fd_hidraw);
		if(piPodContext->fd_hidraw >= 0)
			close(piPodContext->fd_hidraw);
		piPodContext->fd_hidraw=-1;
		gHidrawFd = -1;
	}
#endif
}


#if 0
void IPOD_DumpMemory(unsigned char *pucBuffer,int length,int op) {
    int i;
    int j;
    unsigned char buf[256];

    if (length>0) {
        for (i=0;i<length;i+=16) {
            int index = 0;
            index += sprintf(&buf[index],"HID %04X: ",i);
            for (j=0;j<16;j++) {
                if ((i+j)<length) {
                    index += sprintf(&buf[index]," %02X",pucBuffer[i+j]);
                } else {
                    index += sprintf(&buf[index],"   ");
                }
            }
            index += sprintf(&buf[index]," : ");
            for (j=0;j<16;j++) {
                if ((i+j)<length) {
                    index += sprintf(&buf[index],"%c",(pucBuffer[i+j]>' ')?pucBuffer[i+j]:'.');
                } else {
                    break;
                    //IAPHID_DBUG("  ");
                }
            }
            if(op==1)
            ALOGE("%s",buf);
            else
            ALOGD("%s",buf);
        }
    }
}

#endif
//#define HID_TIME_CHECK 

void iPOD_DumpMemory(unsigned char *pucBuffer,int length) {
#if 1
    int i;
    int j;
    char *buf = NULL;

    buf = (char *)malloc(length*20);

    if (length>0) {
        for (i=0;i<length;i+=16) {
            int index = 0;
            index += sprintf(&buf[index],"DUMP %04X: ",i);
            for (j=0;j<16;j++) {
                if ((i+j)<length) {
                    index += sprintf(&buf[index]," %02X",pucBuffer[i+j]);
                } else {
                    index += sprintf(&buf[index],"   ");
                }
            }
            index += sprintf(&buf[index]," : ");
            for (j=0;j<16;j++) {
                if ((i+j)<length) {
                    index += sprintf(&buf[index],"%c",(pucBuffer[i+j]>' ')?pucBuffer[i+j]:'.');
                } else {
                    break;
                    //IAPHID_DBUG("  ");
                }
            }
            ALOGD(" %s",buf);
        }
    }
    free(buf);
#else
    int i;
    int j;
    int max = 32;
    char buf[512];
    if(length > max)
        length = max;

    if (length>0) {
        ALOGD(" <HID DUMP> ");
        for (i=0;i<length;i+=16) {
            int index = 0;
            index += sprintf(&buf[index],"HID %04X: ",i);
            for (j=0;j<16;j++) {
                if ((i+j)<length) {
                    index += sprintf(&buf[index]," %02X",pucBuffer[i+j]);
                } else {
                    index += sprintf(&buf[index],"   ");
                }
            }
            index += sprintf(&buf[index]," : ");
            for (j=0;j<16;j++) {
                if ((i+j)<length) {
                    index += sprintf(&buf[index],"%c",(pucBuffer[i+j]>' ')?pucBuffer[i+j]:'.');
                } else {
                    break;
                    //IAPHID_DBUG("  ");
                }
            }
            //if(length == 6 )
            //    ALOGE(" wskim bingo %s",buf);
            ALOGD(" %s",buf);
        }
    }
#endif    
}


struct timespec absTime = { 0, 0 };
#define NANO_S (1000*1000*1000)
int ghidReadCount = 0;
int iPOD_HID_ReadHID(void *piPodPALHandle,unsigned char *pBuffer, int bufferLength)
{
	IPOD_HID_CONTEXT *piPodContext=piPodPALHandle;
	int retval=0;

	IPOD_ERROR Err = IPOD_NO_ERROR;
	if(piPodContext->fd_hidraw < 0)
	{
		return -1;
	}
	IPOD_CBACK(ipod_stack_cbacks,iPodCetDetectStatus_cb,&Err);
	if(Err != IPOD_NO_ERROR){
		if(piPodContext->fd_hidraw >= 0)
			close(piPodContext->fd_hidraw);
		piPodContext->fd_hidraw = -1;
		gHidrawFd = -1;
		ghidReadCount++;
		//if(ghidReadCount % 500 == 0)
		//	ALOGI("[%s] read after disconnecting fd %d ghidReadCount %d\n", __func__,piPodContext->fd_hidraw,ghidReadCount);
		return -1;
	}else{
		ghidReadCount = 0;
	}
            
        #ifdef HID_TIME_CHECK
       struct timespec absStime = { 0, 0 };
       struct timespec absEtime = { 0, 0 };
       struct timespec absDtime = { 0, 0 };
       clock_gettime(CLOCK_REALTIME, &absStime);
       #endif
	if (piPodContext && pBuffer && bufferLength)
	{
		int ret;
		ret=fcntl(piPodContext->fd_hidraw,F_GETFL,0);
		//IAPHID_DBUG("GETFL: 0x%X\n",ret);
		ret=fcntl(piPodContext->fd_hidraw,F_SETFL,ret | O_NONBLOCK);
		//IAPHID_DBUG("SETFL restult: %d\n",ret);
		retval=read(piPodContext->fd_hidraw,pBuffer,bufferLength);
        if(retval>0)
		//ALOGD("ACTURE READ retval=%d ",retval);
		
		//int i;
		//for(i=0;i<retval;i++)
			//{
			  //ALOGD(" 0x%02X ",pBuffer[i]);
			//}
		 
		if (retval<0)
		{
			switch (errno)
			{
				case  11:	// try again
					retval=0;
					break;
				default:
					ALOGI("[%s:%d] erro : %s(%d) \n", __func__, __LINE__, strerror(errno),errno);
					retval=0;
					break;
			}
		}
		//else {
			//IAPHIDD_DBUG("read success\n");
		//	IPOD_DumpMemory(pBuffer, retval);
		//}

		ret=fcntl(piPodContext->fd_hidraw,F_SETFL,ret);
		ret=fcntl(piPodContext->fd_hidraw,F_GETFL,0);
		//IAPHID_DBUG("GETFL: 0x%X\n",ret);
	}
    #ifdef HID_TIME_CHECK
    clock_gettime(CLOCK_REALTIME, &absEtime);
    if(absEtime.tv_nsec < absStime.tv_nsec){
        absEtime.tv_sec -= 1;
        absEtime.tv_nsec += NANO_S;
    }

    absDtime.tv_sec =  absEtime.tv_sec - absStime.tv_sec;
    absDtime.tv_nsec = absEtime.tv_nsec - absStime.tv_nsec;
    //if(retval > 900)
    //iPOD_DumpMemory(pBuffer, 20);
    {
        int realLength = 0;
        int offset = 2;
        if(pBuffer[1 + offset] == 0x00){
            realLength = (pBuffer[2 + offset] << 8) | pBuffer[3 + offset];
        }else{
            realLength = pBuffer[1 + offset];
        }
        if(realLength > 800){
            absTime.tv_sec += absDtime.tv_sec;
            absTime.tv_nsec +=  absDtime.tv_nsec;
            if(absTime.tv_nsec > NANO_S){
                absTime.tv_sec += 1;
                absTime.tv_nsec -= NANO_S;
            }
            //ALOGD(" Hid read %ld.%09ld realLength %d %ld.%09ld",absDtime.tv_sec,absDtime.tv_nsec,realLength,absTime.tv_sec,absTime.tv_nsec);
        }
    }
    #endif
	return retval;
}

static int iPOD_GetReportIDToSend(void *pHandle,int length,unsigned char *pucReportID) {
	int retval=0;
//	IPOD_IO_HANDLE *piPodHandle=pHandle;
	IPOD_HID_CONTEXT *piPodHIDContext = pHandle;
	int i;
	int lastMaxReportLength=0;
	int lastMaxReportID=0;
	
	if (piPodHIDContext && pucReportID) {
		if(piPodHIDContext->HIDReportCount)
			{
			 // ALOGD("%d %s piPodHIDContext->HIDReportCount:%d\n",__LINE__,__FUNCTION__,piPodHIDContext->HIDReportCount);
			}
			else
				{
				}
		for (i=0;i<piPodHIDContext->HIDReportCount;i++) {
			if (piPodHIDContext->HIDReport[i].direction==HID_DIRECTION_OUTPUT) {
				IAPHIDD_DBUG("piPodHandle->HIDReport[i].length=%d\n",piPodHIDContext->HIDReport[i].length);
				if (piPodHIDContext->HIDReport[i].length>length) {
					retval=piPodHIDContext->HIDReport[i].length;
					*pucReportID=piPodHIDContext->HIDReport[i].ID;
					break;
				}
				// limit report ID to send via control transfer
				if (piPodHIDContext->HIDReport[i].length>(64-1)) {
					retval=lastMaxReportLength;
					*pucReportID=lastMaxReportID;
					break;
				}
				lastMaxReportLength=piPodHIDContext->HIDReport[i].length;
				lastMaxReportID=i;
			}
		}

	}
	if(retval==0&&lastMaxReportID!=0){
		retval=lastMaxReportLength;
		*pucReportID=lastMaxReportID;
	}
	return retval;
}
int iPOD_HID_WriteHID(void *piPodPALHandle,unsigned char *pBuffer, int bufferLength)
{
    IPOD_HID_CONTEXT *piPodContext=piPodPALHandle;
    int retval=-1;
#ifdef HID_TIME_CHECK
    struct timespec absStime = { 0, 0 };
    struct timespec absEtime = { 0, 0 };
    struct timespec absDtime = { 0, 0 };
    clock_gettime(CLOCK_REALTIME, &absStime);
#endif

    if (piPodContext && pBuffer && bufferLength)
    {
        if(piPodContext->fd_hidraw >= 0)
        	{
            retval=write(piPodContext->fd_hidraw,pBuffer,bufferLength);
        	}
        else
        	
            ALOGD(" iPOD_HID_WriteHID fd_hidraw = %d\n",piPodContext->fd_hidraw);

        if(IAPHIDD2_DEBUG == 1){
            int len = pBuffer[3];
            int lenOffset = 0;
            int cmdOffset = 0;
            int lingoPos = 4;
            int cmdPos = 5;
            int transIdPos = 6;
            if(len == 0)
                lenOffset = 2;

            if(pBuffer[4 + lenOffset] == LINGO_EXTENDED_INTERFACE)
                cmdOffset = 1;

            lingoPos += lenOffset;
            cmdPos += lenOffset;
            transIdPos += (lenOffset + cmdOffset);
                
            if(bufferLength >= 7 ){
                if(Ipod_Lingo_type >= 0 ){
                    if(pBuffer[2] == 0x55 && pBuffer[lingoPos] == Ipod_Lingo_type){
                        if(Ipod_Lingo_cmd > 0){
                            if(Ipod_Lingo_type == LINGO_EXTENDED_INTERFACE ){
                                if(Ipod_Lingo_cmd == pBuffer[cmdPos + 1])
                                    iPOD_DumpMemory(pBuffer, bufferLength);
                            }else{
                                if(Ipod_Lingo_cmd == pBuffer[cmdPos])
                                    iPOD_DumpMemory(pBuffer, bufferLength);
                            }
                        }else{
                            iPOD_DumpMemory(pBuffer, bufferLength);
                        }
                    }
                }else{
                    iPOD_DumpMemory(pBuffer, bufferLength);
                }
            }
        }
#if 0
        if(bufferLength >= 6){
            if(pBuffer[2] == 0x55 && pBuffer[3] == 0x06 && pBuffer[4] == 0x0A && pBuffer[5]  == 0x00){          
                iPOD_DumpMemory(pBuffer, bufferLength);
            }
        }
#endif        
    }
    else
    {
        if(piPodContext == NULL)
            ALOGD(" iPOD_HID_WriteHID piPodContext == NULL\n");
		
        if(pBuffer == NULL)
            ALOGD(" iPOD_HID_WriteHID pBuffer == NULL\n");
        if(bufferLength <= 0)
            ALOGD(" iPOD_HID_WriteHID bufferLength = %d\n",bufferLength);
    }

#ifdef HID_TIME_CHECK
    //iPOD_DumpMemory(pBuffer, 20);
    if(pBuffer[4] == 0x03 && pBuffer[5] == 0x18)
    {
        absTime.tv_sec = 0;
        absTime.tv_nsec = 0;
    }
    clock_gettime(CLOCK_REALTIME, &absEtime);
    if(absEtime.tv_nsec < absStime.tv_nsec){
        absEtime.tv_sec -= 1;
        absEtime.tv_nsec += 1000*1000*1000;
    }

    absDtime.tv_sec =  absEtime.tv_sec - absStime.tv_sec;
    absDtime.tv_nsec = absEtime.tv_nsec - absStime.tv_nsec;

#endif

    if(retval < 0 && piPodContext->fd_hidraw >= 0)
    {
        ALOGD("[ERROR]:%s fd_hidraw %d retval  %d errno[%s] %d \n", __FUNCTION__,piPodContext->fd_hidraw, retval,strerror(errno), errno);
        close(piPodContext->fd_hidraw);
        piPodContext->fd_hidraw = -1;
        gHidrawFd = -1;
    }
	return retval;
}

IPOD_ERROR IPOD_SendPacket(void *HID_Handle, unsigned char *pBuffer, unsigned long length)
{
	unsigned long retval=0;	
	IPOD_ERROR ret=IPOD_NO_ERROR;
	// USB HID only
	if (HID_Handle && pBuffer && length)
	{
		static char tempBuffer[512];
		IPOD_HID_CONTEXT *piPodContext=(IPOD_HID_CONTEXT *)HID_Handle;
		ALOGD("%d %s write length:%ld\n",__LINE__,__FUNCTION__,length);
		retval=write(piPodContext->fd_hidraw, (const void *)pBuffer, (size_t)length);
		if ((long)retval == -1)
		{
			ALOGE("[ERROR]:%s(line=%d)wirte failed. error number is %d\n", __FUNCTION__, __LINE__, errno);
			ALOGE("[ERROR]:%s(line=%d)(fd_hidraw=0x%x)(pBuffer=%p)(length=%ld)(ret=0x%lx)",__FUNCTION__,__LINE__,piPodContext->fd_hidraw,pBuffer,length,retval);
			ret = IPOD_HID_WRITE_ERROR;
		}
		else
		{
			IAPHIDD_DBUG("%s retval:%ld written from %ld\n",__FUNCTION__, retval, length);
			ret = IPOD_NO_ERROR;
		}
	}
	else
	{
		ret = IPOD_INVALID_PARAM;
	}

	return retval;
}

IPOD_ERROR IPOD_HID_SendPacket(IPOD_HID_CONTEXT *iPodIOHandle, unsigned char *pBuffer,long length)
{
	IPOD_HID_CONTEXT *piPodHidHandle = (IPOD_HID_CONTEXT *)iPodIOHandle;
	IPOD_ERROR ret;
#if 1
	int retval=-1;
	int reportLength;
	unsigned char ucReportID = 0;
	unsigned char *pSendBuffer;
	unsigned char *pSendBufferBase;
	int partialNum;
	int mallocSize;
#endif
#if 1
        int fSetDspImage = 0;
        if(length >= 7){
            int lingoPos = 4;
            int cmdPos = lingoPos + 1;
            int cmd = 0;
            int lingo = pBuffer[lingoPos];
            if(pBuffer[0] == 0x55)
            {
                if(lingo== LINGO_EXTENDED_INTERFACE){
                    cmd = pBuffer[cmdPos + 1];
                    if(cmd == 0x32){
                         fSetDspImage = 1;
                        // IAPHID_DBUG(" %s cmd SetDspImage %l ",__FUNCTION__,length);
                    }
                }
            }            
        }

		IAPHIDD_DBUG("__length=%ld\n",length);
		
		reportLength=iPOD_GetReportIDToSend(iPodIOHandle,length+1,&ucReportID);
		
		IAPHIDD_DBUG("Report ID : %d, Length = %d\n",ucReportID,reportLength);
		if (ucReportID && reportLength) {
			
			//malloc_size = reportLength/4;
			//if(reportLength%4)
			//{
			//	malloc_size+=1;
			//}
			//IAPHID_DBUG("malloc size : %d, report lentrh : %d real malloc siae : %d \n", malloc_size, reportLength, sizeof(int)*malloc_size);
			//pSendBufferBase=(unsigned char *)IPOD_MemAlloc(sizeof(int)*malloc_size);
			partialNum = length/(reportLength-1);
			if(length%(reportLength-1))
			{
				partialNum++;
			}
			mallocSize = (reportLength+1)*partialNum;
			IAPHIDD_DBUG("	malloc lentgh=%d, partialNum=%d  \n",mallocSize,partialNum);
			
			pSendBufferBase=(unsigned char *)IPOD_MemAlloc(partialNum+2048*4);
			
			IAPHIDD_DBUG("	>> Memory alloc Address = %p \n",pSendBufferBase);
			if (pSendBufferBase) {
				int len;
				int flag=0;
				pSendBuffer=pSendBufferBase;
				do {
					len=reportLength-1;
					if (len>length) {
						len=length;
					}
					*pSendBuffer=ucReportID;
					(void)IPOD_MemCopy(pSendBuffer+2,pBuffer,len);
					IPOD_MemSet(pSendBuffer+2+len,0,reportLength-1-len);
					IAPHIDD_DBUG("		>> !! reportlength-1-len = %d\n",reportLength-1-len);
					length-=len;
					pSendBuffer[1]=0;
					if (flag) {
						pSendBuffer[1]|=1;
					}
					if (length>0) {
						pSendBuffer[1]|=2;
						flag=1;
					}
					
					retval=iPOD_HID_WriteHID(piPodHidHandle,pSendBuffer,reportLength+1);//KDH
					
			//		ALOGD("HID sent : Length = %d\n",retval);
					if (retval==(reportLength+1)) {
						// success
					//	IAPHIDD_DBUG("%s(), Line=%d\n",__FUNCTION__,__LINE__);
						pBuffer+=len;
						//IPOD_DumpMemory(pSendBuffer,retval);
					} else {
						// fail
						//ALOGI(" iPOD_HID_WriteHID retval = %d\n",retval);
						ret = IPOD_HID_WRITE_ERROR;
						IPOD_MemDealloc(pSendBufferBase);
						return ret;
					}
					pSendBuffer+=(reportLength+1);
					if(length>0 && fSetDspImage == 1){
						usleep(500);
					}
				} while(length>0);
				IAPHIDD_DBUG("%s(), Line=%d\n",__FUNCTION__,__LINE__);
				IAPHIDD_DBUG("	<< Memory dealloc Address = %p \n",pSendBufferBase);	
				
				IPOD_MemDealloc(pSendBufferBase);

				if(length == 0)
					ret = IPOD_NO_ERROR;
				
				IAPHIDD_DBUG("%s(), Line=%d\n",__FUNCTION__,__LINE__);
			}
		}
		IAPHIDD_DBUG("HID send finished\n");

#endif

	if(length > 0)
		{
		
		ret = IPOD_SendPacket(piPodHidHandle, pBuffer, length);
		
		}
	IAPHIDD_DBUG("HID send IPOD_SendPacket ret = %d\n",ret);
	return ret;
}

void IPOD_HID_ReadPacketCB(IPOD_IO_HANDLE *iPodIOHandle, unsigned char * HidRcvBuffer, unsigned long BufferSize, unsigned long *HidDataReadSize)
{
	IPOD_HID_CONTEXT *piPodHidHandle = (IPOD_HID_CONTEXT *)iPodIOHandle->iPodHIDHandle;
	long ErrorCnt = 0;
	int ret;
        

	if(iPodIOHandle&&HidRcvBuffer&&HidDataReadSize&&BufferSize)
	{
		ret=iPOD_HID_ReadHID(piPodHidHandle, HidRcvBuffer, BufferSize);

		if (ret>0)
		{
			if(HidDataReadSize != NULL)
			{
				*HidDataReadSize=ret;
				ErrorCnt = 0;
				// header 2byte
				//IPOD_RX_Put_Buffer(&HidRcvBuffer[2],(ret-2));
			}
			else
			{
				ALOGE("invalid parameter : %s , line=%d\n", __FUNCTION__, __LINE__);
			}
		}
		else
		{
			*HidDataReadSize = -1;
			ErrorCnt ++;
			usleep(100);
		}
	}
	else
	{
		ALOGE("invalid parameter   : %s , line=%d\n", __FUNCTION__, __LINE__);
	}
}
#if 0
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



int main(int argc, char **argv){
	ALOGD("main start \n");
	g_handle = (IPOD_HID_CONTEXT *)IPOD_HID_Init();
	IPOD_HID_CheckiPodDevice(g_handle);
	IPOD_HID_SetHIDChannel(g_handle,"/dev/hidraw0",-1);

		  ALOGD("main start 111 \N");
	 ALOGD("g_handleg_handle->HIDReportCount =%d line:%d\n",g_handle->HIDReportCount,__LINE__);
	pthread_t write_thead;
	ALOGD("main start 333 \n");

	  int ret = pthread_create(&write_thead, NULL, iAP2TransportReadThread,
        		g_handle);
	  ALOGD("main start 111 ret=%d \n",ret);

		pthread_t read_thead;
	

	   ret = pthread_create(&read_thead, NULL, iAP2TransportWriteThread,
        		g_handle);

	 ALOGD("main start 2222 ret=%d \n",ret);

     while(1)
     	{
     	  sleep(1);
     	}
		
	//usb_claim_interface();

return 0;
}
#endif

