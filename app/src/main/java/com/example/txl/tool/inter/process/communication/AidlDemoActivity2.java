package com.example.txl.tool.inter.process.communication;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.example.txl.tool.R;
import com.example.txl.tool.utils.AppExecutors;

import java.util.List;

public class AidlDemoActivity2 extends AppCompatActivity {
    private final String TAG = "AidlDemoActivity2";
    private TextView tvGetBookList, tvAddBook;
    IBookManager bookManager;
    IOnBookArrivedListener onBookArrivedListener =  new IOnBookArrivedListener.Stub(){
        @Override
        public void onNewBookArrived(Book book) throws RemoteException {
            Log.d( TAG,"onNewBookArrived  "+book);
        }
    };
    ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            bookManager = IBookManager.Stub.asInterface( service );
            try {
                bookManager.registerBookArrivedListener(onBookArrivedListener);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            bookManager = null;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_aidl_demo2 );
        initView();
        bindAidlService();
        AidlConfig.config();
    }

    private void initView() {
        tvGetBookList = findViewById( R.id.tv_get_book_list );
        tvGetBookList.setOnClickListener( new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                if(bookManager != null){
                    AppExecutors.getInstance().networkIO().execute(new Runnable() {
                        @Override
                        public void run() {
                            //这个东西是一个耗时操作
                            List<Book> bookList = null;
                            try {
                                bookList = bookManager.getBookList();
                            } catch (RemoteException e) {
                                e.printStackTrace();
                            }
                            for (Book book : bookList){
                                Log.d( "AidlDemoActivity", "bookList book name: "+book.getName()+"  id: "+book.getBookId()+" author: "+book.getAuthor());
                            }
                        }
                    });
                }
            }
        } );
        tvAddBook = findViewById( R.id.tv_add_book );
        tvAddBook.setOnClickListener( new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(bookManager != null){
                    try {
                        //这个东西是一个耗时操作
                        int id = (int) (Math.random()*1000);
                        Book book = new Book(id,"测试添加book:"+id,"作者:"+id );
                        bookManager.addBook( book );
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                }
            }
        } );
    }

    private void bindAidlService(){
        Intent intent = new Intent(  );
        intent.setPackage(getPackageName());
        intent.setAction( "com.txl.aidl.demo" );
        bindService( intent,serviceConnection ,BIND_AUTO_CREATE);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(bookManager != null && bookManager.asBinder().isBinderAlive()){
            try {
                bookManager.unRegisterBookArrivedListener( onBookArrivedListener );
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
        unbindService( serviceConnection );
    }
}