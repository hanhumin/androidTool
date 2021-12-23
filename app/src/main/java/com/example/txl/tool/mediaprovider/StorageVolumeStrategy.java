package com.example.txl.tool.mediaprovider;

public class StorageVolumeStrategy {
    private static final String internal = "internal";
    private static final String external = "external";

    private static boolean isInternal = true;

    public static String getVolume(){
        return isInternal?internal:external;
    }

    public static void updateInternal(boolean isInternal){
        StorageVolumeStrategy.isInternal = isInternal;
    }
}
