package com.example.txl.tool.inter.process.communication;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;

import com.example.txl.tool.ICompute;
import com.example.txl.tool.R;

public class ComputeActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_compute );
        Intent intent = new Intent( this,ComputeService.class );
        bindService( intent, new ServiceConnection() {
            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {
                ICompute.Stub.asInterface( service );
            }

            @Override
            public void onServiceDisconnected(ComponentName name) {

            }
        } ,BIND_AUTO_CREATE);
    }
}
