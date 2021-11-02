/****************************************************************************
 *   FileName    : iAP2SessionControl.h
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


#ifndef _IAP2_SESSIONCONTROL_H_
#define _IAP2_SESSIONCONTROL_H_

#include "iAP2LinkRunLoop.h"
#include "iAP2Interface.h"

namespace android{

#if defined(__cplusplus)
extern "C"
{
#endif

enum{
    iap2_session_control_ver = 2,
    iap2_session_file_ver = 2,/*iap2_session_file_ver = 2 for ipod*/
    iap2_session_ea_ver = 1,
};

void iAP2SessCtrlSetMemDebug(BOOL flag);
void iAP2SessCtrlSetPckDebug(BOOL flag);

void iAP2SessCtrlInitMallocInfo();
void *iAP2SessCtrlMalloc(int size,const char *funcName,int lineNum);
void iAP2SessCtrlFree(void *pMem,const char *funcName);
void iAP2SessCtrlMallocDebug();

void iAP2SessCtrlLinkPacketDebugInit();
void iAP2SessCtrlLinkPacketAdd(BOOL isSend,void *pPck,int listIndex,int seq,int ack);
void iAP2SessCtrlLinkPacketDel(BOOL isSend,void *pPck,int listIndex);
void iAP2SessCtrlLinkPacketDebugPrint();

extern void iAP2SessCtrl_DebugTrigger(void *pControl,void *pEa,void *pFile);
extern void *iAP2SessCtrlInitSession(struct iAP2Link_st* link);
extern void iAP2SessCtrlDeinitSession(void *session);
extern void iAP2SessCtrlSetUsedSessionThread(BOOL flag);

extern void iAP2SessionCtrlGetVersion(uint8_t *pVersion, int bufLen,int *pStrLen);

extern void iAP2SessiCtrlPoolInit(void *pSessPool);

extern void iAP2SessiCtrlPoolClean(void *pSessPool);

extern int iAP2SessCtrlGetSupportedEAPCnt(void *pData);
extern int iAP2SessCtrlGetSupportedFileSession(void *pData);
extern int iAP2SessCtrlGetSupportedCarplay(void *pData);

//extern void *iAP2SessCtrlCreateControlSession(
   //                                                                                 iAP2Transport_t*     trans,
  //                                                                                  iAP2LinkRunLoop_t*  runloop,
    //                                                                                void *pData,
   //                                                                                 void *pCpData
   //                                                                                 );
//
extern void iAP2SessCtrlDestroyControlSession(void *pControl);

extern void *iAP2SessCtrlCreateEASession(iAP2Link_t* link, void *pControlSession,iAP2Notify_callback noti_cb);

extern void iAP2SessCtrlDestroyEASession(void *eapSession);

extern void *iAP2SessCtrlCreateFileSession(iAP2Link_t* link);

extern BOOL iAP2SessCtrlDestroyFileSession(void *pFileSession);



extern void iAP2SessCtrlSetSupportFlagCtrlSessMsg(
                                                                        void *controlSession,
                                                                        ControlSessionType_t     type, 
                                                                        BOOL flag
                                                                        );

extern BOOL iAP2SessCtrlUpdateVehicleStatus(
                                                                struct iAP2LinkRunLoop_st* runLoop,
                                                                void *pData
                                                                );

extern BOOL iAP2SessCtrlSendLocationInformation(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            void *pData);

extern int iAP2SessCtrlCopyCertificate(void  * pLink,void *reply);

extern int iAP2SessCtrlCreateSignature(void  * pLink,void *data,void *reply);


extern int iAP2SessCtrlCpInit(iAP2_CPInit_t cpinit_cb);

extern void iAP2SessCtrlCpDeinit(int handle,iAP2_CPDeinit_t cpdeinit_cb);

extern int iAP2SessCtrlCreateSignatureWithoutLink(int handle,
                                                                                    void *data,
                                                                                    void *reply,
                                                                                    IAP2_CPRead_t cpread_cb,
                                                                                    IAP2_CPWrite_t cpwrite_cb);

extern int iAP2SessCtrlCopyCertificateWithoutLink(int handle,
                                                                                    void *pData,
                                                                                    IAP2_CPRead_t cpread_cb,
                                                                                    IAP2_CPWrite_t cpwrite_cb);

extern BOOL iAP2SessCtrlSendPowerSourceUpdate(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            int support,
                                                            int availableCurrent);

extern BOOL iAP2SessCtrlTelephonyStartCallState(
                                                                struct iAP2LinkRunLoop_st* runLoop
                                                                );

extern BOOL iAP2SessCtrlTelephonyStopCallState(
                                                                struct iAP2LinkRunLoop_st* runLoop
                                                                );

extern BOOL iAP2SessCtrlTelephonyStartUpdates(
                                                                struct iAP2LinkRunLoop_st* runLoop
                                                                );

extern BOOL iAP2SessCtrlTelephonyStopUpdates(
                                                                struct iAP2LinkRunLoop_st* runLoop
                                                                );
