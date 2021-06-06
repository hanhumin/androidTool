package com.txl.player.demo

import android.graphics.PixelFormat
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.FragmentActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.huaqiyun.player.R
import com.txl.player.controller.AbsBasePlayerController
import com.txl.player.ui.PlayerView
import kotlinx.android.synthetic.main.activity_recycler_demo.*

class RecyclerDemoActivity : FragmentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        window.setFormat(PixelFormat.TRANSLUCENT)
        setContentView(R.layout.activity_recycler_demo)
        recycler_view.layoutManager = LinearLayoutManager(this,LinearLayoutManager.HORIZONTAL,false)
        recycler_view.adapter = DemoAdapter()
    }
}

class DemoAdapter: RecyclerView.Adapter<RecyclerView.ViewHolder>() {
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecyclerView.ViewHolder {
        return DemoViewHolder(LayoutInflater.from(parent.context).inflate(R.layout.item_recycler_player, parent, false))
    }

    override fun getItemCount(): Int {
        return 1
    }

    override fun onBindViewHolder(holder: RecyclerView.ViewHolder, position: Int) {

    }
}

class DemoViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
    private var playerController: AbsBasePlayerController? = null
    private val playerView = itemView.findViewById<PlayerView>(R.id.player_view)
    init {
        itemView.setOnClickListener(object :View.OnClickListener{
            override fun onClick(v: View?) {
                if(playerController == null){
                    playerController = object:AbsBasePlayerController(v!!.context,playerView){}
                    playerController?.playByUrl("https://video.dmqhyadmin.com/qhswxcb/vod/2020/04/18/b38e82e229fd4983bfc276254cca19f8/b38e82e229fd4983bfc276254cca19f8_h264_800k_mp4.mp4")
                }else{
                    playerController?.start()
                }
            }
        })
    }
}