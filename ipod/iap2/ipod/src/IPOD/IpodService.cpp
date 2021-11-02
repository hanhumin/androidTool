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

#define LOG_TAG "IIpodService"
#include <utils/Log.h>

#include <stdint.h>
#include <sys/types.h>
#include <binder/Parcel.h>
#include "IIpodService.h"
#include "IpodService.h"

#include <unistd.h>
#include <utils/String8.h>
#include <cutils/properties.h>
#include "iAP2_utils.h"
#include "iAP2Client.h"
#include "usbhost.h"

#include <utils/Log.h>
#define LOGD1(...) __android_log_print(ANDROID_LOG_DEBUG , "ppp", __VA_ARGS__)


namespace android {
int  usb_device_removed(const char *devname, void* client_data);
int usb_device_added(const char *devname, void* client_data);

int usb_device_added(const char *devname, void* client_data) {

	 struct usb_descriptor_header* desc;
    struct usb_descriptor_iter iter;
	LOGI("usb_device_added \n");

    struct usb_device *device = usb_device_open(devname);
    if (!device) {
        LOGI("usb_device_open failed \n");
        return 0;
    }

    const usb_device_descriptor* deviceDesc = usb_device_get_device_descriptor(device);

    uint16_t vendorId =  usb_device_get_vendor_id(device);
    uint16_t productId = usb_device_get_product_id(device);

    int fd = -1;
	
	LOGI("vendorId=0x%04x \n",vendorId);
	 if(vendorId==0x05ac)
    {
    
       mIpodService->startIpod();
	 }
	
	return 0;
   }
   
   int	usb_device_removed(const char *devname, void* client_data) {
   
	  return 0;
   }
   
   void*  monitorUsb(void* param)
	   {
		 
   
		 ///for test here
		 struct usb_host_context* context = usb_host_init();
		   if (!context) {
	   
			   return NULL;
		   }
   
		   usb_host_run(context, usb_device_added, usb_device_removed, NULL, NULL);
		   return NULL;
	   }

IpodService::IpodService()
    :BnIpodService()
{
        mIpodService = this;

	// pthread_t pThreadMonitorUsb;
	//pthread_create(&pThreadMonitorUsb, NULL, monitorUsb, (void *)mIpodService);
	
}
   


   void IpodService:: playNextTrack()
   	{
   	// Mutex::Autolock lock(mPlayLock);
   	 playNextTrackAccessoryHidReport(g_sessionId);
	// playPauseAccessoryHidReport(g_sessionId);
	  setPlayMode( PLAY_MODE_PLAY);
   	}

     void IpodService:: playProvTrack()
   	{
   	// Mutex::Autolock lock(mPlayLock);
   	  	playProvTracAccessoryHidReport(g_sessionId);
	   // playPauseAccessoryHidReport(g_sessionId);
		 setPlayMode( PLAY_MODE_PLAY);
   	}

    void IpodService:: StopMediaLibraryInformation()
    {
        iAP2StopMediaLibraryInformation(g_sessionId);
    }
    void IpodService:: StartMediaLibraryInformation()
    {
    
	LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
       iAP2StartMediaLibraryInformation(g_sessionId);
    }


	 void IpodService::PlayMediaLibraryItems(uint8_t * itemsPids,int sizePidsByte,int itemsStartIndex,
	 	uint8_t * mediaLibraryUid,int sizeMediaLibraryUidBytes)
	 {
	   LOGI("FUNCTION:%s line=%d sizePidsByte=%d\n",__FUNCTION__,__LINE__,sizePidsByte);
   	  LOGI("FUNCTION:%s line=%d sizeMediaLibraryUidBytes=%d\n",__FUNCTION__,__LINE__,sizeMediaLibraryUidBytes);
	  LOGI("FUNCTION:%s line=%d itemsStartIndex=%d\n",__FUNCTION__,__LINE__,itemsStartIndex);
	 	  iAP2StartMediaLibraryItems(g_sessionId, itemsPids, sizePidsByte,itemsStartIndex,
											mediaLibraryUid,sizeMediaLibraryUidBytes);
		    setPlayMode( PLAY_MODE_PLAY);
		   LOGI("FUNCTION:%s line=%d\n",__FUNCTION__,__LINE__);
	 }

	void IpodService:: PlayMediaLibrarySpecial(int mediaLibraryUidSize,uint8_t *bufMediaLibraryUId,
														bool isPlayAll,int startMediaItemPidSize,uint8_t *bufMediaItemPid)
	{
	iAP2PlayMediaLibrarySpecial(g_sessionId,
		mediaLibraryUidSize,bufMediaLibraryUId,isPlayAll,startMediaItemPidSize,bufMediaItemPid);
	 setPlayMode( PLAY_MODE_PLAY);
	}

	void IpodService:: PlayMediaLibraryCollection(int pid_size,uint8_t * bufPid,int type,int startIndex,
		int mediaLibraryUidSize,uint8_t *bufMediaLibraryUId,int startMediaItemPidSize,uint8_t *bufMediaItemPid)
	{
	iAP2PlayMediaLibraryCollection(g_sessionId,pid_size,bufPid,type,startIndex,
		mediaLibraryUidSize,bufMediaLibraryUId,startMediaItemPidSize,bufMediaItemPid);
	 setPlayMode( PLAY_MODE_PLAY);
	}

