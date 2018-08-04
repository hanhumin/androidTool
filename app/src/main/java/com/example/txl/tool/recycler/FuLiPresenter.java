package com.example.txl.tool.recycler;

import android.util.Log;

import com.example.txl.tool.recycler.bean.BeautyGirls;
import com.example.txl.tool.recycler.data.GankIoApi;
import com.example.txl.tool.utils.AppExecutors;
import com.example.txl.tool.utils.TextUtils;
import com.google.gson.Gson;

import java.io.IOException;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/3
 * description：
 */
public class FuLiPresenter implements FuLiContract.Presenter {
    private static String TAG = FuLiPresenter.class.getSimpleName();
    /**
     * 默认每页的请求的数据
     * */
    int defaultPageCount = 20;

    /**
     * 当前页数
     * */
    int currentPageIndex = 1;

    private final FuLiContract.View mFuLiView;

    private AppExecutors mAppExcutor;

    public FuLiPresenter(FuLiContract.View mFuLiView) {
        this.mFuLiView = mFuLiView;
        mAppExcutor = new AppExecutors();
    }


    @Override
    public void start() {
        refresh();
    }

    @Override
    public void refresh() {
        currentPageIndex = 1;
        getFuLiData(true);
    }

    @Override
    public void loadMore() {
        currentPageIndex++;
        getFuLiData(false);
    }

    public void getFuLiData(final boolean refresh){
        String url = GankIoApi.URL_GET_FULI_DATA +""+defaultPageCount+"/"+currentPageIndex;
        Log.d(TAG, "getFuLiData url : "+url);
        OkHttpClient okHttpClient = GankIoApi.getClient();
        final Request request = new Request.Builder()
                .cacheControl( GankIoApi.getDefaultCacheControl() )
                .url( url)
                .get()//默认就是GET请求，可以不写
                .build();
        Call call = okHttpClient.newCall(request);
        call.enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                Log.d(TAG, "getFuLiData onFailure: ");
                if(refresh){
                    mFuLiView.refreshFailed();
                }else {
                    mFuLiView.loadMoreFailed();
                }

            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                String jsonString = TextUtils.replaceBlank( response.body().string());
                Log.d( TAG,"getFuLiData onResponse " +jsonString);
                Gson gson = new Gson();
                final BeautyGirls root = gson.fromJson( jsonString, BeautyGirls.class);
                mAppExcutor.mainThread().execute( new Runnable() {
                    @Override
                    public void run() {
                        if(root.isError() || root.getResults() == null){
                            Log.e( TAG, "getFuLiData onResponse "+root.isError()+" root.getCategories() "+ root.getResults());
                            if(refresh){
                                mFuLiView.refreshFailed();
                            }else {
                                mFuLiView.loadMoreFailed();
                            }
                            return;
                        }
                        if(refresh){
                            mFuLiView.refreshFinish(root.getResults());
                        }else {
                            mFuLiView.loadMoreFinish(root.getResults());
                        }
                    }
                } );
            }
        });
    }
}
