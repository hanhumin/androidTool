package com.example.txl.tool.bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothProfile;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.AssetFileDescriptor;
import android.media.AudioFocusRequest;
import android.media.AudioManager;
import android.media.MediaMetadata;
import android.media.MediaPlayer;
import android.media.session.MediaController;
import android.media.session.MediaSession;
import android.media.session.PlaybackState;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.ResultReceiver;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import com.example.txl.tool.R;
import com.example.txl.tool.bluetooth.helper.AvrcpControllerHelper;
import com.example.txl.tool.utils.AppExecutors;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

/**
 * Copyright (c) 2021 唐小陆 All rights reserved.
 * author：txl
 * date：2021/6/5
 * description：
 */
public class BluetoothPlayerActivity extends AppCompatActivity {
    private final String TAG = BluetoothPlayerActivity.class.getSimpleName();
    BluetoothProfile bluetoothAvrcpController;
    MediaPlayer mediaPlayer = new MediaPlayer();

    AudioManager audioManager;
    AudioFocusRequest audioFocusRequest;
    MediaSession mediaSession;
    private boolean destroy = false;

    private final BluetoothProfile.ServiceListener serviceListener = new BluetoothProfile.ServiceListener() {
        @Override
        public void onServiceConnected(int profile, BluetoothProfile proxy) {

            if(profile == AvrcpControllerHelper.AVRCP_CONTROLLER){
                Log.d(TAG,"connect bluetoothAvrcpController ");
                bluetoothAvrcpController = proxy;
                List<BluetoothDevice> devices = bluetoothAvrcpController.getConnectedDevices();
                if(devices != null && devices.size() != 0){
                    BluetoothDevice device = devices.get(0);
                    Log.d(TAG,"connect bluetoothAvrcpController devices "+device.getName()+"  address : "+device.getAddress());
                }
                Object avrcpControllerService = AvrcpControllerHelper.getBluetoothAvrcpControllerService(proxy);
                if(avrcpControllerService != null){
                    Log.d(TAG,"connect bluetoothAvrcpController devices "+avrcpControllerService.getClass());
                }
            }else {
                Log.d(TAG,"connect other ");
            }
        }

        @Override
        public void onServiceDisconnected(int profile) {

        }
    };

    String name,author, currentTime, totalTime;
    boolean play = false;

    BroadcastReceiver avrcpBroadcastReceiver = new BroadcastReceiver() {
        @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
        @Override
        public void onReceive(Context context, Intent intent) {
            if(intent.getAction().equals(AvrcpControllerHelper.ACTION_TRACK_EVENT)){
                final MediaMetadata mediaMetadata = intent.getParcelableExtra(AvrcpControllerHelper.EXTRA_METADATA);
                final PlaybackState playbackState = intent.getParcelableExtra(AvrcpControllerHelper.EXTRA_PLAYBACK);
//                if(mediaMetadata == null || playbackState == null){
//                    Log.e(TAG,"some info null ? = "+mediaMetadata+"   "+playbackState);
//                    return;
//                }
                if(mediaMetadata != null){
                    name = mediaMetadata.getString(MediaMetadata.METADATA_KEY_TITLE);
                    author = mediaMetadata.getString(MediaMetadata.METADATA_KEY_ARTIST);
                    totalTime = timeCover(+mediaMetadata.getLong(MediaMetadata.METADATA_KEY_DURATION));
                }
                if(playbackState != null){
                    currentTime = timeCover(playbackState.getPosition());
                    play = playbackState.getState() == PlaybackState.STATE_PLAYING;
                }
                TextView textView = findViewById(R.id.tvPlayTime);
                textView.setText("开始时间"+currentTime);
                textView = findViewById(R.id.tvTotalTime);
                textView.setText("总时长"+totalTime);
                textView = findViewById(R.id.tvMusicInfo);
                textView.setText("歌名："+name   +"------------ 作者 ： "+author);
                textView = findViewById(R.id.tvPlayToggle);
                textView.setText("远程播放器 "+(play?"正在播放":"未播放"));
            }else if(intent.getAction().equals(AvrcpControllerHelper.ACTION_FOLDER_LIST ) || intent.getAction().equals(AvrcpControllerHelper.EXTRA_FOLDER_LIST)){
                Log.d(TAG,"ACTION_FOLDER_LIST");
            }
        }
    };

