package com.example.txl.tool.mediaprovider;

import android.content.BroadcastReceiver;
import android.net.Uri;
import android.util.Log;
import android.content.Intent;
import android.content.Context;

public class MediaProviderReceiver extends BroadcastReceiver {
    private final String TAG = "MediaProviderReceiver";
    @Override
    public void onReceive(Context context, Intent intent) {
        final String action = intent.getAction();
        if("com.hase.test.lunchself".equals(action)){
            Log.d(TAG,"lunch self test");
            return;
        }
        final Uri uri = intent.getData();
        Log.d(TAG, "action: " + action + "uri " + uri);
    }
}
