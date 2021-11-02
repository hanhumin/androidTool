/****************************************************************************
 *   FileName    : iPod_sample.c
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

#include <cutils/properties.h>
#include <utils/Log.h>

#include "iPod_error.h"
#include "iPod_lib.h"
#include "iPod_sample.h"
#include "iPod_interface.h"

#include "porting_layer/iPod_OS.h"
#include "porting_layer/iPod_PAL.h"

//#include "../ipodconfig.h"

#ifndef NULL
#define NULL 0
#endif

/*************************************************************************/
/*Global Variable                                                                                                      */
/*************************************************************************/

#define LOG_TAG "IapService"

int IAPSPL_DEBUG = -1;
#define IAPSPL_DBUG(msg...)	if (IAPSPL_DEBUG == 1) { ALOGD( " " msg); }

#define TRUE 1
#define FALSE 0

int gRepeatState = 0;
int gShuffleMode = 0;

void setRepeatState(int flag) {
    gRepeatState = flag;
}

void setShuffleMode(int flag) {
    gShuffleMode = flag;
}

unsigned char IPOD_Bitmap[24*50] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x03, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0xff, 0xff, 0xff, 0xf4, 0x03, 0xff, 0xff, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x15, 0x55, 0xf5, 0x55, 0x54, 0x0f, 0xc0, 0x0f, 0xc0, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x00, 0x3f, 0x00, 0x03, 0xf0, 0x00, 0xfc, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x00, 0xfc, 0x00, 0x00, 0xfc, 0x03, 0xf0, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x3c, 0x03, 0xc0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x3c, 0x0f, 0xc0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x3c, 0x0f, 0xc0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x3c, 0x03, 0xc0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x00, 0xfc, 0x00, 0x00, 0xfc, 0x03, 0xf0, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x00, 0x3f, 0x00, 0x03, 0xf0, 0x00, 0xfc, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x00, 0x0f, 0xc0, 0x0f, 0xc0, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xf4, 0x00, 0x00, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x03, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x0f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0xff, 0xfc, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x03, 0xff, 0xff, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x0f, 0x00, 0x00, 0xfc, 0x00, 0x0f, 0xc0, 0x0f, 0xc0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x0f, 0x00, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x03, 0xf0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xfc, 0x00, 0x00, 0xfc,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0x3c,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x0f, 0x00, 0x00, 0x0f, 0x03, 0xf0, 0x00, 0x00, 0x3c,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x0f, 0x00, 0x00, 0x3f, 0x03, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x0f, 0x00, 0x00, 0xfc, 0x03, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x0f, 0xff, 0xff, 0xf0, 0x03, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x0f, 0xff, 0xff, 0xc0, 0x03, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x0f, 0x00, 0x03, 0xf0, 0x03, 0xc0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x3c, 0x0f, 0x00, 0x00, 0xfc, 0x03, 0xf0, 0x00, 0x00, 0x3c,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0xfc, 0x0f, 0x00, 0x00, 0x3f, 0x00, 0xf0, 0x00, 0x00, 0x3c,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0xfc, 0x00, 0x00, 0xfc,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x03, 0xf0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0x3f, 0x00, 0x03, 0xf0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x0f, 0xc0, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0xc0, 0x0f, 0xc0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0x03, 0xff, 0xff, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0x00, 0xff, 0xfc, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


void IPOD_Sample_InitDebug(void)
{
    //if(IAPSPL_DEBUG == -1)
    {
        char iapDebug[PROPERTY_VALUE_MAX];

        property_get("persist.iap.debug.sample", iapDebug, "f");
        switch(iapDebug[0]){
            case 't':
            case 'T':
                IAPSPL_DEBUG = 1;
                break;
            case 'f':
            case 'F':
            default:
                IAPSPL_DEBUG = 0;
                break;
        }
    }
}


/*************************************************************************/
/* sample function                                                                                                         */
/*************************************************************************/

/**************************************************************************
*	Function : IPOD_Sample_Get_AudioPlayInfo
*
*	Description : This function returns play information of iPod
*
*	Parameter : iPod_Handle
*
*  	Return : pIPOD_PLAY_INFO pointer
*
**************************************************************************/
pIPOD_PLAY_INFO IPOD_Sample_Get_AudioPlayInfo(void *iPod_Handle)
{
	pIPOD_PLAY_INFO pInfo;
	pInfo = (pIPOD_PLAY_INFO)IPOD_GetExtInfo(iPod_Handle);
	return pInfo;
}


