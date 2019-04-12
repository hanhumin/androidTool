package com.example.txl.tool.utils.floatview

import android.annotation.TargetApi
import android.os.Build
import android.support.annotation.RequiresApi
import android.util.Log
import android.view.Gravity
import android.view.View
import android.widget.AbsListView
import android.widget.FrameLayout
import android.widget.ListView
import com.example.txl.tool.R

/**
 *@author TXL
 *description :为ListView添加悬浮效果
 */

/**
 * 当需要显示悬浮效果的item滑出界面可视范围的时候悬浮view隐藏
 * */
const val OVER_MODEL_GONE = 0
/**
 * 当需要显示悬浮效果的item滑出界面可视范围的时候悬浮view显示在底部
 * */
const val OVER_MODEL_SHOW_BOTTOM = 1

/**
 * 悬浮view处于不可见状态
 * */
const val SUSPENSION_VIEW_STATE_GONE = 0
/**
 * 悬浮view处于可见状态跟随listView item滑动
 * */
const val SUSPENSION_VIEW_STATE_FLOAT = 1

/**
 * 悬浮view处于它的父容器底部
 * */
const val SUSPENSION_VIEW_STATE_BOTTOM = 2

class SuspensionListViewUtils {
    val tag: String = javaClass.simpleName

    /**
     * 悬浮的view停止滑动时的状态
     * OVER_MODEL_GONE 不可见
     * OVER_MODEL_SHOW_BOTTOM 显示在底部
     * */
    var mOverModel = OVER_MODEL_GONE

    var mSuspensionViewState = SUSPENSION_VIEW_STATE_GONE

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

    /**
     *用这个来作为存储的key，来取每个item对应的position
     * */
    private var mKeyPositionTagKey = Int.MAX_VALUE

    constructor(mKeyPositionTagKey: Int) {
        this.mKeyPositionTagKey = mKeyPositionTagKey
    }


    @TargetApi(Build.VERSION_CODES.M)
    fun setNeedSuspensionView(needSuspensionChild: View, needSuspensionChildPosition: Int) {
        this.needSuspensionChild = needSuspensionChild
        this.needSuspensionChildPosition = needSuspensionChildPosition
        needSuspensionChild.setOnScrollChangeListener(object : View.OnScrollChangeListener{
            override fun onScrollChange(v: View?, scrollX: Int, scrollY: Int, oldScrollX: Int, oldScrollY: Int) {
                Log.d(tag,"setNeedSuspensionView onScrollChange ")
            }
        })
        setSuspensionViewFloatingLayoutParams(needSuspensionChild)
        suspensionView?.translationY = needSuspensionChild.top.toFloat()
        mSuspensionViewState = SUSPENSION_VIEW_STATE_FLOAT
        Log.d(tag,"setNeedSuspensionView width: ${needSuspensionChild.width!!} height: ${needSuspensionChild.height} view: $needSuspensionChild position: $needSuspensionChildPosition")
    }

    private fun setSuspensionViewFloatingLayoutParams(needSuspensionChild: View?) {
        if(needSuspensionChild != null){
            suspensionView?.visibility = View.VISIBLE
            val layoutParams = suspensionView?.layoutParams as FrameLayout.LayoutParams
            layoutParams.width = needSuspensionChild.width!!
            layoutParams.height = needSuspensionChild.height!!
            layoutParams.gravity = Gravity.NO_GRAVITY
            suspensionView?.layoutParams = layoutParams
        }
    }

