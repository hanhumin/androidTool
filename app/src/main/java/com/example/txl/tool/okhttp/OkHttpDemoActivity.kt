package com.example.txl.tool.okhttp

import android.graphics.BitmapFactory
import android.net.Uri
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import com.bumptech.glide.Glide
import com.bumptech.glide.load.engine.DiskCacheStrategy
import com.example.txl.tool.R
import com.example.txl.tool.utils.AppExecutors
import kotlinx.android.synthetic.main.activity_ok_http_demo.*
import okhttp3.*
import java.io.IOException
import java.net.URL

class OkHttpDemoActivity : AppCompatActivity() {

    companion object{
        const val TAG = "OkHttpDemoActivity"
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_ok_http_demo)
//        AppExecutors().diskIO().execute {
//            Glide.get(this).clearDiskCache()
//            Glide.get(this).clearMemory()
//        }
        Glide.with(this).asBitmap().diskCacheStrategy(DiskCacheStrategy.NONE).load(Uri.parse("http://ottback.wmsj.cqccn.com/upload/img/2020/0413/41_104516_22895.jpeg")).into(image2)
//        Glide.with(this).asBitmap().diskCacheStrategy(DiskCacheStrategy.NONE).load("http://a4.att.hudong.com/21/09/01200000026352136359091694357.jpg").into(image2)
        testNet()
    }

    private fun testNet(){
        val client = OkHttpClient.Builder().build()
        val request = Request.Builder().url("http://ottback.wmsj.cqccn.com/upload/img/2020/0413/41_104516_22895.jpeg")
//        val request = Request.Builder().url("http://ottback.wmsj.cqccn.com/upload/img/2020/0413/41_104441_49692.png")
//        val request = Request.Builder().url("http://a4.att.hudong.com/21/09/01200000026352136359091694357.jpg")
                .get()
                .build()
        client.newCall(request).enqueue(object : Callback{
            override fun onFailure(call: Call, e: IOException) {
                Log.e(TAG,"onFailure :: ${e.message}")
            }

            override fun onResponse(call: Call, response: Response) {
                Log.e(TAG,"onResponse")
                val bit = BitmapFactory.decodeStream(response.body()?.byteStream())
                image.post {
                    image.setImageBitmap(bit)
                }

            }
        })
    }
}
