package com.example.txl.tool.coordinatorlayout

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.support.v7.widget.LinearLayoutManager
import android.support.v7.widget.RecyclerView
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import com.example.txl.tool.R
import java.util.ArrayList

class CustomBehaviorActivity : AppCompatActivity() {

    private var listItem:  ArrayList<String> =ArrayList()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_custom_behavior)
        initView()
    }

    private fun initView() {
        val recyclerView = findViewById<RecyclerView>(R.id.recycler_view)
        recyclerView.layoutManager = LinearLayoutManager(this,LinearLayoutManager.VERTICAL,false)
        listItem = ArrayList<String>()
        for (i in 0..99) {
            listItem.add("我是第" + i + "个元素")
        }
        recyclerView?.adapter = object : RecyclerView.Adapter<MyViewHolder>() {
            override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): MyViewHolder {
                val tv = layoutInflater.inflate(R.layout.cordinatory_test_item, null, false) as TextView
                return MyViewHolder(tv)
            }

            override fun onBindViewHolder(holder: MyViewHolder, position: Int) {
                holder.tv.text = listItem[position]
            }

            override fun getItemCount(): Int {
                return listItem.size
            }
        }
    }

    class MyViewHolder (itemView: View): RecyclerView.ViewHolder(itemView) {
        var tv: TextView = itemView as TextView
    }
}
