
/****************************************************************************
 *   FileName    : iAP2EventQueue.h
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

#ifndef IAP2_EVENT_QUEUE_H
#define IAP2_EVENT_QUEUE_H

#include "iAP2EventSemaphore.h"

#ifdef __cplusplus
extern "C" {
#endif


#define CONFIG_QUEUE_SIZE 256
#define CONFIG_EMEGENCY_QUEUE_SIZE 16

typedef enum
{
    IAP2_DEFAULT_EVENT_TYPE = 0,
    IAP2_TIMER_EVENT_TYPE,
    IAP2_FILE_TRANS_START_EVENT_TYPE,

    /* add by bear22 */
    IAP2_AUTH_START_EVENT_TYPE

} iAP2EvnetType_t;


typedef struct iAP2Event_st
{
    uint32_t    type;
    long        param[2];
} ;

typedef struct iAP2Event_st iAP2Event_t;


typedef void (*piAP2EventQueueFunc)(iAP2Event_t* evt);

typedef struct iAP2Queue_st
{
    pthread_mutex_t         mutex;
    iAP2EventSemaphore_t*    sem;
    uint32_t                in;
    uint32_t                out;
    uint32_t                size;
    piAP2EventQueueFunc cleapFunc_cb;
    iAP2Event_t              event[CONFIG_QUEUE_SIZE];
} iAP2Queue_t;

iAP2Queue_t* iAP2EventQueueCreate(void);
void iAP2EventQueueDestroy(iAP2Queue_t* queue);
BOOL iAP2EventQueuePut(iAP2Queue_t* queue, iAP2Event_t* event, BOOL priority);
BOOL iAP2EventQueueGet(iAP2Queue_t* queue, iAP2Event_t* event, uint32_t msec);
void iAP2EventQueueRemove(iAP2Queue_t* queue, piAP2EventQueueFunc cleanupFunc);

#ifdef __cplusplus
}
#endif

#endif  // IAP2_EVENT_QUEUE_H


