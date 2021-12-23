package com.example.txl.tool.mediaprovider;

import android.view.LayoutInflater;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.example.txl.tool.R;

import java.util.ArrayList;
import java.util.List;

public class MediaProviderItemAdapter extends RecyclerView.Adapter<MediaProviderItemViewHolder> implements MediaProviderItemViewHolder.OnItemClick {
    private MediaProviderItemViewHolder.OnItemClick onItemClick;
    private List<MediaInfo> mediaInfos;

    @NonNull
    @Override
    public MediaProviderItemViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        return new MediaProviderItemViewHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.item_media_provider,parent,false),this);
    }

    @Override
    public void onBindViewHolder(@NonNull MediaProviderItemViewHolder holder, int position) {
        holder.onBindViewHolder(mediaInfos.get(position));
    }

    @Override
    public int getItemCount() {
        return mediaInfos == null? 0 : mediaInfos.size();
    }

    @Override
    public void onItemClick(MediaInfo mediaInfo) {
        if(onItemClick != null){
            onItemClick.onItemClick(mediaInfo);
        }
    }

    public void setOnItemClick(MediaProviderItemViewHolder.OnItemClick onItemClick) {
        this.onItemClick = onItemClick;
    }

    public void setMediaInfos(List<MediaInfo> mediaInfos) {
        this.mediaInfos = mediaInfos;
        notifyDataSetChanged();
    }

    public List<MediaInfo> getMediaInfos() {
        if(mediaInfos == null){
            return new ArrayList<>();
        }
        return new ArrayList<>(mediaInfos);
    }
}
