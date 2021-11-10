package com.example.txl.tool.mediasession;

import android.app.Service;
import android.content.Intent;
import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;
import android.media.session.MediaSession;
import android.media.session.MediaSession.Token;
import android.media.session.PlaybackState;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.Surface;

import com.example.txl.tool.IMediaSessionToken;

import java.io.IOException;

/**
 * 通过MediaSession 跨进程来进行 视频播放测试
 * */
public class MediaPlayerService extends Service {
    private final String TAG = MediaPlayerService.class.getSimpleName();
    private MediaSession mediaSession;
    private MediaPlayer mediaPlayer;

    public MediaPlayerService() {

    }

    @Override
    public void onCreate() {
        super.onCreate();

        mediaPlayer = new MediaPlayer();
        mediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                Log.d(TAG,"onPrepared");
            }
        });
        mediaPlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mp, int what, int extra) {
                Log.d(TAG,"onError "+what+"  extra "+extra);
                return false;
            }
        });
        try {
            AssetFileDescriptor afd = getResources().getAssets().openFd("test_video.mp4");
            mediaPlayer.setDataSource(afd.getFileDescriptor(),afd.getStartOffset(),afd.getDeclaredLength());
        } catch (IOException e) {
            e.printStackTrace();
        }
        mediaPlayer.setLooping(true);
        mediaPlayer.prepareAsync();
        mediaSession = new MediaSession(this,TAG);
        mediaSession.setCallback(new MediaSession.Callback() {
            @Override
            public void onPlay() {
                super.onPlay();
                Log.d(TAG,"onPlay");
                mediaPlayer.start();
                mediaSession.setPlaybackState(new PlaybackState.Builder().setState(PlaybackState.STATE_PLAYING,mediaPlayer.getCurrentPosition(),1.0f)
                        .build());
            }

            @Override
            public void onPause() {
                super.onPause();
                Log.d(TAG,"onPause");
                mediaPlayer.pause();
                mediaSession.setPlaybackState(new PlaybackState.Builder().setState(PlaybackState.STATE_PAUSED,mediaPlayer.getCurrentPosition(),1.0f)
                        .build());
            }
        });
        mediaSession.setActive(true);
    }

    @Override
    public IBinder onBind(Intent intent) {
        return new RemoteSessionToken(mediaSession,mediaPlayer);
    }

    public static class RemoteSessionToken extends IMediaSessionToken.Stub {
        private final MediaSession mediaSession;
        private final MediaPlayer mediaPlayer;

        public RemoteSessionToken(MediaSession mediaSession, MediaPlayer mediaPlayer) {
            this.mediaSession = mediaSession;
            this.mediaPlayer = mediaPlayer;
        }

        @Override
        public Token getToken() throws RemoteException {
            return mediaSession.getSessionToken();
        }

        @Override
        public void setSurface(Surface surface) throws RemoteException {
            mediaPlayer.setSurface(surface);
        }
    }
}