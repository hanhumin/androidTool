/****************************************************************************
 *   FileName    : iAP2EaNativeManager.cpp
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
#include <cutils/properties.h>

#include "iAP2EaNativeCustom.h"
#include "iAP2EaNativeTransport.h"
#include "iAP2EaNativeReceiver.h"
#include "iAP2EaNativeManager.h"

using namespace android;

#define LOG_TAG "iAP2EaNative"
#define EANATIVE_VERSION "V161025.1"

bool iAP2EaNativeManager::mDebug;
int iAP2EaNativeManager::mScenario;
iAP2EaNativeTransport *iAP2EaNativeManager::mvTransport;
iAP2EaNativeReceiver *iAP2EaNativeManager::mvEventReceiver;

iAP2EaNativeNotify_callback iAP2EaNativeManager::mNotify_cb;
iAP2EaNativeAttached_callback iAP2EaNativeManager::mAttached_cb;
iAP2EaNativeDoRead_callback iAP2EaNativeManager::mDoRead_cb;
iAP2EaNativeDetached_callback iAP2EaNativeManager::mDetached_cb;
iAP2EaNativeReadDone_callback iAP2EaNativeManager::mReadDone_cb; 

pthread_mutex_t iAP2EaNativeManager::mSyncMutex;

iAP2EaNativeManager::iAP2EaNativeManager(iAP2EaNativeNotify_callback notify_cb,
                                    iAP2EaNativeAttached_callback attached_cb,
                                    iAP2EaNativeDoRead_callback doRead_cb,
                                    iAP2EaNativeDetached_callback detached_cb,
                                    iAP2EaNativeReadDone_callback readDone_cb)
{
    ALOGI( "%s start %s", __FUNCTION__,EANATIVE_VERSION);

    mDebug = false;
    checkLog();

    mNotify_cb = notify_cb;
    mAttached_cb = attached_cb;
    mDoRead_cb = doRead_cb;
    mDetached_cb = detached_cb;
    mReadDone_cb = readDone_cb;    

    pthread_mutex_init(&mSyncMutex, NULL);

    ALOGI( "%s end", __FUNCTION__);
}

iAP2EaNativeManager::~iAP2EaNativeManager()
{
    if(mDebug) ALOGI( "%s start", __FUNCTION__);
    
    if(mvEventReceiver != NULL)
    {
        mvEventReceiver->destroy();
        delete mvEventReceiver;
    }
    mvEventReceiver = NULL;    

    if(mvTransport != NULL)
    {
        mvTransport->destroy();
        delete mvTransport;
    }
    mvTransport = NULL;    

    pthread_mutex_init(&mSyncMutex, NULL);

    ALOGI( "%s end", __FUNCTION__);
}

void iAP2EaNativeManager::checkLog(void)
{
    char iapDebug[PROPERTY_VALUE_MAX];
    property_get("persist.iap2.debug.eanative", iapDebug, "f");
   
    switch(iapDebug[0]){
        case 't':
        case 'T':
            mDebug = true;
            break;
        default:
            mDebug = false;;
            break;
    }
}

void iAP2EaNativeManager::notifyCallback(int what,int arg1,int arg2)
{
    pthread_mutex_lock(&mSyncMutex);
    if(mNotify_cb != NULL)
        mNotify_cb(what,arg1,arg2);
    pthread_mutex_unlock(&mSyncMutex);
}

void iAP2EaNativeManager::setScenario(int scenario)
{
    mScenario = scenario;
}

int iAP2EaNativeManager::getScenario()
{
    return mScenario;
}

void iAP2EaNativeManager::disconnected()
{
    if(mvTransport != NULL)
        mvTransport->setConnected(false);
    else
        ALOGE("%s mvTransport is null (start)",__FUNCTION__);
}

void iAP2EaNativeManager::createReceiver()
{
    if(mDebug) ALOGI( "%s", __FUNCTION__);
    if(mvEventReceiver != NULL)
        destroyReceiver();
    mvEventReceiver = new  iAP2EaNativeReceiver(notifyCallback,mDebug);
    mvEventReceiver->create();
    if(mDebug) ALOGI( "%s end", __FUNCTION__);
}

void iAP2EaNativeManager::destroyReceiver()
{
    if(mDebug) ALOGI( "%s", __FUNCTION__);
    if(mvEventReceiver != NULL)
    {
        mvEventReceiver->destroy();
        delete mvEventReceiver;
    }
    mvEventReceiver = NULL;
    if(mDebug) ALOGI( "%s end", __FUNCTION__);
}

void iAP2EaNativeManager::createTransport()
{
    if(mDebug) ALOGI( "%s", __FUNCTION__);
    if(mvTransport != NULL)
        destroyTransport();
    mvTransport = new iAP2EaNativeTransport(notifyCallback,mAttached_cb,mDoRead_cb,mDetached_cb,mReadDone_cb,mDebug);
    if(mvTransport != NULL)
        mvTransport->create();
    else
        ALOGE("%s mvTransport is null (start)",__FUNCTION__);

    if(mDebug) ALOGI( "%s end", __FUNCTION__);
}

void iAP2EaNativeManager::destroyTransport()
{
    if(mDebug) ALOGI( "%s", __FUNCTION__);
    if(mvTransport != NULL)
    {
        mvTransport->destroy();
        delete mvTransport;
    }
    mvTransport = NULL;
    if(mDebug) ALOGI( "%s end", __FUNCTION__);
}

void iAP2EaNativeManager::updateDebug(void)
{
    if(mvTransport != NULL)
        mvTransport->setDebug(mDebug);
    if(mvEventReceiver != NULL)
        mvEventReceiver->setDebug(mDebug);
}

int iAP2EaNativeManager::writeEaNative(void *buffer,int size)
{
    int ret = -1;
    if(mDebug) ALOGI( "%s buffer %p size %d", __FUNCTION__,buffer,size);
    if(mvTransport != NULL)
        ret = mvTransport->writeEaNative(buffer,size);
    else
        ALOGE("%s mvTransport is null",__FUNCTION__);
    
    return ret;
}