	void IpodService:: PlayMediaLibraryCurrentSelection(uint8_t * mediaLibraryUid, int sizeUid)
   	{
   	// Mutex::Autolock lock(mPlayLock);
   	      LOGI("FUNCTION:%s line=%d\n",__FUNCTION__,__LINE__);
   	
   	  	  iAP2PlayMediaLibraryCurrentSelection(g_sessionId,mediaLibraryUid,sizeUid);
		   setPlayMode( PLAY_MODE_PLAY);
		  LOGI("FUNCTION:%s line=%d\n",__FUNCTION__,__LINE__);
	
   	}
	/*
	void IpodService:: PlayMediaLibraryCurrentSelection(uint8_t * mediaLibraryUid, int sizeUid)
   	{
   	// Mutex::Autolock lock(mPlayLock);
   	      LOGI("FUNCTION:%s line=%d\n",__FUNCTION__,__LINE__);
   	
   	  	  iAP2PlayMediaLibraryCurrentSelection(g_sessionId,mediaLibraryUid,sizeUid);
		  
		  LOGI("FUNCTION:%s line=%d\n",__FUNCTION__,__LINE__);
	
   	}
	*/
    void IpodService:: StopMediaLibraryUpdates(uint8_t * mediaLibraryUid, int sizeUid)
   	{
   	// Mutex::Autolock lock(mPlayLock);
   	      LOGI("FUNCTION:%s line=%d\n",__FUNCTION__,__LINE__);
   	
   	  	  iAP2StopMediaLibraryUpdates(g_sessionId,mediaLibraryUid,sizeUid);
		  
		  LOGI("FUNCTION:%s line=%d\n",__FUNCTION__,__LINE__);
	
   	}
     void IpodService:: StartMediaLibraryUpdates(uint8_t * mediaLibraryUid, int sizeUid,
											uint8_t *lastKnownMediaLibraryRevision,int sizeRevision)
   	{
   	// Mutex::Autolock lock(mPlayLock);
   	      LOGI("FUNCTION:%s line=%d\n",__FUNCTION__,__LINE__);
   	
   	  	  iAP2StartMediaLibraryUpdates(g_sessionId,mediaLibraryUid,sizeUid,lastKnownMediaLibraryRevision,sizeRevision);
		  
		  LOGI("FUNCTION:%s line=%d\n",__FUNCTION__,__LINE__);
	
   	}
	  void IpodService:: StartNowPlayingUpdates()
   	{
   	 LOGI("FUNCTION:%s line=%d\n",__FUNCTION__,__LINE__);
   	// Mutex::Autolock lock(mPlayLock);
   	  	  iAP2StartNowPlayingUpdates(g_sessionId);
	
   	}

	   void IpodService:: StopNowPlayingUpdates()
   	{
   	  //  Mutex::Autolock lock(mPlayLock);
   	  	iAP2StopNowPlayingUpdates(g_sessionId);
	
   	}

	    void IpodService:: stopUSBDeviceModeAudio()
   	{
   	// Mutex::Autolock lock(mPlayLock);
	    iAP2StopNowPlayingUpdates(g_sessionId);
	 iAP2StopHid(g_sessionId);
   	  	iAP2StopUSBDeviceModeAudio(g_sessionId);
	
   	}
	 

   void IpodService::stopHid()
   	{
   	// Mutex::Autolock lock(mPlayLock);
   	  iAP2StopHid(g_sessionId);
	  
   	}

   void IpodService::stopIpod()
   	{
   	    setPlayMode( PLAY_MODE_STOP);
     	stopHid();
   	  if(g_client!=NULL)
		 iAP2ClientStop(g_client);	
		 g_client = NULL;
		 if(g_handle!=NULL)		 	
		   IPOD_HID_CloseHIDChannel(g_handle);
		 g_handle = NULL;
		 
		 freeAllIap2FtDataBuffer();
		 freeAllIap2FtReceiveDataBuffer();
		 freeAllIap2Session1DataBuffer();
		 
   	}
   void IpodService::startIpod()
   	{
   	#if 1

	LOGI("startIpod\n");

	// Mutex::Autolock lock(mPlayLock);
	   g_client = (iAP2Client_t *)iAP2ClientStart();
	   readFileListOne("/dev/usb/");
   	   g_handle = (IPOD_HID_CONTEXT *)IPOD_HID_Init();
	   IPOD_HID_CheckiPodDevice(g_handle);
	   IPOD_HID_SetHIDChannel(g_handle,/*"/dev/hidraw0",*/-1);
	//mIpodService->play();
	   
	#endif
		
   	}

   void IpodService::send0Hid()
   	{
	if(g_iap2handle==NULL)
		return;
	hid0AccessoryHidReport();
}
   void IpodService::random()
   	{
   	  randomAccessoryHidReport(g_sessionId);
	   hid0AccessoryHidReport();
   	}
   void IpodService::repeat()
   	{
   	   repeatAccessoryHidReport(g_sessionId);
	   hid0AccessoryHidReport();
   	}
   void IpodService::play( )
	  { 
	  if(PLAY_MODE_PLAY == getPlayMode())
	  	return;
	  setPlayMode( PLAY_MODE_PLAY);
	   playAccessoryHidReport(g_sessionId);
	   hid0AccessoryHidReport();
   	}
   void IpodService::pause( )
	   { 
	    if(PLAY_MODE_PAUSE == getPlayMode())
	  	return;
	   setPlayMode( PLAY_MODE_PAUSE);
		pauseAccessoryHidReport(g_sessionId);
		hid0AccessoryHidReport();
	 }
   
   void IpodService::setPQListContentTransferStartIndex(int index )
   {
	 iAP2SetPQListContentTransferStartIndex( index);
   }

    void IpodService::setPlaybackQueueIndex(int index )
    {
      iAP2StartPlaybackQueueIndex( index);
    }

  
    void IpodService::setPlayPosition(int position )
    {
      iAP2StartPlayPosition( position);
    }
	void IpodService::notifyMicRecord(int startStop)
	{

	  set_record_mode(startStop);
	}
	
