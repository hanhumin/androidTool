package com.example.txl.tool;

import android.content.Context;
import android.util.DisplayMetrics;
import android.util.Log;

import androidx.multidex.MultiDexApplication;

//import com.squareup.leakcanary.LeakCanary;
//import com.squareup.leakcanary.RefWatcher;
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
//    private RefWatcher refWatcher;

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
//        refWatcher = setupRefWatch();
    }

    public static Context getContext() {
        return mContext;
    }

    public static ImageLoader getImageLoader(){
        return mImageLoader;
    }

//    private RefWatcher setupRefWatch(){
//        if(LeakCanary.isInAnalyzerProcess(this)){
//            return RefWatcher.DISABLED;
//        }
//        return LeakCanary.install(this);
//    }
//
//    public static RefWatcher getRefWatch(Context context){
//        App app = (App) context.getApplicationContext();
//        return app.refWatcher;
//    }
}
