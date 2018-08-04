package com.example.txl.tool.recycler;

import com.example.txl.tool.BasePresenter;
import com.example.txl.tool.BaseView;
import com.example.txl.tool.recycler.bean.BeautyGirls;

import java.util.List;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/2
 * description：
 */
public interface FuLiContract {
    interface Presenter extends BasePresenter{
        void refresh();
        void loadMore();
    }

    interface View extends BaseView {
        void refreshFinish(List<BeautyGirls.Girl> results);
        void loadMoreFinish(List<BeautyGirls.Girl> results);

        void refreshFailed();
        void loadMoreFailed();
    }
}
