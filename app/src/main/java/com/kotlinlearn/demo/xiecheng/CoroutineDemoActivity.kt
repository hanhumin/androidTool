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
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import okhttp3.*
import java.io.IOException
import java.lang.Exception

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
                // 没有指定调度器  效果和Dispatchers.Default相同
                GlobalScope.launch() {
                    testNet()
                }
            }
        }
        tvTest3.setOnClickListener {
            tvTest3.post {
                //  没有指定调度器  在线程池中运行
                GlobalScope.launch(Dispatchers.Default) {
                    testNet()
                }
            }
        }
        tvTest4.setOnClickListener {
            tvTest3.post {
                //直接执行会出现异常  Main调度器  会在主线程中执行
                GlobalScope.launch(Dispatchers.Main) {
                    testNet()
                }
            }
        }
        tvTest5.setOnClickListener {
            tvTest3.post {
                //直接执行会出现异常  IO调度器  从日志观察来看  io和Default的线程名字是一样的
                GlobalScope.launch(Dispatchers.IO) {
                    testNet()
                }
            }
        }
        tvTest6.setOnClickListener {
            tvTest3.post {
                //直接执行会出现异常  Unconfined调度器  会在所在创建线程的进行调度执行
                GlobalScope.launch(Dispatchers.Unconfined) {
                    testNet()
                }
            }
        }
        //协程调度是并行的  多个协程可以同时开始运行
        GlobalScope.launch {
            testCoroutineSerialParallel("default 01")
        }
        GlobalScope.launch {
            testCoroutineSerialParallel("default 02")
        }

        //线程池执行
        GlobalScope.launch(Dispatchers.IO) {
            testCoroutineSerialParallel("IO 01")
        }
        GlobalScope.launch(Dispatchers.IO) {
            testCoroutineSerialParallel("IO 02")
        }

        //同一个线程，协程的执行情况  串行执行
        GlobalScope.launch(Dispatchers.Unconfined) {
            testCoroutineSerialParallel("Unconfined 01")
        }
        GlobalScope.launch(Dispatchers.Unconfined) {
            testCoroutineSerialParallel("Unconfined 02")
        }
    }


    /**
     * 空耗cpu
     * */
    private fun consumeCpu(){
        var start = System.currentTimeMillis()
        var i = 0;
        while (i<5000){
            i++
            var j = 0;
            while (j<5000){
                j++
            }
        }
        Log.e(TAG,"consumeCpu time = ${System.currentTimeMillis()-start} ms")
    }

    /**
     * 测试协程串行，并行
     * */
    private suspend fun testCoroutineSerialParallel(tip:String){
        Log.e(TAG,"$tip start testCoroutineSerialParallel thread name : ${Thread.currentThread().name}")
//        consumeCpu();
        delay(500)
        Log.e(TAG,"$tip end testCoroutineSerialParallel thread name : ${Thread.currentThread().name}")
    }

    /**
     * 主线程协程 进行网络请求
     * */
    private fun testNet(){
        Log.e(TAG,"testNet start thread name ${Thread.currentThread().name} ")
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
        try {
//            delay(5*1000)
            val response = client.newCall(request).execute()
            Log.e(TAG,"testNet response $response")
        }catch (e:Exception){
            e.printStackTrace()
        }

    }
}