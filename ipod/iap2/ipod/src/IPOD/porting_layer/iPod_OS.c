/****************************************************************************
 *   FileName    : iPod_OS.c
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

#include "iPod_PAL.h"
#include "iPod_OS.h"
#include "iPod_CP.h"
#include "iPod_HID.h"

#include "../iPod_error.h"

#ifndef NULL
#define NULL 0
#endif

#define TRUE 1
#define FALSE 0

#define LOG_TAG "IapService"

//static int IAPOS_DEBUG	   = 1;
int IAPOS_DEBUG = -1;
#define IAPOS_DBUG(msg...)	if (IAPOS_DEBUG == 1) { ALOGD( " " msg); }

int IAPOSQ_DEBUG = -1;
#define IAPOSQ_DBUG(msg...)	if (IAPOSQ_DEBUG == 1) { ALOGD( " " msg); }

void *TCC_IPOD0_SEM;
void *TCC_IPOD1_SEM;
void *TCC_IPOD2_SEM;
void *TCC_IPOD3_SEM;
void *TCC_IPOD4_SEM;

sem_t ipod0_sem;
sem_t ipod1_sem;
sem_t ipod2_sem;
sem_t ipod3_sem;
sem_t ipod4_sem;

typedef struct IPOD_OS_PTHREAD_st
{
	pthread_t mThread;
	pthread_mutex_t mMutex;
	pthread_cond_t  mRunCond;
}IPOD_OS_PTHREAD;


void *IPOD_MemAlloc(int length)
{
    return malloc(length);
}

int IPOD_MemDealloc(void *pMem)
{
    if (pMem)
    {
        free(pMem);
    }

    return 0;
}
void IPOD_MemSet(void *pBuffer, unsigned char ucVal, int length)
{
    memset(pBuffer,ucVal,length);
}

int IPOD_MemCopy(void *pDest,void *pSrc, int length)
{
    int retval=0;

    if (pDest && pSrc && length)
    {
        (void)memcpy(pDest,pSrc,length);
        retval=length;
    }

    return retval;
}

void IPOD_InitSemaphore(void)
{
	IPOD_ERROR ret_val;
	ret_val= IPOD_OS_CreateSemaphore(&ipod0_sem, 0);
	ret_val= IPOD_OS_CreateSemaphore(&ipod1_sem, 0);
	ret_val= IPOD_OS_CreateSemaphore(&ipod2_sem, 0);
	ret_val= IPOD_OS_CreateSemaphore(&ipod3_sem, 0);
	ret_val= IPOD_OS_CreateSemaphore(&ipod4_sem, 0);
	IPOD_InitVar(&ipod0_sem, &ipod1_sem, &ipod2_sem, &ipod3_sem, &ipod4_sem);
}

#define QUEUE_NAME_LENGTH_MAX (64)
#define QUEUE_BUFFER_SIZE (1024)
#define QUEUE_NODE_MAX  (5)

//#define NEW_IAP_QUEUE 1

#if NEW_IAP_QUEUE
typedef struct _queuenode
{
    int uid;
    unsigned char buffer[QUEUE_BUFFER_SIZE];
    unsigned long  bufferSize;
    struct _queuenode *next;
} QUEUENODE;

typedef struct _queue
{
    struct _queuenode *head;
    struct _queuenode *tail;
    struct _queuenode *deqNode;
    struct _queuenode *enqNode;
    int uid;
    int maxSize;
    int size;
//    pthread_mutex_t mutex;
    pthread_cond_t   evt_cond;
    pthread_mutex_t     evt_mutex;
    unsigned char name[QUEUE_NAME_LENGTH_MAX];
    QUEUENODE node[QUEUE_NODE_MAX]
} QUEUE;
#else
typedef struct _queuenode
{
    int uid;
    unsigned char *buffer;
    unsigned long  bufferSize;
    struct _queuenode *next;
} QUEUENODE;

#define QUEUE_NAME_LENGTH_MAX (64)

typedef struct _queue
{
    struct _queuenode *head;
    struct _queuenode *tail;
    struct _queuenode *deqNode;
    struct _queuenode *enqNode;
    int uid;
    int maxSize;
    int size;
//    pthread_mutex_t mutex;
    pthread_cond_t   evt_cond;
    pthread_mutex_t     evt_mutex;
    unsigned char name[QUEUE_NAME_LENGTH_MAX];
} QUEUE;
#endif

#define NANOSEC_PER_MILLISEC (1000000)
#define NSEC_PER_SEC (1000*NANOSEC_PER_MILLISEC)

QUEUE* queue_create(void)
{
    QUEUE* queue = (QUEUE*)IPOD_MemAlloc(sizeof(QUEUE));
    
    IPOD_MemSet(queue, 0, sizeof(QUEUE));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
//    pthread_mutex_init(&(queue->mutex), NULL);
    pthread_mutex_init(&(queue->evt_mutex), NULL);
    pthread_cond_init (&(queue->evt_cond), NULL);
    return queue;
}

int  queue_remove(QUEUE *queue)
{
    int ret = FALSE;
    QUEUENODE *tmpNode = NULL;
    // BEGIN OF ATOMIC REGION
    pthread_mutex_lock(&(queue->evt_mutex));

    #if NEW_IAP_QUEUE
    #else
    if(queue->head != NULL && queue->size > 0){
        do
        {
            tmpNode = queue->head;
            queue->head = queue->head->next;
            queue->size--;
            IPOD_MemDealloc(tmpNode);
        }
        while (queue->head != NULL);
    }
    #endif
    pthread_mutex_unlock (&(queue->evt_mutex));
    // END OF ATOMIC REGION
    pthread_mutex_destroy(&(queue->evt_mutex));

    IPOD_MemDealloc(queue);
    return TRUE;
}

//#define Q_DEBUG
int queue_enq(void *pQueue, void *buffer,unsigned long  bufferSize)
{
    int ret = FALSE;
    QUEUE *queue = (QUEUE *)pQueue;
    unsigned char *pMsgBuf = NULL;
    #ifdef Q_DEBUG
    pIPOD_LIB_msg ptest = buffer;
    unsigned short transid = 0;
    if(ptest!=NULL)
        if(ptest->pCmdParam!=NULL)
            if(ptest->pCmdParam->m_TransID!=NULL)
                transid  = ptest->pCmdParam->m_TransID;

    ALOGE(" >>>> %s() ENQUQUE start ++++++ %s, 0x%X",__FUNCTION__,queue->name,transid);
    #endif
    IAPOSQ_DBUG("%d: %s() start Q %p buffer %p Size %ld", __LINE__, __FUNCTION__,pQueue, buffer,bufferSize);

    if (queue == NULL)
        return ret;

    if (queue->size >= queue->maxSize)
        return ret;

    #if NEW_IAP_QUEUE
    if(bufferSize > QUEUE_BUFFER_SIZE){
        ALOGE("%d: %s() bufferSize(%ld) > QUEUE_BUFFER_SIZE(%d)", __LINE__, __FUNCTION__,bufferSize, QUEUE_BUFFER_SIZE);
        return ret;
    }
    #endif

    pthread_mutex_lock(&(queue->evt_mutex));
    queue->enqNode = NULL;

    #if NEW_IAP_QUEUE
    if(queue->head == NULL){
        queue->head = &queue->node[0];
        queue->head->next = NULL;
        queue->tail = queue->head;
    }else{
        QUEUENODE *pNode;
        int index;
        for(index = 0; index < QUEUE_NODE_MAX;index++){            
            if(queue->tail == &queue->node[index]){
                if(index == (QUEUE_NODE_MAX -1)){
                    pNode = &queue->node[0];
                }else{
                    pNode = &queue->node[index+1];
                }
                queue->tail->next = pNode;
                queue->tail = pNode;
                break;
            }                
        }
    }
    queue->enqNode = queue->tail;

    IPOD_MemSet(queue->enqNode, 0, sizeof(QUEUENODE));
    IPOD_MemCopy(queue->enqNode->buffer,buffer,bufferSize);
    queue->enqNode->bufferSize= bufferSize;
    
    #else
    queue->enqNode = (QUEUENODE*)IPOD_MemAlloc(sizeof(QUEUENODE) + bufferSize);
    if (queue->enqNode == NULL){
        #ifdef Q_DEBUG
        ALOGE(" >>>> %s() ENQUQUE no memory ************************** %s, 0x%X",__FUNCTION__,queue->name,transid);
        #endif
        pthread_mutex_unlock(&(queue->evt_mutex));
        return ret;
    }

    IPOD_MemSet(queue->enqNode, 0, sizeof(QUEUENODE));
    pMsgBuf = (unsigned char*)queue->enqNode;
    queue->enqNode->buffer = &pMsgBuf[sizeof(QUEUENODE)];

    IPOD_MemCopy(queue->enqNode->buffer,buffer,bufferSize);
    queue->enqNode->bufferSize= bufferSize;
    // BEGIN OF ATOMIC REGION
    queue->enqNode->next = NULL;
    queue->enqNode->uid = queue->uid;

    if (queue->tail != NULL)
        queue->tail->next = queue->enqNode;

    queue->tail = queue->enqNode;

    if (queue->head == NULL)
        queue->head = queue->enqNode;
    #endif
    queue->size++;
    if((queue->uid & 0xFFFFFF)  == 0xFFFFFF){
        queue->uid >>= 24; 
        queue->uid <<= 24;
    }else{
        queue->uid++;
    }
    pthread_mutex_unlock(&(queue->evt_mutex));
    // END OF ATOMIC REGION

    // BEGIN OF ATOMIC REGION
    pthread_mutex_lock( &queue->evt_mutex );
    pthread_cond_signal( &queue->evt_cond );
    pthread_mutex_unlock( &queue->evt_mutex );
    // END OF ATOMIC REGION
    #if NEW_IAP_QUEUE
    IAPOSQ_DBUG("%d: %s() end queue->enqNode  %p Size %ld", __LINE__, __FUNCTION__,queue->enqNode , bufferSize);
    #else
    IAPOSQ_DBUG("%d: %s() end queue->enqNode  %p Size %ld", __LINE__, __FUNCTION__,queue->enqNode , (sizeof(QUEUENODE) + bufferSize));
    #endif
    ret = TRUE;

    #ifdef Q_DEBUG
    ALOGE(" <<<< %s() ++++++ ENQUQUE end %s, 0x%X",__FUNCTION__,queue->name,transid);
    #endif

    return ret;
}


/* Returns null if queue is empty. */
int queue_deq(void *pQueue, void *buffer,unsigned long  bufferSize,unsigned long *readDataSize,unsigned long timeout)
{
    QUEUE *queue = (QUEUE *)pQueue;
    void *data = NULL;
    unsigned long dataLen = 0;
    int ret = FALSE;
    struct timespec abstime = { 0, 0 };
   #ifdef Q_DEBUG
    if(memcmp(queue->name, "IPOD_LIB_EVENT_QUEUE",20)
        && memcmp(queue->name, "IPOD_LIB_SEND_QUEUE",19)) 
        ALOGE(" >>>> %s() DEQUQUE start------1 %s",__FUNCTION__,queue->name);
    #endif
    IAPOSQ_DBUG("%d: %s() start Q %p msg %p msgSize %ld readSize %p timeout = %ld", __LINE__, __FUNCTION__,pQueue, buffer,bufferSize,readDataSize,timeout);
    if (queue == NULL)
    {
        dr_dbg("[ERROR]:%s(line=%d)",__FUNCTION__,__LINE__);
        return ret;
    }

    if (buffer == NULL)
    {
        dr_dbg("[ERROR]:%s(line=%d)",__FUNCTION__,__LINE__);
        return ret;
    }

    if (readDataSize == NULL)
    {
        dr_dbg("[ERROR]:%s(line=%d)",__FUNCTION__,__LINE__);
        return ret;
    }    

    #if NEW_IAP_QUEUE
    if(bufferSize > QUEUE_BUFFER_SIZE){
        ALOGE("%d: %s() bufferSize(%ld) > QUEUE_BUFFER_SIZE(%d)", __LINE__, __FUNCTION__,bufferSize, QUEUE_BUFFER_SIZE);
        return ret;
    }    
    #endif


    // BEGIN OF ATOMIC REGION
    pthread_mutex_lock(&(queue->evt_mutex));
    *readDataSize = 0;

    if (timeout > 0 && queue->head == NULL)
    {
        int ret1;
        int sec;
        int nano_sec;
        //pthread_mutex_unlock(&(queue->mutex));
        // END OF ATOMIC REGION
        clock_gettime(CLOCK_REALTIME, &abstime);
        sec = timeout / 1000;
        nano_sec = (timeout % 1000) * NANOSEC_PER_MILLISEC;
        abstime.tv_nsec += nano_sec;

        if (abstime.tv_nsec > NSEC_PER_SEC)
        {
            abstime.tv_sec += (abstime.tv_nsec / NSEC_PER_SEC);
            abstime.tv_nsec = abstime.tv_nsec % NSEC_PER_SEC;
        }

        abstime.tv_sec += sec;

        #ifdef Q_DEBUG
        if(memcmp(queue->name, "IPOD_LIB_SEND_QUEUE",19) == 0) {
            //ALOGE(" >>>> %s() DEQUQUE start------2 %s",__FUNCTION__,queue->name);
        }
        #endif

        //pthread_mutex_lock(&(queue->evt_mutex));	
        ret1 = pthread_cond_timedwait(&queue->evt_cond,&queue->evt_mutex,&abstime);

        if(ret1==110/*#define    ETIMEDOUT    110    -- Connection timed out --*/)
        {
        	ret = 2;
        }
        //pthread_mutex_unlock(&(queue->evt_mutex));	
        // BEGIN OF ATOMIC REGION
        //pthread_mutex_lock(&(queue->mutex));
    }else{
        #ifdef Q_DEBUG
        if(memcmp(queue->name, "IPOD_LIB_SEND_QUEUE",19) == 0) 
            ALOGE(" >>>> %s() DEQUQUE start------3 %s",__FUNCTION__,queue->name);    
        #endif
    }

    if (queue->head != NULL)
    {
        queue->deqNode = queue->head;
        dataLen = queue->deqNode->bufferSize;

        if (bufferSize < dataLen)
        {
            //IAPOS_DBUG("%d: %s() bufferSize %d bufferSize %d", __LINE__, __FUNCTION__,dataLen,bufferSize);
            dataLen = bufferSize;
        }

        IPOD_MemCopy(buffer,queue->deqNode->buffer,dataLen);
        *readDataSize = dataLen;

        queue->head = queue->deqNode->next;
        queue->size--;
        ret = TRUE;
        #if NEW_IAP_QUEUE
        if(queue->head == queue->tail){
            queue->head = NULL;
            queue->tail = NULL;            
        }
        #else
        if (queue->head == NULL)
        {
            queue->tail = NULL;
        }

        if (queue->deqNode != NULL)
        {
            IPOD_MemDealloc(queue->deqNode);
            IAPOSQ_DBUG("%d: %s() deqNode %p ", __LINE__, __FUNCTION__,queue->deqNode);
            queue->deqNode = NULL;
        }
        #endif

    }
    pthread_mutex_unlock(&(queue->evt_mutex));
    #ifdef Q_DEBUG
    if(ret == TRUE)
        ALOGE(" <<<< %s() ----- DEQUQUE end %s",__FUNCTION__,queue->name);
    #endif
    // END OF ATOMIC REGION
    IAPOSQ_DBUG("%d: %s() end %s readDataSize = %ld ", __LINE__, __FUNCTION__,queue->name,*readDataSize);
    return ret;
}

