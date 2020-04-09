package com.example.txl.tool.recyclerView

import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.Path
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

class FlyActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_fly)
        recycler_view.layoutManager = LinearLayoutManager(this, LinearLayoutManager.VERTICAL,false)
        val adapter = FlyAdapter()
        recycler_view.adapter = adapter
        recycler_view.addItemDecoration(TestItemDecoration())
        recycler_view.addItemDecoration(FlyItemDecoration())
    }
}

class FlyItemDecoration: RecyclerView.ItemDecoration() {
    var allPath:Path = Path()
    var itemPath:Path = Path()
    val mPaint = Paint(Paint.ANTI_ALIAS_FLAG)

    /**
     * 将宽度拆分成多少分
     * */
    val total = 5f

    init {
        mPaint.color = 0x77edcbaa
        mPaint.style = Paint.Style.STROKE
        mPaint.strokeWidth = 50f
        mPaint.strokeJoin = Paint.Join.ROUND
    }

    override fun onDrawOver(canvas: Canvas, parent: RecyclerView, state: RecyclerView.State) {
        if(parent.adapter == null){
            return
        }
        canvas.save()
        val childCount = parent.childCount
        for (i in 0 until childCount) {
            val child = parent.getChildAt(i)

            val pos = parent.getChildAdapterPosition(child)
            itemPath.reset()
            if(pos % 2 == 0){//左侧管
                itemPath.moveTo(child.width/total,child.top.toFloat())
                itemPath.lineTo(child.width/total,child.top.toFloat()+child.height*4/total)
                itemPath.lineTo(child.width*4/total,child.top.toFloat()+child.height*4/total)
                itemPath.lineTo(child.width*4/total,child.top.toFloat()+child.height)
            }else{
                itemPath.moveTo(child.width*4/total,child.top.toFloat())
                itemPath.lineTo(child.width*4/total,child.top.toFloat()+child.height*4/total)
                itemPath.lineTo(child.width/total,child.top.toFloat()+child.height*4/total)
                itemPath.lineTo(child.width/total,child.top.toFloat()+child.height)
            }
            canvas.drawPath(itemPath,mPaint)
        }
        canvas.restore()
    }
}

class FlyAdapter: RecyclerView.Adapter<FlyViewHolder>() {
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): FlyViewHolder {
        return FlyViewHolder(LayoutInflater.from(parent.context).inflate(R.layout.item_decoration_text,parent,false))
    }

    override fun getItemCount(): Int {
        return Int.MAX_VALUE
    }

    override fun onBindViewHolder(holder: FlyViewHolder, position: Int) {
        holder.tv.text = "我是第 $position 个"
        if(position %2 == 0){
            holder.tv.setBackgroundColor(Color.parseColor("#78945612"))
        }else{
            holder.tv.setBackgroundColor(Color.parseColor("#78123456"))
        }
    }
}

class FlyViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
    val tv: TextView = itemView as TextView

    init {
        val params= tv.layoutParams
        params.height = 360
        tv.layoutParams = params
    }
}