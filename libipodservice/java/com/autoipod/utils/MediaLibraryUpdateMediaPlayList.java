package com.autoipod.utils;

import android.util.Log;

import com.autoipod.jni.IMediaPlayList;
import java.io.PrintWriter;
import java.io.StringWriter;

public class MediaLibraryUpdateMediaPlayList implements IMediaPlayList {
    final  static String TAG = "MediaLibraryUpdateMediaPlayList";
    @Override
    public void OnMediaLibraryUpdatePlaylistID(int len, byte[] pidArray) {
        Log.i(TAG,"OnMediaLibraryUpdatePlaylistID");

    }

    @Override
    public void OnMediaLibraryUpdatePlaylistName(int len, String name) {
        try {
            Log.i(TAG, "OnMediaLibraryUpdatePlaylistName :" + name);
        }catch (Exception e){
            printErrorInfoFromException("OnMediaLibraryUpdatePlaylistName",e);
        }
    }



    @Override
    public void OnMediaLibraryUpdateParentPlaylistID(int len, byte[] pidArray) {
        Log.i(TAG,"OnMediaLibraryUpdateParentPlaylistID");

    }

    @Override
    public void OnMediaUpdatePlaylistIsGeniusMix(boolean isGeniusMix) {
        Log.i(TAG,"OnMediaUpdatePlaylistIsGeniusMix isGeniusMix:"+isGeniusMix);
    }

    @Override
    public void OnMediaUpdatePlaylistIsFolder(boolean isFolder) {
        Log.i(TAG,"OnMediaUpdatePlaylistIsFolder isFolder:"+isFolder);
    }

    @Override
    public void OnMediaUpdatePlaylistIsFolder(int id) {
        Log.i(TAG,"OnMediaUpdatePlaylistIsFolder id:"+id);
    }

    @Override
    public void OnMediaUpdatePlaylistIsRadioStation(boolean isRadioStation) {
        Log.i(TAG,"OnMediaUpdatePlaylistIsRadioStation isRadioStation:"+isRadioStation);
    }

    @Override
    public void OnMediaUpdatePlaylistContentTransfer() {
        Log.i(TAG,"OnMediaUpdatePlaylistContentTransfer :");
    }

    public void printErrorInfoFromException(String tag,Exception e) {
        try {
            StringWriter sw = new StringWriter();
            PrintWriter pw = new PrintWriter(sw);
            e.printStackTrace(pw);
            Log.i(TAG,tag+": "+sw.toString());
            //  return "\r\n" + sw.toString() + "\r\n";
            sw.close();
            pw.close();
        } catch (Exception e2) {
            e2.printStackTrace();
            //  return "ErrorInfoFromException";
        }
    }
}
