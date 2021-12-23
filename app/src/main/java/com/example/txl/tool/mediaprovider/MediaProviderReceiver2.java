package com.example.txl.tool.mediaprovider;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;

/**
 * MediaProviderReceiver 为静态注册
 * MediaProviderReceiver2 动态注册测试
 * */
public class MediaProviderReceiver2 extends BroadcastReceiver {
    private final String TAG = "MediaProviderReceiver2";
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
