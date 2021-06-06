package com.txl.player.ui;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.SurfaceView;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsSeekBar;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.annotation.DrawableRes;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.constraintlayout.widget.ConstraintLayout;


import com.huaqiyun.player.R;
import com.txl.player.base.OnPlayerListener;
import com.txl.player.controller.IPlayerController;
import com.txl.player.controller.IPlayerUiController;
import com.txl.player.utils.PlayerUtils;
import com.txl.player.utils.PointSeekBar;
import com.txl.player.utils.TimeUtils;

import org.jetbrains.anko.ToastsKt;

import java.util.ArrayList;

/**
 * 参考google的Exo播放器的PlayerView将播放器的ui进行公告抽象
 * 为什么不直接使用google Exo播放器的PlayerView?  因为项目后期可能会有播放器内核变化（比如使用 IJK 、 MediaPlayer）进行播放，
 * Exo播放器的ui与播放器进行了强关联播放器切换不是太方便。因此参考Exo播放器的ui进行重写。
 */
public class PlayerView extends FrameLayout implements IPlayerUiController {
    private static final String TAG = "PlayerView";

    /**
     * 默认全屏
     */
    private int screen = SCREEN_FULL;

    public static int SCREEN_SMALL = 0;
    public static int SCREEN_FULL = 1;

    private static final int SURFACE_TYPE_NONE = 0;
    private static final int SURFACE_TYPE_SURFACE_VIEW = 1;
    private static final int SURFACE_TYPE_TEXTURE_VIEW = 2;

    private boolean _playerControllerEnable = true;

    /**
     * 播放器逻辑控制器
     */
    protected IPlayerController mPlayerController;

    /**
     * 播放监听
     */
    private OnPlayerListener onPlayerListener;

    /**
     * 渲染视频View的父容器方便控制显示比例
     */
    @Nullable
    private final FrameLayout contentFrame;

    /**
     * 渲染视频的那个View
     */
    @Nullable
    private final View surfaceView;

    @Nullable
    private final FrameLayout controllerRootView;

    /**
     * 播放暂停状态切换
     */
    private final ImageView imageTogglePlayer;
    /**
     * 进度条
     */
    private final PointSeekBar seekBar;

    /**
     * 当前的播放时间
     */
    private final TextView tvPlayTime;

    /**
     * 视频/音频总时长
     */
    private final TextView tvPlayLongTime;

    /**
     * 播放出错消息提示
     */
    private TextView tvErrorMessageView;

    /**
     * 视频title
     */
    private TextView tvTitle;

    /**
     * 占位图
     */
    private ImageView imagePlaceHolder;

    private ProgressBar progressBarBuffering;

    private ViewBorder viewBorder = new ViewBorder();

    private ViewGroup.LayoutParams oldLayoutParams;
    private ViewGroup oldParent;

    public PlayerView(@NonNull Context context) {
        this(context, null);
    }

    public PlayerView(@NonNull Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public PlayerView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        int surfaceType = SURFACE_TYPE_SURFACE_VIEW;
        //跟布局不能重新加载
        int playerLayoutId = R.layout.default_player_controller_view;
        if (attrs != null) {
            TypedArray a = context.getTheme().obtainStyledAttributes(attrs, R.styleable.PlayerView, 0, 0);
            try {
                surfaceType = a.getInt(R.styleable.PlayerView_surface_type, surfaceType);
                playerLayoutId = a.getResourceId(R.styleable.PlayerView_player_controller_layout_id, R.layout.default_player_controller_view);
                viewBorder.drawBorder = a.getBoolean(R.styleable.PlayerView_hasFocusBorder, false);
                viewBorder.setBorderWidth(a.getDimensionPixelSize(R.styleable.PlayerView_selectBorderWidth, 10));
                viewBorder.setBorderColor(a.getColor(R.styleable.PlayerView_borderColor, Color.WHITE));
            } finally {
                a.recycle();
            }
        }
        LayoutInflater.from(context).inflate(R.layout.default_player_view, this);
        LayoutInflater.from(context).inflate(playerLayoutId, this);
        contentFrame = findViewById(R.id.player_content_frame);
        if (contentFrame != null && surfaceType != SURFACE_TYPE_NONE) {
            switch (surfaceType) {
                case SURFACE_TYPE_TEXTURE_VIEW: {
                    surfaceView = new TextureView(context);
                    break;
                }
                default: {
                    surfaceView = new SurfaceView(context);
                    break;
                }
            }
            ViewGroup.LayoutParams params = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
            surfaceView.setLayoutParams(params);
            contentFrame.addView(surfaceView, 0);
            tvErrorMessageView = contentFrame.findViewById(R.id.player_error_message);
            imagePlaceHolder = contentFrame.findViewById(R.id.player_place_holder_image);
            progressBarBuffering = contentFrame.findViewById(R.id.player_buffering);
        } else {
            surfaceView = null;
        }
        controllerRootView = findViewById(R.id.player_controller);
        tvTitle = findViewById(R.id.tv_player_title);
        if (controllerRootView != null) {
            controllerRootView.setVisibility(View.GONE);
            imageTogglePlayer = controllerRootView.findViewById(R.id.image_toggle_player);
            seekBar = controllerRootView.findViewById(R.id.player_seek_bar);
            tvPlayTime = controllerRootView.findViewById(R.id.player_play_time);
            tvPlayLongTime = controllerRootView.findViewById(R.id.player_video_long_time);
        } else {
            throw new RuntimeException("must container a FrameLayout is is player_controller");
        }
        setWillNotDraw(false);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        if (isFocused()) {//這個玩意有bug 在没有padding的时候不能成功绘制,或者说是被surfaceView填充无法显示绘制的内容？
            viewBorder.drawableBorder(canvas);
        }
    }

