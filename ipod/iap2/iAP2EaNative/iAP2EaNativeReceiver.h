/****************************************************************************
 *   FileName    : iAP2EaNativeReceiver.h
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

#ifndef _IAP2_EA_NATIVE_RECEIVER_H_
#define _IAP2_EA_NATIVE_RECEIVER_H_

#define UEVENT_READ_BUFFER_SIZE (1024)

class iAP2EaNativeReceiver 
{
    static bool mDebug;
    static iAP2EaNativeNotify_callback mNotify_cb;
    static int mNetlinkSocket;
    static int mNetLinkControlPipe[2];
    static pthread_mutex_t mSyncMutex;
    static pthread_cond_t  mSyncCond;       
    pthread_t mNetLinkEventThread;
    static struct sockaddr_nl mSockAddr;
    
    //static iAP2EaNativeReceiver *instance;
    static char mReadBuff[UEVENT_READ_BUFFER_SIZE];

public:
    iAP2EaNativeReceiver(iAP2EaNativeNotify_callback notify_cb,bool debug);
    ~iAP2EaNativeReceiver();
    void setDebug(bool debug);
    //static iAP2EaNativeReceiver *getInstance(notify_callback notify_cb);  
    void cleanup();
    int setFdCloexec();
    void closePipe();
    int netLinkPipe();
    int create(void);
    int destroy(void);
    static const char *parse (const char *buffer, size_t len, const char *key);
    static void parseMsg(char *buffer, size_t len);
    static int readMessage(void);
    static void *eventThreadRun(void *arg);
};

#endif /*_IAP2_EA_NATIVE_RECEIVER_H_*/
