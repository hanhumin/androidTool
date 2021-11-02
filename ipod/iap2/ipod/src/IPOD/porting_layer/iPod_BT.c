/****************************************************************************
 *   FileName    : iPod_BT.c
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

#include "iPod_PAL.h"
#include "iPod_CP.h"
#include "iPod_OS.h"
#include "iPod_BT.h"

void *IPOD_BT_Init(void)
{
	return 0;
}
void IPOD_BT_Deinit(void *piPodPALHandle)
{
}

IPOD_ERROR IPOD_BT_CheckiPodDevice(IPOD_BT_CONTEXT *piPodContext)
{
	return IPOD_NO_ERROR;
}

int IPOD_BT_SetBTChannel(IPOD_BT_CONTEXT *piPodContext, void *driver_id)
{
	return 0;
}


unsigned long IPOD_BT_SendPacket(IPOD_IO_HANDLE *iPodIOHandle, unsigned char *pBuffer,long length)
{
	return 0;
}
