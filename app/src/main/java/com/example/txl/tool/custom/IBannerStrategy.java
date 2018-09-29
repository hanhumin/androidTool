package com.example.txl.tool.custom;

import android.support.v4.view.ViewPager;
import android.view.View;
import java.util.List;

/**
 * create by txl
 * at 2018/9/29
 * description : 轮播的策略，例如普通轮播，无限轮播
 */
public interface IBannerStrategy {

    /**
     * 计算当前显示View的数据位置
     * */
    int toRealPosition(int position);

    void initBannerItemView(List<View> itemViews, FlexibleBannerView.ViewCreator creator);

    /**
     * 根据状态，计算当前应该被选中的位置
     * */
    void setPageIndex(ViewPager viewPager, int state, int currentItemPosition);
}
