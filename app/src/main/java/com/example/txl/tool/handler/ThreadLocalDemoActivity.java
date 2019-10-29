package com.example.txl.tool.handler;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;

import com.example.txl.tool.R;

public class ThreadLocalDemoActivity extends AppCompatActivity {
    private final String TAG = getClass().getSimpleName();

    ThreadLocal<Boolean> test = new ThreadLocal<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_thread_local_demo );

        Log.d( TAG,"主线程： test "+test.get() );
        test.set( false );
        Log.d( TAG,"主线程： test "+test.get() );
        new Thread( new Runnable() {
            @Override
            public void run() {
                test.set( true );
                Log.d( TAG,"线程1： test "+test.get() );
            }
        } ).start();
        new Thread( new Runnable() {
            @Override
            public void run() {
                Log.d( TAG,"线程2： test "+test.get() );
            }
        } ).start();
    }

    class TestThreadLocal extends ThreadLocal<Boolean>{
        @Nullable
        @Override
        protected Boolean initialValue() {
            return true;
        }
    }

}
