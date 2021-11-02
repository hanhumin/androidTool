/*
**
** Copyright (C) 2013 Telechips Inc.
** Copyright 2008, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/



#ifndef ANDROID_HARDWARE_IPODSERVICE_H
#define ANDROID_HARDWARE_IPODSERVICE_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/RefBase.h>
#include <utils/Errors.h>
#include <binder/IInterface.h>
#include <binder/IMemory.h>
#include <binder/Parcel.h>
#include <utils/String8.h>
#include <binder/BinderService.h>
#include "IIpodService.h"
#include "iPod_HID.h"
#include "iAP2Client.h"




namespace android {


class IpodService: 
	public BinderService<IpodService>, 
	public BnIpodService,
	public IBinder::DeathRecipient
{

public:
    virtual     void        binderDied(const wp<IBinder>& who);
    static char const* getServiceName() { return "IpodService"; }
	virtual status_t setListener(const sp<IIapServiceNotify>& listener,String8 listenerName);
	virtual status_t setPcmListener(const sp<IPcmServiceNotify>& listener,String8 listenerName);
	
	virtual status_t setPcmFileListener(const sp<IPcmServiceNotify>& listener,String8 listenerName);
	
    virtual void stopHid();
	virtual void startIpod();
	virtual void stopIpod();
	virtual void send0Hid();
	virtual void random();
	virtual void repeat();
	virtual void play();
	virtual void pause();
	virtual void setPlayPosition(int position);	
	virtual void setPlaybackQueueIndex(int index);	
	virtual void setPQListContentTransferStartIndex(int index);
	
	virtual void notifyMicRecord(int startStop);
	virtual void playPause(int mode);
	virtual void playNextTrack();
	virtual void playProvTrack();
	virtual void StartMediaLibraryInformation();
	virtual void StopMediaLibraryInformation();
	virtual void PlayMediaLibraryItems(uint8_t * itemsPids,int sizePidsByte,int itemsStartIndex,uint8_t * mediaLibraryUid,int sizeMediaLibraryUidBytes);
	virtual void StartMediaLibraryUpdates(uint8_t * mediaLibraryUid, int sizeUid,
											uint8_t *lastKnownMediaLibraryRevision,int sizeRevision);
	virtual void StopMediaLibraryUpdates(uint8_t * mediaLibraryUid, int sizeUid);
	virtual void PlayMediaLibraryCurrentSelection(uint8_t * mediaLibraryUid, int sizeUid);
	
	virtual void PlayMediaLibrarySpecial(int mediaLibraryUidSize,uint8_t *bufMediaLibraryUId,bool isPlayAll,
		int startMediaItemPidSize,uint8_t *bufMediaItemPid);
	virtual void PlayMediaLibraryCollection(int pid_size,uint8_t * bufPid,int type,int startIndex,int mediaLibraryUidSize,uint8_t *bufMediaLibraryUId,
		int startMediaItemPidSize,uint8_t *bufMediaItemPid);
	virtual void StartNowPlayingUpdates();
	virtual void StopNowPlayingUpdates();
	virtual void stopUSBDeviceModeAudio();
	
    virtual status_t removeListener(String8 listenerName);
    status_t removeListener(String8 listenerName,bool notiFlag);
    void notifyUSBDeviceModeAudioInformation(int sampleRate);
	void notifyOnMediaLibraryName(uint8_t * data,int len);
	void notifyOnMediaItemTile(uint8_t * data,int len);
	void notifyOnPcmDataRecive(uint8_t * data,int len);	
	void notifyOnMediaLibraryInformation(uint8_t * data,int len);	
	void notifyOnMediaLibraryUpdate(uint8_t * data,int len);		
	void notifyOnNowPlayingMediaItemAttributes(uint8_t * data,int len);	
	void notifyOnNowPlayingPlayBackAttributes(uint8_t * data,int len);
	void notifyOnMediaItemAlumArtist(uint8_t * data,int len);
	void notifyOnMediaItemArtist(uint8_t * data,int len);
	void notifyOnMediaItemAlumTitle(uint8_t * data,int len);
	void notifyOnPlayBackPosition(int position);
	void notifyOnPlayBackQueueIndex(int index);
	void notifyOnPlayBackQueueCount(int count);
	void notifyOnPlayBackQueueChapterIndex(int index);
	void notifyOnPlayBackStatus(int status);
	void notifyOnPlayBackShuffleMode(int mode);
	void notifyOnPlayBackRepeatMode(int mode);
	void notifyOnMeidaLibUpdatePlayListName(uint8_t * data,int len);
	void notifyOnPlayBackAppName(uint8_t * data,int len);
	void notifyOnPBMediaLibraryUniqueIdentifier(uint8_t * data,int len);
	void notifyOnPBAppleMusicRadioAd(int status);
	void notifyOnPBAppleMusicRadioStationName(uint8_t * data,int len);
	void notifyOnMeidaLibUpdateParentPlayListPID(uint8_t * data,int len); 
	void notifyOnMeidaLibUpdatePlayListPID(uint8_t * data,int len); 
	void notifyOnPBAppleMusicRadioStationID(uint8_t * data,int len); 
	void notifyOnMeidaLibraryUID(uint8_t * data,int len); 
	void notifyOnPlayBackSpeed(int position);
    void notifyOnSetElapsedTimeAvailable(int status);
	void notifyOnContainedMediaItemsFsId(int id);
	void notifyOnMediaUpdatePlayListIsRadioStation(int status);
	void notifyOnMediaUpdatePlayListIsFolder(int status);
	void notifyOnMediaUpdatePlayListIsGeniusMix(int status);
	void notifyOnPlaybackQueueListAvail(int status);
	void notifyOnPlayBackQueueListTransferID(int mode);
	void notifyOnPlayBackBundleID(uint8_t * data,int len);
	void notifyOnPlayBackQueueListContentTransfer(int mode);
	void notifyOnMediaItemPID(uint8_t * data,int len); 
	void notifyMediaItemMediaType(int type);
	void notifyMediaLibraryType(int type);
	void notifyMediaItemRating(int type);
	void notifyOnMediaItemAlbumPID(uint8_t * data,int len);
	void notifyOnMediaItemDeleteUpdatePID(uint8_t * data,int len);
	void notifyOnMediaPlayListDeleteUpdatePID(uint8_t * data,int len);
	void notifyMediaItemAlbumTrackNumber(int number);
	void notifyMediaItemAlbumTrackCount(int count);
	void notifyMediaItemAlbumDiscNumber(int number);
	void notifyMediaItemAlbumDiscCount(int number);
	void notifyOnMediaItemArtistPID(uint8_t * data,int len);
	void notifyOnMediaItemAlbumArtistPID(uint8_t * data,int len);
	void notifyOnMediaItemGenrePID(uint8_t * data,int len);
	void notifyOnMediaItemGenre(uint8_t * data,int len);
	void notifyOnMediaItemComposerPID(uint8_t * data,int len);
	void notifyOnMediaItemComposer(uint8_t * data,int len);
	void notifyOnPlayAllSongsCapable(int status);
	void notifyOnMediaUpdatePlayListContentTransfer();
	void notifyOnMediaNowPlayingArtworkStart();
	void NowPlayingPBQueueContentsStart(uint8_t * data,int len);
	void MediaItemNowPlayingPBQueueContentsFinish(uint8_t * data,int len);
	void MediaItemNowPlayingPBQueueContentsStart(uint8_t * data,int len);
	void MediaItemListUpdatePlaylistContentsStart(int fileName_len, const unsigned char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libraryUID);
	void MediaItemListUpdatePlaylistContentsFinish(int fileName_len, const unsigned char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libraryUID);
	void MediaLibraryUpdatePlaylistContentsStart(int fileName_len, const unsigned char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libraryUID);
	void MediaLibraryUpdatePlaylistContentsFinish(int fileName_len, const unsigned char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libraryUID);
	void NowPlayingPBQueueContentsFinish(uint8_t * data,int len);
	void notifyOnMediaNowPlayingArtworkFinish();	
	void notifyOnIpodStartReady();
	
	void notifyOnMediaLibraryReset();
	void notifyOnMediaLibraryUPdateProgress(int progress);
	void notifyOnIsHidingRemoteItems(int status);
	void notifyOnMediaItemIsPartOfCompilation(int status);
	void notifyOnMediaItemIsLikeSupported(int status);
	void notifyOnMediaItemIsBeanSupported(int status);
	void notifyOnMediaItemIsLiked(int isLiked);
	void notifyOnMediaItemIsBanned(int IsBanned);
	void notifyOnMediaItemIsResidentOnDevice(int IsResident);
	void notifyMediaItemArtWorkFileTransferID(int id);
	void notifyMediaItemChapterCount(int count);
	void notifyOnMeidaLibraryUpdateUID(uint8_t * data,int len);
	void notifyOnMeidaLibraryUpdateRevision(uint8_t * data,int len);
	/*
	
	(g_sessionId);

	*/

	void notifyOnDuration(int duration);
                        IpodService();
    virtual             ~IpodService();

	mutable Mutex       	mListenerLock;
	mutable Mutex       	mPcmListenerLock;
	mutable Mutex       	mPlayLock;
	KeyedVector<String8, sp<IIapServiceNotify> > mListenerList; // protocol name, listener
	iAP2Client_t * g_client ;

	sp<IIapServiceNotify> mIapServiceNotify;
	sp<IPcmServiceNotify> mPcmServiceNotify;
	sp<IPcmServiceNotify> mPcmFileNotify;


  
};


};

#endif // ANDROID_IAPSERVICE_H


#if 0
#ifndef ANDROID_HARDWARE_IIPODSERVICE_H
#define ANDROID_HARDWARE_IIPODSERVICE_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/RefBase.h>
#include <utils/Errors.h>
#include <binder/IInterface.h>
#include <binder/IMemory.h>
#include <binder/Parcel.h>
#include <utils/String8.h>
#include <binder/BinderService.h>
#include "IIpodService.h"



namespace android {

// ----------------------------------------------------------------------------
/*
class IpodService:
	public BinderService<IpodService>, 
	public BnIpodService
{

    public:
		  static char const* getServiceName() { return "IpodService"; }
};
*/
// ----------------------------------------------------------------------------

}; // namespace android

#endif //ANDROID_HARDWARE_IIPODSERVICE_H
#endif

