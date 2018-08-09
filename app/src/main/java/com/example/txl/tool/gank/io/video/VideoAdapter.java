package com.example.txl.tool.gank.io.video;

import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;

import com.example.txl.tool.App;
import com.example.txl.tool.R;
import com.example.txl.tool.player.TextureVideoPlayerView;

import java.util.ArrayList;
import java.util.List;


/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/7/8
 * description：
 */
public class VideoAdapter extends RecyclerView.Adapter {
    String TAG = VideoAdapter.class.getSimpleName();

    List<VideoBean.VideoInfo> results = new ArrayList<>(  );

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        TextureVideoPlayerView textureVideoPlayerView = (TextureVideoPlayerView) LayoutInflater.from(parent.getContext())
                .inflate( R.layout.video_item_textrueplayerview, parent, false);
        return new ViewHolder(textureVideoPlayerView);
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        Log.d( TAG,"VideoActivity  "+"onBindViewHolder" );
        ViewHolder viewHolder = (ViewHolder) holder;
        ImageView backImage = viewHolder.textureVideoPlayerView.getBackImage();
        viewHolder.textureVideoPlayerView.setTag( results.get( position ).getContent().getPlayAddr() );
        App.getImageLoader().bindBitmap( results.get( position ).getContent().getCover(),backImage,0,0 );

    }

    @Override
    public int getItemCount() {
        return results.size();
    }

    public void updateData(List<VideoBean.VideoInfo> infoContents){
        results.clear();
        results.addAll( infoContents );
        Log.e( TAG,"updateData results "+results.size() );
        notifyDataSetChanged();
    }

    public void addData(List<VideoBean.VideoInfo> infoContents){
        int start = results.size();
        results.addAll( infoContents );
        notifyItemRangeInserted(start, infoContents.size() );
        Log.e( TAG,"addData results "+results.size() );
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {

        public TextureVideoPlayerView textureVideoPlayerView;
        public ViewHolder(View v) {
            super(v);
            textureVideoPlayerView = (TextureVideoPlayerView) v;
        }
    }
}
