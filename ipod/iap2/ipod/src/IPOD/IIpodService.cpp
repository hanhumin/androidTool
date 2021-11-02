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
#include "IIapServiceNotify.h"

#include <unistd.h>
#include <utils/String8.h>
#include <cutils/properties.h>
#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "IPODJni", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "IPODJni", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , "IPODJni", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "IPODJni", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , "IPODJni", __VA_ARGS__)



//int IIAPSVC_DEBUG	   =  -1;
//#define IIAPSVC_DBUG(msg...)	if (IIAPSVC_DEBUG == 1) { ALOGD( " " msg); }

namespace android {

enum {
    IAP_CMDCTRL_INIT = IBinder::FIRST_CALL_TRANSACTION,

    IAP_CMDCTRL_PLAY,
    IAP_CMDCTRL_PLAY_IN_BRWSLIST,
    IAP_CMDCTRL_PAUSE,
    IAP_CMDCTRL_NEXTTRACK,
    IAP_CMDCTRL_PREVIOUSTRACK,
    IAP_CMDCTRL_FF,
    IAP_CMDCTRL_REW,

    IAP_CMDCTRL_GETCURRENTTRACKINDEX,
    IAP_CMDCTRL_GETTOTALPLAYTRACKS,
    IAP_CMDCTRL_GETTITLE,
    IAP_CMDCTRL_GETARTIST,
    IAP_CMDCTRL_GETALBUM,
    IAP_CMDCTRL_GETGENRE,
    IAP_CMDCTRL_GETALBUMART,
    IAP_CMDCTRL_INFORM_SAMPLINGFREQ_CHECKED,

    IAP_CMDCTRL_GET_DEVICEDISPLAYINFO,
    IAP_CMDCTRL_SET_DEVICEDISPLAYIMAGE,



    IAP_CMDCTRL_START_HID,
    IAP_CMDCTRL_STOP_HID,
    IAP_CMDCTRL_HID0,
    IAP_CMDCTRL_RANDOM,
    IAP_CMDCTRL_REPEAT,
    IAP_CMDCTRL_PLAY_2,
    IAP_CMDCTRL_PAUSE_2,
    IAP_CMDCTRL_PLAY_PAUSE,
    IAP_CMDCTRL_CARLIFE_RECORD,
    IAP_CMDCTRL_PLAY_POSITION,
    IAP_CMDCTRL_PLAY_QUEUE_INDEX,
    IAP_CMDCTRL_PQ_LIST_CT_START_INDEX,
    IAP_CMDCTRL_PLAY_NEXT_TRACK,
    IAP_CMDCTRL_PLAY_PROV_TRACK,
    IAP_CMDCTRL_STOP_MEDIA_LIBRARY_INFORMATION,
    IAP_CMDCTRL_START_MEDIA_LIBRARY_INFORMATION,
    IAP_CMDCTRL_START_MEDIA_LIBRARY_UPDATES,
    IAP_CMDCTRL_STOP_MEDIA_LIBRARY_UPDATES,
    IAP_CMDCTRL_PLAY_MEDIA_LIBRARY_CURRENT_SELLECTION,
    IAP_CMDCTRL_PLAY_MEDIA_LIBRARY_COLLECTION,    
    IAP_CMDCTRL_PLAY_MEDIA_LIBRARY_SPECIAL,
    IAP_CMDCTRL_PLAY_MEDIA_LIBRARY_ITEMS,
    IAP_CMDCTRL_START_NOW_PLAYING_UPDATES,
    IAP_CMDCTRL_STOP_NOW_PLAYING_UPDATES,
    IAP_CMDCTRL_STOP_USB_DEVICE_MODE_AUDIO,
    IAP_CMDCTRL_SET_LISTENER,
     IAP_CMDCTRL_SET_PCM_FILE_LISTENER,
    IAP_CMDCTRL_SET_PCM_LISTENER,
    IAP_CMDCTRL_REMOVE_LISTENER,
    IAP_CMDCTRL_GET_DR_STATUS,
    IAP_CMDCTRL_GET_PLAY_STATUS_INFO,
    IAP_CMDCTRL_TOGGLE_REPEAT,
    IAP_CMDCTRL_TOGGLE_SHUFFLE,
    IAP_CMDCTRL_GET_TRACKINFOS,
    IAP_CMDCTRL_GET_TRACKMETAS,
    IAP_CMDCTRL_DISCONNECTED,
    IAP_CMDCTRL_SETPOSITION,
    IAP_CMDCTRL_SETCURRENTPLAYINGTRACK,
    IAP_CMDCTRL_SETREPEAT,
    IAP_CMDCTRL_GETREPEAT,
    IAP_CMDCTRL_SETSHUFFLE,
    IAP_CMDCTRL_GETSHUFFLE,
    IAP_CMDCTRL_GETINDEXEDPLAYINGINFO,
    IAP_CMDCTRL_GETIPODMODELNAME,
    IAP_CMDCTRL_GETPROTOCOLVERSION,
    IAP_CMDCTRL_GETIPODSERIALNUMBER,
    IAP_CMDCTRL_GETIPODSOFTWAREVERSION,
    IAP_CMDCTRL_GETIPODNAME,
    IAP_CMDCTRL_ENDFFREW,
    IAP_CMDCTRL_ACCESSORYDATATRANSFER,
    IAP_CMDCTRL_APPLICATIONLAUNCH,
    IAP_CMDCTRL_MUTECONTROL,
    IAP_CMDCTRL_MSGFLUSH,
    IAP_CMDCTRL_SENDPACKEDLINGOCMD,
    IAP_CMDCTRL_EXTRACOMMAND,
    IAP_CMDCTRL_WRAP_COMMAND,

