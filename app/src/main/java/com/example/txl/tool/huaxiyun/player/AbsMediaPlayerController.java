package com.example.txl.tool.huaxiyun.player;

import android.os.Bundle;

import com.example.txl.tool.utils.AppExecutors;
import com.example.txl.tool.utils.CancellableRunnable;

public abstract class AbsMediaPlayerController implements IMediaPlayer.IPlayerEvents {
    protected BasePlayerAdapter _adapter;
    private CancellableRunnable _delayPlay;

    public AbsMediaPlayerController(BasePlayerAdapter _adapter) {
        this._adapter = _adapter;
    }

//    public UiComponentInfo[] getUiComponents() {
//        return defaultUiComponents;
//    }

    public IMediaPlayer getMediaPlayer() {
        return _adapter.getMediaPlayer();
    }

    public Object getDataDataUtils() {
        return _adapter.getDataUtils();
    }

    public void init(Bundle initParameters) {
    }

    public void stop() {
    }

    public void pause() {

    }

    public void destroy() {
    }

    public void resume() {

    }

    public boolean doTogglePlayPause() {
        if (!_adapter.isMediaRunning()) {
            return false;
        }

        IMediaPlayer mediaPlayer = getMediaPlayer();
        if (mediaPlayer.isPlaying()) {
            mediaPlayer.pause();
        } else {
            mediaPlayer.play();
        }
        _adapter.onPlayPaused(!mediaPlayer.isPlaying());
        return true;
    }

    public void doPlay(String playUrl) {
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
        new AppExecutors().mainThread().executeDelay(delayPlay,ms);
    }

    public long doSeek(float pos) {
        IMediaPlayer mediaPlayer = getMediaPlayer();
        long duration = mediaPlayer.getDuration();
        long seekTargetPos = percentageToOffset(pos, duration);
        mediaPlayer.seekTo(seekTargetPos);
        return seekTargetPos;
    }

    @Override
    public boolean onError(IMediaPlayer iMediaPlayer, int code, String msg) {
        _adapter.setMediaRunning(false);
        IMediaPlayer mediaPlayer = getMediaPlayer();
        mediaPlayer.stop();
        return false;
    }

    @Override
    public boolean onPrepared(IMediaPlayer iMediaPlayer) {

        return false;
    }

    @Override
    public boolean onSeekComplete(IMediaPlayer iMediaPlayer, long pos) {
        return false;
    }

    @Override
    public boolean onComplete(IMediaPlayer iMediaPlayer) {
        _adapter.onPlayPaused(!iMediaPlayer.isPlaying());
        _adapter.setMediaRunning(false);
        return false;
    }

    @Override
    public boolean onBuffering(IMediaPlayer iMediaPlayer, boolean buffering, float percentage) {
        return false;
    }

    @Override
    public boolean onProgress(IMediaPlayer iMediaPlayer, long pos) {
        IMediaPlayer mediaPlayer = getMediaPlayer();
        if (mediaPlayer == null) {
            return false;
        }
        long duration = mediaPlayer.getDuration();
        if (duration > 0) {
            _adapter.setProgress(offsetToPercent(pos, duration));
        }
        return false;
    }

    protected long percentageToOffset(float percent, long duration) {
        return (long) (percent * duration / 100.0f);
    }

    protected float offsetToPercent(long pos, long duration) {
        return 100.0f * pos / duration;
    }

    @Override
    public void onDestroy(IMediaPlayer iMediaPlayer) {
        _adapter.setMediaRunning(false);
    }
}
