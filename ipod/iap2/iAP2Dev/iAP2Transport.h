/****************************************************************************
 *   FileName    : iAP2Transport.h
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


#ifndef _IAP2_TRANSPORT_H_
#define _IAP2_TRANSPORT_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include "iAP2LinkRunLoop.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#define MAX_IAP2_TRANSPORT_BUF_SIZE      1024

#define TRANS_ERR_EIO (0x01)

//                                                                 struct iAP2Transport_st
typedef void (*iAP2TransportAttachedCB_t) (iAP2LinkRunLoop_t* linkRunLoop);
typedef void (*iAP2TransportRecvCB_t)(void * transport,uint8_t* buffer, int len);
typedef int (*iAP2Notify_callback)(int type,int arg0,int arg1,void *obj);
typedef void (*iAP2NotiTransportError)(void * pTransport,int type);

typedef void *(*iAP2ExtWriteAttached_callback)(void *pEnv, int *pAttached);
typedef int (*iAP2ExtDoWrite_callback)(void *pEnv,void *buffer,int len);
typedef void (*iAP2ExtWriteDetached_callback)(int attached);

typedef struct iAP2Transport_st
{
    iAP2LinkRunLoop_t*      linkRunLoop;
    pthread_t               thread;
    pthread_mutex_t syncMutex;
    pthread_cond_t  syncCond;    
    int                     fd;
    int                     openFailCount;
    pthread_mutex_t openMutex;
    BOOL                    bDestroy;
    BOOL                    bDevice;
    BOOL                    bReading;
    BOOL                    bWriting;
    iAP2Packet_t*           packet;
    uint8_t*            buffer;
    uint32_t             bufferSize;

    iAP2TransportAttachedCB_t loopAttachedCb;
    iAP2TransportRecvCB_t receiveCb;
    iAP2Notify_callback noti_cb;
    iAP2NotiTransportError transErr_cb;
    int transportType;
    int btAttached;
    void *pEnv;
    iAP2ExtWriteAttached_callback attached_cb;
    iAP2ExtDoWrite_callback doWrite_cb;
    iAP2ExtWriteDetached_callback detached_cb;
} iAP2Transport_t;

iAP2Transport_t* iAP2TransportCreate(
                                                    iAP2LinkRunLoop_t* linkRunLoop, 
                                                    BOOL bDevice,
                                                    iAP2TransportAttachedCB_t loopAttachedCb,
                                                    iAP2TransportRecvCB_t receiveCb,
                                                    iAP2Notify_callback noti_cb,
                                                    iAP2NotiTransportError transErr_cb,
                                                    int transportType,
                                                    iAP2ExtWriteAttached_callback attached_cb,
                                                    iAP2ExtDoWrite_callback doWrite_cb,
                                                    iAP2ExtWriteDetached_callback detached_cb
                                                                    );
void iAP2TransportDestroy(iAP2Transport_t* transport);
BOOL iAP2TransportOpen(iAP2Transport_t* transport);
void iAP2TransportClose(iAP2Transport_t* transport);
int32_t iAP2TransportSend(iAP2Transport_t* transport, uint8_t* buffer,uint32_t len);
void* iAP2TransportRecvThread(void* param);

#if defined(__cplusplus)
};
#endif

#endif

