package com.example.txl.tool.coordinatorlayout;

import android.content.Context;
import android.support.design.widget.AppBarLayout;
import android.support.design.widget.CoordinatorLayout;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

/**
 * create by txl
 * at 2018/9/21
 * description :
 */
public class MyBehavior extends CoordinatorLayout.Behavior {
    private static final String TAG = "MyBehavior";

    public MyBehavior(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public boolean layoutDependsOn(CoordinatorLayout parent, View child, View dependency) {
        return dependency instanceof AppBarLayout;
    }

    @Override
    public boolean onDependentViewChanged(CoordinatorLayout parent, View child, View dependency) {
        Log.d(TAG, "onDependentViewChanged");

        int top = dependency.getScrollY();
        int left = dependency.getScrollX();
        Log.d(TAG,"top:"+top+"  left:"+left);
        CoordinatorLayout.LayoutParams params = (CoordinatorLayout.LayoutParams) child.getLayoutParams();
        child.scrollTo(0,-top);
        return super.onDependentViewChanged(parent, child, dependency);
    }
}
