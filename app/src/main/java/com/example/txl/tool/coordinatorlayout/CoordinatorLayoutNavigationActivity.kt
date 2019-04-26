package com.example.txl.tool.coordinatorlayout

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.TextView
import com.example.txl.tool.R

/**
 * 导航
 * */
class CoordinatorLayoutNavigationActivity : AppCompatActivity(), View.OnClickListener {
    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.tv_jump_CoordinatorLayoutActivity -> {
                val intent = Intent(this,CoordinatorLayoutActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_CollapsingToolbarLayout -> {
                val intent = Intent(this,CollapsingToolbarLayoutActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_FloatActionBar -> {
                val intent = Intent(this,FloatingActionButtonActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_custom_behavior->{
                val intent = Intent(this,CustomBehaviorActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_simple_behavior->{
                val intent = Intent(this,SimpleTitleBehaviorActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_recycler_behavior->{
                val intent = Intent(this,RecyclerViewBehaviorActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_LayoutAnchor->{
                val intent = Intent(this,LayoutAnchorActivity::class.java)
                startActivity(intent)
            }
            R.id.tv_jump_LayoutDodgeInsetEdges->{
                val intent = Intent(this,LayoutDodgeInsetEdgesActivity::class.java)
                startActivity(intent)
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_coordinator_layout_navigation)
        initView()
    }

    private fun initView() {
        val tvJumpToolbar: TextView = findViewById(R.id.tv_jump_CoordinatorLayoutActivity)
        tvJumpToolbar.setOnClickListener(this)
        val tvJumpCollapsing: TextView = findViewById(R.id.tv_jump_CollapsingToolbarLayout)
        tvJumpCollapsing.setOnClickListener(this)
        val tvJumpFloatingActionButton: TextView = findViewById(R.id.tv_jump_FloatActionBar)
        tvJumpFloatingActionButton.setOnClickListener(this)
        val tvJumpcustomBehavior: TextView = findViewById(R.id.tv_jump_custom_behavior)
        tvJumpcustomBehavior.setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_simple_behavior).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_recycler_behavior).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_LayoutAnchor).setOnClickListener(this)
        findViewById<View>(R.id.tv_jump_LayoutDodgeInsetEdges).setOnClickListener(this)
    }
}