/**************************************************************************
*	Function : IPOD_Sample_ResetVideoSupportDB
*
*	Description : It reset the DB for video
*				This function is example fucntion.
*				It just select the first item of media kind lists.
*
*	Parameter : none
*
*  	Return : none
*
**************************************************************************/
unsigned char media_kind[10][40];

long IPOD_Sample_ResetVideoSupportDB(void *iPod_Handle, int *totalNumber)
{
	long TotalVideo_MediaCnt, i, ret;
	unsigned long index;
	unsigned long ActualLen;
	ret = IPOD_ResetDBSelectionHierarchy(iPod_Handle, DBSEL_VIDEO);

	if (ret == IPOD_NO_ERROR)
	{
		ret = IPOD_GetVideoMediaKindCnt(iPod_Handle, &TotalVideo_MediaCnt);

		if (IPOD_NO_ERROR == ret)
		{
			/*
			 This is a sample code to select the Movies category
			 In case of iPod touch
			 0 : Movies
			 1 : Music Video
			 2 : TV shows
			 3 : Video poadcasts
			 Select the Media kind from media_kind name data
			*/
#if 1
			IPOD_RECORD_TYPE *pMediaKindName;
			pMediaKindName = (IPOD_RECORD_TYPE *)malloc(sizeof(IPOD_RECORD_TYPE)*TotalVideo_MediaCnt);
			ret = IPOD_RetrieveCategorizedDatabaseRecords(iPod_Handle, MEDIA_KIND, 0, TotalVideo_MediaCnt, pMediaKindName);

			if (IPOD_NO_ERROR == ret)
			{
				ret = IPOD_SelectVedeoDBRecord(iPod_Handle, TotalVideo_MediaCnt, 0);
				*totalNumber = TotalVideo_MediaCnt;
			}

			free(pMediaKindName);
#else
			ret = IPOD_RetrieveCategorizedDatabaseRecords(iPod_Handle, MEDIA_KIND, 0, TotalVideo_MediaCnt);

			if (IPOD_NO_ERROR == ret)
			{
				for (i=0; i < TotalVideo_MediaCnt; i++)
				{
					(void)IPOD_GetFileName(iPod_Handle, &index, 40, media_kind[i], &ActualLen);
					//			IPOD_GetVideoMediaKindname(iPod_Handle, i, media_kind[i], 40);
				}

				ret = IPOD_SelectVedeoDBRecord(iPod_Handle, TotalVideo_MediaCnt, 0);
			}

#endif
		}
	}
	else
	{
		ret = IPOD_ResetDBSelection(iPod_Handle);
	}

	return ret;
}





/**************************************************************************
*	Function : IPOD_Sample_CheckPlayStatus
*
*	Description : This function initialize the Play status of iPod
*
*	Parameter : iPod_Handle
*
*  	Return : none
*
**************************************************************************/
void IPOD_Sample_CheckPlayStatus(void *iPod_Handle)
{
    IPOD_ERROR Err;
    long TotalTrackLength, lTime, lStatus;
    pIPOD_PLAY_INFO pPlayInfo = IPOD_Sample_Get_AudioPlayInfo(iPod_Handle);

    if (pPlayInfo != 0)
    {
        Err = IPOD_GetPlayStatus(iPod_Handle, &TotalTrackLength, &lTime, &lStatus);

        if (IPOD_NO_ERROR == Err)
        {
            IPOD_Sample_SetDuration(iPod_Handle,TotalTrackLength);
            IPOD_Sample_SetPostion(iPod_Handle,lTime);
            pPlayInfo->DisplayRemoteInfo.DR_TrackPosition = lTime;

            if (IPOD_STOPPING == lStatus)
            {
                IPOD_Sample_SetDRPlayStatus(iPod_Handle,IPOD_DR_PLAYBACK_STOPPED);
            }
            else if (IPOD_PAUSED == lStatus)
            {
                IPOD_Sample_SetDRPlayStatus(iPod_Handle,IPOD_DR_PLAYBACK_PAUSED);
            }
            else if (IPOD_PLAYING == lStatus)
            {
                IPOD_Sample_SetDRPlayStatus(iPod_Handle,IPOD_DR_PLAYING);
            }
            else
            {
                /* No statement */
            }
        }
    }
}

