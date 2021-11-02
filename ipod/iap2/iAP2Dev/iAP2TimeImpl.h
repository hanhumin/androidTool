/****************************************************************************
 *   FileName    : iAP2TimeImpl.h
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

#ifndef IAP2_TIME_IMP_H
#define IAP2_TIME_IMP_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t iAP2TimeTick(void);
void iAP2TimespecNow(struct timespec *ts);
void iAP2TimespecAddMS(struct timespec *ts, uint32_t ms);
/*
uint32_t iAP2TimespecMS(struct timespec *ts);
*/
/*
uint64_t iAP2TimeTick64(void);
void iAP2TimespecSet64(struct timespec *ts, uint64_t ms);
uint64_t iAP2TimespecMS64(struct timespec *ts);
*/
#ifdef __cplusplus
}
#endif

#endif  // IAP2_TIME_IMP_H


