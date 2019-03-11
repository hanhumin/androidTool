package com.example.txl.tool.player.music;

/**
 * @author TXL
 * description :
 */
public interface IMusicPlayer {
    void init();

    /**
     * 是否显示notification
     * */
    void startNotification(boolean has);

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
     * description :音频播放通过Notification操作需要及时更改界面
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
