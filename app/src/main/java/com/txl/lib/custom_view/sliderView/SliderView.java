package com.txl.lib.custom_view.sliderView;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.VelocityTracker;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Scroller;

import com.example.txl.tool.R;

import java.util.LinkedHashMap;
import java.util.Map;

/**
 * 仿qq的消息列表侧滑效果
 * 目前只有一个内容view
 */
public class SliderView extends ViewGroup {
    protected String TAG = SliderView.class.getSimpleName();
    /**
     * 左右菜单
     */
    private Map<View, View> mLeftMenuViews = new LinkedHashMap<>();
    private Map<View, View> mRightMenuViews = new LinkedHashMap<>();
    private Map<View, View> mContentViews = new LinkedHashMap<>();
    private int mTotalLeftMenuLength, mTotalRightMenuLength;

    private int mLastXIntercept;
    private int mLastYIntercept;

    private int mLastX;
    private int mLastY;

    private Scroller mScroller;
    private VelocityTracker mVelocityTracker;

    public SliderView(Context context) {
        super(context);
        init();
    }

    public SliderView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public SliderView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    public SliderView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        init();
    }

    private void init(){
        mScroller = new Scroller(getContext());
        mVelocityTracker = VelocityTracker.obtain();
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        layoutContentView(l, t, r, b);
        layoutLeftMenu(l, t, r, b);
        LayoutRightMenu(l, t, r, b);
    }

    private void layoutLeftMenu(int l, int t, int r, int b) {
        final int paddingTop = getPaddingTop();
        final int paddingLeft = getPaddingLeft();

        int leftStart = paddingLeft - mTotalLeftMenuLength;
        for (View v : mLeftMenuViews.keySet()) {
            final LayoutParams lp = (LayoutParams) v.getLayoutParams();
            int top = paddingTop + lp.topMargin;
            int bottom = top + v.getMeasuredHeight();
            int left = leftStart + lp.leftMargin;
            int right = left + v.getMeasuredWidth();
            v.layout(left, top, right, bottom);
            leftStart = right + lp.rightMargin;
        }
    }

    private void LayoutRightMenu(int l, int t, int r, int b) {
        final int paddingTop = getPaddingTop();
        final int paddingRight = getPaddingRight();

        int leftStart = getMeasuredWidth() - paddingRight;
        for (View v : mRightMenuViews.keySet()) {
            final LayoutParams lp = (LayoutParams) v.getLayoutParams();
            int top = paddingTop + lp.topMargin;
            int bottom = top + v.getMeasuredHeight();
            int left = leftStart + lp.leftMargin;
            int right = left + v.getMeasuredWidth();
            v.layout(left, top, right, bottom);
            leftStart = right + lp.rightMargin;
        }
    }

    private void layoutContentView(int l, int t, int r, int b) {
        final int paddingLeft = getPaddingLeft();
        final int paddingTop = getPaddingTop();

        //第一个内容子元素开始的位置是父容器的左padding
        int leftStart = paddingLeft;
        for (View v : mContentViews.keySet()) {
            final LayoutParams lp = (LayoutParams) v.getLayoutParams();
            int top = paddingTop + lp.topMargin;
            int bottom = top + v.getMeasuredHeight();
            int left = leftStart + lp.leftMargin;
            int right = left + v.getMeasuredWidth();
            v.layout(left, top, right, bottom);
            leftStart = right + lp.rightMargin;
        }
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        final int paddingLeft = getPaddingLeft();
        final int paddingRight = getPaddingRight();
        final int paddingTop = getPaddingTop();
        final int paddingBottom = getPaddingBottom();
        int maxWidth = 0;
        int maxHeight = 0;
        int childState = 0;
        //测量内容
        for (View v : mContentViews.keySet()) {
            measureChildWithMargins(v, widthMeasureSpec, 0, heightMeasureSpec, 0);
            final LayoutParams lp = (LayoutParams) v.getLayoutParams();
            maxWidth += v.getMeasuredWidth() + lp.leftMargin + lp.rightMargin;
            maxHeight = Math.max(maxHeight, v.getMeasuredHeight() + lp.topMargin + lp.bottomMargin);
            childState = combineMeasuredStates(childState, v.getMeasuredState());
        }

        // Account for padding too
        maxWidth += paddingLeft + paddingRight;
        maxHeight += paddingTop + paddingBottom;

        // Check against our minimum height and width
        maxHeight = Math.max(maxHeight, getSuggestedMinimumHeight());
        maxWidth = Math.max(maxWidth, getSuggestedMinimumWidth());

        final Drawable drawable = getBackground();
        if (drawable != null) {
            maxHeight = Math.max(maxHeight, drawable.getMinimumHeight());
            maxWidth = Math.max(maxWidth, drawable.getMinimumWidth());
        }
        setMeasuredDimension(resolveSizeAndState(maxWidth, widthMeasureSpec, childState), resolveSizeAndState(maxHeight, heightMeasureSpec, childState));

        //测量左右菜单
        for (View v : mLeftMenuViews.keySet()) {
            if (v.getVisibility() != GONE) {
                measureMenu(v, heightMeasureSpec, widthMeasureSpec);
                LayoutParams layoutParams = (LayoutParams) v.getLayoutParams();
                mTotalLeftMenuLength += layoutParams.leftMargin + layoutParams.rightMargin + v.getMeasuredWidth();
            }
        }
        for (View v : mRightMenuViews.keySet()) {
            if (v.getVisibility() != GONE) {
                measureMenu(v, heightMeasureSpec, widthMeasureSpec);
                LayoutParams layoutParams = (LayoutParams) v.getLayoutParams();
                mTotalRightMenuLength += layoutParams.leftMargin + layoutParams.rightMargin + v.getMeasuredWidth();
            }
        }

    }

    private void measureMenu(View v, int heightMeasureSpec, int widthMeasureSpec) {
        final LayoutParams lp = (LayoutParams) v.getLayoutParams();
        //父容器不对菜单的宽度进行任何限制
        final int childWidthMeasureSpec = MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED);
        final int childHeightMeasureSpec = getChildMeasureSpec(heightMeasureSpec,
                getPaddingTop() + getPaddingBottom() + lp.topMargin + lp.bottomMargin, lp.height);

        v.measure(childWidthMeasureSpec, childHeightMeasureSpec);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        Log.d(TAG,"onTouchEvent "+ event.getAction());
        mVelocityTracker.addMovement(event);
        int x = (int) event.getX();
        int y = (int) event.getY();
        switch (event.getAction()){
            case MotionEvent.ACTION_DOWN:{
                if(!mScroller.isFinished()){
                    mScroller.abortAnimation();
                }
                break;
            }
            case MotionEvent.ACTION_MOVE:{
                int deltaX = x - mLastX;
                int deltaY = y - mLastY;
                scrollBy(-deltaX,0);
                break;
            }
            case MotionEvent.ACTION_UP:{
                int scrollX = getScrollX();
//                int scrollToChildIndex = scrollX / mChildWidth;
                mVelocityTracker.computeCurrentVelocity(1000);
                float xVelocity = mVelocityTracker.getXVelocity();
                if(xVelocity >= 50){

                }
                mVelocityTracker.clear();
                break;
            }
        }
        mLastX = x;
        mLastY = y;
        return true;
    }

    @Override
    public boolean onInterceptTouchEvent(MotionEvent ev) {
        Log.d(TAG,"onInterceptTouchEvent "+ ev.getAction());
        boolean intercepted = false;
        int x = (int) ev.getX();
        int y = (int) ev.getY();
        switch (ev.getAction()){
            case MotionEvent.ACTION_DOWN:{
                intercepted = false;
                if(!mScroller.isFinished()){
                    mScroller.abortAnimation();
                    intercepted = true;
                }
                break;
            }
            case MotionEvent.ACTION_MOVE:{
                int deltaX = x - mLastXIntercept;
                int deltaY = y - mLastYIntercept;
                if(Math.abs(deltaX) > Math.abs(deltaY)){
                    intercepted = true;
                }else {
                    intercepted = false;
                }
                break;
            }
            case MotionEvent.ACTION_UP:{
                intercepted = false;
                break;
            }
        }
        mLastX = x;
        mLastY = y;
        mLastXIntercept = x;
        mLastYIntercept = y;
        Log.d(TAG,"onInterceptTouchEvent "+ev.getAction() + " intercepted :: "+intercepted);
        return intercepted;
    }

    @Override
    public LayoutParams generateLayoutParams(AttributeSet attrs) {
        return new LayoutParams(getContext(), attrs);
    }

    @Override
    protected LayoutParams generateDefaultLayoutParams() {
        return new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
    }

    @Override
    protected LayoutParams generateLayoutParams(ViewGroup.LayoutParams lp) {
        return new LayoutParams(lp);
    }


    @Override
    public void addView(View child, int index, ViewGroup.LayoutParams params) {
        if (child == null) {
            throw new IllegalStateException("addView child is null");
        }
        if (params == null) {
            throw new IllegalStateException("addView params is null");
        }
        if (params instanceof LayoutParams) {
            LayoutParams layoutParams = (LayoutParams) params;
            switch (layoutParams.childType) {
                case leftMenu: {
                    mLeftMenuViews.put(child, child);
                    break;
                }
                case rightMenu: {
                    mRightMenuViews.put(child, child);
                    break;
                }
                default: {
                    mContentViews.put(child, child);
                    break;
                }
            }
        }
        super.addView(child, index, params);
    }

    @Override
    public void onViewRemoved(View child) {
        super.onViewRemoved(child);
        mLeftMenuViews.remove(child);
        mRightMenuViews.remove(child);
        mContentViews.remove(child);
    }

    public static class LayoutParams extends MarginLayoutParams {
        ChildType childType;

        public LayoutParams(Context c, AttributeSet attrs) {
            super(c, attrs);
            TypedArray a = c.obtainStyledAttributes(attrs, R.styleable.SliderView_Layout);
            childType = ChildType.fromIndex(a.getInt(R.styleable.SliderView_Layout_layout_type, 2));
            a.recycle();
        }

        public LayoutParams(int width, int height) {
            super(width, height);
        }

        public LayoutParams(MarginLayoutParams source) {
            super(source);
        }

        public LayoutParams(ViewGroup.LayoutParams source) {
            super(source);
        }

        enum ChildType {
            leftMenu,
            rightMenu,
            content;

            static ChildType fromIndex(int index) {
                switch (index) {
                    case 0:
                        return leftMenu;
                    case 1:
                        return rightMenu;
                    case 2:
                        return content;
                }
                return content;
            }
        }
    }
}
