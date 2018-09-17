package com.example.txl.tool.hencoder;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
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
        drawArc(canvas,new Paint(  ));
    }

    /**
     * 填充画布的背景
     * */
    private void drawArgb(Canvas canvas){
        canvas.drawARGB(0xf0,0xff,0xf3,0x4e);
    }

    private void drawArc(Canvas canvas, Paint paint){
        canvas.drawArc( 0,0,getRight(),getBottom(),15,90,false,paint );
        canvas.drawArc( new RectF( 0,0,getRight(),getBottom() ),195,90,true,paint );
    }
}
