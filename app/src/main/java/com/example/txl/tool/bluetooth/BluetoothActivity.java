package com.example.txl.tool.bluetooth;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.recyclerview.widget.RecyclerView;

import android.Manifest;
import android.bluetooth.BluetoothA2dp;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothHeadset;
import android.bluetooth.BluetoothProfile;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.example.txl.tool.R;
import com.example.txl.tool.base.BaseAdapter;

import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

/**
 * 蓝牙demo
 * */
public class BluetoothActivity extends AppCompatActivity {
    private static final String TAG = BluetoothActivity.class.getSimpleName();

    private static final int REQUEST_ENABLE_BT = 10086;
    private static final int MY_PERMISSION_REQUEST_CONSTANT = 12345;

    private BluetoothAdapter bluetoothAdapter;
    private BluetoothProfile.ServiceListener mServiceListener;
    private RecyclerView rvBound,rvFound;
    // Create a BroadcastReceiver for ACTION_FOUND.
    private BroadcastReceiver receiver;
    private FoundDeviceAdapter foundDeviceAdapter;
    private BoundDeviceAdapter boundDeviceAdapter;

    //耳机
    private BluetoothHeadset headset;
    //高质量音频传输
    private BluetoothA2dp a2dp;

    private BluetoothDevice connectDevice;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);
        if (Build.VERSION.SDK_INT >= 6.0) {
            ActivityCompat.requestPermissions(this, new String[]{
                            Manifest.permission.ACCESS_FINE_LOCATION},
                    MY_PERMISSION_REQUEST_CONSTANT);
        }
        initView();
        initBluetooth();
    }

    private void initView() {
        rvBound = findViewById(R.id.rv_bound_device);
        rvFound = findViewById(R.id.rv_found_device);
        TextView tvFound = findViewById(R.id.tv_found_device);
        tvFound.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(bluetoothAdapter == null){
                    Toast.makeText(BluetoothActivity.this, "当前设备不支持蓝牙", Toast.LENGTH_SHORT).show();
                    return;
                }
                discoverBluetoothDevices();
            }
        });
        boundDeviceAdapter = new BoundDeviceAdapter(this);
        rvBound.setAdapter(boundDeviceAdapter);
        foundDeviceAdapter = new FoundDeviceAdapter(this);
        rvFound.setAdapter(foundDeviceAdapter);
        boundDeviceAdapter.setItemClickListener(new BaseAdapter.OnItemClickListener<BluetoothDevice>() {
            @Override
            public void onItemClick(@NotNull View clickView, int position, @org.jetbrains.annotations.Nullable BluetoothDevice data, @NotNull BaseAdapter<BluetoothDevice> adapter) {

            }
        });
        foundDeviceAdapter.setItemClickListener(new BaseAdapter.OnItemClickListener<BluetoothDevice>() {
            @Override
            public void onItemClick(@NotNull View clickView, int position, @org.jetbrains.annotations.Nullable BluetoothDevice data, @NotNull BaseAdapter<BluetoothDevice> adapter) {

            }
        });
    }

    /**
     * 蓝牙设备的连接分成下面几个步骤
     * 1、manifest 请求蓝牙权限
     * 2、使用配置文件
     * 3、设置蓝牙  判断是否支持蓝牙设备，开启蓝牙
     * 4、查找设备  已配对设备，发现设备
     * 5、连接设备
     * */
    private void initBluetooth(){
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(bluetoothAdapter == null){
            Toast.makeText(this, "当前设备不支持蓝牙", Toast.LENGTH_SHORT).show();
            return;
        }
        useProfile();
        settingBluetoothActivity();
    }

    private void settingBluetoothActivity(){
        if(bluetoothAdapter.isEnabled()){
            Toast.makeText(this, "蓝牙已开启", Toast.LENGTH_SHORT).show();
            discoverBluetoothDevices();
        }else {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        }
    }


    private void discoverBluetoothDevices(){
        if(receiver == null){
            receiver  = new BroadcastReceiver() {
                public void onReceive(Context context, Intent intent) {
                    String action = intent.getAction();
                    if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                        // Discovery has found a device. Get the BluetoothDevice
                        // object and its info from the Intent.
                        BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                        Log.d(TAG,"discoverBluetoothDevices BroadcastReceiver device found  "+device.getName()+" address: "+device.getAddress());
                        foundDeviceAdapter.addData(device);
                    }
                }
            };

            IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
            registerReceiver(receiver, filter);
        }



        Set<BluetoothDevice> devices =  bluetoothAdapter.getBondedDevices();
        if(boundDeviceAdapter.getListData() != null)
        boundDeviceAdapter.getListData().clear();
        boundDeviceAdapter.addListData(new ArrayList<>(devices));
        for (BluetoothDevice device :devices){
            Log.d(TAG,"discoverBluetoothDevices BondedDevices  "+device.getName()+" address: "+device.getAddress());
        }
        if(bluetoothAdapter.isDiscovering()){
            bluetoothAdapter.cancelDiscovery();
        }

        bluetoothAdapter.startDiscovery();
        Log.d(TAG,"discoverBluetoothDevices startDiscovery  ");
    }

    private void useProfile() {
        mServiceListener = new BluetoothProfile.ServiceListener() {
            @Override
            public void onServiceConnected(int profile, BluetoothProfile proxy) {
                Log.d(TAG,"onServiceConnected profile is "+profile);
                if(profile == BluetoothProfile.HEADSET){
                    headset = (BluetoothHeadset) proxy;
                    List<BluetoothDevice> bluetoothDevices =  headset.getConnectedDevices();
                    if(bluetoothDevices != null && bluetoothDevices.size()>0){
                        connectDevice = bluetoothDevices.get(0);
                    }
                }else if(profile == BluetoothProfile.A2DP){
                    a2dp = (BluetoothA2dp) proxy;
                    List<BluetoothDevice> bluetoothDevices = a2dp.getConnectedDevices();
                    if(bluetoothDevices != null && bluetoothDevices.size()>0){
                        connectDevice = bluetoothDevices.get(0);
                    }
                }
            }

            @Override
            public void onServiceDisconnected(int profile) {
                Log.d(TAG,"onServiceDisconnected profile is "+profile);
                if(profile == BluetoothProfile.HEADSET){
                    headset = null;
                }else if(profile == BluetoothProfile.A2DP){
                    a2dp = null;
                }
            }
        };
        bluetoothAdapter.getProfileProxy(this,mServiceListener,BluetoothProfile.HEADSET);
        bluetoothAdapter.getProfileProxy(this,mServiceListener,BluetoothProfile.A2DP);
    }

    private void connect(BluetoothDevice device){
        if(connectDevice != null && connectDevice.getAddress().equals(device.getAddress())){
            Toast.makeText(this, "当前设备已连接！", Toast.LENGTH_SHORT).show();
            return;
        }
        disConnect(connectDevice);
        if(headset != null){
            headset.connect(device);
        }
        if(a2dp != null){
            a2dp.connect(device);
        }
    }

    private void disConnect(BluetoothDevice device){

    }


    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if(requestCode == MY_PERMISSION_REQUEST_CONSTANT){
            if(grantResults[0] == -1){
                Toast.makeText(this, "您没有蓝牙相关的权限", Toast.LENGTH_SHORT).show();
            }
        }
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        if(requestCode == REQUEST_ENABLE_BT){
            if(resultCode == RESULT_OK){
                settingBluetoothActivity();
                return;
            }
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(bluetoothAdapter != null){
            if(mServiceListener != null){
//                bluetoothAdapter.closeProfileProxy();
            }
        }
        if(receiver != null){
            unregisterReceiver(receiver);
            receiver = null;
        }
        if(headset != null){
            bluetoothAdapter.closeProfileProxy(BluetoothProfile.HEADSET,headset);
            headset = null;
        }
        if(a2dp != null){
            bluetoothAdapter.closeProfileProxy(BluetoothProfile.A2DP,a2dp);
            a2dp = null;
        }
    }
}