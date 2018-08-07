package com.example.txl.tool;

import android.app.Application;
import android.content.Context;

import com.example.txl.tool.utils.image.utils.ImageLoader;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/6
 * description：
 */
public class App extends Application {
    private  static Context mContext;

    private static ImageLoader mImageLoader;

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
