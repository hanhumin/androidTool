package com.autoipod.jni;

public interface IMediaPlayList {
    public void OnMediaLibraryUpdatePlaylistID(int len,byte[] pidArray);
    public void OnMediaLibraryUpdatePlaylistName(int len,String name);
    public void OnMediaLibraryUpdateParentPlaylistID(int len,byte[] utf8Data);
    public void OnMediaUpdatePlaylistIsGeniusMix(boolean isGeniusMix);
    public void OnMediaUpdatePlaylistIsFolder(boolean isFolder);
    public void OnMediaUpdatePlaylistIsFolder(int id);
    public void OnMediaUpdatePlaylistIsRadioStation(boolean isRadioStation);
    public void OnMediaUpdatePlaylistContentTransfer();
}
