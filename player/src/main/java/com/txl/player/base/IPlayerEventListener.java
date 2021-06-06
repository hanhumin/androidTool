package com.txl.player.base;

/**
 * 参考MediaPlayer的播放器事件回调
 * */
public interface IPlayerEventListener {
    void onPrepared(IPlayer mp);

    void onCompletion(IPlayer mp);

    void onBufferingUpdate(IPlayer mp, int percent);

    void onSeekComplete(IPlayer mp);

    void onVideoSizeChanged(IPlayer mp, int width, int height);

    boolean onError(IPlayer mp, int what, int extra, String message);

    boolean onInfo(IPlayer mp, int what, int extra);
}
