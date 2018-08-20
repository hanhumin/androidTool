package com.example.txl.tool.custom;

import android.content.Context;
import android.graphics.Canvas;
import android.support.v7.widget.PagerSnapHelper;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Scroller;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/18
 * description：
 */
public class CustomBannerView extends ViewGroup {
    private static final String TAG = "CustomBannerView";

    private int childrenCount;
    private int childWidth;
    private int childHeight;
    /**
     * 当前处于轮播图的位置
     * */
    private int currentIndex = 0;

    private Scroller mScroller;

    public CustomBannerView(Context context) {
        this( context ,null);
    }

    public CustomBannerView(Context context, AttributeSet attrs) {
        this( context, attrs, 0 );
    }

    public CustomBannerView(Context context, AttributeSet attrs, int defStyleAttr) {
        super( context, attrs, defStyleAttr );
        mScroller = new Scroller( context );
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure( widthMeasureSpec, heightMeasureSpec );
        childWidth = getMeasuredWidth();
        childHeight = getMeasuredHeight();
        childrenCount = getChildCount();
//        if(0 == childrenCount){
//            setMeasuredDimension( 0,0 );
//        }else {
//            View view = getChildAt( 0 );
            measureChildren( widthMeasureSpec,heightMeasureSpec );
//            childHeight = view.getMeasuredHeight();
//            childWidth = view.getMeasuredWidth() ;
//            setMeasuredDimension( childWidth*childrenCount,childHeight );
//        }
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        if(changed){
            int leftMargin = 0;
            for(int i=0; i<childrenCount; i++){
                View view = getChildAt( i );
                view.layout( leftMargin, 0, leftMargin+childWidth, childHeight );
                leftMargin += childWidth;
            }
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw( canvas );

    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent ev){
        return super.dispatchTouchEvent( ev );
    }

    @Override
    public boolean onInterceptTouchEvent(MotionEvent ev) {
        return true;
    }

    int mLastX = 0, mLastY = 0;
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int x = (int) event.getRawX();
        int y = (int) event.getRawY();
        switch (event.getAction()){
            case MotionEvent.ACTION_DOWN:
                if(!mScroller.isFinished()){
                    mScroller.abortAnimation();
                }
                return true;
            case MotionEvent.ACTION_MOVE:
                int deltaX = (int) (event.getRawX() -x);
                int deltaY = (int) (event.getRawY() - y);
                Log.d( TAG,"move, deltaX:"+deltaX+" deltaY:"+deltaY +"  childrenCount:"+childrenCount);
                scrollBy( 20,0 );
                break;
            case MotionEvent.ACTION_UP:
                int mScrollX = getScrollX();
                currentIndex = (mScrollX)/childWidth+1;
                if(currentIndex < 0){
                    currentIndex = childrenCount-1;
                }else if(currentIndex >childrenCount-1){
                    currentIndex = 0;
                }
                Log.d( TAG,"move, mScrollX:"+mScrollX+"  currentIndex:"+currentIndex);
//                scrollTo( currentIndex+childWidth,0 );
//                scrollBy( currentIndex *(x - mLastX),0 );
                int scrollToX = currentIndex * childWidth;
                smoothScrollTo(scrollToX,getScrollY());
                break;
        }
        mLastX = x;
        mLastY = y;
        return true;
    }

    private void smoothScrollTo(int destX, int destY){
        int scrollX = getScrollX();
        int scrollY = getScrollY();
        int deltaX = destX - scrollX;
        int deltaY = destY - scrollY;
        mScroller.startScroll( scrollX,scrollY,deltaX,deltaY,1000 );
        invalidate();
    }

    @Override
    public void computeScroll() {
        if(mScroller.computeScrollOffset()){
            scrollTo( mScroller.getCurrX(),mScroller.getCurrY() );
        }
    }
}