/**************************************************************************
*	Function : IPOD_Sample_SetCustomLogo
*
*	Description : This function send the bitmap data to iPod to display it.
*
*	Parameter :
*			- iPod_Handle
*			- Block : Block number (It just used on TCC800X serises )
*
*  	Return : none
*
**************************************************************************/
long IPOD_Sample_SetCustomLogo(void *iPod_Handle, unsigned long Block)
{
	long ret = -1;
	IPOD_PIXEL_FORMAT format;
	long height, width;
	IPOD_ERROR Err;
	MLingoType supportMultiLingo;
	IPOD_GetSupportMultiLingo(iPod_Handle, &supportMultiLingo);
	/*IPOD_API_SetSkipProcessMode(1);*/

	if (supportMultiLingo & ML_REMOTE_UI_EXTENDED)
	{
		Err = IPOD_GetColorDisplayLimits(iPod_Handle, &format, &width, &height);

		if (IPOD_NO_ERROR == Err)
		{
			/*Todo: check format, width, height.*/
			Err = IPOD_SetDisplayImage(iPod_Handle, IPOD_PIXEL_MONOCHROME, 96, 50, IPOD_Bitmap);

			if (IPOD_NO_ERROR == Err)
			{
				ret = 1;
			}
		}
	}

	/*IPOD_API_SetSkipProcessMode(0);*/
	return ret;
}

