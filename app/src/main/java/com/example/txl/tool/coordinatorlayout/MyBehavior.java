package com.example.txl.tool.coordinatorlayout;

import android.content.Context;
import android.graphics.Rect;
import android.os.Parcelable;
import android.support.annotation.NonNull;
import android.support.design.widget.CoordinatorLayout;
import android.support.v4.view.WindowInsetsCompat;
import android.support.v7.widget.RecyclerView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

/**
 * create by txl
 * at 2018/9/21
 * description :
 */
public class MyBehavior extends CoordinatorLayout.Behavior {
    private static final String TAG = "MyBehavior";

    public MyBehavior() {
        super();
    }

    public MyBehavior(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public boolean layoutDependsOn(CoordinatorLayout parent, View child, View dependency) {
        return dependency instanceof RecyclerView;
    }

    @Override
    public boolean onDependentViewChanged(CoordinatorLayout parent, View child, View dependency) {
        Log.d(TAG, "onDependentViewChanged");

        int top = dependency.getScrollY();
        int left = dependency.getScrollX();
        Log.d(TAG,"top:"+top+"  left:"+left);
        CoordinatorLayout.LayoutParams params = (CoordinatorLayout.LayoutParams) child.getLayoutParams();
        child.scrollTo(0,-top);
        return true;
    }

    @Override
    public void onNestedScroll(@NonNull CoordinatorLayout coordinatorLayout, @NonNull View child, @NonNull View target, int dxConsumed, int dyConsumed, int dxUnconsumed, int dyUnconsumed, int type) {
        Log.d(TAG, "onNestedScroll");
        super.onNestedScroll(coordinatorLayout, child, target, dxConsumed, dyConsumed, dxUnconsumed, dyUnconsumed, type);
    }

    @Override
    public void onAttachedToLayoutParams(@NonNull CoordinatorLayout.LayoutParams params) {
        super.onAttachedToLayoutParams(params);
    }

    @Override
    public void onDetachedFromLayoutParams() {
        super.onDetachedFromLayoutParams();
    }

    @Override
    public boolean onInterceptTouchEvent(CoordinatorLayout parent, View child, MotionEvent ev) {
        Log.d(TAG, "onInterceptTouchEvent");
        return super.onInterceptTouchEvent(parent, child, ev);
    }

    @Override
    public boolean onTouchEvent(CoordinatorLayout parent, View child, MotionEvent ev) {
        return super.onTouchEvent(parent, child, ev);
    }

    @Override
    public int getScrimColor(CoordinatorLayout parent, View child) {
        return super.getScrimColor(parent, child);
    }

    @Override
    public float getScrimOpacity(CoordinatorLayout parent, View child) {
        return super.getScrimOpacity(parent, child);
    }

    @Override
    public boolean blocksInteractionBelow(CoordinatorLayout parent, View child) {
        return super.blocksInteractionBelow(parent, child);
    }

    @Override
    public void onDependentViewRemoved(CoordinatorLayout parent, View child, View dependency) {
        super.onDependentViewRemoved(parent, child, dependency);
    }

    @Override
    public boolean onMeasureChild(CoordinatorLayout parent, View child, int parentWidthMeasureSpec, int widthUsed, int parentHeightMeasureSpec, int heightUsed) {
        return super.onMeasureChild(parent, child, parentWidthMeasureSpec, widthUsed, parentHeightMeasureSpec, heightUsed);
    }

    @Override
    public boolean onLayoutChild(CoordinatorLayout parent, View child, int layoutDirection) {
        return super.onLayoutChild(parent, child, layoutDirection);
    }

    @Override
    public boolean onStartNestedScroll(@NonNull CoordinatorLayout coordinatorLayout, @NonNull View child, @NonNull View directTargetChild, @NonNull View target, int axes, int type) {
        return super.onStartNestedScroll(coordinatorLayout, child, directTargetChild, target, axes, type);
    }

    @Override
    public void onNestedScrollAccepted(@NonNull CoordinatorLayout coordinatorLayout, @NonNull View child, @NonNull View directTargetChild, @NonNull View target, int axes, int type) {
        super.onNestedScrollAccepted(coordinatorLayout, child, directTargetChild, target, axes, type);
    }

    @Override
    public void onStopNestedScroll(@NonNull CoordinatorLayout coordinatorLayout, @NonNull View child, @NonNull View target, int type) {
        super.onStopNestedScroll(coordinatorLayout, child, target, type);
    }

    @Override
    public void onNestedPreScroll(@NonNull CoordinatorLayout coordinatorLayout, @NonNull View child, @NonNull View target, int dx, int dy, @NonNull int[] consumed, int type) {
        super.onNestedPreScroll(coordinatorLayout, child, target, dx, dy, consumed, type);
    }

    @Override
    public boolean onNestedFling(@NonNull CoordinatorLayout coordinatorLayout, @NonNull View child, @NonNull View target, float velocityX, float velocityY, boolean consumed) {
        return super.onNestedFling(coordinatorLayout, child, target, velocityX, velocityY, consumed);
    }

    @Override
    public boolean onNestedPreFling(@NonNull CoordinatorLayout coordinatorLayout, @NonNull View child, @NonNull View target, float velocityX, float velocityY) {
        return super.onNestedPreFling(coordinatorLayout, child, target, velocityX, velocityY);
    }

    @NonNull
    @Override
    public WindowInsetsCompat onApplyWindowInsets(CoordinatorLayout coordinatorLayout, View child, WindowInsetsCompat insets) {
        return super.onApplyWindowInsets(coordinatorLayout, child, insets);
    }

    @Override
    public boolean onRequestChildRectangleOnScreen(CoordinatorLayout coordinatorLayout, View child, Rect rectangle, boolean immediate) {
        return super.onRequestChildRectangleOnScreen(coordinatorLayout, child, rectangle, immediate);
    }

    @Override
    public void onRestoreInstanceState(CoordinatorLayout parent, View child, Parcelable state) {
        super.onRestoreInstanceState(parent, child, state);
    }

    @Override
    public Parcelable onSaveInstanceState(CoordinatorLayout parent, View child) {
        return super.onSaveInstanceState(parent, child);
    }

    @Override
    public boolean getInsetDodgeRect(@NonNull CoordinatorLayout parent, @NonNull View child, @NonNull Rect rect) {
        return super.getInsetDodgeRect(parent, child, rect);
    }
}
