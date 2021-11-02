/****************************************************************************
 *   FileName    : iPod_error.h
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



#ifndef _IPOD_ERROR_H_
#define _IPOD_ERROR_H_


typedef enum
{
	IPOD_NO_ERROR = 0x00,
	IPOD_UNKNOWN_DATABASE_CATEGORY = 0x01,
	IPOD_COMMAND_FAILED = 0x02,
	IPOD_OUT_OF_RESOURCE = 0x03,
	IPOD_BAD_PARAMETER = 0x04,
	IPOD_UNKNOWN_COMMAND = 0x05,
	IPOD_CMD_PENDING = 0x06,
	IPOD_ACCESSORY_NOT_AUTHENTICATED = 0x07,
	IPOD_BAD_AUTHENTICATION_VERSION = 0x08,
	IPOD_ACCESSORY_POWER_MODE_REQUEST_FAILED = 0x09,
	IPOD_CERTIFICATE_INVALID = 0x0A,
	IPOD_CERTIFICATE_PERMISSION_INVALID = 0x0B,
	IPOD_FILE_IS_IN_USE	= 0x0C,
	IPOD_INVALID_FILE_HANDLE = 0x0D,
	IPOD_DIRECTORY_NOT_EMPTY = 0x0E,
	IPOD_OPERATION_TIMED_OUT = 0x0F,
	IPOD_CMD_UNAVILABLE_IN_THIS_IPOD_MODE = 0x10,
	IPOD_INVALID_ACCESSORY_REGISTOR_ID_VAL = 0x11,
	IPOD_SELECTION_NOT_GENIUS = 0x12,
	IPOD_MULTISECTION_DATA_SELECTION_REVEICED_SUCCESSFULLY = 0x13,
	IPOD_LINGO_BUSY = 0x14,
	IPOD_MAXIMUM_NUMBER_OF_ACCESSORY_CONNECTIONS_ALREADY_REACHED = 0x15,
	IPOD_HID_DESCRIPTOR_INDEX_ALREADY_IN_USE = 0x16,
	IPOD_DROPPED_DATA = 0x17,
	IPOD_ATTEMPT_TO_ENTER_IPOD_OUT_MODE_WITH_INCOMPATIBLE_VIDEO_SETTING = 0x18,

	IPOD_INTERNAL_LIB_ERROR_START = 0xFF,
	
	IPOD_INVALID_PARAM=0x100, 				/* iPod library gets the invalid parameter Just NULL pointer check */
	IPOD_INVALID_HANDLE,
	IPOD_INVALID_TX_BUFFER,
	IPDO_INVALID_BASIC_FORMATID,
	IPOD_INVALID_PARAM_FROM_IPOD,

	IPOD_IDPS_ERROR=0x200,
	IPOD_IDPS_RETRY_IDPS,				/* etry Init !!*/
	IPOD_IDPS_TIMEOUT,
	IPOD_IDPS_ERROR_STATE,
	IPOD_IDPS_RETRY_STARTIDPS,
	IPOD_IDPS_CANCEL_FAIL,
	
	IPOD_MSGQ_OPEN_ERROR=0x300,				/* iPod Library Message Queue Error*/
	IPOD_MSGQ_READ_ERROR,				/* iPod Library Message Queue Error	*/
	IPOD_MSGQ_WRITE_ERROR,				/* iPod Library Message Queue Error	*/
	IPOD_MSGQ_CLOSE_ERROR, 				/* iPod Library Message Queue Error	*/
	IPOD_MSGQ_TIMEOUT_ERROR,			/* iPod Library Message Queue Error	*/
	
	IPOD_COMMAND_ACCEPTED=0x400,
	IPOD_COMMAND_TIMEOUT,
	IPOD_COMMAND_MISMATCH,
	IPOD_COMMAND_SYNC_ERROR,
	IPOD_COMMAND_NOT_SUPPORTED,
		
	IPOD_TOKEN_PARSING_ERROR=0x500,			/* parsing Error of FID token ACK.*/
	IPOD_TOKEN_ERROR, 					/* Error of FID token ACK.*/
	IPOD_TOKEN_NO_VALUE,

	IPOD_AUTH_INFO_ERROR=0x600,
	IPOD_AUTH_SIG_ERROR,
	IPOD_AUTH_CP_ERROR,
	IPOD_AUTH_CP_INIT_ERROR,
	IPOD_AUTH_CERTIFICATE_READ_ERROR,
	IPOD_AUTH_INFO_TIMEOUT_ERROR,
	IPOD_AUTH_NO_RESPONSE_TIMEOUT_ERROR,
	IPOD_AUTH_INFO_DATA_ERROR,
	
	IPOD_IO_CHANNEL_ERROR=0x700, 				/*	UART or USB IO interface Errorwrite error*/
	IPOD_IO_CP_ERROR, 					/* I2C interface Error of authentication IC.read or write error */
	IPOD_IO_CHECKSUM_ERROR,
	IPOD_IO_HANDLE_MALLOC_ERROR,
	IPOD_CONTEXT_ERROR,					/* If the iPod is not initialized, It is not prepared the library context */
	IPOD_THREAD_ERROR,					/* iPod library thread error wrong thread ID or thread is not running.	*/
	IPOD_SEMA_ERROR,					/* semaphore errorIPOD_API_Init() should be called.*/
	IPOD_MALLOC_ERROR,
	IPOD_MALLOC_FREE_ERROR,
	IPOD_BUFFER_ERROR,
	IPOD_BUFFER_LENGTH_ERROR,
	IPOD_OVERFLOW_TX_BUFFER,
	IPOD_BUFFER_OVERFLOW,
	IPOD_WORK_QUEUE_OVERFLOW ,
	IPOD_CONNECTION_TYPE_ERROR,
	IPOD_CHECKDEVICE_ERROR,
	IPOD_CHANNEL_ERROR,
	
	IPOD_ALREADY_INIT=0x800, 					/* Initialize error of IPOD_init_XXX function.*/
	IPOD_OPEN_ERROR,
	IPOD_HAL_OPEN_ERROR, 
	IPOD_NOT_EXIST,
	IPOD_DISCONNECT_ERROR,				/* iPod is disconnected.*/
	IPOD_TIMOUT_ERROR,					/* There is no response of specific command */
	IPOD_NO_DATA, 						/*	There is no return data.*/	
	IPOD_TRANSACTION_ERROR,				/* Packet Transaction Error (under IDPS) */
	IPOD_DATA_PARSING_ERROR,			/* Invalid return data from iPod */
	IPOD_CLASSID_NOT_MATCHED,
	IPOD_UNKNOWN_TRACKID,				/*IC2 ACK*/
	IPOD_COMMAND_PENDING,				/*IC2 ACK*/
	IPOD_NOT_AUTHENTICATED,				/*IC2 ACK*/
	IPOD_MISMATCHED_AUTH_PROTOCOL_VER,	/*IC2 ACK*/
	IPOD_DATA_PASING_FINISHED,
	IPOD_NO_DATA_TO_PASING,
	IPOD_NOT_ESTABLISHED_SESSION_ERROR, /*Data Transfer Session*/
	IPOD_DATA_PASING_ERROR,
	
	IPOD_HID_WRITE_ERROR = 0x900,
	IPOD_HID_READ_ERROR = 0x901,
	IPOD_SEND_DUPLICATION_ERROR = 0x902,
	
	IPOD_OS_CREATE_THREAD_ERROR = 0x1000,
	IPOD_OS_CLOSE_THREAD_ERROR = 0x1001,
	IPOD_OS_CREATE_QUEUE_ERROR = 0x1002,
	IPOD_OS_CLOSE_QUEUE_ERROR = 0x1003,
	IPOD_OS_READ_MSGQUEUE_ERROR = 0x1004,
	
	IPOD_STATE_WAIT_ATS_TIME = 0x1100,
	
	IPOD_UNKNOWN_ERROR=0x9000,
	IPOD_TIMELIMIT_ERROR=0x9001,
	IPOD_INIT_TIMEOUT_ERROR=0x9002,
	IPOD_MAX_ERROR = 0x7FFFFFFF
} IPOD_ERROR;

#endif	// _IPOD_ERROR_H_
