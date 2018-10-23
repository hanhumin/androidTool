package com.example.txl.tool.recycler.adapter;

import android.support.v7.widget.CardView;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.StaggeredGridLayoutManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.util.SparseArray;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.example.txl.tool.App;
import com.example.txl.tool.R;
import com.example.txl.tool.recycler.bean.BeautyGirls;
import com.txl.lib.image_load.ImageLoader;

import java.util.ArrayList;
import java.util.List;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/7/8
 * description：实际放置的是图片
 */
public class FiLiAdapter extends RecyclerView.Adapter{

    String TAG = FiLiAdapter.class.getSimpleName();

    List<BeautyGirls.Girl> results = new ArrayList<>(  );
    SparseArray<Integer> heightArray;

    RecyclerView recyclerView;
    RecyclerView.LayoutManager layoutManager;
    int itemWidth = 0;
    boolean mIsRecyclerViewIdle = true;

    public FiLiAdapter(RecyclerView recyclerView) {
        this.recyclerView = recyclerView;
        recyclerView.addOnScrollListener( new RecyclerView.OnScrollListener() {
            @Override
            public void onScrollStateChanged(RecyclerView recyclerView, int newState) {
                FiLiAdapter.this.onScrollStateChanged( recyclerView, newState);
                super.onScrollStateChanged( recyclerView, newState );
            }

            @Override
            public void onScrolled(RecyclerView recyclerView, int dx, int dy) {
                super.onScrolled( recyclerView, dx, dy );
            }
        } );
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        CardView v = (CardView) LayoutInflater.from(parent.getContext())
                .inflate( R.layout.beauty_girl_item, parent, false);
        return new ViewHolder(v);
    }
    ImageLoader  loader = App.getImageLoader();
    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, final int position) {
        if(mIsRecyclerViewIdle){
            final ViewHolder viewHolder = (ViewHolder) holder;
            final ImageView imageView = viewHolder.cardView.findViewById( R.id.image_item );
            if(heightArray == null){
                heightArray = new SparseArray<>(  );
            }
            if(heightArray.get( position ) == null){
                loader.decodeBitmapSize( results.get( position ).getUrl(), new ImageLoader.SourceReady() {
                    @Override
                    public void bitmapSourceReady(int bitmapWidth, int bitmapHeight) {
                        LinearLayout.LayoutParams layoutParams = (LinearLayout.LayoutParams) imageView.getLayoutParams();
                        int realHeight = viewHolder.cardView.getWidth()*bitmapHeight/bitmapWidth;
                        layoutParams.height = realHeight;
                        imageView.setLayoutParams(layoutParams);
                        heightArray.put( position,realHeight );
                        loader.bindBitmap(results.get( position ).getUrl() ,imageView ,itemWidth/3,heightArray.get( position ));
                        TextView textView = viewHolder.cardView.findViewById( R.id.name_item );
                        String createAt = results.get( position ).getCreatedAt();
                        createAt = createAt.split( "T" )[0];
                        textView.setText( createAt );
                    }
                } );
            }else {
                int height = heightArray.get(position);
                LinearLayout.LayoutParams layoutParams = (LinearLayout.LayoutParams) imageView.getLayoutParams();
                layoutParams.height = height;
                imageView.setLayoutParams(layoutParams);
                loader.bindBitmap(results.get( position ).getUrl() ,imageView ,itemWidth/3,height);
                TextView textView = viewHolder.cardView.findViewById( R.id.name_item );
                String createAt = results.get( position ).getCreatedAt();
                createAt = createAt.split( "T" )[0];
                textView.setText( createAt );
            }
        }
    }

    @Override
    public int getItemCount() {
        return results.size();
    }

    public void updateData(List<BeautyGirls.Girl> infoContents){
        if(heightArray != null){
            heightArray.clear();
        }
        results.clear();
        results.addAll( infoContents );
        Log.e( TAG,"updateData results "+results.size()+" infoContents "+infoContents.size() );
        notifyDataSetChanged();
    }

    public void addData(List<BeautyGirls.Girl> infoContents){
        int start = results.size();
        results.addAll( infoContents );
        notifyItemRangeInserted(start, infoContents.size() );
        Log.e( TAG,"addData results "+results.size() );
    }

    public void onScrollStateChanged(RecyclerView view, int scrollState){
        if(scrollState == RecyclerView.SCROLL_STATE_IDLE){
            mIsRecyclerViewIdle = true;
            notifyDataSetChanged();
        }else {
            mIsRecyclerViewIdle = false;
        }
    }

    public int calculateItemwidth(){
        if(layoutManager != null && itemWidth != 0){
            return itemWidth;
        }
        DisplayMetrics dm = App.getContext().getResources().getDisplayMetrics();
        int screenWidth = dm.widthPixels;
        layoutManager = recyclerView.getLayoutManager();
        if(layoutManager instanceof StaggeredGridLayoutManager){
            int spanCount = ((StaggeredGridLayoutManager) layoutManager).getSpanCount();
            itemWidth = (int) (1.0* screenWidth/spanCount);
        }else if(layoutManager instanceof GridLayoutManager){
            int spanCount = ((GridLayoutManager) layoutManager).getSpanCount();
            itemWidth = (int) (1.0* screenWidth/spanCount);
        }
        return itemWidth;
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        public CardView cardView;
        public ViewHolder(View v) {
            super(v);
            cardView = (CardView) v;
        }
    }

}
