package com.example.txl.tool.coordinatorlayout

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.support.design.widget.CoordinatorLayout
import com.example.txl.tool.R
import android.support.design.widget.Snackbar
import android.view.View


class FloatingActionButtonActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_floating_action_button)
        val mFab = findViewById<View>(R.id.fab)
        val mCoordinatorLayout = findViewById<CoordinatorLayout>(R.id.main_content)
        mFab.setOnClickListener(object : View.OnClickListener {
            override fun onClick(view: View) {
                Snackbar.make(mCoordinatorLayout, "点我分享哦！", Snackbar.LENGTH_SHORT).show()
            }
        })
    }
}
