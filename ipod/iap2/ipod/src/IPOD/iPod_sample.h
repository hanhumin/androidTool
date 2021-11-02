/****************************************************************************
 *   FileName    : iPod_sample.h
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


#ifndef _IPOD_API_H_
#define _IPOD_API_H_

#include "iPod_error.h"
#include "iPod_lib.h"

#if defined(__cplusplus)
extern "C"
{
#endif
    /*************************************************************************/
    /*IPOD sample function                                                   */
    /*************************************************************************/
    extern void IPOD_Sample_InitDebug(void);
    extern long IPOD_Sample_ResetVideoSupportDB(void *iPod_Handle, int *totalNumber);
    extern long IPOD_Sample_DB_Init(void *iPod_Handle);
    extern long IPOD_Sample_SetCustomLogo(void *iPod_Handle, unsigned long Block);
    extern void IPOD_Sample_CheckPlayStatus(void *iPod_Handle);
    extern pIPOD_PLAY_INFO IPOD_Sample_Get_AudioPlayInfo(void *iPod_Handle);
    extern IPOD_ERROR IPOD_Sample_Transfer_Accessory_to_iOSApp(void *iPod_Handle, unsigned char *SendBuff, unsigned long size,unsigned long sessionID);
    extern IPOD_ERROR iPod_ExampleCode_getLingoProtocolVer(void *iPod_Handle, unsigned long Lingo, unsigned char *inform, unsigned char *major, unsigned char *minor);
    extern IPOD_ERROR IPOD_Sample_GetDRPlayStatus(void *iPod_Handle, IPOD_DR_PLAY_STATUS *playStatus);
    extern void IPOD_Sample_SetDRPlayStatus(void *iPod_Handle, IPOD_DR_PLAY_STATUS playStatus);
    extern IPOD_ERROR IPOD_Sample_GetCurrentIndex(void *iPod_Handle, long *pIndex);
    extern void IPOD_Sample_SetCurrentIndex(void *iPod_Handle, long Index);
    extern IPOD_ERROR IPOD_Sample_GetRepeat(void *iPod_Handle, unsigned char *pRepeat);
    extern void IPOD_Sample_SetRepeat(void *iPod_Handle, int repeat);
    extern IPOD_ERROR IPOD_Sample_GetShuffle(void *iPod_Handle, unsigned char *pShuffle);
    extern void IPOD_Sample_SetShuffle(void *iPod_Handle, int shuffle);
    extern IPOD_ERROR IPOD_Sample_GetPosition(void *iPod_Handle, unsigned long *position);
    extern void IPOD_Sample_SetPostion(void *iPod_Handle, unsigned long position);
    extern IPOD_ERROR IPOD_Sample_GetDuration(void *iPod_Handle, unsigned long *duration);
    extern void IPOD_Sample_SetDuration(void *iPod_Handle, unsigned long duration);
    extern IPOD_ERROR IPOD_Sample_SetDefaultArtworkFormatID(void *iPod_Handle, unsigned long Lingo);
    extern IPOD_ERROR iPod_ExampleCode_getLingoProtocolVer(void *iPod_Handle, unsigned long Lingo, unsigned char *inform, unsigned char *major, unsigned char *minor);
    extern void setRepeatState(int flag);
    extern void setShuffleMode(int flag);
#if defined(__cplusplus)
};
#endif

#endif	/* _IPOD_API_H_ */

/* End of File */


