package com.example.txl.tool.mediaprovider;

import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.core.content.res.ResourcesCompat;
import androidx.recyclerview.widget.RecyclerView;

import com.bumptech.glide.Glide;
import com.bumptech.glide.request.target.CustomViewTarget;
import com.bumptech.glide.request.target.ViewTarget;
import com.bumptech.glide.request.transition.Transition;
import com.example.txl.tool.R;

import java.util.concurrent.ExecutionException;

public class MediaProviderItemViewHolder extends RecyclerView.ViewHolder {
    private MediaInfo mediaInfo;
    private TextView tvTitle;

    public MediaProviderItemViewHolder(@NonNull View itemView,OnItemClick itemClick) {
        super(itemView);
        tvTitle = itemView.findViewById(R.id.tv_title);
        tvTitle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(itemClick != null && mediaInfo != null){
                    itemClick.onItemClick(mediaInfo);
                }
            }
        });
    }

    public void onBindViewHolder(MediaInfo mediaInfo){
        this.mediaInfo = mediaInfo;
        tvTitle.setText(mediaInfo.getDisplayName());

        Drawable left = null;
        left = mediaInfo.isDir()? ResourcesCompat.getDrawable(tvTitle.getResources(),R.drawable.icon_dir,tvTitle.getResources().newTheme()): null;
        if(left != null){
            left.setBounds(0,0,50,50);
        }
        tvTitle.setCompoundDrawables(left,null,null,null);
        Glide.with(tvTitle).asDrawable().load(Uri.parse("file://"+mediaInfo.getPath())).into(new CustomViewTarget<TextView,Drawable>(tvTitle) {

            @Override
            public void onLoadFailed(@Nullable Drawable errorDrawable) {

            }

            @Override
            public void onResourceReady(@NonNull Drawable resource, @Nullable Transition<? super Drawable> transition) {
                resource.setBounds(0,0,50,50);
                tvTitle.setCompoundDrawables(resource,null,null,null);
            }

            @Override
            protected void onResourceCleared(@Nullable Drawable placeholder) {

            }
        });
    }

    public interface OnItemClick{
        public void onItemClick(MediaInfo mediaInfo);
    }
}
