package com.txl.lib.wiget;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.graphics.drawable.BitmapDrawable;
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
    private Paint mTextPaint;
    private int mTextBgColor;

    /**
     *图片显示边距
     */
    private int imagePaddingLeft, imagePaddingTop, imagePaddingRight, imagePaddingBottom;

    /**
     * 绘制图片的内容区域
     * */
    private RectF dstRect;

    boolean change = true;



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
        dstRect = new RectF();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        if(mDrawable != null){
            Log.d(TAG,"mDrawable not null");
            canvas.save();
            mDrawable.setBounds(0,0,getWidth()/2,getHeight()/3);
            mDrawable.draw(canvas);
            BitmapDrawable bitmapDrawable = (BitmapDrawable) mDrawable;
            if(dstRect == null || change){
                dstRect = new RectF(0,0,getWidth(),getHeight());
            }
//            canvas.drawBitmap(bitmapDrawable.getBitmap(),null, dstRect,mTextPaint);
            canvas.restore();
        }
    }


    public void setImageDrawable(){}

    public void setImageRes(int imageRes){
        mDrawable = ContextCompat.getDrawable(getContext(),imageRes);
        invalidate();
    }

    public void setImagePaddingLeft(int imagePaddingLeft) {
        this.imagePaddingLeft = imagePaddingLeft;
        dstRect.left = imagePaddingLeft;
    }

    public void setImagePaddingTop(int imagePaddingTop) {
        this.imagePaddingTop = imagePaddingTop;
        dstRect.top = imagePaddingTop;
    }

    public void setImagePaddingRight(int imagePaddingRight) {
        this.imagePaddingRight = imagePaddingRight;
        dstRect.right = imagePaddingRight;
    }

    public void setImagePaddingBottom(int imagePaddingBottom) {
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
}
