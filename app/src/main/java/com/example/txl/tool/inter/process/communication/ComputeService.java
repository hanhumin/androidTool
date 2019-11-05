package com.example.txl.tool.inter.process.communication;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.os.RemoteException;

import com.example.txl.tool.ICompute;

public class ComputeService extends Service {
    Binder binder = new ICompute.Stub() {
        @Override
        public int add(int a, int b) throws RemoteException {
            return (a+b)*2;
        }
    };
    public ComputeService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
       return binder;
    }
}
