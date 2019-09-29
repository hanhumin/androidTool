package com.example.txl.tool.custom;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/18
 * description：
 */
public class DragTextView extends androidx.appcompat.widget.AppCompatTextView {
    private static final String TAG = "DragTextView";
    public DragTextView(Context context) {
        super( context );
    }

    public DragTextView(Context context, AttributeSet attrs) {
        super( context, attrs );
    }

    public DragTextView(Context context, AttributeSet attrs, int defStyleAttr) {
        super( context, attrs, defStyleAttr );
    }


    int mLastX,mLastY;
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        Log.d( TAG,"move, onTouchEvent  " +event.getAction());
        int x = (int) event.getRawX();
        int y = (int) event.getRawY();
        switch (event.getAction()){
            case MotionEvent.ACTION_DOWN:
                return true;
            case MotionEvent.ACTION_MOVE:
                int deltaX = x - mLastX;
                int deltaY = y - mLastY;
                Log.d( TAG,"move, deltaX:"+deltaX+" deltaY:"+deltaY );
                int translationX = (int) (getTranslationX() + deltaX);
                int translationY = (int) (getTranslationY() + deltaY);
                setTranslationX( translationX );
                setTranslationY( translationY );
                break;
        }
        mLastX = x;
        mLastY = y;
        return true;
    }

    @Override
    public void computeScroll() {
        super.computeScroll();
    }
}
