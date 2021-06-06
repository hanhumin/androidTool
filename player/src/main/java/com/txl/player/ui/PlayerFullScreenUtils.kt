package com.txl.player.ui

import android.app.Activity
import android.view.ViewGroup
import com.txl.player.controller.AbsBasePlayerController
import com.txl.player.utils.PlayerUtils

/**
 * Copyright (c) 2020 唐小陆 All rights reserved.
 * author：txl
 * date：2020/4/15
 * description：将播放器全屏/半屏切换
 */
class PlayerFullScreenUtils(private val playerView: PlayerView,private val playerController: AbsBasePlayerController) {
    var isFullScreen = false
        private set
    private lateinit var oldLayoutParams:ViewGroup.LayoutParams
    private var oldParent:ViewGroup? = null
    fun toggleFullScreen(activity: Activity){
//        playerController.pause()
        if(isFullScreen){
            val tempParent = playerView.parent as ViewGroup
            tempParent.removeView(playerView)
            oldParent?.addView(playerView,oldLayoutParams)
            isFullScreen = false
            playerView.requestFocus()
            playerView.controllerEnable(false)
        }else{
            oldParent = playerView.parent as ViewGroup
            oldLayoutParams = playerView.layoutParams
            oldParent?.removeView(playerView)
            val decorView = PlayerUtils.scanForActivity(playerView.context)!!.window.decorView as ViewGroup
            decorView.addView(playerView,ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))
            isFullScreen = true
            playerView.requestFocus()
            playerView.controllerEnable(true)
            playerView.seekBarView.requestFocus()
        }
//        playerController.start()
    }


}