package com.example.txl.tool.coordinatorlayout.behavior

import android.content.Context
import android.support.design.widget.CoordinatorLayout
import android.util.AttributeSet
import android.view.View
import com.example.txl.tool.R

/**
 * 依赖于header变化独自生进行处理
 * */
class HeaderDependsOnBehavior : CoordinatorLayout.Behavior<View> {
    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)

    constructor() : super()

    override fun layoutDependsOn(parent: CoordinatorLayout?, child: View?, dependency: View?): Boolean {
        return isDependsOn(dependency)
    }

    private fun isDependsOn(dependency: View?): Boolean {
        return dependency?.id == R.id.frame_header
    }

    override fun onDependentViewChanged(parent: CoordinatorLayout?, child: View?, dependency: View?): Boolean {
        child?.translationY = dependency!!.translationY + dependency.height
        return super.onDependentViewChanged(parent, child, dependency)
    }

    override fun onDependentViewRemoved(parent: CoordinatorLayout?, child: View?, dependency: View?) {
        child?.translationY = 0f
    }
}