package com.kotlinlearn.demo.xiecheng

import android.graphics.BitmapFactory
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import com.example.txl.tool.R
import com.example.txl.tool.okhttp.OkHttpDemoActivity
import kotlinx.android.synthetic.main.activity_coroutine_demo2.*
import kotlinx.android.synthetic.main.activity_ok_http_demo.*
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import okhttp3.*
import java.io.IOException

class CoroutineDemoActivity : AppCompatActivity() {
    val TAG = "CoroutineDemoActivity"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_coroutine_demo2)
        tvTest1.setOnClickListener {
            tvTest1.post {
                //直接执行会出现异常
                testNet()
            }
        }
        tvTest2.setOnClickListener {
            tvTest2.post {
                //直接执行会出现异常
                GlobalScope.launch(Dispatchers.Default) {
                    testNet()
                }
            }
        }
    }

    /**
     * 主线程协程 进行网络请求
     * */
    private fun testNet(){
        Log.d(TAG,"testNet start thread name ${Thread.currentThread().name} ")
        val client = OkHttpClient.Builder().build()
        val request = Request.Builder().url("http://ottback.wmsj.cqccn.com/upload/img/2020/0413/41_104516_22895.jpeg")
//        val request = Request.Builder().url("http://ottback.wmsj.cqccn.com/upload/img/2020/0413/41_104441_49692.png")
//        val request = Request.Builder().url("http://a4.att.hudong.com/21/09/01200000026352136359091694357.jpg")
                .get()
                .build()
//        client.newCall(request).enqueue(object : Callback {
//            override fun onFailure(call: Call, e: IOException) {
//                Log.e(OkHttpDemoActivity.TAG,"onFailure :: ${e.message}")
//            }
//
//            override fun onResponse(call: Call, response: Response) {
//                Log.e(OkHttpDemoActivity.TAG,"onResponse")
//                val bit = BitmapFactory.decodeStream(response.body()?.byteStream())
//                image.post {
//                    image.setImageBitmap(bit)
//                }
//
//            }
//        })
        val response = client.newCall(request).execute()
        Log.d(TAG,"testNet response $response")
    }
}