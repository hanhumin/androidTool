package com.txl.player.base;

import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.TextureView;

import androidx.annotation.Nullable;

/**
 * 参考exo播放器将播放器的显示封装在一个Component中
 * */
public interface VideoRenderComponent {
    /**
     * Clears any {@link Surface}, {@link SurfaceHolder}, {@link SurfaceView} or {@link TextureView}
     * currently set on the player.
     */
    void clearVideoSurface();

    /**
     * Clears the {@link Surface} onto which video is being rendered if it matches the one passed.
     * Else does nothing.
     *
     * @param surface The surface to clear.
     */
    void clearVideoSurface(Surface surface);

    /**
     * Sets the {@link Surface} onto which video will be rendered. The caller is responsible for
     * tracking the lifecycle of the surface, and must clear the surface by calling {@code
     * setVideoSurface(null)} if the surface is destroyed.
     *
     * <p>If the surface is held by a {@link SurfaceView}, {@link TextureView} or {@link
     * SurfaceHolder} then it's recommended to use {@link #setVideoSurfaceView(SurfaceView)}, {@link
     * #setVideoTextureView(TextureView)} or {@link #setVideoSurfaceHolder(SurfaceHolder)} rather
     * than this method, since passing the holder allows the player to track the lifecycle of the
     * surface automatically.
     *
     * @param surface The {@link Surface}.
     */
    void setVideoSurface(@Nullable Surface surface);

    /**
     * Sets the {@link SurfaceHolder} that holds the {@link Surface} onto which video will be
     * rendered. The player will track the lifecycle of the surface automatically.
     *
     * @param surfaceHolder The surface holder.
     */
    void setVideoSurfaceHolder(SurfaceHolder surfaceHolder);

    /**
     * Clears the {@link SurfaceHolder} that holds the {@link Surface} onto which video is being
     * rendered if it matches the one passed. Else does nothing.
     *
     * @param surfaceHolder The surface holder to clear.
     */
    void clearVideoSurfaceHolder(SurfaceHolder surfaceHolder);

    /**
     * Sets the {@link SurfaceView} onto which video will be rendered. The player will track the
     * lifecycle of the surface automatically.
     *
     * @param surfaceView The surface view.
     */
    void setVideoSurfaceView(SurfaceView surfaceView);

    /**
     * Clears the {@link SurfaceView} onto which video is being rendered if it matches the one
     * passed. Else does nothing.
     *
     * @param surfaceView The texture view to clear.
     */
    void clearVideoSurfaceView(SurfaceView surfaceView);

    /**
     * Sets the {@link TextureView} onto which video will be rendered. The player will track the
     * lifecycle of the surface automatically.
     *
     * @param textureView The texture view.
     */
    void setVideoTextureView(TextureView textureView);

    /**
     * Clears the {@link TextureView} onto which video is being rendered if it matches the one
     * passed. Else does nothing.
     *
     * @param textureView The texture view to clear.
     */
    void clearVideoTextureView(TextureView textureView);

    interface OnRenderChangeListener extends TextureView.SurfaceTextureListener, SurfaceHolder.Callback{}
}
