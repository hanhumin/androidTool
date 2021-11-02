package com.example.txl.tool.ipod;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.example.txl.tool.R;

public class IpodActivity extends AppCompatActivity {

    static {
        System.loadLibrary("ipod");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ipod);
    }
}