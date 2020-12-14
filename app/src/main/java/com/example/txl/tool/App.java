package com.example.txl.tool;

import android.app.Application;
import android.content.Context;
import android.util.Log;

import androidx.multidex.MultiDex;
import androidx.multidex.MultiDexApplication;

import com.tencent.lbssearch.object.param.SearchParam;
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
        MultiDex.install(this);
        new SearchParam("北京", new SearchParam.Region("北京"));
        Log.d("App","create 之地");
    }

    @Override
    public void onCreate() {
        super.onCreate();
        mContext = getApplicationContext();
        mImageLoader = ImageLoader.build( this );
    }

    public static Context getContext() {
        return mContext;
    }

    public static ImageLoader getImageLoader(){
        return mImageLoader;
    }
}
