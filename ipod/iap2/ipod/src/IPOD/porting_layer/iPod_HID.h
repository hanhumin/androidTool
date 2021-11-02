/****************************************************************************
 *   FileName    : iPod_HID.h
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


#ifndef _IPOD_HID_H_
#define _IPOD_HID_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#include "../iPod_lib.h"
typedef struct
{
	int fd_hidraw;
	void *usb_dev_handle;

	unsigned char HIDReportCount;
	struct
	{
		unsigned char ID;
		unsigned int length;
		unsigned char direction;
	} HIDReport[HID_REPORT_MAX];
		
} IPOD_HID_CONTEXT;
extern IPOD_ERROR IPOD_SendPacket(void *HID_Handle, unsigned char *pBuffer, unsigned long length);
IPOD_ERROR IPOD_HID_SendPacket(IPOD_HID_CONTEXT *iPodIOHandle, unsigned char *pBuffer,long length);
extern void IPOD_HID_ReadPacketCB(IPOD_IO_HANDLE *iPodIOHandle, unsigned char *HidRcvBuffer, unsigned long BufferSize, unsigned long *HidDataReadSize);
extern void IPOD_HID_InitDebug(void);
extern void *IPOD_HID_Init(void);
extern void iPOD_HID_Deinit(void *piPodPALHandle);
extern int IPOD_HID_CheckiPodDevice(void *piPodPALHandle);
extern void iPOD_HID_InitChannelParam(pIPOD_INIT_CHANNER_PARAM pParam, void *pHandle);
extern void iPOD_HID_CloseiPodDevice(void* piPodPALHandle);
extern void *iPOD_HID_FindiPodDevice(int *cfg_num, int *iface_num,int *num_interface);
extern int IPOD_HID_SetHIDChannel(IPOD_HID_CONTEXT *piPodHIDContext,/*const char *path,*/ unsigned int block);
extern int IPOD_SetHIDChannel(void *piPodPALHandle,const char *path);
extern unsigned char *iPOD_HID_GetHIDReportDescriptorInfo(void *piPodPALHandle,int *pLength);
extern void IPOD_HID_CloseHIDChannel(void *piPodPALHandle);
extern int iPOD_HID_ReadHID(void *piPodPALHandle,unsigned char *pBuffer, int bufferLength);
extern int iPOD_HID_WriteHID(void *piPodPALHandle,unsigned char *pBuffer, int bufferLength);
extern void iPOD_DumpMemory(unsigned char *pucBuffer,int length);
extern int IPOD_HID_IsExistHid(char *file_name);
extern IPOD_HID_CONTEXT *g_handle;
int IPOD_HID_CheckiPodDevice2();

#if defined(__cplusplus)
};
#endif
#endif /* _IPOD_HID_H_ */ 

