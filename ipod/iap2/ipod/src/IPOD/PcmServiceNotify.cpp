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
#include "PcmServiceNotify.h"
#include <unistd.h>
#include <utils/String8.h>
#include <cutils/properties.h>
#include <nativehelper/JNIHelp.h>
#include "iAP2_utils.h"
#include "android_runtime/AndroidRuntime.h"
#include <iPod_jni.h>

#include <utils/Log.h>



//int IIAPSVC_DEBUG	   =  -1;
//#define IIAPSVC_DBUG(msg...)	if (IIAPSVC_DEBUG == 1) { ALOGD( " " msg); }

namespace android {


PcmServiceNotify::PcmServiceNotify()
{
    ALOGI(" PcmServiceNotify ");
    serverDied = false;
/*
	{
        sp<IServiceManager> sm = defaultServiceManager();
        sp<IBinder> binder;
        do {
            binder = sm->getService(String16("IpodService"));
            if (binder != 0)
            	{
                break;
            	}
            ALOGE("IpodService not published, waiting... ");
            usleep(500000); // 0.5 s
        } while(true);

        sp<PcmServiceNotify> listenerApi= this;
        status_t ret = binder->linkToDeath(listenerApi);
        sp<IIpodService> service = interface_cast<IIpodService>(binder);
		 gIpodService = service;
		const char *  cListenerName="PcmServiceNotify";
		//gListenerCount++;
		//sprintf(cListenerName,"PcmServiceNotify%d",gListenerCount);
        service->setPcmListener(listenerApi,String8(cListenerName));
       
       // ALOGI("%s new service ret %d %s",__FUNCTION__,ret,miAP1ListenerName.string());
    }
	*/
}

PcmServiceNotify::~PcmServiceNotify()
{
  //  ALOGI(" ~JNITelechipsIap1Listener %s",miAP1ListenerName.string());
   // clearService();
}

void PcmServiceNotify::clearService(void)
{
    //miAP1ClientLock.lock();
   // ALOGI(" ~clearService  API this %p serverDied %d get(%p) %s",this,serverDied,giAP1Service.get(),miAP1ListenerName.string());
    //if(giAP1Service.get() != 0){
      //  status_t ret = giAP1Service->asBinder()->unlinkToDeath(this);
        //giAP1Service.clear();
        //ALOGI(" ~Check  ret %d  get(%p)",ret,giAP1Service.get());
   // }
    //miAP1ClientLock.unlock();
}

void PcmServiceNotify::notify(int msg, int ext1, int ext2,int ext3)
{
    //IAP1JNI_DBUG("JNITelechipsIap1Listener::notify");
    if((unsigned int)(msg & 0xF0000) != 0xF0000){
       // callByLibraryiAP1(msg,ext1,ext2,ext3);
    }else{
        switch(msg){
			case NOTIFY_CODE_IPOD_START_READY:
				
				LOGI("%s %d\n",__FUNCTION__,__LINE__);
				OnIPODStartReady();
				break;
			case NOTIFY_CODE_MEDIA_NOW_PLAYING_ARTWORK_DATA_FINISH:
				OnMeidaNowPlayingArtworkFinish();
				break;
			
		    case NOTIFY_CODE_MEDIA_NOW_PLAYING_ARTWORK_DATA_START:
				OnMeidaNowPlayingArtworkStart();
	        case NOTIFY_CODE_MEDIA_UPDATE_PLAYLIST_CONTENT_TRANSFER:
				OnMediaUpdatePlaylistContentTransfer();
				break;
		    case NOTIFY_CODE_CONTAINED_MEDIA_ITEM_FS_ID:
				OnContainedMediaItemFsId(ext1);
				break;
			case NOTIFY_CODE_MEDIA_UPDATE_PLAYLIST_IS_RADIO_STATION:
				OnMediaUpdatePlaylistIsRadioStation(ext1);
				break;
		    case NOTIFY_CODE_MEDIA_UPDATE_PLAYLIST_IS_FOLDER:
				OnMediaUpdatePlaylistIsFolder(ext1);
				break;
		    case NOTIFY_CODE_MEDIA_UPDATE_PLAYLIST_IS_GENIUSMIX:
				
				OnMediaUpdatePlaylistIsGeniusMix(ext1);
				break;
		    case NOTIFY_CODE_PLAY_BACK_QUEUELIST_AVAIL:
				OnPlayBackQueueListAvail(ext1);
				break;
			case NOTIFY_CODE_SET_ELAPSED_TIME_AVAILABLE:
				OnSetElapsedTimeAvailable(ext1);
				break;

			case NOTIFY_CODE_MEDIA_ITEM_IS_RESIDENT_ON_DEVICE:
				OnMediaItemIsResidentOnDevice(ext1);
				break;
			case NOTIFY_CODE_MEDIA_ITEM_IS_BANNED:				
				OnMediaItemIsBanned(ext1);
				break;
			case NOTIFY_CODE_MEDIA_ITEM_IS_LIKED:				
				OnMediaItemIsLiked(ext1);
				break;
			case NOTIFY_CODE_MEDIA_ITEM_IS_BEAN_SUPPORTED:
				OnMediaItemIsBeanSupported(ext1);
				break;
			case NOTIFY_CODE_MEDIA_ITEM_IS_LIKE_SUPPORTED:
				OnMediaItemIsLikeSupported(ext1);
				break;
			case NOTIFY_CODE_MEDIA_LIBRARY_RESET:
				//OnMediaLibraryReset();
				break;
			case NOTIFY_CODE_MEDIA_LIBRARY_UPDATE_PROGRESS:
				//OnMediaLibraryUpdateProgress(ext1);
				break;
			case NOTIFY_CODE_IS_HIDING_REMOTE_ITEMS:
				
			//	OnIsHidingRemoteItems(ext1);
				break;
			case NOTIFY_CODE_PLAY_ALL_SONGS_CAPABLE:
				//OnPlayAllSongsCapable(ext1);
				break;
			case NOTIFY_CODE_MEDIA_ITEM_ISPARTOF_COMPILATION:
				OnMediaItemIsPartOfCompilation(ext1);
				break;
		    case NOTIFY_CODE_PB_APPLE_MUSIC_RADIO_AD:
				OnPBAppleMusicRadioAd(ext1);
				break;
			case NOTIFY_CODE_MEDIA_PLAYBACK_QUEUE_CAPTER_INDEX:
				OnPlayBackQueueChapterIndex(ext1);
				break;
			case NOTIFY_CODE_MEDIA_PLAYBACK_QUEUE_COUNT:
				OnPlayBackQueueCount(ext1);
				break;
			case NOTIFY_CODE_MEDIA_PLAYBACK_QUEUE_INDEX:
				OnPlayBackQueueIndex(ext1);
				break;
			case NOTIFY_CODE_MEDIA_PLAYBACK_REPEAT_MODE:
				OnPlayBackRepeatMode(ext1);
				break;
			case NOTIFY_CODE_PLAY_BACK_QUEUELIST_CONTENT_TRANSFER:
				OnPlayBackQueueListContentTransfer();
				break;
			case NOTIFY_CODE_PLAY_BACK_QUEUELIST_TRANSFER_ID:
				OnPlayBackQueueListTransferID(ext1);
				break;
			case NOTIFY_CODE_MEDIA_ITEM_ART_WORK_FILE_TRANSFER_ID:
				OnMediaItemArtWorkFileTransferID(ext1);
				break;
			case NOTIFY_CODE_MEDIA_ITEM_MEDIA_RATING:
				OnMediaItemRating(ext1);
				break;
			case NOTIFY_CODE_MEDIA_ITEM_ALBUM_DISC_COUNT:
				OnMediaItemAlbumDiscCount(ext1);
				break;
			case NOTIFY_CODE_MEDIA_ITEM_ALBUM_DISC_NUMBER:
				OnMediaItemAlbumDiscNumber(ext1);
				break;
			case NOTIFY_CODE_MEDIA_ITEM_CHAPTER_COUNT:
				
				OnMediaItemChapterCount(ext1);
				break;
			case NOTIFY_CODE_MEDIA_ITEM_ALBUM_TRACK_COUNT:
				OnMediaItemAlbumTrackCount(ext1);
				break;
			case NOTIFY_CODE_MEDIA_ITEM_ALBUM_TRACK_NUMBER:
				OnMediaItemAlbumTrackNumber(ext1);
				break;
			case NOTIFY_CODE_MEDIA_LIBRARY_TYPE:
				OnMediaLibraryType(ext1);
				break;
			case NOTIFY_CODE_MEDIA_ITEM_MEDIA_TYPE:
				OnMediaItemMediaType(ext1);
				break;
			case NOTIFY_CODE_MEDIA_PLAYBACK_SHUFFLE_MODE:
				OnPlayBackShuffleMode(ext1);
				break;
			case NOTIFY_CODE_MEDIA_PLAYBACK_STATUS:
				OnPlayBackStatus( ext1);
				break;
			case NOTIFY_CODE_MEDIA_PLAYBACK_SPEED:
				OnPlayBackSpeed(ext1);
				break;
			case NOTIFY_CODE_MEDIA_PLAYBACK_POSITION:
				OnPlayBackPosition( ext1);
				break;
			case NOTIFY_CODE_DURATION:
				 OnDuration(ext1);
				break;
			case NOTIFY_CODE_SAMPLERATE: //audio sampelrate
			   OnUSBDeviceModeAudioInformation(ext1);
				break;
            case 0xF0001:
                ALOGI(" IapService died 222 ~~~~~~~~~~~~~~~~~~ ");
               // serverDied = true;
               // mIap1SvcReconnect = true;
               // clearService();
               // callByLibraryiAP1(msg,ext1,ext2,ext3);
                break;
            case 0xF0002:
                ALOGI(" Listener is registered tid %d",gettid());
                break;
            case 0xF0003:
                ALOGI(" Listener is removed tid %d callService",gettid());
               // clearService();
                break;
        }
    }
}


