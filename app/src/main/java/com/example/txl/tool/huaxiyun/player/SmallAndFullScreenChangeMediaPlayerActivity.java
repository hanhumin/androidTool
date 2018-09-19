package com.example.txl.tool.huaxiyun.player;


import android.content.Context;
import android.graphics.SurfaceTexture;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;

import com.example.txl.tool.BaseActivity;


/**
 * 播放器可以大小屏幕切换
 * */

public class SmallAndFullScreenChangeMediaPlayerActivity extends BaseActivity{

    private static final String TAG = SmallAndFullScreenChangeMediaPlayerActivity.class.getSimpleName();
    private HuaXiYunSimplePlayer huaXiYunSimplePlayer;
    CommonPlayerUISwitcher uiSwitcher;
    CommonPlayerController playerController;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );

        LinearLayout frameLayout = new LinearLayout( this );
        frameLayout.setLayoutParams( new ViewGroup.LayoutParams( ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT ) );
        frameLayout.setOrientation(LinearLayout.VERTICAL);
        setContentView( frameLayout);

        huaXiYunSimplePlayer = new HuaXiYunSimplePlayer(true,true);
        huaXiYunSimplePlayer.init(this);
        // FIXME: 2018/9/18 设计有问题 PlayerAdapter 会和播放器相互持有
        PlayerAdapter playerAdapter = new PlayerAdapter(huaXiYunSimplePlayer);
        huaXiYunSimplePlayer.setEventListener(playerAdapter);
        uiSwitcher = new CommonPlayerUISwitcher( playerAdapter );
        uiSwitcher.addSurfaceTextureListener(playerAdapter);
        uiSwitcher.initView(frameLayout,this);
        playerController = new CommonPlayerController( playerAdapter );
        playerController.init(null);
    }

    @Override
    protected void onStop() {
        super.onStop();
    }


    /**
     *  FIXME: 2018/9/18 感觉基本adapter设计不是很合理，
     想要的效果是：逻辑控制和ui控制本身并不知道播放器是一个什么样子的对象。也不需要知道播放器对
     象的具体实现，只需要通过Adapter来实现一系列的交互操作即可
     */
    class PlayerAdapter extends BasePlayerAdapter<HuaXiYunSimplePlayer> implements IMediaPlayer.IPlayerEvents<HuaXiYunSimplePlayer>,TextureView.SurfaceTextureListener {

        private boolean prepared, surfaceAvailable;

        public PlayerAdapter(HuaXiYunSimplePlayer player) {
            super(player);
        }

        @Override
        public void showUI(String componentId, boolean show) {

        }

        @Override
        public HuaXiYunSimplePlayer getPlayer() {
            return super.getPlayer();
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
            if (paused){
                huaXiYunSimplePlayer.play();
            }else {
                huaXiYunSimplePlayer.pause();
            }
        }

        @Override
        public void init(Context context) {

        }

        @Override
        public long getDuration() {
            return 0;
        }

        @Override
        public long getCurrentPosition() {
            return 0;
        }

        @Override
        public boolean play() {
            huaXiYunSimplePlayer.play();
            return false;
        }

        @Override
        public boolean open(String url) {
            prepared = surfaceAvailable =  false;
            huaXiYunSimplePlayer.open(url);
            return true;
        }

        @Override
        public boolean pause() {
            return huaXiYunSimplePlayer.pause();
        }

        @Override
        public boolean stop() {
            return huaXiYunSimplePlayer.stop();
        }

        @Override
        public boolean isPlaying() {
            return huaXiYunSimplePlayer.isPlaying();
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
        public boolean onError(HuaXiYunSimplePlayer player, int code, String msg) {
            playerController.onError(player,code,msg);
            return false;
        }

        @Override
        public boolean onPrepared(HuaXiYunSimplePlayer player) {
            prepared = true;
            if(surfaceAvailable){
                playerController.onPrepared(player);
            }
            return true;
        }

        @Override
        public boolean onSeekComplete(HuaXiYunSimplePlayer player, long pos) {
            return false;
        }

        @Override
        public boolean onComplete(HuaXiYunSimplePlayer player) {
            return playerController.onComplete(player);
        }

        @Override
        public boolean onBuffering(HuaXiYunSimplePlayer player, boolean buffering, float percentage) {
            return false;
        }

        @Override
        public boolean onProgress(HuaXiYunSimplePlayer player, long pos) {
            return false;
        }

        @Override
        public void onDestroy(HuaXiYunSimplePlayer player) {

        }

        @Override
        public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
            surfaceAvailable = true;
            huaXiYunSimplePlayer.setMediaPlayerSurface(new Surface(surface));
            if(prepared){
                playerController.onPrepared(player);
            }
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

        }
    }
}
