package com.autoipod.utils;

import android.os.Handler;

import com.autoipod.jni.IPlaylistContents;

public class MediaItemListUpdatePlaylistContents implements IPlaylistContents {
    Handler mHandler;
    public MediaItemListUpdatePlaylistContents(Handler handler)
    {
        mHandler = handler;
    }
    @Override
    public void onStart(String fileName, int pidLen, byte[] pid, int uidLen, String mediaLibraryUid) {

    }

    @Override
    public void onFinish(String fileName, int pidLen, byte[] pid, int uidLen,String mediaLibraryUid) {

    }
}