    IAP_CMDCTRL_SYNC_COMMAND,
    IAP_CMDCTRL_ASYNC_COMMAND,
    IAP_CMDCTRL_START_IPOD,
    IAP_CMDCTRL_STOP_IPOD,
    IAP_CMDCTRL_MAX
	
};


class BpIpodService : public BpInterface<IIpodService>
{
public:
    BpIpodService(const sp<IBinder>& impl)
        : BpInterface<IIpodService>(impl)
    {
       // InitDebug();
    }
   

   virtual void stopUSBDeviceModeAudio()
  	{
  	 Parcel data, reply;
	  remote()->transact(IAP_CMDCTRL_STOP_USB_DEVICE_MODE_AUDIO, data, &reply);
  	}

  virtual void stopIpod()
  	{
  	  Parcel data, reply;
	  remote()->transact(IAP_CMDCTRL_STOP_IPOD, data, &reply);
  	}
  
  virtual void startIpod()
  	{
  	  Parcel data, reply;
	  remote()->transact(IAP_CMDCTRL_START_IPOD, data, &reply);
  	}


  
  	
  virtual void StopMediaLibraryInformation()
  {
	  Parcel data, reply;
	   remote()->transact(IAP_CMDCTRL_STOP_MEDIA_LIBRARY_INFORMATION, data, &reply);
  }
  virtual void StartMediaLibraryInformation()
  {
  
	  Parcel data, reply;
	  
	  LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	   remote()->transact(IAP_CMDCTRL_START_MEDIA_LIBRARY_INFORMATION, data, &reply);
  }


  
	virtual void PlayMediaLibraryItems(uint8_t * itemsPids,int sizePidsByte,int itemsStartIndex,uint8_t * mediaLibraryUid,int sizeMediaLibraryUidBytes)
	   {
		Parcel data, reply;
		data.writeInt32(sizePidsByte);
		  data.write(itemsPids,sizePidsByte);
		   data.writeInt32(itemsStartIndex);
		   data.writeInt32(sizeMediaLibraryUidBytes);
		  data.write(mediaLibraryUid,sizeMediaLibraryUidBytes);
  //	  LOGI("file:%s line:%d\n",__FILE__,__LINE__);
		 remote()->transact(IAP_CMDCTRL_PLAY_MEDIA_LIBRARY_ITEMS, data, &reply);
	   }


virtual void PlayMediaLibrarySpecial(int mediaLibraryUidSize,uint8_t *bufMediaLibraryUId,bool isPlayAll,
	int startMediaItemPidSize,uint8_t *bufMediaItemPid)
{
  
	Parcel data, reply;
	data.writeInt32(mediaLibraryUidSize);	
	data.write(bufMediaLibraryUId,mediaLibraryUidSize);
	data.writeBool(isPlayAll);
	data.writeInt32(startMediaItemPidSize);
	if(startMediaItemPidSize > 0)		
	  data.write(bufMediaItemPid,startMediaItemPidSize);
	remote()->transact(IAP_CMDCTRL_PLAY_MEDIA_LIBRARY_SPECIAL, data, &reply);
}

virtual void PlayMediaLibraryCollection(int pid_size,uint8_t * bufPid,int type,int startIndex,int mediaLibraryUidSize,uint8_t *bufMediaLibraryUId,
	int startMediaItemPidSize,uint8_t *bufMediaItemPid)
{
  
	Parcel data, reply;
	data.writeInt32(pid_size);
  	data.write(bufPid,pid_size);
	data.writeInt32(type);
	data.writeInt32(startIndex);
	data.writeInt32(mediaLibraryUidSize);	
	data.write(bufMediaLibraryUId,mediaLibraryUidSize);
	
	data.writeInt32(startMediaItemPidSize);
	if(startMediaItemPidSize > 0)		
  	  data.write(bufMediaItemPid,startMediaItemPidSize);
 	remote()->transact(IAP_CMDCTRL_PLAY_MEDIA_LIBRARY_COLLECTION, data, &reply);
}


virtual void PlayMediaLibraryCurrentSelection(uint8_t * mediaLibraryUid, int sizeUid)
{
	Parcel data, reply;
	data.writeInt32(sizeUid);
  	data.write(mediaLibraryUid,sizeUid);
  
//	  LOGI("file:%s line:%d\n",__FILE__,__LINE__);
 	remote()->transact(IAP_CMDCTRL_PLAY_MEDIA_LIBRARY_CURRENT_SELLECTION, data, &reply);
}  
virtual void StopMediaLibraryUpdates(uint8_t * mediaLibraryUid, int sizeUid)
   {
	Parcel data, reply;
	data.writeInt32(sizeUid);
	  data.write(mediaLibraryUid,sizeUid);
	  
//	  LOGI("file:%s line:%d\n",__FILE__,__LINE__);
	 remote()->transact(IAP_CMDCTRL_STOP_MEDIA_LIBRARY_UPDATES, data, &reply);
		   }

