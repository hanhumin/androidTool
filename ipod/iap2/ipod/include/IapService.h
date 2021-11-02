/****************************************************************************
 *   FileName    : IapService.h
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

#ifndef ANDROID_IAPSERVICE_H
#define ANDROID_IAPSERVICE_H

#include <binder/BinderService.h>
#include <media/AudioTrack.h>
#include <media/AudioRecord.h>
#include "IIapService.h"
#include "IIapServiceNotify.h"
#include "IIapServiceCallback.h"

#include <utils/Looper.h>
#include <utils/threads.h>
#include <gui/BitTube.h>
#include "utils/KeyedVector.h"

namespace android {

#define USE_DIRECT_AUDIO_HAL	// To use Audio HAL directly. by shmin
#define USE_TIMER

#define AUDIO_OUT_FRAME_MAX_SIZE 	(4*1024)
#define AUDIO_IN_FRAME_MAX_SIZE 	(8*1024)
#define AUDIO_CHUNK_BYTES 	  		(2*1024)
#define PLAY_REAL_PREVIOUS_LIMIT_MS   (3000)

#define  IAP_CMD_MASK (0x100)
#define  IAP_CMD_INIT (IAP_CMD_MASK + 1)
#define  IAP_CMD_DEINIT (IAP_CMD_MASK + 2)
#define  IAP_CMD_TERMINATE (IAP_CMD_MASK + 3)
#define  IAP_CMD_PLAY (IAP_CMD_MASK + 4)
#define  IAP_CMD_PAUSE (IAP_CMD_MASK + 5)
#define  IAP_CMD_NEXT_TRACK (IAP_CMD_MASK + 6)
#define  IAP_CMD_PREVIOUS_TRACK (IAP_CMD_MASK + 7)
#define  IAP_CMD_FF (IAP_CMD_MASK + 8)
#define  IAP_CMD_REW (IAP_CMD_MASK + 9)
#define  IAP_CMD_GET_CURRENT_TRACKINDEX (IAP_CMD_MASK + 10)
#define  IAP_CMD_GET_TOTAL_TRACK (IAP_CMD_MASK + 11)
#define  IAP_CMD_GET_TITLE (IAP_CMD_MASK + 12)
#define  IAP_CMD_GET_ARTIST  (IAP_CMD_MASK + 13)
#define  IAP_CMD_GET_ALBUM  (IAP_CMD_MASK + 14)
#define  IAP_CMD_GET_GENRE  (IAP_CMD_MASK + 15)
#define  IAP_CMD_GET_ALBUMART  (IAP_CMD_MASK + 16)
#define  IAP_CMD_GET_DEVDSP_INFO  (IAP_CMD_MASK + 17)
#define  IAP_CMD_SET_DEVDSP_IMAGE  (IAP_CMD_MASK + 18)
#define  IAP_CMD_DATABASE_INIT  (IAP_CMD_MASK + 19)
#define  IAP_CMD_DATABASE_SELECT_ALL  (IAP_CMD_MASK + 20)
#define  IAP_CMD_DATABASE_SELECT  (IAP_CMD_MASK + 21)
#define  IAP_CMD_DATABASE_MOVE_UPPER  (IAP_CMD_MASK + 22)
#define  IAP_CMD_DATABASE_GET_NAMES  (IAP_CMD_MASK + 23)
#define  IAP_CMD_EXT_FUNC1  (IAP_CMD_MASK + 24)
#define  IAP_CMD_EXT_FUNC2  (IAP_CMD_MASK + 25)
#define  IAP_CMD_EXT_FUNC3  (IAP_CMD_MASK + 26)
#define  IAP_CMD_EXT_FUNC4  (IAP_CMD_MASK + 27)
#define  IAP_CMD_EXT_FUNC5  (IAP_CMD_MASK + 28)
#define  IAP_CMD_GET_DR_PLAYSTATUS  (IAP_CMD_MASK + 29)
#define  IAP_CMD_GET_PLAYSTATUS_INFO  (IAP_CMD_MASK + 30)
#define  IAP_CMD_GET_TRACKINFO  (IAP_CMD_MASK + 31)
#define  IAP_CMD_TOGGLE_REPEAT  (IAP_CMD_MASK + 32)
#define  IAP_CMD_TOGGLE_SHUFFLE  (IAP_CMD_MASK + 33)
#define  IAP_CMD_SET_POSITION  (IAP_CMD_MASK + 34)
#define  IAP_CMD_DISCONNECTED  (IAP_CMD_MASK + 35)
#define  IAP_CMD_GET_TRACKMETA  (IAP_CMD_MASK + 36)
#define  IAP_CMD_SETUP  (IAP_CMD_MASK + 37)
#define  IAP_CMD_SET_CURRENT_PLAYINGTRACK  (IAP_CMD_MASK + 38)
#define  IAP_CMD_SET_REPEAT  (IAP_CMD_MASK + 39)
#define  IAP_CMD_GET_REPEAT  (IAP_CMD_MASK + 40)
#define  IAP_CMD_SET_SHUFFLE  (IAP_CMD_MASK + 41)
#define  IAP_CMD_GET_SHUFFLE  (IAP_CMD_MASK + 42)
#define  IAP_CMD_GET_INDEXEDPLAYINGINFO  (IAP_CMD_MASK + 43)
#define  IAP_CMD_GET_MODEL_NUM  (IAP_CMD_MASK + 44)
#define  IAP_CMD_GET_PROTOCOL_VESION  (IAP_CMD_MASK + 45)
#define  IAP_CMD_GET_SOFTWARE_VERSION  (IAP_CMD_MASK + 46)
#define  IAP_CMD_GET_SERIAL_NUM  (IAP_CMD_MASK + 47)
#define  IAP_CMD_GET_IPOD_NAME  (IAP_CMD_MASK + 48)
#define  IAP_CMD_ENDFFREW (IAP_CMD_MASK + 49)
#define  IAP_CMD_ACCESSORYDATATRANSFER (IAP_CMD_MASK + 50)
#define  IAP_CMD_APPLICATIONLAUNCH (IAP_CMD_MASK + 51)
#define  IAP_CMD_UNREGISTER (IAP_CMD_MASK + 52)
#define  IAP_CMD_MUTECONTROL (IAP_CMD_MASK + 53)
#define  IAP_CMD_SEND_PACKEDLINGOCMD (IAP_CMD_MASK + 54)
#define  IAP_CMD_EXTRA_COMMAND (IAP_CMD_MASK + 55)
#define  IAP_CMD_CHK_READYSESSION (IAP_CMD_MASK + 56)
// Extra command start 1
#define  IAP_CMD_GETNOWPLAYINGAPPBUNDLENAME (IAP_CMD_MASK + 57)
#define  IAP_CMD_PLAYCONTROL (IAP_CMD_MASK + 58)
#define  IAP_CMD_GETUIDTRACKINFO (IAP_CMD_MASK + 59)
#define  IAP_CMD_GETDBTRACKINFO (IAP_CMD_MASK + 60)
#define  IAP_CMD_GETPBTRACKINFO (IAP_CMD_MASK + 61)
#define  IAP_CMD_GETIPODCAPS (IAP_CMD_MASK + 62)
#define  IAP_CMD_GETIPODFREESPACE (IAP_CMD_MASK + 63)
#define  IAP_CMD_OPENIPODFEATUREFILE (IAP_CMD_MASK + 64)
#define  IAP_CMD_WRITEIPODFILEDATA (IAP_CMD_MASK + 65)
#define  IAP_CMD_CLOSEIPODFILE (IAP_CMD_MASK + 66)
#define  IAP_CMD_WRAP_COMMAND (IAP_CMD_MASK + 67)
// Extra command end 1
// start for other
#define  IAP_CMD_SYNC_COMMAND (IAP_CMD_MASK + 68)
#define  IAP_CMD_ASYNC_COMMAND (IAP_CMD_MASK + 69)
#define  IAP_CMD_GET_IAP_STATE (IAP_CMD_MASK + 70)
#define  IAP_CMD_MSG_FLUSH (IAP_CMD_MASK + 71)
#define  IAP_CMD_MUTECONTROL2 (IAP_CMD_MASK + 72)
// end for other
// Extra command start 2
#define  IAP_CMD_GET_ARTWORK_FORMATS (IAP_CMD_MASK + 73)
#define  IAP_CMD_GET_ARTWORK_COUNT (IAP_CMD_MASK + 74)
#define  IAP_CMD_GET_ARTWORK_TIMES (IAP_CMD_MASK + 75)
#define  IAP_CMD_GET_ARTWORK_DATA (IAP_CMD_MASK + 76)
// Extra command end 2
#define  IAP_CMD_EAP_PLAY (IAP_CMD_MASK + 77)
#define  IAP_CMD_EAP_PAUSE (IAP_CMD_MASK + 78)
#define  IAP_CMD_EAP_GET_DR_PLAYSTATUS (IAP_CMD_MASK + 79)
#define  IAP_CMD_GET_IAP_SVC_STATE (IAP_CMD_MASK + 80)
#define  IAP_CMD_NEWPLAY (IAP_CMD_MASK + 81)
#define  IAP_CMD_NEWPAUSE (IAP_CMD_MASK + 82)
#define  IAP_CMD_NEXT (IAP_CMD_MASK + 83)
#define  IAP_CMD_PREVIOUS (IAP_CMD_MASK + 84)

#define  IAP_RSP_MASK (0x200)
#define  IAP_RSP_INIT (IAP_RSP_MASK + 1)
#define  IAP_RSP_DEINIT (IAP_RSP_MASK + 2)
#define  IAP_RSP_TERMINATE (IAP_RSP_MASK + 3)
#define  IAP_RSP_PLAY (IAP_RSP_MASK + 4)
#define  IAP_RSP_PAUSE (IAP_RSP_MASK + 5)
#define  IAP_RSP_NEXT_TRACK (IAP_RSP_MASK + 6)
#define  IAP_RSP_PREVIOUS_TRACK (IAP_RSP_MASK + 7)
#define  IAP_RSP_FF (IAP_RSP_MASK + 8)
#define  IAP_RSP_REW (IAP_RSP_MASK + 9)
#define  IAP_RSP_GET_CURRENT_TRACKINDEX (IAP_RSP_MASK + 10)
#define  IAP_RSP_GET_TOTAL_TRACK (IAP_RSP_MASK + 11)
#define  IAP_RSP_GET_TITLE (IAP_RSP_MASK + 12)
#define  IAP_RSP_GET_ARTIST  (IAP_RSP_MASK + 13)
#define  IAP_RSP_GET_ALBUM  (IAP_RSP_MASK + 14)
#define  IAP_RSP_GET_GENRE  (IAP_CMD_MASK + 15)
#define  IAP_RSP_GET_ALBUMART  (IAP_RSP_MASK + 16)
#define  IAP_RSP_GET_DEVDSP_INFO  (IAP_RSP_MASK + 17)
#define  IAP_RSP_SET_DEVDSP_IMAGE  (IAP_RSP_MASK + 18)
#define  IAP_RSP_DATABASE_INIT  (IAP_RSP_MASK + 19)
#define  IAP_RSP_DATABASE_SELECT_ALL  (IAP_RSP_MASK + 20)
#define  IAP_RSP_DATABASE_SELECT  (IAP_RSP_MASK + 21)
#define  IAP_RSP_DATABASE_MOVE_UPPER  (IAP_RSP_MASK + 22)
#define  IAP_RSP_DATABASE_GET_NAMES  (IAP_RSP_MASK + 23)
#define  IAP_RSP_EXT_FUNC1  (IAP_RSP_MASK + 24)
#define  IAP_RSP_EXT_FUNC2  (IAP_RSP_MASK + 25)
#define  IAP_RSP_EXT_FUNC3  (IAP_RSP_MASK + 26)
#define  IAP_RSP_EXT_FUNC4  (IAP_RSP_MASK + 27)
#define  IAP_RSP_EXT_FUNC5  (IAP_RSP_MASK + 28)
#define  IAP_RSP_GET_DR_PLAYSTATUS  (IAP_RSP_MASK + 29)
#define  IAP_RSP_GET_PLAYSTATUS_INFO  (IAP_RSP_MASK + 30)
#define  IAP_RSP_GET_TRACKINFO  (IAP_RSP_MASK + 31)
#define  IAP_RSP_TOGGLE_REPEAT  (IAP_RSP_MASK + 32)
#define  IAP_RSP_TOGGLE_SHUFFLE  (IAP_RSP_MASK + 33)
#define  IAP_RSP_SET_POSITION  (IAP_RSP_MASK + 34)
#define  IAP_RSP_DISCONNECTED  (IAP_RSP_MASK + 35)
#define  IAP_RSP_GET_TRACKMETA  (IAP_RSP_MASK + 36)
#define  IAP_RSP_SETUP  (IAP_RSP_MASK + 37)
#define  IAP_RSP_SET_CURRENT_PLAYINGTRACK  (IAP_RSP_MASK + 38)
#define  IAP_RSP_SET_REPEAT  (IAP_RSP_MASK + 39)
#define  IAP_RSP_GET_REPEAT  (IAP_RSP_MASK + 40)
#define  IAP_RSP_SET_SHUFFLE  (IAP_RSP_MASK + 41)
#define  IAP_RSP_GET_SHUFFLE  (IAP_RSP_MASK + 42)
#define  IAP_RSP_GET_INDEXEDPLAYINGINFO  (IAP_RSP_MASK + 43)
#define  IAP_RSP_GET_MODEL_NUM  (IAP_RSP_MASK + 44)
#define  IAP_RSP_GET_PROTOCOL_VESION  (IAP_RSP_MASK + 45)
#define  IAP_RSP_GET_SOFTWARE_VERSION  (IAP_RSP_MASK + 46)
#define  IAP_RSP_GET_SERIAL_NUM  (IAP_RSP_MASK + 47)
#define  IAP_RSP_GET_IPOD_NAME  (IAP_RSP_MASK + 48)
#define  IAP_RSP_ENDFFREW (IAP_RSP_MASK + 49)
#define  IAP_RSP_ACCESSORYDATATRANSFER (IAP_RSP_MASK + 50)
#define  IAP_RSP_APPLICATIONLAUNCH (IAP_RSP_MASK + 51)
#define  IAP_RSP_UNREGISTER (IAP_RSP_MASK + 52)
#define  IAP_RSP_MUTECONTROL (IAP_RSP_MASK + 53)
#define  IAP_RSP_SEND_PACKEDLINGOCMD (IAP_RSP_MASK + 54)
#define  IAP_RSP_EXTRA_COMMAND (IAP_RSP_MASK + 55)
#define  IAP_RSP_CHK_READYSESSION (IAP_RSP_MASK + 56)
// Extra command start 1
#define  IAP_RSP_GETNOWPLAYINGAPPBUNDLENAME (IAP_RSP_MASK + 57)
#define  IAP_RSP_PLAYCONTROL (IAP_RSP_MASK + 58)
#define  IAP_RSP_GETUIDTRACKINFO (IAP_RSP_MASK + 59)
#define  IAP_RSP_GETDBTRACKINFO (IAP_RSP_MASK + 60)
#define  IAP_RSP_GETPBTRACKINFO (IAP_RSP_MASK + 61)
#define  IAP_RSP_GETIPODCAPS (IAP_RSP_MASK + 62)
#define  IAP_RSP_GETIPODFREESPACE (IAP_RSP_MASK + 63)
#define  IAP_RSP_OPENIPODFEATUREFILE (IAP_RSP_MASK + 64)
#define  IAP_RSP_WRITEIPODFILEDATA (IAP_RSP_MASK + 65)
#define  IAP_RSP_CLOSEIPODFILE (IAP_RSP_MASK + 66)
#define  IAP_RSP_WRAP_COMMAND (IAP_RSP_MASK + 67)
// Extra command end 1
// start for other
#define  IAP_RSP_SYNC_COMMAND (IAP_RSP_MASK + 68)
#define  IAP_RSP_ASYNC_COMMAND (IAP_RSP_MASK + 69)
#define  IAP_RSP_GET_IAP_STATE (IAP_RSP_MASK + 70)
#define  IAP_RSP_MSG_FLUSH (IAP_RSP_MASK + 71)
#define  IAP_RSP_MUTECONTROL2 (IAP_RSP_MASK + 72)
// end for other
// Extra command start 2
#define  IAP_RSP_GET_ARTWORK_FORMATS (IAP_RSP_MASK + 73)
#define  IAP_RSP_GET_ARTWORK_COUNT (IAP_RSP_MASK + 74)
#define  IAP_RSP_GET_ARTWORK_TIMES (IAP_RSP_MASK + 75)
#define  IAP_RSP_GET_ARTWORK_DATA (IAP_RSP_MASK + 76)
// Extra command end 2
#define  IAP_RSP_EAP_PLAY (IAP_RSP_MASK + 77)
#define  IAP_RSP_EAP_PAUSE (IAP_RSP_MASK + 78)
#define  IAP_RSP_EAP_GET_DR_PLAYSTATUS (IAP_RSP_MASK + 79)
#define  IAP_RSP_GET_IAP_SVC_STATE (IAP_RSP_MASK + 80)
#define  IAP_RSP_NEWPLAY (IAP_RSP_MASK + 81)
#define  IAP_RSP_NEWPAUSE (IAP_RSP_MASK + 82)
#define  IAP_RSP_NEXT (IAP_RSP_MASK + 83)
#define  IAP_RSP_PREVIOUS (IAP_RSP_MASK + 84)
enum{
    IAP_SVC_STATE_DISCONNECT = 0,
    IAP_SVC_STATE_HW_CONNECT = 1,
    IAP_SVC_STATE_CONNECT_SUCCESS = 2,
    IAP_SVC_STATE_CONNECT_FAIL = 3
};

enum{
    IAP_EXT_CMD_GETNOWPLAYINGAPPBUNDLENAME = 1,
    IAP_EXT_CMD_PLAYCONTROL = 2,
    IAP_EXT_CMD_GETUIDTRACKINFO = 3,
    IAP_EXT_CMD_GETDBTRACKINFO = 4,
    IAP_EXT_CMD_GETPBTRACKINFO = 5,
    IAP_EXT_CMD_GETIPODCAPS = 6,
    IAP_EXT_CMD_GETIPODFREESPACE = 7,
    IAP_EXT_CMD_OPENIPODFEATUREFILE = 8,
    IAP_EXT_CMD_WRITEIPODFILEDATA = 9,
    IAP_EXT_CMD_CLOSEIPODFILE = 10
};

#define DUMMY_LEN (1)

typedef struct
{
    int what;
    int argInt;
    int argInt2;
    long argLong1;
    long argLong2;
    bool async;
    unsigned int msgFlushSig;
    const Parcel *data;
    Parcel *reply;
    sp<IIapServiceCallback> cbFunc;
} IAP_CMD_DATA_TYPE;

typedef struct
{
    int what;
    bool ret;
}IAP_RSP_DATA_TYPE;

//#define USED_ONLY_IAP1
#define PROP_IAP1_SUPPORT_ONLY "tcc.iap.surpport.only"
#define PROP_IAP2_SUPPORT "tcc.iap2.surpport"
    
class IapService: 
	public BinderService<IapService>, 
	public BnIapService,
	public IBinder::DeathRecipient
{
    enum {
        IPOD_CONTROL_STATE_NONE,
        IPOD_CONTROL_STATE_INITIALIZED,
        IPOD_CONTROL_STATE_TERMINATED,

        IPOD_CONTROL_STATE_MAX
    }; // iPodControlState
    enum
    {
        IPOD_UPDATE_PLAY = 0,
        IPOD_UPDATE_PAUSE
    };

    enum
    {
        IPOD_EVTTIME_DATABASE = 0,
        IPOD_EVTTIME_PLAYINFOR,
        IPOD_EVTTIME_REPEAT,
        IPOD_EVTTIME_SHUFFLE,
        IPOD_EVTTIME_MAX
    };

    typedef enum {
        IPOD_STATE_INITIALIZED,
        IPOD_STATE_TERMINATED,

        IPOD_STATE_MAX
    } EVENTPROCESSOR_STATE;

    enum{
        IPOD_TRACKINFO_CAPABILITIES = 0,
        IPOD_TRACKINFO_TRACKNAME = 1,
        IPOD_TRACKINFO_ARTISTNAME = 2,
        IPOD_TRACKINFO_ALBUMNAME = 3,
        IPOD_TRACKINFO_GENRENAME = 4,
        IPOD_TRACKINFO_COMPOSERNAME = 5,
        IPOD_TRACKINFO_TOTALTRACKDURATION = 6,
        IPOD_TRACKINFO_ITUNESUID = 7,
        IPOD_TRACKINFO_CHAPTERCOUNT = 8,
        IPOD_TRACKINFO_CHAPTERTIMES = 9,
        IPOD_TRACKINFO_CHAPTERNAME = 10,
        IPOD_TRACKINFO_LYRICS = 11,
        IPOD_TRACKINFO_DESCRIPTION = 12,
        IPOD_TRACKINFO_ALBUMTRACKINDEX = 13,
        IPOD_TRACKINFO_DISCSETALBUMINDEX = 14,
        IPOD_TRACKINFO_PLAYCOUNT = 15,
        IPOD_TRACKINFO_SKIPCOUNT = 16,
        IPOD_TRACKINFO_PODCASERELEASEDATE = 17,
        IPOD_TRACKINFO_LASTPLAYED = 18,
        IPOD_TRACKINFO_YEAR = 19,
        IPOD_TRACKINFO_STARRATING = 20,
        IPOD_TRACKINFO_SERIESNAME = 21,
        IPOD_TRACKINFO_SEASONNUMBER = 22,
        IPOD_TRACKINFO_TRACKVOLUMEADJUST = 23,
        IPOD_TRACKINFO_TRACKEQPRESET = 24,
        IPOD_TRACKINFO_DATARATE = 25,
        IPOD_TRACKINFO_BOOKMARKOFFSET = 26,
        IPOD_TRACKINFO_STARTSTOPOFFSET = 27,
        IPOD_TRACKINFO_RESERVED = 28
    };

public:
    enum{
        IPOD_TIME_SCHEDULE_PLAYINFO = 0,
        IPOD_TIME_SCHEDULE_REPEAT = 1,
        IPOD_TIME_SCHEDULE_SHUFFLE = 2,
        IPOD_TIME_SCHEDULE_MAX
    };

    void *piPodLibApiHandle;
    int	iPodControlState;
    bool fUsedAsync;
    int64_t gLastEventTime;
    int64_t gLastEventGetPlayInfoTime[IPOD_TIME_SCHEDULE_MAX];
    short *gAudioBuff;
    void *mIapTimer;
    bool mUsbConnected;
    unsigned int mSvcState;
    
    //static  void        instantiate();
    void SetUsbConnected(bool flag);
    bool GetUsbConnected(void);
    void SetSvcState(int state);
    int GetSvcState(void);
    int IsExistHid(void);
    bool Util_IsInvalidIndex(long index);
    int UtilGetCurrentTrackIndex(long *currentIndex);
    int UtilGetDrPlayStatus(void *playStatus);
    virtual bool WrapCommand(const Parcel *data,Parcel *reply);
    bool CheckElapsedTime(int64_t startTime);
    bool ProcInit(const Parcel *data,Parcel *reply);
    virtual bool Init(const Parcel *data);
    bool ProcDeinit();
    virtual bool Deinit();
    virtual bool GetIapState(Parcel *reply);    
    virtual bool GetIapSvcState(Parcel *reply);
    bool ProcTerminate();
    virtual bool Terminate();
    bool ProcDoPlay(Parcel *reply,int sendNoti,int cmdFrom);
    bool ProcPlay(Parcel *reply);
    virtual bool Play(const Parcel *data);
    virtual bool PlayInBrwsList();
    bool ProcNewPlay(Parcel *reply);
    bool NewPlay(const Parcel *data);
    bool ProcNewPause(Parcel *reply);
    bool NewPause(const Parcel *data);
    bool ProcDoPause(Parcel *reply,int sendNoti,int cmdFrom);
    bool ProcPause(Parcel *reply);
    virtual bool Pause(const Parcel *data);
    bool ProcNext(Parcel *reply);
    bool Next(const Parcel *data);
    bool ProcPrevious(Parcel *reply);
    bool Previous(const Parcel *data);
    bool ProcNextTrack(Parcel *reply);
    virtual bool NextTrack(const Parcel *data);
    bool ProcPreviousTrack(Parcel *reply);
    virtual bool PreviousTrack(const Parcel *data);
    bool ProcFF(Parcel *reply);
    virtual bool FF(const Parcel *data);
    bool ProcRew(Parcel *reply);
    virtual bool Rew(const Parcel *data);
    bool DoEndFfRew();
    bool ProcEndFfRew(Parcel *reply);
    virtual bool EndFfRew(const Parcel *data);
    bool ProcGetCurrentTrackIndex(Parcel *reply);
    virtual bool GetCurrentTrackIndex(const Parcel *data,Parcel *reply);
    bool ProcGetTotalPlayTracks(Parcel *reply);
    virtual bool GetTotalPlayTracks(const Parcel *data,Parcel *reply);
    bool ProcGetTitle(const Parcel *data,Parcel *reply);    
    virtual bool GetTitle(const Parcel *data,Parcel *reply);    
    bool ProcGetArtist(const Parcel *data,Parcel *reply);
    virtual bool GetArtist(const Parcel *data,Parcel *reply);
    bool ProcGetAlbum(const Parcel *data,Parcel *reply);
    virtual bool GetAlbum(const Parcel *data,Parcel *reply);
    bool ProcGetGenre(const Parcel *data,Parcel *reply);
    virtual bool GetGenre(const Parcel *data,Parcel *reply);
    bool ProcGetAlbumArt(const Parcel *data,Parcel *reply);
    virtual bool GetAlbumArt(const Parcel *data,Parcel *reply);
    bool GetArtworkFormats(const Parcel *data,Parcel *reply);
    bool ProcGetArtworkFormats(const Parcel *data,Parcel *reply);
    bool GetArtworkCount(const Parcel *data,Parcel *reply);
    bool ProcGetArtworkCount(const Parcel *data,Parcel *reply);
    bool GetArtworkTimes(const Parcel *data,Parcel *reply);
    bool ProcGetArtworkTimes(const Parcel *data,Parcel *reply);
    bool GetArtworkData(const Parcel *data,Parcel *reply);
    bool ProcGetArtworkData(const Parcel *data,Parcel *reply);
    bool ProcGetArtworkSizeWithFormatID(unsigned long formatId,unsigned long *pImageSize);
    bool ProcGetDeviceDisplayInfo(const Parcel *data,Parcel *reply);
    virtual bool GetDeviceDisplayInfo(const Parcel *data,Parcel *reply);
    bool ProcSetDeviceDisplayImage(const Parcel *data,Parcel *reply);
    virtual bool SetDeviceDisplayImage(const Parcel *data,Parcel *reply);
    bool ProcSetTrackPosition(const Parcel *data,Parcel *reply);
    virtual bool SetTrackPosition(const Parcel *data);
    bool ProcDatabaseInit(const Parcel *data,Parcel *reply);
    virtual bool DatabaseInit(const Parcel *data,Parcel *reply);
    bool ProcDatabaseSelectAll(const Parcel *data,Parcel *reply);
    virtual bool DatabaseSelectAll(const Parcel *data,Parcel *reply);
    bool ProcDatabaseSelect(const Parcel *data,Parcel *reply);
    virtual bool DatabaseSelect(const Parcel *data,Parcel *reply);
    bool ProcDatabaseMoveUpper(const Parcel *data,Parcel *reply);
    virtual bool DatabaseMoveUpper(const Parcel *data,Parcel *reply);
    bool ProcDatabaseGetNames(const Parcel *data,Parcel *reply);
    virtual bool DatabaseGetNames(const Parcel *data,Parcel *reply);
    virtual bool ExtFunc1();
    virtual bool ExtFunc2();
    virtual bool ExtFunc3();
    virtual bool ExtFunc4();
    virtual bool ExtFunc5();
    bool ProcGetDrPlayStatus(const Parcel *data,Parcel *reply);
    virtual bool GetDrPlayStatus(const Parcel *data,Parcel *reply);
    bool ProcGetPlayStatusInfo(const Parcel *data,Parcel *reply);
    virtual bool GetPlayStatusInfo(const Parcel *data,Parcel *reply);
    bool ProcToggleRepeat(const Parcel *data,Parcel *reply);
    virtual bool ToggleRepeat(const Parcel *data);
    bool ProcToggleShuffle(const Parcel *data,Parcel *reply);
    virtual bool ToggleShuffle(const Parcel *data);
    bool ProcGetTrackInfos(const Parcel *data,Parcel *reply);
    virtual bool GetTrackInfos(const Parcel *data,Parcel *reply);
    bool ProcGetTrackMetas(const Parcel *data,Parcel *reply);
    virtual bool GetTrackMetas(const Parcel *data,Parcel *reply);
    bool ProcDisconnected();
    virtual bool Disconnected();
    bool setDisconnected(bool fCausedByHw);
    bool ProcSetCurrentPlayingTrack(const Parcel *data,Parcel *reply);
    virtual bool SetCurrentPlayingTrack(const Parcel *data);
    bool ProcSetRepeat(const Parcel *data,Parcel *reply);
    virtual bool SetRepeat(const Parcel *data);
    bool ProcGetRepeat(const Parcel *data,Parcel *reply);
    virtual bool GetRepeat(const Parcel *data,Parcel *reply);
    bool ProcSetShuffle(const Parcel *data,Parcel *reply);
    virtual bool SetShuffle(const Parcel *data);
    bool ProcGetShuffle(const Parcel *data,Parcel *reply);
    virtual bool GetShuffle(const Parcel *data,Parcel *reply);
    bool ProcGetIndexedPlayingInfo(const Parcel *data,Parcel *reply);
    virtual bool GetIndexedPlayingInfo(const Parcel *data,Parcel *reply);
    bool ProcGetModelNum(const Parcel *data,Parcel *reply);
    virtual bool GetModelNum(const Parcel *data,Parcel *reply);
    bool ProcGetProtocolVersion(const Parcel *data,Parcel *reply);
    virtual bool GetProtocolVersion(const Parcel *data,Parcel *reply);
    bool ProcGetIpodSerialNumber(const Parcel *data,Parcel *reply);
    virtual bool GetIpodSerialNumber(const Parcel *data,Parcel *reply);
    bool ProcGetIpodSoftwareVersion(const Parcel *data,Parcel *reply);
    virtual bool GetIpodSoftwareVersion(const Parcel *data,Parcel *reply);
    bool ProcGetIpodName(const Parcel *data,Parcel *reply);
    virtual bool GetIpodName(const Parcel *data,Parcel *reply);
    bool ProcResponseFailByListenerName(
                                                                    IAP_CMD_DATA_TYPE *cmd,
                                                                    IAP_RSP_DATA_TYPE *rsp,
                                                                    const Parcel *cmdData,
                                                                    Parcel *replyData,
                                                                    String8 *pName8);
    bool ProcResponseFailByProtocolName(
                                                                    const Parcel *data,
                                                                    Parcel *reply,
                                                                    String8 *pName8);
    bool ProcResponseFailBySessionId(const Parcel *data,Parcel *reply,unsigned long *pSessionId);
    bool ProcAccessoryDataTransfer(const Parcel *data,Parcel *reply,unsigned long *pSessionId);
    virtual bool AccessoryDataTransfer(const Parcel *data,Parcel *reply);
    bool ProcApplicationLaunch(const Parcel *data,Parcel *reply,String8 *pName8);
    virtual bool ApplicationLaunch(const Parcel *data,Parcel *reply);
    bool ProcMuteControl(bool flag);
    virtual bool MuteControl(bool flag);
    bool ProcSendPackedLingoCmd(const Parcel *data,Parcel *reply);
    virtual bool SendPackedLingoCmd(const Parcel *data,Parcel *reply);
    bool ProcExtraCmd(const Parcel *data,Parcel *reply);
    virtual bool ExtraCmd(const Parcel *data,Parcel *reply);
    uint8_t *SendPacketedCmd(uint8_t lingo,
                                                            uint16_t command,
                                                            uint16_t retCommand,
                                                            uint8_t *payload,
                                                            int payloadLen,
                                                            void *pCMDErrorCode,
                                                            int *pRetPayloadLen
                                                            );
    bool SendPacketedCmdForTrackInfo(Parcel *reply,
                                                            uint8_t lingo,
                                                            uint16_t command,
                                                            uint16_t retCommand,
                                                            uint8_t *payload,
                                                            int payloadLen,
                                                            void *pCMDErrorCode,
                                                            int receiveCnt,
                                                            bool isUidTrackInfo,
                                                            long long trackUID
                                                            );    
    bool ProcGetNowPlayingAppBundleName(const Parcel *data,Parcel *reply);
    bool GetNowPlayingAppBundleName(const Parcel *data,Parcel *reply);
    bool ProcResumeIpod(const Parcel *data,Parcel *reply);
    bool ResumeIpod(const Parcel *data,Parcel *reply);
    bool ProcGetUIDTrackInfo(const Parcel *data,Parcel *reply);
    bool GetUIDTrackInfo(const Parcel *data,Parcel *reply);
    bool ProcGetPBTrackInfo(const Parcel *data,Parcel *reply);
    bool GetPBTrackInfo(const Parcel *data,Parcel *reply);
    bool ProcGetDBTrackInfo(const Parcel *data,Parcel *reply);
    bool GetDBTrackInfo(const Parcel *data,Parcel *reply);
    bool ProcGetIpodCaps(const Parcel *data,Parcel *reply);
    bool GetIpodCaps(const Parcel *data,Parcel *reply);
    bool ProcGetIpodFreeSpace(const Parcel *data,Parcel *reply);
    bool GetIpodFreeSpace(const Parcel *data,Parcel *reply);
    bool ProcOpenIpodFeatureFile(const Parcel *data,Parcel *reply);
    bool OpenIpodFeatureFile(const Parcel *data,Parcel *reply);
    bool ProcWriteIpodFileData(const Parcel *data,Parcel *reply);
    bool WriteIpodFileData(const Parcel *data,Parcel *reply);
    bool ProcIsSupportTagging(const Parcel *data,Parcel *reply);
    bool ProcCloseIpodFile(const Parcel *data,Parcel *reply);
    bool CloseIpodFile(const Parcel *data,Parcel *reply);
    virtual bool MsgFlush(void);
    virtual status_t setListener(const sp<IIapServiceNotify>& listener,String8 listenerName);
    virtual status_t removeListener(String8 listenerName);
    status_t removeListener(String8 listenerName,bool notiFlag);
    virtual     void        binderDied(const wp<IBinder>& who);
    static char const* getServiceName() { return "iapservice"; }

    bool ProcCheckAlbumArtFormats(void);
    bool ProcGetAlbumArtworkParams(long index,void *pTrackParam);
    bool ProcCheckAlbumArtwork(long index,Parcel *reply);
    bool ProcGetSmallArtworkFormatId(long index,unsigned long *pNewFormatId);
    bool ProcGetSmallArtwork(
                                                    long nIndex,
                                                    int bufSize,
                                                    void *pData,
                                                    Parcel *reply
                                                    );
    bool ProcSearchAlbumArtworkFormat(void *pTrackParam,int *formatId,int *artworkIndex,int *imageSize);
    bool ProcNewGetAlbumArtwork(long index,int formatId,int timeOffset,int imageSize,Parcel *reply);
    bool ProcGetArtworkTimes(long index,int formatId,int artworkIndex,int *times);

    void AddTrackInfos(Parcel *itemPool,
                                                Parcel *trackInfoPool,
                                                int itemCnt,
                                                int lastIndex
                                                );
    void AddTrackInfosUid(Parcel *itemPool,
                                                Parcel *trackInfoPool,
                                                int itemCnt,
                                                long long lastUid
                                                );    
    bool ProcGetTrackInfo(const Parcel *data,Parcel *reply,bool fIsPB);
    bool ProcEapPlay(const Parcel *data,Parcel *reply,String8 *pName8);
    bool EapPlay(const Parcel *data);
    bool ProcEapPause(const Parcel *data,Parcel *reply,String8 *pName8);
    bool EapPause(const Parcel *data);
    bool ProcEapGetDrPlayStatus(const Parcel *data,Parcel *reply,String8 *pName8);
    bool EapGetDrPlayStatus(const Parcel *data,Parcel *reply);

    bool SyncCmd1(int cmdID,const Parcel *data,Parcel *reply);
    bool ProcSyncCmd1(int cmdID,const Parcel *data,Parcel *reply);
    bool AsyncCmd1(int cmdID,const sp<IIapServiceCallback>& cbFunc ,const Parcel *data,Parcel *reply);
    bool ProcCallCb(const sp<IIapServiceCallback>& cbFunc ,Parcel *reply);
    bool ProcAsyncCmd1(int cmdID,const sp<IIapServiceCallback>& cbFunc ,const Parcel *data,Parcel *reply);
    bool ProcDispatchCmd(IAP_CMD_DATA_TYPE *cmd,const Parcel *cmdData,Parcel *replyData,void *pData);
    bool ProcExcuteCmd(int cmdID,
                                            IAP_CMD_DATA_TYPE *cmd,
                                            IAP_RSP_DATA_TYPE *rsp,
                                            Parcel *replyData,
                                            unsigned long *eapSessionId,
                                            String8 *protocolName,
                                            bool asyncCmd
                                            );
    void ProcParseCmdWithCb(IAP_CMD_DATA_TYPE *cmd);
    
    int CheckIndexofTrackInfo(uint8_t *pPayload,int payloadLen,int *trackIndex);
    int CheckUIDofTrackInfo(uint8_t *pPayload,int payloadLen,long long *trackIndex);
    bool ParseTrackInformation(uint8_t *pPayload,int payloadLen,Parcel *reply,bool isUidTrackInfo);
    int PutShort(uint8_t *buf,int value);
    int PutInt(uint8_t *buf,int value);
    int PutLong(uint8_t *buf,long long value);
    int GetShort(uint8_t *buf,int *value);
    int GetInt(uint8_t *buf,int *value);
    int GetLong(uint8_t *buf,long long *value);
    int CheckBitMask(uint32_t bitMask);
    void InitDebug(void);
    bool IsInitialized(){return (IPOD_CONTROL_STATE_INITIALIZED == iPodControlState)? true:false; };
    void InitVariable(const Parcel *data);
    bool Initialize(const Parcel *data);
    bool InitPreparation(const Parcel *data);
    void AfterInit(bool ret);
    bool Deinitialize(bool fCausedByHw);
    bool IsUsedAsync();
    bool NextPreviousControl(int tyep,Parcel *reply);    
    bool FfRewControl(int type,Parcel *reply);
    void *GetIPodHandle(void);
    void SetIPodHandle(void *pHandle);
    void SetLastEventTime(int64_t newTime){gLastEventTime = newTime;};
    int64_t GetLastEventTime(void){return gLastEventTime;};
    bool UpdatePlayInfoTimeSchedule(int eventType);
    void SetLastEventPlayInfoTime(int type,int64_t newTime){gLastEventGetPlayInfoTime[type] = newTime;};
    int64_t GetLastEventPlayInfoTime(int type){return gLastEventGetPlayInfoTime[type];};
    int GetSvcLocalPlayStatus(void);
    void SetSvcLocalPlayStatus(int status);
    void SetSvcLocalPlayStatus(int status,int type);
    void SetSvcLocalPlayStatus(int status,int sendNoti,int cmdFrom);
    bool GetSvcLocalFFRewStatus(void);
    void SetSvcLocalFFRewStatus(bool status);
    int GetSvcApplePlayStatus(void);
    void SetSvcApplePlayStatus(int status);
    bool GetSvcAppleFFRewStatus(void);
    void SetSvcAppleFFRewStatus(bool status);
    void SetSvcWantedNextAction(int status);
    bool SendNextAction(int type);
    
    void registrerEapSessionId(void *pData);
    bool Sample_GetDRPlayStatus(void *playStatus);
    bool IsValidEapSessionId(unsigned long sessionID);
    void ClearEapSessionList(void);
    void ClearReadySessionList(void);
    bool IsEmptyReadySessionList(void);
    bool AddReadySessionList(unsigned long SessionID, unsigned long nProtocolIndex);
    bool RemoveReadySessionListBySessionId(unsigned long SessionID);
    bool RemoveReadySessionListByProtocolId(unsigned long nProtocolIndex);
    bool ProcCheckReadySession(int nProtocolIndex);
    int CloseDataSession(unsigned long SessionID );
    void startCommandThread();
    void stopCommandThread();
    void startEventThread();
    void stopEventThread();
    void startAudioThread();
    void stopAudioThread();
    void startTimerThread();
    void stopTimerThread();
    void playAudio();
    void pauseAudio();    
    void initAudio();
    void setInitAfterAudio();
    void stopAudio();
    void muteAudio(bool flag);
    void Dump(const char *buff,int length);
    virtual bool IsValidTimeSchedule(int64_t newTime);
    bool IsValidPlayInfoTimeSchedule(int eventType);

    class IapCommandThread : public Thread{
            IapService *mIapService;
            bool mCommandTreadExit;
            //sp<Looper> mLooper;
            //sp<WeakMessageHandler> mHandler;
            sp<BitTube> mCommand;
            sp<BitTube> mResponse;
            mutable Mutex mLooperLock;
            mutable sp<Looper> mLooper;
            mutable Mutex mLooperRspLock;
            mutable sp<Looper> mLooperRsp;
            mutable Mutex       	mCommandLock;
            mutable Mutex       	mCmdProcessLock;

            unsigned int mMsgFlushSig;
            long mMsgCount;
            mutable Mutex       	mMsgSyncSigLock;
            int mLastSendCmd;
            int mLastReqCmd;
            int mLastReqRsp;
            long mLastMsgCount;
            int mLastSyncReqCmd;
            int mLastSyncReqRsp;

            void *QueueCmd;
            void *QueueRsp;

            enum
            {
                IPOD_CMD_NONE = 0,
                IPOD_CMD_BUSY
            };
        
        public:
            IapCommandThread(IapService *iapService)
            : Thread(false), mIapService(iapService),mCommandTreadExit(false),
                mCommand(new BitTube()),mResponse(new BitTube()) ,
                mMsgFlushSig(0){CommandInit();}
            int CommandRun();
            void CommandInit();
            void CommandDeinit();
            virtual bool threadLoop(){
                CommandRun();
                return false;
            }
            virtual void requestExit();
            //void init();
            int getCmdFd() const;
            sp<Looper> getCmdLooper() const;
            status_t waitForCmd() const;
            status_t wakeCmd() const;

            int getRspFd() const;
            sp<Looper> getRspLooper() const;
            status_t waitForRsp() const;
            status_t wakeRsp() const;
            void incMsgFlushSig(void);
            unsigned int getMsgFlushSig(void);
            long getMsgCount(void);
            bool sendCommandMessage(IAP_CMD_DATA_TYPE *pCmd);
            bool sendCommand(int what, const Parcel *data,Parcel *reply,bool async);
            bool sendCommandArg1(int what,int data,bool async);
            bool sendCommandCbFunc(int what, const Parcel *data,Parcel *reply,bool async,int cmdID,const sp<IIapServiceCallback>& cbFunc);
            ssize_t readCommand(void *msg, size_t numEvents);
            bool sendResponse(IAP_RSP_DATA_TYPE *rsp);
            ssize_t readResponse(void *msg, size_t numEvents);
            bool waitResponse();            
            bool IsInvalidState(int what,unsigned int msgFlushSig);
            void WaitUntilCmdFinish();
            bool DispatchCmd(IAP_CMD_DATA_TYPE *cmd,const Parcel *cmdData,Parcel *replyData,void *pData);
            void ParseCmd(IAP_CMD_DATA_TYPE *cmd);
            void ParseCmdWithCb(IAP_CMD_DATA_TYPE *cmd);
            void SetLastMsgCount(long count);
            void SetLastReqRsp(int rsp);
            void SetLastSyncReqCmd(int cmd);
            void SetLastSyncReqRsp(int rsp);
    };


    class IapEventThread : public Thread {
        IapService *mIapService;
        bool mEventTreadExit;
        public:
        IapEventThread(IapService *iapService)
        : Thread(false), mIapService(iapService),mEventTreadExit(false) { }

        int EventRun();
        virtual bool threadLoop() {
            EventRun();
            return false;
        }

        virtual void requestExit();
    };	

    class IapAudioThread : public Thread {
            IapService *mIapService;

            bool                mPaused;
            Condition           mAudioThreadRunCond;
            bool                mIsAudioRunning;

            bool                mWaitPaused;
            Mutex               mAudioPauseLock;
            Condition           mAudioPausedCond;
            
            bool mAudioTreadExit;
            uint32_t g_sample_rate;
            bool g_sample_rate_change;
            bool g_firstAfterInit;
            uint32_t g_channels;

            short *gPcmBuff;
            int   gBufferSize;

            mutable Mutex       	mPlaybackLock;
            sp<AudioTrack>  gPlaybackHandle;
            bool mMuteFlag;

            mutable Mutex       	mCaptureLock;
#ifdef USE_DIRECT_AUDIO_HAL
            void* gCaptureHandle;
#else
            AudioRecord* gCaptureHandle;
#endif /* USE_DIRECT_AUDIO_HAL */
            
        public:
            IapAudioThread(IapService *iapService);

            virtual void requestExit();
            int AudioInit(int *pBufSize);
            int AudioInitFunc(int *pBufSize);
            bool AudioResetFirst();
            void AudioDeinit();
            void AudioSetSampleRate(uint32_t sampleRate);
            void AudioSetChannels(uint32_t channels);
            void AudioSetBuffer(short * pPcmBuff);
            bool AudioStart();
            bool AudioFirstStart();
            int AudioRun();
            void AudioPause();
            bool AudioMute(bool flag);
            bool IsRunning();
            virtual bool threadLoop() {
                AudioRun();
                return false;
            }
    };	

    class IapStatusManager{
        IapService *mIapService;
        long mCurrentIndex;
        int mLocalPlayStatus;
        int mApplePlayStatus;
        bool mLocalFFRewStatus;
        bool mAppleFFRewStatus;
        int mExpectNextStatus;
        int mWantedNextAction;
        void *mIapTimer;
        public:
            IapStatusManager(IapService *iapService,void *timer);
            void ClearAll(void);
            void SetWantedNextAction(int status);
            int GetWantedNextAction(void);
            void SetExpectNextStatus(int status,int sendNoti,unsigned int cmdFrom);
            void ClearExpectNextStatus(void);
            int GetExpectNextStatus(void);
            void SetLocalPlayStatus(int status,int sendNoti,unsigned int cmdFrom);
            void SetLocalPlayStatus(int status,bool setExpected,int sendNoti,unsigned int cmdFrom);
            int GetLocalPlayStatus(void);
            void SetLocalFFRewStatus(bool status);
            bool GetLocalFFRewStatus(void);
            void SetApplePlayStatus(int status);
            int GetApplePlayStatus(void);
            void SetAppleFFRewStatus(bool status);
            bool GetAppleFFRewStatus(void);
    };

    mutable Mutex       	mLibStackOsLock;
    mutable Mutex       	mCommandLock;
    sp<IapCommandThread>  mIapCommandThread;
    mutable Mutex       	mEventLock;
    sp<IapEventThread>  mIapEventThread;
    mutable Mutex       	mAudioLock;
    sp<IapAudioThread>   mIapAudioThread;
    mutable Mutex       	mEaSessionLock;
    mutable Mutex       	mListenerLock;
    mutable Mutex       	mReadySessioLock;
    IapStatusManager     *mStatusManager;

                        IapService();
    virtual             ~IapService();

