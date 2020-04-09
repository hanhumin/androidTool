package com.example.txl.tool.gif

import android.graphics.Rect
import android.graphics.drawable.LayerDrawable
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.Gravity
import androidx.constraintlayout.solver.widgets.WidgetContainer.getBounds
import androidx.core.content.ContextCompat
import com.bumptech.glide.Glide
import com.bumptech.glide.load.DataSource
import com.bumptech.glide.load.engine.GlideException
import com.bumptech.glide.load.resource.gif.GifDrawable
import com.bumptech.glide.request.RequestListener
import com.bumptech.glide.request.target.Target
import com.example.txl.tool.R
import kotlinx.android.synthetic.main.activity_gif.*

class GifActivity : AppCompatActivity(), RequestListener<GifDrawable> {
    override fun onLoadFailed(e: GlideException?, model: Any?, target: Target<GifDrawable>?, isFirstResource: Boolean): Boolean {
        return true
    }

    override fun onResourceReady(resource: GifDrawable?, model: Any?, target: Target<GifDrawable>?, dataSource: DataSource?, isFirstResource: Boolean): Boolean {
        val drawable = ContextCompat.getDrawable(this,R.drawable.empty_bg)
//        resource?.setBounds(0,0,150,150)
        val layerDrawable = LayerDrawable(arrayOf(drawable,resource!!))
        tv_test_gif.background = layerDrawable
        resource.start()
        tv_test_gif.postDelayed({
            try {
                val field = GifDrawable::class.java.getDeclaredField("destRect")
                field.isAccessible = true
                val rect = Rect(50, 50, 200, 200)
                field.set(resource, rect)
            }catch (e:Exception){
                e.printStackTrace()
            }
        },200)
        return true
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_gif)


        Glide.with(this).asGif().listener(this).load(R.drawable.xiongmao).submit()
//        Glide.with(this).load(R.drawable.xiongmao).into(image_test_gif)
    }
}