  virtual void StartMediaLibraryUpdates(uint8_t * mediaLibraryUid, int sizeUid,
											uint8_t *lastKnownMediaLibraryRevision,int sizeRevision)
  	 {
	  Parcel data, reply;
	  data.writeInt32(sizeUid);
        data.write(mediaLibraryUid,sizeUid);
		 data.writeInt32(sizeRevision);
        data.write(lastKnownMediaLibraryRevision,sizeRevision);
//		LOGI("file:%s line:%d\n",__FILE__,__LINE__);
	   remote()->transact(IAP_CMDCTRL_START_MEDIA_LIBRARY_UPDATES, data, &reply);
	 }
  virtual void StartNowPlayingUpdates()
	 {
	  Parcel data, reply;
	   LOGI("%s :%s line=%d\n",__FILE__,__FUNCTION__,__LINE__);
	   remote()->transact(IAP_CMDCTRL_START_NOW_PLAYING_UPDATES, data, &reply);
	 }

   virtual void StopNowPlayingUpdates()
	 {
	  Parcel data, reply;
	   remote()->transact(IAP_CMDCTRL_STOP_NOW_PLAYING_UPDATES, data, &reply);
	 }
  

  
  virtual void stopHid()
	 {
	  Parcel data, reply;
	   remote()->transact(IAP_CMDCTRL_STOP_HID, data, &reply);
	 }

   virtual void playProvTrack()
	 {
		Parcel data, reply;
	   remote()->transact(IAP_CMDCTRL_PLAY_PROV_TRACK, data, &reply);
	 }
	
  

