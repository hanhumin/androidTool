package com.autoipod.jni;

public class Ipod {

    private static final String TAG = "IpodLib";
    private static Ipod mIpod;
    static {
        System.loadLibrary("ipod");
    }

    private Ipod() {
    }

    public static Ipod getInstance() {
        if(mIpod == null) {
            mIpod = new Ipod();
        }
        return mIpod;
    }

    public  native void startIpod();//启动ipod
    public  native void stopIpod();
    public  native void notifyMicRecord(int satus);
    public  native void stopUSBDeviceModeAudio();
    public native  void playPause(int mod);//0,stop,1 play,2,pause
    public native  void play();//0,stop,1 play,2,pause
    public native  void pause();//0,stop,1 play,2,pause
    public native  void playNextTrack();
    public native  void playProvTrack();
    public native void random();//shuffle
    public native void repeat();//repeat
    public native void send0Hid();

    public  native void  registerAudioInformationCallBack(IAudioInformationCallBack callBack);
    public  native void  reigsterOnPcmDataReceiveCallBack(IPcmDataReceiveCallBack callBack);
    public native void   registerNowPlayingPlaybackAttributesCallback(IPlaybackAttributes callback);
    public native void   registerNowPlayingMediaItemAttributesCallback(IMediaItemAttributes callback);
    public native void   registerMediaLibraryInformationCallback(IMediaLibraryInformation callback);
    public native void   registerMediaLibraryUpdateCallback(IMediaLibraryUpdate callback);
    public native void   registerMediaLibraryUpdatePlayListCallback(IMediaPlayList callback);
    public native void   registerMediaNowPlayingArtworkDataCallback(INowPlayingArtwork callback);
    public native void   registerNowPlayingPlaybackQueueContentsCallback(INowPlayingPlaybackQueueContents callback);
    public native void   registerMediaLibraryUpdatePlaylistContentsCallback(IPlaylistContents callback);
    public native void   registerMediaItemNowPlayingPlaybackQueueContentsCallback(IMediaItemNowPlayingPlaybackQueueContents callback);
    public native void   registerMediaItemListUpdatePlaylistContentsCallback(IPlaylistContents callback);

    public native  void iAP2StartNowPlayingUpdates();
    public native  void iAP2StopNowPlayingUpdates();

    public native void   iAP2StartMediaLibraryInformation();
    public native void   iAP2StopMediaLibraryInformation();

    //同步媒体库
    public native void   iAP2StartMediaLibraryUpdates(int uid_size,byte[] uid,int revision_size,byte[] revision);
    public native void   iAP2StopMediaLibraryUpdates(int uid_size,byte[] uid);//每次调用只能停止一个，停止多个，需要多次调用
    public native void   iAP2PlayMediaLibraryCurrentSelection(int uid_size,byte[] uid);//每次调用只能停止一个，需要多次调用

    //播放媒体库中的一种类型的歌曲
    public native void   iAP2PlayMediaLibraryCollection(int pid_size,byte[]pid,int type,int index,int mediaLibraryUidSize,
                                                        byte[]mediaLibraryUid,int startMediaItemPidSize,byte[]startMediaItemPid);
    public native void   iAP2PlayMediaLibrarySpecial(int uidSize,byte[] uid,boolean isPlayAll,int startMediaItemPidSize,byte[]startMediaItemPid);
    //播放mediaitem
    public native void   iAP2PlayMediaLibraryItems(int itemsPidsSize,byte[] itemsPids,int itemStartIndex,int mediaLibraryUidSize,byte[] mediaLibraryUid);
    public native void   registerIpodStartCallback(IIpodStart callback);

    public  native int  getIpodStatus();
    public  native void setIpodStatus(int status);
    public  native int  getIpodPluginStatus();
    public  native void setIpodPluginStatus(int status);

    //设置播放进度
    public  native void setIpodPlayPosition(int position);
    public  native void iAP2SetPQListContentTransferStartIndex(int playbackQueueIndex);//PlaybackQueueListContentTransferStartIndex


}
