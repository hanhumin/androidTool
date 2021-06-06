package com.txl.player.lifecycle

/**
 * Copyright (c) 2020 唐小陆 All rights reserved.
 * author：txl
 * date：2020/4/17
 * description：实现生命周期的监听
 */
interface ILifecycleListener {
    fun lifecycleCreate()

    fun lifecycleStart()

    fun lifecycleResume()

    fun lifecyclePause()

    fun lifecycleStop()

    fun lifecycleDestroy()
}
