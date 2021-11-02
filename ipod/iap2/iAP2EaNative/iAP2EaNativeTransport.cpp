/****************************************************************************
 *   FileName    : iAP2EaNativeTransport.cpp
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
#include <utils/Log.h>
#include <errno.h>

#include "iAP2EaNativeCustom.h"
#include "iAP2EaNativeHal.h"
#include "iAP2EaNativeTransport.h"

#define LOG_TAG "iAP2EaNative"

using namespace android;

bool iAP2EaNativeTransport::mDebug;
void *iAP2EaNativeTransport::mvEaNativeHal;
pthread_mutex_t iAP2EaNativeTransport::mSyncMutex;
pthread_cond_t  iAP2EaNativeTransport::mSyncCond;      
bool iAP2EaNativeTransport::bDestroy;
bool iAP2EaNativeTransport::bConnected;

iAP2EaNativeNotify_callback iAP2EaNativeTransport::mNotify_cb;
iAP2EaNativeAttached_callback iAP2EaNativeTransport::mAttached_cb;
iAP2EaNativeDoRead_callback iAP2EaNativeTransport::mDoRead_cb;
iAP2EaNativeDetached_callback iAP2EaNativeTransport::mDetached_cb;
iAP2EaNativeReadDone_callback iAP2EaNativeTransport::mReadDone_cb;    

uint8_t gTransportBuffer[EA_NATIVE_BULK_SIZE];

iAP2EaNativeTransport::iAP2EaNativeTransport(//int scenario, 
                                    iAP2EaNativeNotify_callback notify_cb,
                                    iAP2EaNativeAttached_callback attached_cb,
                                    iAP2EaNativeDoRead_callback doRead_cb,
                                    iAP2EaNativeDetached_callback detached_cb,
                                    iAP2EaNativeReadDone_callback readDone_cb,bool debug)
{
    mDebug = debug;
    ALOGI( "%s start", __FUNCTION__);
    bDestroy = false;
    bConnected = false;

    mNotify_cb = notify_cb;
    mAttached_cb = attached_cb;
    mDoRead_cb = doRead_cb;
    mDetached_cb = detached_cb;
    mReadDone_cb = readDone_cb;    
    
    mvEaNativeHal = (void *)(new iAP2EaNativeHal(mDebug));
    
    pthread_mutex_init(&mSyncMutex, NULL);
    pthread_cond_init(&mSyncCond, NULL);    
    ALOGI( "%s end", __FUNCTION__);
}

iAP2EaNativeTransport::~iAP2EaNativeTransport()
{
    ALOGI( "%s start", __FUNCTION__);
    destroy();
    pthread_cond_destroy(&mSyncCond);
    pthread_mutex_destroy(&mSyncMutex);
    if(mvEaNativeHal != NULL)
        delete (iAP2EaNativeHal *)mvEaNativeHal;
    mvEaNativeHal = NULL;
    ALOGI( "%s end", __FUNCTION__);
}

void iAP2EaNativeTransport::setDebug(bool debug)
{
    mDebug = debug;
}

void iAP2EaNativeTransport::setConnected(bool flag)
{
    bConnected = flag;
}

int iAP2EaNativeTransport::create(void)
{
    int ret = 0;
    iAP2EaNativeHal *eaNativeHal = (iAP2EaNativeHal *)mvEaNativeHal;
    bDestroy = false;

    
    pthread_mutex_lock(&mSyncMutex);
    ret = pthread_create(&mThread, NULL, readRun, NULL);
    pthread_cond_wait(&mSyncCond, &mSyncMutex);
    pthread_mutex_unlock(&mSyncMutex);
    if (0 != ret) 
    {
        ret = -1;
    }
    if(ret != 0)
        eaNativeHal->closeEaNative();
    return ret;
}

int iAP2EaNativeTransport::destroy(void)
{
    int r;
    bDestroy = true;
    if(mThread)
        pthread_join(mThread, NULL);
    mThread = 0;
    bDestroy = false;
    return 0;
}

int iAP2EaNativeTransport::writeEaNative(void * buffer, int size)
{
    int i;
    int ret = 0;
    uint8_t *pSendData = (uint8_t *)buffer;
    int remainSize = size;
    int writeSize = 0;
    int position = 0;
    iAP2EaNativeHal *eaNativeHal = (iAP2EaNativeHal *)mvEaNativeHal;
    while(remainSize > 0)
    {
        writeSize = remainSize;
        if(writeSize > EA_NATIVE_BULK_SIZE)
            writeSize = EA_NATIVE_BULK_SIZE;
        ret = eaNativeHal->writeEaNative((void *)&pSendData[position], writeSize);

        if(ret <= 0)
        {
            break;
        }
        remainSize -= writeSize;
        position+= writeSize;
    }
    return ret;
}

void * iAP2EaNativeTransport::readRun(void *arg)
{
    iAP2EaNativeHal *eaNativeHal = (iAP2EaNativeHal *)mvEaNativeHal;
    int len;
    void *buffer = (void *)gTransportBuffer;
    int bufferLen = EA_NATIVE_BULK_SIZE;
    bool attached = false;
    void *pEnv = NULL;
    bool opened =false;

    ALOGI("%s Thread Start \n",__FUNCTION__);
    /* silence compiler warning */
    (void) arg;

    if(mAttached_cb)
        pEnv = mAttached_cb(pEnv,&attached);

    pthread_mutex_lock(&mSyncMutex);
    pthread_cond_signal(&mSyncCond);
    pthread_mutex_unlock(&mSyncMutex);

    while (bDestroy == false ) 
    {
        if (eaNativeHal->openEaNative(&opened) < 0) {
            if(bConnected == false)
                break;
            usleep(1000 * 10);
            continue;
        }

        if(opened == true)
        {
            if(mNotify_cb != NULL)
                mNotify_cb(IAP2_NOTI_EAP_NATIVE_OPENED,0,0);
        }

        #if 0
        if(mSendStartNotify)
        {
            mSendStartNotify = false;
            mvListener->notify(IAP2_NOTI_EAP_NATIVE_START,true);
        }
        #endif

        //ALOGI("Transport Read Thread\n");
        if(mDoRead_cb)
           len = mDoRead_cb(pEnv,eaNativeHal->getFd());
        else
           len = eaNativeHal->readEaNative(buffer, bufferLen);
        ALOGI("Transport Thread read done len %d\n",len);
        
        if(len < 0 && bDestroy == true){
            ALOGI(" %s %d return error after disconnect \n",__FUNCTION__,__LINE__);
        }
        
        if (len < 0) {
            ALOGI(" %s %d return len %d \n",__FUNCTION__,__LINE__,len);
            if (errno == EIO || errno == ENODEV) {
                ALOGE(" %s %d return errno %d from the read function\n",__FUNCTION__,__LINE__,errno);
                if(bDestroy == false && mNotify_cb){
                    mNotify_cb(IAP2_NOTI_EAP_NATIVE_IO_ERROR,0,0);
                }
                eaNativeHal->closeEaNative();
                ALOGE("Transport Thread intruppt \n");
                bDestroy = true;
                goto done;
            }               

            ALOGE("Transport Thread read errno %d\n",errno);
        }

        /* parse DATA & Packet */
        if(bDestroy == false && len > 0)
        {
            if(mReadDone_cb)
            {
                mReadDone_cb(pEnv,buffer, len);
            }
        }
    }
done:   
    if(mDetached_cb)
        mDetached_cb(attached);
    ALOGI("%s stop by flag \n",__FUNCTION__);
    eaNativeHal->closeEaNative();
    pthread_exit((void *) 0);

    return NULL;
}

