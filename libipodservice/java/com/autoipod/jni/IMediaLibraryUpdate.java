package com.autoipod.jni;

import com.autoipod.data.MediaItem;
import com.autoipod.data.MediaPlaylist;

public interface IMediaLibraryUpdate {
    public void OnMediaLibraryUpdateUID(int len,String uidStr,byte[] byteArryUid);
    public void OnMediaLibraryUpdateRevision(int len,String revision,int uidLen,String mediaLibraryUid,byte[] byteArryUid);
    public void OnMediaItemDeleteUpdatePID(int datalen,byte data[],int uidLen,String mediaLibraryUid);
    public void OnMediaPlayListDeleteUpdatePID(int datalen,byte data[],int uidLen,String mediaLibraryUid);
    public void OnPlayAllSongsCapable(boolean capable,int uidLen,String mediaLibraryUid,byte[] byteArryUid);
    public void OnIsHidingRemoteItems(boolean isHiding,int uidLen,String mediaLibraryUid,byte[] byteArryUid);
    public void OnMediaLibraryUpdateProgress(int progress,int uidLen,String mediaLibraryUid,byte[] byteArryUid);
    public void OnMediaLibraryReset(int uidLen,String mediaLibraryUid);

    public void  onMediaItem(MediaItem item, int uidLen, String mediaLibraryUid,byte[] byteArryUid);
    public void  onMediaPlaylist(MediaPlaylist playlist, int uidLen, String mediaLibraryUid,byte[] byteArryUid);
}
