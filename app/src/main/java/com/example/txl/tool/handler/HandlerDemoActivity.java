package com.example.txl.tool.handler;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.View;

import com.example.txl.tool.R;

public class HandlerDemoActivity extends AppCompatActivity {

    private final String TAG = getClass().getSimpleName();
    //继承Handler重写handleMessage方法处理自己的消息
    class MyHandler extends Handler{
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage( msg );
            Log.d( TAG,"接收到消息，可以更新ui" );
        }
    }

    Handler handler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_handler_demo );
        findViewById( R.id.tv_open_thread_local ).setOnClickListener( new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent( HandlerDemoActivity.this,ThreadLocalDemoActivity.class );
                startActivity( intent );
            }
        } );
        handler = new MyHandler();

        new Thread( new Runnable() {
            @Override
            public void run() {
                try {
                    Thread.sleep( 3000 );
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                Message message = handler.obtainMessage( 1 );
                handler.sendMessage( message );

            }
        } ).start();

        new Thread( new Runnable() {
            @Override
            public void run() {
                try {
                    Thread.sleep( 3000 );
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                handler.post( new Runnable() {
                    @Override
                    public void run() {
                        Log.d( TAG,"Runnable 执行，可以更新ui" );
                    }
                } );
            }
        } ).start();


        final MyHandlerThread myHandlerThread = new MyHandlerThread( );
        myHandlerThread.setName( "HandlerThread-001" );
        myHandlerThread.start();

        //延时300ms的原因是线程启动需要时间，如果立即发送消息线程可能还没有启动起来
        findViewById( R.id.tv_open_thread_local ).postDelayed( new Runnable() {
            @Override
            public void run() {
                myHandlerThread.childHandler.sendEmptyMessage(0  );
                myHandlerThread.childHandler.post( new Runnable() {
                    @Override
                    public void run() {
                        Log.d( TAG,"handler在运行中...  当前线程 ：： " +Thread.currentThread().getName());
                    }
                } );


                //在主线程创建发送消息到子线程处理的Handler需要指定Looper
                Handler handler = new Handler( myHandlerThread.myLooper );
                handler.post( new Runnable() {
                    @Override
                    public void run() {
                        Log.d( TAG,"子线程处理消息 "+Thread.currentThread().getName() );
                    }
                } );



            }
        } ,300);


    }



    class MyHandlerThread extends Thread{

        //设置默认值的目的是防止在线程还没有开始执行的情况下childHandler值为空
        Handler childHandler =  new Handler(Looper.getMainLooper());
        private Looper myLooper;

        public MyHandlerThread() {
            super(  );
        }

        @Override
        public void run() {
            super.run();
            Looper.prepare();
            myLooper = Looper.myLooper();
            childHandler = new Handler( myLooper ){
                @Override
                public void handleMessage(Message msg) {
                    super.handleMessage( msg );
                    Log.d( TAG,"子线程handler在运行中...  当前线程： " +Thread.currentThread().getName());
                }
            };
            Log.d( TAG,"子线程开始运行...   " +Thread.currentThread().getName());
            childHandler.sendEmptyMessage( 0 );
            Looper.loop();
            Log.d( TAG,"子线程结束运行...   " +Thread.currentThread().getName());
        }
    }

}
