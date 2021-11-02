/****************************************************************************
 *   FileName    : iAP2Client.h
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


#ifndef _IAP2_CLIENT_H_
#define _IAP2_CLIENT_H_

#include "iAP2Transport.h"
#include "iAP2LinkRunLoop.h"
#include "iAP2Interface.h"
#include "iAP2_jni.h"

namespace android{

#if defined(__cplusplus)
extern "C"
{
#endif

#define MAX_PACKET_SENT_AT_ONCE (64)

#define kMFiAuthDevicePath					"/dev/i2c-2"
#define kMFiAuthDeviceAddress				0x10
#define kMFiAuthRetryDelayMics					5000 // 5 ms.
#define kMFiAuthRetryDelayMics1				5000 // 5 ms.

#define kMFiAuthReg_AuthControlStatus			0x10
#define kMFiAuthFlagError						0x80
#define kMFiAuthControl_GenerateSignature		1
#define kMFiAuthReg_SignatureSize				0x11
#define kMFiAuthReg_SignatureData				0x12
#define kMFiAuthReg_ChallengeSize				0x20
#define kMFiAuthReg_ChallengeData				0x21
#define kMFiAuthReg_DeviceCertificateSize		0x30
#define kMFiAuthReg_DeviceCertificateData1		0x31 // Note: auto-increments so next read is Data2, Data3, etc.
#define kMFiAuthReg_DeviceSerial				0x4e



extern int g_sessionId;

typedef struct iAP2SessPool_st
{
    size_t totalLen;
    size_t writeLen;
    void * poolBuf;
}iAP2SessPool_t;

typedef struct iAP2Client_st
{
    void*               buffer;
    void*               bufferNext;
    size_t              bufferSize;
    size_t              bufferRemain;
    iAP2Transport_t*     trans;
    iAP2LinkRunLoop_t*  runloop;
	iAP2FileTransfer_t * fileTrans;
	
    void *controlSession;
    void *fileSession;
    void *eaSession;
    void *sessionThread;
    
    iAP2InterfaceCB_t interfaceCb;
    iAP2SessPool_t sessPool;
    int transportType;
} iAP2Client_t;

struct iAP2FileTransferDataBuffer_st {  
  //  u8 *buf;
	//int bufLen;
	iAP2FileTransferSetupData_t *setupData;
	iAP2FileTransferDataBuffer_st *next;
    
} ;


extern iAP2Client_t* g_iap2handle;
	



extern void iAP2Client_DebugTrigger(int handle);
 int iAP2ClientExternalReadDone(iAP2Client_t* handle,void *buffer,int size);
 iAP2Client_t * iAP2ClientStart(/*iAP2Notify_callback noti_cb,
                                    void *pData,
                                    void *pCpData,
                                    iAP2ChkConn_callback chkConn_cb,
                                    int transportType,
                                    iAP2ExtWriteAttached_callback attached_cb,
                                    iAP2ExtDoWrite_callback doWrite_cb,
                                    iAP2ExtWriteDetached_callback detached_cb
                                    */);
 int iAP2ClientStop(iAP2Client_t * handle);
 void freeAllIap2FtDataBuffer();
 void freeAllIap2FtReceiveDataBuffer();
void freeAllIap2Session1DataBuffer();
#if defined(__cplusplus)
};
#endif

}

#endif
