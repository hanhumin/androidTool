package com.example.txl.tool.coordinatorlayout

import android.content.Context
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.support.v7.widget.LinearLayoutManager
import android.support.v7.widget.RecyclerView
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import com.example.txl.tool.R
import com.example.txl.tool.base.BaseAdapter
import com.example.txl.tool.utils.ColorUtils

class SimpleTitleBehaviorActivity : AppCompatActivity() {

    var recyclerView:RecyclerView? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_simple_title_behavior)
        initView()
    }

    private fun initView() {
        recyclerView = findViewById(R.id.recycler_view)
        recyclerView?.layoutManager = LinearLayoutManager(this,LinearLayoutManager.VERTICAL,false)
        var adapter = MyAdapter(this)
        val list = ArrayList<String>()
        for (i in 0..20){
            list.add("我是第 $i 个元素")
        }
        adapter.addListData(list)
        recyclerView?.adapter = adapter
    }

    class MyAdapter(context: Context) : BaseAdapter<String>(context){
        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): BaseViewHolder<String> {
            return MyViewHolder(layoutInflater.inflate(R.layout.textview_item_height_60dp,parent,false))
        }
    }

    class MyViewHolder(itemView: View) : BaseAdapter.BaseViewHolder<String>(itemView){
        override fun onBindViewHolder(position: Int, data: String, adapter: BaseAdapter<String>) {
           super.onBindViewHolder(position, data, adapter)
            itemView.setBackgroundColor(ColorUtils.randomColor())
            itemView as TextView
            itemView.text = data
        }
    }
}
