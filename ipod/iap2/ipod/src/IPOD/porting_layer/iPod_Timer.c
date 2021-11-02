/****************************************************************************
 *   FileName    : iPod_Timer.c
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


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/times.h>
#include <sys/poll.h>
#include <utils/Log.h>
#include <cutils/properties.h>
#include "iPod_Timer.h"
#include <stdlib.h>
#include <pthread.h>




#ifndef NULL
#define NULL 0
#endif

#define TRUE 1
#define FALSE 0

#define LOG_TAG "IapService"

int IAPTIME_DEBUG = -1;
#define IAPTIME_DBUG(msg...)	if (IAPTIME_DEBUG == 1) { ALOGD( " " msg); }

int IAPTIMED_DEBUG = -1;
#define IAPTIMED_DBUG(msg...)	if (IAPTIMED_DEBUG == 1) { ALOGD( " " msg); }

iPodTimerTypeName_t gTypeNameList[]={
        {IPOD_TM_TYPE_PLAYING, "Timer_Playing" },
        {IPOD_TM_TYPE_PAUSE, "Timer_Pause" },
        {IPOD_TM_TYPE_FF, "Timer_FF" },
        {IPOD_TM_TYPE_REW, "Timer_Rew" },
        {IPOD_TM_TYPE_ENDFFREW, "Timer_EndFFRew" },
        {IPOD_TM_TYPE_ALL, "Timer_All" },
};

void iPod_Time_InitDebug(void)
{
    //if(IAPOS_DEBUG == -1)
    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.debug.time", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPTIME_DEBUG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPTIME_DEBUG = 0;
                break;
        }

        property_get("persist.iap.debug.timed", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPTIMED_DEBUG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPTIMED_DEBUG = 0;
                break;
        }
    }
}

int iPod_Timer_IsDebugLog(void)
{
    return (IAPTIME_DEBUG== 1)? 1:0;
}

char *iPod_Timer_GetNameString(unsigned int type)
{
    int i;
    int max;
    char *pName = NULL;

    max = sizeof(gTypeNameList)/sizeof(iPodTimerTypeName_t);
    for(i = 0; i < max;i++){
        if(gTypeNameList[i].type == (int)type)
            pName = gTypeNameList[i].typeName;
    }
    return pName;
}

void iPod_Timer_Test1(void)
{
    struct timespec now;
    struct timeval delta;
    clock_gettime(CLOCK_MONOTONIC, &now);

    ALOGI( " %s sec %ld nsec %ld",__FUNCTION__,now.tv_sec,now.tv_nsec);
}

int iPod_Timer_CompTimer(struct timespec time1,struct timespec time2)
{
    struct timespec delta;
    if(time1.tv_sec > time2.tv_sec)
        return 1;
    if(time1.tv_sec == time2.tv_sec){
        if(time1.tv_nsec > time2.tv_nsec){
            return 1;
        }
        if(time1.tv_nsec == time2.tv_nsec)
            return 0;
    }
    return -1;
}

int iPod_Timer_IsExpired(struct timespec timeout)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    if(iPod_Timer_CompTimer( now,timeout) >= 0)
        return TRUE;
    return FALSE;
}

int iPod_Timer_MakeNewTimeout(time_t delaySec,long delayNsec, void * newTime)
{
    struct timespec *newTimeout = (struct timespec*)newTime;
    clock_gettime(CLOCK_MONOTONIC, newTimeout);
    newTimeout->tv_sec += delaySec;
    newTimeout->tv_nsec += delayNsec;
    if(newTimeout->tv_nsec >= SEC_NSEC){
        newTimeout->tv_sec++;
        newTimeout->tv_nsec -= SEC_NSEC;
    }
    return 1;
}

void iPod_Timer_SendRunSig(void *pTimer)
{
    iPodTimer_t *timer = (iPodTimer_t *)pTimer;
    if(timer == NULL){
        ALOGI("%s context is null",__FUNCTION__);
        return;
    }
    IAPTIMED_DBUG("%s ",__FUNCTION__);
    pthread_mutex_lock(&timer->mMutex);
    pthread_cond_signal(&timer->mRunCond);
    pthread_mutex_unlock(&timer->mMutex);
}

int iPod_Timer_CancelTimerEntry(void * pTimer, unsigned int type)
{
    iPodTimer_t *timer = (iPodTimer_t *)pTimer;
    if(timer == NULL){
        ALOGI("%s context is null",__FUNCTION__);
        return 0;
    }

    IAPTIME_DBUG("%s start type0x%x (arraySize %d)",
                                                                __FUNCTION__,
                                                                type,
                                                                timer->mArraySize);
    
    pthread_mutex_lock(&timer->mListMutex);
    if(timer->mFirst != NULL)
    {
        iPodTimerNode_t *pCurrNode = NULL;
        iPodTimerNode_t *pCurrPrev = NULL;
        iPodTimerNode_t *pCurrNext = NULL;
        
        pCurrNode = (iPodTimerNode_t *)timer->mLast;
        while(pCurrNode != NULL){
            IAPTIMED_DBUG("%s curr type %s",__FUNCTION__,iPod_Timer_GetNameString(pCurrNode->type));
            pCurrPrev = pCurrNode->prev;
            pCurrNext = pCurrNode->next;
            if((pCurrNode->type & type) == pCurrNode->type){
                IAPTIMED_DBUG("%s find same type %s timeCnt %ld state %d arraySize %d",  
                                                                        __FUNCTION__,
                                                                        iPod_Timer_GetNameString(pCurrNode->type),
                                                                        pCurrNode->timeCnt,
                                                                        pCurrNode->state,
                                                                        timer->mArraySize
                                                                        );
                // Fist Node(Now running);
                if(pCurrNode->state == IPOD_LIST_RUNNING){
                    pCurrNode->req = IPOD_LIST_CANCELED;
                    iPod_Timer_SendRunSig(pTimer);
                }
                else
                {
                    // Last Node
                    if(pCurrNext == NULL)
                    {
                        if(pCurrPrev == NULL){
                            // Last & First Node (not yet running)
                            timer->mFirst = NULL;
                            timer->mLast = NULL;
                            IAPTIMED_DBUG("%s first & last node",__FUNCTION__);

                        }else{
                            pCurrPrev->next = NULL;
                            timer->mLast = pCurrPrev;
                            IAPTIMED_DBUG("%s last node after  %s",
                                                                                __FUNCTION__,
                                                                                iPod_Timer_GetNameString(pCurrPrev->type));
                        }
                    }
                    else
                    {
                        if(pCurrPrev == NULL)
                        {
                            // First Node (not yet running)
                            pCurrNext->prev = NULL;
                            timer->mFirst = pCurrNext;
                            IAPTIMED_DBUG("%s  first node before %s",
                                                                                __FUNCTION__,
                                                                                iPod_Timer_GetNameString(pCurrNext->type));
                        }
                        else
                        {
                            // Middle Node
                            pCurrPrev->next = pCurrNext;
                            pCurrNext->prev = pCurrPrev;
                            IAPTIMED_DBUG("%s  between %s and %s",  
                                                                        __FUNCTION__,
                                                                        iPod_Timer_GetNameString(pCurrPrev->type),
                                                                        iPod_Timer_GetNameString(pCurrNext->type)
                                                                        );
                        }
                    }
                    if(pCurrNode->cancal_cb){
                        pCurrNode->cancal_cb(pCurrNode->pData,
                                                            IPOD_LIST_CANCELED, 
                                                            pCurrNode->type,
                                                            pCurrNode->sendNoti,
                                                            pCurrNode->cmdFrom);
                    }
                    timer->mArraySize--;
                    IAPTIMED_DBUG("%s arraySize %d ",__FUNCTION__,timer->mArraySize);
                    free(pCurrNode);
                }
            }
            pCurrNode = pCurrPrev;
        }    
    }
    pthread_mutex_unlock(&timer->mListMutex);   
    IAPTIME_DBUG("%s end type 0x%x (arraySize %d)",__FUNCTION__,type,timer->mArraySize);
    return 1;
}

int iPod_Timer_CreateTimerEntry(
                                                                void *pTimer,
                                                                unsigned int type,
                                                                time_t delaySec,
                                                                long delayMsec,
                                                                iPodListCB_t cancal_cb,
                                                                iPodListCB_t expired_cb,
                                                                void *pData,
                                                                unsigned int sendNoti,
                                                                unsigned int cmdFrom
                                                                )
{
    iPodTimer_t *timer = (iPodTimer_t *)pTimer;
    iPodTimerNode_t *pNewNode = NULL;
    struct timespec newTimeout;
    int fSendSig = 0;

    if(timer== NULL){
        ALOGI("%s timer is null",__FUNCTION__);
        return 0;
    }

    IAPTIME_DBUG("%s start type %s [%ld s:%ld ms]",
                                                            __FUNCTION__,
                                                            iPod_Timer_GetNameString(type),
                                                            delaySec,
                                                            delayMsec);
    if(type >= IPOD_TM_TYPE_MAX){
        ALOGI("%s type(%d) is invalid",__FUNCTION__,type);
        return 0;
    }

    if(delaySec <= 0 && delayMsec <= 0){
        ALOGI("%s delay is invalid",__FUNCTION__);
        return 0;
    }
    
    pNewNode = (iPodTimerNode_t *)malloc(sizeof(iPodTimerNode_t));
    if(pNewNode == NULL){
        ALOGI("%s ..... can not alloc for timeEntry",__FUNCTION__);
        return 0;
    }

    iPod_Timer_CancelTimerEntry(pTimer, type);
    if(delayMsec > MSEC_MAX)
        delayMsec = MSEC_MAX;
    iPod_Timer_MakeNewTimeout(delaySec,delayMsec*MSEC_NSEC,&newTimeout);
    IAPTIMED_DBUG("%s 2 ",__FUNCTION__);
    pthread_mutex_lock(&timer->mListMutex);
    pNewNode->state = IPOD_LIST_WAIT;
    pNewNode->req = IPOD_LIST_WAIT;
    pNewNode->mTimeout = newTimeout;
    //pNewNode->pData = pData;
    pNewNode->pData = pNewNode;
    pNewNode->cancal_cb = cancal_cb;
    pNewNode->expired_cb = expired_cb;
    pNewNode->timeCnt = timer->nextTimerCnt;
    pNewNode->type = type;
    pNewNode->prev = NULL;
    pNewNode->next = NULL;
    pNewNode->sendNoti = sendNoti;
    pNewNode->cmdFrom = cmdFrom;
    timer->nextTimerCnt++;
    IAPTIMED_DBUG("%s new timer type %s timeCnt %ld sec %ld nsec %ld",
                                                                __FUNCTION__,
                                                                iPod_Timer_GetNameString(type),
                                                                pNewNode->timeCnt,
                                                                pNewNode->mTimeout.tv_sec,
                                                                pNewNode->mTimeout.tv_nsec
                                                                );
    
    if(timer->mFirst == NULL)
    {
        pNewNode->prev = NULL;
        pNewNode->next = NULL;
        timer->mFirst = pNewNode;
        timer->mLast = pNewNode;
        IAPTIMED_DBUG("%s start timer(first location)  ",__FUNCTION__);
        fSendSig= 1;
    }else{
        iPodTimerNode_t *pCurrNode = NULL;
        pCurrNode = (iPodTimerNode_t *)timer->mFirst;
        while(pCurrNode != NULL){
            if( iPod_Timer_CompTimer(pCurrNode->mTimeout, pNewNode->mTimeout) > 0)
            {
                iPodTimerNode_t *pCurrPrev = (iPodTimerNode_t *)pCurrNode->prev;
                iPodTimerNode_t *pCurrNext = (iPodTimerNode_t *)pCurrNode->next;
                if(pCurrNode->prev == NULL){
                    pNewNode->next = pCurrNode;
                    pCurrNode->prev = pNewNode;
                    timer->mFirst = pNewNode;
                    IAPTIMED_DBUG("%s add to first location before %s ",
                                                            __FUNCTION__,
                                                            iPod_Timer_GetNameString(pCurrNode->type));
                    fSendSig = 1;
                }else{
                    pNewNode->prev = pCurrNode->prev;
                    pNewNode->next = pCurrNode;
                    pCurrPrev->next = pNewNode;
                    pCurrNode->prev = pNewNode;                    
                    IAPTIMED_DBUG("%s add to middle location between %s and %s ",
                                                            __FUNCTION__,
                                                            iPod_Timer_GetNameString(pCurrPrev->type),
                                                            iPod_Timer_GetNameString(pCurrNode->type)
                                                            );
                }
                break;
            }

            //  final node
            if(pCurrNode->next == NULL){
                pNewNode->prev = pCurrNode;
                pCurrNode->next = pNewNode;
                timer->mLast = pNewNode;
                IAPTIMED_DBUG("%s add to last location after %s ",
                                                                __FUNCTION__,
                                                                iPod_Timer_GetNameString(pCurrNode->type));
                break;
            }
            pCurrNode = (iPodTimerNode_t *)pCurrNode->next;
        }
    }
    timer->mArraySize++;
    pthread_mutex_unlock(&timer->mListMutex);
    if(fSendSig > 0){
        iPod_Timer_SendRunSig(pTimer);
    }
    IAPTIME_DBUG("%s end (arraySize %d) ",__FUNCTION__,timer->mArraySize);
    return 1;
}

void iPod_Timer_DeleteNUpdateTimerList(iPodTimer_t *timer, int fExpired,int fAleayExpired)
{
    iPodTimerNode_t * pCurrNode = NULL;
    iPodTimerNode_t *pNextNode = NULL;
    unsigned long currTimerCnt = 0;
    unsigned int currTimerType = 0;

    IAPTIME_DBUG("%s start fExpired %d fAleayExpired %d ",__FUNCTION__,fExpired,fAleayExpired);
    pthread_mutex_lock(&timer->mListMutex);
    pCurrNode = (iPodTimerNode_t *)timer->mFirst;
    if(pCurrNode == NULL){
        pthread_mutex_unlock(&timer->mListMutex);
        ALOGI("%s currTimer is null",__FUNCTION__);
        return;
    }
    currTimerCnt = pCurrNode->timeCnt;
    currTimerType = pCurrNode->type; 
    if(pCurrNode->timeCnt != timer->currTimerCnt && fAleayExpired == 0)
    {
        pthread_mutex_unlock(&timer->mListMutex);
        ALOGI("%s currTimer is changed",__FUNCTION__);
        return;
    }
    
    pNextNode = (iPodTimerNode_t *)pCurrNode->next;
    timer->mFirst = (void *)pNextNode;
    if(pNextNode == NULL){
        IAPTIMED_DBUG("%s mFirst & mLast is set to NULL (arraySize %d)",__FUNCTION__,timer->mArraySize);
        timer->mFirst = NULL;
        timer->mLast = NULL;
    }else{
        pNextNode->prev = NULL;
    }
  
    if(iPod_Timer_IsDebugLog()){ // for debug
        if(pNextNode != NULL){
            iPodTimerNode_t * pNode = pNextNode;
            unsigned long nextTimerCnt = 0;
            unsigned int nextTimerType = 0;

            if(pNode->next != NULL){
                nextTimerCnt = pNode->timeCnt;
                nextTimerType = pNode->type;
            }else{
                nextTimerCnt= (unsigned long)(-1);
                nextTimerType = (unsigned int)(-1);
             }
            IAPTIME_DBUG("%s update curr timeType(%ld)[%d] nextType(%ld)[%d]",
                                                                    __FUNCTION__,
                                                                    currTimerCnt,
                                                                    currTimerType,
                                                                    nextTimerCnt,
                                                                    nextTimerType
                                                                    );
        }
    }
    pthread_mutex_unlock(&timer->mListMutex);
    
    IAPTIME_DBUG("%s fExpired %d  type %s",
                                                    __FUNCTION__,
                                                    fExpired,
                                                    iPod_Timer_GetNameString(pCurrNode->type));
    if(fExpired == TRUE){
        if(pCurrNode->expired_cb){
            pCurrNode->expired_cb(pCurrNode->pData,
                                                IPOD_LIST_EXPIRED, 
                                                pCurrNode->type,
                                                pCurrNode->sendNoti,
                                                pCurrNode->cmdFrom
                                                );
        }
    }else{
        if(pCurrNode->cancal_cb){
            pCurrNode->cancal_cb(pCurrNode->pData,
                                                IPOD_LIST_CANCELED, 
                                                pCurrNode->type,
                                                pCurrNode->sendNoti,
                                                pCurrNode->cmdFrom);
        }
    }       

    if(pCurrNode != NULL){
        timer->mArraySize--;
        free(pCurrNode);
    }
    
    IAPTIME_DBUG("%s end (arraySize %d)",__FUNCTION__,timer->mArraySize);
}

void *iPod_Timer_Run(void *data)
{
    iPodTimer_t *timer = (iPodTimer_t *)data;
    int retExpired;
    int aleadyExpired = 0;
    int ret;

    pthread_mutex_lock(&timer->syncMutex);
    pthread_cond_signal(&timer->syncCond);
    pthread_mutex_unlock(&timer->syncMutex);
            
    pthread_mutex_lock(&timer->mMutex);
    IAPTIME_DBUG("%s start Ipod Timer trhead ",__FUNCTION__);

    while(timer->bThreadExit == FALSE)
    {
        retExpired = FALSE;
        aleadyExpired = FALSE;
        if(timer->bThreadExit != FALSE){
            IAPTIME_DBUG("%s set bThreadExit ",__FUNCTION__);
            break;
        }

        if(timer->state == IPOD_TM_STATE_NONE && timer->mFirst == NULL){
            IAPTIME_DBUG("%s wait StartCond ",__FUNCTION__);
            ret = pthread_cond_wait(&timer->mRunCond,&timer->mMutex);
            IAPTIME_DBUG("%s recive StartCond ",__FUNCTION__);
            pthread_mutex_unlock(&timer->mMutex);
        }

        pthread_mutex_lock(&timer->mListMutex);        
        if(timer->mFirst != NULL)
        {
            iPodTimerNode_t *pNode = NULL;
            pNode = (iPodTimerNode_t *)timer->mFirst;
            pNode->state = IPOD_LIST_RUNNING;
            IAPTIME_DBUG("%s start sec %ld nsec %ld ",
                                                                __FUNCTION__, 
                                                                pNode->mTimeout.tv_sec,
                                                                pNode->mTimeout.tv_nsec);
            retExpired = iPod_Timer_IsExpired(pNode->mTimeout);
            pthread_mutex_unlock(&timer->mListMutex);
            
            if(retExpired == FALSE)
            {
                struct timespec abstime = { 0, 0 };
                abstime.tv_sec = pNode->mTimeout.tv_sec;
                abstime.tv_nsec = pNode->mTimeout.tv_nsec;
                timer->state = IPOD_TM_STATE_START;
                IAPTIMED_DBUG("%s timer state START +++++++ type %d sec %ld nsec %ld",
                                                                                        __FUNCTION__,
                                                                                        pNode->type,
                                                                                        abstime.tv_sec, 
                                                                                        abstime.tv_nsec
                                                                                        );
                timer->currTimerCnt = pNode->timeCnt;
                ret = pthread_cond_timedwait_monotonic(&timer->mRunCond, &timer->mMutex, &abstime);
                retExpired = ( ret == ETIMEDOUT)? TRUE:FALSE;
                timer->state = IPOD_TM_STATE_NONE;
                IAPTIMED_DBUG("%s timer state NONE --------- ",__FUNCTION__);
                //pthread_mutex_unlock(&timer->mMutex);
            }else{
                IAPTIME_DBUG("%s timer is already expired type %d",__FUNCTION__,pNode->type);
                aleadyExpired = TRUE;
            }
            pthread_mutex_unlock(&timer->mMutex);

            iPod_Timer_DeleteNUpdateTimerList(timer,retExpired,aleadyExpired);
            IAPTIMED_DBUG("%s timer state %d exist mFirst == NULL %d ",
                                                                                        __FUNCTION__,
                                                                                        timer->state,
                                                                                        (timer->mFirst == NULL)? TRUE:FALSE
                                                                                        );
        }else{
            pthread_mutex_unlock(&timer->mListMutex);
        }
        pthread_mutex_lock(&timer->mMutex);
    }
    pthread_mutex_unlock(&timer->mMutex);
    IAPTIME_DBUG("%s end Ipod Timer trhead ",__FUNCTION__);
    return 0;
}

void *iPod_Timer_CreateThread( void)
{
    iPodTimer_t* timer = (iPodTimer_t*)malloc(sizeof(iPodTimer_t));
    ALOGI("%s start ",__FUNCTION__);
    timer->mFirst = NULL;
    timer->mLast= NULL;
    timer->mArraySize= 0;
    timer->state = IPOD_TM_STATE_NONE;
    timer->bThreadExit = FALSE;
    timer->nextTimerCnt = 1;

    pthread_mutex_init(&(timer->mListMutex), NULL);
    pthread_mutex_init(&(timer->mMutex), NULL);
    pthread_cond_init(&timer->mRunCond, NULL);

    pthread_mutex_init(&timer->syncMutex, NULL);
    pthread_cond_init(&timer->syncCond, NULL);    

    pthread_mutex_lock(&timer->syncMutex);
    int id = pthread_create(&timer->mThread, NULL, iPod_Timer_Run, (void *)timer);
    pthread_cond_wait(&timer->syncCond, &timer->syncMutex);
    pthread_mutex_unlock(&timer->syncMutex);
    
    ALOGI("%s end ",__FUNCTION__);
    return (void *)timer;
}

void iPod_Timer_Destroy(void *pTimer)
{
    iPodTimer_t *timer = (iPodTimer_t *)pTimer;
    if(timer == NULL){
        ALOGI("%s context is null ",__FUNCTION__);
        return;
    }

    ALOGI("%s start ",__FUNCTION__);

    iPod_Timer_CancelTimerEntry(pTimer,IPOD_TM_TYPE_ALL);
    
    timer->bThreadExit = TRUE;
    iPod_Timer_SendRunSig(pTimer);
    pthread_join(timer->mThread, NULL);
    ALOGI("%s destroy the Timer trhead ",__FUNCTION__);
    pthread_cond_destroy(&timer->mRunCond);
    pthread_mutex_destroy(&timer->mMutex);
    pthread_mutex_destroy(&timer->mListMutex);
    pthread_cond_destroy(&timer->syncCond);
    pthread_mutex_destroy(&timer->syncMutex);
    ALOGI("%s end ",__FUNCTION__);
}

