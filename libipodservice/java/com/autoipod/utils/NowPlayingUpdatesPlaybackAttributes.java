package com.autoipod.utils;

import android.util.Log;

import com.autoipod.jni.IPlaybackAttributes;

import java.io.PrintWriter;
import java.io.StringWriter;

public class NowPlayingUpdatesPlaybackAttributes implements IPlaybackAttributes{
    static final String TAG = "PlayStatus";

    @Override
    public void OnPlaybackAttributesRawData(int datalen, byte[] data) {

    }

    @Override
    public void onPlayBackPosition(int position) {

    }

    @Override
    public void onPlayBackStatus(int status) {

    }

    @Override
    public void onPlayBackQueueIndex(int index) {


    }

    @Override
    public void onPlayBackQueueCount(int count) {

    }

    @Override
    public void onPlayBackQueueChapterIndex(int index) {
    }

    @Override
    public void OnPlayBackShuffleMode(int mode) {

    }

    @Override
    public void OnPlayBackRepeatMode(int mode) {

    }

    @Override
    public void OnPlayBackAppName(int len, String name) {

    }

    @Override
    public void OnPBMediaLibraryUID(int len, String mediaLibraryUid) {

    }

    @Override
    public void OnPBAppleMusicRadioAd(boolean isRadioAd) {

    }

    @Override
    public void OnPBAppleMusicRadioStationName(int len, String name) {

    }

    @Override
    public void OnPBAppleMusicRadioStationMediaPlaylistID(int datalen, byte[] data) {

    }

    @Override
    public void OnPlayBackSpeed(int speed) {

    }

    @Override
    public void OnSetElapsedTimeAvailable(boolean isSet) {

    }

    @Override
    public void OnPlayBackQueueListAvail(boolean isAvail) {

    }

    @Override
    public void OnPlayBackQueueListTransferID(int id) {

    }

    @Override
    public void onPlayBackBundleID(int len, String  id) {

    }

    @Override
    public void OnPlayBackQueueListContentTransfer() {

    }
}
