package com.example.txl.tool.bluetooth;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.RecyclerView;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.example.txl.tool.R;
import com.example.txl.tool.base.BaseAdapter;
import com.example.txl.tool.utils.AppExecutors;

import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;

public class BluetoothSocketDemoActivity extends AppCompatActivity {
    private static final String TAG = BluetoothSocketDemoActivity.class.getSimpleName();
    private static final int REQUEST_ENABLE_BT = 10086;

    private  UUID uuid = null;

    private final BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    private ServerRunnable serverRunnable = null;
    private ClientRunnable clientRunnable = null;
    private final BroadcastReceiver receiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if(BluetoothAdapter.ACTION_STATE_CHANGED.equals(action)){//蓝牙状态改变
                int currentState =  intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,BluetoothAdapter.STATE_OFF);
                int perState =  intent.getIntExtra(BluetoothAdapter.EXTRA_PREVIOUS_STATE,BluetoothAdapter.STATE_OFF);
                Log.d(TAG,"蓝牙状态改变 onReceive BluetoothAdapter.ACTION_STATE_CHANGED perState = "+perState+" currentState = "+currentState);
            }else if(BluetoothDevice.ACTION_FOUND.equals(action)) {//设备发现
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                Log.d(TAG,"onReceive BluetoothDevice.ACTION_FOUND "+device.getName()+" address: "+device.getAddress());
                foundDeviceAdapter.addData(device);//因为是Demo这里不关心是否有重复设备，设备被移除
            }else if(BluetoothAdapter.ACTION_SCAN_MODE_CHANGED.equals(action)){//蓝牙可见性
                int currentState =  intent.getIntExtra(BluetoothAdapter.EXTRA_SCAN_MODE,BluetoothAdapter.SCAN_MODE_NONE);
                int perState =  intent.getIntExtra(BluetoothAdapter.EXTRA_PREVIOUS_SCAN_MODE,BluetoothAdapter.SCAN_MODE_NONE);
                Log.d(TAG," 蓝牙可见性 onReceive BluetoothAdapter.ACTION_SCAN_MODE_CHANGED perState = "+perState+" currentState = "+currentState);
            }
        }
    };


    private RecyclerView rvBound,rvFound;
    private FoundDeviceAdapter foundDeviceAdapter;
    private BoundDeviceAdapter boundDeviceAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth_socket_demo);
        initView();
        settingBluetooth();
    }

    private void initView() {
        rvBound = findViewById(R.id.recyclerViewBound);
        rvFound = findViewById(R.id.recyclerViewFind);
        TextView tvFound = findViewById(R.id.tvStartDiscovery);
        tvFound.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(bluetoothAdapter == null){
                    Toast.makeText(BluetoothSocketDemoActivity.this, "当前设备不支持蓝牙", Toast.LENGTH_SHORT).show();
                    return;
                }
                findDevice();
            }
        });
        tvFound = findViewById(R.id.tvMakeVisible);
        tvFound.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(bluetoothAdapter == null){
                    Toast.makeText(BluetoothSocketDemoActivity.this, "当前设备不支持蓝牙", Toast.LENGTH_SHORT).show();
                    return;
                }
                makeBluetoothVisible();
            }
        });
        tvFound = findViewById(R.id.tvJumpMusic);
        tvFound.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(BluetoothSocketDemoActivity.this,BluetoothPlayerActivity.class);
                startActivity(intent);
            }
        });
        findViewById(R.id.tvAsServer).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                asServer();
                Toast.makeText(BluetoothSocketDemoActivity.this,"以作为服务端,可以通过其他设备连接进行通信",Toast.LENGTH_SHORT).show();
            }
        });
        EditText editText = findViewById(R.id.edInput);
        editText.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
                if(actionId == EditorInfo.IME_ACTION_SEND){
                    sendMessage(editText.getText().toString());
                    editText.getText().clear();
                    return true;
                }
                return false;
            }
        });
        boundDeviceAdapter = new BoundDeviceAdapter(this);
        rvBound.setAdapter(boundDeviceAdapter);
        foundDeviceAdapter = new FoundDeviceAdapter(this);
        rvFound.setAdapter(foundDeviceAdapter);
        boundDeviceAdapter.setItemClickListener(new BaseAdapter.OnItemClickListener<BluetoothDevice>() {
            @Override
            public void onItemClick(@NotNull View clickView, int position, @org.jetbrains.annotations.Nullable BluetoothDevice data, @NotNull BaseAdapter<BluetoothDevice> adapter) {
                asClient(data);
            }
        });
        foundDeviceAdapter.setItemClickListener(new BaseAdapter.OnItemClickListener<BluetoothDevice>() {
            @Override
            public void onItemClick(@NotNull View clickView, int position, @org.jetbrains.annotations.Nullable BluetoothDevice data, @NotNull BaseAdapter<BluetoothDevice> adapter) {
                asClient(data);
            }
        });
    }

    private void sendMessage(String message){
        if(message == null || message.isEmpty()){
            Toast.makeText(this,"不能发送空消息",Toast.LENGTH_SHORT).show();
            return;
        }
        if(serverRunnable != null){
            serverRunnable.write(message);
            return;
        }
        if(clientRunnable != null){
            clientRunnable.write(message);
            return;
        }
        Toast.makeText(this,"没有连接的对象发送消息",Toast.LENGTH_SHORT).show();
    }

    private void asServer(){
        resetServer();
        resetClient();
        serverRunnable = new ServerRunnable();
        AppExecutors.getInstance().networkIO().execute(serverRunnable);
    }

    private void asClient(BluetoothDevice device){
        resetServer();
        resetClient();
        clientRunnable = new ClientRunnable(device);
        AppExecutors.getInstance().networkIO().execute(clientRunnable);
    }

    private void resetClient(){
        if(clientRunnable != null){
            clientRunnable.cancel();
            clientRunnable = null;
        }
    }

    private void resetServer(){
        if(serverRunnable != null){
            serverRunnable.cancel();
            serverRunnable = null;
        }
    }


    // Unique UUID for this application
    private static final UUID MY_UUID_SECURE =
            UUID.fromString("fa87c0d0-afac-11de-8a39-0800200c9a66");
    private static final UUID MY_UUID_INSECURE =
            UUID.fromString("8ce255c0-200a-11e0-ac64-0800200c9a66");

    //step2 设置蓝牙
    private void settingBluetooth() {
        if (bluetoothAdapter == null) {//当bluetoothAdapter 为空的时候说明应用不支持蓝牙
            Toast.makeText(this, "当前设备不支持蓝牙", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }

//        uuid = UUID.nameUUIDFromBytes((getPackageName()+bluetoothAdapter.getAddress()).getBytes());
        uuid = MY_UUID_SECURE;

        if (!bluetoothAdapter.isEnabled()) {//蓝牙不可用，开启蓝牙
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        }
        registerReceiver();
//        makeBluetoothVisible();

    }

    @Override
    protected void onResume() {
        super.onResume();
        if(bluetoothAdapter != null){
            Set<BluetoothDevice> devices =  bluetoothAdapter.getBondedDevices();
            boundDeviceAdapter.setListData(new ArrayList<>(devices));
        }

    }

    private void registerReceiver() {
        IntentFilter intentFilter = new IntentFilter();
        //监听蓝牙状态改变
        intentFilter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
        //监听设备发现
        intentFilter.addAction(BluetoothDevice.ACTION_FOUND);
        //监听蓝牙设备的可见性
        intentFilter.addAction(BluetoothAdapter.ACTION_SCAN_MODE_CHANGED);
        registerReceiver(receiver,intentFilter);
    }

    /**
     * 设置蓝牙在30s内可见
     * */
    private void makeBluetoothVisible() {
        Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
        discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 30);
        startActivity(discoverableIntent);
    }


    //step3 发现设备
    private void findDevice(){
        if(bluetoothAdapter != null && bluetoothAdapter.isEnabled()){
            if(bluetoothAdapter.isDiscovering()){
                Toast.makeText(this,"处于设备发现中",Toast.LENGTH_SHORT).show();
                return;
            }
            foundDeviceAdapter.setListData(new ArrayList<>());
            foundDeviceAdapter.notifyDataSetChanged();
            bluetoothAdapter.startDiscovery();
            Toast.makeText(this,"设备发现开启",Toast.LENGTH_SHORT).show();
        }else {
            Toast.makeText(this,"蓝牙状态异常",Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        if(requestCode == REQUEST_ENABLE_BT){
            if(resultCode == RESULT_OK){
                Toast.makeText(this,"蓝牙已开启",Toast.LENGTH_SHORT).show();
                return;
            }
        }
        super.onActivityResult(requestCode, resultCode, data);
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(receiver);
        resetClient();
        resetServer();

    }

    class ServerRunnable implements Runnable{
        boolean runAccept = true;
        BluetoothServerSocket serverSocket;
        BluetoothSocket bluetoothSocket;
        InputStream inputStream = null;
        OutputStream outputStream = null;
        BluetoothDevice remoteDevice = null;

        public ServerRunnable() {
            try {
                serverSocket = bluetoothAdapter.listenUsingRfcommWithServiceRecord("bluetoothSocket",uuid );
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void run() {
            try {
                bluetoothSocket = serverSocket.accept();
            } catch (IOException e) {
                e.printStackTrace();
            }
            if(bluetoothSocket != null){
                try {
                    serverSocket.close();
                    serverSocket = null;
//                    bluetoothSocket.connect();
                    remoteDevice = bluetoothSocket.getRemoteDevice();
                    bluetoothSocket.getRemoteDevice();
                    inputStream = bluetoothSocket.getInputStream();
                    outputStream =bluetoothSocket.getOutputStream();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                byte[] buffer = new byte[1024];
                int bytes;
                while (runAccept){
                    try {
                        bytes = inputStream.read(buffer);
                        String readMessage = new String(buffer, 0, bytes);
                        Log.d(TAG,"receive device "+remoteDevice.getName()+":: "+readMessage);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }

        }

        public void cancel(){
            runAccept =  false;
            if(outputStream  != null){
                try {
                    outputStream.close();
                    outputStream = null;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if(inputStream  != null){
                try {
                    inputStream.close();
                    inputStream = null;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if(bluetoothSocket != null){
                try {
                    bluetoothSocket.close();
                    bluetoothSocket = null;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if(serverSocket != null){
                try {
                    serverSocket.close();
                    serverSocket = null;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        private void write(String data){
            if(outputStream != null){

                try {
                    outputStream.write(data.getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    class ClientRunnable implements Runnable{
        private BluetoothSocket bluetoothSocket;
        InputStream inputStream = null;
        OutputStream outputStream = null;
        BluetoothDevice device;
        boolean runAccept;
        public ClientRunnable(BluetoothDevice device) {
            this.device = device;
            try {
                bluetoothSocket = device.createRfcommSocketToServiceRecord(uuid);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void run() {
            if(bluetoothSocket != null){
                try {
                    bluetoothSocket.connect();
                    AppExecutors.getInstance().mainThread().execute(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(BluetoothSocketDemoActivity.this,"连接成功，可以发送消息拉",Toast.LENGTH_SHORT).show();
                        }
                    });

                    inputStream = bluetoothSocket.getInputStream();
                    outputStream =bluetoothSocket.getOutputStream();
                } catch (IOException e) {
                    e.printStackTrace();
                    AppExecutors.getInstance().mainThread().execute(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(BluetoothSocketDemoActivity.this,"连接失败，请重试",Toast.LENGTH_SHORT).show();
                        }
                    });
                    return;
                }
                byte[] buffer = new byte[1024];
                int bytes;
                while (runAccept){
                    try {
                        bytes = inputStream.read(buffer);
                        String readMessage = new String(buffer, 0, bytes);
                        Log.d(TAG,"receive device "+device.getName()+":: "+readMessage);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }

        public void cancel(){
            runAccept =  false;
            if(outputStream  != null){
                try {
                    outputStream.close();
                    outputStream = null;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if(inputStream  != null){
                try {
                    inputStream.close();
                    inputStream = null;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if(bluetoothSocket != null){
                try {
                    bluetoothSocket.close();
                    bluetoothSocket = null;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        private void write(String data){
            if(outputStream != null){
                try {
                    outputStream.write(data.getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }


}