/**************************************************************************
*	Function : IPOD_sample_DB_Init
*
*	Description : This functin set the iPod DB state and get a Total file / Current file / current Time
*
*	Parameter :
*		- pPlaybackInfo
*
*  	Return : none
*
**************************************************************************/
long IPOD_Sample_DB_Init(void *iPod_Handle)
{
    long ret = -1;
    long i;
    unsigned long lTotalTrackLength, lTime;
    IPOD_DR_PLAY_STATUS lStatus;
    long TotalFileNum = 0;
    IPOD_ERROR Err = IPOD_INVALID_PARAM;
    unsigned long Ex_Evtoption = (IPOD_EVT_BASIC_PLAY_STATUS);
    //unsigned long DR_Evtoption = (0x0F);
    unsigned long DR_Evtoption = (   IPOD_DR_EVT_SET_TRACK_PLAY_INDEX
                                                    | IPOD_DR_EVT_SET_TRACK_TIME_POS_MS
                                                    | IPOD_DR_EVT_SET_TRACK_PLAY_INDEX
                                                    | IPOD_DR_EVT_SET_CHAPTER_INDEX
                                                    | IPOD_DR_EVT_SET_PLAY_STATUS
                                                    | IPOD_DR_EVT_SET_SHUFFLE_SETTING
                                                    | IPOD_DR_EVT_SET_REPEAT_SETTING
                                                    | IPOD_DR_EVT_SET_TRACK_POS_S
                                                    //| IPOD_DR_EVT_SET_TRACK_CAPABILITIES
                                                    );
    unsigned long long eventNotiMask;
    unsigned long maxPayload;
    long CurrentIndx;
    pIPOD_PLAY_INFO pPlayInfo;
    MLingoType supportMultiLingo;
    /*IPOD_API_SetSkipProcessMode(1);*/

    IPOD_Sample_InitDebug();
    IAPSPL_DBUG("%d %s ",__LINE__,__FUNCTION__);
    
    (void)IPOD_GetSupportMultiLingo(iPod_Handle, &supportMultiLingo);
    IAPSPL_DBUG("%d %s supportMultiLingo(0x%x)",__LINE__,__FUNCTION__,supportMultiLingo );
    pPlayInfo = IPOD_Sample_Get_AudioPlayInfo(iPod_Handle);

    if (pPlayInfo != 0)
    {

        #if 0
        IAPSPL_DBUG("  ===== Play Information ======================== ");
        IAPSPL_DBUG("   m_Status (0x%x)",pPlayInfo->m_Status);
        IAPSPL_DBUG("    DisconnecEvt(0x%x)",pPlayInfo->DisconnecEvt);
        IAPSPL_DBUG("    m_SupportMode(0x%x)",pPlayInfo->m_SupportMode);
        IAPSPL_DBUG("    TotalPlayNum(0x%x)",pPlayInfo->TotalPlayNum);
        IAPSPL_DBUG("    CurrentIdx(0x%x)",pPlayInfo->CurrentIdx);
        IAPSPL_DBUG("    TotalTime(0x%x)",pPlayInfo->TotalTime);
        IAPSPL_DBUG("    ShuffleStatus(0x%x)",pPlayInfo->ShuffleStatus);
        IAPSPL_DBUG("    RepeatStatus(0x%x)",pPlayInfo->RepeatStatus);
        IAPSPL_DBUG("    m_iPod_OSApp_DataSesiongID(0x%lx)",pPlayInfo->m_iPod_OSApp_DataSesiongID);
        IAPSPL_DBUG("    m_iPod_OSApp_ProtocolIndex(0x%lx)",pPlayInfo->m_iPod_OSApp_ProtocolIndex);
        IAPSPL_DBUG("    m_Noti_WaitTime(0x%x)",pPlayInfo->m_Noti_WaitTime);
        IAPSPL_DBUG("    m_Noti_OverflowTransID(0x%x)",pPlayInfo->m_Noti_OverflowTransID);
        IAPSPL_DBUG("  ");
        IAPSPL_DBUG("  ===== IPOD_DISPLAY_REMOTE_INFO Information ======================== ");
        IAPSPL_DBUG("   DR_TrackPosition(0x%lx)",pPlayInfo->DisplayRemoteInfo.DR_TrackPosition);
        IAPSPL_DBUG("   DR_TrackIndex(0x%lx)",pPlayInfo->DisplayRemoteInfo.DR_TrackIndex);
        IAPSPL_DBUG("   DR_PlayStatus(0x%x)",pPlayInfo->DisplayRemoteInfo.DR_PlayStatus);
        IAPSPL_DBUG("   DR_Shuffle(0x%x)",pPlayInfo->DisplayRemoteInfo.DR_Shuffle);
        IAPSPL_DBUG("   DR_Repeat(0x%x)",pPlayInfo->DisplayRemoteInfo.DR_Repeat);
        IAPSPL_DBUG("   DR_HoldSwitch(0x%x)",pPlayInfo->DisplayRemoteInfo.DR_HoldSwitch);
        IAPSPL_DBUG("   DR_SoundCheck(0x%x)",pPlayInfo->DisplayRemoteInfo.DR_SoundCheck);
        IAPSPL_DBUG("   DR_AudioBookSpeed(0x%x)",pPlayInfo->DisplayRemoteInfo.DR_AudioBookSpeed);
        IAPSPL_DBUG("   DR_TrackPositionInSecond(0x%x)",pPlayInfo->DisplayRemoteInfo.DR_TrackPositionInSecond);
        IAPSPL_DBUG("   DR_TrackCapabilities(0x%lx)",pPlayInfo->DisplayRemoteInfo.DR_TrackCapabilities);
        IAPSPL_DBUG("  ");
        IAPSPL_DBUG("  ===== IPOD_GENERAL_INFO Information ======================== ");
        IAPSPL_DBUG("   GEN_NowPlayingAppBundleName(%s)",pPlayInfo->GeneralInfo.GEN_NowPlayingAppBundleName.GEN_NowPlayingApplicationBundleName);
        IAPSPL_DBUG("   GEN_NowPlayingAppDisplayName(%s)",pPlayInfo->GeneralInfo.GEN_NowPlayingAppDisplayName.GEN_NowPlayingApplicationDisplayName);
        IAPSPL_DBUG("  ");
        IAPSPL_DBUG("  ===== IPOD_BASIC_INFO Information ======================== ");        
        IAPSPL_DBUG("   ipod_state(0x%x)",pPlayInfo->Basic_Info.ipod_state);
        IAPSPL_DBUG("   DevNo(0x%lx)",pPlayInfo->Basic_Info.DevNo);
        IAPSPL_DBUG("   SamplingFreq(0x%lx)",pPlayInfo->Basic_Info.SamplingFreq);
        #endif
        
        if (supportMultiLingo & ML_REMOTE_UI_EXTENDED)
        {
            ShuffleMode shuffle;
            CurrentRepeatState repeat;
            IPOD_Sample_SetCurrentIndex(iPod_Handle,-1);

            IPOD_Sample_SetDuration(iPod_Handle,lTotalTrackLength);
            if(ipod_stack_cbacks->iPodSetTimeForRepeat_cb){
                if(ipod_stack_cbacks->iPodCheckTimeForRepeat_cb() < 0)
                    return ret;
            }
            Err = IPOD_GetRepeat(iPod_Handle, &repeat);
            if(ipod_stack_cbacks->iPodSetTimeForRepeat_cb){
                ipod_stack_cbacks->iPodSetTimeForRepeat_cb();
            }
            
            if (IPOD_NO_ERROR == Err)
            {
                IAPSPL_DBUG("%d %s repeat(0x%x) repeatStatie(%d)",__LINE__,__FUNCTION__,repeat, gRepeatState );
                if (gRepeatState == FALSE) {
                    if (REPEATALLTRACKS != repeat) {
                        Err = IPOD_SetRepeat(iPod_Handle, REPEATALLTRACKS, RESTORE_ORG_SETTING);
                        repeat = REPEATALLTRACKS;
                    }
                }

                IPOD_Sample_SetRepeat(iPod_Handle, repeat);
            }

            //set Shuffle mode
            if(ipod_stack_cbacks->iPodSetTimeForShuffle_cb){
                if(ipod_stack_cbacks->iPodCheckTimeForShuffle_cb() < 0)
                    return ret;
            }
            Err = IPOD_GetShuffle(iPod_Handle, &shuffle);
            if(ipod_stack_cbacks->iPodSetTimeForShuffle_cb){
                ipod_stack_cbacks->iPodSetTimeForShuffle_cb();
            }

            if (IPOD_NO_ERROR == Err)
            {
                IAPSPL_DBUG("%d %s shuffle(0x%x) shuffleMode",__LINE__,__FUNCTION__,shuffle, gShuffleMode );
                if (gShuffleMode == FALSE) {
                    if (SHUFFLEOFF != shuffle) {
                        Err = IPOD_SetShuffle(iPod_Handle, SHUFFLEOFF, RESTORE_ORG_SETTING);
                        shuffle = SHUFFLEOFF;
                    }
                }

                IPOD_Sample_SetShuffle(iPod_Handle, shuffle);
            }


            /*ATS2.2 Accessory sent GetPlayStatus without first attempting SetPlayStatusChangenotification with bit 0 set*/
            IAPSPL_DBUG("%d %s  Ex_Evtoption(0x%lx) DR_Evtoption(0x%lx)",__LINE__,__FUNCTION__,Ex_Evtoption,DR_Evtoption );
            Err = IPOD_SetPlayStatusChangeNotification(iPod_Handle, Ex_Evtoption, 1);
            if( Err != IPOD_NO_ERROR)
                IPOD_SetPlayStatusChangeNotification(iPod_Handle, ENABLENOTIFICATION, 0);
            Err = IPOD_DR_SetRemoteEventNotification(iPod_Handle, DR_Evtoption);
            IAPSPL_DBUG("%d %s  Err(0x%x)",__LINE__,__FUNCTION__,Err );
            (void)IPOD_Sample_SetDefaultArtworkFormatID(iPod_Handle, ML_REMOTE_UI_EXTENDED);
          
            IPOD_Sample_GetDRPlayStatus(iPod_Handle,&lStatus);
            IPOD_Sample_GetDuration(iPod_Handle,&lTotalTrackLength);
            lTime = pPlayInfo->DisplayRemoteInfo.DR_TrackPosition;

            IAPSPL_DBUG("%d %s lStatus(0x%x) lTotalTrackLength(%ld) lTime(%ld)",__LINE__,__FUNCTION__,lStatus,lTotalTrackLength,lTime );

            if (lStatus == IPOD_DR_PLAYBACK_STOPPED)
            {
                if (IPOD_NO_ERROR == IPOD_ResetDBSelection(iPod_Handle))
                {
                    (void) IPOD_GetNumberCategorizedDBRecords(iPod_Handle, IPOD_TRACK, &TotalFileNum);
                }

                if (TotalFileNum == 0)
                {
                    /*IPOD_Display_No_SongMsg(iPod_Handle);*/
                }
                else
                {
                    (void)IPOD_SelectDBRecord(iPod_Handle, IPOD_TRACK, 0);
                    (void) IPOD_PlayControl(iPod_Handle, PLAYPAUSE);
                }
            }
            else
            {
                IPOD_Sample_SetDRPlayStatus(iPod_Handle,IPOD_DR_PLAYBACK_PAUSED);
                if (lTime != 0)
                {
                    lTime = (long)((unsigned long)lTime/1000);
                }

                /*IPOD_SetPlayTime((unsigned long)lTime, pPlayInfo);*/
                (void) IPOD_GetNumPlayingTracks(iPod_Handle, &TotalFileNum);
            }
            if (TotalFileNum > 0)
            {
                if (IPOD_NO_ERROR == IPOD_GetCurrentPlayingTrackIndex(iPod_Handle, &CurrentIndx))
                {
                    IAPSPL_DBUG("%d %s CurrentIndx(%ld) Err 0x%x",__LINE__,__FUNCTION__,CurrentIndx ,Err);
                    IPOD_Sample_SetCurrentIndex(iPod_Handle,CurrentIndx);
                }
            }
            IPOD_Sample_SetDuration(iPod_Handle,lTotalTrackLength);
        }
        else if (supportMultiLingo & ML_DISPLAY_REMOTE)
        {
            IAPSPL_DBUG("%d %s DR_Evtoption(0x%lx)",__LINE__,__FUNCTION__,DR_Evtoption );
            Err = IPOD_DR_SetRemoteEventNotification(iPod_Handle, DR_Evtoption);
            (void)IPOD_Sample_SetDefaultArtworkFormatID(iPod_Handle, ML_DISPLAY_REMOTE);
        }
    }

    if (IPOD_NO_ERROR == Err)
    {
        ret = 1; /* DB Init Success */
    }

    /*IPOD_API_SetSkipProcessMode(0);*/
    return ret;
}

