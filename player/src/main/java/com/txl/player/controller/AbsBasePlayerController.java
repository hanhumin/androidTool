package com.txl.player.controller;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.SurfaceView;
import android.view.TextureView;
import android.view.View;

import androidx.annotation.ColorInt;
import androidx.lifecycle.Lifecycle;

import com.txl.player.lifecycle.BasicLifecycleObserver;
import com.txl.player.lifecycle.ILifecycleListener;
import com.txl.player.base.IPlayer;
import com.txl.player.base.IPlayerEventListener;
import com.txl.player.playerimpl.PlayerManager;
import com.txl.player.utils.CancelRunnable;

import org.jetbrains.annotations.NotNull;

import java.io.IOException;

/**
 * 与业务无关的播放器逻辑实现
 */
public abstract class AbsBasePlayerController implements IPlayerController, IPlayerEventListener, ILifecycleListener {
    /**
     * seek偏移 单位 ms  参考爱奇艺一次偏移10s
     */
    private static final long _seekOffset = 10 * 1000;

    /**
     * 未操作的时候隐藏进度条的延时
     */
    private static final long CONTROLLER_HIDE_DELAY_TIME = 5 * 1000;

    private static final int MESSAGE_HIDE_PLAYER_CONTROLLER = 1;
    /**
     * 更新进度
     */
    private static final int MESSAGE_AUTO_UPDATE_PROGRESS = 2;

    protected final String TAG = getClass().getSimpleName();

    protected IPlayerUiController mPlayerUiController;
    protected IPlayer mPlayer;
    protected UiHandler mHandler;


    protected String mPlayUrl = "";

    /**
     * seek 操作的方向
     * true 向前
     * false 向后
     */
    private boolean _seekForward;

    /**
     * 本地记录播放位置
     */
    private long _playPosition;

    /**
     * 总时长
     */
    private long _duration;

    private long _seekPosition;

    protected Context mContext;

    /**
     * 是否因为生命周期stop
     */
    private boolean callLifeCycleStop = false;

    private CancelRunnable updateTimeRunnable = new CancelRunnable() {
        @Override
        protected void realRun() {
            _playPosition = mPlayer.getCurrentPosition();
            _duration = mPlayer.getDuration();
            long currentPlayPosition = _playPosition / 1000;
            long totalPlayTime = _duration / 1000;
            float percent = totalPlayTime <= 0 ? 0 : ((float) currentPlayPosition) / totalPlayTime;
            mPlayerUiController.updatePlayTime(percent, currentPlayPosition, totalPlayTime);
//            Log.d(TAG, "update play time currentPlayPosition is " + currentPlayPosition + " totalPlayTime is " + totalPlayTime + " percent is " + percent);
            mHandler.postDelayed(this, 32);
        }
    };

    private CancelRunnable hideControllerRunnable = new HideControllerRunnable();

    /**
     * 提供无参数的构造方法，但是在使用无参数的构造方法的时候需要人为调用init进行初始化
     */
    public AbsBasePlayerController() {

    }

    public AbsBasePlayerController(Context context, IPlayerUiController playerUiController) {
        init(context, playerUiController);
    }

    public void init(Context context, IPlayerUiController playerUiController) {
        mContext = context;
        this.mPlayerUiController = playerUiController;
        mHandler = new UiHandler(context);
        if (mPlayer != null) {//如果有播放器，将原来的播放器销毁重置
            destroy();
        }
        mPlayer = createPlayer(context);
        mPlayer.setPlayerEventListener(this);
        initPlayerSurfaceView(playerUiController);
    }

    /**
     * 初始化
     */
    protected void initPlayerSurfaceView(IPlayerUiController playerUiController) {
        View playerView = playerUiController.getSurfaceView();
        if (playerView instanceof SurfaceView) {
            SurfaceView surfaceView = (SurfaceView) playerView;
            mPlayer.setVideoSurfaceView(surfaceView);
        } else if (playerView instanceof TextureView) {
            TextureView textureView = (TextureView) playerView;
            mPlayer.setVideoTextureView(textureView);
        }
    }

    public void resetDisPlayView(IPlayerUiController playerUiController) {
        if (mContext != null) {
            resetPlayer();
        }
        this.mPlayerUiController = playerUiController;
        initPlayerSurfaceView(playerUiController);
    }

