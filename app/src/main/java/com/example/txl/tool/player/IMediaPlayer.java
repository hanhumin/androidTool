package com.example.txl.tool.player;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;

import org.xml.sax.Locator;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/7
 * description：
 */
public interface IMediaPlayer {
    View init(Context ctx, ViewGroup parent);

    /**
     * 设置视频窗口大小和位置
     * @param location 参数格式 "top=left=width=height"
     * @return
     */
    boolean setLocation(String location);

    boolean setScaleMode(int mode);

    long getDuration();

    long getCurrentPosition();

    boolean seekTo(long pos);

    boolean setStopMode(int mode);

    boolean stop();

    boolean pause();

    boolean play();

    /**
     * 关闭视频层，播放广播流
     * @return
     */
    boolean closeVideo();

    /**
     * 打开视频层，播放视频流
     * @return
     */
    boolean openVideo();

    void setVFreez(int mode);

    boolean open(String url);

    boolean open(Locator url);

    boolean releasePlayer();

    void destroy();

    void updateProgress();

    boolean sendCommand(String cmd, Bundle extInfo);

    void setEventListener(IMediaPlayerEvents listener);

    boolean isPlaying();

    interface IMediaPlayerEvents {
        boolean onError(IMediaPlayer xmp, int code, String msg);

        boolean onPrepared(IMediaPlayer xmp);

        boolean onSeekComplete(IMediaPlayer xmp, long pos);

        boolean onComplete(IMediaPlayer xmp);

        boolean onBuffering(IMediaPlayer xmp, boolean buffering, float percentage);

        boolean onProgress(IMediaPlayer xmp, long pos);

        void onDestroy(IMediaPlayer xmp);
    }
}
