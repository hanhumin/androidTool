package com.example.txl.tool.utils.floatview

import android.annotation.TargetApi
import android.os.Build
import android.support.annotation.RequiresApi
import android.util.Log
import android.view.View
import android.widget.AbsListView
import android.widget.ListView

/**
 *@author TXL
 *description :为ListView添加悬浮效果
 */

class SuspensionListViewUtils {
    val tag: String = javaClass.simpleName

    private var scrollerListeners: ArrayList<AbsListView.OnScrollListener>? = null
    private var _listView: ListView? = null
    /**
     * 悬浮View
     * */
    var suspensionView: View? = null

    /**
     * 需要在上面再是悬浮效果的那个View
     * 在view复用的情况下这个东西并不可靠
     * */
    private var needSuspensionChild:View? = null

    private var needSuspensionChildPosition = 0

    private var needSuspensionChildVisible: Boolean = false

    @TargetApi(Build.VERSION_CODES.M)
    fun setNeedSuspensionView(needSuspensionChild: View, needSuspensionChildPosition: Int) {
        this.needSuspensionChild = needSuspensionChild
        this.needSuspensionChildPosition = needSuspensionChildPosition
        needSuspensionChild.tag = needSuspensionChildPosition
        needSuspensionChild.setOnScrollChangeListener(object : View.OnScrollChangeListener{
            override fun onScrollChange(v: View?, scrollX: Int, scrollY: Int, oldScrollX: Int, oldScrollY: Int) {
                Log.d(tag,"setNeedSuspensionView onScrollChange ")
            }
        })
        suspensionView?.visibility = View.VISIBLE
        suspensionView?.layoutParams?.width = needSuspensionChild.width!!
        suspensionView?.layoutParams?.height = needSuspensionChild.height!!
        suspensionView?.translationY = needSuspensionChild.top.toFloat()
        suspensionView?.layoutParams = suspensionView?.layoutParams
        Log.d(tag,"setNeedSuspensionView width: ${needSuspensionChild.width!!} height: ${needSuspensionChild.height} view: $needSuspensionChild position: $needSuspensionChildPosition")
    }

    private var currentState = AbsListView.OnScrollListener.SCROLL_STATE_IDLE

    private val myScrollerListener = object : AbsListView.OnScrollListener {
        override fun onScrollStateChanged(view: AbsListView, scrollState: Int) {
            scrollerListeners?.forEach {
                it.onScrollStateChanged(view,scrollState)
            }
            Log.d(tag,"onScrollStateChanged  scrollState: $scrollState")
            if (suspensionView == null) {
                return
            }
            currentState = scrollState
            when (scrollState) {
                // 停止滑动
                AbsListView.OnScrollListener.SCROLL_STATE_IDLE -> {
                    // 对正在显示的浮层的child做个副本，为了判断显示浮层的child是否发现了变化
                    val tempFloatChild = needSuspensionChild
                    updateFloatScrollStopTranslateY()
                    // 如果firstChild没有发生变化，回调floatView滑动停止的监听
                    if (tempFloatChild === needSuspensionChild) {

                    }
                }
                // 开始滑动
                AbsListView.OnScrollListener.SCROLL_STATE_TOUCH_SCROLL ->{
                    updateFloatScrollStopTranslateY()
                }
                AbsListView.OnScrollListener.SCROLL_STATE_FLING ->{

                }
            }
        }

        override fun onScroll(view: AbsListView, firstVisibleItem: Int, visibleItemCount: Int, totalItemCount: Int) {
            scrollerListeners?.forEach {
                it.onScroll(view,firstVisibleItem,visibleItemCount,totalItemCount)
            }
            // 判断显示浮层的child是否已经划出屏幕
            needSuspensionChildVisible = true
            if (needSuspensionChild != null) {
                if(needSuspensionChildPosition < firstVisibleItem || needSuspensionChildPosition > firstVisibleItem+visibleItemCount){
                    needSuspensionChildVisible = false
                    needSuspensionChildOverRange()
                }else{
                    needSuspensionChildInRange()
                }
            }
//            Log.d(tag,"needSuspensionChildPosition $needSuspensionChildPosition  needSuspensionChildVisible：$needSuspensionChildVisible  firstVisibleItem:$firstVisibleItem  visibleItemCount；$visibleItemCount  totalItemCount: $totalItemCount")
            if(needSuspensionChildVisible){
                when (currentState) {
                    // 停止滑动
                    AbsListView.OnScrollListener.SCROLL_STATE_IDLE -> updateFloatScrollStopTranslateY()
                    // 开始滑动
                    AbsListView.OnScrollListener.SCROLL_STATE_TOUCH_SCROLL -> {
                        updateFloatScrollStopTranslateY()
                        getChildAdapterPosition(needSuspensionChild)
                    }
                    // Fling
                    AbsListView.OnScrollListener.SCROLL_STATE_FLING -> {
                        updateFloatScrollStopTranslateY()
                    }
                }
            }
        }
    }

    /**
     * 获取child在列表中的位置
     */
    private fun getChildAdapterPosition(item: View?): Int {
        val firstVisiblePosition = _listView?.firstVisiblePosition!!
        val childCount = _listView?.childCount!!
        for (i in 0 until childCount) {
            val child = _listView?.getChildAt(i)
            // 判断是否是当前的item
            if (child === item) {
                Log.d(tag,"getChildAdapterPosition  i ::: $i")
                return (firstVisiblePosition + i)
            }
        }
        return -1
    }

    /**
     * 更新位置
     * */
    private fun updateFloatScrollStopTranslateY() {
        if (needSuspensionChild == null) {
            return
        }
        if(needSuspensionChildVisible){
            suspensionView?.translationY = needSuspensionChild?.top!!.toFloat()
        }
    }

    /**
     * 超出显示范围
     * */
    fun needSuspensionChildOverRange(){
//        suspensionView?.visibility = View.GONE
    }

    /**
     * 显示范围内
     * */
    fun needSuspensionChildInRange(){
        suspensionView?.visibility = View.VISIBLE
    }

    /**
     * @param listView 需要显示悬浮View的ListView
     */
    @RequiresApi(Build.VERSION_CODES.M)
    fun setListView(listView: ListView){
        _listView = listView
        _listView?.setOnScrollListener(myScrollerListener)
    }

    fun addListViewOnScrollListener(l: AbsListView.OnScrollListener ){
        if(scrollerListeners == null){
            scrollerListeners =  arrayListOf()
        }
        scrollerListeners?.add(l)
    }

    fun removeListViewOnScrollListener(l: AbsListView.OnScrollListener ){
        if(scrollerListeners == null || !scrollerListeners?.remove(l)!!){
            Log.d(tag,"removeListViewOnScrollListener failed")
        }
    }
}