/**************************************************************************
*	Function : IPOD_SAMPLE_DevDataTransfer
*
*	Description : iPhone OS application communicatin sample program
*
*	Parameter : Mode
*
*  	Return : none
*
**************************************************************************/
IPOD_ERROR IPOD_Sample_Transfer_Accessory_to_iOSApp(    void *iPod_Handle,
        unsigned char *SendBuff,
        unsigned long size,
        unsigned long sessionID
        )
{
    long ret = 0;
    void *ExtInfo = IPOD_GetExtInfo(iPod_Handle);
    pIPOD_PLAY_INFO pInfo = (pIPOD_PLAY_INFO)ExtInfo;
    unsigned long DevDataTransferRetryCnt = 0;
    unsigned long RetryTransID;
    unsigned long RetryDataCount;
    unsigned long compareTransID;

    if ( NULL == iPod_Handle )
    {
        ret = -1;
    }
    //else if ( IPOD_NOT_ESTABLISED_SESSIONGID == pInfo->m_iPod_OSApp_DataSesiongID )
    //{
    //    ret = -2;
    //}
    else
    {
        ret = IPOD_AccessoryDataTransfer( iPod_Handle,
                                sessionID,
                                SendBuff,
                                size,
                                IPOD_DATATRANSFER_NORMAL,
                                &RetryTransID,
                                &RetryDataCount);
    }

    return ret;
}