	void PcmServiceNotify::notifyRawByte(int msg,  int ext1, const unsigned char	*obj) 
	{
		switch(msg){
		case NOTIFY_CODE_MEDIA_PLAY_LIST_DELETE_UPDATE_PID:
			//OnMediaPlayListDeleteUpdatePID((uint8_t *)obj,ext1);
			break;
		case NOTIFY_CODE_MEDIA_ITEM_DELETE_UPDATE_PID:
			//OnMediaItemDeleteUpdatePID((uint8_t *)obj,ext1);
			break;
		case NOTIFY_CODE_MEDIA_ITEM_ALBUM_PERSISTENT_ID:
			OnMediaItemAlbumPID((uint8_t *)obj,ext1);
			break;
		case NOTIFY_CODE_MEDIA_ITEM_COMPOSER_PID:
			OnMediaItemComposerPID((uint8_t *)obj,ext1);
			break;
		case NOTIFY_CODE_MEDIA_ITEM_GENRE_PID:
			OnMediaItemGenrePID((uint8_t *)obj,ext1);
			break;
		case NOTIFY_CODE_MEDIA_ITEM_ALBUM_ARTIST_PID:
			OnMediaItemAlbumArtistPID((uint8_t *)obj,ext1);
			break;
		case NOTIFY_CODE_MEDIA_ITEM_ARTIST_PID:
			OnMediaItemArtistPID((uint8_t *)obj,ext1);
			break;
		case NOTIFY_CODE_MEDIA_ITEM_PERSISTENT_ID:
			OnMediaItemPID((uint8_t *)obj,ext1);
			break;
		case NOTIFY_CODE_MEDIA_LIBRARY_UPDATE_PARENT_PLAYLIST_PID:
			OnMediaLibraryUpdateParentPlaylistID((uint8_t *)obj,ext1);
			break;
		case NOTIFY_CODE_MEDIA_LIBRARY_UPDATE_PLAYLIST_PID:
			OnMediaLibraryUpdatePlaylistID((uint8_t *)obj,ext1);
			break;
		case NOTIFY_CODE_PB_APPLE_MUSIC_RADIO_STATION_PLAYLIST_ID:
			OnPBAppleMusicRadioStationMediaPlaylistID((uint8_t *)obj,ext1);
			break;
		case NOTIFY_CODE_MEDIA_PCM_DATA_RECEIVE:
			OnPcmDataReceive((uint8_t *)obj,ext1);
			break;
		case NOTIFY_ON_MEDIA_LIBRARY_INFOMATION:
			
		LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
			OnMediaLibraryInformation((uint8_t *)obj,ext1);
			
			break;
		case NOTIFY_ON_MEDIA_LIBRARY_UPDATE:
			OnMediaLibraryUpdate((uint8_t *)obj,ext1);
			break;
		case NOTIFY_ON_NOW_PLAYING_PLAYBACK_ATTRIBUTES:
			OnNowPlayingPlayBackAttributes((uint8_t *)obj,ext1);
			break;
		case NOTIFY_ON_NOW_PLAYING_MEDIA_ITEM_ATTRIBUTES:
			OnNowPlayingMediaItemAttributes((uint8_t *)obj,ext1);
			break;
			}
		
	  
	}

