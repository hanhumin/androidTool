package com.example.txl.tool.huaxiyun.player;


import android.content.Context;
import android.os.Bundle;
import android.view.KeyEvent;
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
        PlayerAdapter playerAdapter = new PlayerAdapter(huaXiYunSimplePlayer);
        uiSwitcher = new CommonPlayerUISwitcher( playerAdapter,frameLayout,this );
        playerController = new CommonPlayerController( playerAdapter );
        huaXiYunSimplePlayer.setEventListener(playerAdapter);
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
    class PlayerAdapter extends BasePlayerAdapter<HuaXiYunSimplePlayer> implements IMediaPlayer.IPlayerEvents<HuaXiYunSimplePlayer> {

        public PlayerAdapter(HuaXiYunSimplePlayer player) {
            super(player);
        }

        @Override
        public void showUI(String componentId, boolean show) {

        }

        @Override
        public Object getDataUtils() {
            return null;
        }

        @Override
        public AbsBasePlayerUiSwitcher getUiSwitcher() {
            return null;
        }

        @Override
        public AbsMediaPlayerController getController() {
            return null;
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
            return false;
        }

        @Override
        public boolean open(String url) {
            huaXiYunSimplePlayer.open(url);
            return true;
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
            playerController.onPrepared(player);
            return false;
        }

        @Override
        public boolean onSeekComplete(HuaXiYunSimplePlayer player, long pos) {
            return false;
        }

        @Override
        public boolean onComplete(HuaXiYunSimplePlayer player) {
            return false;
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
    }
}