/**************************************************************************
*	Function : IPOD_Sample_GetDRPlayStatus
*
*	Description : It returns play status of iPod
*
*	Parameter :
*		void *iPod_Handle
*		IPOD_DR_PLAY_STATUS *playStatus
*
*  	Return : none
*
**************************************************************************/
IPOD_ERROR IPOD_Sample_GetDRPlayStatus(void *iPod_Handle, IPOD_DR_PLAY_STATUS *playStatus)
{
    IPOD_ERROR ret= IPOD_NO_ERROR;
    pIPOD_PLAY_INFO pInfo;
    void *ExtInfo = IPOD_GetExtInfo(iPod_Handle);

    if (NULL != ExtInfo)
    {
        pInfo = (pIPOD_PLAY_INFO)ExtInfo;
        *playStatus = pInfo->DisplayRemoteInfo.DR_PlayStatus;
    }
    else
    {
        ret = IPOD_INVALID_HANDLE;
    }

    return ret;
}

/**************************************************************************
*	Function : IPOD_Sample_SetDRPlayStatus
*
*	Description : It set the play status of iPod
*
*	Parameter :
*		void *iPod_Handle
*		IPOD_DR_PLAY_STATUS playStatus
*
*  	Return : none
*
**************************************************************************/
void IPOD_Sample_SetDRPlayStatus(void *iPod_Handle, IPOD_DR_PLAY_STATUS playStatus)
{
    pIPOD_PLAY_INFO pInfo;
    void *ExtInfo = IPOD_GetExtInfo(iPod_Handle);

    if (NULL != ExtInfo)
    {
        pInfo = (pIPOD_PLAY_INFO)ExtInfo;
        IAPSPL_DBUG(" playstatus old %d -> new %d", pInfo->DisplayRemoteInfo.DR_PlayStatus, playStatus);
        pInfo->DisplayRemoteInfo.DR_PlayStatus = playStatus;
    }
}

/**************************************************************************
*	Function : IPOD_Sample_GetCurrentIndex
*
*	Description : It returns currentindex 
*
*	Parameter :
*		void *iPod_Handle
*		long *pIndex
*
*  	Return : none
*
**************************************************************************/
IPOD_ERROR IPOD_Sample_GetCurrentIndex(void *iPod_Handle, long *pIndex)
{
    IPOD_ERROR ret= IPOD_NO_ERROR;
    pIPOD_PLAY_INFO pInfo = IPOD_Sample_Get_AudioPlayInfo(iPod_Handle);

    if (NULL != pInfo)
        *pIndex = pInfo->CurrentIdx;
    else
        ret = IPOD_INVALID_HANDLE;

    return ret;
}
  
void IPOD_Sample_SetCurrentIndex(void *iPod_Handle, long Index)
{
    pIPOD_PLAY_INFO pInfo = IPOD_Sample_Get_AudioPlayInfo(iPod_Handle);

    if (NULL != pInfo)
        pInfo->CurrentIdx = Index;
}


