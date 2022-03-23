package com.example.txl.tool.partition.storage;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;

import com.example.txl.tool.R;

import java.util.Arrays;

/**
 * 查看缓存的文件路径
 * */
public class StorageDemoActivity extends AppCompatActivity {
    private final String TAG = "StorageDemoActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_storage_demo);
        printInternalCache();
        printExternalCache();
    }

    private void printInternalCache(){
        Log.d(TAG,"filesDir : "+getFilesDir());
        Log.d(TAG,"cacheDir : "+getCacheDir());
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            Log.d(TAG,"dataDir : "+getDataDir());
        }
        Log.d(TAG,"obbDir : "+getObbDir());
    }


    private void printExternalCache(){
        Log.d(TAG,"extCacheDir : "+getExternalCacheDir());
        Log.d(TAG,"extCacheDirs : "+ Arrays.toString(getExternalCacheDirs()));
        Log.d(TAG,"extCacheDirs : "+ Arrays.toString(getExternalMediaDirs()));
        Log.d(TAG,"extCacheDirs : "+ getExternalFilesDir(Environment.DIRECTORY_MUSIC));
        Log.d(TAG,"extCacheDirs : "+ getExternalFilesDir(Environment.DIRECTORY_PICTURES));

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            Log.d(TAG,"ext dataDir : "+ Environment.getDataDirectory());
        }
        Log.d(TAG,"ExternalStorageDirectory : "+Environment.getExternalStorageDirectory());
        Log.d(TAG,"getExternalStoragePublicDirectory : "+Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MUSIC));
        Log.d(TAG,"getExternalStoragePublicDirectory : "+Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES));
        Log.d(TAG,"getRootDirectory : "+Environment.getRootDirectory());
    }
}