   void IpodService::playPause(int mode )
   	{ 
   	// Mutex::Autolock lock(mPlayLock);

	 if(mode==PLAY_MODE_STOP)
	 {
	    if(g_client!=NULL)
		 iAP2ClientStop(g_client);	
		 g_client = NULL;
		 if(g_handle!=NULL)		 	
		   IPOD_HID_CloseHIDChannel(g_handle);
		 g_handle = NULL;
		 setPlayMode( mode);

	     
	   return;
	 }
	// setPlayMode( mode);
   //	 playPauseAccessoryHidReport(g_sessionId);
	 //testAccessoryHidReport2(g_sessionId);
	// playPauseAccessoryHidReport2(g_sessionId);
	playPauseAccessoryHidReport(g_sessionId);
	hid0AccessoryHidReport();
	if(getPlayMode() == PLAY_MODE_PLAY)
		{
		   setPlayMode( PLAY_MODE_PAUSE);
		}
	else
		{
		   setPlayMode( PLAY_MODE_PLAY);
		}
		
	 
   	}

    
	
	void  IpodService::notifyOnMediaItemComposer(uint8_t * data,int len)
	 {

		// Mutex::Autolock lock(mListenerLock);
		   if(mPcmServiceNotify!=NULL)
		   mPcmServiceNotify->notifyRawString(NOTIFY_CODE_MEDIA_COMPOSER,len ,(unsigned char *)data);
	 }

  
  void	IpodService::notifyOnMediaItemGenre(uint8_t * data,int len)
   {
  
	  // Mutex::Autolock lock(mListenerLock);
	     if(mPcmServiceNotify!=NULL)
		 mPcmServiceNotify->notifyRawString(NOTIFY_CODE_MEDIA_GENRE,len ,(unsigned char	*)data);
   }

  void  IpodService::notifyOnMediaItemAlumArtist(uint8_t * data,int len)
  {

	 // Mutex::Autolock lock(mListenerLock);
	    if(mPcmServiceNotify!=NULL)
	 	mPcmServiceNotify->notifyRawString(NOTIFY_CODE_MEDIA_ABLUM_ARTIST,len ,(unsigned char  *)data);
  }

  void  IpodService::notifyOnMediaItemArtist(uint8_t * data,int len)
  {
  // Mutex::Autolock lock(mListenerLock);
     if(mPcmServiceNotify!=NULL)
	 	mPcmServiceNotify->notifyRawString(NOTIFY_CODE_MEDIA_ARTIST,len ,(unsigned char  *)data);
  }
  
  void IpodService::NowPlayingPBQueueContentsFinish(uint8_t * data,int len)
	{ 
	
	LOGI("%s %d\n",__FUNCTION__,__LINE__);
		if(mPcmServiceNotify!=NULL)
		{
			LOGI("%s %d\n",__FUNCTION__,__LINE__);
		   mPcmServiceNotify->notifyRawString(NOW_PLAYING_PB_QUEUE_CONTENTS_FINISH,len ,(unsigned char  *)data);
		   
		   LOGI("%s %d\n",__FUNCTION__,__LINE__);
		}
	}
  
  
  void IpodService::MediaItemListUpdatePlaylistContentsFinish(int fileName_len, const unsigned char * fileName,
									   int playListPid_len,  const unsigned char  *playListPid,
									   int libraryUID_len,const unsigned char  *libraryUID)
	  { 


	  LOGI("%s %d\n",__FUNCTION__,__LINE__);
		  if(mPcmServiceNotify!=NULL)
		  {
		  LOGI("%s %d\n",__FUNCTION__,__LINE__);
			 mPcmServiceNotify->notifyIap2FileTransfer(MEDIA_ITEM_LIST_UPDATE_PLAYLIST_CONTENTS_FINISH,fileName_len,fileName,
														   playListPid_len,playListPid,libraryUID_len,libraryUID);
		  }
	  }
  void IpodService::MediaItemListUpdatePlaylistContentsStart(int fileName_len, const unsigned char * fileName,
									  int playListPid_len,	const unsigned char  *playListPid,
									  int libraryUID_len,const unsigned char  *libraryUID)
	 { 
	 
		 if(mPcmFileNotify!=NULL)
		 {
			mPcmFileNotify->notifyIap2FileTransfer(MEDIA_ITEM_LIST_UPDATE_PLAYLIST_CONTENTS_START,fileName_len,fileName,
														  playListPid_len,playListPid,libraryUID_len,libraryUID);
		 }
	 }

  void IpodService::MediaLibraryUpdatePlaylistContentsStart(int fileName_len, const unsigned char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libraryUID)
   { 
   
	   if(mPcmFileNotify!=NULL)
	   {
	   
	 //  LOGI("xx1 %s %d  fileName_len=%d fileName=%s\n",__FUNCTION__,__LINE__,fileName_len,fileName);
		  mPcmFileNotify->notifyIap2FileTransfer(MEDIA_LIBRARY_UPDATE_PLAYLIST_CONTENTS_START,fileName_len,fileName,
		  												playListPid_len,playListPid,libraryUID_len,libraryUID);
	   }
   }

   void IpodService::MediaLibraryUpdatePlaylistContentsFinish(int fileName_len, const unsigned char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libraryUID)
   { 
  // LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);
   
	   if(mPcmFileNotify!=NULL)
	   {
	 //  LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);
		  mPcmFileNotify->notifyIap2FileTransfer(MEDIA_LIBRARY_UPDATE_PLAYLIST_CONTENTS_FINISH,fileName_len,fileName,
		  												playListPid_len,playListPid,libraryUID_len,libraryUID);
		  	  // LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);
	   }
   }


void IpodService::MediaItemNowPlayingPBQueueContentsFinish(uint8_t * data,int len)
 { 
 LOGI("%s %d\n",__FUNCTION__,__LINE__);
	 if(mPcmServiceNotify!=NULL)
	 {
		mPcmServiceNotify->notifyRawString(MEDIA_ITEM_NOW_PLAYING_PB_QUEUE_CONTENTS_FINISH,len ,(unsigned char  *)data);
	 }
	 LOGI("%s %d\n",__FUNCTION__,__LINE__);
 }


