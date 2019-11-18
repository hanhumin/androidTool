package com.example.txl.tool.keyevent;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.widget.TextView;

import com.example.txl.tool.R;

public class KeyEventDemoActivity extends AppCompatActivity {
    private static final String TAG = KeyEventDemoActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_key_event_demo );
//        findViewById( R.id.tv_focus ).setFocusedByDefault( true );
        final TextView tvFocus = findViewById( R.id.tv_focus );
        tvFocus.postDelayed( new Runnable() {
            @Override
            public void run() {
                tvFocus.requestFocus();
            }
        } ,500);

    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        Log.d( TAG,"dispatchKeyEvent" );
        return super.dispatchKeyEvent( event );
    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent ev) {
        Log.d( TAG,"dispatchTouchEvent" );
        return super.dispatchTouchEvent( ev );
    }
}
