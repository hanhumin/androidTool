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

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

#include "IPcmServiceNotify.h"

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "IPODJni", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "IPODJni", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , "IPODJni", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "IPODJni", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , "IPODJni", __VA_ARGS__)

namespace android {

enum {
    NOTIFY = IBinder::FIRST_CALL_TRANSACTION,
    NOTIFY_OBJ,
    NOTIFY_RAWSTR,
    NOTIFY_RAWBYTE,
    NOTIFY_IAP2_FILE_TRANSFER
};

class BpPcmServiceNotify: public BpInterface<IPcmServiceNotify>
{
public:
    BpPcmServiceNotify(const sp<IBinder>& impl)
        : BpInterface<IPcmServiceNotify>(impl)
    {
    }

    virtual void notify(int msg, int ext1, int ext2,int ext3)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IPcmServiceNotify::getInterfaceDescriptor());
        data.writeInt32(msg);
        data.writeInt32(ext1);
        data.writeInt32(ext2);
        data.writeInt32(ext3);  
        remote()->transact(NOTIFY, data, &reply, IBinder::FLAG_ONEWAY);


    }

    virtual void notifyRawByte(int msg, int ext1,  const unsigned char  *obj)
    {
      
        Parcel data, reply;
        data.writeInterfaceToken(IPcmServiceNotify::getInterfaceDescriptor());
        data.writeInt32(msg);
        data.writeInt32(ext1);
        data.write(obj,ext1);
        remote()->transact(NOTIFY_RAWBYTE, data, &reply, IBinder::FLAG_ONEWAY);
    }
	
	virtual void notifyIap2FileTransfer(int msg, int fileName_len, const unsigned char * fileName,
									int playListPid_len,  const unsigned char  *playListPid,
									int libraryUID_len,const unsigned char  *libraryUID)
    {
   //  LOGI("xx1 %s %d fileName_len=%d msg=0x%05x\n",__FUNCTION__,__LINE__,fileName_len,msg);
	 // LOGI("xx1 %s %d fileName=%s \n",__FUNCTION__,__LINE__,fileName);
     // LOGI("xx1 %s %d playListPid_len=%d libraryUID_len=%d\n",__FUNCTION__,__LINE__,playListPid_len,libraryUID_len);
	 unsigned char * pf = (unsigned char *)malloc(fileName_len+1);
	 memset(pf,0,fileName_len+1);
	  memcpy(pf,fileName,fileName_len);
        Parcel data, reply;
        data.writeInterfaceToken(IPcmServiceNotify::getInterfaceDescriptor());
        data.writeInt32(msg);
        data.writeInt32(fileName_len+1);
        data.write(pf,fileName_len+1);
		data.writeInt32(playListPid_len);
        data.write(playListPid,playListPid_len);
		data.writeInt32(libraryUID_len);
        data.write(libraryUID,libraryUID_len);
       // if (obj && obj->dataSize() > 0) {
          //  data.appendFrom(const_cast<Parcel *>(obj), 0, obj->dataSize());
        //}
       // LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);
        remote()->transact(NOTIFY_IAP2_FILE_TRANSFER, data, &reply, IBinder::FLAG_ONEWAY);
		// LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);
		free(pf);
		// LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);
    }
	
	virtual void notifyRawString(int msg, int ext1,  const unsigned char  *obj)
    {
      
        Parcel data, reply;
        data.writeInterfaceToken(IPcmServiceNotify::getInterfaceDescriptor());
        data.writeInt32(msg);
        data.writeInt32(ext1);
        data.write(obj,ext1);
        remote()->transact(NOTIFY_RAWSTR, data, &reply, IBinder::FLAG_ONEWAY);
    }
    virtual void notifyObj(int msg, int ext1, int ext2, const Parcel *obj)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IPcmServiceNotify::getInterfaceDescriptor());
        data.writeInt32(msg);
        data.writeInt32(ext1);
        data.writeInt32(ext2);
        if (obj && obj->dataSize() > 0) {
            data.appendFrom(const_cast<Parcel *>(obj), 0, obj->dataSize());
        }
        remote()->transact(NOTIFY_OBJ, data, &reply, IBinder::FLAG_ONEWAY);
    }
};

