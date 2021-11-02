package com.autoipod.data;



public class MediaPlaylist {
    final static String TAG = "MediaPlaylist";

    final static int MSG_MEDIA_PLAYLIST_PID = 0;
    final static int MSG_MEDIA_PLAYLIST_NAME = 1;
    final static int MSG_MEDIA_PLAYLIST_PARENT_PID = 2;
    final static int MSG_MEDIA_PLAYLIST_GENIUS_MIX= 3;
    final static int MSG_MEDIA_PLAYLIST_ISFOLDER = 4;
    final static int MSG_MEDIA_PLAYLIST_CONTAINED_MEIDA_ITEMS_FT_ID= 5;
    final static int MSG_MEDIA_PLAYLIST_IS_APPLE_MUSIC_RADIO_STATION = 6;
    final static int MSG_MEDIA_PLAYLIST_CONTENT_TRANSFER = 7;


    public  static final String  MEDIA_PLAYLIST_TABLE_COLUME_PID = "pid";
    public  static final String  MEDIA_PLAYLIST_TABLE_COLUME_NAME = "name";
    public  static final String  MEDIA_PLAYLIST_TABLE_COLUME_PARENT_PID = "parent_pid";
    public  static final String  MEDIA_PLAYLIST_TABLE_COLUME_GENIUS_MIX = "genius_mix";
    public  static final String  MEDIA_PLAYLIST_TABLE_COLUME_ISFOLDER = "isfolder";
    public  static final String  MEDIA_PLAYLIST_TABLE_COLUME_FILE_TRANSFER_PID = "file_transfer_id";
    public  static final String  MEDIA_PLAYLIST_TABLE_COLUME_IS_RADIO_STATION = "is_radio_station";
    public  static final String  MEDIA_PLAYLIST_TABLE_COLUME_EXIST_ATTRIBUTE = "exist_attribute";

    int mExistAttributeByte = 0x00;

    byte[] mPid;
    String mName;
    byte[] mParentPid;
    boolean isGeniusMix;
    boolean isFolder;
    int containedMeidaItemFileTransferID = -1;
    boolean isAppMusicRadioStation;
    String mMediaLibraryUid;

    public void setMediaLibraryUid(String uid)
    {
        mMediaLibraryUid = uid;
    }
    public String getMediaLibraryUid()
    {
        return mMediaLibraryUid ;
    }
    public void setAppMusicRadioStation(boolean status)
    {
        isAppMusicRadioStation = status;
        setExistAttribute(MSG_MEDIA_PLAYLIST_IS_APPLE_MUSIC_RADIO_STATION);

    }
    public boolean getAppMusicRadioStation()
    {
        return  isAppMusicRadioStation;
    }
    public void setFileTransferID(int id)
    {
        containedMeidaItemFileTransferID = id;
        setExistAttribute(MSG_MEDIA_PLAYLIST_CONTAINED_MEIDA_ITEMS_FT_ID);
    }
    public int getFileTransferID()
    {
        return containedMeidaItemFileTransferID;
    }
    public void setFolder(boolean status)
    {
        isFolder = status;
        setExistAttribute(MSG_MEDIA_PLAYLIST_ISFOLDER);
    }
    public boolean getFolder()
    {
        return isFolder;
    }
    public void setGeniusMix(boolean status)
    {
        isGeniusMix = status;
        setExistAttribute(MSG_MEDIA_PLAYLIST_GENIUS_MIX);
    }
    public boolean getGeniusMix()
    {
        return isGeniusMix;
    }

    public void setParentPid(byte[] pid)
    {
        mParentPid = pid;
        setExistAttribute(MSG_MEDIA_PLAYLIST_PARENT_PID);
    }
    public byte[] getParentPid()
    {
        return mParentPid;
    }
    public void setName(String name)
    {
        mName = name;
        setExistAttribute(MSG_MEDIA_PLAYLIST_NAME);
    }
    public String getName()
    {
        return mName;
    }
    public void setPid(byte[] pid)
    {
        mPid = pid;
        setExistAttribute(MSG_MEDIA_PLAYLIST_PID);
    }
    public byte[] getPid()
    {
       return mPid;
    }

    public void setExistAttributeByte(int existAttributeByte)
    {
        mExistAttributeByte = existAttributeByte;
    }
    public int getExistAttributeByte()
    {
        return mExistAttributeByte;
    }

    public void setExistAttribute(int item)
    {
        int i = 0x01;
        i= i << item;
        mExistAttributeByte = mExistAttributeByte | i;
    }

    public boolean isExistAttribute(int item)
    {
        int i = 0x01;
        i= i << item;
        mExistAttributeByte = mExistAttributeByte & i;
        if(mExistAttributeByte==0)
            return false;
        return  true;
    }



}