  void IpodService::MediaItemNowPlayingPBQueueContentsStart(uint8_t * data,int len)
  { 
  LOGI("%s %d\n",__FUNCTION__,__LINE__);
	  if(mPcmServiceNotify!=NULL)
	  {
	  LOGI("%s %d\n",__FUNCTION__,__LINE__);
		 mPcmServiceNotify->notifyRawString(MEDIA_ITEM_NOW_PLAYING_PB_QUEUE_CONTENTS_START,len ,(unsigned char  *)data);
	  }
	  LOGI("%s %d\n",__FUNCTION__,__LINE__);
  }
   
  void IpodService::NowPlayingPBQueueContentsStart(uint8_t * data,int len)
  { 

  LOGI("%s %d\n",__FUNCTION__,__LINE__);
	  if(mPcmServiceNotify!=NULL)
	  {
	  
	  LOGI("%s %d\n",__FUNCTION__,__LINE__);
		 mPcmServiceNotify->notifyRawString(NOW_PLAYING_PB_QUEUE_CONTENTS_START,len ,(unsigned char  *)data);
	  }
  }

  void  IpodService::notifyOnMediaItemAlumTitle(uint8_t * data,int len)
  {
     // Mutex::Autolock lock(mListenerLock);
        if(mPcmServiceNotify!=NULL)
	 	mPcmServiceNotify->notifyRawString(NOTIFY_CODE_MEDIA_ABLUM_TITLE,len ,(unsigned char  *)data);
  }

  
  void	IpodService::notifyOnMeidaLibraryUID(uint8_t * data,int len)
  {
	  if(mPcmServiceNotify!=NULL)

   mPcmServiceNotify->notifyRawString(NOTIFY_CODE_MEDIA_LIBRARY_UID,len ,(unsigned char	*)data);
  }
  
	
 void	IpodService::notifyOnMeidaLibUpdateParentPlayListPID(uint8_t * data,int len)
{
	 if(mPcmServiceNotify!=NULL)

	mPcmServiceNotify->notifyRawByte(NOTIFY_CODE_MEDIA_LIBRARY_UPDATE_PARENT_PLAYLIST_PID,len ,(unsigned char	*)data);
}
  void	IpodService::notifyOnMeidaLibUpdatePlayListPID(uint8_t * data,int len)
  {
	  if(mPcmServiceNotify!=NULL)
       mPcmServiceNotify->notifyRawByte(NOTIFY_CODE_MEDIA_LIBRARY_UPDATE_PLAYLIST_PID,len ,(unsigned char	*)data);
  }
  void	IpodService::notifyOnPBAppleMusicRadioStationID(uint8_t * data,int len)
  {
	  if(mPcmServiceNotify!=NULL)

   mPcmServiceNotify->notifyRawByte(NOTIFY_CODE_PB_APPLE_MUSIC_RADIO_STATION_PLAYLIST_ID,len ,(unsigned char	*)data);
  }
  
	
		
void IpodService::notifyOnMediaPlayListDeleteUpdatePID(uint8_t * data,int len)
{
	if(mPcmServiceNotify!=NULL)

 mPcmServiceNotify->notifyRawByte(NOTIFY_CODE_MEDIA_PLAY_LIST_DELETE_UPDATE_PID,len ,(unsigned char	  *)data);
}
void IpodService::notifyOnMediaItemDeleteUpdatePID(uint8_t * data,int len)
{
	if(mPcmServiceNotify!=NULL)

 mPcmServiceNotify->notifyRawByte(NOTIFY_CODE_MEDIA_ITEM_DELETE_UPDATE_PID,len ,(unsigned char	  *)data);
}

void IpodService::notifyOnMediaItemAlbumPID(uint8_t * data,int len)
{
	if(mPcmServiceNotify!=NULL)

 mPcmServiceNotify->notifyRawByte(NOTIFY_CODE_MEDIA_ITEM_ALBUM_PERSISTENT_ID,len ,(unsigned char	  *)data);
}


void	IpodService::notifyOnMediaItemComposerPID(uint8_t * data,int len)
{
	if(mPcmServiceNotify!=NULL)

mPcmServiceNotify->notifyRawByte(NOTIFY_CODE_MEDIA_ITEM_COMPOSER_PID,len ,(unsigned char	*)data);
}

void	IpodService::notifyOnMediaItemGenrePID(uint8_t * data,int len)
{
	if(mPcmServiceNotify!=NULL)

mPcmServiceNotify->notifyRawByte(NOTIFY_CODE_MEDIA_ITEM_GENRE_PID,len ,(unsigned char	*)data);
}

void	IpodService::notifyOnMediaItemAlbumArtistPID(uint8_t * data,int len)
{
	if(mPcmServiceNotify!=NULL)

mPcmServiceNotify->notifyRawByte(NOTIFY_CODE_MEDIA_ITEM_ALBUM_ARTIST_PID,len ,(unsigned char	*)data);
}

void	IpodService::notifyOnMediaItemArtistPID(uint8_t * data,int len)
  {
	if(mPcmServiceNotify!=NULL)

   mPcmServiceNotify->notifyRawByte(NOTIFY_CODE_MEDIA_ITEM_ARTIST_PID,len ,(unsigned char	*)data);
  }

void	IpodService::notifyOnMediaItemPID(uint8_t * data,int len)
  {
	if(mPcmServiceNotify!=NULL)

   mPcmServiceNotify->notifyRawByte(NOTIFY_CODE_MEDIA_ITEM_PERSISTENT_ID,len ,(unsigned char	*)data);
  }

void IpodService::notifyOnNowPlayingPlayBackAttributes(uint8_t * data,int len)
{
  
  if(mPcmServiceNotify!=NULL)
   mPcmServiceNotify->notifyRawByte(NOTIFY_ON_NOW_PLAYING_PLAYBACK_ATTRIBUTES,len ,(unsigned char  *)data);
}

void IpodService::notifyOnNowPlayingMediaItemAttributes(uint8_t * data,int len)
{
// Mutex::Autolock lock(mPcmListenerLock);
//if(mPcmServiceNotify == NULL)
	//LOGI("%s line:%d mPcmServiceNotify == NULL \n",__FILE__,__LINE__);
//else 
	
// LOGI("%s line:%d mPcmServiceNotify != NULL \n",__FILE__,__LINE__);
if(mPcmServiceNotify!=NULL)
 mPcmServiceNotify->notifyRawByte(NOTIFY_ON_NOW_PLAYING_MEDIA_ITEM_ATTRIBUTES,len ,(unsigned char  *)data);
}

void IpodService::notifyOnMediaLibraryUpdate(uint8_t * data,int len)
{
// Mutex::Autolock lock(mPcmListenerLock);
//if(mPcmServiceNotify == NULL)
	//LOGI("%s line:%d mPcmServiceNotify == NULL \n",__FILE__,__LINE__);
//else 
	
// LOGI("%s line:%d mPcmServiceNotify != NULL \n",__FILE__,__LINE__);
if(mPcmServiceNotify!=NULL)
 mPcmServiceNotify->notifyRawByte(NOTIFY_ON_MEDIA_LIBRARY_UPDATE,len ,(unsigned char  *)data);
}
void IpodService::notifyOnMediaLibraryInformation(uint8_t * data,int len)
{
// Mutex::Autolock lock(mPcmListenerLock);
//if(mPcmServiceNotify == NULL)
	//LOGI("%s line:%d mPcmServiceNotify == NULL \n",__FILE__,__LINE__);
//else 
	
// LOGI("%s line:%d mPcmServiceNotify != NULL \n",__FILE__,__LINE__);
LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);

if(mPcmServiceNotify!=NULL)
{
	LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);

