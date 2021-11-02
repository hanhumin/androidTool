package com.autoipod.utils;

import android.os.Handler;
import android.util.Log;

import com.autoipod.jni.IMediaItemNowPlayingPlaybackQueueContents;
import com.autoipod.jni.INowPlayingPlaybackQueueContents;

public class MediaItemNowPlayingPlaybackQueueContents implements IMediaItemNowPlayingPlaybackQueueContents {
    Handler mHandler;
    public MediaItemNowPlayingPlaybackQueueContents(Handler handler)
    {
        mHandler = handler;
    }
    @Override
    public void onStart(String fileName) {
        Log.i("MediaItemNowPlayingPlaybackQueueContents","onStart filename = "+fileName);

    }

    @Override
    public void onFinish(String fileName) {
        //"/storage/emulated/0/Download/ipod/"
                Log.i("MediaItemNowPlayingPlaybackQueueContents","onFinish filename = "+fileName);

    }
}
