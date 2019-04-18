package com.example.txl.tool.coordinatorlayout;

import android.content.Context;
import android.graphics.Rect;
import android.os.Parcelable;
import android.support.annotation.NonNull;
import android.support.design.widget.CoordinatorLayout;
import android.support.v4.view.ViewCompat;
import android.support.v4.view.WindowInsetsCompat;
import android.support.v4.widget.NestedScrollView;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;

/**
 * create by txl
 * at 2018/9/21
 * description :天猫顶部效果
 */
public class TianMaoBehavior extends CoordinatorLayout.Behavior<View> {
    private static final String TAG = "TianMaoBehavior";

    /**
     * 顶部title在y方向的偏移
     * */
    private int offectY = 0;

    /**
     * 最大偏移
     * */
    private int maxOffect = 0;

    // 界面整体向上滑动，达到列表可滑动的临界点
    private boolean upReach;
    // 列表向上滑动后，再向下滑动，达到界面整体可滑动的临界点
    private boolean downReach;
    // 列表上一个全部可见的item位置
    private int lastPosition = -1;

    public TianMaoBehavior() {
        super();
    }

    public TianMaoBehavior(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public boolean onInterceptTouchEvent(CoordinatorLayout parent, View child, MotionEvent ev) {
        switch (ev.getAction()) {
            case MotionEvent.ACTION_DOWN:
                downReach = false;
                upReach = false;
                break;
        }
        return super.onInterceptTouchEvent(parent, child, ev);
    }

    @Override
    public boolean onStartNestedScroll(@NonNull CoordinatorLayout coordinatorLayout, @NonNull View child, @NonNull View directTargetChild, @NonNull View target, int axes, int type) {
        if ((axes & ViewCompat.SCROLL_AXIS_VERTICAL) != 0) {
            maxOffect = child.getHeight()/2;
            return true;
        }
        return super.onStartNestedScroll(coordinatorLayout,child,directTargetChild,target,axes,type);
    }

    @Override
    public void onNestedPreScroll(@NonNull CoordinatorLayout coordinatorLayout, @NonNull View child, @NonNull View target, int dx, int dy, @NonNull int[] consumed, int type) {
        super.onNestedPreScroll(coordinatorLayout, child, target, dx, dy, consumed, type);
        Log.d(TAG,"onNestedPreScroll");
        if (dy != 0) {
            RecyclerView recyclerView;
            if(!(target instanceof RecyclerView)){
                return;
            }
            recyclerView = (RecyclerView) target;
            RecyclerView.LayoutManager layoutManager = recyclerView.getLayoutManager();
            if(layoutManager instanceof LinearLayoutManager){
                LinearLayoutManager linearLayoutManager = (LinearLayoutManager) layoutManager;
                int firstVisiblePosition = linearLayoutManager.findFirstCompletelyVisibleItemPosition();
                if (firstVisiblePosition == 0 && firstVisiblePosition < lastPosition) {//上滑
                    downReach = true;
                }
                // 整体可以滑动，否则RecyclerView消费滑动事件
                if (canScroll(child, dy) && firstVisiblePosition == 0) {
                    float finalY = child.getTranslationY() - dy;
                    if (finalY < -child.getHeight()) {
                        finalY = -child.getHeight();
                        upReach = true;
                    } else if (finalY > 0) {
                        finalY = 0;
                    }
                    child.setTranslationY(finalY);
                    // 让CoordinatorLayout消费滑动事件
                    consumed[1] = dy;
                }
                lastPosition = firstVisiblePosition;
            }
            if(dy < 0){//上滑

            }else {//下滑

            }

        }
    }

    private boolean canScroll(View child, float scrollY) {
        if (scrollY > 0 && child.getTranslationY() == maxOffect && !upReach) {
            return false;
        }

        if (downReach) {
            return false;
        }
        return true;
    }

    @Override
    public void onNestedScroll(@NonNull CoordinatorLayout coordinatorLayout, @NonNull View child, @NonNull View target, int dxConsumed, int dyConsumed, int dxUnconsumed, int dyUnconsumed, int type) {
        super.onNestedScroll(coordinatorLayout, child, target, dxConsumed, dyConsumed, dxUnconsumed, dyUnconsumed, type);
    }
}
