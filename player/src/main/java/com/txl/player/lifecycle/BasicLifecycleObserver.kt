package com.txl.player.lifecycle

import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleObserver
import androidx.lifecycle.OnLifecycleEvent

/**
 * Copyright (c) 2020 唐小陆 All rights reserved.
 * author：txl
 * date：2020/4/17
 * description：
 */
class BasicLifecycleObserver() : LifecycleObserver {
    private var listener: ILifecycleListener?  = null

    constructor(listener: ILifecycleListener):this(){
        this.listener = listener
    }

    fun setLifecycleListener(listener: ILifecycleListener){
        this.listener = listener
    }

    @OnLifecycleEvent(Lifecycle.Event.ON_CREATE)
    fun onCreate() {
        listener?.lifecycleCreate()
    }

    @OnLifecycleEvent(Lifecycle.Event.ON_START)
    fun onStart() {
        listener?.lifecycleStart()
    }

    @OnLifecycleEvent(Lifecycle.Event.ON_RESUME)
    fun onResume() {
        listener?.lifecycleResume()
    }

    @OnLifecycleEvent(Lifecycle.Event.ON_PAUSE)
    fun onPause() {
        listener?.lifecyclePause()
    }

    @OnLifecycleEvent(Lifecycle.Event.ON_STOP)
    fun onStop() {
        listener?.lifecycleStop()
    }

    @OnLifecycleEvent(Lifecycle.Event.ON_DESTROY)
    fun onDestroy() {
        listener?.lifecycleDestroy()
    }
}