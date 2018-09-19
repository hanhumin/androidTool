package com.example.txl.tool.huaxiyun.player;

import android.os.Bundle;

import com.example.txl.tool.utils.AppExecutors;
import com.example.txl.tool.utils.CancellableRunnable;

public abstract class AbsMediaPlayerController<P> implements IMediaPlayer.IPlayerEvents<P> {
    protected BasePlayerAdapter _adapter;
    private CancellableRunnable _delayPlay;

    public AbsMediaPlayerController(BasePlayerAdapter _adapter) {
        this._adapter = _adapter;
    }


    public Object getDataDataUtils() {
        return _adapter.getDataUtils();
    }

    /**
     * init params ready to play
     * */
    public void init(Bundle initParameters) {
    }

    public void stop() {
        _adapter.stop();
    }

    public void pause() {
        _adapter.pause();
    }

    public void destroy() {
    }

    public void resume() {

    }

    public boolean doTogglePlayPause() {
        if (!_adapter.isMediaRunning()) {
            return false;
        }
        _adapter.onPlayPaused(!_adapter.isPlaying());
        return true;
    }

    public void doPlay(String playUrl) {
        _adapter.open(playUrl);
    }

    public void delayDoPlay(final String playUrl, int ms) {
        CancellableRunnable delayPlay = _delayPlay;
        if (delayPlay != null) {
            delayPlay.cancel();
        }
        _delayPlay = new CancellableRunnable() {
            @Override
            protected void doRun() {
                doPlay(playUrl);
            }
        };
        // FIXME: 2018/9/8 AppExecutors 全局单例
        new AppExecutors().mainThread().executeDelay(_delayPlay,ms);
    }

    public long doSeek(float pos) {
        long duration = _adapter.getDuration();
        long seekTargetPos = percentageToOffset(pos, duration);
        _adapter.seekTo(seekTargetPos);
        return seekTargetPos;
    }

    @Override
    public boolean onError(P player, int code, String msg) {
        return false;
    }

    @Override
    public boolean onPrepared(P player) {
        _adapter.setMediaRunning(true);
        return false;
    }

    @Override
    public boolean onSeekComplete(P player, long pos) {
        return false;
    }

    @Override
    public boolean onComplete(P player) {
        _adapter.setMediaRunning(false);
        return false;
    }

    @Override
    public boolean onBuffering(P player, boolean buffering, float percentage) {
        return false;
    }

    @Override
    public boolean onProgress(P player, long pos) {
        return false;
    }

    protected long percentageToOffset(float percent, long duration) {
        return (long) (percent * duration / 100.0f);
    }

    protected float offsetToPercent(long pos, long duration) {
        return 100.0f * pos / duration;
    }

    @Override
    public void onDestroy(P player) {
        _adapter.setMediaRunning(false);
    }
}
