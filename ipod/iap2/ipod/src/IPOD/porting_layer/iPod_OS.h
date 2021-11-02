/****************************************************************************
 *   FileName    : iPod_OS.h
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


#ifndef _IPOD_OS_H_
#define _IPOD_OS_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#include "../iPod_error.h"
#include "../iPod_lib.h"
#include <semaphore.h>

//	#define TCC890X
//	#define TCC880X
//	#define TCC892X
#define TCC893X

//#define IAP_USE_TCC_QUEUE

//#define DEBUG
#ifdef  DEBUG
#define dr_dbg(format, arg...) printf( "dr:"format"\n", ##arg)
#else
#define dr_dbg(format, arg...) do {} while(0)
#endif

#define IPOD_TIME_TICK 1

#ifndef  TRUE
#define  TRUE 1
#endif
#ifndef  FALSE
#define  FALSE 0
#endif

	/* IPOD NO OPERATION to process the event*/
#define IPOD_NO_OP 0

#define IPOD_TIMEOUT_INFINITE 0xFFFFFFFF
#define IPOD_READ_QUEUE_TIMEOUT 1000 /*1 second*/



	extern void *IPOD_MemAlloc(int length);
	extern int IPOD_MemDealloc(void *pMem);
	extern void IPOD_MemSet(void *pBuffer, unsigned char ucVal, int length);
	extern int IPOD_MemCopy(void *pDest,void *pSrc, int length);

	/**************************************************************************
	* This define is just for using semaphore in all iPod APIs to avoid the
	* Multiple executed function on other task each.
	*
	**************************************************************************/
#define IPOD_USING_SEMA
#ifdef IPOD_USING_SEMA
#define IPOD_SEMA_SUSPEND 0xFFFFFFFF
#define IPOD_SEMA_NO_SUSPEND 0
#define IPOD_SEMA_SUCCESS IPOD_NO_ERROR
#define IPOD_SEMA_RELEASE_SUCCESS TRUE

    extern void IPOD_OS_InitDebug(void);
	extern IPOD_ERROR IPOD_OS_GetSemaphore(void *sema, unsigned long timeout);
	extern IPOD_ERROR IPOD_OS_ReleaseSemaphore(void *sema);
	extern IPOD_ERROR IPOD_OS_ResetSemaphore(void *sema, unsigned long count);
	extern unsigned long IPOD_OS_GetTimeTick(void);
#endif

	typedef enum{
		IPOD_NON_STATUS = 0x00,
		IPOD_CHANNEL_INITED_STATUS,
		IPOD_INIT_FINISHED_STATUS
	} IPOD_CMD_STATUS;

#if 0
	typedef struct tag_iPod_Thread_Condition
	{
		void *hiPodThread;
		bool ThreadStatus;
		bool ThreadStopFlag;
		IPOD_CMD_STATUS m_iPodCmdStatus;
	} iPod_Thread_Condition, piPod_Thread_Condition;
#endif

	typedef enum
	{
		IPOD_NO_TASK_CMD = 0x0000UL,

		IPOD_CHANNEL_INIT_CMD = 0x0100UL,
		IPOD_CHANNEL_INIT_RCV = 0x0200UL,
		IPOD_TASK_INIT_IPOD_CMD = 0x0300UL,
		IPOD_TASK_INIT_IPOD_RCV = 0x0400UL,

		IPOD_SET_STARTIDPS_STATUS_CMD = 0x0500UL,
		IPOD_SET_STARTIDPS_STATUS_RCV = 0x0600UL,

		IPOD_TASK_UNKNOWN_RCV = 1000UL,

		IPOD_TASK_CMD_MAX= 0x7FFFFFFFUL
	} IPOD_TASK_CMD;

#define IPOD_TASK_CMD_MASK 0x0000FF00UL


	typedef struct tag_IPOD_LIB_msg
	{
		IPOD_TASK_CMD TaskCmd;
		IPOD_TASK_CMD RcvTaskCmd;
		unsigned long iAPLingo;
		unsigned long iAPCommand;
		pIPOD_CMD_PARAM pCmdParam;
		void *ExtData;
		IPOD_ERROR result;
	} IPOD_LIB_msg, *pIPOD_LIB_msg;

	typedef struct tag_IPOD_EVT_msg
	{
		IPOD_EVENT_ID EventID;
		unsigned long Param1;
		unsigned long Param2;
		unsigned long Param3;
		unsigned long Param4;
		unsigned long Param5;

	} IPOD_EVT_msg, *pIPOD_EVT_msg;



	typedef struct tag_IPOD_Thread_param
	{
		pIPOD_CMD_PARAM pCmdParam;
		pIPOD_INIT_PARAM pParam;
	} IPOD_Thread_param, *pIPOD_Thread_param;

	typedef struct tag_IPOD_RCV_CMD
	{
		IPOD_TASK_CMD RcvTaskCmd;
		pIPOD_CMD_PARAM pRcvCmdParam;
		unsigned long RcvLingo;
		unsigned long RcviAPCmd;
	} IPOD_RCV_CMD, *pIPOD_RCV_CMD;

	extern sem_t ipod0_sem;
	extern sem_t ipod1_sem;
	extern sem_t ipod2_sem;
	extern sem_t ipod3_sem;
	extern sem_t ipod4_sem;

	/********************************/
	/* function                     */
	/********************************/
	extern void IPOD_InitSemaphore(void);
	extern IPOD_ERROR IPOD_OS_CreateThread(void **handle,void *(*taskFunction)(void*),void *param);
	extern IPOD_ERROR IPOD_OS_CloseThread(void *handle);
	extern void IPOD_OS_ExitThread(void *returnvalue);

	extern IPOD_ERROR IPOD_OS_CreateMessageQueue(void **hWriteQueue,void **hReadQueue,unsigned char *messageQueueName, unsigned long messageSize);
	extern IPOD_ERROR IPOD_OS_SendToMessageQueue(void *hQueue, void *message, unsigned long size);
	extern IPOD_ERROR IPOD_OS_ReadMessageQueue(void *hQueue, void *messageBuffer, unsigned long messageBufferSize, unsigned long *readSize, unsigned long timeout, unsigned long *flags);
	extern IPOD_ERROR IPOD_OS_CloseMessageQueue(void *hWriteQueue,void *hReadQueue);
	extern unsigned long IPOD_OS_SleepForCPTimeMs(void);
	extern unsigned long IPOD_OS_Sleep(void *ExtInfo, unsigned short time_ms);
	extern IPOD_ERROR IPOD_OS_CreateSemaphore(void *sema, unsigned long count);
	extern void IPOD_OS_SendSignal(void *handle);
#if defined(__cplusplus)
};
#endif
#endif	/* _IPOD_OS_H_ */
