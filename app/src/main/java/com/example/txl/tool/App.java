package com.example.txl.tool;

import android.content.Context;
import android.util.DisplayMetrics;
import android.util.Log;

import androidx.multidex.MultiDexApplication;

import com.txl.lib.image_load.ImageLoader;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/6
 * description：
 */
public class App extends MultiDexApplication {
    private  static Context mContext;

    private static ImageLoader mImageLoader;

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
//        MultiDex.install(this);
//        Log.d("App","create 之地");
    }

    @Override
    public void onCreate() {
        super.onCreate();
        mContext = getApplicationContext();
        mImageLoader = ImageLoader.build( this );
        DisplayMetrics displayMetrics = getResources().getDisplayMetrics();
        Log.d("App"," density: "+ displayMetrics.density+" densityDpi "+ displayMetrics.densityDpi+" width "+displayMetrics.widthPixels+"  height "+displayMetrics.heightPixels);
    }

    public static Context getContext() {
        return mContext;
    }

    public static ImageLoader getImageLoader(){
        return mImageLoader;
    }
}
