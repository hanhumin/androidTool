package com.example.txl.tool.recyclerView

import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.Rect
import android.graphics.drawable.ColorDrawable
import android.graphics.drawable.Drawable
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.txl.tool.R
import kotlinx.android.synthetic.main.activity_item_decoration_demo.*


class ItemDecorationDemoActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_item_decoration_demo)
        recycler_view.layoutManager = LinearLayoutManager(this,LinearLayoutManager.VERTICAL,false)
        val adapter = ItemDecorAdapter()
        adapter.data.add("提交订单")
        adapter.data.add("已支付")
        adapter.data.add("商品出库")
        adapter.data.add("签收")
        recycler_view.adapter = adapter
        recycler_view.addItemDecoration(TestItemDecoration())
    }
}

class ItemDecorAdapter : RecyclerView.Adapter<TextViewHolder>() {
    val data = ArrayList<String>()
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): TextViewHolder {
        return TextViewHolder(LayoutInflater.from(parent.context).inflate(R.layout.item_decoration_text,parent,false))
    }

    override fun getItemCount(): Int {
        return data.size
    }

    override fun onBindViewHolder(holder: TextViewHolder, position: Int) {
        holder.tv.text = data[position]
    }
}

class TextViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
    val tv:TextView = itemView as TextView
}

class TestItemDecoration : RecyclerView.ItemDecoration() {
    private var mDrawable: Drawable

    //轴线距离左侧的距离，单位pix。
    private val marginLeft = 20

    //轴线的宽度，单位pix。
    private val lineSize = 10

    private var mPaint:Paint = Paint()

    init {
        mPaint.isAntiAlias = true
        mPaint.color = Color.GREEN

        mDrawable = ColorDrawable(Color.GRAY)
    }



    override fun onDraw(canvas: Canvas, parent: RecyclerView, state: RecyclerView.State) {
        if(parent.adapter == null){
            return
        }
        canvas.save()
        val childCount = parent.childCount
        for (i in 0 until childCount) {
            val child = parent.getChildAt(i)

            val pos = parent.getChildAdapterPosition(child)
            if(pos <= 2){//这里模拟小状态变化，小于等于2的看做已完成状态
                mDrawable = ColorDrawable(Color.GREEN)
                mPaint.color = Color.GREEN
            }else{
                mDrawable = ColorDrawable(Color.GRAY)
                mPaint.color = Color.GRAY
            }
            mDrawable.setBounds(marginLeft, child.top, marginLeft + lineSize, child.bottom)
            mDrawable.draw(canvas)

            val radius = ((lineSize + marginLeft) / 2f)
            canvas.drawCircle((marginLeft + lineSize / 2f), (child.top + radius), radius, mPaint)
            if(i == parent.adapter!!.itemCount-1){
                canvas.drawCircle((marginLeft + lineSize / 2f), (child.bottom - radius), radius, mPaint)
            }
        }

        canvas.restore()
    }

    override fun getItemOffsets(outRect: Rect, view: View, parent: RecyclerView, state: RecyclerView.State) {
        outRect.set((marginLeft + lineSize) * 2, 0, 0, 0)
    }
}
