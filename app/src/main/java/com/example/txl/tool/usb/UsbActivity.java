package com.example.txl.tool.usb;

import androidx.appcompat.app.AppCompatActivity;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.util.Log;

import com.example.txl.tool.R;

public class UsbActivity extends AppCompatActivity {
    private static final String TAG = UsbActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_usb);
        UsbManager manager = (UsbManager) getSystemService(Context.USB_SERVICE);
        Log.d(TAG,"device list "+manager.getDeviceList().size()+"   "+manager.getDeviceList()+"  ");
    }

    private static class UsbConnectBroadcast extends BroadcastReceiver{
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.d(TAG,"UsbConnectBroadcast  onReceive  ");
        }
    }
}