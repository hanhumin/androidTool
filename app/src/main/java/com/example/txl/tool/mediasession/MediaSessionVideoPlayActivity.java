package com.example.txl.tool.mediasession;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.app.ActivityManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.media.session.MediaController;
import android.media.session.MediaSessionManager;
import android.media.session.PlaybackState;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Toast;

import com.example.txl.tool.IMediaSessionToken;
import com.example.txl.tool.R;

import java.util.List;

public class MediaSessionVideoPlayActivity extends AppCompatActivity {
    private final String TAG = MediaSessionVideoPlayActivity.class.getSimpleName();
    MediaController controller;
    IMediaSessionToken token;
    MediaController.Callback callback = new MediaController.Callback() {
        @Override
        public void onPlaybackStateChanged(@Nullable PlaybackState state) {
            super.onPlaybackStateChanged(state);
            if(state == null){
                Toast.makeText(MediaSessionVideoPlayActivity.this,"播放器状态变化 "+"state is null",Toast.LENGTH_SHORT).show();
                return;
            }
            Toast.makeText(MediaSessionVideoPlayActivity.this,"播放器状态变化 "+(state.getState()==PlaybackState.STATE_PLAYING?"播放":"暂停"),Toast.LENGTH_SHORT).show();
        }
    };
    ServiceConnection connection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            try {
                token = IMediaSessionToken.Stub.asInterface(service);
                controller = new MediaController(MediaSessionVideoPlayActivity.this,token.getToken());
                controller.registerCallback(callback);
                setSurface();
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {

        }
    };

    private SurfaceView surfaceView;
    private Surface surface;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_media_session_video_play);
//        Intent intent = new Intent(this,MediaPlayerService.class);
        Intent intent = new Intent("com.txl.test.MediaPlayerService");
        intent.setPackage(getPackageName());
        bindService(intent,connection,Context.BIND_AUTO_CREATE);
        findViewById(R.id.tvPlay).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(controller != null){
                    controller.getTransportControls().play();
                }
            }
        });
        findViewById(R.id.tvPause).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(controller != null){
                    controller.getTransportControls().pause();
                }
            }
        });
        surfaceView = findViewById(R.id.surfaceView);
        surfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                surface = holder.getSurface();
                setSurface();
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {

            }
        });
        ActivityManager manager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
//        List<ActivityManager.ProcessErrorStateInfo> processErrorStateInfos =  manager.getProcessesInErrorState();
//        Log.d(TAG,"info");
//        Log.d(TAG,processErrorStateInfos.toString());
    }

    private void setSurface(){
        if(token != null && surface != null){
            try {
                token.setSurface(surface);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(controller != null){
            controller.getTransportControls().pause();
            controller.unregisterCallback(callback);
        }
        unbindService(connection);
    }
}