IPOD_ERROR IPOD_OS_CreateThread(void **handle,void *(*taskFunction)(void*),void *param)
{
    IPOD_ERROR retval = IPOD_INVALID_PARAM;

    IAPOS_DBUG("%d: %s() ", __LINE__, __FUNCTION__);

    if (handle && taskFunction)
    {
        int ret;
        
        IPOD_OS_PTHREAD *pIpodOsThread = (IPOD_OS_PTHREAD *)IPOD_MemAlloc(sizeof(IPOD_OS_PTHREAD));
        if(pIpodOsThread == NULL)
            return retval;

        pthread_mutex_init(&(pIpodOsThread->mMutex), NULL);
        pthread_cond_init(&pIpodOsThread->mRunCond, NULL);
        IAPOS_DBUG("%d: %s() pIpodOsThread %p", __LINE__, __FUNCTION__,pIpodOsThread);

        #if 0
        pthread_attr_t attr;
        int max,min;
        struct sched_param param1;
        int Policy = SCHED_RR;
        // for test, for checking the thead priority      
        pthread_attr_init(&attr); //SCHED_FIFO

        if(pthread_attr_setschedpolicy(&attr, Policy)){
            IAPOS_DBUG("%d: %s() error pthread_attr_setschedpolicy ", __LINE__, __FUNCTION__);
        }
        param1.sched_priority = sched_get_priority_max(Policy);
        if(pthread_attr_setschedparam(&attr, &param1)){
            IAPOS_DBUG("%d: %s() error pthread_attr_setschedparam ", __LINE__, __FUNCTION__);
        }
        ret=pthread_create(pThread,&attr,(void*)taskFunction,param);
        #else  
        *handle=pIpodOsThread;
        pthread_mutex_lock(&pIpodOsThread->mMutex);
        ret=pthread_create(&pIpodOsThread->mThread,NULL,(void*)taskFunction,param);
        pthread_cond_wait(&pIpodOsThread->mRunCond, &pIpodOsThread->mMutex);
        pthread_mutex_unlock(&pIpodOsThread->mMutex);        
        #endif

        IAPOS_DBUG("%d: %s() ret = %d, %p", __LINE__, __FUNCTION__,ret, pIpodOsThread);

        if (ret<0)
        {
            (void)IPOD_MemDealloc(pIpodOsThread);
            retval = IPOD_INVALID_HANDLE;
        }
        else
        {
            retval = IPOD_NO_ERROR;
            #if 0
            // for test, for checking the thead priority
            {
                if(pthread_getschedparam(*pThread, &Policy, &param1) == 0){
                    ALOGE("%d: %s() priority %d policy %d piority %d",
                                                                                            __LINE__, 
                                                                                            __FUNCTION__,
                                                                                            param1.sched_priority,
                                                                                            Policy,
                                                                                            param1.sched_priority
                                                                                            );
                }
            }
            #endif
        }
    }
    IAPOS_DBUG("%d: %s() retval = %d", __LINE__, __FUNCTION__,retval);

    return retval;
}

