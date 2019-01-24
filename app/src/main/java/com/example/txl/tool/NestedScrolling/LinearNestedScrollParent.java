package com.example.txl.tool.NestedScrolling;

import android.content.Context;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.support.v4.view.NestedScrollingParent;
import android.support.v4.view.NestedScrollingParentHelper;
import android.util.AttributeSet;
import android.view.View;
import android.widget.LinearLayout;

/**
 * @author TXL
 * description :嵌套滑动demo
 */
public class LinearNestedScrollParent extends LinearLayout {
    NestedScrollingParentHelper nestedScrollingParentHelper;

    public LinearNestedScrollParent(Context context) {
        super(context);
    }

    public LinearNestedScrollParent(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public LinearNestedScrollParent(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    public LinearNestedScrollParent(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    @Override
    public boolean onStartNestedScroll(View child, View target, int nestedScrollAxes) {
        return true;
    }

    @Override
    public void onNestedPreScroll(View target, int dx, int dy, int[] consumed) {

        consumed[0] = dx;
        consumed[1] = dy;
        scrollTo(dx,dy);
    }
}
