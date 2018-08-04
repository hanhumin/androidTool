package com.example.txl.tool.wiget;

import android.content.Context;
import android.support.annotation.Nullable;
import android.support.v4.util.SparseArrayCompat;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.StaggeredGridLayoutManager;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/2
 * description：
 */
public class PullRefreshRecyclerView extends RecyclerView {

    private final String TAG = PullRefreshRecyclerView.class.getSimpleName();
    /**
     * 阻尼效果
     */
    private final float OFFSET_RADIO = 1.5f;

    /**
     * 是否可以下拉   默认可以
     */
    private boolean mEnablePullRefresh = true;


    private Context context;
    private AbsPullRefreshView mHeader;
    private AbsPullRefreshView mFooter;

    private AdapterWrapper adapterWrapper;
    private OnPullRefreshListener listener;

    public PullRefreshRecyclerView(Context context) {
        this( context,null );
    }

    public PullRefreshRecyclerView(Context context, @Nullable AttributeSet attrs) {
        this( context, attrs, 0);
    }

    public PullRefreshRecyclerView(Context context, @Nullable AttributeSet attrs, int defStyle) {
        super( context, attrs, defStyle );
        this.context = context;
    }

    private void init(){
        mHeader = new CommonPullRefreshView(context,this,AbsPullRefreshView.VIEW_TYPE_HEADER);
        mHeader.updateViewState( AbsPullRefreshView.VIEW_STATE_RUNNING );
        mFooter = new CommonPullRefreshView( context,this,AbsPullRefreshView.VIEW_TYPE_FOOTER );
        adapterWrapper.AddHeaderView( mHeader.getView( context,this ) );
        adapterWrapper.addFootView( mFooter.getView( context,this ) );
    }

    @Override
    public void setAdapter(Adapter adapter) {
        adapterWrapper = new AdapterWrapper( adapter );
        adapter.registerAdapterDataObserver( new AdapterDataObserver() {
            @Override
            public void onChanged() {
                adapterWrapper.notifyDataSetChanged();
            }

            @Override
            public void onItemRangeChanged(int positionStart, int itemCount) {
                adapterWrapper.notifyItemRangeChanged( positionStart+adapterWrapper.mHeaderViews.size(),itemCount );
            }

            @Override
            public void onItemRangeChanged(int positionStart, int itemCount, Object payload) {
                adapterWrapper.notifyItemRangeChanged( positionStart+adapterWrapper.mHeaderViews.size(),itemCount );
            }

            @Override
            public void onItemRangeInserted(int positionStart, int itemCount) {
                adapterWrapper.notifyItemRangeInserted( positionStart+adapterWrapper.mHeaderViews.size(),itemCount );
            }

            @Override
            public void onItemRangeRemoved(int positionStart, int itemCount) {
                adapterWrapper.notifyItemRangeRemoved( positionStart+adapterWrapper.mHeaderViews.size(),itemCount );
            }

            @Override
            public void onItemRangeMoved(int fromPosition, int toPosition, int itemCount) {
                adapterWrapper.notifyDataSetChanged();
            }
        } );
        init();
        super.setAdapter( adapterWrapper );

    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent ev) {
        return super.dispatchTouchEvent( ev );
    }

    int mLastRawX, mLastRawY;
    @Override
    public boolean onTouchEvent(MotionEvent e) {
        if(!mEnablePullRefresh){
            return super.onTouchEvent( e );
        }
        int rawX = (int) e.getRawX();
        int rawY = (int) e.getRawY();
        switch (e.getAction()){
            case MotionEvent.ACTION_DOWN:
                mLastRawX = (int) e.getRawX();
                mLastRawY = (int) e.getRawY();
                break;
            case MotionEvent.ACTION_MOVE:
                int offsetY = (int) (e.getRawY() - mLastRawY);
                Log.e( TAG,"onTouchEvent offsetY  = "+offsetY +" isSlideToTop "+isSlideToTop() +" isSlideToBottom "+isSlideToBottom());
                if(isSlideToTop() && offsetY>0){
                    mHeader.updateViewState( AbsPullRefreshView.VIEW_STATE_PULL );
                    mHeader.setViewMarginTop( (int) (offsetY / OFFSET_RADIO) );
                }else if(isSlideToBottom() && offsetY<0){
                    mFooter.updateViewState( AbsPullRefreshView.VIEW_STATE_PULL );
                    mFooter.setViewMarginBottom( (int) -(offsetY / OFFSET_RADIO) );
                }
//                mLastRawX = rawX;
//                mLastRawY = rawY;
                break;
            case MotionEvent.ACTION_UP:
                mLastRawX = -1;
                mLastRawY = -1;
                if(mFooter.getViewState() == AbsPullRefreshView.VIEW_STATE_PULL){
                    mFooter.setViewMarginBottom( 0 );
                    if(listener!=null){
                        mFooter.updateViewState( AbsPullRefreshView.VIEW_STATE_RUNNING );
                        listener.loadMore();
                    }
                }else if(mHeader.getViewState() == AbsPullRefreshView.VIEW_STATE_PULL){
                    mHeader.setViewMarginTop( 0 );
                    if(listener!=null){
                        mHeader.updateViewState( AbsPullRefreshView.VIEW_STATE_RUNNING );
                        listener.onRefresh();
                    }
                }
                break;
        }
        return super.onTouchEvent( e );
    }

