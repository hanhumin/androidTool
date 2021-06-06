package com.txl.player.base;

/**
 * 播放监听用在PlayerView中 <br>
 * <p>
 * <b>date:</b> 2020/10/23 16:52 <br>
 * <b>author:</b> Jasonzhou <br>
 * <b>version:</b> 1.0 <br>
 * <b>last modified date:</b> 2020/10/23 16:52.
 */
public interface OnPlayerListener {

    /**
     * 视频开始播放的时候会回调这个方法
     */
    void onVideoStart();

    /**
     * 视频正在播放的时候会回调这个方法
     *
     * @param percent     播放位置的百分比
     * @param currentTime 当前播放到的时间
     * @param totalTime   视频总长度
     */
    void onVideoPlaying(float percent, long currentTime, long totalTime);

    /**
     * 视频暂停播放的时候会回调这个方法
     */
    void onVideoPause();

    /**
     * 快退或快进到指定位置
     *
     * @param forward      true:快进，false:快退
     * @param seekPosition 指定的位置
     */
    void onSeek(boolean forward, long seekPosition);
}
