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
#include "IIapServiceNotify.h"
#include "IPcmServiceNotify.h"



namespace android {

// ----------------------------------------------------------------------------

class IIpodService : public IInterface
{
public: 
	DECLARE_META_INTERFACE(IpodService);
    virtual status_t setListener(const sp<IIapServiceNotify>& ,String8) = 0;
	
	virtual status_t setPcmListener(const sp<IPcmServiceNotify>& ,String8) = 0;
	virtual status_t setPcmFileListener(const sp<IPcmServiceNotify>& ,String8) = 0;
	virtual void stopHid() = 0;
	virtual void startIpod()=0;
	virtual void stopIpod()=0;
	virtual void playNextTrack() = 0;
	virtual void playProvTrack() = 0;
	virtual void StopMediaLibraryInformation() = 0;
	virtual void StartMediaLibraryInformation() = 0;
	virtual void PlayMediaLibraryItems(uint8_t * itemsPids,int sizePidsByte,int itemsStartIndex,uint8_t * mediaLibraryUid,int sizeMediaLibraryUidBytes) = 0;
	virtual void StartMediaLibraryUpdates(uint8_t * mediaLibraryUid, int sizeUid,
											uint8_t *lastKnownMediaLibraryRevision,int sizeRevision) = 0;
	virtual void StopMediaLibraryUpdates(uint8_t * mediaLibraryUid, int sizeUid) = 0;
	virtual void PlayMediaLibraryCurrentSelection(uint8_t * mediaLibraryUid, int sizeUid) = 0;
	virtual void PlayMediaLibraryCollection(int pid_size,uint8_t * bufPid,int type,int startIndex,int mediaLibraryUidSize,uint8_t *bufMediaLibraryUId,
		int startMediaItemPidSize,uint8_t *bufMediaItemPid)=0;
	
	virtual void PlayMediaLibrarySpecial(int mediaLibraryUidSize,uint8_t *bufMediaLibraryUId,bool isPlayAll,
		int startMediaItemPidSize,uint8_t *bufMediaItemPid)=0;
	virtual void StartNowPlayingUpdates() = 0;
	virtual void StopNowPlayingUpdates() = 0;
	virtual void stopUSBDeviceModeAudio() = 0;
	virtual void random() = 0;
	virtual void repeat() = 0;
	virtual void play() = 0;
	virtual void send0Hid()=0;
	virtual void pause() = 0;
	virtual void notifyMicRecord(int startStop) = 0;
	virtual void playPause(int mode) = 0;
	virtual void setPlayPosition(int position) = 0;	
	virtual void setPlaybackQueueIndex(int index) = 0;
	
	virtual void setPQListContentTransferStartIndex(int index) = 0;
	
    virtual status_t removeListener(String8) = 0; 
	
};

// ----------------------------------------------------------------------------

class BnIpodService : public BnInterface<IIpodService>
{
public:
	virtual status_t    onTransact( uint32_t code,
					const Parcel& data,
					Parcel* reply,
					uint32_t flags = 0);
};

// ----------------------------------------------------------------------------

}; // namespace android

#endif //ANDROID_HARDWARE_IIAPSERVICE_H

