/****************************************************************************
 *   FileName    : iAP2Callback.h
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

#ifndef _IAP2_CALLBACK_H_
#define _IAP2_CALLBACK_H_

#include "iAP2Defines.h"

#if defined(__cplusplus)
extern "C"
{
#endif

typedef void *(*iAP2Malloc_callback)(int size,const char *funcName,int lineNum);
typedef void (*iAP2Free_callback)(void *pMem,const char *funcName);
typedef void (*iAP2PckDebugInit_callback)(void);
typedef void (*iAP2PckDebugAdd_callback)(BOOL isSend,void * pck,int listIndex,int seq,int ack);
typedef void (*iAP2PckDebugDel_callback)(BOOL isSend,void * pck,int listIndex);
typedef void (*iAP2PckDebugPrint_callback)(void);

typedef struct{
    iAP2Malloc_callback mallocCB;
    iAP2Free_callback freeCB;
    iAP2PckDebugInit_callback pckDbgInitCB;
    iAP2PckDebugAdd_callback pckDbgAddCB;
    iAP2PckDebugDel_callback pckDbgDelCB;
    iAP2PckDebugPrint_callback pckDbgPrintCB;
}iAP2Callback_t;

extern iAP2Callback_t *gpiAP2_Callback;

#define IAP2_CBACK(P_CB, P_CBACK, ...) P_CB->P_CBACK(__VA_ARGS__)


void setCallback(iAP2Callback_t* callbacks);

#define Iap2Malloc(x) IAP2_CBACK(gpiAP2_Callback,mallocCB,x,__FUNCTION__,__LINE__)
#define Iap2Free(x) IAP2_CBACK(gpiAP2_Callback,freeCB,x,__FUNCTION__)

#define Iap2PckDbgAdd(v,w,x,y,z) IAP2_CBACK(gpiAP2_Callback,pckDbgAddCB,v,w,x,y,z)
#define Iap2PckDbgDel(w,x,z) IAP2_CBACK(gpiAP2_Callback,pckDbgDelCB,w,x,z)

#if defined(__cplusplus)
};
#endif


#endif

