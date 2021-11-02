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

#ifndef ANDROID_HARDWARE_JNIIPODNOTIFY_H
#define ANDROID_HARDWARE_JNIIPODNOTIFY_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/RefBase.h>
#include <utils/Errors.h>
#include <binder/IInterface.h>
#include <binder/IMemory.h>
#include <binder/Parcel.h>
#include <utils/String8.h>
#include <binder/BinderService.h>
#include "IIapServiceNotify.h"


#include <android_runtime/AndroidRuntime.h>

#include <binder/IServiceManager.h>

#include "android_os_Parcel.h"

namespace android {

class JNITelechipsIap1Listener: public BinderService<JNITelechipsIap1Listener>, public BnIapServiceNotify,public IBinder::DeathRecipient


{
public:
    bool serverDied;
    JNITelechipsIap1Listener();
    ~JNITelechipsIap1Listener();
    void notify(int msg, int ext1, int ext2,int ext3);
	void notifyRawString(int msg, int ext1,  const unsigned char  *obj); 
	void notifyRawByte(int msg, int ext1,  const unsigned char  *obj); 
	static char const* getServiceName() { return "JNITelechipsIap1Listener"; }
    void notifyObj(int what,int arg1, int arg2,const Parcel *obj);
    void binderDied(const wp<IBinder>& who);
    void clearService(void);
	int gListenerCount = 0;
};




//------------------------------------------------

}; // namespace android

#endif //ANDROID_HARDWARE_IIAPSERVICE_H

