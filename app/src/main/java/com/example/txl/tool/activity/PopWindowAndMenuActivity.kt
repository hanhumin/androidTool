package com.example.txl.tool.activity

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.support.v7.widget.PopupMenu
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.PopupWindow
import com.example.txl.tool.R

class PopWindowAndMenuActivity : AppCompatActivity() {
    var popWindow: PopupWindow? = null
    var popMenu: PopupMenu? = null
    var btPopWindow: Button? = null
    var btPopMenu: Button? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_pop_window_and_menu)
        initView()
    }

    var state = false;
    private fun initView() {
        btPopWindow = findViewById(R.id.bt_popWindow)
        btPopWindow?.setOnClickListener {
            if(!state){
                popWindow?.showAsDropDown(btPopWindow)
            }else{
                popWindow?.dismiss()
            }


        }
        btPopMenu = findViewById(R.id.bt_popMenu)

        var view = layoutInflater.inflate(R.layout.popwindow_demo_content,null,false)
        popWindow = PopupWindow(view,ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT)

    }

}
