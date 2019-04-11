package com.txl.lib.custom_view.floatitem;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v7.widget.RecyclerView;
import android.util.AttributeSet;
import android.view.View;
import android.widget.FrameLayout;

/**
 * @author TXL
 * description :
 */
public class FloatItemRecyclerView<V extends RecyclerView> extends FrameLayout {
    /**
     * 要悬浮的View
     */
    private View floatView;

    /**
     * recyclerView
     */
    private V recyclerView;

    /**
     * 控制每一个item是否要显示floatView
     */
    private FloatViewShowHook<V> floatViewShowHook;


    public FloatItemRecyclerView(@NonNull Context context) {
        super(context);
    }

    public FloatItemRecyclerView(@NonNull Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public FloatItemRecyclerView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public FloatItemRecyclerView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    /**
     * 根据item设置是否显示浮动的View
     */
    public interface FloatViewShowHook<V extends RecyclerView> {

        /**
         * 当前item是否要显示floatView
         *
         * @param child    itemView
         * @param position 在列表中的位置
         */
        boolean needShowFloatView(View child, int position);

        V initFloatItemRecyclerView();
    }
}
