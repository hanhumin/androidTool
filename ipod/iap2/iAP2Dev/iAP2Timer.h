/****************************************************************************
 *   FileName    : iAP2Timer.h
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

#ifndef IAP2_TIMER_H
#define IAP2_TIMER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>

#include <iAP2Defines.h>
#include "iAP2Time.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef void (*iAP2TimerCB_t) (void* param);

typedef enum
{
    IAP2_TIMER_STATE_STOP = 0,
    IAP2_TIMER_STATE_START
} iAP2TimerState_t;

typedef struct iAP2ImplTimer_st
{
    pthread_t       thread;
    pthread_mutex_t mutex;
    pthread_cond_t  runCond;
    pthread_cond_t  idleCond;
    pthread_cond_t  waitRunCond;
    pthread_cond_t  waitIdleCond;
    
    pthread_mutex_t syncMutex;
    pthread_cond_t  syncCond;
    BOOL            syncFlag;

    uint32_t        delay;
    struct timespec timeout;
    void*           param;

    iAP2TimerCB_t    cancelCB;
    iAP2TimerCB_t    expiredCB;
    BOOL            callbackFlag;

    // for iAPTimer
    iAP2TimeCB_t    iAPCB;

    iAP2TimerState_t state;
    BOOL            destroy;
} iAP2ImplTimer_t;

iAP2ImplTimer_t* iAP2TimerCreate(iAP2TimerCB_t expiredCB, iAP2TimerCB_t cancelCB, void* param);
void iAP2TimerDestroy(iAP2ImplTimer_t* timer);
BOOL iAP2TimerStart(iAP2ImplTimer_t* timer, uint32_t delay);
BOOL iAP2TimerStop(iAP2ImplTimer_t* timer, BOOL callbackFlag);
void iAP2TimerSetDestroy(iAP2ImplTimer_t* timer);

#ifdef __cplusplus
}
#endif

#endif  // IAP2_TIMER_H