/**************************************************************************
*	Function : IPOD_Sample_GetDRRepeat
*
*	Description : It returns pepeat status of iPod
*
*	Parameter :
*		void *iPod_Handle
*		unsigned char *pRepeat
*
*  	Return : none
*
**************************************************************************/
IPOD_ERROR IPOD_Sample_GetRepeat(void *iPod_Handle, unsigned char *pRepeat)
{
    IPOD_ERROR ret= IPOD_NO_ERROR;
    pIPOD_PLAY_INFO pInfo = IPOD_Sample_Get_AudioPlayInfo(iPod_Handle);

    if (NULL != pInfo)
        *pRepeat = pInfo->RepeatStatus;
    else
        ret = IPOD_INVALID_HANDLE;

    return ret;
}

void IPOD_Sample_SetRepeat(void *iPod_Handle, int repeat)
{
    pIPOD_PLAY_INFO pInfo = IPOD_Sample_Get_AudioPlayInfo(iPod_Handle);

    if (NULL != pInfo)
        pInfo->RepeatStatus = repeat;
}


/**************************************************************************
*	Function : IPOD_Sample_GetDRShuffle
*
*	Description : It returns shuffle status of iPod
*
*	Parameter :
*		void *iPod_Handle
*		unsigned char *pShuffle
*
*  	Return : none
*
**************************************************************************/
IPOD_ERROR IPOD_Sample_GetShuffle(void *iPod_Handle, unsigned char *pShuffle)
{
    IPOD_ERROR ret= IPOD_NO_ERROR;
    pIPOD_PLAY_INFO pInfo = IPOD_Sample_Get_AudioPlayInfo(iPod_Handle);

    if (NULL != pInfo)
        *pShuffle = pInfo->ShuffleStatus;
    else
        ret = IPOD_INVALID_HANDLE;

    return ret;
}

void IPOD_Sample_SetShuffle(void *iPod_Handle, int shuffle)
{
    pIPOD_PLAY_INFO pInfo = IPOD_Sample_Get_AudioPlayInfo(iPod_Handle);

    if (NULL != pInfo)
        pInfo->ShuffleStatus = shuffle;
}

IPOD_ERROR IPOD_Sample_GetPosition(void *iPod_Handle, unsigned long *position)
{
    IPOD_ERROR ret= IPOD_NO_ERROR;
    pIPOD_PLAY_INFO pInfo = IPOD_Sample_Get_AudioPlayInfo(iPod_Handle);

    if (NULL != pInfo)
        *position = pInfo->PostionMs;
    else
        ret = IPOD_INVALID_HANDLE;

    return ret;    
}

void IPOD_Sample_SetPostion(void *iPod_Handle, unsigned long position)
{
    pIPOD_PLAY_INFO pInfo = IPOD_Sample_Get_AudioPlayInfo(iPod_Handle);

    if (NULL != pInfo)
        pInfo->PostionMs= position;
}

IPOD_ERROR IPOD_Sample_GetDuration(void *iPod_Handle, unsigned long *duration)
{
    IPOD_ERROR ret= IPOD_NO_ERROR;
    pIPOD_PLAY_INFO pInfo = IPOD_Sample_Get_AudioPlayInfo(iPod_Handle);

    if (NULL != pInfo)
        *duration = pInfo->TotalTime;
    else
        ret = IPOD_INVALID_HANDLE;

    return ret;    
}

void IPOD_Sample_SetDuration(void *iPod_Handle, unsigned long duration)
{
    pIPOD_PLAY_INFO pInfo = IPOD_Sample_Get_AudioPlayInfo(iPod_Handle);

    if (NULL != pInfo)
        pInfo->TotalTime= duration;
}

