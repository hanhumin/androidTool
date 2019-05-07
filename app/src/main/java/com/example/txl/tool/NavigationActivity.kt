package com.example.txl.tool

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.example.txl.tool.activity.http.HttpCookieDemoActivity
import com.example.txl.tool.activity.point9.Point9Activity
import com.example.txl.tool.coordinatorlayout.CoordinatorLayoutNavigationActivity

class NavigationActivity : AppCompatActivity(), View.OnClickListener {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_navigation)
        initView()
    }

    fun initView(){
        findViewById<View>(R.id.tv_jump_CoordinatorLayout).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_Http).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_nine_picture).setOnClickListener(this)
    }

    override fun onClick(v: View?) {
        when(v?.id){
            R.id.tv_jump_CoordinatorLayout->{
                val intent = Intent(this, CoordinatorLayoutNavigationActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_Http->{
                val intent = Intent(this, HttpCookieDemoActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_nine_picture->{
                val intent = Intent(this, Point9Activity::class.java)
                startActivity(intent)
            }
        }
    }
}
