/****************************************************************************
 *   FileName    : iPod_CP.h
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



#ifndef _IPOD_CP_H_
#define _IPOD_CP_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#include "../iPod_lib.h"
#define AUTH_IC_20B
//#define AUTH_IC_20C
#define	AUTH_IC_I2C_ADDR	0x20
typedef struct
{
	void *i2c_handle;
	unsigned long i2c_slaveaddr;
	unsigned long i2c_channelnumber;
	unsigned long i2c_retries;    
	int cpState;
	void *gxp_handle;
} IPOD_AUTHCP_CONTEXT;
extern void IPOD_CP_InitDebug(void);
extern IPOD_ERROR IPOD_CP_Init(void *ExtInfo, IPOD_IO_HANDLE *iPodIOHandle);
extern long IPOD_CP_Read(IPOD_IO_HANDLE *iPodIOHandle, unsigned char *ptr, unsigned char SUBADDR, unsigned long length);
extern long IPOD_CP_Write(IPOD_IO_HANDLE *iPodIOHandle, unsigned char *ptr, unsigned char SUBADDR, unsigned long length);
extern void *IPOD_AuthCP_Init(void);
extern IPOD_ERROR IPOD_AuthCP_Deinit(void *piPodAuthHandle);
extern void IPOD_CP_Clear();
extern int IPOD_Check_WithIap2(void);
#if defined(__cplusplus)
};
#endif
#endif /*_IPOD_CP_H_*/