package com.autoipod.jni;

public interface IMediaItemNowPlayingPlaybackQueueContents {
    public void onStart(String fileName);
    public void onFinish(String fileName);
}
