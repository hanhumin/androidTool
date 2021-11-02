/*
**
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
#include <unistd.h> // for getpid

#undef LOG_TAG
#define LOG_TAG "IapAudioHw"
#include <utils/Log.h>

#include <utils/threads.h>
#include <utils/Errors.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <media/AudioSystem.h>

#include <cutils/properties.h> // for property_get

#include "iap_audio_hw.h"

#define PCM_CARD    1
#define PCM_DEVICE  0

namespace android {

#define DEBUG_ON
#undef DEBUG_ON

#if defined(DEBUG_ON)
#define  GET_SHORT_FILENAME(longfilename)   \
    ( (strrchr(longfilename, '/')==NULL) ? \
        longfilename: ((char*)(strrchr(longfilename, '/')+1)))


#define gprintf(fmt, args...)      \
    ALOGE("[%d], %s:%d, %s(): "fmt,           \
        gettid(), GET_SHORT_FILENAME(__FILE__), __LINE__, __FUNCTION__, ##args)                         
#else

#define gprintf
#endif

Mutex IapAudioHw::mLock;
float IapAudioHw::mVolume = 1.0f;

IapAudioHw::IapAudioHw(uint32_t mode)
{

    mMode       = mode;
    mFrameSize  = 4;
    mRequestChannels = 2;

	mConfig.channels = 2;
	mConfig.rate = 44100;
	mConfig.period_size = 512;
	mConfig.period_count = 4;
	mConfig.format = PCM_FORMAT_S16_LE;
	mConfig.start_threshold = 1;
	mConfig.stop_threshold = 512*4;
	mConfig.silence_threshold = 0;
	mConfig.avail_min = 512;

    mPcmHandle = NULL;

}

IapAudioHw::~IapAudioHw()
{
	gprintf("IapAudioHw destroyed");
}


status_t IapAudioHw::open(int b, int l, int p,int sound_cardNum)
{
    int err = NO_ERROR;

  //  AutoMutex lock(mLock);
   // AutoMutex lock2(mAccessLock);

	gprintf("IapAudioHw::open start in");


    if(mPcmHandle != NULL)
    {
        ALOGE("Error: Already opened");        
       // return -1;
       close();
    }
    else if(p < 10000)
    {
        
        ALOGE("Error: too short period, it must be over 100000");        
        return NO_ERROR;
    }

    mConfig.period_size = (mConfig.rate * (p/1000))/1000;

    if(mConfig.period_size)
    {
        int comp_val;
        int rest;
        int i;

        for(i=31; i>=0; i--)
        {
            if(mConfig.period_size & (0x00000001 << i))
                break;
        }

        comp_val = 0x00000001 << i;

        if(comp_val < mConfig.period_size)
        {
            rest = mConfig.period_size - comp_val;
            
            if(rest >= (comp_val/2))
                mConfig.period_size = comp_val << 0x01;
            else
                mConfig.period_size = comp_val;                       
        }            
    }
    else
    {
        ALOGD("mConfig.period_size %d", mConfig.period_size);    
        return -1;
    }

    mConfig.period_count = b / mConfig.period_size;

    if(mConfig.period_count < 2)
    {
        ALOGE("Error: too small mConfig.period_count, it must be over 2");        

    }

	mConfig.stop_threshold = mConfig.period_size*mConfig.period_count;
	mConfig.avail_min = mConfig.period_size;

    ALOGD("mConfig.period_size %d", mConfig.period_size);
    ALOGD("mConfig.period_count %d", mConfig.period_count);
    ALOGD("sound_cardNum %d", sound_cardNum);
	    ALOGD("mMode %d", mMode);
     if(mMode == 0)
    	mPcmHandle = pcm_open(sound_cardNum, PCM_DEVICE, PCM_OUT, &mConfig);	
    else
    	mPcmHandle = pcm_open(sound_cardNum, PCM_DEVICE, PCM_IN,  &mConfig);	  

    if(!pcm_is_ready(mPcmHandle))
    {
        ALOGE("pcm not open!!!!!! : %s", pcm_get_error(mPcmHandle));
        pcm_close(mPcmHandle);
        mPcmHandle = NULL;
        return -1;
    }

	return err;
}


status_t IapAudioHw::close(void)
{
    status_t err = NO_ERROR;

   // AutoMutex lock(mLock);
    //AutoMutex lock2(mAccessLock);


    if(mPcmHandle != NULL && pcm_is_ready(mPcmHandle))
    {
        pcm_close(mPcmHandle);
        mPcmHandle = NULL;
    }

    ALOGD("mode(%d): close", mMode);
    return err;
}

int IapAudioHw::write(const void* buffer, size_t size)
{
    size_t remain_size = 0;
    size_t written = 0;
    const size_t max_size = PCM_TEMP_BUFF_SIZE/2;
    short *sbuffer;
    status_t err = 0;

    AutoMutex lock(mAccessLock);
	if(mPcmHandle == NULL)
		   return -1;


    if(!pcm_is_ready(mPcmHandle)){
        ALOGE("handler write is NULL");
        return (-1);
    }
    else {
    	
        if(mRequestChannels == 1){
            remain_size = size;
            sbuffer = (short*)buffer;
            
            while(remain_size)
            {
                if(remain_size > max_size)
                {                   
                    pcm_mono_to_stereo(&sbuffer[written], mWriteTempPcmBuff, max_size);

                    err = pcm_write(mPcmHandle, mWriteTempPcmBuff, PCM_TEMP_BUFF_SIZE);
                    if(err < 0)
                        break;
					#ifdef USE_IAP2_TCC
                    if(pcm_check_underrun(mPcmHandle))
                        ALOGE("IapAudioHw::write pipe error %d", pcm_check_underrun(mPcmHandle));
                     #endif   
                    remain_size -= max_size;
                    written += max_size;
                }
                else
                {
                    pcm_mono_to_stereo(&sbuffer[written], mWriteTempPcmBuff, remain_size);
                    err = pcm_write(mPcmHandle, mWriteTempPcmBuff, remain_size*2);
                    if(err < 0)
                        break;
#ifdef USE_IAP2_TCC

                    if(pcm_check_underrun(mPcmHandle))
                        ALOGE("IapAudioHw::write pipe error %d", pcm_check_underrun(mPcmHandle));
#endif
                    remain_size = 0;
                }
            }
        }
        else{
            err = pcm_write(mPcmHandle, (char *)buffer, size); // Planet 20120126
            #ifdef USE_IAP2_TCC
            if(pcm_check_underrun(mPcmHandle))
                ALOGE("IapAudioHw::write pipe error %d", pcm_check_underrun(mPcmHandle));
			#endif
        }
      
        if(err < 0) {
			ALOGE("alsa write fail!!!.. %s\n", pcm_get_error(mPcmHandle));
            return err;
		}
			
    }

//    gprintf("write: buffer=0x%p, bytes=%d, size=%d, frames=%d", buffer, written, size, snd_pcm_bytes_to_frames(mPcmHandle, size));
    return size;
}


int IapAudioHw::read(const void* buffer, size_t size)
{
    status_t err = 0;
    size_t remain_size = 0;
    size_t readbytes = 0;
    const size_t max_size = PCM_TEMP_BUFF_SIZE/2;
    short *sbuffer;

    AutoMutex lock(mAccessLock);
     if(mPcmHandle == NULL)
	 	return -1;
    if(!pcm_is_ready(mPcmHandle)){
        ALOGE("handler read is NULL");
        return -1;
    }
    else {    	
		if(mRequestChannels == 1){
            remain_size = size;
            sbuffer = (short*)buffer;
            
            while(remain_size)
            {
                if(remain_size > max_size)
                {                   
        			err = pcm_read(mPcmHandle, mReadTempPcmBuff, PCM_TEMP_BUFF_SIZE);	// Planet 20120126
        			if(err < 0)
                        break;
#ifdef USE_IAP2_TCC

                    if(pcm_check_underrun(mPcmHandle))
                        ALOGE("IapAudioHw::read pipe error %d", pcm_check_underrun(mPcmHandle));
                    #endif
        			pcm_stereo_to_mono(mReadTempPcmBuff, &sbuffer[readbytes], max_size);
                    remain_size -= max_size;
                    readbytes += max_size;
                }
                else
                {
        			err = pcm_read(mPcmHandle, mReadTempPcmBuff, remain_size*2);	// Planet 20120126
        			if(err < 0)
                        break;
#ifdef USE_IAP2_TCC

                    if(pcm_check_underrun(mPcmHandle))
                        ALOGE("IapAudioHw::read pipe error %d", pcm_check_underrun(mPcmHandle));
#endif
        			pcm_stereo_to_mono(mReadTempPcmBuff, &sbuffer[readbytes], remain_size);
                    remain_size = 0;
                }
            }
		}
		else{
			err = pcm_read(mPcmHandle, (char *)buffer, size);	// Planet 20120126
			#ifdef USE_IAP2_TCC
            if(pcm_check_underrun(mPcmHandle))
                ALOGE("IapAudioHw::read pipe error %d", pcm_check_underrun(mPcmHandle));
			#endif
		}
        
        if(err < 0) {
			ALOGE("alsa read fail!!!.. %s\n", pcm_get_error(mPcmHandle));
            return err;
		}
			
    }

//    return static_cast<ssize_t>(snd_pcm_frames_to_bytes(mPcmHandle, n));
    return size;
}

void IapAudioHw::flush(void)
{
    gprintf("device flush");
}

void IapAudioHw::pause(void)
{
    gprintf("device pause");

    close();

    gprintf("device pause done...");
}

void IapAudioHw::setSampleRate(int sampleRate)
{

    gprintf("device setSampleRate=%d", sampleRate);
	mConfig.rate = sampleRate;
}

void IapAudioHw::setChannelCount(int channel)
{

    gprintf("device setChannelCount=%d", channel);

//	mConfig.channels = channel;
		mRequestChannels = channel;
}

void IapAudioHw::setFrameSize(int framesize)
{
    mFrameSize = framesize;
}

int IapAudioHw::getDelay(long *delayp)
{
   int err = 0;
//   err = pcm_avail_update(mPcmHandle);
   return err;
}

int IapAudioHw::setRoute(int device)
{
/*
    const sp<IAudioFlinger>& af = AudioSystem::get_audio_flinger();
    if (af == 0) return PERMISSION_DENIED;

    AudioParameter param = AudioParameter();
    param.addInt(String8(AudioParameter::keyRouting), (int)device);


    return af->setParameters(0, param.toString());
*/
	return 0;    
}

int IapAudioHw::setMute(bool mute)
{
    // To do.
    ALOGD("setMute(%d)", mute);
    
    return 0;
}

int IapAudioHw::setVolume(float volume)
{
    // To do.
    ALOGD("setVolume(%f)", volume);
    
    mVolume = volume;
    return 0;
}

void IapAudioHw::pcm_stereo_to_mono(void *input, void *output, int size)
{	
    size /=2;
    for(int i = 0; i < size; i++){
        ((short *)output)[i] = ((short *)input)[i*2];											
    }	
}

void IapAudioHw::pcm_mono_to_stereo(void *input, void *output, int size)
{
    int out_idx = 0;    

    size /=2;
    for(int i = 0; i < size; i++, out_idx+=2){							
        ((short *)output)[out_idx] = ((short *)input)[i];						
        ((short *)output)[out_idx+1] = ((short *)input)[i];
    }	
}

}; // namespace android