    private boolean isSlideToTop(){
        LayoutManager layoutManager = getLayoutManager();
        if(layoutManager instanceof StaggeredGridLayoutManager){
            StaggeredGridLayoutManager staggeredGridLayoutManager = (StaggeredGridLayoutManager) layoutManager;
            int firstCompletelyVisibleItemPositions[] = new int[staggeredGridLayoutManager.getSpanCount()];
            firstCompletelyVisibleItemPositions = staggeredGridLayoutManager.findFirstCompletelyVisibleItemPositions( firstCompletelyVisibleItemPositions );
            if(firstCompletelyVisibleItemPositions[0] == 0 || firstCompletelyVisibleItemPositions[0]  == 1){
                return true;
            }
        }
        if(layoutManager instanceof LinearLayoutManager){
            LinearLayoutManager linearLayoutManager = (LinearLayoutManager) layoutManager;
            int position = linearLayoutManager.findFirstVisibleItemPosition();
            if(position <= 1){
                return true;
            }else {
                return false;
            }
        }
        return !canScrollVertically(1);
    }

    private boolean isSlideToBottom() {
        LayoutManager layoutManager = getLayoutManager();
        if(layoutManager instanceof StaggeredGridLayoutManager){
            StaggeredGridLayoutManager staggeredGridLayoutManager = (StaggeredGridLayoutManager) layoutManager;

            int spanCount = staggeredGridLayoutManager.getSpanCount();
            int lastCompletelyVisibleItemPositions[] = new int[spanCount];
            lastCompletelyVisibleItemPositions = staggeredGridLayoutManager.findLastCompletelyVisibleItemPositions(lastCompletelyVisibleItemPositions  );
            for (int i=0; i<spanCount; i++){
                Log.e( TAG,"isSlideToBottom  第+"+i+"个    "+lastCompletelyVisibleItemPositions[i]+"  "+"  RealItemCount  " +adapterWrapper.getRealItemCount()+"   "+canScrollVertically(-1));
                if(lastCompletelyVisibleItemPositions[i] >= adapterWrapper.getRealItemCount()-staggeredGridLayoutManager.getSpanCount() || !canScrollVertically(-1)){
                    Log.e( TAG,"isBottom" );
                    return true;
                }
            }
            return false;
        }
        if(layoutManager instanceof LinearLayoutManager){
            LinearLayoutManager linearLayoutManager = (LinearLayoutManager) layoutManager;
            int position = linearLayoutManager.findLastCompletelyVisibleItemPosition();
            if(position >= adapterWrapper.getItemCount()-2){
                return true;
            }else {
                return false;
            }
        }
        Log.e( TAG,"canScrollVertically  "+canScrollVertically(-1) );
        return !canScrollVertically(-1);
    }

    public void setOnPullRefreshListener(OnPullRefreshListener listener){
        this.listener = listener;
    }

    public void setRefreshFinish(){

        mHeader.updateViewState( AbsPullRefreshView.VIEW_STATE_NORMAL );
    }

    public void setLoadMoreFinish(){
        mFooter.updateViewState( AbsPullRefreshView.VIEW_STATE_NORMAL);
    }

    private class AdapterWrapper extends RecyclerView.Adapter<RecyclerView.ViewHolder>{

        private static final int BASE_ITEM_TYPE_HEADER = 100000;
        private static final int BASE_ITEM_TYPE_FOOTER = 200000;
        //头集合 尾结合
        private SparseArrayCompat<View> mHeaderViews = new SparseArrayCompat<>();
        private SparseArrayCompat<View> mFootViews = new SparseArrayCompat<>();