    @NotNull
    private IPlayer createPlayer(Context context) {
        return PlayerManager.getIPlayer(context);
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
//        Log.d(TAG, " dispatchKeyEvent action : " + event.getAction() + " keyCode: " + event.getKeyCode());
        if (onInterceptKeyEvent(event)) {
            return onKeyEvent(event);
        }
        return false;
    }

    protected boolean onInterceptKeyEvent(KeyEvent event) {
        switch (event.getKeyCode()) {
            case KeyEvent.KEYCODE_MEDIA_REWIND:
            case KeyEvent.KEYCODE_MEDIA_PLAY:
            case KeyEvent.KEYCODE_FORWARD:
            case KeyEvent.KEYCODE_DPAD_UP:
            case KeyEvent.KEYCODE_DPAD_DOWN:
            case KeyEvent.KEYCODE_DPAD_LEFT:
            case KeyEvent.KEYCODE_DPAD_RIGHT:
            case KeyEvent.KEYCODE_DPAD_CENTER: {
                //默认拦截这些按键，根据onTouchEvent返回值判断是否处理
                return true;
            }
            case KeyEvent.KEYCODE_BACK: {
                return mPlayerUiController.isControllerRootViewShow();
            }
        }
        return false;
    }

    protected boolean checkHistoryPosition() {
        return false;
    }

    protected boolean onKeyEvent(KeyEvent event) {
        //处理接收了按键要重置隐藏controllerView的时间
        resetHideControllerTime();
        boolean match = false;
        //除了返回键 操作界面未显示先显示
        if (event.getKeyCode() != KeyEvent.KEYCODE_BACK && !mPlayerUiController.isControllerRootViewShow()) {
            mPlayerUiController.showController(true);
            if (event.getKeyCode() == KeyEvent.KEYCODE_DPAD_LEFT || event.getKeyCode() == KeyEvent.KEYCODE_DPAD_RIGHT) {
                mPlayerUiController.getSeekBarView().requestFocus();
            }
            match = true;
        }
        switch (event.getKeyCode()) {
            case KeyEvent.KEYCODE_DPAD_CENTER: {//确认键
                if (event.getAction() == KeyEvent.ACTION_DOWN && checkHistoryPosition()) {
                    Log.d(TAG, "receive keyEvent enter");
                    match = true;
                    break;
                }
                if (mPlayerUiController.getSeekBarView().hasFocus() || (!mPlayerUiController.isControllerRootViewShow() && mPlayer.isPlaying())) {//进度条没有焦点不能进行播放暂停操作
                    if (event.getAction() == KeyEvent.ACTION_UP) {
                        togglePlayer();
                    }
                    match = true;
                } else {
                    match = false;
                }
                break;
            }
            //向左seek
            case KeyEvent.KEYCODE_DPAD_LEFT:
                //向右seek
            case KeyEvent.KEYCODE_DPAD_RIGHT: {
                if (mPlayerUiController.getSeekBarView().hasFocus()) {//进度条没有焦点不能seek
                    seekUi(event);
                    match = true;
                } else {
                    match = false;
                }
                break;
            }
            case KeyEvent.KEYCODE_BACK: {
                if (mPlayerUiController.isControllerRootViewShow()) {
                    if (event.getAction() == KeyEvent.ACTION_UP) {
                        mPlayerUiController.showController(false);
                    }
                    match = true;
                } else {
                    match = false;
                }
                break;
            }
            case KeyEvent.KEYCODE_MEDIA_PLAY: {
                if (event.getAction() == KeyEvent.ACTION_UP) {
                    togglePlayer();
                }
                match = true;
                break;
            }
            case KeyEvent.KEYCODE_MEDIA_REWIND:
            case KeyEvent.KEYCODE_FORWARD: {
                seekUi(event);
                match = true;
                break;
            }
        }
        return match;
    }

    /**
     * 重置隐藏播放控制器的时间
     */
    protected void resetHideControllerTime() {
        hideControllerRunnable.setCancel(true);
        mHandler.removeCallbacks(hideControllerRunnable);

        hideControllerRunnable = new HideControllerRunnable();
        if (mPlayer.isPlaying()) {
            mHandler.postDelayed(hideControllerRunnable, CONTROLLER_HIDE_DELAY_TIME);
        }
    }

    private void seekUi(KeyEvent event) {
        mHandler.removeCallbacks(updateTimeRunnable);
        _seekForward = event.getKeyCode() == KeyEvent.KEYCODE_DPAD_RIGHT || event.getKeyCode() == KeyEvent.KEYCODE_FORWARD;
        seekUi();
        if (event.getAction() == KeyEvent.ACTION_UP) {
            seek(_seekForward, _seekPosition);
            _seekPosition = 0;
        }
    }


