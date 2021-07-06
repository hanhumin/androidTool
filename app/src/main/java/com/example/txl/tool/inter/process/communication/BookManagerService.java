package com.example.txl.tool.inter.process.communication;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.os.RemoteCallbackList;
import android.os.RemoteException;
import android.util.Log;

import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.atomic.AtomicBoolean;

public class BookManagerService extends Service {
    private static final String TAG = "BookManagerService";
//    private CopyOnWriteArrayList<IOnBookArrivedListener> onBookArrivedListeners = new CopyOnWriteArrayList<>(  );
    private RemoteCallbackList<IOnBookArrivedListener> onBookArrivedListenerRemoteCallbackList = new RemoteCallbackList<>();
    private AtomicBoolean atomicBoolean = new AtomicBoolean( false );


    private CopyOnWriteArrayList<Book> mBookList = new CopyOnWriteArrayList<>(  );

    private Binder mBinder = new IBookManager.Stub() {
        @Override
        public List<Book> getBookList() throws RemoteException {
            Log.d( TAG,"getBookList thread :"+Thread.currentThread().getName() );
            return mBookList;
        }

        @Override
        public void addBook(Book book) throws RemoteException {
            Log.d( TAG,"addBook thread :"+Thread.currentThread().getName() );
            mBookList.add( book );
        }

        @Override
        public void registerBookArrivedListener(IOnBookArrivedListener listenser) throws RemoteException {

            if(onBookArrivedListenerRemoteCallbackList.register( listenser )){
                Log.d( TAG,"registerBookArrivedListener success" );
            }else {
                Log.d( TAG,"registerBookArrivedListener failed" );
            }
//            if(!onBookArrivedListeners.contains( listenser )){
//                onBookArrivedListeners.add( listenser );
//                Log.d( TAG,"registerBookArrivedListener success" );
//            }else {
//                Log.d( TAG,"registerBookArrivedListener failed" );
//            }
        }

        @Override
        public void unRegisterBookArrivedListener(IOnBookArrivedListener listenser) throws RemoteException {
            if(onBookArrivedListenerRemoteCallbackList.unregister( listenser )){
                Log.d( TAG,"unRegisterBookArrivedListener success" );
            }else {
                Log.d( TAG,"unRegisterBookArrivedListener failed" );
            }
//            if(onBookArrivedListeners.contains( listenser )){
//                Log.d( TAG,"unRegisterBookArrivedListener success" );
//                onBookArrivedListeners.remove( listenser );
//            }else {
//                Log.d( TAG,"unRegisterBookArrivedListener failed" );
//            }
        }
    };

    public BookManagerService() {
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d( TAG,"onCreate" );
        mBookList.add( new Book( 1,"Android","小王" ) );
        mBookList.add( new Book( 2,"IOS" ,"小张") );
        new Thread( new ServiceWorker() ).start();
    }

    @Override
    public void onRebind(Intent intent) {
        super.onRebind(intent);
    }

    @Override
    public IBinder onBind(Intent intent) {

        return mBinder;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d( TAG,"onDestroy" );
        atomicBoolean.set( true );
    }

    private class ServiceWorker implements Runnable{

        @Override
        public void run() {
            while (!atomicBoolean.get()){
                try {
                    Thread.sleep( 5000 );
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                Book book = new Book( mBookList.size(),"自动添加书籍"+mBookList.size(),"自动"+mBookList.size() );
                try {
                    onBookArrived(book);
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private void onBookArrived(Book book) throws RemoteException {
        mBookList.add( book );
        int count = onBookArrivedListenerRemoteCallbackList.beginBroadcast();
        for (int i = 0; i<count; i++){
            onBookArrivedListenerRemoteCallbackList.getBroadcastItem( i ).onNewBookArrived( book );
        }
        onBookArrivedListenerRemoteCallbackList.finishBroadcast();
//        for (IOnBookArrivedListener listener:onBookArrivedListeners){
//            listener.onNewBookArrived( book );
//        }
    }
}
