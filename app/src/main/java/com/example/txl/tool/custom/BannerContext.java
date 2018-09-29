package com.example.txl.tool.custom;

import android.support.annotation.NonNull;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.View;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * create by txl
 * at 2018/9/29
 * description :
 */
public class BannerContext {
    private static final String TAG = "BannerContext";

    private static Map<String,IBannerStrategy> bannerStrategyMap = new HashMap<>();

    public static void registerStrategy(String strategyKey, IBannerStrategy strategy){
        if(bannerStrategyMap.containsKey(strategyKey)){
            Log.w(TAG,"register Strategy the strategyKey is has been register the old value is replaced by the specified value");
        }
        bannerStrategyMap.put(strategyKey,strategy);
    }

    private IBannerStrategy currentStrategy;

    // FIXME: 2018/9/29 strategy应该一开始就加载到内存中，暂时没有找到实现方法
    public BannerContext(String strategyName) {
        Log.d(TAG,"strategyName:"+strategyName);
        changeBannerStrategy(strategyName);
    }

    /**
     * 在使用对应的策略的时候一定要保证类被加载在内存中
     * */
    public void changeBannerStrategy(String strategyKey){
        currentStrategy = bannerStrategyMap.get(strategyKey);
        if(currentStrategy == null){
            throw new RuntimeException("bannerStrategyMap not contain the "+strategyKey+" key please check");
        }
    }

    public int toRealPosition(int position){
        return currentStrategy.toRealPosition(position);
    }

    public void initBannerItemView(@NonNull List<View> itemViews, FlexibleBannerView.ViewCreator creator){
        currentStrategy.initBannerItemView(itemViews,creator);
    }

    public void setPageIndex(ViewPager viewPager, int state, int currentItemPosition){
        currentStrategy.setPageIndex(viewPager,state,currentItemPosition);
    }
}
