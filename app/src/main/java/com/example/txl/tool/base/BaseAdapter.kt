package com.example.txl.tool.base

import android.content.Context
import android.support.v7.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup

/**
 *@author TXL
 *description :
 */
abstract class BaseAdapter<T>(context: Context, list: ArrayList<T>?) : RecyclerView.Adapter<BaseAdapter.BaseViewHolder<T>>() {

    var itemClickListener: OnItemClickListener<T>? = null

    var listData: ArrayList<T>? = null
        set(value) {
            field = value
            notifyDataSetChanged()
        }
    protected var layoutInflater: LayoutInflater = LayoutInflater.from(context)

    constructor(context: Context) : this(context, null)

    init {
        this.listData = list
    }

    fun addListData(list: ArrayList<T>) {
        if (this.listData == null) {
            listData = list
        } else {
            listData?.addAll(list)
        }
        notifyDataSetChanged()
    }

    override fun getItemCount(): Int {
        return if (listData != null) {
            listData?.size!!
        } else {
            0
        }
    }

    override fun onBindViewHolder(holder: BaseViewHolder<T>, position: Int) {
        holder.onBindViewHolder(position, listData!![position], this)
    }

    abstract class BaseViewHolder<D>(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var mData: D? = null
        var mPosition = -1
        var mAdapter: BaseAdapter<D>? = null

        init {
            itemView.setOnClickListener(object : View.OnClickListener {
                override fun onClick(v: View?) {
                    if (mPosition != -1 && mAdapter != null && mAdapter?.itemClickListener != null) {
                        mAdapter?.itemClickListener?.onItemClick(itemView,mPosition,mData,mAdapter!!)
                    }
                }
            })
        }

        open fun onBindViewHolder(position: Int, data: D, adapter: BaseAdapter<D>) {
            mData = data
            mAdapter = adapter
            mPosition = position
        }
    }


    interface OnItemClickListener<D> {
        fun onItemClick(clickView: View, position: Int, data: D?, adapter: BaseAdapter<D>)
    }
}