IPOD_ERROR IPOD_OS_CloseThread(void *handle)
{
    IPOD_ERROR retval=IPOD_INVALID_PARAM;
	int ret;
    IAPOS_DBUG("%d: %s() %p", __LINE__, __FUNCTION__,handle);
    if (handle)
    {
        int status;
        IPOD_OS_PTHREAD *pIpodOsThread = (IPOD_OS_PTHREAD *)handle;
        //ret = pthread_cancel(*(pthread_t*)handle);
        ret = pthread_join(pIpodOsThread->mThread,(void**)&status);
        pthread_cond_destroy(&pIpodOsThread->mRunCond);
        pthread_mutex_destroy(&pIpodOsThread->mMutex);        
        IAPOS_DBUG("%d: %s() ret = %d", __LINE__, __FUNCTION__,ret);
        if(ret == 0)
        {
        	(void)IPOD_MemDealloc(handle); 
	        retval=IPOD_NO_ERROR;
	    }
	    else
	    {
	    	retval = IPOD_THREAD_ERROR;
	    }
    }
    IAPOS_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    return retval;
}

void IPOD_OS_ExitThread(void *returnvalue)
{
	ALOGI("%d: %s() %s", __LINE__, __FUNCTION__,returnvalue);
	pthread_exit(returnvalue);
}

