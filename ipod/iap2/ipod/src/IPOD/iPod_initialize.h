/****************************************************************************
 *   FileName    : IPod_initialize.h
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



/*************************************************************************/
/*Command ID define                                                                                               */
/*************************************************************************/

#ifndef _IPOD_USB_INTERFACE_
#define _IPOD_USB_INTERFACE_


#if defined(__cplusplus)
extern "C"
{
#endif
    extern void IPOD_Malloc_iPodBufInit(void);
    extern void IPOD_API_InitDebug(void);
    extern void IPOD_API_SetAccInfoInit(void);
    extern void IPOD_API_SetAccName(char const*pData,int size);
    extern void IPOD_API_SetMenufacturer(char const *pData,int size);
    extern void IPOD_API_SetModelNumber(char const *pData,int size);
    extern void IPOD_API_SetFirmwareVerInAccInfo(
                                                                                        int majorNum,
                                                                                        int minorNum,
                                                                                        int revisionNum
                                                                                        );
    extern void IPOD_API_SetHardwareVerInAccInfo(
                                                                                        int majorNum,
                                                                                        int minorNum,
                                                                                        int revisionNum
                                                                                        );
    extern void IPOD_API_EAProtocolTokenInit(void);
    extern void IPOD_API_SetEAProtocolTokenInit(void);
    extern IPOD_ERROR IPOD_API_AddEAProtocol(
                                                                                    char const *pData,
                                                                                    int size
                                                                                    );
    extern IPOD_ERROR IPOD_API_SetBundleSeedIDString(
                                                                                    char const *pData,
                                                                                    int size
                                                                                    );
    extern IPOD_ERROR IPOD_API_SetEAProtocolMeta(int protocolIndex,int fMeta);

    extern MLingoType IPOD_ConvertSingleToMultiLingo(const IPOD_INIT_PARAM *pParam);//initialize
    extern IPOD_ERROR IPOD_API_USB_InitChannelParam(
                                                            pIPOD_INIT_CHANNER_PARAM pParam, 
                                                            void *Hid_Handle,
                                                            void *I2C_Handle, 
                                                            unsigned long EventThreadID, 
                                                            unsigned long I2C_ChannelNum, 
                                                            void *GXPDeviceHandle);
    extern IPOD_ERROR IPOD_API_USB_InitParam(
                                                            void *Handle, 
                                                            pIPOD_INIT_PARAM pParam, 
                                                            const IPOD_LINGO_INFO_DATA *pLingo_InfoData, 
                                                            
                                                            int fWithStorage);
 	extern IPOD_ERROR IPOD_API_InitChannelParam(pIPOD_INIT_CHANNER_PARAM pParam, void *pHandle, IPOD_CONNECTION_TYPE type, unsigned long driverID, unsigned long Block); 
	extern void IPOD_InitBuff(pIPOD_BUF_PARAM buf_param, const unsigned char *buffer,IPOD_CONNECTION_TYPE type, unsigned int Block);		/* QAC : 16.7 */
    extern void *IPOD_Malloc_iPodBuf(void); // need move to porting layer
	extern void IPOD_Free_iPodBuf(void *iPod_Handle);
    extern void IPOD_API_InitArtworkInfo(pIPOD_PLAY_INFO pPlayInfo);
    extern void IPOD_API_SetDisplayDataSize(int dataSize);

#if defined(__cplusplus)
};
#endif


#endif /* _IPOD_USB_INTERFACE_ */