IMPLEMENT_META_INTERFACE(PcmServiceNotify, "android.iap.IPcmServiceNotifyy");

// ----------------------------------------------------------------------

status_t BnPcmServiceNotify::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch(code) {
        case NOTIFY: {
            CHECK_INTERFACE(IPcmServiceNotify, data, reply);
            int msg = data.readInt32();
			
            int ext1 = data.readInt32();
            int ext2 = data.readInt32();
            int ext3 = data.readInt32();
            notify(msg, ext1, ext2,ext3);
            return NO_ERROR;
        } break;
		case NOTIFY_RAWBYTE:
			{
		  	CHECK_INTERFACE(IPcmServiceNotify, data, reply);
			int msg = data.readInt32();
            int ext1 = data.readInt32();
            if(ext1>0){
			unsigned char *pData = (unsigned char *)malloc( ext1);
                    if(data.read(pData,ext1) == 0){    
            notifyRawByte(msg, ext1, pData);
			free(pData);
			}
            	}
			return NO_ERROR;
			}
			break;

		case NOTIFY_IAP2_FILE_TRANSFER:
			{
				unsigned char *pLibraryUID = NULL;
				unsigned char *pPlayListId = NULL;
				unsigned char *pFileName = NULL;
				//LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);
			  CHECK_INTERFACE(IPcmServiceNotify, data, reply);			  
			  int msg = data.readInt32();
			  int fileNameLen = data.readInt32();
			  LOGI("xx1 %s %d fileNameLen=%d\n",__FUNCTION__,__LINE__,fileNameLen);

			  if(fileNameLen > 0)
			  	{
			   		pFileName = (unsigned char *)malloc( fileNameLen);
			  		data.read(pFileName,fileNameLen);
					
					//LOGI("xx1 %s %d pFileName=%s\n",__FUNCTION__,__LINE__,pFileName);
			  	}
			  else 
			  	{
			  	// LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);
			  	break;
			  	}
			  int playListIdLen = data.readInt32();
			  if(playListIdLen> 0)
			  	{
			  pPlayListId = (unsigned char *)malloc( playListIdLen);
			  data.read(pPlayListId,playListIdLen);
			  	}
			  else
			  	{
			  	 free(pFileName);
				//  LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);
			  	break;
			  	}
			  int libraryUidLen = data.readInt32();
			  if(libraryUidLen > 0)
			  	{
			  pLibraryUID = (unsigned char *)malloc( libraryUidLen);
			  data.read(pLibraryUID,libraryUidLen);
			  	}
			  else
			  	{
			  	  free(pFileName);
				  free(pPlayListId);
				 //  LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);
				  break;
			  	}
			// LOGI("xx1 %s %d msg=0x%0x05x\n",__FUNCTION__,__LINE__,msg);
			  	notifyIap2FileTransfer(msg,fileNameLen,pFileName,playListIdLen,pPlayListId,libraryUidLen,pLibraryUID);
				//LOGI("xx1 %s %d \n",__FUNCTION__,__LINE__);
			       free(pFileName);
				  free(pPlayListId);
				   free(pLibraryUID);
			}
			break;
		case NOTIFY_RAWSTR:
		  {
		  	CHECK_INTERFACE(IPcmServiceNotify, data, reply);
			int msg = data.readInt32();
            int ext1 = data.readInt32();
            if(ext1>0){
			unsigned char *pData = (unsigned char *)malloc( ext1);
                    if(data.read(pData,ext1) == 0){    
            notifyRawString(msg, ext1, pData);
			free(pData);
			
                  }
            	}
			}
			break;
        case NOTIFY_OBJ:{
            CHECK_INTERFACE(IPcmServiceNotify, data, reply);
            int msg = data.readInt32();
            int ext1 = data.readInt32();
            int ext2 = data.readInt32();
            Parcel obj;
            if (data.dataAvail() > 0) {
                obj.appendFrom(const_cast<Parcel *>(&data), data.dataPosition(), data.dataAvail());
            }
            notifyObj(msg, ext1, ext2, &obj);
            return NO_ERROR;
        }break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
	    return NO_ERROR;
}

}; // namespace android

