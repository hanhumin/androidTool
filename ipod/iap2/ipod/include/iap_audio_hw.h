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

#ifndef IAP_AUDIO_HW_H
#define IAP_AUDIO_HW_H


#include <tinyalsa/asoundlib.h>


namespace android {


class IapAudioHw {
public:
	int   	 	open(int b, int l, int p,int sound_cardNum);
	int   		close(void);
    int			write(const void* buffer, size_t size);
    int 	    read(const void* buffer, size_t size);
    void        flush(void);
    void        pause(void);
    void        setSampleRate(int sampleRate);
    void        setChannelCount(int channel);
    void        setFrameSize(int framesize);
    int         getDelay(long *delayp);
    int		    setRoute(int device);
    int		    setMute(bool mute);
    int		    setVolume(float volume);

	            IapAudioHw(uint32_t mode);
	virtual     ~IapAudioHw();

private:
    void 				pcm_stereo_to_mono(void *input, void *output, int size);
    void 				pcm_mono_to_stereo(void *input, void *output, int size);

    int                 inited;
    struct pcm*			mPcmHandle;
	struct pcm_config	mConfig;

    static Mutex        mLock;
    Mutex               mAccessLock;

    uint32_t            mMode;
    uint32_t            mFrameSize;
	uint32_t			mRequestChannels;

    static float        mVolume;

#define PCM_TEMP_BUFF_SIZE      4096
    char               mWriteTempPcmBuff[PCM_TEMP_BUFF_SIZE];
    char               mReadTempPcmBuff[PCM_TEMP_BUFF_SIZE];

};

}; // namespace android

#endif

