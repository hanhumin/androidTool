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

public class ServiceDemoActivity extends AppCompatActivity {

    private static final String TAG = "ServiceDemoActivity";
    List<ServiceConnection> connectionList = new ArrayList<>();



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_service_demo);
        findViewById(R.id.tv_jump_onrebind).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ServiceDemoActivity.this,ServiceOnRebindActivity.class);
                startActivity(intent);
            }
        });
        findViewById(R.id.tv_start_service).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ServiceDemoActivity.this,DemoService.class);
                startService(intent);
            }
        });
        findViewById(R.id.tv_stop_service).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ServiceDemoActivity.this,DemoService.class);
                stopService(intent);
            }
        });
        findViewById(R.id.tv_bind_service).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ServiceDemoActivity.this,DemoService.class);
                ServiceConnection connection = new ServiceConnection() {
                    @Override
                    public void onServiceConnected(ComponentName name, IBinder service) {
                        Log.d(TAG,"DemoService onServiceConnected "+service);
                    }

                    @Override
                    public void onServiceDisconnected(ComponentName name) {

                    }
                };
                connectionList.add(connection);
                Log.d(TAG,"DemoService bindService result "+bindService(intent,connection, Context.BIND_AUTO_CREATE));

            }
        });
        findViewById(R.id.tv_unbind_service).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                for (ServiceConnection connection:connectionList){
                    unbindService(connection);
                }
                connectionList.clear();
                Log.d(TAG,"DemoService call unbind ");
            }
        });
    }
}