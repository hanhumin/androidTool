/****************************************************************************
 *   FileName    : iAP2EaNativeHal.cpp
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
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <cutils/properties.h>

#include "iAP2EaNativeHal.h"
#include <utils/Log.h>

#define LOG_TAG "iAP2EaNative"

#define DEV_IAP2_EA_NATIVE            "/dev/iap2_ext_acc"

iAP2EaNativeHal::iAP2EaNativeHal(bool debug)
{
    setDebug(debug);
    if(mDebug) ALOGI("%s start",__FUNCTION__);
    mFd = -1;
    mRead = false;
    mWrite = false;
    mDumpBuffer = NULL;
    mDumpBuffer = (uint8_t *)malloc(EA_NATIVE_DUMP_BUFFER_SIZE);
    if(mDebug) ALOGI("%s end",__FUNCTION__);
}

iAP2EaNativeHal::~iAP2EaNativeHal()
{
    if(mDebug) 
        ALOGI("%s start",__FUNCTION__);
    closeEaNative();
    if(mDumpBuffer != NULL)
        free(mDumpBuffer);
    mDumpBuffer = NULL;
    if(mDebug) 
        ALOGI("%s end",__FUNCTION__);
}

void iAP2EaNativeHal::setDebug(bool debug)
{
    char iapDebug[PROPERTY_VALUE_MAX];

    mDebug = debug;
    property_get("persist.iap2.dump.eanative", iapDebug, "f");
   
    switch(iapDebug[0]){
        case 't':
        case 'T':
            mDebug = true;
            break;
        default:
            mDebug = true;;
            break;
    }    
}

#if 0
iAP2EaNativeHal *iAP2EaNativeHal::getInstance() 
{
    if (instance == NULL)
    {
        instance = new iAP2EaNativeHal();
    }
    return instance;
}
#endif

int iAP2EaNativeHal::openEaNative(bool *opened)
{
    int fd = -1;
    *opened = false;
    //closeEaNative();
    if(mFd >= 0)
        return mFd;

    mRead = false;
    mWrite = false;
    fd = open(DEV_IAP2_EA_NATIVE, O_RDWR|O_NONBLOCK);
    if (fd < 0) {
        mFd = -1;
        ALOGE(" %s open failed fd = %d \n",__FUNCTION__,mFd);
        return fd;
    }
    mFd = fd;
    *opened = true;
    ALOGD("%s open fd %d", __FUNCTION__,mFd );
    return fd;
}
int iAP2EaNativeHal::closeEaNative(void)
{
    if(mRead)
    {
        ALOGE("%s mRead is true", __FUNCTION__ );
    }

    if(mWrite)
    {
        ALOGE("%s mWrite is true", __FUNCTION__ );
    }
    ALOGD("%s fd %d", __FUNCTION__, mFd);
    if(mFd >= 0)
    {
        close(mFd);
        mFd = -1;
    }
    mRead = false;
    mWrite = false;
    return 0;
}

int iAP2EaNativeHal::getFd()
{
    return mFd;
}

void iAP2EaNativeHal::dumpEa(const char* data, int dataLen)
{
    int i;
    int j;
    char *buf= (char *)mDumpBuffer;
    int length = dataLen;

    if (length>0) {
        ALOGD("  ------------< Dump start >------------------ %p \n",buf);
        for (i=0;i<length;i+=16) {
            int index = 0;
            index += sprintf(&buf[index]," %04X: ",i);
            for (j=0;j<16;j++) {
                if ((i+j)<length) {
                    index += sprintf(&buf[index]," %02X",data[i+j]);
                } else {
                    index += sprintf(&buf[index],"   ");
                }
            }
            index += sprintf(&buf[index]," : ");
            for (j=0;j<16;j++) {
                if ((i+j)<length) {
                    index += sprintf(&buf[index],"%c",(data[i+j]>' ')?data[i+j]:'.');
                } else {
                    break;
                }
            }

            ALOGD("  %s\n",buf);
           
        }
        ALOGD(" ------------< Dump end >------------------ \n");
    }
}

int iAP2EaNativeHal::writeEaNative(void* buffer, int size)
{
    int ret = 0;
    
    if(mFd < 0)
    {
        ALOGE("%s invalid fd %d", __FUNCTION__,mFd );
        return -1;
    }
    if(mDebug) ALOGD("%s buffer %p, size %d",__FUNCTION__,buffer,size);
    mWrite = true;
    ret = write(mFd, buffer, size);
    mWrite = false;
    if(ret < 0)
    {
        ALOGE("%s write error ret = %d errno = %d", __FUNCTION__, ret,errno);
    }
    else
    {
        if(mDumpDebug) dumpEa((const char *)buffer,size);
    }
    return ret;
}

int iAP2EaNativeHal::readEaNative(void* buffer, int size)
{
    int ret = 0;
    if(mFd < 0)
    {
        ALOGE("%s invalid fd", __FUNCTION__ );
        return -1;
    }
    mRead = true;
    ret = read(mFd, buffer, size);
    mRead = false;
    if(mDebug) ALOGD("%s buffer %p, ret %d",__FUNCTION__,buffer,ret);
    if(ret < 0)
    {
        ALOGE("%s write error ret = %d errno = %d", __FUNCTION__, ret,errno);
    }
    else
        if(mDumpDebug) dumpEa((const char *)buffer,ret);
    return ret;
}