    /**
     * 返回渲染视频的那个View
     */
    @Nullable
    @Override
    public View getSurfaceView() {
        return surfaceView;
    }

    @Override
    public void setPlayerController(IPlayerController playerController) {
        mPlayerController = playerController;
    }

    @Nullable
    @Override
    public FrameLayout getControllerRootView() {
        return controllerRootView;
    }

    @Override
    public boolean isControllerRootViewShow() {
        boolean s = controllerRootView != null && controllerRootView.getVisibility() == VISIBLE;
        Log.d(TAG, "isControllerRootViewShow show :: " + s);
        return s;
    }

    @Override
    public void showErrorMessageView(boolean show) {
        if (tvErrorMessageView == null) {
            return;
        }
        tvErrorMessageView.setVisibility(show ? View.VISIBLE : View.GONE);
    }

    @Override
    public void setErrorMessage(String message) {
        if (tvErrorMessageView == null) {
            return;
        }
        tvErrorMessageView.setText(message);
    }

    @Override
    public void showBufferingView(boolean show) {
        if (progressBarBuffering == null) {
            return;
        }
        progressBarBuffering.setVisibility(show ? View.VISIBLE : View.GONE);
    }

    @Override
    public void showPlaceHolder(boolean show) {
        if (imagePlaceHolder == null) {
            return;
        }
        imagePlaceHolder.setVisibility(show ? View.VISIBLE : View.GONE);
    }

    @Override
    public void showController(boolean show) {
        if (controllerRootView == null) {
            return;
        }
        if ((show && controllerRootView.getVisibility() == VISIBLE) || (!show && controllerRootView.getVisibility() == GONE)) {
            return;
        }
        controllerRootView.setVisibility((show && _playerControllerEnable) ? View.VISIBLE : View.GONE);
    }

    /**
     * 日志查看布局参数是否正常
     */
    void searchLayoutParams(View root) {
        if (root instanceof ViewGroup) {
            int childCount = ((ViewGroup) root).getChildCount();
            for (int i = 0; i < childCount; i++) {
                View child = ((ViewGroup) root).getChildAt(i);
                searchLayoutParams(child);
                Log.e(TAG, "查看LayoutParams parent name :: " + child.getParent().getClass().getCanonicalName() + " ------------ child layout params:: " + child.getLayoutParams().getClass().getCanonicalName());
            }
        }
        Log.e(TAG, "查看LayoutParams parent name :: " + root.getParent().getClass().getCanonicalName() + " ------------ child layout params:: " + root.getLayoutParams().getClass().getCanonicalName());
    }

    @Override
    public void updatePlayTime(float percent, long currentPlayPosition, long totalTime) {
        String currentPlayTime = currentPlayPosition <= 0 ? "" : TimeUtils.convertTimeFormat((int) currentPlayPosition, true);
        String totalPlayTime = totalTime <= 0 ? "" : TimeUtils.convertTimeFormat((int) totalTime, true);

        ViewGroup.LayoutParams layoutParams = tvPlayTime.getLayoutParams();
        if (layoutParams instanceof ConstraintLayout.LayoutParams) {
            ConstraintLayout.LayoutParams constraintLayoutParams = (ConstraintLayout.LayoutParams) layoutParams;
            constraintLayoutParams.horizontalBias = percent;
            tvPlayTime.setLayoutParams(constraintLayoutParams);
        }
        seekBar.setProgress((int) (percent * 100));
        tvPlayTime.setText(currentPlayTime);
        tvPlayLongTime.setText(totalPlayTime);

        if (onPlayerListener != null) {
            onPlayerListener.onVideoPlaying(percent, currentPlayPosition, totalTime);
        }
    }

    @Override
    public AbsSeekBar getSeekBarView() {
        return seekBar;
    }

