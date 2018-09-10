package com.example.txl.tool.huaxiyun.player;

import android.content.Context;
import android.view.Surface;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/9/7
 * description：播放控制器抽象
 */
public interface IMediaPlayer {

    void init(Context context);

    long getDuration();

    long getCurrentPosition();

    boolean play();

    boolean open(String url);

    boolean pause();

    boolean stop();

    boolean isPlaying();

    boolean seekTo(long pos);

    boolean releasePlayer();

    void destroy();

    void updateProgress();

    void setEventListener(IPlayerEvents listener);

    /**
     * 可以考虑继承mediaPlayer的事件回调接口
     * */
    interface IPlayerEvents {
        boolean onError(IMediaPlayer player, int code, String msg);

        boolean onPrepared(IMediaPlayer player);

        boolean onSeekComplete(IMediaPlayer player, long pos);

        boolean onComplete(IMediaPlayer player);

        boolean onBuffering(IMediaPlayer player, boolean buffering, float percentage);

        boolean onProgress(IMediaPlayer player, long pos);

        void onDestroy(IMediaPlayer player);
    }
}
