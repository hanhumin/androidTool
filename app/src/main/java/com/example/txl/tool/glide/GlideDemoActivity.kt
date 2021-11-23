package com.example.txl.tool.glide

import android.graphics.BitmapFactory
import android.graphics.drawable.Drawable
import android.net.Uri
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.ImageView
import com.bumptech.glide.Glide
import com.bumptech.glide.load.DataSource
import com.bumptech.glide.load.engine.GlideException
import com.bumptech.glide.load.resource.bitmap.CenterCrop
import com.bumptech.glide.load.resource.bitmap.RoundedCorners
import com.bumptech.glide.request.RequestListener
import com.bumptech.glide.request.RequestOptions
import com.bumptech.glide.request.target.CustomTarget
import com.bumptech.glide.request.target.Target
import com.bumptech.glide.request.transition.Transition
import com.example.txl.tool.R
import com.example.txl.tool.utils.DisplayUtil
import kotlinx.android.synthetic.main.activity_glide_demo.*


class GlideDemoActivity : AppCompatActivity() {
    private var image:ImageView? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_glide_demo)
        image = findViewById(R.id.image_test_glide)

        image?.postDelayed({
            val requestOptions = RequestOptions()
            requestOptions
                    .placeholder(R.drawable.coordinator_demo)
                    .error(R.drawable.coordinator_demo)

            Glide.with(this).`as`(Drawable::class.java).load("http://pic13.nipic.com/20110409/7119492_114440620000_2.jpg").addListener(object :RequestListener<Drawable>{
                override fun onLoadFailed(e: GlideException?, model: Any?, target: Target<Drawable>?, isFirstResource: Boolean): Boolean {
                 return false
                }

                override fun onResourceReady(resource: Drawable?, model: Any?, target: Target<Drawable>?, dataSource: DataSource?, isFirstResource: Boolean): Boolean {
                    return false
                }
            }).apply(requestOptions).into(image!!)
        },5)
        val roundedCorners= RoundedCorners(30)
//        val options = RequestOptions().transform(CenterCrop()).transform(roundedCorners)
        var options = RequestOptions().transform(CenterCrop(),roundedCorners)
//        Glide.with(this).load("http://mserver.wjdev.chinamcloud.cn/cms/mrzd/upload/Image/mrtp/2019/12/08/1_25f95541a8a04f7eb549b6cf33de808e.jpg").apply(options).into(image_test_glide_circle_radius)
        Glide.with(this)
                .load("https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1588009151719&di=839eb1f4efdcb634e9a349c1cf60a145&imgtype=0&src=http%3A%2F%2Fimg2.imgtn.bdimg.com%2Fit%2Fu%3D1120712552%2C1362700559%26fm%3D214%26gp%3D0.jpg")
                .apply(options)
                .into(image_test_glide_circle_radius)
//        options = RequestOptions.bitmapTransform(roundedCorners)
//        Glide.with(this)
//                .load("http://mserver.wjdev.chinamcloud.cn/cms/mrzd/upload/Image/64522/2020/02/28/1_780793f0fb594ce5bf6a99eefd956d32.jpg")
//                .override(DisplayUtil.dip2px(this,60f),DisplayUtil.dip2px(this,60f))//
//                .apply(options)
//                .addListener(object:RequestListener<Drawable>{
//                    override fun onLoadFailed(e: GlideException?, model: Any?, target: Target<Drawable>?, isFirstResource: Boolean): Boolean {
//                        return false
//                    }
//
//                    override fun onResourceReady(resource: Drawable?, model: Any?, target: Target<Drawable>?, dataSource: DataSource?, isFirstResource: Boolean): Boolean {
//                        image_test_glide_circle_radius2.setImageDrawable(resource)
//                        return true
//                    }
//                }).submit()
//        Glide.get(this).registry.append()
        Glide.with(this).load(Uri.parse("file:///android_asset/mygif.gif")).into(image_test_glide_test)
        Glide.with(this).load(assets.openFd("test_video.mp4")).into(image_test_glide_mp4)
//        Glide.with(this).load(assets.open("test_video.mp4")).into(image_test_glide_mp4)
        Glide.with(this).load(Uri.parse("file:///android_asset/test_bluetooth.mp3")).into(image_test_glide_mp3)
//        Glide.with(this).load(assets.open("test_bluetooth.mp3")).into(image_test_glide_mp3)
    }
}
