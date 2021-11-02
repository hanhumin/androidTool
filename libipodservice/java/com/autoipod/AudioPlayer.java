package com.autoipod;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import com.autoipod.jni.Ipod;

import static android.content.Context.AUDIO_SERVICE;
public class AudioPlayer {

    private static final String TAG = "AudioPlayer";

    private static final int DEFAULT_STREAM_TYPE = AudioManager.STREAM_MUSIC;
    private static  int DEFAULT_SAMPLE_RATE = 44100;
    private static final int DEFAULT_CHANNEL_CONFIG = AudioFormat.CHANNEL_IN_STEREO;
    private static final int DEFAULT_AUDIO_FORMAT = AudioFormat.ENCODING_PCM_16BIT;
    private static final int DEFAULT_PLAY_MODE = AudioTrack.MODE_STREAM;

    private boolean mIsPlayStarted = false;
    private int mMinBufferSize = 0;
    private AudioTrack mAudioTrack;
    private AudioManager mAm;
    private boolean hasAudioFocus = false;
    private Context mContext;



    public AudioPlayer(Context context) {
        mContext = context;
        mAm = (AudioManager) context.getSystemService(AUDIO_SERVICE);

        IntentFilter intentFilter = new IntentFilter();

        intentFilter.addAction(Constants.ACTION_IPOD_REQUEST_AUDIO_FOCUS);
        LocalBroadcastManager localBroadcastManager = LocalBroadcastManager.getInstance(context);
        localBroadcastManager.registerReceiver(receiver, intentFilter);
		// localBroadcastManager.sendBroadcast(new Intent(Constants.ACTION_IPOD_REQUEST_AUDIO_FOCUS));
    }

    private BroadcastReceiver receiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
			Log.i(TAG,"intent.getAction:"+intent.getAction()+" hasAudioFocus="+hasAudioFocus);
            if (action.equals(Constants.ACTION_IPOD_REQUEST_AUDIO_FOCUS)) {
				if(!hasAudioFocus)
                  hasAudioFocus = requestFocus();
            }
        }
    };

    public boolean startPlayer() {
        return startPlayer(DEFAULT_SAMPLE_RATE,DEFAULT_CHANNEL_CONFIG,DEFAULT_AUDIO_FORMAT);
    }

    AudioManager.OnAudioFocusChangeListener afChangeListener = new AudioManager.OnAudioFocusChangeListener() {
        public void onAudioFocusChange(int focusChange) {
            Log.d(TAG,"foucsChange "+focusChange);
            if (focusChange == AudioManager.AUDIOFOCUS_LOSS_TRANSIENT) {
                hasAudioFocus = false;
                Ipod.getInstance().pause();

            } else if (focusChange == AudioManager.AUDIOFOCUS_GAIN) {
                hasAudioFocus = true;
                Ipod.getInstance().play();


            } else if (focusChange == AudioManager.AUDIOFOCUS_LOSS_TRANSIENT_CAN_DUCK) {
                hasAudioFocus = false;


            } else if (focusChange == AudioManager.AUDIOFOCUS_LOSS) {
                hasAudioFocus = false;
                // mAm.unregisterMediaButtonEventReceiver(RemoteControlReceiver);
               // mAm.abandonAudioFocus(afChangeListener);
                Ipod.getInstance().pause();
                // Stop playback
                // stop();
            }
        }
    };

    public boolean requestFocus() {
        // Request audio focus for playback
        int result = mAm.requestAudioFocus(afChangeListener,
                // Use the music stream.
                AudioManager.STREAM_MUSIC,
                // Request permanent focus.
                AudioManager.AUDIOFOCUS_GAIN);
        Log.d(TAG,"request Focus:"+result);
        return result == AudioManager.AUDIOFOCUS_REQUEST_GRANTED;
    }

    public boolean startPlayer(int sampleRateInHz, int channelConfig, int audioFormat) {

        if (mIsPlayStarted) {
            Log.e(TAG, "Player   already started !");
            return false;
        }
        Log.e(TAG, "sampleRateInHz :"+sampleRateInHz+" channelConfig:"+channelConfig+" audioFormat:"+audioFormat);
        mMinBufferSize = AudioTrack.getMinBufferSize(sampleRateInHz,channelConfig,audioFormat);
        if (mMinBufferSize == AudioTrack.ERROR_BAD_VALUE) {
            Log.e(TAG, "Invalid parameter !");
            return false;
        }
        Log.d(TAG , "getMinBufferSize = "+mMinBufferSize+" bytes !");

        mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC,sampleRateInHz,channelConfig,audioFormat,mMinBufferSize,DEFAULT_PLAY_MODE);
        if (mAudioTrack.getState() == AudioTrack.STATE_UNINITIALIZED) {
            Log.e(TAG, "AudioTrack initialize fail !");
            return false;
        }

        mIsPlayStarted = true;

        Log.d(TAG, "Start audio player success !");
        mAudioTrack.play();
        if (!hasAudioFocus)
            hasAudioFocus = requestFocus();
        return true;
    }

    public int getMinBufferSize() {
        return mMinBufferSize;
    }

    public void stopPlayer() {

        if (!mIsPlayStarted) {
            return;
        }

        if (mAudioTrack.getPlayState() == AudioTrack.PLAYSTATE_PLAYING) {
            mAudioTrack.stop();
        }

        mAudioTrack.release();
        mIsPlayStarted = false;
        if (hasAudioFocus)
            mAm.abandonAudioFocus(afChangeListener);
        hasAudioFocus = false;
        Log.d(TAG, "Stop audio player success !");
    }

    public void pause() {
        if (!mIsPlayStarted) {
            //  Log.e(TAG, "Player not started !");
            return;
        }



    }

    public void setStereoVolume(float leftVolume, float rightVolume){
        mAudioTrack.setStereoVolume(leftVolume,rightVolume);
    }

    public boolean play(byte[] audioData, int offsetInBytes, int sizeInBytes) {
        //       Log.e(TAG, "play mIsPlayStarted  ="+mIsPlayStarted+" hasAudioFocus="+hasAudioFocus);
        if (!mIsPlayStarted) {
              Log.e(TAG, "Player not started !");
            return false;
        }
        if (!hasAudioFocus)
        {
            Log.d(TAG,"play no audio foucs return ");
            return false;
        }
      /*  if (sizeInBytes < mMinBufferSize) {
            Log.e(TAG, "audio data is not enough !");
            return false;
        }*/
      //     Log.i("huanghanjing","play sizeInBytes="+sizeInBytes);
        if (mAudioTrack.write(audioData,offsetInBytes,sizeInBytes) != sizeInBytes) {
            Log.e(TAG, "Could not write all the samples to the audio device !");
        }



        //   Log.d(TAG , "OK, Played "+sizeInBytes+" bytes !");

        return true;
    }

    public boolean hasAudioFocus() {
        return hasAudioFocus;
    }
}