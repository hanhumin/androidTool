package com.txl.player.controller;

import android.view.KeyEvent;

import androidx.lifecycle.Lifecycle;

public interface IPlayerController {
    void play();
    void playByUrl(String playUrl);
    void start();
    void pause();

    void togglePlayer();

    /**
     * @param forward 向前
     * @param offsetMs 偏移时间 单位 ms
     * */
    void seek(boolean forward,long offsetMs);

    /**
     * 下一个
     * */
    void playNext();
    /**
     * 上一个
     * */
    void playPre();

    boolean dispatchKeyEvent(KeyEvent event);

    /**
     * 不在使用，销毁播放器
     * */
    void destroy();

    void attachLifecycle(Lifecycle lifecycle);
}
