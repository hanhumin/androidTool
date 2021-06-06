package com.example.txl.tool.bluetooth;

import android.content.res.AssetFileDescriptor;
import android.media.AudioFocusRequest;
import android.media.AudioManager;
import android.media.MediaMetadata;
import android.media.MediaPlayer;
import android.media.session.MediaSession;
import android.media.session.PlaybackState;
import android.os.Build;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.example.txl.tool.R;
import com.example.txl.tool.utils.AppExecutors;

import java.io.IOException;
import java.io.InputStream;

/**
 * Copyright (c) 2021 唐小陆 All rights reserved.
 * author：txl
 * date：2021/6/5
 * description：
 */
class BluetoothPlayerActivity extends AppCompatActivity {
    private final String TAG = BluetoothPlayerActivity.class.getSimpleName();
    MediaPlayer mediaPlayer = new MediaPlayer();

    AudioManager audioManager;
    AudioFocusRequest audioFocusRequest;
    MediaSession mediaSession;
    private boolean destroy = false;


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth_player);
        audioManager = (AudioManager) getSystemService(AUDIO_SERVICE);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {//
            audioFocusRequest = new AudioFocusRequest.Builder(AudioManager.AUDIOFOCUS_GAIN)
                    .setOnAudioFocusChangeListener(new AudioManager.OnAudioFocusChangeListener() {
                        @Override
                        public void onAudioFocusChange(int focusChange) {
                            Log.d(TAG,"onAudioFocusChange  "+focusChange);
                            if(AudioManager.AUDIOFOCUS_GAIN == focusChange){
                                Toast.makeText(BluetoothPlayerActivity.this,"长期持有音频焦点",Toast.LENGTH_SHORT).show();
                            }
                        }
                    })
                    .build();
            audioManager.requestAudioFocus(audioFocusRequest);
        }
        initView();
        initPlayer();
    }

    private void initPlayer() {
        mediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                mediaPlayer.start();
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                    mediaSession = new MediaSession(BluetoothPlayerActivity.this, "test");
                    mediaSession.setActive(true);
                    mediaSession.setMetadata(new MediaMetadata.Builder()
                            .putString(MediaMetadata.METADATA_KEY_TITLE, "你是风而我是沙")
                            .putString(MediaMetadata.METADATA_KEY_TITLE, "你是风而我是沙lalalla")
                            .build());
                    Toast.makeText(BluetoothPlayerActivity.this,"已经开始执行发送信息，重置需要退出重进",Toast.LENGTH_SHORT).show();
                    AppExecutors.getInstance().networkIO().execute(new Runnable() {
                        @Override
                        public void run() {
                            int i = 0;
                            while (!destroy){
                                Log.d(TAG,"update PlaybackState  "+mediaPlayer.getCurrentPosition());
                                mediaSession.setPlaybackState(new PlaybackState.Builder()
                                        .setState(PlaybackState.STATE_PLAYING, mediaPlayer.getCurrentPosition(), 1.0f)
                                        .build());
                                mediaSession.setMetadata(new MediaMetadata.Builder()
                                        .putString(MediaMetadata.METADATA_KEY_TITLE, "你是风而我是沙"+i)
                                        .putString(MediaMetadata.METADATA_KEY_AUTHOR, "我是谁谁谁")
                                        .putLong(MediaMetadata.METADATA_KEY_DURATION, mediaPlayer.getDuration())
                                        .build());
                                SystemClock.sleep(500);
                                i++;
                            }
                        }
                    });
                }
            }
        });
    }

    private void initView() {
        TextView tvLocalPlayer = findViewById(R.id.tvLocalPlayer);
        tvLocalPlayer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //作为本地 音乐播放
                try {
                    mediaPlayer.reset();
                    mediaPlayer.setLooping(true);//循环播放
                    AssetFileDescriptor assetFileDescriptor = getResources().getAssets().openFd("test_bluetooth.mp3");
                    mediaPlayer.setDataSource(assetFileDescriptor.getFileDescriptor(),assetFileDescriptor.getStartOffset(),assetFileDescriptor.getDeclaredLength());
                    mediaPlayer.prepareAsync();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        TextView tvRemotePlayer = findViewById(R.id.tvRemotePlayer);
        tvRemotePlayer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mediaPlayer.reset();

            }
        });
    }


    private void registerProfile(){

    }

    private void unRegisterProfile(){

    }


    private void requestAudioFocus(){
        audioManager = (AudioManager) getSystemService(AUDIO_SERVICE);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            audioManager.abandonAudioFocusRequest(audioFocusRequest);
            audioManager.requestAudioFocus(audioFocusRequest);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mediaPlayer.reset();
        mediaPlayer.release();
        mediaPlayer = null;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            audioManager.abandonAudioFocusRequest(audioFocusRequest);
        }
        destroy = true;
    }
}