void IPOD_OS_SendSignal(void *handle)
{
    IPOD_OS_PTHREAD *pIpodOsThread = (IPOD_OS_PTHREAD *)handle;
    ALOGI(" %s pIpodOsThread %p",  __FUNCTION__,pIpodOsThread);
    pthread_mutex_lock(&pIpodOsThread->mMutex);
    pthread_cond_signal(&pIpodOsThread->mRunCond);
    pthread_mutex_unlock(&pIpodOsThread->mMutex);    
}

int gUidStart = 1;
IPOD_ERROR IPOD_OS_CreateMessageQueue(void **hWriteQueue,void **hReadQueue,unsigned char *messageQueueName, unsigned long messageSize)
{
    IPOD_ERROR retval=IPOD_INVALID_PARAM;
    IAPOS_DBUG("%d: %s() (%s)", __LINE__, __FUNCTION__,messageQueueName);
    if (hWriteQueue && hReadQueue)
    {
        if (messageQueueName)
        {
            void *ProcessHandle;
            unsigned long Error;
            QUEUE *queue = NULL;
            int nameLen = strlen((const char *)messageQueueName);

            if( nameLen >= (QUEUE_NAME_LENGTH_MAX-1))
                nameLen = (QUEUE_NAME_LENGTH_MAX-1);
            queue= queue_create();
            IPOD_MemSet(queue,0x00,sizeof(QUEUE));
            #ifdef IAP_USE_TCC_QUEUE
            queue->maxSize = 256;
            #else
            queue->maxSize = 5;
            #endif
            queue->uid = (gUidStart << 24);
            gUidStart <<= 1;
            IPOD_MemCopy(queue->name, messageQueueName, nameLen);
            *hWriteQueue = queue;
            *hReadQueue = queue;
#if 0
            queue= queue_create();
            IPOD_MemSet(queue,0x00,sizeof(QUEUE));
            queue->maxSize = 5;
            *hReadQueue = queue;
#endif

            if (*hWriteQueue && *hReadQueue)
            {
                retval = IPOD_NO_ERROR;
            }
            else
            {
                retval=IPOD_MSGQ_OPEN_ERROR;
            }
        }
        else
        {
            retval=IPOD_INVALID_PARAM;	// queue name is not presented or not supported
        }
    }
    IAPOS_DBUG("%d: %s() end", __LINE__, __FUNCTION__);
    return retval;
}