 mPcmServiceNotify->notifyRawByte(NOTIFY_ON_MEDIA_LIBRARY_INFOMATION,len ,(unsigned char  *)data);
 
 LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
}
}


void  IpodService::notifyOnPcmDataRecive(uint8_t * data,int len)
{
// Mutex::Autolock lock(mPcmListenerLock);
//if(mPcmServiceNotify == NULL)
	//LOGI("%s line:%d mPcmServiceNotify == NULL \n",__FILE__,__LINE__);
//else 
	
// LOGI("%s line:%d mPcmServiceNotify != NULL \n",__FILE__,__LINE__);
if(mPcmServiceNotify!=NULL)
 mPcmServiceNotify->notifyRawByte(NOTIFY_CODE_MEDIA_PCM_DATA_RECEIVE,len ,(unsigned char  *)data);
}

void	IpodService::notifyOnPlayBackBundleID(uint8_t * data,int len)
{
   // Mutex::Autolock lock(mListenerLock);
   if(mPcmServiceNotify!=NULL)
	  mPcmServiceNotify->notifyRawString(NOTIFY_CODE_MEDIA_PLAYBACK_BUNDLE_ID,len ,(unsigned char  *)data);
}

void	IpodService::notifyOnMeidaLibUpdatePlayListName(uint8_t * data,int len)
{
   // Mutex::Autolock lock(mListenerLock);
      if(mPcmServiceNotify!=NULL)
	  mPcmServiceNotify->notifyRawString(NOTIFY_CODE_MEDIA_LIBRARY_UPDATE_PLAYLIST_NAME,len ,(unsigned char  *)data);
}

void	IpodService::notifyOnPlayBackAppName(uint8_t * data,int len)
{
   // Mutex::Autolock lock(mListenerLock);
      if(mPcmServiceNotify!=NULL)
	  mPcmServiceNotify->notifyRawString(NOTIFY_CODE_MEDIA_PLAYBACK_APP_NAME,len ,(unsigned char  *)data);
}

void	 IpodService::notifyOnPBAppleMusicRadioStationName(uint8_t * data,int len)
{
 // Mutex::Autolock lock(mListenerLock);
    if(mPcmServiceNotify!=NULL)
	mPcmServiceNotify->notifyRawString(NOTIFY_CODE_PB_APPLE_MUSIC_RADIO_STATION_NAME,len ,(unsigned char *)data);
}

void	  IpodService::notifyOnPBMediaLibraryUniqueIdentifier(uint8_t * data,int len)
{
 // Mutex::Autolock lock(mListenerLock);
 if(mPcmServiceNotify!=NULL)
	mPcmServiceNotify->notifyRawString(NOTIFY_CODE_PB_MEDIA_LIBRARY_UID,len ,(unsigned char	*)data);
}
 
	  

  
	  

void	IpodService::notifyOnMediaLibraryName(uint8_t * data,int len)
{
  // Mutex::Autolock lock(mListenerLock);
     if(mPcmServiceNotify!=NULL)
	 mPcmServiceNotify->notifyRawString(NOTIFY_CODE_MEDIA_LIBRARY_NAME,len ,(unsigned char  *)data);
}

  
  void  IpodService::notifyOnMediaItemTile(uint8_t * data,int len)
  {
     // Mutex::Autolock lock(mListenerLock);
        if(mPcmServiceNotify!=NULL)
	 	mPcmServiceNotify->notifyRawString(NOTIFY_CODE_MEDIA_TITLE,len ,(unsigned char  *)data);
  }

  void IpodService::notifyOnPlayBackStatus(int status)
  	{ 
  	//0 : stopped,1:playing;2:paused
	if(status==0 || status == 2)
		setPlayMode( PLAY_MODE_PAUSE);
	else		
  	 setPlayMode( PLAY_MODE_PLAY);
   //	 Mutex::Autolock lock(mListenerLock);
   	   if(mPcmServiceNotify!=NULL)
   	   	{
		   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_PLAYBACK_STATUS,status, 0,0);
   	   	}
   	}
  
