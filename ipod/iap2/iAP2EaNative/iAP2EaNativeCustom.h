/****************************************************************************
 *   FileName    : iAP2EaNativeCustom.h
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

#ifndef _IAP2_EA_NATIVE_CUSTOM_H_
#define _IAP2_EA_NATIVE_CUSTOM_H_

enum{
    IAP2_NOTI_EAP_NATIVE_USB_DISCONNECTED = 110,   //3 // this enum must not modify.
    IAP2_NOTI_EAP_NATIVE_USB_CONNECTED = 111, //3  // this enum must not modify.
    IAP2_NOTI_EAP_NATIVE_START = 112, //3  // this enum must not modify.
    IAP2_NOTI_EAP_NATIVE_OPENED = 113, //3 // this enum must not modify.
    IAP2_NOTI_EAP_NATIVE_DATA = 114,   //3 // this enum must not modify.
    IAP2_NOTI_EAP_NATIVE_IO_ERROR = 115, //3 // this enum must not modify.
    IAP2_NOTI_EAP_NATIVE_STOP = 116,   //3 // this enum must not modify.
};

enum{
    IAP2_EA_SCENARIO_IAP2SERVICE = 0,
    IAP2_EA_SCENARIO_IN_APPLICATION_WITH_JAVA = 1,
    IAP2_EA_SCENARIO_IN_APPLICATION_WITH_NATIVE = 2,
};

typedef int (*iAP2EaNativeCustom_WriteCallback)(int handle, void *buffer,int size);

void iAP2EaNativeCustom_SetWriteCallback(iAP2EaNativeCustom_WriteCallback callback);
void iAP2EaNativeCustom_Notify(int handle,int what,int arg1,int arg2);
void iAP2EaNativeCustom_ReadDone(void *buffer, int readLength);
void iAP2EaNativeCustom_Write(void *buffer, int readLength);

#endif

