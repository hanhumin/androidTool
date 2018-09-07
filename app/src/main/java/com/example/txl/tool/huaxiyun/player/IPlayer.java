package com.example.txl.tool.huaxiyun.player;

import android.view.Surface;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/9/7
 * description：播放控制器抽象
 */
public interface IPlayer {

    long getDuration();

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

    void setSurface( Surface surface );


    interface IPlayerEvents {
        boolean onError(IPlayer player, int code, String msg);

        boolean onPrepared(IPlayer player);

        boolean onSeekComplete(IPlayer player, long pos);

        boolean onComplete(IPlayer player);

        boolean onBuffering(IPlayer player, boolean buffering, float percentage);

        boolean onProgress(IPlayer player, long pos);

        void onDestroy(IPlayer player);
    }
}
