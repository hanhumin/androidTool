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

#include "IIapServiceNotify.h"

namespace android {

enum {
    NOTIFY = IBinder::FIRST_CALL_TRANSACTION,
    NOTIFY_OBJ,
    NOTIFY_RAWSTR,
    NOTIFY_RAWBYTE,
    NOTIFY_IAP2_FILE_TRANSFER
};

class BpIapServiceNotify: public BpInterface<IIapServiceNotify>
{
public:
    BpIapServiceNotify(const sp<IBinder>& impl)
        : BpInterface<IIapServiceNotify>(impl)
    {
    }

    virtual void notify(int msg, int ext1, int ext2,int ext3)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IIapServiceNotify::getInterfaceDescriptor());
        data.writeInt32(msg);
        data.writeInt32(ext1);
        data.writeInt32(ext2);
        data.writeInt32(ext3);  
	
        remote()->transact(NOTIFY, data, &reply, IBinder::FLAG_ONEWAY);
	
    }

    virtual void notifyRawByte(int msg, int ext1,  const unsigned char  *obj)
    {
      
        Parcel data, reply;
        data.writeInterfaceToken(IIapServiceNotify::getInterfaceDescriptor());
        data.writeInt32(msg);
        data.writeInt32(ext1);
        data.write(obj,ext1);
       // if (obj && obj->dataSize() > 0) {
          //  data.appendFrom(const_cast<Parcel *>(obj), 0, obj->dataSize());
        //}
        remote()->transact(NOTIFY_RAWBYTE, data, &reply, IBinder::FLAG_ONEWAY);
    }


		
	virtual void notifyRawString(int msg, int ext1,  const unsigned char  *obj)
    {
      
        Parcel data, reply;
        data.writeInterfaceToken(IIapServiceNotify::getInterfaceDescriptor());
        data.writeInt32(msg);
        data.writeInt32(ext1);
        data.write(obj,ext1);
       // if (obj && obj->dataSize() > 0) {
          //  data.appendFrom(const_cast<Parcel *>(obj), 0, obj->dataSize());
        //}
        remote()->transact(NOTIFY_RAWSTR, data, &reply, IBinder::FLAG_ONEWAY);
    }
    virtual void notifyObj(int msg, int ext1, int ext2, const Parcel *obj)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IIapServiceNotify::getInterfaceDescriptor());
        data.writeInt32(msg);
        data.writeInt32(ext1);
        data.writeInt32(ext2);
        if (obj && obj->dataSize() > 0) {
            data.appendFrom(const_cast<Parcel *>(obj), 0, obj->dataSize());
        }
        remote()->transact(NOTIFY_OBJ, data, &reply, IBinder::FLAG_ONEWAY);
    }
};

IMPLEMENT_META_INTERFACE(IapServiceNotify, "android.iap.IIapServiceNotify");

// ----------------------------------------------------------------------

status_t BnIapServiceNotify::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch(code) {
        case NOTIFY: {
            CHECK_INTERFACE(IIapServiceNotify, data, reply);
            int msg = data.readInt32();
            int ext1 = data.readInt32();
            int ext2 = data.readInt32();
            int ext3 = data.readInt32();
            notify(msg, ext1, ext2,ext3);
            return NO_ERROR;
        } break;
		case NOTIFY_RAWBYTE:
			  {
		  	CHECK_INTERFACE(IIapServiceNotify, data, reply);
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
		
		case NOTIFY_RAWSTR:
		  {
		  	CHECK_INTERFACE(IIapServiceNotify, data, reply);
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
            CHECK_INTERFACE(IIapServiceNotify, data, reply);
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

