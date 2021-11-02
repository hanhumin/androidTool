/****************************************************************************
 *   FileName    : iAP2Timer.c
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

#include "iAP2Timer.h"
#include "iAP2TimeImpl.h"

#include "iAP2Log.h"
#include "iAP2Time.h"
#include "iAP2Link.h"
#include <utils/Log.h>
#include "iAP2Callback.h"
#include <utils/Log.h>

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "IPODJni", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "IPODJni", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , "IPODJni", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "IPODJni", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , "IPODJni", __VA_ARGS__)

#define LOG_TAG "iAP2"

static void* _iAP2TimerThreadFunc(void* arg);

iAP2ImplTimer_t* iAP2TimerCreate(iAP2TimerCB_t expiredCB, iAP2TimerCB_t cancelCB, void* param)
{

    iAP2ImplTimer_t* timer = (iAP2ImplTimer_t*)malloc(sizeof(iAP2ImplTimer_t));
    assert(timer);

    memset(timer,0,sizeof(iAP2ImplTimer_t));
    #if 0
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&timer->mutex, &attr);
    pthread_mutexattr_destroy(&attr);

    #else
    pthread_mutex_init(&timer->mutex, NULL);
    #endif
    pthread_cond_init(&timer->runCond, NULL);
    pthread_cond_init(&timer->idleCond, NULL);
    pthread_cond_init(&timer->waitRunCond, NULL);
    pthread_cond_init(&timer->waitIdleCond, NULL);
    
    pthread_mutex_init(&timer->syncMutex, NULL);
    pthread_cond_init(&timer->syncCond, NULL);

    timer->delay    = 0;
    timer->destroy  = FALSE;
    timer->state    = IAP2_TIMER_STATE_STOP;
    timer->callbackFlag   = TRUE;
    timer->syncFlag = TRUE;

    timer->param    = param;
    timer->expiredCB = expiredCB;
    timer->cancelCB = cancelCB;
    timer->iAPCB    = NULL;

    pthread_mutex_lock(&timer->syncMutex);
#if 0
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, stacksize);
    int id = pthread_create(&timer->thread, &attributes, _iAP2TimerThreadFunc, (void *)&timer);
    pthread_attr_destroy(&attr);
#else

    int id = pthread_create(&timer->thread, NULL, _iAP2TimerThreadFunc, (void *)timer);
#endif
    assert(id == 0);

    pthread_cond_wait(&timer->syncCond, &timer->syncMutex);
    pthread_mutex_unlock(&timer->syncMutex);

    return timer;
}

void iAP2TimerDestroy(iAP2ImplTimer_t* timer)
{
    assert(timer);
    iAP2LogDbg("%s:%d tid %d\n",__FUNCTION__, __LINE__,gettid());
    //pthread_mutex_lock(&timer->mutex);

    timer->destroy = TRUE;
    pthread_cond_signal(&timer->runCond);
    pthread_cond_signal(&timer->idleCond);
    pthread_cond_signal(&timer->waitRunCond);
    pthread_cond_signal(&timer->waitIdleCond);

    //pthread_mutex_unlock(&timer->mutex);

    pthread_join(timer->thread, NULL);

    pthread_cond_destroy(&timer->idleCond);
    pthread_cond_destroy(&timer->runCond);
    pthread_cond_destroy(&timer->waitIdleCond);
    pthread_cond_destroy(&timer->waitRunCond);
    pthread_mutex_destroy(&timer->mutex);

    pthread_cond_destroy(&timer->syncCond);
    pthread_mutex_destroy(&timer->syncMutex);

    free(timer);
}

iAP2TimerState_t iAP2TimerGetState(iAP2ImplTimer_t* timer)
{
    return timer->state;
}

BOOL iAP2TimerStart(iAP2ImplTimer_t* timer, uint32_t delay)
{   
    BOOL fWait = FALSE;
    assert(timer);
    iAP2LogDbg("%s:%d tid %d \n",__FUNCTION__, __LINE__,gettid());

    timer->delay = delay;
    if(iAP2TimerGetState(timer) == IAP2_TIMER_STATE_START){
        ALOGE(" %s invalid request(start) in start state",__FUNCTION__);
    }else{
        fWait = TRUE;
        pthread_mutex_lock(&timer->mutex);
    }
    iAP2TimespecNow(&timer->timeout);
    iAP2TimespecAddMS(&timer->timeout, delay);
    pthread_cond_signal(&timer->idleCond);

    if(fWait){
        iAP2LogDbg("%s:%d wait signal tid %d \n",__FUNCTION__, __LINE__,gettid());
        pthread_cond_wait(&timer->waitIdleCond, &timer->mutex);
        pthread_mutex_unlock(&timer->mutex);
    }

    iAP2LogDbg("%s:%d end tid %d \n",__FUNCTION__, __LINE__,gettid());

    return TRUE;
}

void iAP2TimerSetDestroy(iAP2ImplTimer_t* timer)
{
    pthread_mutex_lock(&timer->mutex);
    timer->destroy = TRUE;
    pthread_mutex_unlock(&timer->mutex);
}

BOOL iAP2TimerStop(iAP2ImplTimer_t* timer, BOOL callbackFlag)
{   
    BOOL fWait = FALSE;
    assert(timer);
    iAP2LogDbg("%s:%d tid %d \n",__FUNCTION__, __LINE__,gettid());

    pthread_mutex_lock(&timer->mutex);
    if(iAP2TimerGetState(timer) == IAP2_TIMER_STATE_START){
        fWait = TRUE;
    }else{
        pthread_mutex_unlock(&timer->mutex);
    }

    timer->callbackFlag = callbackFlag;
    pthread_cond_signal(&timer->runCond);

    if(fWait == TRUE){
        iAP2LogDbg("%s:%d wait signal tid %d \n",__FUNCTION__, __LINE__,gettid());
        
        pthread_cond_wait(&timer->waitRunCond, &timer->mutex);
        pthread_mutex_unlock(&timer->mutex);
    }

    
    iAP2LogDbg("%s:%d end tid %d \n",__FUNCTION__, __LINE__,gettid());

    return TRUE;
}

void* _iAP2TimerThreadFunc(void* arg)
{
    iAP2ImplTimer_t* timer = (iAP2ImplTimer_t*) arg;
    BOOL        ret;

    iAP2TimerCB_t    cancelCB;
    iAP2TimerCB_t    expiredCB;
    BOOL            callbackFlag;
    void*           callbackParam;
    BOOL threadStoped = FALSE;
    int retval = 0;

    pthread_mutex_lock(&timer->mutex);

    while(timer->destroy != TRUE)
    {
        if (timer->syncFlag)
        {      
            pthread_mutex_lock(&timer->syncMutex);
            pthread_cond_signal(&timer->syncCond);
            pthread_mutex_unlock(&timer->syncMutex);
        }

        cancelCB = timer->cancelCB;
        expiredCB = timer->expiredCB;
        callbackFlag = timer->callbackFlag;
        callbackParam = timer->param;

        if (timer->state == IAP2_TIMER_STATE_START)
        {
            iAP2LogDbg("%s:%d thread wait timeout : %d tid %d\n", __FUNCTION__, __LINE__, timer->delay,gettid());
            retval = pthread_cond_timedwait(&timer->runCond, &timer->mutex, &timer->timeout);
            ret = ( retval !=ETIMEDOUT);
            threadStoped = timer->destroy;
            if (ret == TRUE)
            {
                iAP2LogDbg("%s:%d thread cancel timeout retval %d tid %d\n", __FUNCTION__, __LINE__,retval,gettid());
                if (callbackFlag && cancelCB && threadStoped == FALSE)
                {
                    cancelCB(callbackParam);
                }
            }
            else
            {
                iAP2LogDbg("%s:%d thread timeout retval %d tid %d\n", __FUNCTION__, __LINE__,retval,gettid());
                if (expiredCB && threadStoped == FALSE)
                {
                    iAP2LogDbg("%s:%d call expiredCB tid %d\n", __FUNCTION__, __LINE__,gettid());
                   expiredCB(callbackParam);
                }
            }
            timer->state = IAP2_TIMER_STATE_STOP;
            pthread_cond_signal(&timer->waitRunCond);
            

            
            iAP2LogDbg("%s:%d end start loop\n", __FUNCTION__, __LINE__,gettid());
        }
        else
        {
            iAP2LogDbg("%s:%d wait start signal %d\n", __FUNCTION__, __LINE__,gettid());
            
            pthread_cond_wait(&timer->idleCond, &timer->mutex);
            timer->state = IAP2_TIMER_STATE_START;
            pthread_cond_signal(&timer->waitIdleCond);            
        }
        if(timer->destroy == TRUE)
            pthread_mutex_unlock(&timer->mutex);
    }

	

    return 0;
}

