package com.autoipod.jni;

public interface IPlaybackAttributes {
    public void OnPlaybackAttributesRawData(int datalen,byte data[]);
    public void onPlayBackPosition(int position);
    public void onPlayBackStatus(int position);
    public void onPlayBackQueueIndex(int index);
    public void onPlayBackQueueCount(int count);
    public void onPlayBackQueueChapterIndex(int index);
    public void OnPlayBackShuffleMode(int mode);
    public void OnPlayBackRepeatMode(int mode);
    public void OnPlayBackAppName(int len,String name);
    public void OnPBMediaLibraryUID(int len,String mediaLibraryUid);
    public void OnPBAppleMusicRadioAd(boolean isRadioAd);
    public void OnPBAppleMusicRadioStationName(int len,String name);
    public void OnPBAppleMusicRadioStationMediaPlaylistID(int datalen,byte data[]);
    public void OnPlayBackSpeed(int speed);
    public void OnSetElapsedTimeAvailable(boolean isSet);
    public void OnPlayBackQueueListAvail(boolean isAvail);
    public void OnPlayBackQueueListTransferID(int id);
    public void onPlayBackBundleID(int len,String id);
    public void OnPlayBackQueueListContentTransfer();


}