    private fun setSuspensionViewBottomLayoutParams(needSuspensionChild: View?) {
        if(needSuspensionChild != null){
            suspensionView?.visibility = View.VISIBLE
            //显示底部的时候设置移动位置为0
            suspensionView?.translationY = 0f
            val layoutParams = suspensionView?.layoutParams as FrameLayout.LayoutParams
            layoutParams.width = 400
            layoutParams.height = 300
            val gravity = Gravity.BOTTOM or Gravity.END
            layoutParams.gravity = (gravity)
            suspensionView?.layoutParams = layoutParams
        }
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
//                    updateFloatScrollStopTranslateY()
                }
                // 开始滑动
                AbsListView.OnScrollListener.SCROLL_STATE_TOUCH_SCROLL -> {
//                    updateFloatScrollStopTranslateY()
                }
                //滑翔
                AbsListView.OnScrollListener.SCROLL_STATE_FLING -> {

                }
            }
        }

        override fun onScroll(view: AbsListView, firstVisibleItem: Int, visibleItemCount: Int, totalItemCount: Int) {
            scrollerListeners?.forEach {
                it.onScroll(view,firstVisibleItem,visibleItemCount,totalItemCount)
            }
            // 判断显示浮层的child是否已经划出屏幕
            Log.d(tag,"needSuspensionChildPosition: $needSuspensionChildPosition  firstVisibleItem: $firstVisibleItem  visibleItemCount: $visibleItemCount mSuspensionViewState::$mSuspensionViewState")
            if (needSuspensionChild != null) {
                if(needSuspensionChildPosition < firstVisibleItem || needSuspensionChildPosition >= firstVisibleItem+visibleItemCount){
                    needSuspensionChildOverRange()
                }else{
                    if(mOverModel == OVER_MODEL_SHOW_BOTTOM){//底部显示悬浮窗，在可视范围内需要重新显示
                        needSuspensionChildInRange()
                    }
                    updateFloatScrollStopTranslateY()
                }
            }
        }
    }

    /**
     * 更新位置
     * */
    private fun updateFloatScrollStopTranslateY() {
        if(needSuspensionChild?.getTag(mKeyPositionTagKey)!=needSuspensionChildPosition){
            getChildAdapterPosition(needSuspensionChildPosition)
        }
        if(mSuspensionViewState == SUSPENSION_VIEW_STATE_FLOAT && needSuspensionChild!= null){
            suspensionView?.translationY = needSuspensionChild?.top!!.toFloat()
        }
    }

    /**
     * 获取child在列表中的位置
     */
    private fun getChildAdapterPosition(position: Int): View? {
        needSuspensionChild = null
        val firstVisiblePosition = _listView?.firstVisiblePosition!!
        val lastVisiblePosition = _listView?.lastVisiblePosition!!
        if(position < firstVisiblePosition || position>lastVisiblePosition){
            return null
        }
        val childCount = _listView?.childCount!!
        for (i in 0 until childCount) {
            val child = _listView?.getChildAt(i)
            // 判断是否是当前的item
            if (child != null && child.getTag(mKeyPositionTagKey) == position) {
                needSuspensionChild = child
                return child
            }
        }
        return null
    }

    /**
     * 超出显示范围
     * */
    fun needSuspensionChildOverRange(){
        when (mOverModel) {
            OVER_MODEL_GONE -> {
                setSuspensionViewState(SUSPENSION_VIEW_STATE_GONE)
            }
            OVER_MODEL_SHOW_BOTTOM -> {
                setSuspensionViewState(SUSPENSION_VIEW_STATE_BOTTOM)
            }
        }
    }

    /**
     * 显示范围内
     * */
    fun needSuspensionChildInRange(){
        setSuspensionViewState(SUSPENSION_VIEW_STATE_FLOAT)
    }

    /**
     * 设置悬浮view的状态
     * @param targetState
     * */
    private fun setSuspensionViewState(targetState:Int) {
        if(mSuspensionViewState == targetState){
            return
        }
        when (targetState) {
            SUSPENSION_VIEW_STATE_GONE->{
                setSuspensionViewGone()
                mSuspensionViewState = SUSPENSION_VIEW_STATE_GONE
            }
            SUSPENSION_VIEW_STATE_BOTTOM -> {
                setSuspensionViewBottomLayoutParams(needSuspensionChild)
                mSuspensionViewState = SUSPENSION_VIEW_STATE_BOTTOM
            }
            SUSPENSION_VIEW_STATE_FLOAT -> {
                setSuspensionViewFloatingLayoutParams(needSuspensionChild)
                mSuspensionViewState = SUSPENSION_VIEW_STATE_FLOAT
            }
        }
    }

    private fun setSuspensionViewGone() {
        suspensionView?.visibility = View.GONE
    }

    /**
     * @param listView 需要显示悬浮View的ListView
     */
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