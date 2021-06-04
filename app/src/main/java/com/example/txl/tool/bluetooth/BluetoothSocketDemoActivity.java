package com.example.txl.tool.bluetooth;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.widget.Toast;

import com.example.txl.tool.R;

public class BluetoothSocketDemoActivity extends AppCompatActivity {
    private static final int REQUEST_ENABLE_BT = 10086;

    private final BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    private BroadcastReceiver receiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(BluetoothAdapter.ACTION_STATE_CHANGED.equals(intent.getAction())){//蓝牙状态改变

            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth_socket_demo);

        initBluetooth();
    }

    private void initBluetooth() {
        settingBluetooth();
    }
    //step2 设置蓝牙
    private void settingBluetooth() {
        if (bluetoothAdapter == null) {//当bluetoothAdapter 为空的时候说明应用不支持蓝牙
            Toast.makeText(this, "当前设备不支持蓝牙", Toast.LENGTH_SHORT).show();
            return;
        }

        if (!bluetoothAdapter.isEnabled()) {//蓝牙不可用，开启蓝牙
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        }
        IntentFilter intentFilter = new IntentFilter();
        //监听蓝牙状态改变
        intentFilter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
        registerReceiver(receiver,intentFilter);

        findDevice();
    }


    //step3 发现设备
    private void findDevice(){
        if(bluetoothAdapter != null && bluetoothAdapter.isEnabled()){
            bluetoothAdapter.startDiscovery();//
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        if(requestCode == REQUEST_ENABLE_BT){
            if(resultCode == RESULT_OK){
                findDevice();
                return;
            }
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

}