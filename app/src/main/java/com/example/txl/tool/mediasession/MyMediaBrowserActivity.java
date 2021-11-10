package com.example.txl.tool.mediasession;

import androidx.appcompat.app.AppCompatActivity;

import android.app.ActivityManager;
import android.content.Context;
import android.media.browse.MediaBrowser;
import android.os.Bundle;
import android.util.Log;

import com.example.txl.tool.R;

import java.util.List;

public class MyMediaBrowserActivity extends AppCompatActivity {
    private final String TAG = MyMediaBrowserActivity.class.getSimpleName();
    private MediaBrowser mediaBrowser;
    private MediaBrowser.ConnectionCallback connectionCallback;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my_media_browser);
        connectionCallback = new MediaBrowser.ConnectionCallback(){
            @Override
            public void onConnected() {
                super.onConnected();
                Log.d(TAG,"onConnected");
            }

            @Override
            public void onConnectionSuspended() {
                super.onConnectionSuspended();
                Log.d(TAG,"onConnectionSuspended");
            }

            @Override
            public void onConnectionFailed() {
                super.onConnectionFailed();
                Log.d(TAG,"onConnectionSuspended");
            }
        };
        mediaBrowser = new MediaBrowser(this,this.getComponentName(),connectionCallback,new Bundle());
        mediaBrowser.connect();
//        ActivityManager manager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
//        List<ActivityManager.ProcessErrorStateInfo> processErrorStateInfos =  manager.getProcessesInErrorState();
//        Log.d(TAG,"info");
//        Log.d(TAG,processErrorStateInfos.toString());
    }
}