package com.example.txl.tool.coordinatorlayout

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.coordinatorlayout.widget.CoordinatorLayout
import com.example.txl.tool.R
import com.google.android.material.snackbar.Snackbar
import android.view.View


class FloatingActionButtonActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_floating_action_button)
        val mFab = findViewById<View>(R.id.fab)
        val mCoordinatorLayout = findViewById<androidx.coordinatorlayout.widget.CoordinatorLayout>(R.id.main_content)
        mFab.setOnClickListener(object : View.OnClickListener {
            override fun onClick(view: View) {
                Snackbar.make(mCoordinatorLayout, "点我分享哦！", Snackbar.LENGTH_SHORT).show()
            }
        })
    }
}