    /**
     * 将ms 时间转化成  00:00 格式
     * */
    private String timeCover(long time){
        long s = time/1000;
        long m = s/60;
        s = s%60;
        if(m>60){
            return (m/60)+":"+(m%60)+":"+s;
        }else {
            return m+":"+s;
        }
    }


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
        IntentFilter filter = new IntentFilter(AvrcpControllerHelper.ACTION_TRACK_EVENT);
        filter.addAction(AvrcpControllerHelper.ACTION_FOLDER_LIST);
        filter.addAction(AvrcpControllerHelper.EXTRA_FOLDER_LIST);
        registerReceiver(avrcpBroadcastReceiver,filter);
    }

    private void initPlayer() {
        mediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                mediaPlayer.start();
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                    mediaSession = new MediaSession(BluetoothPlayerActivity.this, "test");
                    mediaSession.setCallback(new MediaSession.Callback() {
                        /**
                         * Called when a controller has sent a command to this session.
                         * The owner of the session may handle custom commands but is not
                         * required to.
                         *
                         * @param command The command name.
                         * @param args    Optional parameters for the command, may be null.
                         * @param cb      A result receiver to which a result may be sent by the command, may be null.
                         */
                        @Override
                        public void onCommand(@NonNull String command, @Nullable Bundle args, @Nullable ResultReceiver cb) {
                            Log.e(TAG,"onCommand");
                            super.onCommand(command, args, cb);
                        }

                        /**
                         * Called when a media button is pressed and this session has the
                         * highest priority or a controller sends a media button event to the
                         * session. The default behavior will call the relevant method if the
                         * action for it was set.
                         * <p>
                         * The intent will be of type {@link Intent#ACTION_MEDIA_BUTTON} with a
                         * KeyEvent in {@link Intent#EXTRA_KEY_EVENT}
                         *
                         * @param mediaButtonIntent an intent containing the KeyEvent as an
                         *                          extra
                         * @return True if the event was handled, false otherwise.
                         */
                        @Override
                        public boolean onMediaButtonEvent(@NonNull Intent mediaButtonIntent) {
                            Log.e(TAG,"onMediaButtonEvent");
                            Toast.makeText(BluetoothPlayerActivity.this,"我对你那么好，你居然操作我",Toast.LENGTH_SHORT).show();
                            return super.onMediaButtonEvent(mediaButtonIntent);
                        }
                    });
                    mediaSession.setActive(true);
                    mediaSession.setMetadata(new MediaMetadata.Builder()
                            .putString(MediaMetadata.METADATA_KEY_TITLE, "你是风而我是沙")
                            .putString(MediaMetadata.METADATA_KEY_ARTIST, "我是谁谁谁")
                            .build());
                    Toast.makeText(BluetoothPlayerActivity.this,"已经开始执行发送信息，重置需要退出重进",Toast.LENGTH_SHORT).show();
                    AppExecutors.getInstance().networkIO().execute(new Runnable() {
                        @Override
                        public void run() {
                            int i = 0;
                            while (!destroy){
                                MediaController controller = mediaSession.getController();
                                controller.getQueue();
                                Log.d(TAG,"update PlaybackState  "+mediaPlayer.getCurrentPosition());
                                mediaSession.setPlaybackState(new PlaybackState.Builder()
                                        .setState(PlaybackState.STATE_PLAYING, mediaPlayer.getCurrentPosition(), 1.0f)
                                        .build());
                                mediaSession.setMetadata(new MediaMetadata.Builder()
                                        .putLong(MediaMetadata.METADATA_KEY_DURATION, mediaPlayer.getDuration())
                                        .putString(MediaMetadata.METADATA_KEY_TITLE, "你是风而我是沙"+i)
                                        .putString(MediaMetadata.METADATA_KEY_ARTIST, "我是谁谁谁")
                                        .build());

                                ArrayList<MediaSession.QueueItem> queueItems = new ArrayList<>();
                                queueItems.add(new MediaSession.QueueItem(new MediaMetadata.Builder()
                                        .putString(MediaMetadata.METADATA_KEY_TITLE, "你是风而我是沙===="+i)
                                        .putString(MediaMetadata.METADATA_KEY_ARTIST, "我是谁谁谁====")
                                        .build().getDescription(),i+1));
                                queueItems.add(new MediaSession.QueueItem(new MediaMetadata.Builder()
                                        .putString(MediaMetadata.METADATA_KEY_TITLE, "你是风而我是沙-----"+i)
                                        .putString(MediaMetadata.METADATA_KEY_ARTIST, "我是谁谁谁----")
                                        .build().getDescription(),1+2));
                                mediaSession.setQueue(queueItems);
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
        findViewById(R.id.tvPlayToggle).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(BluetoothPlayerActivity.this,"操作远端播放器",Toast.LENGTH_SHORT).show();
                List<BluetoothDevice> list =  AvrcpControllerHelper.getConnectedDevices(bluetoothAvrcpController);
                if(list!= null && !list.isEmpty()){
                    BluetoothDevice device = list.get(0);
                    Log.d(TAG,"call cmd ");
                    AvrcpControllerHelper.sendGroupNavigationCmd(bluetoothAvrcpController,device,AvrcpControllerHelper.BluetoothAvrcp.PASSTHROUGH_ID_FORWARD, AvrcpControllerHelper.BluetoothAvrcp.PASSTHROUGH_STATE_PRESS);
                    AvrcpControllerHelper.sendGroupNavigationCmd(bluetoothAvrcpController,device,AvrcpControllerHelper.BluetoothAvrcp.PASSTHROUGH_ID_FORWARD, AvrcpControllerHelper.BluetoothAvrcp.PASSTHROUGH_STATE_RELEASE);
                }else {
                    Log.d(TAG,"call cmd failed");
                }

            }
        });
        TextView tvLocalPlayer = findViewById(R.id.tvLocalPlayer);
        tvLocalPlayer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //作为本地 音乐播放
                try {
                    destroy = false;
                    mediaPlayer.reset();
                    mediaPlayer.setLooping(true);//循环播放
                    AssetFileDescriptor assetFileDescriptor = getResources().getAssets().openFd("test_bluetooth.mp3");
                    mediaPlayer.setDataSource(assetFileDescriptor.getFileDescriptor(),assetFileDescriptor.getStartOffset(),assetFileDescriptor.getDeclaredLength());
                    mediaPlayer.prepareAsync();
                    unRegisterProfile();
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
                destroy = true;
                registerProfile();
            }
        });
    }


    private void registerProfile(){
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        adapter.getProfileProxy(this,serviceListener,AvrcpControllerHelper.AVRCP_CONTROLLER);
    }

    private void unRegisterProfile(){
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        if(bluetoothAvrcpController != null){
            adapter.closeProfileProxy( AvrcpControllerHelper.AVRCP_CONTROLLER,bluetoothAvrcpController);
            bluetoothAvrcpController = null;
        }

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
        unRegisterProfile();
        unregisterReceiver(avrcpBroadcastReceiver);
        if(mediaSession != null){
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                mediaSession.release();
            }
            mediaSession = null;
        }
    }
}