void IpodService::notifyOnPlayBackRepeatMode(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	 mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_PLAYBACK_REPEAT_MODE,mode, 0,0);
	}
}


void IpodService::notifyOnPlayBackQueueListContentTransfer(int mode)
{ 

 if(mPcmServiceNotify!=NULL)
  {
	 mPcmServiceNotify->notify(NOTIFY_CODE_PLAY_BACK_QUEUELIST_CONTENT_TRANSFER,mode, 0,0);
  }
}

void IpodService::notifyOnPlayBackQueueListTransferID(int mode)
{ 

 if(mPcmServiceNotify!=NULL)
  {
	 mPcmServiceNotify->notify(NOTIFY_CODE_PLAY_BACK_QUEUELIST_TRANSFER_ID,mode, 0,0);
  }
}

void IpodService::notifyMediaItemArtWorkFileTransferID(int mode)
{ 

 if(mPcmServiceNotify!=NULL)
  {
	 mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_ART_WORK_FILE_TRANSFER_ID,mode, 0,0);
  }
}

void IpodService::notifyMediaItemRating(int mode)
{ 

 if(mPcmServiceNotify!=NULL)
  {
	 mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_MEDIA_RATING,mode, 0,0);
  }
}

void IpodService::notifyMediaItemAlbumDiscCount(int count)
{ 

 if(mPcmServiceNotify!=NULL)
  {
	 mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_ALBUM_DISC_COUNT,count, 0,0);
  }
}

void IpodService::notifyMediaItemAlbumDiscNumber(int number)
{ 

 if(mPcmServiceNotify!=NULL)
  {
	 mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_ALBUM_DISC_NUMBER,number, 0,0);
  }
}


void IpodService::notifyMediaItemChapterCount(int number)
{ 

 if(mPcmServiceNotify!=NULL)
  {
	 mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_CHAPTER_COUNT,number, 0,0);
  }
}

void IpodService::notifyMediaItemAlbumTrackCount(int number)
{ 

 if(mPcmServiceNotify!=NULL)
  {
	 mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_ALBUM_TRACK_COUNT,number, 0,0);
  }
}

void IpodService::notifyMediaItemAlbumTrackNumber(int number)
{ 

 if(mPcmServiceNotify!=NULL)
  {
	 mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_ALBUM_TRACK_NUMBER,number, 0,0);
  }
}

void IpodService::notifyMediaLibraryType(int mode)
{ 

 if(mPcmServiceNotify!=NULL)
  {
	 mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_LIBRARY_TYPE,mode, 0,0);
  }
}

void IpodService::notifyMediaItemMediaType(int mode)
{ 

 if(mPcmServiceNotify!=NULL)
  {
	 mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_MEDIA_TYPE,mode, 0,0);
  }
}

void IpodService::notifyOnPlayBackShuffleMode(int mode)
{ 

 if(mPcmServiceNotify!=NULL)
  {
	 mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_PLAYBACK_SHUFFLE_MODE,mode, 0,0);
  }
}

void IpodService::notifyOnContainedMediaItemsFsId(int id)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_CONTAINED_MEDIA_ITEM_FS_ID,id, 0,0);
	}
}

void IpodService::notifyOnMediaUpdatePlayListIsRadioStation(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_UPDATE_PLAYLIST_IS_RADIO_STATION,mode, 0,0);
	}
}

void IpodService::notifyOnMediaUpdatePlayListIsFolder(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_UPDATE_PLAYLIST_IS_FOLDER,mode, 0,0);
	}
}

void IpodService::notifyOnMediaUpdatePlayListIsGeniusMix(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_UPDATE_PLAYLIST_IS_GENIUSMIX,mode, 0,0);
	}
}

void IpodService::notifyOnPlaybackQueueListAvail(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_PLAY_BACK_QUEUELIST_AVAIL,mode, 0,0);
	}
}

void IpodService::notifyOnSetElapsedTimeAvailable(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_SET_ELAPSED_TIME_AVAILABLE,mode, 0,0);
	}
}



void IpodService::notifyOnMediaItemIsResidentOnDevice(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_IS_RESIDENT_ON_DEVICE,mode, 0,0);
	}
}

void IpodService::notifyOnMediaItemIsBanned(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_IS_BANNED,mode, 0,0);
	}
}

void IpodService::notifyOnMediaItemIsLiked(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_IS_LIKED,mode, 0,0);
	}
}

void IpodService::notifyOnMediaItemIsBeanSupported(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_IS_BEAN_SUPPORTED,mode, 0,0);
	}
}


void IpodService::notifyOnMediaItemIsLikeSupported(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_IS_LIKE_SUPPORTED,mode, 0,0);
	}
}

void IpodService::notifyOnIpodStartReady()
{ 

	LOGI("%s %d\n",__FUNCTION__,__LINE__);
   
	if(mPcmServiceNotify!=NULL)
	{
	LOGI("%s %d\n",__FUNCTION__,__LINE__);
	   mPcmServiceNotify->notify(NOTIFY_CODE_IPOD_START_READY,0, 0,0);
	}
}

	
void IpodService::notifyOnMediaNowPlayingArtworkFinish()
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_NOW_PLAYING_ARTWORK_DATA_FINISH,0, 0,0);
	}
}



void IpodService::notifyOnMediaNowPlayingArtworkStart()
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_NOW_PLAYING_ARTWORK_DATA_START,0, 0,0);
	}
}

void IpodService::notifyOnMediaUpdatePlayListContentTransfer()
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_UPDATE_PLAYLIST_CONTENT_TRANSFER,0, 0,0);
	}
}

