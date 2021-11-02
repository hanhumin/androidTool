/****************************************************************************
 *   FileName    : iAP2EventSemaphore.h
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

#ifndef IAP2_EVENT_SEMAPHORE_H
#define IAP2_EVENT_SEMAPHORE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>

#include <iAP2Defines.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct iAP2EventSemaphore_st
{
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int32_t         count;
} iAP2EventSemaphore_t;


iAP2EventSemaphore_t* iAP2EventSemaphoreCreate(int32_t count);
void iAP2EventSemaphoreDestroy(iAP2EventSemaphore_t* pSem);
void iAP2EventSemaphoreTake(iAP2EventSemaphore_t* pSem);
BOOL iAP2EventSemaphoreTry(iAP2EventSemaphore_t* pSem, uint32_t msec);
void iAP2EventSemaphoreGive(iAP2EventSemaphore_t* pSem);

#ifdef __cplusplus
}
#endif

#endif  // IAP2_EVENT_SEMAPHORE_H

