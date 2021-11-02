/****************************************************************************
 *   FileName    : iAP2EaNativeTransport.h
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

#ifndef _IAP2_EA_NATIVE_TRANSPORT_H_
#define _IAP2_EA_NATIVE_TRANSPORT_H_

typedef void (*iAP2EaNativeNotify_callback)(int what,int arg1,int arg2);
typedef void *(*iAP2EaNativeAttached_callback)(void *pEnv, bool *pAttached);
typedef int (*iAP2EaNativeDoRead_callback)(void *pEnv,int fd);
typedef void (*iAP2EaNativeDetached_callback)(int attached);
typedef void (*iAP2EaNativeReadDone_callback)(void *pEnv,void *buffer, int readLength);

class iAP2EaNativeTransport
{
    int mInited;
    static bool mDebug;

    static iAP2EaNativeNotify_callback mNotify_cb;
    static iAP2EaNativeAttached_callback mAttached_cb;
    static iAP2EaNativeDoRead_callback mDoRead_cb;
    static iAP2EaNativeDetached_callback mDetached_cb;
    static iAP2EaNativeReadDone_callback mReadDone_cb;    
    
    static bool bDestroy;
    static bool bConnected;
    static pthread_mutex_t mSyncMutex;
    static pthread_cond_t  mSyncCond;       
    static void *mvEaNativeHal;
    pthread_t mThread;

public:
    iAP2EaNativeTransport(
                                    iAP2EaNativeNotify_callback notify_cb,
                                    iAP2EaNativeAttached_callback attached_cb,
                                    iAP2EaNativeDoRead_callback doRead_cb,
                                    iAP2EaNativeDetached_callback detached_cb,
                                    iAP2EaNativeReadDone_callback readDone_cb,bool debug);
    ~iAP2EaNativeTransport();
    void setDebug(bool debug);
    void setConnected(bool flag);
    int create(void);
    int destroy(void);
    int writeEaNative(void *buffer, int size);
    static void * readRun(void *arg);

};

#endif /*_IAP2_EA_NATIVE_TRANSPORT_H_*/

