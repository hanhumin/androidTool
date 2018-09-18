package com.example.txl.tool.huaxiyun.player;


import android.content.Context;
import android.graphics.SurfaceTexture;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.support.constraint.ConstraintLayout;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.RotateAnimation;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.SeekBar;

import com.example.txl.tool.BaseActivity;
import com.example.txl.tool.R;

import java.io.IOException;

/**
 * 播放器可以大小屏幕切换
 * */

public class SmallAndFullScreenChangeMediaPlayerActivity extends BaseActivity implements IMediaPlayer {

    private static final String TAG = SmallAndFullScreenChangeMediaPlayerActivity.class.getSimpleName();
    private MediaPlayer mediaPlayer;
    CommonPlayerUISwitcher uiSwitcher;
    CommonPlayerController playerController;
    private TextureView textureView;
    boolean prepared = false, surfaceTextureAvailable = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );

        LinearLayout frameLayout = new LinearLayout( this );
        frameLayout.setLayoutParams( new ViewGroup.LayoutParams( ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT ) );
        frameLayout.setOrientation(LinearLayout.VERTICAL);
        setContentView( frameLayout);
        textureView = new TextureView(this);
        textureView.setLayoutParams( new ViewGroup.LayoutParams( ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT ) );
        PlayerAdapter playerAdapter = new PlayerAdapter();
        uiSwitcher = new CommonPlayerUISwitcher( playerAdapter,frameLayout,this );
        uiSwitcher.setSurfaceTextureListener( playerAdapter );
        playerController = new CommonPlayerController( playerAdapter );
        setEventListener( playerController );
        mediaPlayer = new MediaPlayer();
        try {
            mediaPlayer.setDataSource( this, Uri.parse("https://aweme.snssdk.com/aweme/v1/playwm/?video_id=v0200ff50000bd0p9ur6936mllnbeo40&line=0") );
        } catch (IOException e) {
            e.printStackTrace();
        }
        mediaPlayer.setLooping(true);
        mediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                playerController.onPrepared(SmallAndFullScreenChangeMediaPlayerActivity.this);
                mediaPlayer.start();
            }
        });
        mediaPlayer.prepareAsync();
        mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
//                mediaPlayer.start();
//                mediaPlayer.setSurface(new Surface(textureView.getSurfaceTexture()));
            }
        });
        frameLayout.addView(textureView);
        DisplayMetrics metric = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metric);
        int width = metric.widthPixels; // 屏幕宽度（像素）
        int height = metric.heightPixels; // 屏幕高度（像素）
        float density = metric.density; // 屏幕密度（0.75 / 1.0 / 1.5）
        int densityDpi = metric.densityDpi; // 屏幕密度DPI（120 / 160 / 240）
        Log.d( TAG,"width:"+width+" height:"+height+" density:"+density+" densityDpi:"+densityDpi );
    }

    @Override
    protected void onStop() {
        super.onStop();
        playerController.stop();
    }

    @Override
    public void init(Context context) {

    }

    @Override
    public long getDuration() {
        return mediaPlayer.getDuration();
    }

    @Override
    public long getCurrentPosition() {
        return 0;
    }

    @Override
    public boolean play() {
        mediaPlayer.start();
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
        mediaPlayer.stop();
        return true;
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


    class PlayerAdapter extends BasePlayerAdapter{

        @Override
        public void showUI(String componentId, boolean show) {

        }

        @Override
        public IMediaPlayer getMediaPlayer() {
            return SmallAndFullScreenChangeMediaPlayerActivity.this;
        }

        @Override
        public Object getDataUtils() {
            return null;
        }

        @Override
        public AbsBasePlayerUiSwitcher getUiSwitcher() {
            return uiSwitcher;
        }

        @Override
        public AbsMediaPlayerController getController() {
            return playerController;
        }

        @Override
        public View getUiComponent(String name) {
            return null;
        }

        @Override
        public void setProgress(float pos) {

        }

        @Override
        public boolean dispatchSeekControllerEvents(KeyEvent event) {
            return false;
        }

        @Override
        public void onPlayPaused(boolean paused) {

        }

        @Override
        public void destroy() {

        }

        @Override
        public String makePlayUrl(String playUrl, Bundle data) {
            return null;
        }

        @Override
        public String getAspectRatio() {
            return null;
        }

        @Override
        public void setAspectRatio(String ratio) {

        }

        @Override
        public View getPlayerView() {
            return null;
        }

        @Override
        public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
            mediaPlayer.setSurface( new Surface( surface ) );
        }

        @Override
        public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {

        }

        @Override
        public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
            return false;
        }

        @Override
        public void onSurfaceTextureUpdated(SurfaceTexture surface) {
            //test git add
        }
    }
}
