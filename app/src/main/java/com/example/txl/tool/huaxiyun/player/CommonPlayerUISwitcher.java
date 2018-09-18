package com.example.txl.tool.huaxiyun.player;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.graphics.Point;
import android.graphics.SurfaceTexture;
import android.support.constraint.ConstraintLayout;
import android.view.LayoutInflater;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.SeekBar;

import com.example.txl.tool.R;
import com.example.txl.tool.utils.DisplayUtil;

public class CommonPlayerUISwitcher extends AbsBasePlayerUiSwitcher{
    private ImageView ivBack, ivMore, ivPlayerController, ivChangeToFull;
    private SeekBar playerSeekBar;

    private int resLayoutId = R.layout.activity_small_and_full_screen_change_player;

    /**
     * 小屏 大屏播放器的高度
     * */
    private int smallUiHeight, fullUiHeight;

    public CommonPlayerUISwitcher(BasePlayerAdapter adapter, ViewGroup parent, Context context) {
        super(adapter,parent, context);
    }

    @Override
    void initView(Context context) {
        rootFrameView = (FrameLayout) LayoutInflater.from( context ).inflate( resLayoutId,parent,false );
        parent.addView(rootFrameView);
        rootConstraintLayout = rootFrameView.findViewById(R.id.cl_small_player_ui_content);
        ivBack = rootConstraintLayout.findViewById( R.id.iv_small_player_ui_back );
        ivBack.setOnClickListener( this );
        ivMore = rootConstraintLayout.findViewById( R.id.iv_small_player_ui_more );
        ivMore.setOnClickListener( this );
        ivPlayerController = rootConstraintLayout.findViewById( R.id.iv_small_player_ui_play_controller );
        ivPlayerController.setOnClickListener( this );
        ivChangeToFull = rootConstraintLayout.findViewById( R.id.iv_small_player_ui_change_to_full );
        ivChangeToFull.setOnClickListener( this );
        // TODO: 2018/9/6 playerSeekBar 添加对应的事件
        playerSeekBar = rootConstraintLayout.findViewById( R.id.sb_small_player_ui_seek );
        playerPresenter = rootFrameView.findViewById(R.id.texture_view_player_content_presenter);
        playerPresenter.setSurfaceTextureListener(this);

    }

    @Override
    public void addUi(View view) {
        super.addUi( view );
    }

    boolean fullScreen = false;
    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.iv_small_player_ui_back:
                break;
            case R.id.iv_small_player_ui_more:
                break;
            case R.id.iv_small_player_ui_play_controller:
                // FIXME: 2018/9/7 播放器不同的状态做不同的处理
                break;
            case R.id.iv_small_player_ui_change_to_full:
                fullScreen = !fullScreen;
                changeScreen(fullScreen);
                break;
        }
    }

    private void changeScreen(boolean fullScreen) {
        if(fullScreen){
            ((Activity) context).setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
            ((Activity) context).getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
            ViewGroup.LayoutParams layoutParams = rootFrameView.getLayoutParams();
            layoutParams.height = ViewGroup.LayoutParams.MATCH_PARENT;
            rootFrameView.setLayoutParams(layoutParams);
            rootFrameView.postInvalidate();
        }else {
            ((Activity) context).setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
            ((Activity) context).getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
            ViewGroup.LayoutParams layoutParams = rootFrameView.getLayoutParams();
            Point point = new Point();
            ((Activity) context).getWindowManager().getDefaultDisplay().getSize(point);
            int screenHeight = point.y;
            layoutParams.height = DisplayUtil.dip2px(context,283);
            rootFrameView.setLayoutParams(layoutParams);
            rootFrameView.postInvalidate();
        }
    }

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        super.onSurfaceTextureAvailable( surface, width, height );
//        _adapter.onSurfaceTextureAvailable( surface, width, height );
        // TODO: 2018/9/7 播放器内容可播放
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        super.onSurfaceTextureSizeChanged( surface,width,height );
    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        return super.onSurfaceTextureDestroyed( surface );
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        super.onSurfaceTextureUpdated( surface );
    }
}
