package com.example.txl.tool.coordinatorlayout

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.support.design.widget.CollapsingToolbarLayout
import android.support.v4.content.ContextCompat
import com.example.txl.tool.R

class CollapsingToolbarLayoutActivity : AppCompatActivity() {

    var collapsingToolbarLayout: CollapsingToolbarLayout?= null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_collapsing_toolbar_layout)

        initView()
    }

    private fun initView() {
        collapsingToolbarLayout = findViewById(R.id.collapsingToolbarLayout)
//        collapsingToolbarLayout?.contentScrim = ContextCompat.getDrawable(this,R.drawable.coordinator_demo)
    }
}
