package com.txl.lib.wiget.image;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapShader;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Shader;
import android.graphics.drawable.BitmapDrawable;

import androidx.annotation.Nullable;
import androidx.core.content.ContextCompat;
import android.util.AttributeSet;
import android.widget.ImageView;

import com.example.txl.tool.R;

/**
 * @author TXL
 * description :多功能ImageView
 */
public class ImageViewX extends ImageView {
    Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);

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
//        int radius = Math.min(getWidth()/2,getHeight()/2);
        paint.setColor(Color.RED);
//        canvas.drawCircle(getWidth()/2,getHeight()/2,radius,paint);
//        Path path = new Path();
//        path.addCircle(getWidth()/2,getHeight()/2,radius, Path.Direction.CCW);
//        canvas.save();
//        canvas.skew(0.1f, 0);
//        BitmapDrawable drawable = (BitmapDrawable) ContextCompat.getDrawable(getContext(), R.drawable.content);
//        canvas.drawRect(getLeft()+50,getTop(),getRight()-50,getBottom(),paint);
//        canvas.drawRoundRect(getLeft()+50,getTop(),getRight()-50,getBottom(),45,45,paint);

//        canvas.clipPath(path);
//        canvas.rotate(90,getWidth()/2,getHeight()/2);
//        canvas.drawBitmap(drawable.getBitmap(), getLeft(), getTop(), paint);
//        paint.setARGB(99,126,125,125);
//        canvas.drawCircle(getWidth()/2,getHeight()/2,radius,paint);
//        canvas.restore();
//        drawCircle(canvas);
        drawFY(canvas);
    }

    private void drawCircle(Canvas canvas){
        BitmapDrawable drawable = (BitmapDrawable) ContextCompat.getDrawable(getContext(), R.drawable.content);
        Bitmap bitmap = drawable.getBitmap();
        bitmap = Bitmap.createBitmap(bitmap);

        Shader shader = new BitmapShader(bitmap, Shader.TileMode.CLAMP, Shader.TileMode.CLAMP);
        paint.setShader(shader);
        int radius = Math.min(getWidth()/2,getHeight()/2);
        float centerX = getWidth()/2.0f;
        float centerY = getHeight()/2.0f;
//        canvas.drawBitmap(bitmap,getLeft(),getTop(),paint);
//        canvas.drawRect(getLeft(),getTop(),getRight(),getBottom(),paint);
//        canvas.save();
//
//
//        ColorFilter lightingColorFilter = new LightingColorFilter(0x00ffff, 0x000000);
//        paint.setColorFilter(lightingColorFilter);
//        canvas.drawRect(getLeft(),getTop(),getRight(),getBottom(),paint);
//        canvas.restore();
        canvas.save();
        Shader shader1 = new BitmapShader(Bitmap.createBitmap(drawable.getBitmap()), Shader.TileMode.CLAMP, Shader.TileMode.CLAMP);
        paint.setShader(shader1);
        canvas.translate(centerX-radius,centerY-radius);
//        canvas.rotate(60,radius,radius);
        canvas.drawCircle(radius,radius, radius, paint);
        canvas.restore();

    }

    /***
     * 翻页效果
     * */
    private void drawFY(Canvas canvas){
        Matrix matrix = new Matrix();
        float pointsSrc[] = {getWidth()/2, getTop(), getWidth()/2, getBottom(), getRight(), getTop(), getRight(), getBottom()};
        float pointsDst[] = {getWidth()/2, getTop(), getWidth()/2, getBottom(), getBottom(), getRight()-50, getTop(), getRight()-50, getBottom()};
        BitmapDrawable drawable = (BitmapDrawable) ContextCompat.getDrawable(getContext(), R.drawable.content);
        matrix.reset();
        matrix.setPolyToPoly(pointsSrc, 0, pointsDst, 0, 4);

        canvas.save();
        canvas.concat(matrix);
        canvas.drawBitmap(drawable.getBitmap(), getLeft(), getTop(), paint);
        canvas.restore();
    }


}
