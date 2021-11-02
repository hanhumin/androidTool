
/****************************************************************************
 *   FileName    : iAP2Coprocessor.h
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


#ifndef _IAP2_COPROCESSOR_H_
#define _IAP2_COPROCESSOR_H_

#ifdef __cplusplus
namespace android{
extern "C" {
#endif

typedef struct
{
    void *i2c_handle;
    unsigned long i2c_slaveaddr;
    unsigned long i2c_channelnumber;
    void *gxp_handle;
    
    int auth20CReset;
    int cpVersion;
    int i2cChannel;
    int cpState;

    unsigned long i2c_retries;
} iAP2_AUTHCP_CONTEXT_t;
extern void iAP2CP_SetConnectionCallback(iAP2ChkConn_callback chkConn_cb);
extern int iAP2CP_Initialize(void *ExtInfo, void *iPodIOHandle);
extern long iAP2CP_Read(void *iPodIOHandle, unsigned char *ptr, unsigned char SUBADDR, unsigned long length);
extern long iAP2CP_Write(void *iPodIOHandle, unsigned char *ptr, unsigned char SUBADDR, unsigned long length);
extern void *iAP2CP_AuthCP_Init(void);
extern void ReadAuth_Prot_Ver(void);
extern void iAP2CP_State_Clear(void * data);
extern int iAP2CP_CheckVer(void *iPodIOHandle,int setFlag);
extern void *iAP2CP_Init();
extern void iAP2CP_Deinit(void * data);

#ifdef __cplusplus
}

}

#endif

#endif /*_IPOD_CP_H_*/
