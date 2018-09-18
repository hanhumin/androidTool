package com.example.txl.tool.huaxiyun.player;

import android.content.Context;
import android.view.Surface;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/9/7
 * description：抽象播放器交互相关的方法
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
    interface IPlayerEvents<P> {
        boolean onError(P player, int code, String msg);

        boolean onPrepared(P player);

        boolean onSeekComplete(P player, long pos);

        boolean onComplete(P player);

        boolean onBuffering(P player, boolean buffering, float percentage);

        boolean onProgress(P player, long pos);

        void onDestroy(P player);
    }
}