IPOD_ERROR IPOD_OS_SendToMessageQueue(void *hQueue, void *message, unsigned long size)
{
    IPOD_ERROR retval = IPOD_INVALID_PARAM;
    int ret;

    //IAPOS_DBUG("%d: SendQ(%s) start msg %p msgSize %d", __LINE__, ((QUEUE *)hQueue)->name, message,size);

    if (hQueue && message && size)
    {
        ret = queue_enq((QUEUE *)hQueue, message, size);

        if (ret != TRUE)
        {
            retval = IPOD_MSGQ_WRITE_ERROR;
        }
        else
        {
            retval = IPOD_NO_ERROR;
        }
    }
    //IAPOS_DBUG("%d: ReSendQadQ(%s) end bufferSize %d Size %d", __LINE__, ((QUEUE *)hQueue)->name,message,size);

    return retval;
}

IPOD_ERROR IPOD_OS_ReadMessageQueue(void *hQueue, void *messageBuffer, unsigned long messageBufferSize, unsigned long *readSize, unsigned long timeout, unsigned long *flags)
{
	int ret;
    unsigned long dataLen;
    IPOD_ERROR result = IPOD_INVALID_HANDLE;
    //IAPOS_DBUG("%d: ReadQ(%s) start msg %p msgSize %d pReadSize %p ", __LINE__, ((QUEUE *)hQueue)->name, messageBuffer,messageBufferSize,readSize);

    if (hQueue && messageBuffer && messageBufferSize && readSize)
    {
        IPOD_MemSet(messageBuffer, 0x00, messageBufferSize);
        ret = queue_deq(hQueue,messageBuffer, messageBufferSize, readSize,timeout);
        
        if (ret==TRUE)
        {
            result = IPOD_NO_ERROR;
        }
        else if (ret == 2)
        {
        	result = IPOD_MSGQ_TIMEOUT_ERROR;
        }
        else
        {
            result = IPOD_MSGQ_READ_ERROR;
        }
    }
    //IAPOS_DBUG("%d: ReadQ(%s) end bufferSize %d readSize %d", __LINE__, ((QUEUE *)hQueue)->name,messageBufferSize,*readSize);

    return result;
}

