package com.example.txl.tool.jetpack.livedata

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.ViewModelProviders
import com.example.txl.tool.R
import kotlinx.android.synthetic.main.activity_live_data.*

class LiveDataActivity : AppCompatActivity() {
    private val TAG = LiveDataActivity::class.java.simpleName
    private lateinit var numViewModel:NumViewModel

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_live_data)
        initView()
    }

    override fun onResume() {
        super.onResume()
    }

    private fun initView() {
        numViewModel = ViewModelProvider(this).get(NumViewModel::class.java)
        val obs = object : Observer<Int> {
            override fun onChanged(t: Int?) {
                Log.d(TAG, "onChanged")
                tv_text.text = "$t"
            }
        }
        numViewModel.numData.observe(this, obs)
        button_sub.setOnClickListener {
            val num = numViewModel.numData.value
            if (num == null) {
                numViewModel.numData.value = 0
            } else {

                numViewModel.numData.value = num - 1
            }
        }
        button_add.setOnClickListener {
            val num = numViewModel.numData.value
            if (num == null) {
                numViewModel.numData.value = 0
            } else {

                numViewModel.numData.value = num + 1
            }
        }
    }
}
