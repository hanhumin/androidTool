package com.example.txl.tool.player;


import android.content.Context;
import android.os.Bundle;
import android.support.constraint.ConstraintLayout;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.ViewGroup;

import com.example.txl.tool.BaseActivity;
import com.example.txl.tool.R;

/**
 * 播放器可以大小屏幕切换
 * */

public class SmallAndFullScreenChangePlayerActivity extends BaseActivity {

    private static final String TAG = SmallAndFullScreenChangePlayerActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_small_and_full_screen_change_player );
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);//display = getWindowManager().getDefaultDisplay();display.getMetrics(dm)（把屏幕尺寸信息赋值给DisplayMetrics dm）;
        int width = dm.widthPixels;
        int height = dm.heightPixels;
        Log.d( TAG,"缩放比例   "+getResources().getDisplayMetrics().density +"  "+dm.density+"  "+width+"  height:"+height);
    }



    public class PlayerUiController<T extends ViewGroup>{
        T parent;
        ConstraintLayout rootConstraintLayout;
        public PlayerUiController(T parent, Context context) {
            this.parent = parent;
            rootConstraintLayout = (ConstraintLayout) LayoutInflater.from( context ).inflate( R.layout.activity_small_and_full_screen_change_player,parent,false );
        }

        private void initView(){

        }
    }
}
