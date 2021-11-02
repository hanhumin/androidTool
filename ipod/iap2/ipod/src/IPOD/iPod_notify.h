/****************************************************************************
 *   FileName    : iPod_notify.h
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



extern long IPOD_Callback_GetReturnPlayInfo(void *iPod_Handle, char *pCmdData);
extern long IPOD_Callback_DisplayRemoteEvent(unsigned char EventNum, const unsigned char *pData, void *ExtInfo);
extern IPOD_TRACK_CAPABILITIES_INFO IPOD_Get_TrackCapabilities(const unsigned char * Data);
extern IPOD_ERROR IPOD_Callback_OpenDataSessionForProtocol( void *iPod_Handle, unsigned long SessionID, unsigned long nProtocolIndex );
extern IPOD_ERROR IPOD_Callback_CloseDataSession( void *iPod_Handle, unsigned long SessiongID );
extern IPOD_ERROR IPOD_Callback_iPodDataTransfer( void *iPod_Handle, unsigned long usSessionID, unsigned char *pData, unsigned long nDataSize );
extern long IPOD_Callback_iPodNotification(void* iPod_Handle, unsigned char EventNum, const unsigned char *pData);
extern void IPOD_Callback_EndAuthentication(void* iPod_Handle, unsigned long IsSuccess);
extern void IPOD_Callback_RequestIdentify(void* iPod_Handle);
extern void IPOD_Callback_SetDiskHandle(void* iPod_Handle);
extern void *IPOD_API_GetEventMsgReadQueue(void *iPod_Handle);