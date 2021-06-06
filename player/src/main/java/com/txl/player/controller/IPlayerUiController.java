package com.txl.player.controller;

import android.graphics.drawable.Drawable;
import android.view.View;
import android.widget.AbsSeekBar;
import android.widget.FrameLayout;
import android.widget.ImageView;

import androidx.annotation.DrawableRes;

/**
 * 播放器ui的逻辑控制
 */
public interface IPlayerUiController {
    /**
     * 获取视频渲染的View
     * TextureView或者SurfaceView
     */
    View getSurfaceView();

    void setPlayerController(IPlayerController playerController);

    FrameLayout getControllerRootView();

    boolean isControllerRootViewShow();

    void showErrorMessageView(boolean show);

    void setErrorMessage(String message);

    void showBufferingView(boolean show);

    void showPlaceHolder(boolean show);

    void showController(boolean show);

    /**
     * @param percent             进度百分比
     * @param currentPlayPosition 当前的播放位置
     * @param totalPlayTime       总的播放时长
     */
    void updatePlayTime(float percent, long currentPlayPosition, long totalPlayTime);

    AbsSeekBar getSeekBarView();

    void onStart();

    void onPause();

    /**
     * 快退或快进到指定位置
     *
     * @param forward      true:快进，false:快退
     * @param seekPosition 指定的位置
     */
    void onSeek(boolean forward, long seekPosition);

    /**
     * 设置播放器占位图
     */
    void setPlayerHolderImage(@DrawableRes int res);

    void setPlayerHolderImage(Drawable drawable);

    ImageView getPlayerHolderImageView();

    void setTitle(String title);

    /**
     * @param enable 设置播放器controller是否可以 true 可用 false 不可用
     */
    void controllerEnable(boolean enable);
}
