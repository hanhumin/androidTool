package com.autoipod.jni;

public interface INowPlayingPlaybackQueueContents {
    public void onStart(String fileName);
    public void onFinish(String fileName);
}
