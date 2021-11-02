/****************************************************************************
 *   FileName    : iAP2EaNativeCustom.cpp
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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils/Log.h>
#include <errno.h>

#include "iAP2EaNativeCustomTest.h"
#include "iAP2EaNativeCustom.h"

#define LOG_TAG "EaNativeCustom"

using namespace android;

int gEaNativeMangerHandler = 0;
iAP2EaNativeCustom_WriteCallback gEaNativeCustomWriteCb = NULL;

void iAP2EaNativeCustom_SetWriteCallback(iAP2EaNativeCustom_WriteCallback callback)
{
    gEaNativeCustomWriteCb = callback;
}

void iAP2EaNativeCustom_Notify(int handle, int what,int arg1,int arg2)
{
    ALOGI("%s what %d , arg1 %d arg2 %d ",__FUNCTION__,what,arg1,arg2);
    switch(what)
    {
        case IAP2_NOTI_EAP_NATIVE_USB_DISCONNECTED:
            gEaNativeMangerHandler = 0;
            break;
        case IAP2_NOTI_EAP_NATIVE_USB_CONNECTED:
            gEaNativeMangerHandler = 0;
            break;
        case IAP2_NOTI_EAP_NATIVE_START:
            gEaNativeMangerHandler = 0;
            break;
        case IAP2_NOTI_EAP_NATIVE_OPENED:
            gEaNativeMangerHandler = handle;
            // for test
            iAP2EaNativeCustomTest_SendTest(); 
            break;
        case IAP2_NOTI_EAP_NATIVE_IO_ERROR:
            gEaNativeMangerHandler = 0;
            break;
        case IAP2_NOTI_EAP_NATIVE_STOP:
            gEaNativeMangerHandler = 0;
            break;
    }
}

void iAP2EaNativeCustom_ReadDone(void *buffer, int readLength)
{
    // for test
    iAP2EaNativeCustomTest_ReadDone(buffer,readLength); 
}
void iAP2EaNativeCustom_Write(void *buffer, int writeLength)
{
    if(gEaNativeCustomWriteCb != NULL)
    {
        if(gEaNativeMangerHandler != 0)
            gEaNativeCustomWriteCb(gEaNativeMangerHandler,buffer,writeLength);
        else
            ALOGE("%s gEaNativeMangerHandler %d",__FUNCTION__,gEaNativeMangerHandler);
    }
    else
        ALOGE("%s gEaNativeCustomWriteCb is null",__FUNCTION__);
}

