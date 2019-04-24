package com.example.txl.tool.coordinatorlayout;

import android.content.Context;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
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
import android.widget.LinearLayout;

import com.example.txl.tool.R;
import com.example.txl.tool.utils.DisplayUtil;

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
    private float offsetY = 0;

    int topMargit =0;
    int leftMargit =0;
    int rightMargit =0;

    /**
     * 最大偏移
     * */
    private int maxOffset = 0;

    public TianMaoBehavior() {
        super();
    }

    public TianMaoBehavior(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    void init(Context context){
        topMargit = DisplayUtil.dip2px(context,55);
        leftMargit = DisplayUtil.dip2px(context,45);
        rightMargit = DisplayUtil.dip2px(context,45);
    }

    @Override
    public boolean layoutDependsOn(CoordinatorLayout parent, View child, View dependency) {
        init(child.getContext());
        return isDependsOn(dependency);
    }

    boolean isDependsOn(View dependency){
        return dependency.getId() == R.id.header;
    }

    @Override
    public boolean onDependentViewChanged(CoordinatorLayout parent, View child, View dependency) {
        boolean handle = false;
        float dependencyTranslationY = -dependency.getTranslationY();
        int dependencyHeight = dependency.getHeight();
        Log.d(TAG,"onDependentViewChanged dependencyTranslationY :: "+dependencyTranslationY+"   dependencyHeight :: "+dependencyHeight);
        if(dependencyTranslationY != 0 && dependencyTranslationY <= dependencyHeight){
            handle = true;
            child.getHeight();
            View v = child.findViewById(R.id.tv_transformation);
            FrameLayout.LayoutParams layoutParams = (FrameLayout.LayoutParams) v.getLayoutParams();
            float offset = dependencyTranslationY/dependencyHeight;

            Drawable drawable = child.getBackground().mutate();
            drawable.setAlpha((int) (255*offset));

            layoutParams.topMargin = (int) (topMargit*(1-offset));
            layoutParams.leftMargin = (int) (leftMargit * offset);
            layoutParams.rightMargin = (int) (rightMargit*offset);
            v.setLayoutParams(layoutParams);
        }
        if(dependencyTranslationY == 0){
            Drawable drawable = child.getBackground().mutate();
            drawable.setAlpha(0);
        }
        return handle;
    }


}
