package com.example.txl.tool.hencoder;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.Rect;
import android.graphics.RectF;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.View;

import com.example.txl.tool.R;

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
//        drawArc(canvas,new Paint(  ));
//        drawBitmap(canvas,new Paint(  ));

//        drawBitmap(canvas,new Rect( 30,30,300,300 ),new Rect( 50,50,200,200 ),new Paint(  ));
//        drawBitmap(canvas,new Rect( 30,30,300,300 ),new Rect( 50,50,getRight(),getBottom() ),new Paint(  ));
//        drawBitmap(canvas,null,new Rect( 50,50,200,200 ),new Paint(  ));

        drawBitmap( canvas,30,30,new Paint(  ) );
        drawCirle(canvas);
        drawColor(canvas);
        drawColorWithPorterDuff(canvas);
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


    private void drawBitmap(Canvas canvas, Paint paint){
        Bitmap bitmap = BitmapFactory.decodeResource( getResources(), R.drawable.draw_bitmap );
        Matrix bitmapMatrix = new Matrix(  );
//        bitmapMatrix.setRotate( 15 );
        //保留后面的矩阵变化效果
        bitmapMatrix.postRotate( 15 );
        //保留前面的矩阵变化效果
        bitmapMatrix.preTranslate( 30,30 );
//        bitmapMatrix.setTranslate( 30,30 );
        canvas.drawBitmap( bitmap, bitmapMatrix ,paint);
    }


    /**
     * @param src 取目标图片的一个矩阵区域进行显示,如果是空则默认取全部
     * @param dst 图片显示的区域
     * */
    private void drawBitmap(Canvas canvas,  Rect src, Rect dst, Paint paint){
        Bitmap bitmap = BitmapFactory.decodeResource( getResources(), R.drawable.draw_bitmap );
        canvas.drawBitmap( bitmap,src,dst,paint );
    }

    private void drawBitmap(Canvas canvas,  float x, float y, Paint paint){
        Bitmap bitmap = BitmapFactory.decodeResource( getResources(), R.drawable.draw_bitmap );
        canvas.drawBitmap( bitmap,x,y,paint );
    }


    private final int WIDTH = 300;
    private final int HEIGHT = 300;
    private int orig[], verts[];
    private void drawBitmapMesh(Canvas canvas, Paint paint){
        Bitmap bitmap = BitmapFactory.decodeResource( getResources(), R.drawable.draw_bitmap );
        float bitmapWidth = bitmap.getWidth();
        float bitmapHeight = bitmap.getHeight();
        int index = 0;
        for (int y=0; y<=HEIGHT; y++){
            float fy = bitmapHeight * y/HEIGHT;
            for (int x=0; x<=WIDTH; x++){
                float fx = bitmapWidth * x / WIDTH;
//                orig[index*2+0] =
            }
        }
//        canvas.drawBitmapMesh( bitmap, );
    }

    private void drawCirle(Canvas canvas){
        canvas.drawCircle(getWidth()/2,getHeight()/2,30,new Paint());
    }

    /**
     * 填充背景色
     * */
    private void drawColor(Canvas canvas){
        canvas.drawColor(Color.BLUE);
    }

    private void drawColorWithPorterDuff(Canvas canvas){
        canvas.drawColor(Color.GRAY, PorterDuff.Mode.ADD);
    }
}