        private RecyclerView.Adapter mInnerAdapter;

        public AdapterWrapper(Adapter mInnerAdapter) {
            this.mInnerAdapter = mInnerAdapter;
        }

        private boolean isHeaderViewPos(int position){
            return position < mHeaderViews.size();
        }

        public void AddHeaderView(View view){
            mHeaderViews.put( mHeaderViews.size()+BASE_ITEM_TYPE_HEADER,view );
        }

        private boolean isFooterViewPos(int position){
            return position >= mHeaderViews.size()+mInnerAdapter.getItemCount();
        }

        public int getHeadersCount(){
            return mHeaderViews.size();
        }

        public int getFootersCount(){
            return mFootViews.size();
        }

        public void addFootView(View view){
            mFootViews.put( mFootViews.size()+BASE_ITEM_TYPE_FOOTER,view );
        }

        public int getRealItemCount(){
            return mInnerAdapter.getItemCount();
        }

        @Override
        public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            if(mHeaderViews.get( viewType ) != null){
                return new HeaderViewHolder(mHeaderViews.get( viewType )  );
            }else if(mFootViews.get( viewType ) != null){
                return new FootViewHolder( mFootViews.get( viewType ) );
            }
            return mInnerAdapter.onCreateViewHolder( parent,viewType );
        }

        @Override
        public int getItemViewType(int position) {
            if(isHeaderViewPos( position )){
                return mHeaderViews.keyAt( position );
            }else if(isFooterViewPos( position )){
                return mFootViews.keyAt( position - mHeaderViews.size() - mInnerAdapter.getItemCount() );
            }
            return mInnerAdapter.getItemViewType(position - mHeaderViews.size());
        }

        @Override
        public void onBindViewHolder(ViewHolder holder, int position) {
            if (isHeaderViewPos(position)) {

                return;
            }
            if (isFooterViewPos(position)) {
                return;
            }
            mInnerAdapter.onBindViewHolder(holder, position - getHeadersCount());
        }

        @Override
        public int getItemCount() {
            return mFootViews.size()+mHeaderViews.size()+mInnerAdapter.getItemCount();
        }

        @Override
        public void onAttachedToRecyclerView(RecyclerView recyclerView) {
//            mInnerAdapter.onAttachedToRecyclerView(recyclerView);

            /**
             * 解决网格布局问题
             */
            RecyclerView.LayoutManager layoutManager = recyclerView.getLayoutManager();
            if (layoutManager instanceof GridLayoutManager) {
                final GridLayoutManager gridLayoutManager = (GridLayoutManager) layoutManager;

                gridLayoutManager.setSpanSizeLookup(new GridLayoutManager.SpanSizeLookup() {
                    @Override
                    public int getSpanSize(int position) {
                        int viewType = getItemViewType(position);
                        if (mHeaderViews.get(viewType) != null) {
                            return gridLayoutManager.getSpanCount();
                        } else if (mFootViews.get(viewType) != null) {
                            return gridLayoutManager.getSpanCount();
                        } else {
                            return 1;
                        }
                    }
                });
            }
        }


    @Override
    public void onViewAttachedToWindow(RecyclerView.ViewHolder holder) {
        mInnerAdapter.onViewAttachedToWindow(holder);
        int position = holder.getLayoutPosition();
        if (isHeaderViewPos(position) || isFooterViewPos(position)) {
            ViewGroup.LayoutParams lp = holder.itemView.getLayoutParams();

            if (lp != null && lp instanceof StaggeredGridLayoutManager.LayoutParams) {

                StaggeredGridLayoutManager.LayoutParams p = (StaggeredGridLayoutManager.LayoutParams) lp;
                p.setFullSpan(true);
            }
            if(lp != null && lp instanceof GridLayoutManager.LayoutParams){
                GridLayoutManager.LayoutParams p = (GridLayoutManager.LayoutParams) lp;
            }
        }
    }


        private class HeaderViewHolder extends RecyclerView.ViewHolder {
            HeaderViewHolder(View itemView) {
                super(itemView);
            }
        }

        private class FootViewHolder extends RecyclerView.ViewHolder {
            FootViewHolder(View itemView) {
                super(itemView);

            }
        }

    }

    public interface OnPullRefreshListener {
        void onRefresh();
        void loadMore();
    }
}