void IpodService::notifyOnMediaLibraryReset()
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_LIBRARY_RESET,0, 0,0);
	}
}

void IpodService::notifyOnMediaLibraryUPdateProgress(int progress)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_LIBRARY_UPDATE_PROGRESS,progress, 0,0);
	}
}

void IpodService::notifyOnIsHidingRemoteItems(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_IS_HIDING_REMOTE_ITEMS,mode, 0,0);
	}
}

void IpodService::notifyOnPlayAllSongsCapable(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_PLAY_ALL_SONGS_CAPABLE,mode, 0,0);
	}
}

void IpodService::notifyOnMediaItemIsPartOfCompilation(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_ITEM_ISPARTOF_COMPILATION,mode, 0,0);
	}
}

void IpodService::notifyOnPBAppleMusicRadioAd(int mode)
{ 

	if(mPcmServiceNotify!=NULL)
	{
	   mPcmServiceNotify->notify(NOTIFY_CODE_PB_APPLE_MUSIC_RADIO_AD,mode, 0,0);
	}
}

  

  void IpodService::notifyOnPlayBackQueueChapterIndex(int index )
   	{ 
   //	 Mutex::Autolock lock(mListenerLock);
   	   if(mPcmServiceNotify!=NULL)
   	   	{
		   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_PLAYBACK_QUEUE_CAPTER_INDEX,index, 0,0);
   	   	}
   	}
  

   void IpodService::notifyOnPlayBackQueueCount(int count )
   	{ 
   //	 Mutex::Autolock lock(mListenerLock);
   	   if(mPcmServiceNotify!=NULL)
   	   	{
		   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_PLAYBACK_QUEUE_COUNT,count, 0,0);
   	   	}
   	}
  
  

  void IpodService::notifyOnPlayBackQueueIndex(int index)
   	{ 
   //	 Mutex::Autolock lock(mListenerLock);
   	   if(mPcmServiceNotify!=NULL)
   	   	{
		   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_PLAYBACK_QUEUE_INDEX,index, 0,0);
   	   	}
   	}
  
 void IpodService::notifyOnPlayBackSpeed(int position)
{ 
//	 Mutex::Autolock lock(mListenerLock);
   if(mPcmServiceNotify!=NULL)
   	{
   

   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_PLAYBACK_SPEED,position, 0,0);
   	}
}

  void IpodService::notifyOnPlayBackPosition(int position)
   	{ 
   //	 Mutex::Autolock lock(mListenerLock);
   	   if(mPcmServiceNotify!=NULL)
   	   	{
		   

		   mPcmServiceNotify->notify(NOTIFY_CODE_MEDIA_PLAYBACK_POSITION,position, 0,0);
   	   	}
   	}
  
   void IpodService::notifyOnDuration(int duration)
   	{ 
   	// Mutex::Autolock lock(mListenerLock);
   	   if(mPcmServiceNotify!=NULL)
   	   	{

		   mPcmServiceNotify->notify(NOTIFY_CODE_DURATION,duration, 0,0);
   	   	}
   	}
   void IpodService::notifyUSBDeviceModeAudioInformation(int samleRate)
   	{ 
   	// Mutex::Autolock lock(mListenerLock);
   	   if(mPcmServiceNotify!=NULL)
   	   	{

		   mPcmServiceNotify->notify(NOTIFY_CODE_SAMPLERATE,samleRate, 0,0);
   	   	}
   	}


	
		
	  status_t IpodService::setPcmFileListener(const sp<IPcmServiceNotify>& listener,String8 listenerName)
	{
	
#if 1
		sp<IPcmServiceNotify> mListener = listener;
		IInterface::asBinder(mListener)->linkToDeath(this);
		//mListener->asBinder()->linkToDeath(this);
		ALOGI("%d: %s() add listenerName %s %p", 
																		__LINE__,
																		__FUNCTION__,
																		listenerName.string(),
																		 IInterface::asBinder(mListener).get()
																	   /* mListener->asBinder().get()*/
																		);
		{
			Mutex::Autolock lock(mListenerLock);
		  
	  #if 1
		  // delete(mPcmServiceNotify);
		   mPcmFileNotify = listener;
		  if(mListener!=NULL)
			mListener->notify(0xF0002,0, 0,0);
			ALOGI("%d: %s()   new listener mListenerList.size() %d", 
																										__LINE__,
																										__FUNCTION__,
																										(int)mListenerList.size()
																										);
	  #else
			if ( index < 0) {
				ALOGI("%d: %s() add listener index %d", __LINE__, __FUNCTION__,index);
				mListenerList.add(listenerName, mListener);
			} else {
				ALOGI("%d: %s() replace listener (index %d )", __LINE__, __FUNCTION__,index);
				mListenerList.replaceValueFor(listenerName, mListener);
			}
	  #endif
		}
		  ALOGI("%d: %s() end", __LINE__, __FUNCTION__);
	#endif
		return NO_ERROR;
	  }
	status_t IpodService::setPcmListener(const sp<IPcmServiceNotify>& listener,String8 listenerName)
  {

  #if 1
	  sp<IPcmServiceNotify> mListener = listener;
      IInterface::asBinder(mListener)->linkToDeath(this);
	  //mListener->asBinder()->linkToDeath(this);
	  ALOGI("%d: %s() add listenerName %s %p", 
																	  __LINE__,
																	  __FUNCTION__,
																	  listenerName.string(),
																	   IInterface::asBinder(mListener).get()
																	 /* mListener->asBinder().get()*/
																	  );
	  {
		  Mutex::Autolock lock(mListenerLock);
		
        #if 1
		// delete(mPcmServiceNotify);
		 mPcmServiceNotify = listener;
		if(mListener!=NULL)
		  mListener->notify(0xF0002,0, 0,0);
		  ALOGI("%d: %s()   new listener mListenerList.size() %d", 
																									  __LINE__,
																									  __FUNCTION__,
																									(int)  mListenerList.size()
																									  );
        #else
		  if ( index < 0) {
			  ALOGI("%d: %s() add listener index %d", __LINE__, __FUNCTION__,index);
			  mListenerList.add(listenerName, mListener);
		  } else {
			  ALOGI("%d: %s() replace listener (index %d )", __LINE__, __FUNCTION__,index);
			  mListenerList.replaceValueFor(listenerName, mListener);
		  }
        #endif
	  }
	    ALOGI("%d: %s() end", __LINE__, __FUNCTION__);
	  #endif
	  return NO_ERROR;
	}

  status_t IpodService::setListener(const sp<IIapServiceNotify>& listener,String8 listenerName)
  {

  #if 1
	  sp<IIapServiceNotify> mListener = listener;
      IInterface::asBinder(mListener)->linkToDeath(this);
	 // mListener->asBinder()->linkToDeath(this);
	  ALOGI("%d: %s() add listenerName %s %p", 
																	  __LINE__,
																	  __FUNCTION__,
																	  listenerName.string(),
																	  IInterface::asBinder(mListener).get()
																	 /* mListener->asBinder().get()*/
																	  );
	  {
		  Mutex::Autolock lock(mListenerLock);
		
        #if 1
		 //delete(mIapServiceNotify);
		 mIapServiceNotify = listener;
		if(mListener!=NULL)
		  mListener->notify(0xF0002,0, 0,0);
		  ALOGI("%d: %s() new listener mListenerList.size() %d", 
																									  __LINE__,
																									  __FUNCTION__,
																									 (int) mListenerList.size()
																									  );
        #else
		  if ( index < 0) {
			  ALOGI("%d: %s() add listener index %d", __LINE__, __FUNCTION__,index);
			  mListenerList.add(listenerName, mListener);
		  } else {
			  ALOGI("%d: %s() replace listener (index %d )", __LINE__, __FUNCTION__,index);
			  mListenerList.replaceValueFor(listenerName, mListener);
		  }
        #endif
	  }
  
	 /* int i;
	  const char *cListenerName = listenerName.string();
	  int listenerNameLan = strlen(cListenerName);
  
	  IAPSVC_DBUG("listenerNameLan %d mEapProtocolList.size() %d",listenerNameLan,mEapProtocolList.size() );
  
	  if(IsEmptyReadySessionList() == false){ 
		  for(i = 0; i < (int)mEapProtocolList.size(); i++)
		  {
			  String8 protocolName = nIapSvc->mEapProtocolList.valueAt(i);
			   const char *cProtocolName = protocolName.string();
			   int protocolNameLen = strlen(cProtocolName);
			   IAPSVC_DBUG("protocolNameLen %d	%s",protocolNameLen,cProtocolName );
			   if(listenerNameLan == protocolNameLen){
				  if(strncmp(cListenerName, cProtocolName,listenerNameLan) == 0){
					  int keyProtocolIndex = mEapProtocolList.keyAt(i);
					  IAPSVC_DBUG("%d: %s() add listenerName %s protocol Index %d", 
																		  __LINE__,
																		  __FUNCTION__,
																		  listenerName.string(),
																		  keyProtocolIndex
																		  );
  
					  mIapCommandThread->sendCommandArg1(
																	  IAP_CMD_CHK_READYSESSION,
																	  keyProtocolIndex,
																	  true
																	  );
				  }
			  }
		  }
	  }
	  */
	  ALOGI("%d: %s() end", __LINE__, __FUNCTION__);
	  #endif
	  return NO_ERROR;
  }

  void IpodService::binderDied(const wp<IBinder>& who) 
  {
	  int i;
	  String8 diedName;
	  bool fFound = false;
	  ALOGE("binderDied() %p, tid %d, calling tid %d", who.unsafe_get(), gettid(),
			  IPCThreadState::self()->getCallingPid());
	  {
		  Mutex::Autolock lock(mListenerLock);
		  for(i = 0; i < (int)mListenerList.size();i++){
			  sp<IIapServiceNotify> listener = mListenerList.valueAt(i);
			  if(who == IInterface::asBinder(listener)/*listener->asBinder()*/){
				  diedName = mListenerList.keyAt(i);
				  ALOGE("	   died listener index	%d(%s)", 
																		  (int)i,
																		  diedName.string()
																		  );
				  fFound = true;
			  }
		  }
	  }
	  if(fFound== true){
		  int strLen = strlen(diedName.string());
		  removeListener(diedName,false);
		  if(strncmp("ipod-connect",diedName.string(),strLen) == 0){
			  ALOGI(" send intent to start iAP1 connect app");
			  //system("am broadcast -a com.telechips.android.iap1.recvstart");
		  }
	  }
  }

  
  status_t IpodService::removeListener(String8 listenerName)
  {
	  return removeListener(listenerName,true);
  }
  
  status_t IpodService::removeListener(String8 listenerName,bool notiFlag)
  {

	  ALOGI("%d: %s() remove listenerName %s", 
																		  __LINE__,
																		  __FUNCTION__,
																		  listenerName.string()
																		  );
	 {
		  Mutex::Autolock lock(mListenerLock);
		  ssize_t index = mListenerList.indexOfKey(listenerName);
		  if ( index >= 0) {
			  sp<IIapServiceNotify> listener = NULL;
			  listener = mListenerList.valueAt(index);
			  if(notiFlag){
			  	IInterface::asBinder(listener)->unlinkToDeath(this);
				//  listener->asBinder()->unlinkToDeath(this);
				  listener->notify(0xF0003,0, 0,0);
			  }
			  mListenerList.removeItem(listenerName);
		  }
	  }
		  
	  return NO_ERROR;
  }
  
  
  IpodService::~IpodService()
{ 
  }
};
