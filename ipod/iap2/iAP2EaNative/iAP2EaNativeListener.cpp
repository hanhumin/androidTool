/****************************************************************************
 *   FileName    : iAP2EaNativeListener.cpp
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
#include "iAP2EaNativeListener.h"
//#include "iAP2EaNativeManager.h"
#include <utils/Log.h>

#define LOG_TAG "iAP2EaNative"

iAP2EaNativeListener::iAP2EaNativeListener(iAP2EaNativeTransportNotify_callback notify_cb,
                                    iAP2EaNativeTransportAttached_callback attached_cb,
                                    iAP2EaNativeTransportDoRead_callback doRead_cb,
                                    iAP2EaNativeTransportDetached_callback detached_cb,
                                    iAP2EaNativeTransportReadDone_callback readDone_cb,
                                    bool debug)
{
    mDebug= debug;
    mpEnv = NULL;
    mAttached = 0;    
    mNotify_cb = NULL;
    mAttached_cb = NULL;
    mDoRead_cb = NULL;
    mDetached_cb = NULL;
    mReadDone_cb = NULL;
    mNotify_cb = notify_cb;
    mAttached_cb = attached_cb;
    mDoRead_cb = doRead_cb;
    mDetached_cb = detached_cb;
    mReadDone_cb = readDone_cb;
}

iAP2EaNativeListener::~iAP2EaNativeListener()
{
    if(mDebug) ALOGI( "%s start", __FUNCTION__);
    detached();
    mpEnv = NULL;
    mAttached = false;    
    mNotify_cb = NULL;
    mAttached_cb = NULL;
    mDoRead_cb = NULL;
    mDetached_cb = NULL;
    mReadDone_cb = NULL;
    if(mDebug) ALOGI( "%s end", __FUNCTION__);
}

void iAP2EaNativeListener::setDebug(bool debug)
{
    mDebug = debug;
}

bool iAP2EaNativeListener::isSupportRead()
{
    return (mDoRead_cb != NULL)? true:false;
}

int iAP2EaNativeListener::attached()
{
    int ret = 0;
    if(mDebug) ALOGI("%s",__FUNCTION__);
    if(mAttached_cb != NULL)
        mpEnv = mAttached_cb(mpEnv,&mAttached);
    return ret;
}

int iAP2EaNativeListener::doRead(int fd)
{
    int ret = 0;
    if(mDoRead_cb != NULL)
        ret = mDoRead_cb(mpEnv,fd);
    return ret;
        
}
void iAP2EaNativeListener::detached()
{
    if(mDebug) ALOGI("%s mAttached%d",__FUNCTION__,mAttached);
    if(mAttached)
    {
        if(mDetached_cb)
            mDetached_cb(mAttached);
        else
            ALOGE("%s mDetached_cb == null ",__FUNCTION__);
    }
    mpEnv = NULL;
    mAttached = false;
}
void iAP2EaNativeListener::readDone(void *buffer,int len)
{
    if(mReadDone_cb != NULL && mDoRead_cb == NULL)
    {
        mReadDone_cb(buffer,len);
    }
    else
        ALOGE("%s mReadDone_cb == null or mDoRead_cb doesn't null",__FUNCTION__);
}
void iAP2EaNativeListener::notify(int type,bool doAttached)
{
    if(mNotify_cb !=NULL)
        mNotify_cb(type,doAttached);
    else
        ALOGE("%s notify is null",__FUNCTION__);
}
