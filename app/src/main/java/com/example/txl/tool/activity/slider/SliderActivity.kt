package com.example.txl.tool.activity.slider

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import androidx.fragment.app.FragmentStatePagerAdapter
import androidx.viewpager.widget.ViewPager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.FrameLayout
import com.example.txl.tool.R
import com.example.txl.tool.coordinatorlayout.SimpleTitleBehaviorActivity

class SliderActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_slider)
        initView()
    }

    /**
     * 引入viewPager的原因是观察他在fragment中嵌套横向滑动的RecyclerView的现象
     * */
    private fun initView() {
        val viewPager = findViewById<androidx.viewpager.widget.ViewPager>(R.id.view_pager)
        val adapter = PagerAdapter(supportFragmentManager)
        adapter.list.add(DemoFragment())
        adapter.list.add(DemoFragment())
        adapter.list.add(DemoFragment1())
        viewPager.adapter = adapter

        val fragmentManager = supportFragmentManager
        val transaction = fragmentManager.beginTransaction()
        transaction.replace(R.id.frameLayout,DemoFragment())
        transaction.commit()
    }

    class PagerAdapter(fm : androidx.fragment.app.FragmentManager) : androidx.fragment.app.FragmentStatePagerAdapter(fm){
        val list :ArrayList<androidx.fragment.app.Fragment> = ArrayList()
        override fun getItem(position: Int): androidx.fragment.app.Fragment {
            return list[position]
        }

        override fun getCount(): Int {
            return list.size
        }
    }
}

class DemoFragment : androidx.fragment.app.Fragment(){
    var rootView: androidx.recyclerview.widget.RecyclerView? = null
    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        rootView = inflater.inflate(R.layout.recycler_view,container,false) as androidx.recyclerview.widget.RecyclerView?
        return rootView
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        rootView as androidx.recyclerview.widget.RecyclerView
        rootView?.layoutManager = androidx.recyclerview.widget.LinearLayoutManager(context, androidx.recyclerview.widget.LinearLayoutManager.HORIZONTAL, false)
        var adapter = SimpleTitleBehaviorActivity.MyAdapter(context!!)
        val list = ArrayList<String>()
        for (i in 0..20){
            list.add("我是第 $i 个元素")
        }
        adapter.addListData(list)
        rootView?.adapter = adapter
    }


}


class DemoFragment1 : androidx.fragment.app.Fragment(){
    var rootView:View? = null
    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        rootView = inflater.inflate(R.layout.horizontal_scroller_view,container,false)
        return rootView
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
//        rootView as RecyclerView
//        rootView?.layoutManager = LinearLayoutManager(context, LinearLayoutManager.HORIZONTAL,false)
//        var adapter = SimpleTitleBehaviorActivity.MyAdapter(context!!)
//        val list = ArrayList<String>()
//        for (i in 0..20){
//            list.add("我是第 $i 个元素")
//        }
//        adapter.addListData(list)
//        rootView?.adapter = adapter
    }
}