    @Override
    public void onStart() {
        imageTogglePlayer.setImageResource(R.drawable.icon_pause);
        showPlaceHolder(false);
//        postDelayed(new Runnable() {
//            @Override
//            public void run() {
//
//            }
//        },100);

        if (onPlayerListener != null) {
            onPlayerListener.onVideoStart();
        }
    }

    @Override
    public void onPause() {
        imageTogglePlayer.setImageResource(R.drawable.icon_play);
        if (onPlayerListener != null) {
            onPlayerListener.onVideoPause();
        }
    }

    @Override
    public void onSeek(boolean forward, long seekPosition) {
        if (onPlayerListener != null) {
            onPlayerListener.onSeek(forward, seekPosition);
        }
    }

    @Override
    public void setPlayerHolderImage(@DrawableRes int res) {
        if (imagePlaceHolder != null) {
            imagePlaceHolder.setImageResource(res);
        }
    }

    @Override
    public void setPlayerHolderImage(Drawable drawable) {
        if (imagePlaceHolder != null) {
            imagePlaceHolder.setImageDrawable(drawable);
        }
    }

    @Override
    public ImageView getPlayerHolderImageView() {
        return imagePlaceHolder;
    }

    @Override
    public void setTitle(String title) {
        tvTitle.setText(title);
    }

    @Override
    public void controllerEnable(boolean enable) {
        if (_playerControllerEnable == enable) {
            return;
        }
        _playerControllerEnable = enable;
        if (!enable) {
            showController(false);
        }
    }

    public void setBorderColor(int color) {
        viewBorder.setBorderColor(color);
    }

    public void setBorderWidth(int width) {
        viewBorder.setBorderWidth(width);
    }

    public void setHasSelectBorder(boolean hasFocusBorder) {
        viewBorder.drawBorder = hasFocusBorder;
    }

    private class ViewBorder {
        boolean drawBorder = false;
        int borderWidth = 5;
        int borderColor = Color.WHITE;
        Paint paint = new Paint();

        ViewBorder() {
            paint.setStyle(Paint.Style.STROKE);
            paint.setAntiAlias(true);
            paint.setStrokeWidth(borderWidth);
            paint.setColor(borderColor);
        }

        void drawableBorder(Canvas canvas) {
            Log.d(TAG, "drawableBorder  " + drawBorder);
            if (drawBorder) {
                Log.d(TAG, "drawableBorder");
                canvas.drawRect(0f, 0f, getWidth(), getHeight(), paint);
            }
        }

        void setBorderWidth(int borderWidth) {
            this.borderWidth = borderWidth;
            paint.setStrokeWidth(borderWidth);
        }

        void setBorderColor(int borderColor) {
            this.borderColor = borderColor;
            paint.setColor(borderColor);
        }
    }

    public void toggleFullScreen() {
        if (screen == SCREEN_FULL) {
            gotoSmallScreen();
        } else {
            gotoFullScreen();
        }
    }

    public void gotoFullScreen() {
        oldParent = (ViewGroup) getParent();
        oldLayoutParams = getLayoutParams();
        oldParent.removeView(this);
        ViewGroup decorView = (ViewGroup) PlayerUtils.INSTANCE.scanForActivity(getContext()).getWindow().getDecorView();
        decorView.addView(this, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
        screen = SCREEN_FULL;
        controllerEnable(true);
        requestFocus();
    }

    public void gotoSmallScreen() {
        ViewGroup decorView = (ViewGroup) PlayerUtils.INSTANCE.scanForActivity(getContext()).getWindow().getDecorView();
        decorView.removeView(this);
        oldParent.addView(this, oldLayoutParams);
        screen = SCREEN_SMALL;
        controllerEnable(false);
    }

    public void setScreen(int screen) {
        this.screen = screen;
    }

    public boolean isFullScreen() {
        return screen == SCREEN_FULL;
    }

    @Override
    public void addFocusables(ArrayList<View> views, int direction, int focusableMode) {
        if (screen == SCREEN_FULL) {
            super.addFocusables(views, direction, focusableMode);
        }
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        boolean handle = false;
        if (event.getKeyCode() == KeyEvent.KEYCODE_BACK && isFullScreen()) {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                backPress();
            }
            handle = true;
        }
        if (!handle) {
            handle = super.dispatchKeyEvent(event);
        }
        return handle;
    }

    private long lastBackPressTime = 0;

    /**
     * @return true goto small screen
     */
    public boolean backPress() {
        if (!isFullScreen()) {//不是全屏不处理返回按键
            return false;
        }
        long temp = System.currentTimeMillis();
        if (temp - lastBackPressTime > 3000) {
            ToastsKt.toast(getContext(), "再按一次退出全屏!");
            lastBackPressTime = temp;
            return false;
        } else {
            gotoSmallScreen();
            return true;
        }
    }

    public void setOnPlayerListener(OnPlayerListener l) {
        this.onPlayerListener = l;
    }
}
