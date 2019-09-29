package com.example.txl.tool.banner;

import androidx.annotation.NonNull;
import androidx.viewpager.widget.ViewPager;
import android.util.Log;
import android.view.View;

import java.util.List;

/**
 * create by txl
 * at 2018/9/29
 * description :
 */
public class NormalBannerStrategy implements IBannerStrategy {
    private static final String TAG = "NormalBannerStrategy";

    public static final String NAME = NormalBannerStrategy.class.getName();

    static {
        Log.d(TAG,"NormalBannerStrategy 初始化");
        BannerContext.registerStrategy(NAME,new NormalBannerStrategy());
    }

    private int count;

    @Override
    public int toRealPosition(int position) {
        return position;
    }

    @Override
    public void initBannerItemView(@NonNull List<View> itemViews, FlexibleBannerView.IViewCreator creator) {
        itemViews.clear();
        if(creator == null || creator.getCount() == 0){
            Log.d(TAG,"IViewCreator creator is null or count is zero please check");
            return;
        }
        count = creator.getCount();
        View itemView;
        for (int i=0; i< count;i++){
            itemView = creator.createView(i);
            itemViews.add(itemView);
        }
    }

    @Override
    public void setPageIndex(ViewPager viewPager, int state, int currentItemPosition) {

    }

}
