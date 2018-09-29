package com.example.txl.tool.custom;

import android.content.Context;
import android.graphics.Canvas;
import android.support.annotation.NonNull;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.widget.PagerSnapHelper;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.Scroller;

import com.example.txl.tool.R;

import java.util.ArrayList;
import java.util.List;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/18
 * description：
 */
public class CustomBannerView extends FrameLayout implements ViewPager.OnPageChangeListener {
    private static final String TAG = "CustomBannerView";

    public static final int BANNER_STYLE_NORMAL = 0;
    public static final int BANNER_STYLE_CYCLE = 1;

    private int bannerStyle = BANNER_STYLE_NORMAL;
    private int currentItem;

    protected ViewPager viewPager;
    private List<ViewPager.OnPageChangeListener> mOnPageChangeListeners;

    private List<View> itemViews;

    protected ViewCreator creator;
    private PagerAdapter adapter;
    private BannerContext _mBannerContext;

    private boolean isAutoPlay;
    private int count;

    private long delayTime;

    private final Runnable task = new Runnable() {
        @Override
        public void run() {
            if (count > 1 && isAutoPlay) {
                currentItem = currentItem % (count + 1) + 1;
//                Log.i(tag, "curr:" + currentItem + " count:" + count);
                if (currentItem == 1) {
                    viewPager.setCurrentItem(currentItem, false);
                    CustomBannerView.this.post(task);
                } else {
                    viewPager.setCurrentItem(currentItem);
                    CustomBannerView.this.postDelayed(task, delayTime);
                }
            }
        }
    };

    public CustomBannerView(Context context) {
        this( context ,null);
    }

    public CustomBannerView(Context context, AttributeSet attrs) {
        this( context, attrs, 0 );
    }

    public CustomBannerView(Context context, AttributeSet attrs, int defStyleAttr) {
        super( context, attrs, defStyleAttr );
        init(context);
    }

    private void init(Context context) {
        initParams();
        initView(context);
    }

    private void initParams() {
        itemViews = new ArrayList<>();
        _mBannerContext = new BannerContext(NormalBannerStrategy.NAME);
    }

    private void initView(Context context){
        viewPager = new ViewPager(context);
        addView(viewPager);
        adapter = new BannerPagerAdapter();
        viewPager.setAdapter(adapter);
        viewPager.addOnPageChangeListener(this);
    }

    /**
     * 设置轮播的类型
     * */
    public void setBannerStyle(String bannerName){
        _mBannerContext.changeBannerStrategy(bannerName);
    }

    @Override
    public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
        if(mOnPageChangeListeners != null){
            for(ViewPager.OnPageChangeListener listener : mOnPageChangeListeners){
                listener.onPageScrolled(_mBannerContext.toRealPosition(position),positionOffset,positionOffsetPixels);
            }
        }
    }

    @Override
    public void onPageSelected(int position) {
        currentItem=position;
        if(mOnPageChangeListeners != null){
            for(ViewPager.OnPageChangeListener listener : mOnPageChangeListeners){
                listener.onPageSelected(_mBannerContext.toRealPosition(position));
            }
        }
    }

    @Override
    public void onPageScrollStateChanged(int state) {
        if(mOnPageChangeListeners != null){
            for(ViewPager.OnPageChangeListener listener:mOnPageChangeListeners){
                listener.onPageScrollStateChanged(state);
            }
        }
        _mBannerContext.setPageIndex(viewPager,state,currentItem);
    }

    public void addOnPageChangeListener(ViewPager.OnPageChangeListener listener){
        if(mOnPageChangeListeners == null){
            mOnPageChangeListeners = new ArrayList<>();
        }
        mOnPageChangeListeners.add(listener);
    }

    public void setAutoPlay(boolean autoPlay) {
        isAutoPlay = autoPlay;
    }

    public void setViewCreator(ViewCreator creator){
        this.creator = creator;
        initItemViews(creator);
    }

    public ViewPager getViewPager() {
        return viewPager;
    }

    private void initItemViews(@NonNull ViewCreator creator){
        _mBannerContext.initBannerItemView(itemViews,creator);
        count = creator.getCount();
        creator.notifyDataSetChanged();
    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent ev) {
//        Log.i(tag, ev.getAction() + "--" + isAutoPlay);
        if (isAutoPlay) {
            int action = ev.getAction();
            if (action == MotionEvent.ACTION_UP || action == MotionEvent.ACTION_CANCEL
                    || action == MotionEvent.ACTION_OUTSIDE) {
                startAutoPlay();
            } else if (action == MotionEvent.ACTION_DOWN) {
                stopAutoPlay();
            }
        }
        return super.dispatchTouchEvent(ev);
    }

    private void stopAutoPlay() {
        this.post(task);
    }

    private void startAutoPlay() {
        this.removeCallbacks(task);
    }

    class BannerPagerAdapter extends PagerAdapter{

        @Override
        public int getCount() {
            return itemViews.size();
        }

        @Override
        public boolean isViewFromObject(View view, Object object) {
            return view == object;
        }

        @Override
        public Object instantiateItem(ViewGroup container, int position) {
            View v = itemViews.get(position);
            container.addView(v);
            return v;
        }

        @Override
        public void destroyItem(ViewGroup container, int position, Object object) {
            container.removeView((View) object);
        }
    }

    public abstract class ViewCreator{
        /**
         * Return the number of views available.
         */
        abstract int getCount();

        abstract View createView(int position);

        public void notifyDataSetChanged(){
            adapter.notifyDataSetChanged();
        }
    }
}
