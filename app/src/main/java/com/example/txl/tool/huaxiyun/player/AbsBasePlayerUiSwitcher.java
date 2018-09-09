package com.example.txl.tool.huaxiyun.player;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.support.constraint.ConstraintLayout;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;


public abstract class AbsBasePlayerUiSwitcher implements View.OnClickListener,TextureView.SurfaceTextureListener{
    protected Context context;
    protected ViewGroup parent;

    BasePlayerAdapter _adapter;

    protected FrameLayout rootFrameView;
    protected ConstraintLayout rootConstraintLayout;//自定义view构造播放控件，需要解决滑动冲突，
    protected TextureView playerPresenter;//播放内容的呈现者
    private TextureView.SurfaceTextureListener surfaceTextureListener;

    public AbsBasePlayerUiSwitcher(BasePlayerAdapter adapter, ViewGroup parent, Context context) {
        this.parent = parent;
        this.context = context;
        initView(context);
        _adapter = adapter;
    }

    abstract void initView(Context context);

    /**
     * Add child elements to the player ui
     * */
    public void addUi(View view){
        rootConstraintLayout.addView( view );
    }

    public void togglePlayPause() {
        AbsMediaPlayerController controller = _adapter.getController();
        controller.doTogglePlayPause();
    }

    public void showUi(int resId, boolean show){

    }

    /**
     * Change the parent container of the root layout
     * @param parent a new ViewGrout tobe the parent container of rootFrameView
     * */
    public void reload(ViewGroup parent){
        if(this.parent == null){
            parent.addView( rootFrameView );
        }else {
            this.parent.removeView( rootFrameView );
            parent.addView( rootFrameView );
        }
        this.parent = parent;
    }

    public void setSurfaceTextureListener(TextureView.SurfaceTextureListener listener){
        this.surfaceTextureListener = listener;
    }

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        if(surfaceTextureListener != null){
            surfaceTextureListener.onSurfaceTextureAvailable( surface,width,height );
        }
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        if(surfaceTextureListener != null){
            surfaceTextureListener.onSurfaceTextureSizeChanged( surface,width,height );
        }
    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        if(surfaceTextureListener != null){
            return surfaceTextureListener.onSurfaceTextureDestroyed( surface );
        }
        return false;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        if(surfaceTextureListener != null){
            surfaceTextureListener.onSurfaceTextureUpdated( surface );
        }
    }
}
