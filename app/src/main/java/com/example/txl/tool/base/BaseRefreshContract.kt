package com.sobey.appfactory.fragment.base

/**
 *@author TXL
 *description :
 */
interface BaseRefreshContract {
    interface BaseRefreshView<D>:BaseView<BaseRefreshPresenter>{
        fun onRefreshSuccess(data:D)
        fun onRefreshFailed(data:Any?)
        fun onLoadMoewSuccess(data:D)
        fun onLoadMoewFailed(data:Any?)
    }

    interface BaseRefreshPresenter:BasePresenter{
        fun refresh()
        fun loadMore()
    }
}