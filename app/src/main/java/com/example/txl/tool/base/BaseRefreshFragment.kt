

import android.app.Fragment
import android.os.Bundle
import androidx.annotation.IdRes
import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.View.NO_ID
import android.view.ViewGroup
import com.example.txl.tool.R
import com.example.txl.tool.base.BaseAdapter
import com.scwang.smartrefresh.layout.SmartRefreshLayout
import com.scwang.smartrefresh.layout.listener.OnLoadMoreListener
import com.scwang.smartrefresh.layout.listener.OnRefreshListener
import com.sobey.appfactory.fragment.base.BaseRefreshContract

/**
 *@author TXL
 *description :
 */
abstract class BaseRefreshFragment<P: BaseRefreshContract.BaseRefreshPresenter,D>: Fragment(), BaseRefreshContract.BaseRefreshView<D> {

    private var rootView:View? = null

    private var loadingView:View? = null

    protected var smartRefreshLayout: SmartRefreshLayout?= null

    protected var recyclerView: androidx.recyclerview.widget.RecyclerView? = null

    protected var baseAdapter : BaseAdapter<D>? = null

    protected var p:BaseRefreshContract.BaseRefreshPresenter?= null

    override fun onViewCreated(view: View?, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        initView()
    }

    override fun onCreateView(inflater: LayoutInflater?, container: ViewGroup?, savedInstanceState: Bundle?): View {
        rootView = inflater?.inflate(R.layout.fragment_base_refresh,container,false)
        return rootView!!
    }

    protected fun <T : View> findViewById(@IdRes id: Int): T? {
        return if (id == NO_ID) {
            null
        } else rootView?.findViewById(id)
    }



    protected fun initView() {
        p = getPresenter()
        loadingView = findViewById(R.id.loadingLayout)
        smartRefreshLayout = findViewById(R.id.ptrLayout)
        smartRefreshLayout?.setEnableRefresh(true)
        smartRefreshLayout?.setEnableLoadMore(true)
        smartRefreshLayout?.setOnRefreshListener(OnRefreshListener {
            p?.refresh()
        })
        smartRefreshLayout?.setOnLoadMoreListener(OnLoadMoreListener {
            p?.loadMore()
        })
        recyclerView = findViewById(R.id.mRecyclerView)
        baseAdapter = getAdapter()
        recyclerView?.adapter = baseAdapter
        recyclerView?.layoutManager = getLayoutManager()
    }

    abstract fun getAdapter(): BaseAdapter<D>

    abstract fun getLayoutManager(): androidx.recyclerview.widget.RecyclerView.LayoutManager



    abstract fun getPresenter():P

}