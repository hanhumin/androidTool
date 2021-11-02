package com.autoipod;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import com.autoipod.jni.Ipod;

import static com.autoipod.IPodDevice.IPOD_DEV_NOT_SYN;

public class IPodService extends Service {
    IPodService mIPodService;
    Context mContext;
    LocalBroadcastManager mLocalBroadcastManager = null;
    @Override
    public void onCreate()
    {
        super.onCreate();
      
        mIPodService = this;
        mContext = this;
        mLocalBroadcastManager = LocalBroadcastManager.getInstance(this);
    }
   
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        IPodDevice iPodDevice= IPodDeviceMannager.getActiveDevice();
        if(iPodDevice!=null && iPodDevice.getDeviceSynState()== IPOD_DEV_NOT_SYN)
        {
            Log.d("MP","start syn IPOD ooo 2222");

            new Thread(new Runnable() {
                @Override
                public void run() {
                    mLocalBroadcastManager.sendBroadcast(new Intent(Constants.ACTION_SYNC_IPOD_STRAT));
                    IPodDeviceMannager.getActiveDevice().startSyn();
                }
            }).start();
        }
        return START_STICKY;
    }

    //private IPodSynServiceImp binder = new IPodSynServiceImp();

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    //private class IPodSynServiceImp extends IIpodSyncService.Stub
    //{
       // @Override
        public boolean hasIpod() {
            IPodDevice iPodDevice = IPodDeviceMannager.getActiveDevice();
            if(iPodDevice!=null)
                return true;
            return false;
        }

       // @Override
        public void stopIpod()
        {
            Ipod.getInstance().stopIpod();
        }
      //  @Override
        public void addIpodDevice(String uuid)
        {
            IPodDeviceMannager.addIpodDevice(mContext,uuid);
        }
       // @Override
        public void removeIPodDevice(String uuid)
        {
            IPodDeviceMannager.removeIPodDevice(uuid);
        }
       // @Override
        public int getDeviceSynState()
        {
             if(IPodDeviceMannager.getActiveDevice()!=null)
            return IPodDeviceMannager.getActiveDevice().getDeviceSynState();
             return IPOD_DEV_NOT_SYN;
        }
        //@Override
        public boolean startSync() {
            if(IPodDeviceMannager.getActiveDevice().getDeviceSynState()== IPOD_DEV_NOT_SYN)
            {
                Log.d("MP","start syn IPOD 333");
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        mLocalBroadcastManager.sendBroadcast(new Intent(Constants.ACTION_SYNC_IPOD_STRAT));
                          //    MediaService.localBroadcastManager.sendBroadcast(new Intent(Constants.ACTION_SYNC_IPOD_STRAT));
                          IPodDeviceMannager.getActiveDevice().startSyn();
                    }
                }).start();
                return true;
            }
            return false;
        }
/*
        @Override
        public void basicTypes(int anInt, long aLong, boolean aBoolean, float aFloat, double aDouble, String aString) throws RemoteException {

        }
        */
   // }



}
