package com.txl.lib.wiget;

import android.view.ViewGroup;
import android.view.animation.Animation;
import android.widget.ImageView;
import android.widget.TextView;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/2
 * description：
 */
public abstract class RecyclerHeaderView extends AbsPullRefreshView {
    private final String TAG = RecyclerHeaderView.class.getSimpleName();

    /**
     *顶部view，这里使用RelativeLayout
     */
    private ViewGroup mContainer;

    private ImageView mImageView;

    private TextView mTitleTextView;

    private Animation rotateAnimation;

    public RecyclerHeaderView(int viewType) {
        super( viewType );
    }


//    public RecyclerHeaderView(Context context) {
//        this( context,null );
//    }
//
//    public RecyclerHeaderView(Context context, @Nullable AttributeSet attrs) {
//        this( context, attrs,0 );
//    }

//    public RecyclerHeaderView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
//        super( context, attrs, defStyleAttr );
//    }
//
//    public void init(Context context){
//        LayoutParams lp = new LayoutParams( LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT );
//        setLayoutParams( lp );
//        setGravity( Gravity.BOTTOM );
//        mContainer = (ViewGroup) LayoutInflater.from( context ).inflate( R.layout.pull_refresh_header, this,false);
//        addView( mContainer,lp );
//        mImageView = mContainer.findViewById( R.id.pull_refresh_header_ImageView );
//        mTitleTextView = mContainer.findViewById( R.id.pull_refresh_header_title_TextView );
//        setState(STATE_NORMAL);
//        setBackgroundColor( Color.BLACK );
//        rotateAnimation = AnimationUtils.loadAnimation( context,R.anim.refreshing_rotate );
//        rotateAnimation.setRepeatCount(Animation.INFINITE);
//        rotateAnimation.setRepeatMode(Animation.RESTART);
//    }
//
//    public void setState(int state){
//        if(currentState == state){
//            return;
//        }
//        switch (state){
//            case STATE_NORMAL:
//                mTitleTextView.setText( "下拉刷新" );
//                mImageView.setImageResource( R.drawable.ic_up_48 );
//                mImageView.clearAnimation();
//                break;
//            case STATE_READY:
//                mTitleTextView.setText( "松开刷新数据" );
//                mImageView.setImageResource( R.drawable.ic_up_48 );
//                mImageView.clearAnimation();
//                break;
//            case STATE_REFRESHING:
//                // FIXME: 2018/8/3 图片的旋转动画并没有一直执行?
//                mTitleTextView.setText( "正在加载。。。" );
//                mImageView.setImageResource( R.drawable.ic_refreshing_48 );
//                mImageView.setAnimation( rotateAnimation );
//                mImageView.startAnimation( rotateAnimation );
//                break;
//        }
//        currentState = state;
//    }
//
//    public int getRealityHeight(){
//        return mContainer.getHeight();
//    }
//
//    public void setVisibleHeight(int height){
//        // getLayoutParams的作用是告诉父容器怎么来摆放自己，这里的layoutParams根据recyclerView的不同会有所不同
//        if(height < 0 ){
//            height = 0;
//        }
//        MarginLayoutParams marginLayoutParams = (MarginLayoutParams)getLayoutParams();
//        marginLayoutParams.topMargin = height;
//        setLayoutParams( marginLayoutParams );
//    }
//
//    public int getVisibleHeight(){
//        return getLayoutParams().height;
//    }

}
