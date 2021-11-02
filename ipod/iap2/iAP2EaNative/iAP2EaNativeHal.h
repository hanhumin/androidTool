/****************************************************************************
 *   FileName    : iAP2EaNativeHal.h
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

#ifndef _IAP2_EA_NATIVE_HAL_H_
#define _IAP2_EA_NATIVE_HAL_H_

#define EA_NATIVE_BULK_SIZE      512
#define EA_NATIVE_DUMP_BUFFER_SIZE (EA_NATIVE_BULK_SIZE * 10)

class iAP2EaNativeHal 
{
    int mInited;
    int mFd;
    bool mDebug;
    bool mDumpDebug;
    bool mRead;
    bool mWrite;
    uint8_t *mDumpBuffer;

public:
    iAP2EaNativeHal(bool debug);
    ~iAP2EaNativeHal();
    void setDebug(bool debug);
    void dumpEa(const char* data, int dataLen);
    
    int     openEaNative(bool *opened);
    int     closeEaNative(void);
    int     getFd();
    int     writeEaNative(void* buffer, int size);
    int     readEaNative(void* buffer, int size);
};

#endif /*_IAP2_EA_NATIVE_HAL_H_*/