IPOD_ERROR IPOD_OS_CloseMessageQueue(void *hWriteQueue,void *hReadQueue)
{
    IPOD_ERROR result = IPOD_INVALID_HANDLE;
    int ret;
    IAPOS_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    if (hWriteQueue && hReadQueue)
    {
        ret = queue_remove((QUEUE *)hWriteQueue);

        if (ret != TRUE)
        {
            result = IPOD_MSGQ_CLOSE_ERROR;
        }
        else
        {
            result = IPOD_NO_ERROR;
            /*
            ret = queue_remove((QUEUE *)hReadQueue);

            if (ret != TRUE)
            {
                result = IPOD_MSGQ_CLOSE_ERROR;
            }
            else
            {
                result = IPOD_NO_ERROR;
            }
            */
        }
    }
    IAPOS_DBUG("%d: %s() ", __LINE__, __FUNCTION__);
    return result;
}

unsigned long IPOD_OS_SleepForCPTimeMs(void)
{
	unsigned long ret;
	ret = 100;

	return ret;
}

/**************************************************************************
*	Function : IPOD_OS_Sleep
*
*	Description : This function is callback function of iPod library
*
*	Parameter :
*		- ticks :
*
*  	Return : none
*
**************************************************************************/
unsigned long IPOD_OS_Sleep(void *ExtInfo, unsigned short time_ms)
{
    unsigned long i;
    unsigned long ticks = 0;
    #if 0
    ticks = time_ms/5;

    if (time_ms%5)
    {
        ticks++;
    }
	#endif
//    for (i=0; i<ticks; i++)
    for (i=0; i<time_ms; i++)
    {
        //IAPOS_DBUG("%d: %s() %p", __LINE__, __FUNCTION__,ExtInfo);
        if (IPOD_GetDetectStatus(ExtInfo) == IPOD_DISCONNECTED)
        {
            break;
        }
        else
        {
            //IAPOS_DBUG("%d: %s() start sleep", __LINE__, __FUNCTION__);
            /*Add 5ms time delay*/
            usleep(1000);
            //IAPOS_DBUG("%d: %s() end sleep", __LINE__, __FUNCTION__);
        }
    }

    return i * 5;
}

