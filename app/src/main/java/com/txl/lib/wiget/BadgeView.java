package com.txl.lib.wiget;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.support.v4.content.ContextCompat;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

/**
 * @author TXL
 * description :
 * 中间图片 右上角文字
 */
public class BadgeView extends View {
    private String TAG = BadgeView.class.getSimpleName();
    private Drawable mDrawable = null;

    /**
     *图片显示边距
     */
    private int imagePaddingLeft, imagePaddingTop, imagePaddingRight, imagePaddingBottom;

    /**
     * 绘制图片的内容区域
     * */
    private RectF dstRect;

    int imageHeight = -1, imageWidth = -1;

    /**
     * 右上角文本
     * */
    String text = "1";
    /**
     * 文本所需要占用的矩形
     * */
    Rect textRect;
    /**
     * 文本背景内容矩形
     * */
    RectF textBgRectF;
    Paint mTextPaint;
    Paint mTextBgPaint;
    int textSize = 36;
    /**
     * 文本内容额外的宽度
     * */
    int textPadding = 12;

    int rectRadius = 60;

    public BadgeView(Context context) {
        super(context);
    }

    public BadgeView(Context context, @android.support.annotation.Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public BadgeView(Context context, @android.support.annotation.Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @android.support.annotation.RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    public BadgeView(Context context, @android.support.annotation.Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    {
        mTextPaint = new Paint();
        mTextPaint.setColor(Color.WHITE);
        mTextPaint.setAntiAlias(true);
        mTextPaint.setTextSize(textSize);
        mTextBgPaint = new Paint();
        mTextBgPaint.setColor(Color.RED);
        dstRect = new RectF();
        textRect = new Rect();
        textBgRectF = new RectF();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        int widthMode = MeasureSpec.getMode(widthMeasureSpec);
        int heightMode = MeasureSpec.getMode(heightMeasureSpec);
        int widthSize = MeasureSpec.getSize(widthMeasureSpec);
        int heightSize = MeasureSpec.getSize(heightMeasureSpec);
        switch (widthMode){
            case MeasureSpec.UNSPECIFIED:
            case MeasureSpec.EXACTLY:
                break;
            case MeasureSpec.AT_MOST:
                if(imageHeight == -1 || imageWidth == -1){
                    if(mDrawable == null){
                        heightSize = 0;
                    }else {
                        widthSize = mDrawable.getIntrinsicWidth()+imagePaddingLeft+imagePaddingRight;
                    }
                }else {
                    widthSize = imageWidth+imagePaddingLeft+imagePaddingRight;
                }
                break;
        }
        switch (heightMode){
            case MeasureSpec.UNSPECIFIED:
            case MeasureSpec.EXACTLY:
                break;
            case MeasureSpec.AT_MOST:
                if(imageHeight == -1 || imageWidth == -1){
                    if(mDrawable == null){
                        heightSize = 0;
                    }else {
                        heightSize = mDrawable.getIntrinsicHeight()+imagePaddingTop+imagePaddingBottom;
                    }
                }else {
                    heightSize = imageWidth+imagePaddingTop+imagePaddingBottom;
                }
                break;
        }
        setMeasuredDimension(MeasureSpec.makeMeasureSpec(widthSize,widthMode),MeasureSpec.makeMeasureSpec(heightSize,heightMode));
    }

    private int getSize(int measureSpec){
        int result = 0;
        int specMode = MeasureSpec.getMode(measureSpec);
        int specSize = MeasureSpec.getSize(measureSpec);
        switch (specMode){
            case MeasureSpec.UNSPECIFIED:
            case MeasureSpec.EXACTLY:
                result = specSize;
                Log.d(TAG,"getSize MeasureSpec.UNSPECIFIED  "+specSize);
                break;
            case MeasureSpec.AT_MOST:
                Log.d(TAG,"getSize MeasureSpec.AT_MOST  "+specSize);
                result = imageWidth + imagePaddingLeft+imagePaddingRight;
                break;
        }

        return MeasureSpec.makeMeasureSpec(result,specMode);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        if(mDrawable != null){
            Log.d(TAG,"mDrawable not null");
            canvas.save();
            if(imageHeight == -1 || imageWidth == -1){
                mDrawable.setBounds(imagePaddingLeft,imagePaddingTop,getWidth()-imagePaddingRight,getHeight()-imagePaddingBottom);
            }else {
                mDrawable.setBounds(imagePaddingLeft,imagePaddingTop,imagePaddingLeft+imageWidth,imagePaddingTop+imageHeight);
            }
            mDrawable.draw(canvas);
            mTextPaint.getTextBounds(text,0,text.length(), textRect);
            float textWidth = mTextPaint.measureText(text);
            textBgRectF.set(getWidth()-textWidth-textPadding*2,0,getWidth(), textRect.height()+textPadding*2);
            canvas.drawRoundRect(textBgRectF,rectRadius,rectRadius,mTextBgPaint);
            canvas.drawText(text,getWidth()-textWidth-textPadding, textRect.height()+textPadding,mTextPaint);
            canvas.restore();
        }
    }


    public void setImageDrawable(Drawable drawable){
        this.mDrawable = drawable;
        invalidate();
    }

    public void setImageRes(int imageRes){
        mDrawable = ContextCompat.getDrawable(getContext(),imageRes);
        invalidate();
    }

    public void setImagePadding(int imagePaddingLeft,int imagePaddingTop, int imagePaddingRight, int imagePaddingBottom){
        this.imagePaddingLeft = imagePaddingLeft;
        dstRect.left = imagePaddingLeft;
        this.imagePaddingTop = imagePaddingTop;
        dstRect.top = imagePaddingTop;
        this.imagePaddingRight = imagePaddingRight;
        dstRect.right = imagePaddingRight;
        this.imagePaddingBottom = imagePaddingBottom;
        dstRect.bottom = imagePaddingBottom;
    }

    public void setImagePadding(int padding){
        imagePaddingLeft = padding;
        imagePaddingRight = padding;
        imagePaddingTop = padding;
        imagePaddingBottom = padding;
        dstRect.bottom = padding;
        dstRect.top = padding;
        dstRect.left = padding;
        dstRect.right = padding;
    }

    /**
     * @param imageWidth  图片宽
     * @param imageHeight  图片高
     * Notice: 调用了这个方法后 imagePaddingRight,imagePaddingBottom 会失效
     * */
    public void setBadgeImageSize(int imageWidth, int imageHeight){
        this.imageWidth = imageWidth;
        this.imageHeight = imageHeight;
    }

    public void setText(String text) {
        this.text = text;
    }
}
