package com.example.txl.tool.player;


import android.content.Context;
import android.os.Bundle;
import android.support.constraint.ConstraintLayout;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.SeekBar;

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

        FrameLayout frameLayout = new FrameLayout( this ){
            @Override
            public void addView(View child) {
                super.addView( child );
            }
        };
        frameLayout.setLayoutParams( new ViewGroup.LayoutParams( ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT ) );
        setContentView( frameLayout);
        new PlayerUiController<FrameLayout>( frameLayout,this );
    }



    /**
     * 持有播放器相应的事件回调接口，对应事件进行处理；播放控制器;
     * 参照IMediaPlayer接口，设计播放器相关接口,初步设想应该持有一个播放控制器
     * */
    public class PlayerUiController<T> implements View.OnClickListener{
        ViewGroup parent;
        ConstraintLayout rootConstraintLayout;
        ImageView ivBack, ivMore, ivPlayerController, ivChangeToFull;
        SeekBar playerSeekBar;

        T playerController;

        public PlayerUiController(ViewGroup parent, Context context) {
            this.parent = parent;
            initView(context);
        }

        private void initView(Context context){

            rootConstraintLayout = (ConstraintLayout) LayoutInflater.from( context ).inflate( R.layout.activity_small_and_full_screen_change_player,parent,false );
            parent.addView(rootConstraintLayout);
            ivBack = rootConstraintLayout.findViewById( R.id.iv_small_player_ui_back );
            ivBack.setOnClickListener( this );
            ivMore = rootConstraintLayout.findViewById( R.id.iv_small_player_ui_more );
            ivMore.setOnClickListener( this );
            ivPlayerController = rootConstraintLayout.findViewById( R.id.iv_small_player_ui_play_controller );
            ivPlayerController.setOnClickListener( this );
            ivChangeToFull = rootConstraintLayout.findViewById( R.id.iv_small_player_ui_change_to_full );
            ivChangeToFull.setOnClickListener( this );
            playerSeekBar = rootConstraintLayout.findViewById( R.id.sb_small_player_ui_seek );
            parent.invalidate();
            // TODO: 2018/9/6 playerSeekBar 添加对应的事件
        }

        @Override
        public void onClick(View v) {
            switch (v.getId()){
                case R.id.iv_small_player_ui_back:
                    break;
                case R.id.iv_small_player_ui_more:
                    break;
                case R.id.iv_small_player_ui_play_controller:
                    break;
                case R.id.iv_small_player_ui_change_to_full:
                    break;
            }
        }

        public void setPlayerController(T playerController){
            this.playerController = playerController;
        }
    }
}
