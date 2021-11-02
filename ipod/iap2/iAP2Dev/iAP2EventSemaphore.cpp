
/****************************************************************************
 *   FileName    : iAP2EventSemaphore.c
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
#include "iAP2TimeImpl.h"

#include "iAP2Time.h"
#include "iAP2Callback.h"
#include "iAP2Client.h"

iAP2EventSemaphore_t* iAP2EventSemaphoreCreate(int32_t count)
{
    iAP2EventSemaphore_t *pSem = (iAP2EventSemaphore_t*) malloc(sizeof( iAP2EventSemaphore_t));
    assert(pSem);

    pthread_mutex_init(&pSem->mutex, NULL);
    pthread_cond_init(&pSem->cond, NULL);

    pSem->count = count;

    return pSem;
}

void iAP2EventSemaphoreDestroy(iAP2EventSemaphore_t* pSem)
{
    assert(pSem);

    pthread_cond_destroy(&pSem->cond);
    pthread_mutex_destroy(&pSem->mutex);

    free(pSem);
}

void iAP2EventSemaphoreTake(iAP2EventSemaphore_t* pSem)
{
    assert(pSem);

    pthread_mutex_lock(&pSem->mutex);

    while (pSem->count <= 0)
    {
        pthread_cond_wait(&pSem->cond, &pSem->mutex);
    }

    pSem->count--;
    pthread_mutex_unlock(&pSem->mutex);
}

BOOL iAP2EventSemaphoreTry(iAP2EventSemaphore_t* pSem, uint32_t msec)
{
    assert(pSem);

    if (msec == 0)
    {
        pthread_mutex_lock(&pSem->mutex);

        if (pSem->count <= 0)
        {
            pthread_mutex_unlock(&pSem->mutex);
		

            return FALSE;
        }

        pSem->count--;
        pthread_mutex_unlock(&pSem->mutex);

        return TRUE;
    }

    struct timespec timeout;

    uint32_t base = iAP2TimeTick();
    uint32_t gap = 0;
    uint32_t progress = 0;

    pthread_mutex_lock(&pSem->mutex);

    while (pSem->count <= 0) 
    {
        iAP2TimespecNow(&timeout);
        iAP2TimespecAddMS(&timeout, msec - gap);

        if (pthread_cond_timedwait(&pSem->cond, &pSem->mutex, &timeout) == ETIMEDOUT)
        {
            pthread_mutex_unlock(&pSem->mutex);

            return FALSE;
        }

        gap = iAP2TimeTick() - base;
        progress += gap;

        if (progress >= msec) 
        {
            pthread_mutex_unlock(&pSem->mutex);
            return FALSE;
        }
    }

    pSem->count--;
    pthread_mutex_unlock(&pSem->mutex);

    return TRUE;
}

void iAP2EventSemaphoreGive(iAP2EventSemaphore_t* pSem)
{
    assert(pSem);

    pthread_mutex_lock(&pSem->mutex);
    pSem->count++;
    pthread_cond_signal(&pSem->cond);
    pthread_mutex_unlock(&pSem->mutex);
}