  virtual void playNextTrack()
	 {
		Parcel data, reply;
	   remote()->transact(IAP_CMDCTRL_PLAY_NEXT_TRACK, data, &reply);
	 }
  
virtual void send0Hid()
{
  
   Parcel data, reply;
				   
  remote()->transact(IAP_CMDCTRL_HID0, data, &reply);
}
virtual void random()
     {
					
				   Parcel data, reply;
				   
				  remote()->transact(IAP_CMDCTRL_RANDOM, data, &reply);
		}
	virtual void repeat()
	{
				
			   Parcel data, reply;
			   
			  remote()->transact(IAP_CMDCTRL_REPEAT, data, &reply);
	}

   virtual void play()
   	{
   		
   	   Parcel data, reply;
	   
	  remote()->transact(IAP_CMDCTRL_PLAY_2, data, &reply);
   	}

   virtual void pause()
	 {
	
		Parcel data, reply;
		
	   remote()->transact(IAP_CMDCTRL_PAUSE_2, data, &reply);
	 }

   virtual void setPlayPosition(int position)
   	{
   	    Parcel data, reply;
	   data.writeInt32(position);
	  remote()->transact(IAP_CMDCTRL_PLAY_POSITION, data, &reply);
   	}

   
   	
   virtual void setPQListContentTransferStartIndex(int index)
   	{
   	    Parcel data, reply;
	   data.writeInt32(index);
	  remote()->transact(IAP_CMDCTRL_PQ_LIST_CT_START_INDEX, data, &reply);
   	}
   virtual void setPlaybackQueueIndex(int index)
   	{
   	    Parcel data, reply;
	   data.writeInt32(index);
	  remote()->transact(IAP_CMDCTRL_PLAY_QUEUE_INDEX, data, &reply);
   	}
   

   
   	virtual void notifyMicRecord(int startStop)
   	{
   	   Parcel data, reply;
	   data.writeInt32(startStop);
	  remote()->transact(IAP_CMDCTRL_CARLIFE_RECORD, data, &reply);
   	}

  
   	
   virtual void playPause(int mode)
   	{
   	   Parcel data, reply;
	   data.writeInt32(mode);
	  remote()->transact(IAP_CMDCTRL_PLAY_PAUSE, data, &reply);
   	}

   
   virtual status_t setPcmFileListener(const sp<IPcmServiceNotify>& listener,String8 listenerName)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IIpodService::getInterfaceDescriptor());
      //  data.writeStrongBinder(listener->asBinder());
       data.writeStrongBinder(IInterface::asBinder(listener));
      
        data.writeString8(listenerName);
        remote()->transact(IAP_CMDCTRL_SET_PCM_FILE_LISTENER, data, &reply);
        return reply.readInt32();    
    }

   virtual status_t setPcmListener(const sp<IPcmServiceNotify>& listener,String8 listenerName)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IIpodService::getInterfaceDescriptor());
      //  data.writeStrongBinder(listener->asBinder());
       data.writeStrongBinder(IInterface::asBinder(listener));
      
        data.writeString8(listenerName);
        remote()->transact(IAP_CMDCTRL_SET_PCM_LISTENER, data, &reply);
        return reply.readInt32();    
    }
   
    virtual status_t setListener(const sp<IIapServiceNotify>& listener,String8 listenerName)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IIpodService::getInterfaceDescriptor());
       // data.writeStrongBinder(listener->asBinder());
		data.writeStrongBinder(IInterface::asBinder(listener));
        data.writeString8(listenerName);
        remote()->transact(IAP_CMDCTRL_SET_LISTENER, data, &reply);
        return reply.readInt32();    
    }
    

virtual status_t removeListener(String8 listenerName)
  {
	  Parcel data, reply;
	  data.writeInterfaceToken(IIpodService::getInterfaceDescriptor());
	  data.writeString8(listenerName);
	  remote()->transact(IAP_CMDCTRL_REMOVE_LISTENER, data, &reply);
	  return reply.readInt32();    
  }

};


IMPLEMENT_META_INTERFACE(IpodService, "android.iap.IIpodService");


