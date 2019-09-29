package com.txl.lib.custom_view.sliderView;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import androidx.core.view.ViewCompat;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.VelocityTracker;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EdgeEffect;
import android.widget.Scroller;

import com.example.txl.tool.R;

import java.util.LinkedHashMap;
import java.util.Map;

/**
 * 仿qq的消息列表侧滑效果
 * 目前只有一个内容view
 */
public class SliderView extends ViewGroup {
    private static final boolean DEBUG = true;
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

    /**
     * 用于实现左右滑动到末端的水波纹效果
     * */
    private EdgeEffect mLeftEdge;
    private EdgeEffect mRightEdge;

    // Offsets of the first and last items, if known.
    // Set during population, used to determine if we are at the beginning
    // or end of the pager data set during touch scrolling.
    private float mFirstOffset = -Float.MAX_VALUE;
    private float mLastOffset = Float.MAX_VALUE;

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
        setWillNotDraw(false);
        mScroller = new Scroller(getContext());
        mVelocityTracker = VelocityTracker.obtain();
        final Context context = getContext();
        mLeftEdge = new EdgeEffect(context);
        mRightEdge = new EdgeEffect(context);
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
        if(DEBUG) Log.d(TAG,"leftStart  ： "+leftStart);
        for (View v : mLeftMenuViews.keySet()) {
            final LayoutParams lp = (LayoutParams) v.getLayoutParams();
            int top = paddingTop + lp.topMargin;
            int bottom = top + v.getMeasuredHeight();
            int left = leftStart - lp.leftMargin;
            int right = left + v.getMeasuredWidth();
            if(DEBUG) Log.d(TAG,"mTotalLeftMenuLength :"+mTotalLeftMenuLength+"   测量宽高：：：： "+v.getMeasuredWidth()+"   "+v.getMeasuredHeight()+" 位置：  "+String.format("l:%d t:%d  r:%d  b%d  :",left,top,right,bottom));
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
            if(v.getVisibility() == GONE){
                continue;
            }
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
        final int mMaxMenuLength = MeasureSpec.getSize(resolveSizeAndState(maxWidth, widthMeasureSpec, childState));
        int myWidthMeasureSpec = MeasureSpec.makeMeasureSpec(maxWidth, MeasureSpec.UNSPECIFIED);
        //暂时让宽度在区域内
        setMeasuredDimension(resolveSizeAndState(maxWidth, widthMeasureSpec, childState), resolveSizeAndState(maxHeight, heightMeasureSpec, childState));

        //测量左右菜单
        mTotalLeftMenuLength = 0;
        for (View v : mLeftMenuViews.keySet()) {
            if (v.getVisibility() != GONE) {
                measureMenu(v, heightMeasureSpec, widthMeasureSpec);
                LayoutParams layoutParams = (LayoutParams) v.getLayoutParams();
                mTotalLeftMenuLength += layoutParams.leftMargin + layoutParams.rightMargin + v.getMeasuredWidth();
                if(mTotalLeftMenuLength>mMaxMenuLength){
                    mTotalLeftMenuLength = mMaxMenuLength;
                    break;
                }
            }
        }
        mTotalRightMenuLength = 0;
        for (View v : mRightMenuViews.keySet()) {
            if (v.getVisibility() != GONE) {
                measureMenu(v, heightMeasureSpec, widthMeasureSpec);
                LayoutParams layoutParams = (LayoutParams) v.getLayoutParams();
                mTotalRightMenuLength += layoutParams.leftMargin + layoutParams.rightMargin + v.getMeasuredWidth();
                if(mTotalRightMenuLength>mMaxMenuLength){
                    mTotalRightMenuLength = mMaxMenuLength;
                    break;
                }
            }
        }

    }

    // FIXME: 2019/6/19 将左右菜单的最大宽度修改成内容的宽度
    private void measureMenu(View v, int heightMeasureSpec, int widthMeasureSpec) {
        final LayoutParams lp = (LayoutParams) v.getLayoutParams();
        //父容器不对菜单的宽度进行任何限制
        int childWidthMeasureSpec = MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED);
        childWidthMeasureSpec = getChildMeasureSpec(childWidthMeasureSpec, lp.leftMargin + lp.rightMargin, lp.width);
        final int childHeightMeasureSpec = getChildMeasureSpec(heightMeasureSpec,
                getPaddingTop() + getPaddingBottom() + lp.topMargin + lp.bottomMargin, lp.height);

