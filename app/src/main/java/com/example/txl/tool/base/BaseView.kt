package com.sobey.appfactory.fragment.base

/**
 *@author TXL
 *description :
 */
interface BaseView<T :BasePresenter> {
    fun setPresenter(presenter:T)
}