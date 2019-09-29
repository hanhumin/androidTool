package com.example.txl.tool.coordinatorlayout

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
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
        val recyclerView = findViewById<androidx.recyclerview.widget.RecyclerView>(R.id.recycler_view)
        recyclerView.layoutManager = androidx.recyclerview.widget.LinearLayoutManager(this, androidx.recyclerview.widget.LinearLayoutManager.VERTICAL, false)
        listItem = ArrayList<String>()
        for (i in 0..99) {
            listItem.add("我是第" + i + "个元素")
        }
        recyclerView?.adapter = object : androidx.recyclerview.widget.RecyclerView.Adapter<MyViewHolder>() {
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

    class MyViewHolder (itemView: View): androidx.recyclerview.widget.RecyclerView.ViewHolder(itemView) {
        var tv: TextView = itemView as TextView
    }
}
