package com.example.txl.tool.draworder;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.example.txl.tool.R;

/**
 * Copyright (c) 2020 唐小陆 All rights reserved.
 * author：txl
 * date：2020/10/14
 * description：
 */
public class DrawOrderTestFrameLayout extends FrameLayout {
    public DrawOrderTestFrameLayout(@NonNull Context context) {
        this(context,null);
    }

    public DrawOrderTestFrameLayout(@NonNull Context context, @Nullable AttributeSet attrs) {
        this(context, attrs,0);
    }

    public DrawOrderTestFrameLayout(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        setChildrenDrawingOrderEnabled(true);
        setLayerType(LAYER_TYPE_HARDWARE, null);
    }

    @Override
    protected int getChildDrawingOrder(int childCount, int i) {
        View child = getChildAt(i);
        Log.d("DrawOrderTestFrameLayout","getChildDrawingOrder DrawOrderTestTextView is "+child.getId());
//        switch (child.getId()){
//            case R.id.tv_01:{
//                return 2;
//            }
//            case R.id.tv_02:{
//                return 0;
//            }
//            case R.id.tv_03:{
//                return 1;
//            }
//        }

        //绘制顺序 231
        switch (child.getId()){
            case R.id.tv_01:{
                return 1;
            }
            case R.id.tv_02:{
                return 2;
            }
            case R.id.tv_03:{
                return 0;
            }
        }
        return super.getChildDrawingOrder(childCount, i);
    }
}