void IPOD_OS_InitDebug(void)
{
    //if(IAPOS_DEBUG == -1)
    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.debug.os", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPOS_DEBUG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPOS_DEBUG = 0;
                break;
        }
    }

    //if(IAPOSQ_DEBUG == -1)
    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.debug.osq", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPOSQ_DEBUG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPOSQ_DEBUG = 0;
                break;
        }
    }    
}

IPOD_ERROR IPOD_OS_CreateSemaphore(void *sema, unsigned long count)
{
    IPOD_ERROR retval=IPOD_INVALID_PARAM;

    IPOD_OS_InitDebug();
    
    if (sem_init(sema, 0, count) == 0)
    {
        retval = IPOD_NO_ERROR;
    }
    else
    {
        dr_dbg("[ERROR]:%s(line=%d)",__FUNCTION__,__LINE__);
    }

    return retval;
}

IPOD_ERROR IPOD_OS_GetSemaphore(void *sema, unsigned long timeout)
{
    IPOD_ERROR retval=IPOD_INVALID_PARAM;

    if (sema)
    {
        if (sem_wait((sem_t *)sema) == 0)
        {
            retval = IPOD_NO_ERROR;
        }
        else
        {
            dr_dbg("[ERROR]:%s(line=%d)",__FUNCTION__,__LINE__);
        }
    }
    else
    {
        dr_dbg("[ERROR]:%s(line=%d)",__FUNCTION__,__LINE__);
    }

    return retval;
}

IPOD_ERROR IPOD_OS_ReleaseSemaphore(void *sema)
{
    IPOD_ERROR retval;

    if (sem_post((sem_t *)sema) == 0)
    {
        retval = IPOD_NO_ERROR;
    }
    else
    {
        dr_dbg("[ERROR]:%s(line=%d)",__FUNCTION__,__LINE__);
    }

    return retval;
}

/*it uses ReleaseSemaphore to increase the count to its maximum value to permit normal access to the protected resource.*/

IPOD_ERROR IPOD_OS_ResetSemaphore(void *sema, unsigned long count)
{
    IPOD_ERROR retval=IPOD_INVALID_PARAM;
    unsigned long  index;

    for (index = 0; index < count; index++)
    {
        if (sem_post((sem_t *)sema) != 0)
            break;
    }

    if (index ==  count)
    {
        retval = IPOD_NO_ERROR;
    }
    else
    {
        dr_dbg("[ERROR]:%s(line=%d)",__FUNCTION__,__LINE__);
    }

    return retval;
}

unsigned long IPOD_OS_GetTimeTick(void)
{
    unsigned long retval;
    #if 0
    retval = times(0);
    #else
    unsigned long value = 0;
    struct timespec t;
    t.tv_sec = t.tv_nsec = 0;
    clock_gettime(CLOCK_MONOTONIC, &t);
    value =  t.tv_sec*1000 + t.tv_nsec/1000000;
    retval = value;
    #endif
    //ALOGI("%s %u 0x%x",__FUNCTION__,retval,retval);
    return retval;
}


