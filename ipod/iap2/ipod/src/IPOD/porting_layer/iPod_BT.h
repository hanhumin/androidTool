/****************************************************************************
 *   FileName    : iPod_BT.h
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


#ifndef _IPOD_BT_H_
#define _IPOD_BT_H_

#include "../iPod_lib.h"
typedef struct
{
	/*add to user context*/
	unsigned long padding;
} IPOD_BT_CONTEXT;

typedef enum
{
	BT_DEVICE_DISCONNECTED = 0,
	BT_DEVICE_CONNECTED
} BT_PORT_STATUS;

extern void *IPOD_BT_Init(void);
extern void IPOD_BT_Deinit(void *piPodPALHandle);
extern IPOD_ERROR IPOD_BT_CheckiPodDevice(IPOD_BT_CONTEXT *piPodPALHandle);
extern int IPOD_BT_SetBTChannel(IPOD_BT_CONTEXT *piPodContext, void *driver_id);
#endif /* _IPOD_HID_H_ */