package com.example.txl.tool.wiget;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.txl.tool.R;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/3
 * description：
 */
public class CommonPullRefreshView extends AbsPullRefreshView {

    private final String TAG = CommonPullRefreshView.class.getSimpleName();

    /**
     *根布局
     */
    private View mContainer;

    private ImageView mImageView;

    private TextView mTitleTextView;

    private Animation rotateAnimation;

    public CommonPullRefreshView(int viewType) {
        super( viewType );
    }

    public CommonPullRefreshView(Context context, ViewGroup parent, int viewType) {
        super( viewType );
        getView( context,parent );
    }

    @Override
    public View getView(Context context, ViewGroup parent) {
        if(mContainer != null){
            return mContainer;
        }
        mContainer = LayoutInflater.from( context ).inflate( R.layout.pull_refresh_header, parent,false);
        mImageView = mContainer.findViewById( R.id.pull_refresh_header_ImageView );
        mTitleTextView = mContainer.findViewById( R.id.pull_refresh_header_title_TextView );
        rotateAnimation = AnimationUtils.loadAnimation( context,R.anim.refreshing_rotate );
        rotateAnimation.setRepeatCount(Animation.INFINITE);
        rotateAnimation.setRepeatMode(Animation.RESTART);
        updateViewState(VIEW_STATE_NORMAL);
        return mContainer;
    }

    @Override
    public void onPull(int currentHeight, int refreshHeight, int state) {
        mImageView.setVisibility( View.VISIBLE );
        switch (this.viewType){
            case VIEW_TYPE_HEADER:
                mImageView.setImageResource( R.drawable.ic_up_48 );
                mTitleTextView.setText( "释放刷新数据" );
                break;
            case VIEW_TYPE_FOOTER:
                mImageView.setImageResource( R.drawable.ic_down_48 );
                mTitleTextView.setText( "释放加载更多" );
        }
    }

    @Override
    public void onRunning() {
        updateViewState(VIEW_STATE_RUNNING);
    }

    @Override
    public void onStopRunning() {
        mImageView.setVisibility( View.GONE );
        updateViewState(VIEW_STATE_NORMAL);
    }

    @Override
    public void setViewMarginTop(int marginTop) {
        if(mContainer == null){
            throw new NullPointerException( "mContainer is null in setViewMarginTop do you have call getView" );
        }
        ViewGroup.MarginLayoutParams marginLayoutParams = (ViewGroup.MarginLayoutParams) mContainer.getLayoutParams();
        marginLayoutParams.topMargin = marginTop;
        mContainer.setLayoutParams( marginLayoutParams );
    }

    @Override
    public void setViewMarginBottom(int marginBottom) {
        if(mContainer == null){
            throw new NullPointerException( "mContainer is null in setViewMarginBottom do you have call getView" );
        }
        ViewGroup.MarginLayoutParams marginLayoutParams = (ViewGroup.MarginLayoutParams) mContainer.getLayoutParams();
        marginLayoutParams.bottomMargin = marginBottom;
        mContainer.setLayoutParams( marginLayoutParams );
    }

    @Override
    protected void updateViewState(int viewState) {
        super.updateViewState( viewState );
        switch (viewState){
            case VIEW_STATE_NORMAL:
                if(viewType == VIEW_TYPE_HEADER){
                    mTitleTextView.setText( "下拉刷新" );
                    mImageView.clearAnimation();
                    mImageView.setImageResource( R.drawable.ic_up_48 );
                }else if(viewType == VIEW_TYPE_FOOTER){
                    mTitleTextView.setText( "上拉加载更多" );
                    mImageView.clearAnimation();
                    mImageView.setImageResource( R.drawable.ic_down_48);
                }
                mContainer.getLayoutParams().height = 0;
                mContainer.requestLayout();
                mContainer.setVisibility( View.GONE);
                break;
            case VIEW_STATE_PULL:
            case VIEW_STATE_RELEASE:
                if(viewType == VIEW_TYPE_HEADER){
                    mTitleTextView.setText( "松开刷新" );
                    mImageView.clearAnimation();
                    mImageView.setImageResource( R.drawable.ic_up_48 );
                }else if(viewType == VIEW_TYPE_FOOTER){
                    mTitleTextView.setText( "松开加载" );
                    mImageView.clearAnimation();
                    mImageView.setImageResource( R.drawable.ic_down_48);
                }
                mContainer.getLayoutParams().height = ViewGroup.LayoutParams.WRAP_CONTENT;
                mContainer.requestLayout();
                mContainer.setVisibility( View.VISIBLE);
                break;
            case VIEW_STATE_RUNNING:
                mTitleTextView.setText( "正在加载。。。" );
                mImageView.setImageResource( R.drawable.ic_refreshing_48 );
                mImageView.setAnimation( rotateAnimation );
                mImageView.startAnimation( rotateAnimation );
                mContainer.getLayoutParams().height = ViewGroup.LayoutParams.WRAP_CONTENT;
                mContainer.requestLayout();
                mContainer.setVisibility( View.VISIBLE);
                break;

        }
    }
}
