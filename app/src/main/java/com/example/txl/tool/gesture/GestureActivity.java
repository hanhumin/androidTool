package com.example.txl.tool.gesture;

import android.os.Bundle;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;

import com.example.txl.tool.BaseActivity;
import com.example.txl.tool.R;

public class GestureActivity extends BaseActivity implements GestureDetector.OnGestureListener{
    GestureDetector gestureDetector;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_gesture );
        gestureDetector = new GestureDetector( this,this );
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        return gestureDetector.onTouchEvent( event );
    }

    @Override
    public boolean onDown(MotionEvent e) {
        Log.d( "GankIoFrameLayout",  "onDown");
        return false;
    }

    @Override
    public void onShowPress(MotionEvent e) {
        Log.d( "GankIoFrameLayout",  "onShowPress");
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        Log.d( "GankIoFrameLayout",  "onSingleTapUp");
        return false;
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        Log.d( "GankIoFrameLayout",  "onScroll");
        return false;
    }

    @Override
    public void onLongPress(MotionEvent e) {
        Log.d( "GankIoFrameLayout",  "onLongPress");
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
        Log.d( "GankIoFrameLayout",  "onFling");
        return false;
    }
}