        v.measure(childWidthMeasureSpec, childHeightMeasureSpec);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
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
//                int deltaY = y - mLastY;
                //内容在View边界的右边 mScrollX = view的左边缘 - view内容的左边缘
                int scrollX = getScrollX();
                //本次滑动的方向 deltaX>0 从左向右边拉
                boolean scrollLeft = deltaX > 0;
                // scrollX > 0 右菜单内容处于显示状态
                boolean scrollStateLeft = scrollX > 0;
                if(scrollLeft == scrollStateLeft){
                    /**
                     * scrollBy x参数为正的时候是由右边向左边滑动，为负的时候是由左边向右边滑动。
                     * */
                    scrollBy(-deltaX,0);
                }else if(scrollLeft && !scrollStateLeft){
                    if(mTotalLeftMenuLength>Math.abs(scrollX-deltaX)){
                        scrollBy(-deltaX,0);
                    }else if(mTotalLeftMenuLength<Math.abs(scrollX-deltaX) && (mTotalLeftMenuLength>Math.abs(scrollX))){
                        //因为前面的计算这里 scrollX一定是负值
                        scrollBy(-mTotalLeftMenuLength-scrollX,0);
                    }else {
                        mLeftEdge.onPull(Math.abs(deltaX) *1.0f/ getClientWidth());
                        ViewCompat.postInvalidateOnAnimation(this);
                        if(DEBUG) Log.d(TAG,"onTouchEvent 右 "+mLeftEdge.isFinished());
                    }
                }else if(!scrollLeft && scrollStateLeft){
                    if(mTotalRightMenuLength>Math.abs(scrollX-deltaX)){
                        scrollBy(-deltaX,0);
                    }else if(mTotalRightMenuLength<Math.abs(scrollX-deltaX) && (mTotalRightMenuLength>Math.abs(scrollX))){
                        scrollBy(mTotalRightMenuLength-scrollX,0);
                    }else {
                        mRightEdge.onPull(Math.abs(deltaX) *1.0f / getClientWidth());
                        ViewCompat.postInvalidateOnAnimation(this);
                        if(DEBUG) Log.d(TAG,"onTouchEvent mRightEdge 左 "+mRightEdge.isFinished());
                    }
                }
                break;
            }
            case MotionEvent.ACTION_UP:{
                mLeftEdge.onRelease();
                mRightEdge.onRelease();
//                int scrollToChildIndex = scrollX / mChildWidth;
                mVelocityTracker.computeCurrentVelocity(1000);
//                float xVelocity = mVelocityTracker.getXVelocity();
                completeScroll();
                mVelocityTracker.clear();
                break;
            }
        }
        mLastX = x;
        mLastY = y;
        return true;
    }

    private void completeScroll() {
        int scrollX = getScrollX();
        if(scrollX > 72){
//            scrollTo(mTotalRightMenuLength,0);
            mScroller.startScroll(scrollX,0,mTotalRightMenuLength-scrollX,0);
        }else if(scrollX < -72){
//            scrollTo(-mTotalLeftMenuLength,0);
            mScroller.startScroll(scrollX,0,-mTotalLeftMenuLength-scrollX,0);
        }else {
//            scrollTo(0,0);
            mScroller.startScroll(scrollX,0,-scrollX,0);
        }
        ViewCompat.postInvalidateOnAnimation(this);
    }

    @Override
    public void computeScroll() {
        if(mScroller != null && mScroller.computeScrollOffset()){
            scrollTo(mScroller.getCurrX(),mScroller.getCurrY());
            ViewCompat.postInvalidateOnAnimation(this);
        }
    }

    private int getClientWidth() {
        return getMeasuredWidth() - getPaddingLeft() - getPaddingRight();
    }

    @Override
    public boolean onInterceptTouchEvent(MotionEvent ev) {
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
                if(canScroll(this,false,deltaX,x,y)){
                    return false;
                }
                intercepted = Math.abs(deltaX) > Math.abs(deltaY);
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
        return intercepted;
    }

    /**
     * 这个是ViewPager实现先滑动子view在滑动自身的关键
     * Tests scrollability within child views of v given a delta of dx.
     *
     * @param v View to test for horizontal scrollability
     * @param checkV Whether the view v passed should itself be checked for scrollability (true),
     *               or just its children (false).
     * @param dx Delta scrolled in pixels
     * @param x X coordinate of the active touch point
     * @param y Y coordinate of the active touch point
     * @return true if child views of v can be scrolled by delta of dx.
     */
    protected boolean canScroll(View v, boolean checkV, int dx, int x, int y) {
        if (v instanceof ViewGroup) {
            final ViewGroup group = (ViewGroup) v;
            final int scrollX = v.getScrollX();
            final int scrollY = v.getScrollY();
            final int count = group.getChildCount();
            // Count backwards - let topmost views consume scroll distance first.
            for (int i = count - 1; i >= 0; i--) {
                // This will not work for transformed views in Honeycomb+
                final View child = group.getChildAt(i);
                if (x + scrollX >= child.getLeft() && x + scrollX < child.getRight()
                        && y + scrollY >= child.getTop() && y + scrollY < child.getBottom()
                        && canScroll(child, true, dx, x + scrollX - child.getLeft(),
                        y + scrollY - child.getTop())) {
                    return true;
                }
            }
        }

        return checkV && v.canScrollHorizontally(-dx);
    }


    private static final double ANGLE = Math.PI / 6;
    private static final float SIN = (float) Math.sin(ANGLE);
    private static final float COS = (float) Math.cos(ANGLE);
    private static final float RADIUS_FACTOR = 0.6f;
    private void textSize(int width, int height){
        final float r = width * RADIUS_FACTOR / SIN;
        final float y = COS * r;
        final float h = r - y;
        final float or = height * RADIUS_FACTOR / SIN;
        final float oy = COS * or;
        final float oh = or - oy;

        Log.d(TAG,"textSize  width :: "+width+"  height :: "+height+" hh "+h+"   min"+Math.min(height, h));
    }

    @Override
    public void draw(Canvas canvas) {
        super.draw(canvas);
        boolean needsInvalidate = false;
        if (!mLeftEdge.isFinished()) {
            final int restoreCount = canvas.save();
            final int width = getWidth()-getPaddingLeft()-getPaddingRight();
            final int height = getHeight()-getPaddingTop()-getPaddingBottom();

            canvas.rotate(270);
            canvas.translate(-height - getPaddingTop(), -mTotalLeftMenuLength+getPaddingLeft());
            mLeftEdge.setSize(height, getWidth());
            textSize(height, width);
            needsInvalidate |= mLeftEdge.draw(canvas);
            canvas.restoreToCount(restoreCount);
        }
        if (!mRightEdge.isFinished()) {
            final int restoreCount = canvas.save();
            final int width = getWidth()-getPaddingLeft()-getPaddingRight();
            final int height = getHeight()-getPaddingTop()-getPaddingBottom();
            Paint paint = new Paint();
            paint.setColor(Color.RED);
            canvas.drawRect(0,0,50,50,paint);
            canvas.rotate(90);
            paint.setColor(Color.BLUE);
            canvas.drawRect(0,0,50,50,paint);
            canvas.translate(getPaddingTop(), -width-mTotalRightMenuLength-getPaddingLeft());
            paint.setColor(Color.BLACK);
            canvas.drawRect(0,0,50,50,paint);
            mRightEdge.setSize(height, width);
            needsInvalidate |= mRightEdge.draw(canvas);
            canvas.restoreToCount(restoreCount);
        }
        if (needsInvalidate) {
            // Keep animating
            ViewCompat.postInvalidateOnAnimation(this);
        }
    }

    @Override
    protected void onDetachedFromWindow() {
        if ((mScroller != null) && !mScroller.isFinished()) {
            mScroller.abortAnimation();
        }
        super.onDetachedFromWindow();
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
