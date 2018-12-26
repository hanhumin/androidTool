package com.example.txl.tool.recycler;

import android.os.Bundle;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.StaggeredGridLayoutManager;

import com.example.txl.tool.BaseActivity;
import com.example.txl.tool.R;
import com.example.txl.tool.recycler.adapter.FiLiAdapter;
import com.example.txl.tool.recycler.bean.BeautyGirls;
import com.txl.lib.wiget.PullRefreshRecyclerView;

import java.util.List;

public class RecyclerViewActivity extends BaseActivity implements FuLiContract.View{

    private PullRefreshRecyclerView refreshRecyclerView;
    private FuLiPresenter mFiLiPresenter;
    private FiLiAdapter adapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_recycler_view );
        initView();
    }


    private void initView(){
        refreshRecyclerView = findViewById( R.id.RecyclerActivity_PullRefreshRecyclerView );
        String manager = getIntent().getStringExtra(  "manager" );
        RecyclerView.LayoutManager recyclerViewLayoutManager;
        switch (manager){
            case "StaggeredGridLayoutManager":
                recyclerViewLayoutManager = new StaggeredGridLayoutManager(3, StaggeredGridLayoutManager.VERTICAL);
                break;
            case "LinearLayoutManager":
                recyclerViewLayoutManager = new LinearLayoutManager(this);
                break;
            case "GridLayoutManager":
                recyclerViewLayoutManager = new GridLayoutManager( this,3 );
                break;
            default:
                recyclerViewLayoutManager = new StaggeredGridLayoutManager(3, StaggeredGridLayoutManager.VERTICAL);
                ((StaggeredGridLayoutManager)recyclerViewLayoutManager).setGapStrategy(StaggeredGridLayoutManager.GAP_HANDLING_NONE);
                break;
        }
//        StaggeredGridLayoutManager recyclerViewLayoutManager =
//                new StaggeredGridLayoutManager(3, StaggeredGridLayoutManager.VERTICAL);
//        recyclerViewLayoutManager.setGapStrategy(StaggeredGridLayoutManager.GAP_HANDLING_NONE);
//        GridLayoutManager recyclerViewLayoutManager = new GridLayoutManager( this,3 );
//        LinearLayoutManager recyclerViewLayoutManager = new LinearLayoutManager( this );
        refreshRecyclerView.setLayoutManager(recyclerViewLayoutManager);
        refreshRecyclerView.setOnPullRefreshListener( new PullRefreshRecyclerView.OnPullRefreshListener() {
            @Override
            public void onRefresh() {
                mFiLiPresenter.refresh();
            }

            @Override
            public void loadMore() {
                mFiLiPresenter.loadMore();
            }
        } );
        adapter = new FiLiAdapter(refreshRecyclerView);
        refreshRecyclerView.setAdapter( adapter );
        mFiLiPresenter = new FuLiPresenter( this );
        mFiLiPresenter.start();
    }

    @Override
    public void setPresenter(Object presenter) {

    }

    @Override
    public void refreshFinish(List<BeautyGirls.Girl> results) {
        refreshRecyclerView.setRefreshFinish();
        adapter.updateData( results );
    }

    @Override
    public void loadMoreFinish(List<BeautyGirls.Girl> results) {
        refreshRecyclerView.setLoadMoreFinish();
        adapter.addData( results );
    }

    @Override
    public void refreshFailed() {

    }

    @Override
    public void loadMoreFailed() {

    }
}
