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

#include "IIapServiceCallback.h"

namespace android {

enum {
    CALLBACK = IBinder::FIRST_CALL_TRANSACTION
};

class BpIapServiceCallback: public BpInterface<IIapServiceCallback>
{
public:
    BpIapServiceCallback(const sp<IBinder>& impl)
        : BpInterface<IIapServiceCallback>(impl)
    {
    }
    virtual void onRspCb(const Parcel *obj)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IIapServiceCallback::getInterfaceDescriptor());
        if (obj && obj->dataSize() > 0) {
            data.appendFrom(const_cast<Parcel *>(obj), 0, obj->dataSize());
        }
        remote()->transact(CALLBACK, data, &reply, IBinder::FLAG_ONEWAY);
    }
};

IMPLEMENT_META_INTERFACE(IapServiceCallback, "android.iap.IIapServiceCallback");

// ----------------------------------------------------------------------

status_t BnIapServiceCallback::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch(code) {
        case CALLBACK:{
            CHECK_INTERFACE(IIapServiceCallback, data, reply);
            Parcel obj;
            if (data.dataAvail() > 0) {
                obj.appendFrom(const_cast<Parcel *>(&data), data.dataPosition(), data.dataAvail());
            }
            onRspCb(&obj);
            return NO_ERROR;
        }break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

}; // namespace android

