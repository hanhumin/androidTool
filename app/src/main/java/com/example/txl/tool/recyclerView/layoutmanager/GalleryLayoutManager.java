package com.example.txl.tool.recyclerView.layoutmanager;

import android.view.ViewGroup;

import androidx.recyclerview.widget.RecyclerView;

/**
 * Copyright (c) 2020, 唐小陆 All rights reserved.
 * author：txl
 * date：2020/4/14
 * description：画廊效果RecyclerView  LayoutManager
 */
public class GalleryLayoutManager extends RecyclerView.LayoutManager {
    @Override
    public RecyclerView.LayoutParams generateDefaultLayoutParams() {
        return new RecyclerView.LayoutParams( ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    }

    @Override
    public void onLayoutChildren(RecyclerView.Recycler recycler, RecyclerView.State state) {
//        super.onLayoutChildren( recycler, state );
    }


}
