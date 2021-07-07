package com.example.txl.tool.service;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

import java.io.FileDescriptor;
import java.io.PrintWriter;

public class DemoService extends Service {
    private static final String TAG = "DemoService";


    public DemoService() {
        Log.d(TAG,"structure");
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG,"onCreate");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(TAG,"onStartCommand");
        return super.onStartCommand(intent, flags, startId);
    }

    private final Binder binder = new Binder();

    @Override
    public IBinder onBind(Intent intent) {
        Log.d(TAG,"onBind");
        return binder;
    }

    @Override
    public void onRebind(Intent intent) {//所有连接对象全部都解绑，且onUnbind返回true
        Log.d(TAG,"onRebind");
        super.onRebind(intent);
    }

    @Override
    public boolean onUnbind(Intent intent) {
        Log.d(TAG,"onUnbind");
//        return super.onUnbind(intent);
        return true;
    }

//    @Override
//    public void onTaskRemoved(Intent rootIntent) {
//        Log.d(TAG,"onTaskRemoved");
//        super.onTaskRemoved(rootIntent);
//    }

    @Override
    public void onDestroy() {
        Log.d(TAG,"onDestroy");
        super.onDestroy();
    }

    @Override
    protected void dump(FileDescriptor fd, PrintWriter writer, String[] args) {
        super.dump(fd, writer, args);
    }
}