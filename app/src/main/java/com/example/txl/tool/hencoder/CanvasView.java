package com.example.txl.tool.hencoder;

import android.content.Context;
import android.graphics.Canvas;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.View;

/**
 * create by txl
 * at 2018/9/17
 * description :
 */
public class CanvasView extends View {
    public CanvasView(Context context) {
        super(context);
    }

    public CanvasView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public CanvasView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        drawArgb(canvas);
    }

    /**
     * 填充画布的背景
     * */
    private void drawArgb(Canvas canvas){
        canvas.drawARGB(0xf0,0xff,0xf3,0x4e);
    }
}
