package com.example.txl.tool.huaxiyun.player;

import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/9/8
 * description：
 */
public abstract class BasePlayerAdapter<P> implements  IMediaPlayer {

    protected P player;
    /**
     * 定义相关的要展示的ui
     * */
    public static final String UI_CONTROL_BAR = "ControlBar";

    protected boolean _paused = false;
    private boolean _isMediaRunning = false;

    public BasePlayerAdapter(P player) {
        this.player = player;
    }

    public boolean isMediaRunning() {
        return _isMediaRunning;
    }

    public void setMediaRunning(boolean running) {
        _isMediaRunning = running;
        _paused = false;
    }

    public boolean isPaused() {
        return _paused;
    }

    public abstract void showUI(String componentId, boolean show);

    public P getPlayer(){
        return player;
    }

    /**
     * 请求数据工具
     * */
    public abstract  Object getDataUtils();

    public abstract AbsBasePlayerUiSwitcher getUiSwitcher();

    public abstract AbsMediaPlayerController getController();

    public abstract View getUiComponent(String name);

    public abstract void setProgress(float pos);

    public abstract boolean dispatchSeekControllerEvents(KeyEvent event);

    public abstract void onPlayPaused(boolean paused);

    public abstract void destroy();

    public abstract String makePlayUrl(String playUrl, Bundle data);

    public abstract String getAspectRatio();

    public abstract void setAspectRatio(String ratio);

    public abstract View getPlayerView();

}
