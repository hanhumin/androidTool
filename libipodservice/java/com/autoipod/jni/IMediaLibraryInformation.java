package com.autoipod.jni;

import com.autoipod.data.MediaLibraryInformation;

import java.util.ArrayList;
import java.util.List;

public interface IMediaLibraryInformation {
    public void OnMediaLibrary(ArrayList<MediaLibraryInformation> list);
    /*
    public void OnMediaLibraryName(int len,byte[] utf8Data);
    public void OnMeidaLibraryUID(int len,byte[] utf8Data);
    public void OnMediaLibraryType(int type);
    */
}
