package com.txl.lib.custom_view.sliderView;

import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;

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
    private Map<View,View> mLeftMenuViews = new LinkedHashMap<>();
    private Map<View,View> mRightMenuViews = new LinkedHashMap<>();
    private Map<View,View> mContentViews = new LinkedHashMap<>();

    public SliderView(Context context) {
        super(context);
    }

    public SliderView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public SliderView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public SliderView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        layoutContentView(l,t,r,b);
    }

    private void layoutLeftMenu(){}

    private void LayoutRightMenu(){}

    private void layoutContentView(int l, int t, int r, int b){
        Log.d(TAG,"layoutContentView ");
        final int paddingLeft = getPaddingLeft();
        final int paddingRight = getPaddingRight();
        final int paddingTop = getPaddingTop();
        final int paddingBottom = getPaddingBottom();
        //第一个内容子元素开始的位置是父容器的左padding
        int leftStart = paddingLeft;
        for (View v : mContentViews.keySet()) {
            final LayoutParams lp = (LayoutParams) v.getLayoutParams();
            int top = paddingTop+lp.topMargin;
            int bottom = top+v.getMeasuredHeight();
            int left = leftStart + lp.leftMargin;
            int right = left + v.getMeasuredWidth();
            v.layout(left,top,right,bottom);
            leftStart = right+lp.rightMargin;
        }
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {

        //测量内容
        Log.d(TAG,"onMeasure mContentViews size is "+mContentViews.size());
        for (View v : mContentViews.keySet()) {
            measureChildWithMargins(v, widthMeasureSpec, 0, heightMeasureSpec, 0);
//            measureChild(v,widthMeasureSpec,heightMeasureSpec);
            Log.d(TAG,"onMeasure view de 测量宽度 ：： "+v.getMeasuredWidth());
        }
        //测量左右菜单
        for (View v : mLeftMenuViews.keySet()) {
//            measureMenu(v, heightMeasureSpec, widthMeasureSpec);
        }
        for (View v : mRightMenuViews.keySet()) {
//            measureMenu(v, heightMeasureSpec, widthMeasureSpec);
        }
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
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
        if(child == null){
            throw new IllegalStateException("addView child is null");
        }
        if(params == null){
            throw new IllegalStateException("addView params is null");
        }
        if(params instanceof LayoutParams){
            LayoutParams layoutParams = (LayoutParams) params;
            switch (layoutParams.childType){
                case leftMenu:{
                    mLeftMenuViews.put(child,child);
                    break;
                }
                case rightMenu:{
                    mRightMenuViews.put(child,child);
                    break;
                }
                default:{
                    mContentViews.put(child,child);
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

    public static class LayoutParams extends MarginLayoutParams{
        ChildType childType;
        public LayoutParams(Context c, AttributeSet attrs) {
            super(c, attrs);
            TypedArray a = c.obtainStyledAttributes(attrs, R.styleable.SliderView_Layout);
            childType = ChildType.fromIndex(a.getInt(R.styleable.SliderView_Layout_layout_type,2));
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

        enum ChildType{
            leftMenu,
            rightMenu,
            content;

            static ChildType fromIndex(int index){
                switch (index){
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
