/****************************************************************************
 *   FileName    : iAP2EaNativeManager.h
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

#ifndef _IAP2_EA_NATIVE_MANAGER_H_
#define _IAP2_EA_NATIVE_MANAGER_H_

class iAP2EaNativeManager 
{
    static bool mDebug;
    static int mScenario;

    static iAP2EaNativeTransport *mvTransport;
    static iAP2EaNativeReceiver *mvEventReceiver;

    static iAP2EaNativeNotify_callback mNotify_cb;
    static iAP2EaNativeAttached_callback mAttached_cb;
    static iAP2EaNativeDoRead_callback mDoRead_cb;
    static iAP2EaNativeDetached_callback mDetached_cb;
    static iAP2EaNativeReadDone_callback mReadDone_cb;    

    static pthread_mutex_t mSyncMutex;

public:
    iAP2EaNativeManager(iAP2EaNativeNotify_callback notify_cb,
                                    iAP2EaNativeAttached_callback attached_cb,
                                    iAP2EaNativeDoRead_callback doRead_cb,
                                    iAP2EaNativeDetached_callback detached_cb,
                                    iAP2EaNativeReadDone_callback readDone_cb);
    ~iAP2EaNativeManager();
    static void checkLog(void);
    static void updateDebug(void);
    static void notifyCallback(int what,int arg1,int arg2);
    static void setScenario(int scenario);
    static int getScenario();
    void disconnected();
    void createReceiver();
    void destroyReceiver();
    void createTransport();
    void destroyTransport();
    int writeEaNative(void *buffer,int size);
};

#endif /*_IAP2_EA_NATIVE_MANAGER_H_*/
