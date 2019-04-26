package com.example.txl.tool.coordinatorlayout

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import com.example.txl.tool.R
import android.R.attr.start
import com.scwang.smartrefresh.layout.util.DensityUtil.dp2px
import android.animation.ObjectAnimator
import android.util.Property
import android.view.View
import android.widget.TextView



class LayoutDodgeInsetEdgesActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_layout_dodge_inset_edges)
        initView()
    }

    private fun initView() {
        val tvMove = findViewById<TextView>(R.id.tvMove)
        val tvFloat = findViewById<TextView>(R.id.tvFloat)

        val oa = ObjectAnimator.ofFloat(tvMove, "Y", dp2px(-50f).toFloat(),dp2px(0f).toFloat()).setDuration(2000)

        tvFloat.setOnClickListener { oa.start() }
    }
}
