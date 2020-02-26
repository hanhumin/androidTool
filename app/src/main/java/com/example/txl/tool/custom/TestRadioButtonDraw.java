package com.example.txl.tool.custom;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.util.AttributeSet;
import android.widget.RadioButton;

import com.example.txl.tool.R;

public class TestRadioButtonDraw extends RadioButton {
    public TestRadioButtonDraw(Context context) {
        super(context);
    }

    public TestRadioButtonDraw(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public TestRadioButtonDraw(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

//    public TestRadioButtonDraw(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
//        super(context, attrs, defStyleAttr, defStyleRes);
//    }

    @Override
    protected void dispatchDraw(Canvas canvas) {
        super.dispatchDraw(canvas);
        drawCircleTitleBg(canvas,0);
    }

    /**
     * 绘制一个顶部圆弧的背景
     * */
    Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private void drawCircleTitleBg(Canvas canvas,  int color){
        paint.setAntiAlias(true);
        paint.setColor(Color.RED);
        paint.setStyle(Paint.Style.FILL);
        paint.setStrokeWidth(getResources().getDimensionPixelOffset(R.dimen.dime_50dp));
        int height = getHeight();
        int width = getWidth();
        int topHeight = getResources().getDimensionPixelOffset(R.dimen.dime_24dp);
        canvas.drawRect(0,topHeight,width,height,paint);
        Path mPath = new Path();
        mPath.moveTo(15, topHeight);
        mPath.quadTo(width/2.0f, 0, width-15, topHeight);
        canvas.drawPath(mPath, paint);
//        canvas.drawLine(0,0,width,height,paint);
//        canvas.drawCircle(width/2.0f,height/2.0f,topHeight,paint);
//        canvas.drawColor(Color.RED);
    }
}
