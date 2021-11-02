package com.autoipod.jni;

public interface IPlaylistContents {
    public void onStart(String fileName,int pidLen,byte[] pid,int uidLen, String mediaLibraryUid);
    public void onFinish(String fileName,int pidLen,byte[] pid,int uidLen, String mediaLibraryUid);
}

