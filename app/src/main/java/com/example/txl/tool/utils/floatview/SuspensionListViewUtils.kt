package com.example.txl.tool.utils.floatview

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
     * */
    private var needFloatChild:View? = null

    private var currentState = AbsListView.OnScrollListener.SCROLL_STATE_IDLE

    private val myScrollerListener = object : AbsListView.OnScrollListener {
        override fun onScrollStateChanged(view: AbsListView, scrollState: Int) {
            scrollerListeners?.forEach {
                it.onScrollStateChanged(view,scrollState)
            }
            if (suspensionView == null) {
                return
            }
            currentState = scrollState
            when (scrollState) {
                // 停止滑动
                AbsListView.OnScrollListener.SCROLL_STATE_IDLE -> {
                    // 对正在显示的浮层的child做个副本，为了判断显示浮层的child是否发现了变化
                    val tempFloatChild = needFloatChild
                    // 更新浮层的位置，覆盖child
                    updateFloatScrollStopTranslateY()
                    // 如果firstChild没有发生变化，回调floatView滑动停止的监听
                    if (tempFloatChild === needFloatChild) {

                    }
                }
                // 开始滑动
                AbsListView.OnScrollListener.SCROLL_STATE_TOUCH_SCROLL ->{

                }
                    // 保存第一个child
                    // 更新浮层的位置

            }// Fling
            // 这里有一个bug，如果手指在屏幕上快速滑动，但是手指并未离开，仍然有可能触发Fling
            // 所以这里不对Fling状态进行处理
            //                    case 2:
            //                        hideFloatView();
            //                        break;
        }

        override fun onScroll(view: AbsListView, firstVisibleItem: Int, visibleItemCount: Int, totalItemCount: Int) {
            scrollerListeners?.forEach {
                it.onScroll(view,firstVisibleItem,visibleItemCount,totalItemCount)
            }
            if (suspensionView == null) {
                return
            }
            // 判断显示浮层的child是否已经划出屏幕
            if (needFloatChild != null) {

            }

            when (currentState) {
                // 停止滑动
                AbsListView.OnScrollListener.SCROLL_STATE_IDLE -> updateFloatScrollStopTranslateY()
                // 开始滑动
                AbsListView.OnScrollListener.SCROLL_STATE_TOUCH_SCROLL -> {}
                // Fling
                AbsListView.OnScrollListener.SCROLL_STATE_FLING -> {

                }
            }
        }
    }

    /**
     * 更新位置
     * */
    private fun updateFloatScrollStopTranslateY() {
        if (needFloatChild == null) {
            return
        }

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