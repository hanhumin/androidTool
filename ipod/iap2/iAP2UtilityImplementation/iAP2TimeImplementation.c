
#include "iAP2Defines.h"
#include "iAP2Link.h"
#include "iAP2LinkRunLoop.h"
#include "iAP2TimeImplementation.h"
#include "iAP2EventQueue.h"

static void _iAP2ExpireCallback(void* param)
{
    iAP2Timer_t* timer = (iAP2Timer_t*)param;
	
    if (timer->iap2Timer->iAPCB)
    {
        /*
        <_iAP2TimeHandleExpired>
        It is called by timer therad and It will contact "timer->listBuffer".
        timer->listBuffer should be protected from other thread.
        */
        /* timer->iap2Timer->iAPCB(timer, iAP2TimeGetCurTimeMs()); */

        iAP2Link_t* link = (iAP2Link_t*)timer->link;
        iAP2LinkRunLoop_t* runLoop = (iAP2LinkRunLoop_t*)link->context;
        iAP2Event_t event;
        event.type = IAP2_TIMER_EVENT_TYPE;
        event.param[0] = (long)timer;
        event.param[1] = (long)timer->iap2Timer->iAPCB;
		
        iAP2EventQueuePut(runLoop->iap2Queue, &event, FALSE);
    }
}

void _iAP2TimeCreateCallback (iAP2Timer_t* timer)
{

    timer->iap2Timer = iAP2TimerCreate(_iAP2ExpireCallback, NULL, timer);
}

void _iAP2TimeCleanupCallback (iAP2Timer_t* timer)
{
    iAP2TimerDestroy(timer->iap2Timer);
    timer->iap2Timer = NULL;
}

BOOL _iAP2TimeCallbackAfter (iAP2Timer_t* timer,
                             uint32_t     delayMs,
                             iAP2TimeCB_t callback)
{
    timer->iap2Timer->iAPCB = callback;
    return iAP2TimerStart(timer->iap2Timer, delayMs);
}

void _iAP2TimePerformCallback (iAP2Timer_t* timer,
                               iAP2TimeCB_t callback)
{
}

void _iAP2TimeCancelCallback (iAP2Timer_t* timer)
{
    iAP2TimerStop(timer->iap2Timer, TRUE);
}

void iAP2TimeSetDestroy(iAP2Timer_t *timer)
{
    iAP2TimerSetDestroy(timer->iap2Timer);
}

