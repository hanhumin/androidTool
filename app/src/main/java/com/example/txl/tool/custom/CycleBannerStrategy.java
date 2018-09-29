package com.example.txl.tool.custom;

import android.support.annotation.NonNull;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.View;

import java.util.List;

/**
 * create by txl
 * at 2018/9/29
 * description :
 */
public class CycleBannerStrategy implements IBannerStrategy {

    private static final String TAG = "NormalBannerStrategy";

    public static final String NAME = CycleBannerStrategy.class.getName();

    static {
        Log.d(TAG,"NormalBannerStrategy 初始化");
        BannerContext.registerStrategy(NAME,new CycleBannerStrategy());
    }

    private int count;

    @Override
    public int toRealPosition(int position) {
        int realPosition = (position - 1) % count;
        if (realPosition < 0)
            realPosition += count;
        return realPosition;
    }

    @Override
    public void initBannerItemView(@NonNull List<View> itemViews, FlexibleBannerView.ViewCreator creator) {
        itemViews.clear();
        if(creator == null || creator.getCount() == 0){
            Log.d(TAG,"ViewCreator creator is null or count is zero please check");
            return;
        }
        count = creator.getCount();
        View itemView;
        for (int i=0; i< count+1;i++){
            int actualLocation;
            if (i == 0) {
                actualLocation = count -1;
            } else if (i == count + 1) {
                actualLocation = 0;
            } else {
                actualLocation = i-1;
            }
            itemView = creator.createView(actualLocation);
            itemViews.add(itemView);
        }
    }

    @Override
    public void setPageIndex(ViewPager viewPager, int state, int currentItemPosition) {
        switch (state) {
            case ViewPager.SCROLL_STATE_IDLE://No operation
                if (currentItemPosition == 0) {
                    viewPager.setCurrentItem(count, false);
                } else if (currentItemPosition == count + 1) {
                    viewPager.setCurrentItem(1, false);
                }
                break;
            case ViewPager.SCROLL_STATE_DRAGGING://start Sliding
                if (currentItemPosition == count + 1) {
                    viewPager.setCurrentItem(1, false);
                } else if (currentItemPosition == 0) {
                    viewPager.setCurrentItem(count, false);
                }
                break;
            case ViewPager.SCROLL_STATE_SETTLING://end Sliding
                break;
        }
    }

}
