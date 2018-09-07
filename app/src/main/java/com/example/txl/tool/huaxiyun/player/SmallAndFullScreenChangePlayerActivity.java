package com.example.txl.tool.huaxiyun.player;


import android.content.Context;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.support.constraint.ConstraintLayout;
import android.view.LayoutInflater;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.RotateAnimation;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.SeekBar;

import com.example.txl.tool.BaseActivity;
import com.example.txl.tool.R;

import java.io.IOException;

/**
 * 播放器可以大小屏幕切换
 * */

public class SmallAndFullScreenChangePlayerActivity extends BaseActivity implements IPlayer{

    private static final String TAG = SmallAndFullScreenChangePlayerActivity.class.getSimpleName();
    private MediaPlayer mediaPlayer;
    CommonPlayerUIController uiController;
    private TextureView textureView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );

        LinearLayout frameLayout = new LinearLayout( this ){
            @Override
            public void addView(View child) {
                super.addView( child );
            }
        };
        frameLayout.setLayoutParams( new ViewGroup.LayoutParams( ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT ) );
        frameLayout.setOrientation(LinearLayout.VERTICAL);
        textureView = new TextureView(this);
        textureView.setLayoutParams( new ViewGroup.LayoutParams( ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT ) );
        setContentView( frameLayout);
        mediaPlayer = new MediaPlayer();
        uiController = new CommonPlayerUIController( this,frameLayout,this );
        try {
            mediaPlayer.setDataSource( this, Uri.parse("https://aweme.snssdk.com/aweme/v1/playwm/?video_id=v0200ff50000bd0p9ur6936mllnbeo40&line=0") );
        } catch (IOException e) {
            e.printStackTrace();
        }
        mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
//                mediaPlayer.start();
//                mediaPlayer.setSurface(new Surface(textureView.getSurfaceTexture()));
            }
        });
        frameLayout.addView(textureView);
    }

    @Override
    public long getDuration() {
        return 0;
    }

    @Override
    public boolean play() {
        // FIXME: 2018/9/7 应该根据播放器状态来
        mediaPlayer.prepareAsync();
        mediaPlayer.setLooping(true);
        mediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                uiController.onPrepared(null);
                mediaPlayer.start();
            }
        });
        return false;
    }

    @Override
    public boolean open(String url) {
        return false;
    }

    @Override
    public boolean pause() {
        return false;
    }

    @Override
    public boolean stop() {
        return false;
    }

    @Override
    public boolean isPlaying() {
        return false;
    }

    @Override
    public boolean seekTo(long pos) {
        return false;
    }

    @Override
    public boolean releasePlayer() {
        return false;
    }

    @Override
    public void destroy() {

    }

    @Override
    public void updateProgress() {

    }

    @Override
    public void setEventListener(IPlayerEvents listener) {

    }

    @Override
    public void setSurface(Surface surface) {
        mediaPlayer.setSurface(surface);




    }


    /**
     * 持有播放器相应的事件回调接口，对应事件进行处理；播放控制器;
     * 参照IMediaPlayer接口，设计播放器相关接口,初步设想应该持有一个播放控制器
     * */
    public class PlayerUiController<T> implements View.OnClickListener{
        Context context;
        ViewGroup parent;
        ConstraintLayout rootConstraintLayout;//自定义view构造播放控件，需要解决滑动冲突，
        ImageView ivBack, ivMore, ivPlayerController, ivChangeToFull;
        SeekBar playerSeekBar;

        T playerController;

        public PlayerUiController(ViewGroup parent, Context context) {
            this.parent = parent;
            this.context = context;
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
                    RotateAnimation rotateAnimation = new RotateAnimation( 0,90 ,100,100);

                    rotateAnimation.setDuration( 300 );
                    rotateAnimation.setFillAfter( true );
                    rootConstraintLayout.startAnimation( rotateAnimation );
                    break;
            }
        }

        public void setPlayerController(T playerController){
            this.playerController = playerController;
        }
    }
}
