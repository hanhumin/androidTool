package com.autoipod.data;

public class MediaLibraryInformation {
    public String mediaLibraryName;
    public String mediaLibraryUID;
    public int    mediaLibraryType;//0 local device library;2 Apple music radio library
    public String getMediaLibraryName()
    {
        return mediaLibraryName;
    }
    public String getMediaLibraryUID()
    {
        return mediaLibraryUID;
    }
    public int getMediaLibraryType()
    {
        return mediaLibraryType;
    }
}
