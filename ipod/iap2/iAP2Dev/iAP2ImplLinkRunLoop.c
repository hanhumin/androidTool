/****************************************************************************
 *   FileName    : iAP2ImplLinkRunLoop.c
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

#include "iAP2LinkRunLoop.h"
#include "iAP2EventQueue.h"
#include "iAP2Time.h"
#include "iAP2TimeImplementation.h"

#include "iAP2Log.h"
#include <utils/Log.h>

static void* _iAP2RunLoopFunc(void* arg)
{


    iAP2LinkRunLoop_t* linkRunLoop = (iAP2LinkRunLoop_t*)arg;
//#ifdef USE_TCC_IAP2
    pthread_mutex_lock(&linkRunLoop->syncMutex);
    pthread_cond_signal(&linkRunLoop->syncCond);
    pthread_mutex_unlock(&linkRunLoop->syncMutex);
//#endif
    
    iAP2LinkRunLoopRun(linkRunLoop);

    return NULL;
}

void iAP2LinkRunLoopInitImplementation (iAP2LinkRunLoop_t* linkRunLoop)
{
//#ifdef USE_TCC_IAP2

    assert(linkRunLoop);

    linkRunLoop->iap2Queue = iAP2EventQueueCreate();
	
    pthread_mutex_init(&linkRunLoop->syncMutex, NULL);
    pthread_cond_init(&linkRunLoop->syncCond, NULL);    

    pthread_mutex_lock(&linkRunLoop->syncMutex);   

    int id = pthread_create(&linkRunLoop->thread, NULL, _iAP2RunLoopFunc, (void *)linkRunLoop);
    pthread_cond_wait(&linkRunLoop->syncCond, &linkRunLoop->syncMutex);
    pthread_mutex_unlock(&linkRunLoop->syncMutex);
    
    assert(id == 0);
		//#endif
}

void iAP2LinkRunLoopCleanupImplementation (iAP2LinkRunLoop_t* linkRunLoop)
{
//#ifdef USE_TCC_IAP2

    assert(linkRunLoop);

    pthread_join(linkRunLoop->thread, NULL);
	
    pthread_cond_destroy(&linkRunLoop->syncCond);
    pthread_mutex_destroy(&linkRunLoop->syncMutex);

    iAP2EventQueueDestroy(linkRunLoop->iap2Queue);
	   // #endif
}

BOOL iAP2LinkRunLoopWait (iAP2LinkRunLoop_t* linkRunLoop)
{
	//iAP2LinkRunLoopRun
	

	if(linkRunLoop==NULL)
		{
		ALOGD("  %s %d linkRunLoop==NULL",__FUNCTION__,__LINE__);
		}

//#ifdef USE_TCC_IAP2
    assert(linkRunLoop);

    iAP2Event_t event;
    if (iAP2EventQueueGet(linkRunLoop->iap2Queue, &event, 300 /* idle time */))
    {
        if (event.type == IAP2_TIMER_EVENT_TYPE)
        {
            iAP2Timer_t* timer = (iAP2Timer_t*)event.param[0];
            iAP2TimeCB_t timerCB = (iAP2TimeCB_t)event.param[1];
            if (timer && timerCB)
            {
                /* _iAP2TimeHandleExpired */
                timerCB(timer, iAP2TimeGetCurTimeMs());
            }
        }
        else
        {
            iAP2LogDbg("%s %p\n",__FUNCTION__, (void*)event.param[0]);
            iAP2LinkRunLoopRunOnce(linkRunLoop, (void*)event.param[0]);
        }
    }
    else
    {

    }
//#endif
    return TRUE;
}


void iAP2LinkRunLoopSignal (iAP2LinkRunLoop_t* linkRunLoop, void* arg)
{


//#ifdef USE_TCC_IAP2
    iAP2Event_t event;
    event.type = IAP2_DEFAULT_EVENT_TYPE;
    event.param[0] = (long)arg;

  //  ALOGD("%s arg:%p\n",__FUNCTION__, (void*)event.param[0]);
	
    iAP2EventQueuePut(linkRunLoop->iap2Queue, &event, FALSE);
	//#endif
}

BOOL iAP2LinkRunLoopProtectedCall (iAP2LinkRunLoop_t* linkRunLoop,
                                   void* arg,
                                   BOOL (*func)(iAP2LinkRunLoop_t* linkRunLoop, void* arg))
{
    return FALSE;
}

void iAP2LinkRunLoopSetEventMaskBit (iAP2LinkRunLoop_t*         linkRunLoop,
                                     iAP2LinkRunLoopEventMask_t bit)
{
//ALOGD(" %s %d",__FUNCTION__,__LINE__);
    linkRunLoop->eventMask |= bit;
#if defined(USE_TCC_IMP)
    if (linkRunLoop->timeoutCb_t)
    {
        /* Send Detect byte seqeunce */
        (*linkRunLoop->timeoutCb_t) ((void *)linkRunLoop, bit);
    }
#endif
}

uint32_t iAP2LinkRunLoopGetResetEventMask (iAP2LinkRunLoop_t* linkRunLoop)
{
	

    uint32_t eventMask = linkRunLoop->eventMask;
    linkRunLoop->eventMask = 0;
    return eventMask;
}

//#if defined(USE_TCC_IMP)
void iAP2LinkRunLoopNotify (iAP2LinkRunLoop_t*         linkRunLoop,
                                     iAP2LinkRunLoopNotifyMask_t bit)
{
	

    if (linkRunLoop->notifyCb_t)
    {
        /* Send Detect byte seqeunce */
        (*linkRunLoop->notifyCb_t) ((void *)linkRunLoop, bit);
    }

}
//#endif

