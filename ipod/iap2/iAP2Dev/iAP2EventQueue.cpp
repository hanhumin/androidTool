/****************************************************************************
 *   FileName    : iAP2EventQueue.c
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

#include "iAP2EventSemaphore.h"
#include "iAP2EventQueue.h"

#include "iAP2Log.h"
#include "iAP2Callback.h"
#include "iAP2Client.h"

iAP2Queue_t* iAP2EventQueueCreate(void)
{
    iAP2Queue_t* queue = (iAP2Queue_t*)malloc(sizeof(iAP2Queue_t));

    pthread_mutex_init(&queue->mutex, NULL);
    queue->sem = iAP2EventSemaphoreCreate(0);
    assert(queue->sem);

    queue->in   = 0;
    queue->out  = 0;
    queue->size = 0;
    queue->cleapFunc_cb = NULL;

    return queue;
}

void iAP2EventQueueDestroy(iAP2Queue_t* queue)
{
    assert(queue);

    iAP2EventQueueRemove(queue, queue->cleapFunc_cb);

    pthread_mutex_destroy(&queue->mutex);
    iAP2EventSemaphoreDestroy(queue->sem);

    free(queue);
}

BOOL iAP2EventQueuePut(iAP2Queue_t* queue, iAP2Event_t* event, BOOL priority)
{
	

    assert(queue);
	

    pthread_mutex_lock(&queue->mutex);
	

    if (((priority == FALSE) ? (queue->size + CONFIG_EMEGENCY_QUEUE_SIZE) : queue->size) >= CONFIG_QUEUE_SIZE)
    {
  
        pthread_mutex_unlock(&queue->mutex);
	

#if iAP2LINK_DEBUG
        iAP2LogDbg("[iAP2EventQueuePut] Event Queue Full\n");
#endif
	
        return FALSE;
    }


    iAP2EventSemaphoreGive(queue->sem);
	

    if (priority == TRUE)
    {
  
        queue->out           = (queue->out + CONFIG_QUEUE_SIZE - 1) % CONFIG_QUEUE_SIZE;
        queue->event[queue->out] = *event;
		
    }
    else
    {
 
        queue->event[queue->in] = *event;
        queue->in           = (queue->in + 1) % CONFIG_QUEUE_SIZE;
		
    }
	

    queue->size++;
    pthread_mutex_unlock(&queue->mutex);


    return TRUE;
}

BOOL iAP2EventQueueGet(iAP2Queue_t* queue, iAP2Event_t* event, uint32_t msec)
{
	
    assert(queue);

	
    while (TRUE)
    {
        if (iAP2EventSemaphoreTry(queue->sem, msec) == FALSE)
        {
      
            return FALSE;
        }

        pthread_mutex_lock(&queue->mutex);

        *event = queue->event[queue->out];
        queue->out = (queue->out + 1) % CONFIG_QUEUE_SIZE;
        queue->size--;

        pthread_mutex_unlock(&queue->mutex);

        return TRUE;
    }
}

void iAP2EventQueueRemove(iAP2Queue_t* queue, piAP2EventQueueFunc cleanupFunc)
{
    assert(queue);

    iAP2Event_t* event = NULL;

    pthread_mutex_lock(&queue->mutex);

    uint32_t size = queue->size;

    queue->size = 0;

    while (size-- != 0)
    {
        iAP2EventSemaphoreTry(queue->sem,0);

        if (cleanupFunc)
        {
            cleanupFunc(&queue->event[queue->out]);
        }

        queue->out = (queue->out + 1) % CONFIG_QUEUE_SIZE;
    }

    pthread_mutex_unlock(&queue->mutex);
}