extern BOOL iAP2SessCtrlCommunicationInitialCall(
                                                                struct iAP2LinkRunLoop_st* runLoop,
                                                                void *data
                                                                );
extern BOOL iAP2SessCtrlCommunicationAcceptCall(
                                                                struct iAP2LinkRunLoop_st* runLoop,
                                                                void *data
                                                                );
extern BOOL iAP2SessCtrlCommunicationEndCall(
                                                                struct iAP2LinkRunLoop_st* runLoop,
                                                                void *data
                                                                );
extern BOOL iAP2SessCtrlCommunicationSwapCalls(
                                                                struct iAP2LinkRunLoop_st* runLoop
                                                                );
extern BOOL iAP2SessCtrlCommunicationMergeCalls(
                                                                struct iAP2LinkRunLoop_st* runLoop
                                                                );
extern BOOL iAP2SessCtrlCommunicationHoldStatusUpdate(
                                                                struct iAP2LinkRunLoop_st* runLoop,
                                                                void *data
                                                                );
extern BOOL iAP2SessCtrlCommunicationMuteStatusUpdate(
                                                                struct iAP2LinkRunLoop_st* runLoop,
                                                                void *data
                                                                );
extern BOOL iAP2SessCtrlCommunicationSendDTMF(
                                                                struct iAP2LinkRunLoop_st* runLoop,
                                                                void *data
                                                                );

extern BOOL iAP2SessCtrlCommunicationStartUpdateList(
                                                                struct iAP2LinkRunLoop_st* runLoop,
                                                                void *data
                                                                );

extern BOOL iAP2SessCtrlCommunicationStopUpdateList(
                                                                struct iAP2LinkRunLoop_st* runLoop
                                                                );

extern BOOL iAP2SessCtrlNowPlayingStart(
                                                                struct iAP2LinkRunLoop_st* runLoop
                                                                );
extern BOOL iAP2SessCtrlNowPlayingStop(
                                                                struct iAP2LinkRunLoop_st* runLoop
                                                                );

extern BOOL iAP2SessCtrlNowPlayingSetInfo(
                                                                struct iAP2LinkRunLoop_st* runLoop,
                                                                void *data
                                                                );

extern BOOL iAP2SessCtrlStartPowerUpdates(
                                                                struct iAP2LinkRunLoop_st* runLoop
                                                                );

extern BOOL iAP2SessCtrlSendBtCompInfo(
                                                                struct iAP2LinkRunLoop_st* runLoop
                                                                );

extern BOOL iAP2SessCtrlSendBtCompInfoUpdate(
                                                                struct iAP2LinkRunLoop_st* runLoop,
                                                                void *data
                                                                );

extern BOOL iAP2SessCtrlStartBtConnectUpdate(
                                                                struct iAP2LinkRunLoop_st* runLoop
                                                                );

extern BOOL iAP2SessCtrlStopBtConnectUpdate(
                                                                struct iAP2LinkRunLoop_st* runLoop
                                                                );

extern BOOL iAP2SessCtrlAppLaunch(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            void *pData
                                                            );

extern BOOL iAP2SessCtrlSendEaData(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            void *data
                                                            );
extern BOOL iAP2SessCtrlSendEaSessionStatus(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            void *data
                                                            );
extern BOOL iAP2SessCtrlIndentificationUpdate(
                                                                struct iAP2LinkRunLoop_st* runLoop,
                                                                void *pData
                                                                );

extern BOOL iAP2SessCtrlGetCertificate(void *pContext,void *reply);

extern BOOL iAP2SessCtrlHidStart(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            void *pData
                                                            );

extern BOOL iAP2SessCtrlHidStop(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            void *pData
                                                            );

extern BOOL iAP2SessCtrlHidAccessoryReport(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            void *pData
                                                            );

extern BOOL iAP2SessCtrlMLAStartInformation(
                                                            struct iAP2LinkRunLoop_st* runLoop
                                                            );

extern BOOL iAP2SessCtrlMLAStopInformation(
                                                            struct iAP2LinkRunLoop_st* runLoop
                                                            );

extern BOOL iAP2SessCtrlMLAStartUpdate(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            void *pData
                                                            );

extern BOOL iAP2SessCtrlMLAStopUpdate(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            void *pData
                                                            );

extern BOOL iAP2SessCtrlMLACurrentSelection(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            void *pData
                                                            );

extern BOOL iAP2SessCtrlMLALibraryIitems(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            void *pData
                                                            );

extern BOOL iAP2SessCtrlMLACollection(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            void *pData
                                                            );

extern BOOL iAP2SessCtrlMLASpecial(
                                                            struct iAP2LinkRunLoop_st* runLoop,
                                                            void *pData
                                                            );

extern int iAP2SessCtrlMLAGetMaxLibraryItemsCount(struct iAP2LinkRunLoop_st* runLoop);

#if defined(__cplusplus)
};
#endif

}

#endif

