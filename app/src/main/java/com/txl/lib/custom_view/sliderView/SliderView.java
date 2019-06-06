package com.txl.lib.custom_view.sliderView;

import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;

import com.example.txl.tool.R;

import java.util.HashMap;
import java.util.Map;

/**
 * 仿qq的消息列表侧滑效果
 * 目前只有一个内容view
 */
public class SliderView extends ViewGroup {
    /**
     * 左右菜单
     * fixme 注意removeView的时候对应的左右菜单清除
     */
    private Map<View,View> mLeftMenuViews = new HashMap<>();
    private Map<View,View> mRightMenuViews = new HashMap<>();
    private Map<View,View> mContentViews = new HashMap<>();

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
        //contentView占和父容器一样的宽度
//        mContentView.layout(l,t,r,b);
        int childCount = getChildCount();
        for (int i = 0; i < childCount; i++) {
            View child = getChildAt(i);
            child.layout(l,t,child.getMeasuredWidth(),child.getMeasuredHeight());
        }
    }

    private void layoutLeftMenu(){}

    private void LayoutRightMenu(){}

    private void layoutContentView(int l, int t, int r, int b){

    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        final int widthMode = MeasureSpec.getMode(widthMeasureSpec);
        final int heightMode = MeasureSpec.getMode(heightMeasureSpec);
        int childCount = getChildCount();
        for (int i = 0; i < childCount; i++) {
            View child = getChildAt(i);
            measureChildWithMargins(child,widthMeasureSpec,0,heightMeasureSpec,0);
        }
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
        if ( mContentViews.size() > 0) {
            throw new IllegalStateException("SliderView can host only one content child");
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
                case content:{
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
//            gravity = a.getInt(com.android.internal.R.styleable.LinearLayout_Layout_layout_gravity, -1);

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
                    case 1:
                        return leftMenu;
                    case 2:
                        return rightMenu;
                    case 3:
                        return content;
                }
                return content;
            }
        }
    }
}