/**************************************************************************
*	Function : IPOD_Sample_SetDefaultArtworkFormatID
*
*	Description : This functin set basic artwork format
*
*	Parameter : none.
*
*  	Return : none.
*
**************************************************************************/
IPOD_ERROR IPOD_Sample_SetDefaultArtworkFormatID(void *iPod_Handle, unsigned long Lingo)
{
	IPOD_ERROR Err;
	long ActualLen;
	long FrameID;
	ArtworkFormat pformat[IPOD_MAX_ARTWORK];
	unsigned long displaysize, MaxDisplaysize;
	unsigned long currentDisplaysize = 0;
	pIPOD_PLAY_INFO pPlayInfo = IPOD_Sample_Get_AudioPlayInfo(iPod_Handle);
	pIPOD_ARTWORK_INFO pArtworkInfo = &(pPlayInfo->ArtworkInfo);
	MaxDisplaysize = pArtworkInfo->MaxDisplaySize;
	pArtworkInfo->BasicFormatID = 0;

	if (ML_REMOTE_UI_EXTENDED == Lingo)
	{
		/* In case of Extended Lingo - Default */
		Err = IPOD_GetArtworkFormats(iPod_Handle, &pformat[0], IPOD_MAX_ARTWORK, &ActualLen);
	}
	else if (ML_DISPLAY_REMOTE == Lingo)
	{
		/* In case of Display Remote Lingo */
		Err = IPOD_DR_GetArtworkFormats(iPod_Handle, &pformat[0], IPOD_MAX_ARTWORK, &ActualLen);
	}
	else
	{
		Err = IPOD_INVALID_PARAM;
	}

	if (IPOD_NO_ERROR == Err)
	{
		pArtworkInfo->MaxFormatCount = (unsigned long)ActualLen;

		for (FrameID=0; FrameID<ActualLen; FrameID++)
		{
			if ((RGB565_LITTLE_ENDIAN_16BPP == pformat[FrameID].PixelFormat))
				/*	||(RGB565_BIG_ENDIAN_16BPP == pformat[FrameID].PixelFormat)) */
			{
				displaysize = (unsigned long)((pformat[FrameID].ImageWidth *2)*(pformat[FrameID].ImageHeight));

				if (displaysize <= MaxDisplaysize)
				{
					pArtworkInfo->CapableFormatID[FrameID] = (unsigned long)pformat[FrameID].FormatID;
					pArtworkInfo->CapableSize[FrameID] = displaysize;

					if (currentDisplaysize < displaysize)
					{
						currentDisplaysize = displaysize;
						pArtworkInfo->BasicFormatID = (unsigned long)pformat[FrameID].FormatID;
					}
				}
				else
				{
					pArtworkInfo->CapableFormatID[FrameID] = -1;
					pArtworkInfo->CapableSize[FrameID] = -1;
				}
			}
		}
	}

	return Err;
}



/**************************************************************************
*	Function : iPod_ExampleCode_getLingoProtocolVer
*
*	Description : iPod user command example - to get a lingo protocol version.
*
*	Parameter :
*		void *iPod_Handle
*		unsigned long Lingo
*		unsigned char *inform
*		unsigned char *major
*		unsigned char *minor
*
*  	Return : none
*
**************************************************************************/
#define TEST_REQUEST_LINGOPROTOCOLVERSION						0x0F
#define TEST_RETURN_LINGOPROTOCOLVERSION						0x10
IPOD_ERROR iPod_ExampleCode_getLingoProtocolVer(void *iPod_Handle, unsigned long Lingo, unsigned char *inform, unsigned char *major, unsigned char *minor)
{
	IPOD_ERROR ret;
	IPOD_CMD_PARAM CmdParam;
	unsigned char *buff = IPOD_GetTxBuffer(iPod_Handle); /*Please use this function to get a Txbuffer */
	unsigned char *pData;
	unsigned long DataSize;
	ret = IPOD_USER_EnterCriticalSection(iPod_Handle);
       IPOD_MemSet((void *)&CmdParam, 0x00, sizeof(IPOD_CMD_PARAM));

	if (IPOD_NO_ERROR == ret)
	{
		if (NULL != buff)
		{
			buff[0] = LINGO_GENERAL;
			buff[1] = TEST_REQUEST_LINGOPROTOCOLVERSION;
			buff[2] = Lingo;
			CmdParam.m_iPod_Handle = iPod_Handle;
			CmdParam.m_CurCmd = TEST_REQUEST_LINGOPROTOCOLVERSION;
			CmdParam.m_SendCmdBuf = buff;
			CmdParam.m_SendCmdBufSize = 3; /*buffer size of User added*/
			CmdParam.m_TimeOut = IPOD_Callback_GetWaitTime();
			CmdParam.m_SendFunc = IPOD_SEND_ADD_HEADER;
			CmdParam.m_Flag = IPOD_DATA_SINGLE_SESSION;
			ret= IPOD_USER_FUNC_SendCmd_RcvResponse(&CmdParam, LINGO_GENERAL, TEST_RETURN_LINGOPROTOCOLVERSION);

			if (ret == IPOD_COMMAND_ACCEPTED)
			{
				ret = IPOD_NO_ERROR;
				pData = CmdParam.m_ResponseBuf;
				DataSize = CmdParam.m_ResponseBufSize;
				*inform =pData[0];
				*major = pData[1];
				*minor = pData[2];
			}
		}

		IPOD_USER_ExitCriticalSection(iPod_Handle);
	}

	return ret;
}

