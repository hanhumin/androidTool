/****************************************************************************
 *   FileName    : iAP2EaNativeListener.h
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

#ifndef _IAP2_EA_NATIVE_LISTENER_H_
#define _IAP2_EA_NATIVE_LISTENER_H_

typedef void (*iAP2EaNativeTransportNotify_callback)(int type,bool doAttached);
typedef void *(*iAP2EaNativeTransportAttached_callback)(void *pEnv,bool *pAttached);
typedef int (*iAP2EaNativeTransportDoRead_callback)(void *pEnv,int fd);
typedef void (*iAP2EaNativeTransportDetached_callback)(bool attached);
typedef void (*iAP2EaNativeTransportReadDone_callback)(void *buffer, int readLength);

class iAP2EaNativeListener 
{
    bool mDebug;
    void *mpEnv;
    bool mAttached;

    iAP2EaNativeTransportNotify_callback mNotify_cb;
    iAP2EaNativeTransportAttached_callback mAttached_cb;
    iAP2EaNativeTransportDoRead_callback mDoRead_cb;
    iAP2EaNativeTransportDetached_callback mDetached_cb;
    iAP2EaNativeTransportReadDone_callback mReadDone_cb;
    
public:
    int mInited;

    iAP2EaNativeListener(iAP2EaNativeTransportNotify_callback notify_cb,
                                    iAP2EaNativeTransportAttached_callback attached_cb,
                                    iAP2EaNativeTransportDoRead_callback doRead_cb,
                                    iAP2EaNativeTransportDetached_callback detached_cb,
                                    iAP2EaNativeTransportReadDone_callback readDone_cb,
                                    bool debug);
    ~iAP2EaNativeListener();
    void setDebug(bool debug);
    bool isSupportRead();
    int attached();
    int doRead(int fd);
    void detached();
    void readDone(void *buffer,int len);
    void notify(int type,bool doAttached);
};

#endif /*_IAP2_EA_NATIVE_HW_H_*/
