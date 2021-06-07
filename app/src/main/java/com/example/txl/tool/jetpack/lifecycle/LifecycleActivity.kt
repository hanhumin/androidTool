package com.example.txl.tool.jetpack.lifecycle

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import com.example.txl.tool.R


class LifecycleActivity : AppCompatActivity() {
    private val TAG = LifecycleActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_lifecycle)
    }

    override fun onResume() {
        super.onResume()
        val observer = BasicLifecycleObserver(object : ILifecycleListener {
            override fun lifecycleCreate() {
                Log.d(TAG, "lifecycleCreate")
            }

            override fun lifecycleStart() {
                Log.d(TAG, "lifecycleStart")
            }

            override fun lifecycleResume() {
                Log.d(TAG, "lifecycleResume")
            }

            override fun lifecyclePause() {
                Log.d(TAG, "lifecyclePause")
            }

            override fun lifecycleStop() {
                Log.d(TAG, "lifecycleStop")
            }

            override fun lifecycleDestroy() {
                Log.d(TAG, "lifecycleDestroy")
            }
        })
        lifecycle.addObserver(observer)
    }
}
