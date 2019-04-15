package com.example.txl.tool.coordinatorlayout

import android.os.Bundle
import android.support.v4.app.Fragment
import android.support.v7.widget.LinearLayoutManager
import android.support.v7.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import com.example.txl.tool.R
import java.util.ArrayList





/**
 *@author TXL
 *description :
 */
const val TITLE = "title"
class TableFragment : Fragment() {


    private var mTitle = "Defaut Value"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        if (arguments != null)
        {
            mTitle = arguments!!.getString(TITLE)
        }

    }

    private var listItem:  ArrayList<String> =ArrayList()
    var recyclerView: RecyclerView? = null
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        listItem = ArrayList<String>()
        for (i in 0..99) {
            listItem.add("我是第" + i + "个元素")
        }
        recyclerView?.layoutManager = LinearLayoutManager(context)
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

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        recyclerView = inflater.inflate(R.layout.table_fragment_item,container,false) as RecyclerView?
        return recyclerView
    }

    internal inner class MyViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var tv: TextView

        init {
            tv = itemView as TextView

        }
    }

    companion object {
        fun newInstance(title:String):TableFragment{
            val  tabFragment = TableFragment()
            val bundle = Bundle()
            bundle.putString(TITLE, title)
            tabFragment.arguments = bundle
            return tabFragment
        }
    }
}