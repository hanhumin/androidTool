package com.example.txl.tool.gank.io.video;



import com.example.txl.tool.BasePresenter;
import com.example.txl.tool.BaseView;

import java.util.List;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/7
 * description：
 */
public class VideoContract {
    interface View extends BaseView<Presenter> {
        void refreshFinish(List<VideoBean.VideoInfo> videoInfoList);
        void loadMoreFinish(List<VideoBean.VideoInfo> videoInfoList);
    }

    interface Presenter extends BasePresenter {
        void refresh();
        void loadMore();
    }
}
