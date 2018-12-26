package com.txl.lib.wiget;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/4
 * description：
 */
public interface IPullRefresh {
    int viewState = -1;
    View getView(Context context, ViewGroup parent);
    void onPull(int currentHeight, int refreshHeight, int state);
    void onRunning();
    void onStopRunning();
}
