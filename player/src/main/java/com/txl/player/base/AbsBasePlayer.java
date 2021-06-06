package com.txl.player.base;

public abstract class AbsBasePlayer implements IPlayer, VideoRenderComponent {
    protected final String TAG = getClass().getSimpleName();
    protected IPlayerEventListener mPlayerEventListener;
    protected boolean isRelease = false;

    protected OnRenderChangeListener renderChangeListener;

    @Override
    public VideoRenderComponent getVideoRenderComponent() {
        return this;
    }

    @Override
    public void setPlayerEventListener(IPlayerEventListener listener) {
        mPlayerEventListener = listener;
    }

    @Override
    public void setEnableMediaCodec(boolean isEnable) {

    }

    @Override
    public void setOnRenderChangeListener(OnRenderChangeListener renderChangeListener) {
        this.renderChangeListener = renderChangeListener;
    }

    @Override
    public boolean isRelease() {
        return isRelease;
    }

    @Override
    public void release() {
        isRelease = true;
    }
}
