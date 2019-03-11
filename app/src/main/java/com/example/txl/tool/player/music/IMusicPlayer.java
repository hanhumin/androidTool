package com.example.txl.tool.player.music;

/**
 * @author TXL
 * description :
 */
public interface IMusicPlayer<T> {
    void init();

    long getCurrentPosition();

    boolean seekTo(long pos);

    boolean stop();

    boolean pause();

    boolean play();

    boolean open(String url);

    void destroy();

    void setEventListener(IMusicPlayerEvents listener);

    void removeEventListener(IMusicPlayerEvents listener);

    boolean isPlaying();

    /**
     * 返回判断是不是同一音频的标记
     * */
    T getPlayTag();

    void setPlayTag(T tag);

    /**
     * description :音频播放需要通过Notification操作需要及时更改界面
     */
    interface IMusicPlayerEvents {
        boolean onError(IMusicPlayer xmp, int code, String msg);

        boolean onPrepared(IMusicPlayer player);

        boolean onSeekComplete(IMusicPlayer player, long pos);

        boolean onComplete(IMusicPlayer player);

        boolean onBuffering(IMusicPlayer player, boolean buffering, float percentage);

        boolean onProgress(IMusicPlayer player, long pos);

        void onDestroy(IMusicPlayer player);

        boolean onPlay(IMusicPlayer player);

        boolean onPause(IMusicPlayer player);

        boolean onStop(IMusicPlayer player);
    }
}
