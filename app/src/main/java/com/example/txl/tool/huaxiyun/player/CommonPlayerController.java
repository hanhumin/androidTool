package com.example.txl.tool.huaxiyun.player;


import android.os.Bundle;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/9/9
 * description：
 */
public class CommonPlayerController extends AbsMediaPlayerController<HuaXiYunSimplePlayer> {

    public CommonPlayerController(BasePlayerAdapter _adapter) {
        super( _adapter );
    }

    @Override
    public Object getDataDataUtils() {
        return super.getDataDataUtils();
    }

    @Override
    public void init(Bundle initParameters) {
        super.init(initParameters);
        doPlay("https://aweme.snssdk.com/aweme/v1/playwm/?video_id=v0200ff50000bd0p9ur6936mllnbeo40&line=0");
    }

    @Override
    public void stop() {
        super.stop();
    }

    @Override
    public void pause() {
        super.pause();
    }

    @Override
    public void destroy() {
        super.destroy();
    }

    @Override
    public void resume() {
        super.resume();
    }

    @Override
    public boolean doTogglePlayPause() {
        return super.doTogglePlayPause();
    }

    @Override
    public void doPlay(String playUrl) {
        super.doPlay(playUrl);
    }

    @Override
    public void delayDoPlay(String playUrl, int ms) {
        super.delayDoPlay(playUrl, ms);
    }

    @Override
    public long doSeek(float pos) {
        return super.doSeek(pos);
    }

    @Override
    public boolean onError(HuaXiYunSimplePlayer player, int code, String msg) {
        return super.onError(player, code, msg);
    }

    @Override
    public boolean onPrepared(HuaXiYunSimplePlayer player) {
        _adapter.play();
        return super.onPrepared(player);
    }

    @Override
    public boolean onSeekComplete(HuaXiYunSimplePlayer player, long pos) {
        return super.onSeekComplete(player, pos);
    }

    @Override
    public boolean onComplete(HuaXiYunSimplePlayer player) {
        _adapter.getUiSwitcher().context = null;
        return super.onComplete(player);
    }

    @Override
    public boolean onBuffering(HuaXiYunSimplePlayer player, boolean buffering, float percentage) {
        return super.onBuffering(player, buffering, percentage);
    }

    @Override
    public boolean onProgress(HuaXiYunSimplePlayer player, long pos) {
        return super.onProgress(player, pos);
    }

    @Override
    protected long percentageToOffset(float percent, long duration) {
        return super.percentageToOffset(percent, duration);
    }

    @Override
    protected float offsetToPercent(long pos, long duration) {
        return super.offsetToPercent(pos, duration);
    }

    @Override
    public void onDestroy(HuaXiYunSimplePlayer player) {
        super.onDestroy(player);
    }
}
