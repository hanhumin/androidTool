package com.txl.player.base;

import android.annotation.TargetApi;
import android.content.Context;
import android.net.Uri;
import android.os.Build;

import java.io.FileDescriptor;
import java.io.IOException;
import java.util.Map;

/**
 * 参考Android系统，MediaPlayer 抽取播放器接口
 * */
public interface IPlayer extends VideoRenderComponent {
    /**
     * 视频的显示封装在VideoComponent
     * */
    VideoRenderComponent getVideoRenderComponent();

    void setDataSource(Context context, Uri uri)
            throws IOException, IllegalArgumentException, SecurityException, IllegalStateException;

    @TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
    void setDataSource(Context context, Uri uri, Map<String, String> headers)
            throws IOException, IllegalArgumentException, SecurityException, IllegalStateException;

    void setDataSource(FileDescriptor fd)
            throws IOException, IllegalArgumentException, IllegalStateException;

    void setDataSource(String path)
            throws IOException, IllegalArgumentException, SecurityException, IllegalStateException;

    String getDataSource();

    void prepareAsync() throws IllegalStateException;

    void start() throws IllegalStateException;

    void stop() throws IllegalStateException;

    void pause() throws IllegalStateException;

    void setScreenOnWhilePlaying(boolean screenOn);

    int getVideoWidth();

    int getVideoHeight();

    boolean isPlaying();

    void seekTo(long msec) throws IllegalStateException;

    long getCurrentPosition();

    long getDuration();

    void release();

    boolean isRelease();

    void reset();

    void setVolume(float leftVolume, float rightVolume);

    int getAudioSessionId();

    void setLooping(boolean looping);

    boolean isLooping();

    void setPlayerEventListener(IPlayerEventListener listener);

    /**
     * 设置硬解码
     */
    void setEnableMediaCodec(boolean isEnable);

    void setOnRenderChangeListener(OnRenderChangeListener renderChangeListener);

    void setAudioStreamType(int streamtype);

}
