package com.txl.lib.wiget;

import android.content.Context;
import androidx.annotation.Nullable;
import androidx.recyclerview.widget.PagerSnapHelper;
import androidx.recyclerview.widget.RecyclerView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import com.example.txl.tool.player.TextureVideoPlayerView;
import com.example.txl.tool.utils.AppExecutors;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/7
 * description：按页滑动的RecyclerView
 */
public class PageScrollerRecyclerView extends RecyclerView {
    private static final String TAG = PageScrollerRecyclerView.class.getSimpleName();

    private IPullRefreshListener pullRefreshListener;
    private IViewPageScrollListener pageScrollListener;
    private PagerSnapHelper mPagerSnapHelper;

    private int targetPos = 0;
    private View targetView = null;

    public PageScrollerRecyclerView(Context context) {
        this( context ,null);
    }

    public PageScrollerRecyclerView(Context context, @Nullable AttributeSet attrs) {
        this( context, attrs ,0);
    }

    public PageScrollerRecyclerView(Context context, @Nullable AttributeSet attrs, int defStyle) {
        super( context, attrs, defStyle );
        init();
    }

    private void init(){
        mPagerSnapHelper = new PagerSnapHelper(){
            @Override
            public int findTargetSnapPosition(RecyclerView.LayoutManager layoutManager, int velocityX, int velocityY) {
                // TODO 找到对应的Index
                int targetPos = super.findTargetSnapPosition(layoutManager, velocityX, velocityY);
                Log.e(TAG, "findTargetSnapPosition targetPos: " + targetPos);
                return targetPos;
            }

            // 在 Adapter的 onBindViewHolder 之后执行
            @Nullable
            @Override
            public View findSnapView(RecyclerView.LayoutManager layoutManager) {
                // TODO 找到对应的View
                TextureVideoPlayerView view = (TextureVideoPlayerView) super.findSnapView(layoutManager);
                targetPos = layoutManager.getPosition( view );
                Log.e(TAG, "findSnapView tag: " +view.getTag());
                if(view == targetView){
                    return view;
                }
                targetView = view;
                if(pageScrollListener != null && targetView != null){
                    new AppExecutors().mainThread().execute( new Runnable() {
                        @Override
                        public void run() {
                            pageScrollListener.onPageScroll( targetPos,targetView );
                        }
                    } );
                }
                return view;
            }
        };
        mPagerSnapHelper.attachToRecyclerView( this );
    }

    public void setPullRefreshListener(IPullRefreshListener listener) {
        this.pullRefreshListener = listener;
    }

    public void setPageScrollListener(IViewPageScrollListener listener){
        this.pageScrollListener = listener;
    }

    @Override
    public void setOnFlingListener(@Nullable OnFlingListener onFlingListener) {
        super.setOnFlingListener( onFlingListener );
    }

    @Override
    public void onScrollStateChanged(int state) {
        Log.e(TAG, "onScrollStateChanged  " +state+"  "+targetPos+"  "+targetView);
        switch (state) {
            case RecyclerView.SCROLL_STATE_IDLE:
//                if(pageScrollListener != null && targetView != null){
//                    pageScrollListener.onPageScroll( targetPos,targetView );
//                }
//                View viewIdle = mPagerSnapHelper.findSnapView(getLayoutManager());
//                int positionIdle = getPosition(viewIdle);
//                getChildAdapterPosition( viewIdle );
//                if (mOnViewPagerListener != null && getChildCount() == 1) {
//                    mOnViewPagerListener.onPageSelected(positionIdle,positionIdle == getItemCount() - 1);
//                }
                break;
            case RecyclerView.SCROLL_STATE_DRAGGING:
//                View viewDrag = mPagerSnapHelper.findSnapView(this);
//                int positionDrag = getPosition(viewDrag);
//                break;
            case RecyclerView.SCROLL_STATE_SETTLING:
//                View viewSettling = mPagerSnapHelper.findSnapView(this);
//                int positionSettling = getPosition(viewSettling);
//                break;
        }

        super.onScrollStateChanged( state );
    }

    public interface IViewPageScrollListener{
        void onPageScroll(int targetPosition, View targetView);
    }
}
