package com.example.txl.tool.recyclerView.layoutmanager;

import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

/**
 * Copyright (c) 2020, 唐小陆 All rights reserved.
 * author：txl
 * date：2020/4/14
 * description：画廊效果RecyclerView  LayoutManager
 */
public class GalleryLayoutManager extends RecyclerView.LayoutManager {
    /**
     * 每页正常情况下显示几个元素
     */
    private int pageLimit = 3;
    /**
     * 页面元素之间的间距
     */
    private int normalViewGap = 30;

    private int mFirstPosition = 0;

    @Override
    public RecyclerView.LayoutParams generateDefaultLayoutParams() {
        return new RecyclerView.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    }

    int startX = 0;

    @Override
    public void onLayoutChildren(RecyclerView.Recycler recycler, RecyclerView.State state) {
        int count = state.getItemCount();
        if (count <= 0) {
            detachAndScrapAttachedViews(recycler);
            return;
        }
        startX = 0;
        for (int i = 0; i < pageLimit; i++) {
            View child = recycler.getViewForPosition(i);
            addView(child);
            measureChildWithMargins(child, 0, 0);
            if (i >= count) {
                break;
            }
            int left, top, right, bottom;
            int width = getDecoratedMeasuredWidth(child);
            int height = getDecoratedMeasuredHeight(child);
            top = 0;
            bottom = getHeight();
            left = startX;
            right = width + startX;
            layoutDecoratedWithMargins(child,left,top,right,bottom);
            startX += width + normalViewGap;

        }


    }


}
