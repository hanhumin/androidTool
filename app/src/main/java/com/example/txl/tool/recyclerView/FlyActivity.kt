package com.example.txl.tool.recyclerView

import android.content.Context
import android.graphics.*
import android.graphics.drawable.BitmapDrawable
import android.os.Bundle
import android.util.Log
import android.util.SparseArray
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.txl.tool.R
import kotlinx.android.synthetic.main.activity_item_decoration_demo.*
import java.lang.RuntimeException
import kotlin.math.max

class FlyActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_fly)
        recycler_view.layoutManager = LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false)
        val adapter = FlyAdapter()
        recycler_view.adapter = adapter
//        recycler_view.addItemDecoration(TestItemDecoration())
        recycler_view.addItemDecoration(FlyItemDecoration(this, recycler_view))
    }
}

class FlyItemDecoration(private val context: Context, private val recyclerView: RecyclerView) : RecyclerView.ItemDecoration() {

    companion object {
        const val TAG = "FlyItemDecoration"
    }

    private var allPath: Path = Path()
    private var itemPath: Path = Path()
    private val mPaint = Paint(Paint.ANTI_ALIAS_FLAG)
    private val flyDrawable: BitmapDrawable = ContextCompat.getDrawable(context, R.drawable.fly) as BitmapDrawable
    private val viewInfoSpareArray: SparseArray<ViewInfo> = SparseArray()

    /**
     * 滑动方向
     * */
    private var direct = 0
    private var totalOffsetY = 0
    private val scrollListener = object : RecyclerView.OnScrollListener() {
        override fun onScrolled(recyclerView: RecyclerView, dx: Int, dy: Int) {
            super.onScrolled(recyclerView, dx, dy)
            direct = dy
            totalOffsetY += dy
        }
    }

    /**
     * 将宽度拆分成多少分
     * */
    private val total = 5f
    private val strokeWidth = 80f

    private var maxPosition = 0

    init {
        recyclerView.addOnScrollListener(scrollListener)
        mPaint.color = 0x77edcbaa
        mPaint.style = Paint.Style.STROKE
        mPaint.strokeWidth = strokeWidth
        mPaint.strokeJoin = Paint.Join.ROUND
    }

    override fun onDrawOver(canvas: Canvas, parent: RecyclerView, state: RecyclerView.State) {
        if (parent.adapter == null) {
            return
        }
        canvas.save()
        val childCount = parent.childCount
        for (i in 0 until childCount) {
            val child = parent.getChildAt(i)
            val pos = parent.getChildAdapterPosition(child)
            saveViewInfo(pos, child)
            maxPosition = max(maxPosition, pos)
            itemPath.reset()
            if (pos % 2 == 0) {//左侧管
                itemPath.moveTo(child.width / total, child.top.toFloat())
                itemPath.lineTo(child.width / total, child.top.toFloat() + child.height * (total - 1) / total)
                itemPath.lineTo(child.width * (total - 1) / total, child.top.toFloat() + child.height * (total - 1) / total)
                itemPath.lineTo(child.width * (total - 1) / total, child.top.toFloat() + child.height)
            } else {
                itemPath.moveTo(child.width * (total - 1) / total, child.top.toFloat())
                itemPath.lineTo(child.width * (total - 1) / total, child.top.toFloat() + child.height * (total - 1) / total)
                itemPath.lineTo(child.width / total, child.top.toFloat() + child.height * (total - 1) / total)
                itemPath.lineTo(child.width / total, child.top.toFloat() + child.height)
            }
            canvas.drawPath(itemPath, mPaint)
        }
        canvas.restore()
        drawFly(canvas, parent)
    }

    private fun saveViewInfo(pos: Int, child: View) {
        val viewInfo = ViewInfo()
        viewInfo.position = pos

        if (pos == 0) {
            viewInfo.top = 0
        } else {
            viewInfo.top = viewInfoSpareArray[pos - 1].bottom

        }
        viewInfo.bottom = viewInfo.top + child.height.toLong()
//        Log.e(TAG,"saveViewInfo  $viewInfo")
        viewInfoSpareArray.put(pos, viewInfo)
    }

