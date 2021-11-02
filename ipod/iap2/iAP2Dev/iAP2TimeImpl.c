/****************************************************************************
 *   FileName    : iAP2TimeImpl.c
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

#include <sys/time.h>

#include "iAP2TimeImpl.h"

uint32_t iAP2TimeTick(void)
{
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return (uint32_t)(tv.tv_sec*1000) + (uint32_t)(tv.tv_usec/1000);

}

void iAP2TimespecNow(struct timespec *ts)
{
    struct timeval  tv;
    gettimeofday(&tv, NULL);
    ts->tv_sec  = tv.tv_sec;
    ts->tv_nsec = tv.tv_usec*1000;
}

void iAP2TimespecAddMS(struct timespec *ts, uint32_t ms)
{
    uint32_t sec = ms/1000;
    ms = ms - sec*1000;

    ts->tv_nsec += ms*1000000;

    ts->tv_sec += ts->tv_nsec/1000000000 + sec;
    ts->tv_nsec = ts->tv_nsec%1000000000;
}
/*
uint32_t iAP2TimespecMS(struct timespec *ts) 
{
    return ts->tv_sec*1000 + ts->tv_nsec/1000000;
}
*/
/*
uint64_t iAP2TimeTick64(void)
{
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return ((uint64_t)tv.tv_sec*1000) + ((uint64_t)tv.tv_usec/1000);
}

void iAP2TimespecSet64(struct timespec *ts, uint64_t ms)
{
    uint64_t sec = ms/1000;
    ms = ms - sec*1000;

    ts->tv_nsec = ms*1000000;

    ts->tv_sec  = ts->tv_nsec/1000000000 + sec;
    ts->tv_nsec = ts->tv_nsec%1000000000;
}

uint64_t iAP2TimespecMS64(struct timespec *ts) 
{
    return ((uint64_t)ts->tv_sec*1000) + ((uint64_t)ts->tv_nsec/1000000);
}

*/

