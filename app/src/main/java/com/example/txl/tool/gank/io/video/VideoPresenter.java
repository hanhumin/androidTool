package com.example.txl.tool.gank.io.video;

import android.util.Log;

import com.example.txl.tool.GankIoApi;
import com.example.txl.tool.utils.AppExecutors;
import com.example.txl.tool.utils.TextUtils;
import com.google.gson.Gson;
import org.jsoup.Connection;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.select.Elements;

import java.io.IOException;
import java.util.List;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/7
 * description：
 */
public class VideoPresenter implements VideoContract.Presenter{
    private static final String TAG = "VideoPresenter";
    /**
     * 默认每页的请求的数据
     * */
    int defaultPageCount = 10;

    /**
     * 当前页数
     * */
    int currentPageIndex = 1;

    VideoContract.View videoView;

    boolean isPullRefresh = false;

    public VideoPresenter(VideoContract.View videoView) {
        this.videoView = videoView;
        videoView.setPresenter( this );
    }

    public void getVideoData(final boolean refresh){
        if(isPullRefresh){
            return;
        }
        isPullRefresh = true;
        String url = GankIoApi.URL_GET_VIDEO_DATA +""+defaultPageCount+"/"+currentPageIndex;
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
//                if(refresh){
//                    iGetFuLiData.updateFuLiDataFailed();
//                }else {
//                    iGetFuLiData.onAddFuLiDataFailed();
//                }
                currentPageIndex--;
                isPullRefresh = false;

            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                String jsonString = TextUtils.replaceBlank( response.body().string());
                Log.d( TAG,"getFuLiData onResponse " +jsonString);
                Gson gson = new Gson();
                final VideoBean root = gson.fromJson( jsonString, VideoBean.class);
                List<VideoBean.VideoInfo> list = root.getResults();
                for (VideoBean.VideoInfo videoInfo:list){

                    Connection connection = Jsoup.connect(videoInfo.getUrl()  );
                    Document document = connection.get();
                    Elements content = document.select("div.main-content-block");
                    Elements script = content.select( "script" );

                    String videoContent = "{"+script.last().toString().split( "\\{" )[2].split( "\\}" )[0]+"}";
                    VideoBean.VideoInfo.VideoContent content2 = gson.fromJson( videoContent, VideoBean.VideoInfo.VideoContent.class);
                    videoInfo.setContent( content2 );
                    Log.d( TAG,"videoContent;  "+ videoContent);

                }
                Runnable runnable = new Runnable() {
                    @Override
                    public void run() {
                        if(root.isError() || root.getResults() == null){
                            Log.e( TAG, "getFuLiData onResponse "+root.isError()+" root.getCategories() "+ root.getResults());
//                            if(refresh){
//                                iGetFuLiData.updateFuLiDataFailed();
//                            }else {
//                                iGetFuLiData.onAddFuLiDataFailed();
//                            }
                            currentPageIndex --;
                            isPullRefresh = false;
                            return;
                        }
                        if(refresh){
                            videoView.refreshFinish(root.getResults());
                        }else {
                            videoView.loadMoreFinish(root.getResults());
                        }
                        isPullRefresh = false;
                    }
                };
                new AppExecutors().mainThread().execute( runnable );
            }
        });
    }

    @Override
    public void start() {
        currentPageIndex = 1;
        getVideoData( true );
    }

    @Override
    public void refresh() {
        currentPageIndex = 1;
        getVideoData( true );
    }

    @Override
    public void loadMore() {
        currentPageIndex ++;
        getVideoData( true );
    }
}
