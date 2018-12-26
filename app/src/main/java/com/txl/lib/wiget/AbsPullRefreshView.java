package com.txl.lib.wiget;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/2
 * description：
 */
public abstract class AbsPullRefreshView implements IPullRefresh {
    public static final int VIEW_TYPE_HEADER = 0;
    public static final int VIEW_TYPE_FOOTER = 1;

    /**
     * view的类型，头
     * */
    protected int viewType = VIEW_TYPE_HEADER;

    /**
     * 未被初始化
     * */
    protected static final int VIEW_STATE_UNINIT = -1;
    protected static final int VIEW_STATE_NORMAL = 0;
    protected static final int VIEW_STATE_PULL = 1;
    protected static final int VIEW_STATE_RELEASE = 2;
    protected static final int VIEW_STATE_RUNNING = 3;

    private int viewState = VIEW_STATE_NORMAL;

    public AbsPullRefreshView(int viewType) {
        this.viewType = viewType;
    }

    public abstract void setViewMarginTop(int marginTop);

    public abstract void setViewMarginBottom(int marginBottom);

    protected void updateViewState(int viewState){
        this.viewState = viewState;
    }

    public int getViewState(){
        return viewState;
    }

}
