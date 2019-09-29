package com.example.txl.tool.activity

import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import android.util.Log
import android.view.Gravity
import android.view.View
import android.view.ViewGroup
import android.widget.*
import com.example.txl.tool.R
import com.example.txl.tool.utils.floatview.OVER_MODEL_SHOW_BOTTOM
import com.example.txl.tool.utils.floatview.SuspensionListViewUtils
import com.txl.lib.custom_view.floatitem.FloatItemRecyclerView


/**
 * 给recyclerView添加一个浮窗
 * */

class FloatItemRecyclerActivity : AppCompatActivity() {

    val tag: String = javaClass.simpleName

    var recyclerView: androidx.recyclerview.widget.RecyclerView? = null
    var listView:ListView? = null
    var floatView:FloatItemRecyclerView<androidx.recyclerview.widget.RecyclerView>? = null


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_float_item_recycler)
        initView()
    }

    fun initView(){
        recyclerView = findViewById(com.example.txl.tool.R.id.recycler_view)
        recyclerView?.layoutManager = androidx.recyclerview.widget.LinearLayoutManager(this, androidx.recyclerview.widget.LinearLayoutManager.VERTICAL, false)
        recyclerView?.adapter = MyAdapter()
        recyclerView?.addItemDecoration(androidx.recyclerview.widget.DividerItemDecoration(this, androidx.recyclerview.widget.LinearLayoutManager.VERTICAL))
        recyclerView?.addOnScrollListener(MyOnScrollListener())
        floatView = FloatItemRecyclerView(this)

        listView = findViewById(R.id.list_view)
        val adapter = object:ArrayAdapter<String>(this,R.layout.float_item_recycle_view, R.id.tv_text,listOf("0","1","2","3","4","5","6","7","8","9","10")){
            override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
                val view = super.getView(position, convertView, parent)
                view.setTag( R.id.tv_text,position)//这个为了解决view重用的问题
                return view
            }
        }
        listView?.adapter = adapter
        val suspensionListViewUtils = SuspensionListViewUtils(R.id.tv_text)
        suspensionListViewUtils.mOverModel = OVER_MODEL_SHOW_BOTTOM
        suspensionListViewUtils.setListView(listView!!)
        val suspensionView = TextView(this)
        suspensionView.text = "我是悬浮"
        suspensionView.setBackgroundColor(0x77888888)
        suspensionView.gravity = Gravity.CENTER
        suspensionView.visibility = View.GONE
        val decorView = findViewById<FrameLayout>(R.id.root_frame)
        decorView.addView(suspensionView)
        suspensionListViewUtils.suspensionView = suspensionView
//        suspensionListViewUtils.setSuspensionViewStateListener(object :SuspensionListViewUtils.SuspensionViewStateListener{
//            override fun onSuspensionViewStateGone(suspensionView: View) {
//
//            }
//
//            override fun onSuspensionViewStateBottom(suspensionView: View) {
//
//            }
//
//            override fun onSuspensionViewStateFloating(needSuspensionChild: View, suspensionView: View) {
//
//            }
//        })
        val tv = TextView(this)
        tv.text = "sad我是悬浮"
        tv.setBackgroundColor(0x77888888)
        tv.gravity = Gravity.CENTER
        listView?.addHeaderView(tv)
        val tv3 = TextView(this)
        tv3.text = "我是悬浮654546564546"
        tv3.setBackgroundColor(0x77888888)
        tv3.gravity = Gravity.CENTER
        listView?.addHeaderView(tv3)

        var tv4 = TextView(this)
        tv4.text = "我是addFooterView1"
        tv4.setBackgroundColor(0x77888888)
        tv4.gravity = Gravity.CENTER
        listView?.addFooterView(tv4)

        tv4 = TextView(this)
        tv4.text = "我是addFooterView2"
        tv4.setBackgroundColor(0x77888888)
        tv4.gravity = Gravity.CENTER
        listView?.addFooterView(tv4)
        listView?.onItemClickListener = object : AdapterView.OnItemClickListener {
            override fun onItemClick(parent: AdapterView<*>?, view: View?, position: Int, id: Long) {
                suspensionView.text = "我是悬浮 position: $position"
                suspensionListViewUtils.setNeedSuspensionView(view!!,position)
            }
        }
    }

    var currentState = 0;
    inner class MyOnScrollListener: androidx.recyclerview.widget.RecyclerView.OnScrollListener (){
//        override fun onScrollStateChanged(recyclerView: RecyclerView, newState: Int) {
//            super.onScrollStateChanged(recyclerView, newState)
//            if (floatView == null) {
//                return
//            }
//            currentState = newState
//            when (newState) {
//                // 停止滑动
//                0 -> {
//                    // 对正在显示的浮层的child做个副本，为了判断显示浮层的child是否发现了变化
//                    val tempFirstChild = needFloatChild
//                    // 更新浮层的位置，覆盖child
//                    updateFloatScrollStopTranslateY()
//                    // 如果firstChild没有发生变化，回调floatView滑动停止的监听
//                    if (tempFirstChild === needFloatChild) {
//                        if (onFloatViewShowListener != null) {
//                            onFloatViewShowListener.onScrollStopFloatView(floatView)
//                        }
//                    }
//                }
//                // 开始滑动
//                1 ->
//                    // 更新浮层的位置
//                    updateFloatScrollStartTranslateY()
//            }// Fling
//            // 这里有一个bug，如果手指在屏幕上快速滑动，但是手指并未离开，仍然有可能触发Fling
//            // 所以这里不对Fling状态进行处理
//            //                    case 2:
//            //                        hideFloatView();
//            //                        break;
//        }
//
//        override fun onScrolled(recyclerView: RecyclerView, dx: Int, dy: Int) {
//            super.onScrolled(recyclerView, dx, dy)
//            if (floatView == null) {
//                return
//            }
//            when (currentState) {
//                // 停止滑动
//                0 -> updateFloatScrollStopTranslateY()
//                // 开始滑动
//                1 -> updateFloatScrollStartTranslateY()
//                // Fling
//                2 -> {
//                    updateFloatScrollStartTranslateY()
//                    if (onFloatViewShowListener != null) {
//                        onFloatViewShowListener.onScrollFlingFloatView(floatView)
//                    }
//                }
//            }
//        }
    }

    inner class MyAdapter : androidx.recyclerview.widget.RecyclerView.Adapter<MyViewHolder>() {
        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): MyViewHolder {

            return MyViewHolder(layoutInflater.inflate(R.layout.float_item_recycle_view,parent,false))
        }

        override fun getItemCount(): Int {
            return 10
        }

        override fun onBindViewHolder(holder: MyViewHolder, position: Int) {

        }

    }

    class MyViewHolder(itemView: View) : androidx.recyclerview.widget.RecyclerView.ViewHolder(itemView) {

    }
}
