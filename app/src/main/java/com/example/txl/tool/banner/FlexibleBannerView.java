package com.example.txl.tool.banner;

import android.content.Context;
import android.content.res.TypedArray;
import androidx.annotation.NonNull;
import androidx.viewpager.widget.PagerAdapter;
import androidx.viewpager.widget.ViewPager;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.example.txl.tool.R;

import java.util.ArrayList;
import java.util.List;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/18
 * description：
 */
public class FlexibleBannerView extends FrameLayout implements ViewPager.OnPageChangeListener {
    private static final String TAG = "CustomBannerView";

    private int currentItem;

    protected ViewPager viewPager;
    private List<ViewPager.OnPageChangeListener> mOnPageChangeListeners;

    private List<View> itemViews;

    protected IViewCreator creator;
    private PagerAdapter adapter;
    private BannerContext _mBannerContext;

    private boolean isAutoPlay;
    private int count;

    private int delayTime = 3 * 1000;
    private int viewPagerId = -1;

    private final Runnable task = new Runnable() {
        @Override
        public void run() {
            if (count > 1 && isAutoPlay) {
                currentItem = currentItem % (count + 1) + 1;
//                Log.i(tag, "curr:" + currentItem + " count:" + count);
                if (currentItem == 1) {
                    viewPager.setCurrentItem(currentItem, false);
                    FlexibleBannerView.this.post(task);
                } else {
                    viewPager.setCurrentItem(currentItem);
                    FlexibleBannerView.this.postDelayed(task, delayTime);
                }
            }
        }
    };

    public FlexibleBannerView(Context context) {
        this( context ,null);
    }

    public FlexibleBannerView(Context context, AttributeSet attrs) {
        this( context, attrs, 0 );
    }

    public FlexibleBannerView(Context context, AttributeSet attrs, int defStyleAttr) {
        super( context, attrs, defStyleAttr );
        init(context,attrs);
    }

    private void init(Context context, AttributeSet attrs) {
        initParams();
        initView(context,attrs);
    }

    private void initParams() {
        itemViews = new ArrayList<>();
        _mBannerContext = new BannerContext( NormalBannerStrategy.NAME);
    }

    private void initView(Context context, AttributeSet attrs){
        handleTypedArray(context, attrs);
        viewPager = findViewById( viewPagerId );
        if(viewPager == null){
            viewPager = new ViewPager( context );
            addView(viewPager,0);
        }

    }

    private void handleTypedArray(Context context, AttributeSet attrs) {
        if(attrs == null){
            return;
        }
        TypedArray typedArray = context.obtainStyledAttributes(attrs, R.styleable.FlexibleBannerView);
        isAutoPlay = typedArray.getBoolean( R.styleable.FlexibleBannerView_auto_play,isAutoPlay );
        delayTime = typedArray.getInt( R.styleable.FlexibleBannerView_delay_time,delayTime );
        viewPagerId = typedArray.getResourceId( R.styleable.FlexibleBannerView_viewPagerId,-1 );
        typedArray.recycle();
    }

    /**
     * 设置轮播的类型
     * */
    public void setBannerStyle(String bannerName){
        _mBannerContext.changeBannerStrategy(bannerName);
    }

    public void setCurrentItem(int position){
        viewPager.setCurrentItem( position );
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

    public FlexibleBannerView setAutoPlay(boolean autoPlay) {
        isAutoPlay = autoPlay;
        return this;
    }

    public FlexibleBannerView setViewCreator(IViewCreator creator){
        this.creator = creator;
        return this;
    }

    public ViewPager getViewPager() {
        return viewPager;
    }

    public FlexibleBannerView start(){
        initItemViews(creator,0);
        return this;
    }

    public FlexibleBannerView start(int item){
        initItemViews(creator,item);
        return this;
    }

    private void initItemViews(@NonNull IViewCreator creator,int item){
        _mBannerContext.initBannerItemView(itemViews,creator);
        count = creator.getCount();
        if(adapter == null){
            adapter = new BannerPagerAdapter();
            viewPager.addOnPageChangeListener(this);
        }
        viewPager.setAdapter(adapter);
        viewPager.setCurrentItem( item );
        if(isAutoPlay){
            startAutoPlay();
        }
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
        this.removeCallbacks(task);

    }

    private void startAutoPlay() {
        this.removeCallbacks(task);
        this.postDelayed(task,delayTime);
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

    public interface IViewCreator {
        /**
         * Return the number of views available.
         */
        abstract int getCount();

        abstract View createView(int position);
    }
}
