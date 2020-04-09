package com.example.txl.tool.recyclerView

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.example.txl.tool.R
import kotlinx.android.synthetic.main.activity_recycler_view_demo.*

class RecyclerViewDemoActivity : AppCompatActivity(), View.OnClickListener {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_recycler_view_demo)
        initView()
    }

    private fun initView() {
        tv_jump_item_decoration_demo.setOnClickListener(this)
    }

    override fun onClick(v: View?) {
        when(v?.id){
            R.id.tv_jump_item_decoration_demo->{
                val intent = Intent(this,ItemDecorationDemoActivity::class.java)
                startActivity(intent)
            }
        }
    }
}