    private void seekUi() {
        _seekPosition += _seekOffset;
        long seek = _seekForward ? _playPosition + _seekPosition : _playPosition - _seekPosition;
        long currentPlayPosition = seek / 1000;
        long totalPlayTime = _duration / 1000;
        if (!_seekForward && currentPlayPosition < 0) {
            currentPlayPosition = 0;
        }
        if (_seekForward && currentPlayPosition > totalPlayTime) {
            currentPlayPosition = totalPlayTime;
        }
        float percent = totalPlayTime <= 0 ? 0 : ((float) currentPlayPosition) / totalPlayTime;
        mPlayerUiController.showController(true);
        mPlayerUiController.updatePlayTime(percent, currentPlayPosition, totalPlayTime);
    }

    @Override
    public void togglePlayer() {
        if (mPlayer.isPlaying()) {
            pause();
        } else {
            start();
        }
    }


    @Override
    public void play() {
        playByUrl(mPlayUrl);
    }

    /**
     *
     */
    @Override
    public void playByUrl(String playUrl) {
//        if(TextUtils.isEmpty(playUrl)){
//            AndroidLogWrapperUtil.e(TAG,"playByUrl url is null ");
//            return;
//        }
        mPlayUrl = playUrl;
        try {
            resetPlayer();
            mPlayer.setDataSource(playUrl);
            mPlayer.prepareAsync();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    protected void resetPlayer() {
        Log.d(TAG, "call resetPlayer");
        mPlayer.reset();
        mPlayerUiController.updatePlayTime(0, 0, 0);
        playUi();
    }

    @Override
    public void start() {
        Log.d(TAG, "call start");
        mPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
        mPlayer.setScreenOnWhilePlaying(true);
        mPlayer.start();
        startUi();
    }

    private void startUi() {
        //开始播放的时候关闭错误的ui显示
        showErrorUi(false, "");
//        mPlayerUiController.getSurfaceView().setVisibility(View.VISIBLE);
        setDisplayBackGround(Color.TRANSPARENT);
        mPlayerUiController.showBufferingView(false);
        updateTimeRunnable.setCancel(false);
        mHandler.post(updateTimeRunnable);
        mHandler.postDelayed(hideControllerRunnable, CONTROLLER_HIDE_DELAY_TIME);
        mPlayerUiController.onStart();
    }

    protected void setDisplayBackGround(@ColorInt int color) {
        View disPlayView = mPlayerUiController.getSurfaceView();
        if (!(disPlayView instanceof TextureView)) {
            disPlayView.setBackgroundColor(color);
        }
    }

    public void setPlaceHolderImage(Drawable drawable) {
        mPlayerUiController.showBufferingView(false);
        mPlayerUiController.showErrorMessageView(false);
        mPlayerUiController.setPlayerHolderImage(drawable);
    }

    @Override
    public void pause() {
        pauseUi();
        mPlayer.pause();
    }

    private void pauseUi() {
        mHandler.removeCallbacks(updateTimeRunnable);
        mPlayerUiController.showController(true);
        mPlayerUiController.onPause();
    }

    protected void showErrorUi(boolean show, String errorMessage) {
        mPlayerUiController.setErrorMessage(errorMessage);
        mPlayerUiController.showErrorMessageView(show);
        if (show) {
            //播放器出错不显示加载圈
            mPlayerUiController.showBufferingView(false);
        }
    }

    /**
     * 开始进行播放,还没有准备好播放数据
     */
    protected void playUi() {
        //数据还没有准备好，更新ui无效
        mHandler.removeCallbacks(updateTimeRunnable);
        mPlayerUiController.showController(false);
        mPlayerUiController.showBufferingView(true);
        showErrorUi(false, "");
        //在视屏切换设置dataSource的时候不能gone 可能会引起h265流硬件解码失效
//        mPlayerUiController.getSurfaceView().setVisibility(View.GONE);
        setDisplayBackGround(Color.BLACK);

    }

    @Override
    public void seek(boolean forward, long offsetMs) {
        if (offsetMs < 2000) {//偏移小于2s不进行seek操作
            return;
        }
        long seekPosition;
        _playPosition = mPlayer.getCurrentPosition();
        if (forward) {
            seekPosition = Math.min(mPlayer.getCurrentPosition() + offsetMs, mPlayer.getDuration()) - 50;
        } else {
            seekPosition = Math.max(5, mPlayer.getCurrentPosition() - offsetMs);
        }

        if (mPlayerUiController != null) {
            mPlayerUiController.onSeek(forward, seekPosition);
        }

        mPlayer.seekTo(seekPosition);
    }

    @Override
    public void destroy() {
        mHandler.removeCallbacks(updateTimeRunnable);
        mHandler.removeCallbacksAndMessages(null);
        mPlayer.setPlayerEventListener(null);
        clearDisplayView();
        mPlayer.reset();
        mPlayer.release();
    }

    private void clearDisplayView() {
        View playerView = mPlayerUiController.getSurfaceView();
        if (playerView instanceof SurfaceView) {
            SurfaceView surfaceView = (SurfaceView) playerView;
            mPlayer.clearVideoSurfaceView(surfaceView);
        } else if (playerView instanceof TextureView) {
            TextureView textureView = (TextureView) playerView;
            mPlayer.clearVideoTextureView(textureView);
        }
    }

    @Override
    public void playNext() {

    }

    @Override
    public void playPre() {

    }

    @Override
    public void onPrepared(IPlayer mp) {
        start();
    }

    @Override
    public void onCompletion(IPlayer mp) {
        updateTimeRunnable.setCancel(true);
        pauseUi();
        //因为移除了播放器的进度条更新，有可能出现进度条偏差 人为的更新为全部完成
        long totalPlayTime = _duration / 1000;
        mPlayerUiController.updatePlayTime(1, totalPlayTime, totalPlayTime);
    }

    @Override
    public void onBufferingUpdate(IPlayer mp, int percent) {
        mPlayerUiController.getSeekBarView().setSecondaryProgress(percent);
    }

    @Override
    public void onSeekComplete(IPlayer mp) {
        start();
    }

    @Override
    public void onVideoSizeChanged(IPlayer mp, int width, int height) {

    }

    @Override
    public boolean onError(IPlayer mp, int what, int extra, String message) {
        showErrorUi(true, message);
        return true;
    }

    @Override
    public boolean onInfo(IPlayer mp, int what, int extra) {
        //注意一下这个what参数，现在是用的是MediaPlayer的，其它的播放器要做对应的转换才不会出错
        switch (what) {
            case MediaPlayer.MEDIA_INFO_BUFFERING_START: {
                mPlayerUiController.showBufferingView(true);
                break;
            }
            case MediaPlayer.MEDIA_INFO_BUFFERING_END: {
                mPlayerUiController.showBufferingView(false);
                break;
            }
        }
        return true;
    }

    protected boolean handleMessage(Message msg) {
        return false;
    }

    @SuppressLint("HandlerLeak")
    private class UiHandler extends Handler {
        UiHandler(Context context) {
            super(context.getMainLooper());
        }

        @Override
        public void handleMessage(Message msg) {
            if (!AbsBasePlayerController.this.handleMessage(msg)) {
                super.handleMessage(msg);
            }
        }
    }

    private class HideControllerRunnable extends CancelRunnable {
        @Override
        protected void realRun() {
            Log.d(TAG, "hide player controller is play " + mPlayer.isPlaying());
            //没有播放不隐藏
            mPlayerUiController.showController(!mPlayer.isPlaying());
        }
    }

    @Override
    public void attachLifecycle(Lifecycle lifecycle) {
        lifecycle.addObserver(new BasicLifecycleObserver(this));
    }

    @Override
    public void lifecycleCreate() {
        Log.d(TAG, "lifecycleCreate");
    }

    @Override
    public void lifecycleStart() {
        Log.d(TAG, "lifecycleCreate");
    }

    @Override
    public void lifecycleResume() {
        Log.d(TAG, "lifecycleResume");
        if (mContext != null && callLifeCycleStop) {//mContext == null代表没有初始化
            callLifeCycleStop = false;
            start();
        }
    }

    @Override
    public void lifecyclePause() {
        Log.d(TAG, "lifecyclePause");
        if (mContext != null) {//mContext == null代表没有初始化
            callLifeCycleStop = true;
            pause();
        }
    }

    @Override
    public void lifecycleStop() {
        Log.d(TAG, "lifecycleStop");
    }

    @Override
    public void lifecycleDestroy() {
        Log.d(TAG, "lifecycleDestroy");
        if (mContext != null) {//mContext == null代表没有初始化
            destroy();
        }
    }

}
