package com.txl.player.utils

import android.content.Context
import android.graphics.Canvas
import android.util.AttributeSet
import android.widget.SeekBar

import android.graphics.Color
import android.graphics.Paint
import com.huaqiyun.player.R

class PointSeekBar(context: Context?, attrs: AttributeSet) : SeekBar(context, attrs) {


    private val pointPaint = Paint()
    private var historyPresent = 0f
    var onPositionListener: OnPositionListener? = null

    init {
        pointPaint.color = Color.parseColor("#FF2974")
        pointPaint.style = Paint.Style.FILL
        pointPaint.strokeWidth = 6.0.toFloat()

    }


    fun setPointColor(color: Int) {
        pointPaint.color = color
    }

    override fun onDraw(canvas: Canvas?) {
        super.onDraw(canvas)
        if (historyPresent > 0) {
            val x = (measuredWidth - paddingStart - paddingEnd) * historyPresent + paddingStart
            val y = measuredHeight / 2
            canvas?.drawLine(
                x,
                (y - resources.getDimension(R.dimen.dp_5)),
                x,
                (y + resources.getDimension(R.dimen.dp_5)),
                pointPaint
            )
            onPositionListener?.onX(x)
        }else{
            onPositionListener = null
        }
    }

    fun setHistoryPoint(present: Float) {
        historyPresent = present
        postInvalidate()
    }

    fun clearHistoryPoint() {
        historyPresent = 0f
        postInvalidate()
    }
    interface OnPositionListener {
        fun onX(x: Float)
    }
}