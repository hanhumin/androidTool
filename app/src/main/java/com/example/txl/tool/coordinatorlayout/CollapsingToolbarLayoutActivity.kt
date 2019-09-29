package com.example.txl.tool.coordinatorlayout

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.google.android.material.appbar.CollapsingToolbarLayout
import androidx.core.content.ContextCompat
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
