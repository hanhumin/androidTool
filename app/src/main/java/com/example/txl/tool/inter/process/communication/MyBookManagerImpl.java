package com.example.txl.tool.inter.process.communication;

import android.os.Binder;
import android.os.IBinder;
import android.os.Parcel;
import android.os.RemoteException;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import java.util.List;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/15
 * description：
 */
public class MyBookManagerImpl extends Binder implements MyBookManager {
    static final String DESCRIPTOR = "com.example.txl.tool.inter.process.communication.MyBookManagerImpl";
    static final int TRANSACTION_getBookList = IBinder.FIRST_CALL_TRANSACTION + 0;
    static final int TRANSACTION_addBook = IBinder.FIRST_CALL_TRANSACTION + 1;

    public MyBookManagerImpl() {
        this.attachInterface( this,DESCRIPTOR );
    }

    public static MyBookManager asInterface(IBinder obj){
        if ((obj == null)) {
            return null;
        }
        android.os.IInterface iin = obj.queryLocalInterface( DESCRIPTOR );
        if(iin != null && iin instanceof MyBookManager){
            return (MyBookManager) iin;
        }
        return new Proxy( obj );
    }

    @Override
    public android.os.IBinder asBinder() {
        return this;
    }

    @Override
    protected boolean onTransact(int code, @NonNull Parcel data, @Nullable Parcel reply, int flags) throws RemoteException {
        return super.onTransact( code, data, reply, flags );
    }

    @Override
    public List<Book> getBookList() throws RemoteException {
        return null;
    }

    @Override
    public void addBook(Book book) throws RemoteException {

    }

    private static class Proxy implements MyBookManager{
        private IBinder mRemote;

        public Proxy(IBinder mRemote) {
            this.mRemote = mRemote;
        }

        @Override
        public IBinder asBinder() {
            return mRemote;
        }

        public String getInterfaceDescriptor(){
            return DESCRIPTOR;
        }


        @Override
        public List<Book> getBookList() throws RemoteException {
            Parcel data = Parcel.obtain();
            return null;
        }

        @Override
        public void addBook(Book book) throws RemoteException {

        }

    }


}
