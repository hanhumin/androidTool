package com.example.txl.tool.custom;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PorterDuff;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.util.AttributeSet;
import android.view.Gravity;
import android.widget.RadioButton;

import androidx.annotation.ColorInt;
import androidx.core.content.ContextCompat;
import androidx.core.graphics.drawable.DrawableCompat;

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

    }

    @Override
    protected void onDraw(Canvas canvas) {
        drawCircleTitleBg(canvas,Color.RED);
        super.onDraw(canvas);
    }

    /**
     * 绘制一个顶部圆弧的背景
     * */
    Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private BitmapDrawable circleTitleBgDrawable;
    private int circleTitleBgColor = Color.WHITE;
    private void drawCircleTitleBg(Canvas canvas,  int color){
        if(circleTitleBgDrawable == null){
            setCircleTitleBgColor(circleTitleBgColor);
            return;
        }
        paint.setAntiAlias(true);
        paint.setColor(color);
        paint.setStyle(Paint.Style.FILL);

        int height = getHeight();
        int width = getWidth();

        float topHeight = getResources().getDimensionPixelSize(R.dimen.dimen24);
        paint.setStrokeWidth(topHeight);
        final int scrollX = getScrollX();
        final int scrollY = getScrollY();
        float r = ((width-15) * (width-15) / (8f * topHeight)) +  (topHeight / 2f);
        if (scrollX == 0 && scrollY == 0) {
//            circleTitleBgDrawable.draw(canvas);
            circleTitleBgDrawable = (BitmapDrawable) ContextCompat.getDrawable(getContext(),R.drawable.top_circle_bg);
            Drawable wrappedDrawable = DrawableCompat.wrap(circleTitleBgDrawable);
            wrappedDrawable = wrappedDrawable.mutate();
            DrawableCompat.setTint(wrappedDrawable, Color.RED);
            wrappedDrawable.setBounds(new Rect(0,0,width, (int) topHeight));
            circleTitleBgDrawable.setGravity(Gravity.CENTER_HORIZONTAL + Gravity.TOP);
            circleTitleBgDrawable.draw(canvas);
//            canvas.drawBitmap(circleTitleBgDrawable.getBitmap(),new Matrix(),paint);
//            canvas.drawBitmap(circleTitleBgDrawable.getBitmap(),null,new Rect(0,0,width, (int) topHeight),paint);
        } else {
            canvas.translate(scrollX, scrollY);
//            canvas.drawBitmap(circleTitleBgDrawable.getBitmap(),new Matrix(),paint);
//            circleTitleBgDrawable.draw(canvas);
            canvas.translate(-scrollX, -scrollY);
        }
    }

    public void setCircleTitleBgColor(@ColorInt int titleBgColor){
        circleTitleBgColor = titleBgColor;
        circleTitleBgDrawable = circleTitleBgDrawable(titleBgColor);
        invalidate();
    }

    private BitmapDrawable circleTitleBgDrawable(@ColorInt int titleBgColor){
        Drawable drawable = ContextCompat.getDrawable(getContext(),R.drawable.top_circle_bg);
        if(drawable == null || getWidth() == 0){
            return null;
        }
        final Drawable wrappedDrawable = DrawableCompat.wrap(drawable);
        wrappedDrawable.mutate();
        DrawableCompat.setTint(wrappedDrawable, titleBgColor);
        drawable = wrappedDrawable;
        //目标图片的宽高
        int targetWidth=getWidth() > drawable.getIntrinsicWidth()? drawable.getIntrinsicWidth():getWidth();
        int targetHeight= drawable.getIntrinsicHeight();
        //宽高应该是当前button的宽高
        Bitmap bitmap=Bitmap.createBitmap(targetWidth, targetHeight, Bitmap.Config.ARGB_8888);
        BitmapDrawable bgDrawable = new BitmapDrawable(bitmap);
        Canvas canvas = new Canvas(bitmap);
        int color=Color.TRANSPARENT;
        canvas.drawColor(color, PorterDuff.Mode.ADD);
        Paint paint=new Paint();
        paint.setColor(color);
        paint.setFilterBitmap(true);
        paint.setStyle(Paint.Style.FILL);
        canvas.drawRect(new Rect(0, 0, targetWidth, targetHeight), paint);
        Bitmap defaultLoading=((BitmapDrawable)drawable).getBitmap();
        defaultLoading = scaleImage(defaultLoading,targetWidth,targetHeight);//先将图片缩放到希望的大小
        Matrix matrix=new Matrix();
        float translateX=0.0f,translateY=0.0f;
        if(defaultLoading.getHeight()>targetHeight||defaultLoading.getWidth()>targetWidth)
        {
            float scaleX=1.0f,scaleY=1.0f;
            float scaleW,scaleH;
            if(defaultLoading.getHeight()>targetHeight&&defaultLoading.getWidth()>targetWidth)
            {
                scaleY=1.0f*targetHeight/defaultLoading.getHeight();
                scaleX=scaleY;
            }
            else if(defaultLoading.getHeight()>targetHeight)
            {
                scaleY=1.0f*targetWidth/defaultLoading.getHeight();
                scaleX=scaleY;
            }
            else if(defaultLoading.getWidth()>targetWidth)
            {
                scaleX=1.0f*targetWidth/defaultLoading.getWidth();
                scaleY=scaleX;
            }
            scaleW=defaultLoading.getWidth()*scaleX;
            scaleH=defaultLoading.getHeight()*scaleY;
            translateX=(targetWidth-scaleW)/2.0f;
            translateY=(targetHeight-scaleH)/2.0f;
            matrix.postScale(scaleX, scaleY);
        }
        else
        {
            translateX=(targetWidth-defaultLoading.getWidth())/2.0f;
            translateY=(targetHeight-defaultLoading.getHeight())/2.0f;
        }
        matrix.postTranslate(translateX, translateY);
        paint.setColor(Color.RED);
        canvas.drawBitmap(defaultLoading, matrix, paint);
        bgDrawable.draw(canvas);
        return bgDrawable;
    }

    /**
     * 缩放图片
     * @param bm 要缩放图片
     * @param newWidth 宽度
     * @param newHeight 高度
     * @return处理后的图片
     */
    public Bitmap  scaleImage(Bitmap bm, float newWidth, float newHeight){
        if (bm == null){
            return null;
        }
        int width = bm.getWidth();
        int height = bm.getHeight();
        float scaleWidth =  newWidth / width;
        float scaleHeight =  newHeight / height;
        if(scaleWidth == scaleHeight && scaleWidth == 1){
            return bm;
        }
        Matrix matrix = new Matrix();
        matrix.postScale(scaleWidth, scaleHeight);
        Bitmap newbm = Bitmap.createBitmap(bm, 0, 0, width, height, matrix,true);
        if (bm != null & !bm.isRecycled()){
//            bm.recycle();//销毁原图片
            bm = null;
        }
        return newbm;
    }

}
