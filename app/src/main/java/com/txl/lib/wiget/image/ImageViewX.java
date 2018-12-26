package com.txl.lib.wiget.image;

import android.content.Context;
import android.graphics.Canvas;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.widget.ImageView;

/**
 * @author TXL
 * description :多功能ImageView
 */
public class ImageViewX extends ImageView {
    public ImageViewX(Context context) {
        super(context);
    }

    public ImageViewX(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public ImageViewX(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
    }
}