status_t BnIpodService::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
	int mode;
	int position;
	int index;

	switch(code) {
	 case IAP_CMDCTRL_SET_PCM_FILE_LISTENER:
	 	{
            CHECK_INTERFACE(IIpodService, data, reply);
            sp<IPcmServiceNotify> listener =
                interface_cast<IPcmServiceNotify>(data.readStrongBinder());
            String8 listenerName(data.readString8());
            reply->writeInt32(setPcmFileListener(listener,listenerName));
            return NO_ERROR;
   
       }
	 	break;
	 case IAP_CMDCTRL_SET_PCM_LISTENER: {
            CHECK_INTERFACE(IIpodService, data, reply);
            sp<IPcmServiceNotify> listener =
                interface_cast<IPcmServiceNotify>(data.readStrongBinder());
            String8 listenerName(data.readString8());
            reply->writeInt32(setPcmListener(listener,listenerName));
            return NO_ERROR;
   
       }
	 break;

	 case IAP_CMDCTRL_SET_LISTENER: {
            CHECK_INTERFACE(IIpodService, data, reply);
            sp<IIapServiceNotify> listener =
                interface_cast<IIapServiceNotify>(data.readStrongBinder());
            String8 listenerName(data.readString8());
            reply->writeInt32(setListener(listener,listenerName));
            return NO_ERROR;
   
       }
	 break;
	 case IAP_CMDCTRL_PLAY_PROV_TRACK:
	 	playProvTrack();
	 	break;
	 case IAP_CMDCTRL_STOP_HID:
	 	stopHid();
	 	break;
	 case IAP_CMDCTRL_PLAY_NEXT_TRACK:
	 	playNextTrack();
		break;
	 case IAP_CMDCTRL_STOP_USB_DEVICE_MODE_AUDIO:
	 	stopUSBDeviceModeAudio();
	 	break;
	 case IAP_CMDCTRL_STOP_NOW_PLAYING_UPDATES:
	 	StopNowPlayingUpdates();
	 	break;
	 case IAP_CMDCTRL_STOP_MEDIA_LIBRARY_INFORMATION:
	 	StopMediaLibraryInformation();
		break;
	 case IAP_CMDCTRL_START_MEDIA_LIBRARY_INFORMATION:
	 	
		LOGI("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	 	StartMediaLibraryInformation();
	 	break;
	case IAP_CMDCTRL_PLAY_MEDIA_LIBRARY_ITEMS:
	
		{
		   int sizePidsByte;
		   uint8_t * itemsPids;
		   int itemsStartIndex;
		   int sizeMediaLibraryUidBytes;
		    uint8_t * mediaLibraryUid;
			  sizePidsByte = data.readInt32();
			 itemsPids = (uint8_t *)malloc(sizePidsByte);
			 data.read(itemsPids,sizePidsByte) ;
			 itemsStartIndex = data.readInt32();
			 sizeMediaLibraryUidBytes = data.readInt32();
			  mediaLibraryUid = (uint8_t *)malloc(sizeMediaLibraryUidBytes);
			 data.read(mediaLibraryUid,sizeMediaLibraryUidBytes) ;
			 PlayMediaLibraryItems(itemsPids,sizePidsByte,itemsStartIndex,mediaLibraryUid,sizeMediaLibraryUidBytes);
			 free(mediaLibraryUid);
			 free(itemsPids);
		   
		}
		break;
	case IAP_CMDCTRL_PLAY_MEDIA_LIBRARY_COLLECTION:
		{

         int pidSize,type,startIndex,mediaLibraryUidSize,startMediaItemPidSize;
		 uint8_t * pPid = NULL;
		 uint8_t * pMediaLibraryUid = NULL;
		 uint8_t * pStartMediaItemPid = NULL;

		 pidSize = data.readInt32();
		 pPid = (uint8_t *)malloc(pidSize);
		 data.read(pPid,pidSize);
		 type = data.readInt32();
		 startIndex = data.readInt32();
		 mediaLibraryUidSize = data.readInt32();
		 pMediaLibraryUid = (uint8_t *)malloc(mediaLibraryUidSize);
		 data.read(pMediaLibraryUid,mediaLibraryUidSize);
		 startMediaItemPidSize = data.readInt32();
		 if(startMediaItemPidSize > 0)
		 {		   
		   pStartMediaItemPid = (uint8_t *)malloc(startMediaItemPidSize);
		   data.read(pStartMediaItemPid,startMediaItemPidSize);
		 }
		 PlayMediaLibraryCollection(pidSize,pPid,type,startIndex,
		 	mediaLibraryUidSize,pMediaLibraryUid,startMediaItemPidSize,pStartMediaItemPid);

		 if(startMediaItemPidSize > 0)
		 	free(pStartMediaItemPid);
		 free(pMediaLibraryUid);
		 free(pPid);
				 
     }
		break;
	case IAP_CMDCTRL_PLAY_MEDIA_LIBRARY_SPECIAL:
		{

         int mediaLibraryUidSize,startMediaItemPidSize;
		 uint8_t * pMediaLibraryUid = NULL;
		 uint8_t * pStartMediaItemPid = NULL;

		 mediaLibraryUidSize = data.readInt32();
		 pMediaLibraryUid = (uint8_t *)malloc(mediaLibraryUidSize);
		 data.read(pMediaLibraryUid,mediaLibraryUidSize);
		 bool isPlayAll = data.readBool();
		 startMediaItemPidSize = data.readInt32();
		 if(startMediaItemPidSize > 0)
		 {		   
		   pStartMediaItemPid = (uint8_t *)malloc(startMediaItemPidSize);
		   data.read(pStartMediaItemPid,startMediaItemPidSize);
		 }
		 PlayMediaLibrarySpecial(mediaLibraryUidSize,pMediaLibraryUid,isPlayAll,startMediaItemPidSize,pStartMediaItemPid);

		 if(startMediaItemPidSize > 0)
		 	free(pStartMediaItemPid);
		 free(pMediaLibraryUid);
				 
     }
		break;
	case IAP_CMDCTRL_PLAY_MEDIA_LIBRARY_CURRENT_SELLECTION:
		{
			 unsigned char *pUid = NULL;
			 unsigned char *pRevision = NULL;
			 
			// LOGI("file:%s line:%d\n",__FILE__,__LINE__);
            int uidSize = data.readInt32();
            if(uidSize>0){
			pUid = (unsigned char *)malloc( uidSize);
			int readUidState = data.read(pUid,uidSize) ;
			
			//LOGI("file:%s line:%d\n",__FILE__,__LINE__);
			if(readUidState!=0)
			 {
				free(pUid);	
				
			//	LOGI("file:%s line:%d\n",__FILE__,__LINE__);
			     break;				
			}
            }
			else 
				break;
            
		//	LOGI("file:%s line:%d\n",__FILE__,__LINE__);
			
        
            	
			
			//LOGI("file:%s line:%d\n",__FILE__,__LINE__);
	 	     PlayMediaLibraryCurrentSelection(pUid,uidSize);
			 
			// LOGI("file:%s line:%d\n",__FILE__,__LINE__);
				 free(pUid);
				
				 
     }
	case IAP_CMDCTRL_STOP_MEDIA_LIBRARY_UPDATES:
		{
			 unsigned char *pUid = NULL;
			 unsigned char *pRevision = NULL;
			 
			// LOGI("file:%s line:%d\n",__FILE__,__LINE__);
            int uidSize = data.readInt32();
            if(uidSize>0){
			pUid = (unsigned char *)malloc( uidSize);
			int readUidState = data.read(pUid,uidSize) ;
			
			//LOGI("file:%s line:%d\n",__FILE__,__LINE__);
			if(readUidState!=0)
			 {
				free(pUid);	
				
			//	LOGI("file:%s line:%d\n",__FILE__,__LINE__);
			     break;				
			}
            }
			else 
				break;
            
		//	LOGI("file:%s line:%d\n",__FILE__,__LINE__);
			
        
            	
			
			//LOGI("file:%s line:%d\n",__FILE__,__LINE__);
	 	     StopMediaLibraryUpdates(pUid,uidSize);
			 
			// LOGI("file:%s line:%d\n",__FILE__,__LINE__);
				 free(pUid);
				
				 
     }
	break;
	case IAP_CMDCTRL_START_MEDIA_LIBRARY_UPDATES:
		{
			 unsigned char *pUid = NULL;
			 unsigned char *pRevision = NULL;
			 
			// LOGI("file:%s line:%d\n",__FILE__,__LINE__);
            int uidSize = data.readInt32();
            if(uidSize>0){
			pUid = (unsigned char *)malloc( uidSize);
			int readUidState = data.read(pUid,uidSize) ;
			
			//LOGI("file:%s line:%d\n",__FILE__,__LINE__);
			if(readUidState!=0)
			 {
				free(pUid);	
				
			//	LOGI("file:%s line:%d\n",__FILE__,__LINE__);
			     break;				
			}
            }
			else 
				break;
            
		//	LOGI("file:%s line:%d\n",__FILE__,__LINE__);
			 int revisionSize = data.readInt32();
            if(revisionSize>0){
			pRevision = (unsigned char *)malloc( revisionSize);
			int readrevisionState = data.read(pRevision,revisionSize) ;
			if(readrevisionState!=0)
				{
				free(pUid);
				free(pRevision);
				
				//LOGI("file:%s line:%d\n",__FILE__,__LINE__);
			  break;
				}
				
            }
			else 
				{
				//free(pUid);
				
				//LOGI("file:%s line:%d\n",__FILE__,__LINE__);
				//free(pRevision)
				//break;
				}
          
            	
			
			//LOGI("file:%s line:%d\n",__FILE__,__LINE__);
	 	     StartMediaLibraryUpdates(pUid,uidSize,pRevision,revisionSize);
			 
			// LOGI("file:%s line:%d\n",__FILE__,__LINE__);
				 free(pUid);
				free(pRevision);
				 
     }
	 	break;
	
	 case IAP_CMDCTRL_START_NOW_PLAYING_UPDATES:
	 	
		LOGI("%s :%s line=%d\n",__FILE__,__FUNCTION__,__LINE__);
	 	StartNowPlayingUpdates();
	 	break;
		//
	 case IAP_CMDCTRL_STOP_IPOD:
	//	 ALOGD("%s %d startIpod 3333",__FUNCTION__,__LINE__);
	 	stopIpod();
		//  ALOGD("%s %d startIpod 4444",__FUNCTION__,__LINE__);
	 	break;
		
	 case IAP_CMDCTRL_START_IPOD:
	//	 ALOGD("%s %d startIpod 3333",__FUNCTION__,__LINE__);
	 	startIpod();
		//  ALOGD("%s %d startIpod 4444",__FUNCTION__,__LINE__);
	 	break;
	case IAP_CMDCTRL_PQ_LIST_CT_START_INDEX:
		
	 	  index = data.readInt32();
		setPQListContentTransferStartIndex(index);
		break;
	case IAP_CMDCTRL_PLAY_QUEUE_INDEX:		
	 	  index = data.readInt32();
		setPlaybackQueueIndex(index);
		//iAP2StartPlayPosition( position);
		break;
	 case IAP_CMDCTRL_PLAY_POSITION:
	 	 position = data.readInt32();
		setPlayPosition(position);
		//iAP2StartPlayPosition( position);
	 	break;
	 case IAP_CMDCTRL_CARLIFE_RECORD:
	 	
	 	 mode = data.readInt32();
	
	 	notifyMicRecord(mode);
	 	break;
	 case IAP_CMDCTRL_PLAY_PAUSE:
	 	 mode = data.readInt32();
	 	playPause(mode);
	 	break;
	case IAP_CMDCTRL_HID0:
		send0Hid();
		break;
	case IAP_CMDCTRL_RANDOM:
		random();
		break;
	case IAP_CMDCTRL_REPEAT:			  
			  repeat();
		 break;
		case IAP_CMDCTRL_PLAY_2:			  
			  play();
		 break;
		case IAP_CMDCTRL_PAUSE_2:			  
			  pause();
		break;

		

	
	}
	 return NO_ERROR;
}

};
