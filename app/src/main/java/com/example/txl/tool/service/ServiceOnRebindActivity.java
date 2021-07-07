package com.example.txl.tool.service;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.View;

import com.example.txl.tool.R;

import java.util.ArrayList;
import java.util.List;

public class ServiceOnRebindActivity extends AppCompatActivity {
    private static final String TAG = "ServiceOnRebindActivity";
    ServiceConnection connection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Log.d(TAG,"DemoService onServiceConnected "+service);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {

        }
    };

    boolean bind = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_service_on_rebind);
        findViewById(R.id.tv_bind_service).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ServiceOnRebindActivity.this,DemoService.class);

                if(!bind){
                    bind = bindService(intent,connection, Context.BIND_AUTO_CREATE);
                }
                Log.d(TAG,"DemoService bindService result "+bind);

            }
        });
        findViewById(R.id.tv_unbind_service).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(bind)
                unbindService(connection);
                bind = false;
                Log.d(TAG,"DemoService call unbind ");
            }
        });
    }
}