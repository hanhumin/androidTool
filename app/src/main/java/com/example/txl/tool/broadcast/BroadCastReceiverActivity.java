package com.example.txl.tool.broadcast;

import androidx.appcompat.app.AppCompatActivity;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.example.txl.tool.R;

public class BroadCastReceiverActivity extends AppCompatActivity {
    private static String TAG = BroadCastReceiverActivity.class.getSimpleName();

    private static final String ACTION_NORMAL = "com.txl.tool.broadcast.normal";
    private static final String ACTION_ORDER = "com.txl.tool.broadcast.normal";
    private static final String ACTION_STICKY = "com.txl.tool.broadcast.normal";

    BroadcastReceiver normal = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.d(TAG,"normal onReceive start");
            try {
                Thread.sleep(190*1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            Log.d(TAG,"normal onReceive end");
        }
    };

    BroadcastReceiver order = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.d(TAG,"order onReceive start");
            try {
                Thread.sleep(190*1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            Log.d(TAG,"order onReceive end");
        }
    };

    BroadcastReceiver sticky = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.d(TAG,"sticky onReceive");
        }
    };



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_broad_cast_receiver);
        findViewById(R.id.tv_send_normal).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendBroadcast(new Intent(ACTION_NORMAL));
            }
        });
        findViewById(R.id.tv_send_order).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendOrderedBroadcast(new Intent(ACTION_NORMAL),null);
            }
        });
        findViewById(R.id.tv_send_sticky).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

            }
        });

        registerReceiver(normal,new IntentFilter(ACTION_NORMAL));
        registerReceiver(order,new IntentFilter(ACTION_ORDER));
        registerReceiver(sticky,new IntentFilter(ACTION_STICKY));
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(normal);
        unregisterReceiver(order);
        unregisterReceiver(sticky);
    }
}