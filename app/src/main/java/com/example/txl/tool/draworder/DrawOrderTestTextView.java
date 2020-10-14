package com.example.txl.tool.draworder;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.TextView;

import androidx.annotation.Nullable;

/**
 * Copyright (c) 2020 唐小陆 All rights reserved.
 * author：txl
 * date：2020/10/14
 * description：
 */
public class DrawOrderTestTextView extends TextView {
    public DrawOrderTestTextView(Context context) {
        super(context);
    }

    public DrawOrderTestTextView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public DrawOrderTestTextView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        Log.d("DrawOrderTestTextView","onDraw "+getText());
    }
}
