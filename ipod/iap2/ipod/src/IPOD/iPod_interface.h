/****************************************************************************
 *   FileName    : iPod_interface.h
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

#ifndef _IPOD_INTERFACE_H_
#define _IPOD_INTERFACE_H_

#include "iPod_error.h"

#if defined(__cplusplus)
extern "C"
{
#endif

    typedef IPOD_ERROR (*openDataSessionForProtocol_callback)(unsigned long SessionID, unsigned long nProtocolIndex );
    typedef IPOD_ERROR (*closeDataSession_callback)(unsigned long SessiongID );
    typedef IPOD_ERROR (*iPodDataTransfer_callback)(unsigned long usSessionID, unsigned char *pData, unsigned long nDataSize );
    typedef IPOD_ERROR (*isValidSessionId_callback)(unsigned long SessiongID, IPOD_ERROR *result);
    typedef IPOD_ERROR (*iPODCbNotifyString_callback)(unsigned int notiType,unsigned char *pStr,int strLen );
    typedef IPOD_ERROR (*iPodCbTest1_callback)(unsigned long value );
    typedef IPOD_ERROR (*iPodCbTest2_callback)(unsigned long value,unsigned char *pData );
    typedef void (*iPodCbReqIdentify_callback)(void);
    typedef void (*iPodCbSetFullBandwidth_callback)(void);
    typedef void *(*iPodAlloc_communication_mem_callback)(unsigned long size);
    typedef void (*iPodAlloc_communication_free_callback)(void *pMem);
    typedef void (*iPodCetDetectStatus_callback)(void * pStatus);
    typedef int (*iPodCheckTimeForRepeat_callback)(void);
    typedef void (*iPodSetTimeForRepeat_callback)(void);
    typedef int (*iPodCheckTimeForShuffle_callback)(void);
    typedef void (*iPodSetTimeForShuffle_callback)(void);

    typedef struct {
        /** set to sizeof(bt_callbacks_t) */
        int size;
        openDataSessionForProtocol_callback openDataSessionForProtocol_cb;
        closeDataSession_callback closeDataSession_cb;
        iPodDataTransfer_callback iPodDataTransfer_cb;
        isValidSessionId_callback isValidSessionId_cb;
        iPODCbNotifyString_callback iPODCbNotifyString_cb;
        iPodCbTest1_callback iPodCbTest1_cb;
        iPodCbTest2_callback iPodCbTest2_cb;
        iPodCbReqIdentify_callback iPodCbReqIdentify_cb;
        iPodCbSetFullBandwidth_callback iPodCbSetFullBandwidth_cb;
        iPodAlloc_communication_mem_callback iPodAlloc_communication_mem_cb;
        iPodAlloc_communication_free_callback iPodAlloc_communication_free_cb;
        iPodCetDetectStatus_callback iPodCetDetectStatus_cb;
        iPodCheckTimeForRepeat_callback iPodCheckTimeForRepeat_cb;
        iPodSetTimeForRepeat_callback iPodSetTimeForRepeat_cb;
        iPodCheckTimeForShuffle_callback iPodCheckTimeForShuffle_cb;
        iPodSetTimeForShuffle_callback iPodSetTimeForShuffle_cb;
    } ipod_callbacks_t;

    typedef struct {
        /** set to sizeof(bt_interface_t) */
        int size;
        /**
         * Opens the interface and provides the callback routines
         * to the implemenation of this interface.
         */
        int (*init)(ipod_callbacks_t* callbacks );
    } ipod_interface_t;

    extern ipod_callbacks_t *ipod_stack_cbacks;

#define IPOD_CBACK(P_CB, P_CBACK, ...)\
        if (P_CB && P_CB->P_CBACK) {            \
            P_CB->P_CBACK(__VA_ARGS__);         \
        }                                       \
        else {                                  \
        }

extern const ipod_interface_t* iPod_get_ipod_interface ();


#if defined(__cplusplus)
};
#endif

#endif
