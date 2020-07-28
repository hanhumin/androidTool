package com.example.txl.tool.jetpack.livedata

import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel

/**
 * Copyright (c) 2020 唐小陆 All rights reserved.
 * author：txl
 * date：2020/7/26
 * description：
 */
class NumViewModel:ViewModel() {
    val numData  = MutableLiveData<Int>()

    init {
        numData.value = 5
    }
}