//    sp<IIapServiceNotify> mListener; 
    KeyedVector<String8, sp<IIapServiceNotify> > mListenerList; // protocol name, listener
    KeyedVector<int, String8 > mEapProtocolList;                    // protocol index, protocol name
    KeyedVector<int, sp<IIapServiceNotify> > mEapSessionList;        // session id , listener
    KeyedVector<int, int > mReadySessionList;        // protocol index , session id

    void notify(int what, int arg1, int arg2,int arg3);
    void notifyByName(int what, int arg1, int arg2,int arg3,String8 listenerName);
    void notifyObj(int what, int arg1, int arg2,Parcel *obj);
    void notifyObjConnect(int what, int arg1, int arg2,Parcel *obj) ;
    void notifyObjEap(int what, int arg1, int arg2,Parcel *obj,int sessionId) ;
    void notifyObjEapByName(int what, int arg1, int arg2,Parcel *obj,String8 name);
    void notifyObjByName(int what, int arg1, int arg2,Parcel *obj,String8 listenerName);
    void notifyEap(int what, int arg1, int arg2,int arg3,int sessionId);
    void notifyObjTag(int what, int arg1, int arg2,Parcel *obj) ;
    int notifyEapOpenData(
                                                                unsigned long SessionID, 
                                                                unsigned long nProtocolIndex );
};


};

#endif // ANDROID_IAPSERVICE_H

