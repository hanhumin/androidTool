package com.txl.player.demo

import android.content.Context
import android.os.Bundle
import android.util.Log
import android.view.KeyEvent
import androidx.fragment.app.FragmentActivity
import com.huaqiyun.player.R
import android.content.pm.PackageManager
import android.Manifest.permission
import android.Manifest.permission.WRITE_EXTERNAL_STORAGE
import android.os.Build
import com.txl.player.base.IPlayer
import com.txl.player.controller.AbsBasePlayerController
import com.txl.player.controller.IPlayerUiController
import com.txl.player.ui.PlayerView


class DemoPlayerActivity : FragmentActivity() {
    private val TAG = "DemoPlayerActivity"
    var playerController: AbsBasePlayerController? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_demo_player)

        val playerView: PlayerView = findViewById(R.id.player_view)

        playerController = PlayerController(this, playerView)

        /**
         * 动态获取权限，Android 6.0 新特性，一些保护权限，除了要在AndroidManifest中声明权限，还要使用如下代码动态获取
         */
        if (Build.VERSION.SDK_INT >= 23) {
            val REQUEST_CODE_CONTACT = 101
            val permissions = arrayOf<String>(WRITE_EXTERNAL_STORAGE, permission.READ_EXTERNAL_STORAGE)
            //验证是否许可权限
            for (str in permissions) {
                if (this.checkSelfPermission(str) != PackageManager.PERMISSION_GRANTED) {
                    //申请权限
                    this.requestPermissions(permissions, REQUEST_CODE_CONTACT)
                    return
                }
            }
        }
        playerController?.play()
        playerController?.attachLifecycle(lifecycle)
    }

    override fun dispatchKeyEvent(event: KeyEvent?): Boolean {
        if(playerController?.dispatchKeyEvent(event)!!){
            return true
        }
        return super.dispatchKeyEvent(event)
    }

    class PlayerController(context:Context,playerView: IPlayerUiController) :AbsBasePlayerController(context,playerView){
        override fun play() {
//            mPlayer.dataSource = "http://livetxqc.chinamcache.com/jntx/zb01.m3u8"
//            mPlayer.dataSource = "https://livepgc.sobeycache.com/pgc/c675811bcab3d57524bc658e3a039ed8.m3u8?auth_key=1584628057-0-0-99c48346c614bbcbe8a1b7eaf3a2f347"
//            mPlayer.dataSource = "https://jndstvod.chinamcache.com/jndst/vod/2019/09/24/c122b4c22b774606ba1d297b4bab1aa7/c122b4c22b774606ba1d297b4bab1aa7_800k.mp4"
//            mPlayer.dataSource = "https://mserver.chinamcloud.cn/vms/video/test/vod/2019/11/28/E450EB8190A242899DA7F27AB1BB425D/E450EB8190A242899DA7F27AB1BB425D_h264_1000k_mp4.mp4"
//            playByUrl("https://video.dmqhyadmin.com/qhswxcb/vod/2020/04/18/b38e82e229fd4983bfc276254cca19f8/b38e82e229fd4983bfc276254cca19f8_h264_800k_mp4.mp4")
//            playByUrl("http://116.77.76.10:5000/nn_vod/nn_x64/aWQ9NmVlYWYyNTU0M2M0NDkyNTc3NTVmZmY5NmFkOGEwNmQmdXJsX2MxPTczNzA1ZjczNzM3YTcxMmYzMjMwMzIzMDJmMzAzNDJmMzIzMjJmNjYzMTM5MzgzMTMyNjU2MjYxNjI2MTY2MzQzOTMwMzg2MjM1MzczNzM3NjIzNTMyMzUzMzMzMzkzMDMyMzQ2MzJmNjYz/MTM5MzgzMTMyNjU2MjYxNjI2MTY2MzQzOTMwMzg2MjM1MzczNzM3NjIzNTMyMzUzMzMzMzkzMDMyMzQ2MzJlNmQzMzc1MzgwMCZubl9haz0wMTViYjIyODY0MjU0NWI4ZDhhN2ZkM2YxNzE2ZDQyNWY1Jm5waXBzPTE3Mi4yNy4xNy43NTo1MTAwJm5jbXNpZD0xMDAw/MDEwMyZuZ3M9NWViZDM5NDIwMDAyNjE1MmIwMGUwMzQ1NDhmN2JiMTkmbm5kPXR3bmV0Lmd1YW5nZG9uZy5zaGVuJm5zZD16Z2R4LnNoZW56aGVuJm5mdD1tM3U4Jm5uX3VzZXJfaWQ9NTdiZWI3Mzk3YWU5OGFhMjBlNWNmYWFmYWJmNjUwMGQmbmR0PXN0YiZuZGk9/ODA3NTU4ODAwOTg4NzEyMyZuZHY9MS40LjE2LlNUQi5GVFRILlNURC5EVkJfU0tXMDEuUmVsZWFzZSZuc3Q9aXB0diZuY2E9JTI2bmFpJTNkZjE5ODEyZWJhYmFmNDkwOGI1Nzc3YjUyNTMzOTAyNGMlMjZubl9jcCUzZHNwX3NzenEmbmFsPTAxNDIzOWJkNWUwNjA3/NGYwNDBmY2QyZTM0MDU0MTlhZTk3MTEwNzUzYTI4NzM,/6eeaf25543c449257755fff96ad8a06d.m3u8")
//            playByUrl("http://116.77.76.7:5000/nn_vod/nn_x64/aWQ9MzUzZTEyOTgzOTg4Yzk1ZGNjZDAwMDg3MDU4MzM1Y2UmdXJsX2MxPTczNzA1ZjczNzM3YTcxMmYzMjMwMzIzMDJmMzAzNTJmMzEzNDJmNjUzOTM0Mzg2NTYxMzAzMjM1Mzc2MTY0MzQzMjM4MzQzOTMyNjMzODY0MzQzMzYxMzczNDYyNjY2MjMyNjQ2NTJmNjUz/OTM0Mzg2NTYxMzAzMjM1Mzc2MTY0MzQzMjM4MzQzOTMyNjMzODY0MzQzMzYxMzczNDYyNjY2MjMyNjQ2NTJlNmQzMzc1MzgwMCZubl9haz0wMWQzYWU3MWQzMzUwNGRlMDJmMmI1NmU5OGU2MzhkMjEyJm5waXBzPTE3Mi4yNy4xNy42Njo1MTAwJm5jbXNpZD0xMDAx/MDEwNiZuZ3M9NWVjNjgwN2QwMDBiM2Y1NTU0M2MyNjU5NTJhYjdhYTAmbm5kPXR3bmV0Lmd1YW5nZG9uZy5zaGVuJm5zZD16Z2R4LnNoZW56aGVuJm5mdD1tM3U4Jm5uX3VzZXJfaWQ9NWM3Nzg3MjUzOThiMDJhZGExNzIwMjFiNWVjYTM5M2YmbmR0PXN0YiZuZGk9/ODA3NTU4ODAxOTEyMjI2MyZuZHY9MS41LjQuU1RCLkZUVEguU1RELkRWQl9TS1cwMS5SZWxlYXNlJm5zdD1pcHR2Jm5jYT0lMjZuYWklM2RlOTQ4ZWEwMjU3YWQ0Mjg0OTJjOGQ0M2E3NGJmYjJkZSUyNm5uX2NwJTNkc3Bfc3N6cSZuYWw9MDE3ZDgwYzY1ZTA2MDdj/YTAzMmVkZjBkMWMyNGVlNmViYzg2MzYzMjIxZjA3Nw,,/353e12983988c95dccd00087058335ce.m3u8")
            playByUrl("https://livepull-sz20.cutv.com/live/stream/playlist.m3u8")
//            mPlayer.reset()
//            mPlayer.dataSource = "sdcard" + "/4k265.mp4"
//            mPlayer.prepareAsync()
        }

        override fun onCompletion(mp: IPlayer?) {
            super.onCompletion(mp)
            Log.d(TAG,"onCompletion")
        }

        override fun onError(mp: IPlayer?, what: Int, extra: Int, message: String?): Boolean {
            Log.d(TAG,"onError")
            return super.onError(mp, what, extra, message)
        }
    }


}
