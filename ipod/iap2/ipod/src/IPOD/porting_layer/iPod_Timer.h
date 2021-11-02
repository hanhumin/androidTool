/****************************************************************************
 *   FileName    : iPod_Timer.h
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


#ifndef _IPOD_TIMER_H_
#define _IPOD_TIMER_H_

#include <pthread.h>

#if defined(__cplusplus)
extern "C"
{
#endif

enum{
    IPOD_LIST_WAIT = 0,
    IPOD_LIST_RUNNING,
    IPOD_LIST_CANCELED,
    IPOD_LIST_EXPIRED,
    IPOD_LIST_MAX
};

enum{
    IPOD_TM_TYPE_PLAYING = (1<<0),
    IPOD_TM_TYPE_PAUSE = (1<<1),
    IPOD_TM_TYPE_FF = (1<<2),
    IPOD_TM_TYPE_REW = (1<<3),
    IPOD_TM_TYPE_ENDFFREW = (1<<4),
    IPOD_TM_TYPE_MAX = (1<<5),
    IPOD_TM_TYPE_ALL = (unsigned int)(-1),
};

typedef void (*iPodListCB_t) (void* data,unsigned int state,unsigned int type,unsigned int sendNoti,unsigned int cmdFrom);

typedef struct{
    void *prev;
    void *next;
    unsigned int state;    
    unsigned int type;
    unsigned int req;
    unsigned int sendNoti;
    unsigned int cmdFrom;
    unsigned long timeCnt;
    iPodListCB_t expired_cb;
    iPodListCB_t cancal_cb;
    struct timespec mTimeout;
    void *pData;
}iPodTimerNode_t;

typedef struct{
    int type;
    char *typeName;
}iPodTimerTypeName_t;

enum{
    IPOD_TIMER_POS_NONE = 0,
    IPOD_TIMER_POS_START,
    //IPOD_TIMER_POS_TRANSIT,
    //IPOD_TIMER_POS_CANCEL,
    //IPOD_TIMER_POS_EXPIRED,
    IPOD_TIMER_POS_MAX
};

#define IPOD_TM_STATE_NONE      (1<<IPOD_TIMER_POS_NONE)
#define IPOD_TM_STATE_START     (1<<IPOD_TIMER_POS_START)
//#define IPOD_TM_STATE_TRANSIT     (1<<IPOD_TIMER_POS_TRANSIT)
//#define IPOD_TM_STATE_CANCEL    (1<<IPOD_TIMER_POS_CANCEL)
//#define IPOD_TM_STATE_EXPIRED   (1<<IPOD_TIMER_POS_EXPIRED)

#define SEC_NSEC (1000000000)
#define MSEC_NSEC (1000000)
#define MSEC_MAX (999)

typedef struct{
    pthread_mutex_t mListMutex;
    void * mFirst;
    void * mLast;
    struct timespec mTimeout;
    unsigned int mArraySize;

    unsigned int state;
    unsigned long nextTimerCnt;
    unsigned long currTimerCnt;
    unsigned long currTimerType;

    int bThreadExit;
    pthread_t mThread;
    pthread_mutex_t mMutex;
    pthread_cond_t  mRunCond;
    pthread_mutex_t syncMutex;
    pthread_cond_t  syncCond;
}iPodTimer_t;

extern int iPod_Timer_CancelTimerEntry(void * pTimer, unsigned int type);
extern int iPod_Timer_CreateTimerEntry(
                                                                void *pTimer,
                                                                unsigned int type,
                                                                time_t delaySec,
                                                                long delayMsec,
                                                                iPodListCB_t cancal_cb,
                                                                iPodListCB_t expired_cb,
                                                                void *pData,
                                                                unsigned int sendNoti,
                                                                unsigned int cmdFrom
                                                                );
extern void iPod_Time_InitDebug(void);
extern char *iPod_Timer_GetNameString(unsigned int type);
extern void iPod_Timer_Test1(void);
extern void *iPod_Timer_CreateThread( void);
extern void iPod_Timer_Destroy(void *pTimer);

#if defined(__cplusplus)
};
#endif
#endif	/* _IPOD_TIMER_H_ */

