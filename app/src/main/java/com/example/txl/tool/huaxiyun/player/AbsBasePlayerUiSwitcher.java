package com.example.txl.tool.huaxiyun.player;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.support.constraint.ConstraintLayout;
import android.util.Log;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import java.util.ArrayList;
import java.util.List;


public abstract class AbsBasePlayerUiSwitcher implements View.OnClickListener,TextureView.SurfaceTextureListener{
    private final String TAG = getClass().getSimpleName();

    protected Context context;
    protected ViewGroup parent;

    BasePlayerAdapter _adapter;

    protected FrameLayout rootFrameView;//自定义view构造播放控件，需要解决滑动冲突，
    protected ConstraintLayout rootConstraintLayout;
    protected TextureView playerPresenter;//播放内容的呈现者
    private List<TextureView.SurfaceTextureListener> surfaceTextureListeners;

    public AbsBasePlayerUiSwitcher(BasePlayerAdapter adapter) {
        _adapter = adapter;
    }

    public void initView(ViewGroup parent, Context context){
        this.context = context;
        this.parent = parent;
    }

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

    public void addSurfaceTextureListener(TextureView.SurfaceTextureListener listener){
        if(surfaceTextureListeners == null){
            surfaceTextureListeners = new ArrayList<>();
        }
        surfaceTextureListeners.add(listener);
    }

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        Log.d(TAG,"onSurfaceTextureAvailable");
        if(surfaceTextureListeners != null){
            for (TextureView.SurfaceTextureListener listener : surfaceTextureListeners){
                listener.onSurfaceTextureAvailable(surface,width,height);
            }
        }
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        Log.d(TAG,"onSurfaceTextureSizeChanged");
        if(surfaceTextureListeners != null){
            for (TextureView.SurfaceTextureListener listener : surfaceTextureListeners){
                listener.onSurfaceTextureSizeChanged( surface,width,height );
            }
        }
    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        Log.d(TAG,"onSurfaceTextureDestroyed");
        if(surfaceTextureListeners != null){
            for (TextureView.SurfaceTextureListener listener : surfaceTextureListeners){
                listener.onSurfaceTextureDestroyed( surface );
            }
            return true;
        }
        return false;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        if(surfaceTextureListeners != null){
            for (TextureView.SurfaceTextureListener listener : surfaceTextureListeners){
                listener.onSurfaceTextureUpdated( surface );
            }
        }
    }
}
