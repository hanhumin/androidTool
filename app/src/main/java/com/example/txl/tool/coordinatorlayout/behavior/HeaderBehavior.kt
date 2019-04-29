package com.example.txl.tool.coordinatorlayout.behavior

import android.content.Context
import android.support.design.widget.CoordinatorLayout
import android.support.v4.view.ViewCompat
import android.util.AttributeSet
import android.util.Log
import android.view.View
import com.example.txl.tool.utils.DisplayUtil

/**
 * 当发起嵌套滑动的时候处理
 * */
class HeaderBehavior : CoordinatorLayout.Behavior<View> {

    val TAG = HeaderBehavior::class.java.simpleName

    var mMaxOffsetY = 0
    var mLeftPadding = 0
    var mRightPadding = 0

    /**
     * y方向上的平移
     * */
    var mTranslationY = 0f

    private var isInit = false

    internal fun init(context: Context) {
        mMaxOffsetY = DisplayUtil.dip2px(context, 55f)
        mLeftPadding = DisplayUtil.dip2px(context, 45f)
        mRightPadding = DisplayUtil.dip2px(context, 45f)
    }

    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)

    constructor() : super()

    override fun onStartNestedScroll(coordinatorLayout: CoordinatorLayout, child: View, directTargetChild: View, target: View, axes: Int, type: Int): Boolean {
        if (!isInit) {
            init(child.context)
            isInit = true
        }
        if (axes and ViewCompat.SCROLL_AXIS_VERTICAL != 0) {
            return true
        }
        return false
    }


    override fun onNestedPreScroll(coordinatorLayout: CoordinatorLayout, child: View, target: View, dx: Int, dy: Int, consumed: IntArray, type: Int) {
        mTranslationY = child.translationY
        if (dy > 0) {//向下
            Log.d(TAG, "onNestedPreScroll 向下")

        } else {//向上
            Log.d(TAG, "onNestedPreScroll 向上")
            if (Math.abs(mTranslationY + dy) < mMaxOffsetY) {
                consumed[1] = dy
            } else if (Math.abs(mTranslationY) < mMaxOffsetY && Math.abs(mTranslationY + dy) > mMaxOffsetY) {
                consumed[1] = (mMaxOffsetY - Math.abs(mTranslationY)).toInt()
            }
            child.translationY = mTranslationY - consumed[1]
        }
    }

    override fun onNestedPreFling(coordinatorLayout: CoordinatorLayout, child: View, target: View, velocityX: Float, velocityY: Float): Boolean {
        var handle = false
        if (velocityY > 0) {//向下
            Log.d(TAG, "onNestedPreFling 向下")
        } else {//向上
            Log.d(TAG, "onNestedPreFling 向上")
        }
        return handle
    }


    override fun onNestedScroll(coordinatorLayout: CoordinatorLayout, child: View, target: View, dxConsumed: Int, dyConsumed: Int, dxUnconsumed: Int, dyUnconsumed: Int, type: Int) {
        super.onNestedScroll(coordinatorLayout, child, target, dxConsumed, dyConsumed, dxUnconsumed, dyUnconsumed, type)
    }

    override fun onNestedFling(coordinatorLayout: CoordinatorLayout, child: View, target: View, velocityX: Float, velocityY: Float, consumed: Boolean): Boolean {
        return super.onNestedFling(coordinatorLayout, child, target, velocityX, velocityY, consumed)
    }
}