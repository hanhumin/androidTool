package com.example.txl.tool.glide

import android.graphics.drawable.Drawable
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.widget.ImageView
import com.bumptech.glide.Glide
import com.bumptech.glide.load.DataSource
import com.bumptech.glide.load.engine.GlideException
import com.bumptech.glide.request.RequestListener
import com.bumptech.glide.request.RequestOptions
import com.bumptech.glide.request.target.CustomTarget
import com.bumptech.glide.request.target.Target
import com.bumptech.glide.request.transition.Transition
import com.example.txl.tool.R



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

    }
}
