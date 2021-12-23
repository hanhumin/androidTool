package com.example.txl.tool.mediaprovider;

public class MediaInfo {
    private long id;
    private String displayName;
    private String path;
    private boolean isDir;

    public MediaInfo(long id, String displayName, String path,boolean isDir) {
        this.id = id;
        this.displayName = displayName;
        this.path = path;
        this.isDir = isDir;
    }

    public long getId() {
        return id;
    }

    public String getDisplayName() {
        return displayName;
    }

    public String getPath() {
        return path;
    }

    public boolean isDir() {
        return isDir;
    }
}