    /**
     * 画飞机
     * */
    private fun drawFly(canvas: Canvas, parent: RecyclerView) {
        canvas.save()
        val attachViewInfo = getAttachViewInfo(parent)
        val attachView = parent.findViewHolderForAdapterPosition(attachViewInfo.position)?.itemView
                ?: return
        //小飞机对当前View的总滑动距离  = 水平距离 + 垂直距离
        val totalDistance = (total - 2) / total * attachView.width + attachView.height
        //计算当前小飞机相对当前VIew的滑动完成度,
        //滑动单位像素小飞机移动的距离应该是  ；； 当前View的高度 / 移动总距离
        val unitPixelDistance = totalDistance / attachView.height
        leftFirstPoint = attachView.height * (total - 1) / total / totalDistance
        leftSecondPoint = leftFirstPoint + (attachView.width.toFloat() * (total - 2) / total / totalDistance)
        val dy = totalOffsetY - attachViewInfo.top.toFloat()
        Log.e(TAG,"dy is $dy  top is : ${attachViewInfo.top}  offset = $totalOffsetY")
        val progress: Float = unitPixelDistance * dy / totalDistance
        if (progress > 1 || progress < 0) {//这个完成度最多是1，如果大于1说明代码 逻辑有问题
            throw RuntimeException("progress is $progress please think more time")
        }
        val pos = parent.getChildAdapterPosition(attachView)
        //这个是否是左侧管道
        val isLeft = pos % 2 == 0
        val offsetX = getOffsetX(isLeft, progress, attachView)
        val offsetY = 0
        val left = (parent.width / total - strokeWidth / 2).toInt() + offsetX
        val top = totalOffsetY + offsetY
        Log.e("FlyItemDecoration", "draw fly attachViewInfo.top ${attachViewInfo.top} unitPixelDistance  :: $unitPixelDistance left :: $left  top :: $top  leftFirstPoint :: $leftFirstPoint  leftSecondPoint :: $leftSecondPoint  progress :: $progress  totalDistance :: $totalDistance")
        val right = (left + strokeWidth).toInt()
        val bottom = top + strokeWidth.toInt()
        val desRect = Rect(left, top, right, bottom)
        val cx = left + strokeWidth / 2f
        val cy = top + strokeWidth / 2f
        var degrees = 180f
        canvas.rotate(degrees, cx, cy)
        canvas.drawBitmap(flyDrawable.bitmap, null, desRect, mPaint)
        canvas.rotate(-degrees)
        canvas.restore()
    }

    /**
     * 左管道第一个拐点
     * */
    private var leftFirstPoint = 0f;
    private var leftSecondPoint = 0f;

    private fun getOffsetX(left: Boolean, progress: Float, attachView: View): Int {
        if (left) {
            if (progress < leftFirstPoint) {
                return 0
            } else if (progress > leftSecondPoint) {
                return (attachView.width * (total - 2) / total).toInt()
            }
        } else {
            return 0
        }
        return 0
    }

    private fun getOffsetY(left: Boolean,progress: Float,attachView: View):Int{
        if (left) {
            if (progress < leftFirstPoint) {
                return 0
            } else if (progress > leftSecondPoint) {
                return (attachView.height * (total - 1) / total).toInt()
            }
        } else {
            return 0
        }
        return 0
    }

    /**
     * 查找小飞机应该在哪个View上面
     * */
    private fun getAttachView(recyclerView: RecyclerView): View? {
        val attachViewInfo = getAttachViewInfo(recyclerView)
        return recyclerView.findViewHolderForAdapterPosition(attachViewInfo.position)?.itemView
    }

    private fun getAttachViewInfo(recyclerView: RecyclerView): ViewInfo {
        if (recyclerView.adapter == null) {
            throw RuntimeException("adapter is null")
        }
        var start = 0
        var end = maxPosition
        var half = (end - start) / 2
        var attachViewInfo = viewInfoSpareArray.get(half)

        while (!isQualifiedCandidateView(attachViewInfo)) {
            if (start == end) {
                Log.e("FlyItemDecoration", "start == end  is $start")
                break
            }
            if (half < 0) {
                Log.e("FlyItemDecoration", "why half < 0  half is $half")
                break
            }
            if (attachViewInfo.top >= totalOffsetY) {//当前的View在滑动距离下面
                end = half
                half = (end - start) / 2 + start
                attachViewInfo = viewInfoSpareArray.get(half)
            } else {
                start = half
                half = (end - start) / 2 + start
                attachViewInfo = viewInfoSpareArray.get(half)
            }
        }
        return attachViewInfo
    }

    private fun isQualifiedCandidateView(viewInfo: ViewInfo): Boolean {
        return viewInfo.top <= totalOffsetY && viewInfo.bottom >= totalOffsetY
    }

    class ViewInfo {
        /**
         * 记录adapter所在位置
         * */
        var position = 0
        /**
         * 相对第一个View总偏移的
         * */
        var top = 0L
        var bottom = 0L

        override fun toString(): String {
            return "ViewInfo(position=$position, top=$top, bottom=$bottom)"
        }
    }
}

class FlyAdapter : RecyclerView.Adapter<FlyViewHolder>() {
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): FlyViewHolder {
        return FlyViewHolder(LayoutInflater.from(parent.context).inflate(R.layout.item_decoration_text, parent, false))
    }

    override fun getItemCount(): Int {
        return 200
    }

    override fun onBindViewHolder(holder: FlyViewHolder, position: Int) {
        holder.tv.text = "我是第 $position 个"
        if (position % 2 == 0) {
            holder.tv.setBackgroundColor(Color.parseColor("#78945612"))
        } else {
            holder.tv.setBackgroundColor(Color.parseColor("#78123456"))
        }
    }
}

class FlyViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
    val tv: TextView = itemView as TextView

    init {
        val params = tv.layoutParams
        params.height = 480
        tv.layoutParams = params
    }
}