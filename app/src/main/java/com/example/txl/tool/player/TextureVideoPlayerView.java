package com.example.txl.tool.player;

import android.content.Context;
import android.graphics.Color;
import android.graphics.SurfaceTexture;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.TextureView;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageView;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/8
 * description：
 */
public class TextureVideoPlayerView extends FrameLayout {
    private static final String TAG = "TextureVideoPlayerView";

    private ImageView backImage;
    private TextureView mTextureView;
    private ViewGroup mContainer;
    //进度条
    //播控按钮
    //desc描述

    public TextureVideoPlayerView(@NonNull Context context) {
        this( context ,null);
    }

    public TextureVideoPlayerView(@NonNull Context context, @Nullable AttributeSet attrs) {
        this( context, attrs, 0);
    }

    public TextureVideoPlayerView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super( context, attrs, defStyleAttr );
        initView( context );
    }

    private void initView(Context context){
        backImage = new ImageView( context );
        backImage.setScaleType( ImageView.ScaleType.FIT_XY );
        this.addView( backImage, new FrameLayout.LayoutParams( ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT ));
        mTextureView = new TextureView( context );
        mTextureView.setSurfaceTextureListener( new TextureView.SurfaceTextureListener() {
            @Override
            public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
                Log.d( TAG,"PageScrollerRecyclerView VideoActivity  onSurfaceTextureAvailable"+TextureVideoPlayerView.this.getTag());
            }

            @Override
            public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
                Log.d( TAG,"PageScrollerRecyclerView VideoActivity  onSurfaceTextureSizeChanged"+TextureVideoPlayerView.this.getTag());
            }

            @Override
            public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
                Log.d( TAG,"PageScrollerRecyclerView VideoActivity  onSurfaceTextureDestroyed"+TextureVideoPlayerView.this.getTag());
                return false;
            }

            @Override
            public void onSurfaceTextureUpdated(SurfaceTexture surface) {
                Log.d( TAG,"PageScrollerRecyclerView VideoActivity  onSurfaceTextureUpdated"+TextureVideoPlayerView.this.getTag());
            }
        } );
        this.addView(mTextureView, new FrameLayout.LayoutParams( ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT ));
        mContainer = new FrameLayout( context );
        mContainer.setBackgroundColor( Color.alpha( 0x00CC00FF ) );
        this.addView( mContainer, new FrameLayout.LayoutParams( ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT ));
    }

    public ImageView getBackImage(){
        return backImage;
    }

    public TextureView getTextureView(){
        return mTextureView;
    }

    public void addTextureView(TextureView textureView){
        mTextureView = textureView;
        addView( textureView,1, new FrameLayout.LayoutParams( ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT ));
    }

    public void setSurfaceTextureListener(TextureView.SurfaceTextureListener listener){
        if(mTextureView == null){
            return;
        }
        mTextureView.setSurfaceTextureListener( listener );
    }
}