 void PcmServiceNotify::notifyIap2FileTransfer(int msg, int fileName_len, const unsigned char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libraryUID)
{
	// LOGI("xx1 %s %d msg=0x%06x\n",__FUNCTION__,__LINE__,msg);

  switch(msg)
  	{
  	  case MEDIA_ITEM_LIST_UPDATE_PLAYLIST_CONTENTS_FINISH:
	  	OnMediaItemListUpdatePlaylistContentsFinish(fileName_len,(char *)fileName,playListPid_len,playListPid,
										libraryUID_len,libraryUID);
	  	break;
      case MEDIA_ITEM_LIST_UPDATE_PLAYLIST_CONTENTS_START:
	  		OnMediaItemListUpdatePlaylistContentsStart(fileName_len,(char *)fileName,playListPid_len,playListPid,
										libraryUID_len,libraryUID);
	  	break;
  	  case MEDIA_LIBRARY_UPDATE_PLAYLIST_CONTENTS_START:
	  	
	//	LOGI("xx1 %s %d msg=0x%06x fileName_len=%d fileName=%s\n",__FUNCTION__,__LINE__,msg,fileName_len,fileName);
	  	OnMediaLibraryUpdatePlaylistContentsStart(fileName_len,(char *)fileName,playListPid_len,playListPid,
										libraryUID_len,libraryUID);
	  	break;
	  case MEDIA_LIBRARY_UPDATE_PLAYLIST_CONTENTS_FINISH:
	  //	LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);
	  	OnMediaLibraryUpdatePlaylistContentsFinish(fileName_len,(char *)fileName,playListPid_len,playListPid,
										libraryUID_len,libraryUID);
		// LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);
	  	break;
  	}
}

void PcmServiceNotify::notifyRawString(int msg,  int ext1, const unsigned char  *obj) 
{

	

	switch(msg){
    case NOTIFY_CODE_PB_APPLE_MUSIC_RADIO_STATION_NAME:
		OnPBAppleMusicRadioStationName((uint8_t	*)obj,ext1);
		break;
	case NOTIFY_CODE_PB_MEDIA_LIBRARY_UID:
		OnPBMediaLibraryUID((uint8_t	*)obj,ext1);
		break;
	case NOTIFY_CODE_MEDIA_PLAYBACK_BUNDLE_ID:
		onPlayBackBundleID((uint8_t	*)obj,ext1);
		break;
	case NOTIFY_CODE_MEDIA_LIBRARY_UPDATE_PLAYLIST_NAME:		
		OnMediaLibraryUpdatePlaylistName((uint8_t	*)obj,ext1);
		break;
    case NOTIFY_CODE_MEDIA_PLAYBACK_APP_NAME:
		OnPlayBackAppName((uint8_t	*)obj,ext1);
		break;
	case NOTIFY_CODE_MEDIA_LIBRARY_UID:
			OnMeidaLibraryUID((uint8_t	*)obj,ext1);
		break;	
	case NOTIFY_CODE_MEDIA_LIBRARY_UPDATE_REVISION:
		//OnMediaLibraryUpdateRevision((uint8_t	*)obj,ext1);
		break;
	case NOTIFY_CODE_MEDIA_LIBRARY_UPDATE_UID:
		OnMediaLibraryUpdateUID((uint8_t	*)obj,ext1);
		break;
	case NOTIFY_CODE_MEDIA_LIBRARY_NAME:
		OnMediaLibraryName((uint8_t	*)obj,ext1);
		break;
    case NOTIFY_CODE_MEDIA_TITLE:
		OnMediaItemTile((uint8_t	*)obj,ext1);
		break;
	case NOW_PLAYING_PB_QUEUE_CONTENTS_FINISH:
		OnNowPlayingPBQContentsFinish((uint8_t	*)obj,ext1);
		break;
	case MEDIA_ITEM_NOW_PLAYING_PB_QUEUE_CONTENTS_FINISH:
		OnMediaItemNowPlayingPBQContentsFinish((uint8_t	*)obj,ext1);
		break;
	case MEDIA_ITEM_NOW_PLAYING_PB_QUEUE_CONTENTS_START:
		OnMediaItemNowPlayingPBQContentsStart((uint8_t	*)obj,ext1);
		break;
	case NOW_PLAYING_PB_QUEUE_CONTENTS_START:
		OnNowPlayingPBQContentsStart((uint8_t	*)obj,ext1);
		break;
	case NOTIFY_CODE_MEDIA_ARTIST:
		OnMediaItemArtist((uint8_t	*)obj,ext1);
		break;
	case NOTIFY_CODE_MEDIA_ABLUM_TITLE:
		
		OnMediaItemAlumTitle((uint8_t	*)obj,ext1);
		break;
	case NOTIFY_CODE_MEDIA_COMPOSER:
		OnMediaItemGenreComposer((uint8_t	*)obj,ext1);
		break;
	case NOTIFY_CODE_MEDIA_GENRE:		
		OnMediaItemGenre((uint8_t	*)obj,ext1);
		break;
	case NOTIFY_CODE_MEDIA_ABLUM_ARTIST:
		OnMediaItemAlumArtist((uint8_t	*)obj,ext1);
		break;
	}
  
}


void PcmServiceNotify::notifyObj(int what,int arg1, int arg2,const Parcel *obj)
{
    //IAP1JNI_DBUG("JNITelechipsIap1Listener::notifyObj");
    if((unsigned int)(what & 0xF0000)  == 0xF0000){
        ALOGI(" IapService died ~~~~~~~~~~~~~~~~~~");
    }else if(((unsigned int)what&0x00100) == 0x00100){
		ALOGI("222 unsigned int)what&0x00100) == 0x00100");
    }else{
        JNIEnv *env = android::AndroidRuntime::getJNIEnv();
        obj->setDataPosition(0);
      //  jclass IpodPlayerClazz = (jclass)obj->readInt32();
       // jmethodID MethodRspID = (jmethodID)obj->readInt32();          
        jobject jParcel = createJavaParcelObject(env);
        Parcel* nativeParcel = parcelForJavaObject(env, jParcel);
        nativeParcel->appendFrom(obj,obj->dataPosition(),obj->dataAvail());
        nativeParcel->setDataPosition(0);
        
       // env->CallStaticVoidMethod(IpodPlayerClazz, MethodRspID, jParcel);                
        //env->DeleteLocalRef(jParcel);            
       // env->DeleteGlobalRef(IpodPlayerClazz);
    }
}

void PcmServiceNotify::binderDied(const wp<IBinder>& who)
{
    notify(0xF0001,0,0,0);
}
};
