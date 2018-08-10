package com.example.txl.tool.gank.io.video;

import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.util.Log;
import android.view.Surface;
import android.view.View;
import android.view.ViewGroup;

import com.example.txl.tool.BaseActivity;
import com.example.txl.tool.R;
import com.example.txl.tool.player.SimpleAndroidPlayer;
import com.example.txl.tool.player.TextureAndroidPlayer;
import com.example.txl.tool.player.TextureVideoPlayerView;
import com.example.txl.tool.wiget.PageScrollerRecyclerView;

import java.util.List;

public class VideoActivity extends BaseActivity implements VideoContract.View{
    private static final String TAG = VideoActivity.class.getSimpleName();

    private View textureView;
    private SimpleAndroidPlayer player;
    private PageScrollerRecyclerView mRecyclerView;
    private VideoAdapter videoAdapter;
    private VideoPresenter videoPresenter;
    private TextureVideoPlayerView currentView = null;
    private int currentIndex = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_video );
        initView();
    }

    private void initView(){
        mRecyclerView = findViewById( R.id.video_activity_RecyclerView );
        LinearLayoutManager linearLayoutManager = new LinearLayoutManager( this);
        linearLayoutManager.setOrientation( LinearLayoutManager.VERTICAL  );
        mRecyclerView.setLayoutManager( linearLayoutManager );
        videoAdapter = new VideoAdapter();
        mRecyclerView.setAdapter( videoAdapter );
        mRecyclerView.setPageScrollListener( new PageScrollerRecyclerView.IViewPageScrollListener() {
            @Override
            public void onPageScroll(int targetPosition, View targetView) {
                currentIndex = targetPosition;
                currentView = (TextureVideoPlayerView) targetView;
                Log.d( TAG,"currentView  "+currentView +"  ");
                Log.d( TAG,"currentView.getTextureView() "+currentView.getTextureView()+"  ");
                player.stop();
                player.open( currentView.getTag().toString() );
                player.setMediaPlaerSurface( new Surface( currentView.getTextureView().getSurfaceTexture() ) );
                player.play();
            }
        } );
        videoPresenter = new VideoPresenter( this );
        videoPresenter.start();
        player = new SimpleAndroidPlayer( true,true );
        player.init( this,null );
    }

    @Override
    public void setPresenter(VideoContract.Presenter presenter) {

    }

    @Override
    public void refreshFinish(List<VideoBean.VideoInfo> videoInfoList) {
        videoAdapter.updateData( videoInfoList );
    }

    @Override
    public void loadMoreFinish(List<VideoBean.VideoInfo> videoInfoList) {
        videoAdapter.addData( videoInfoList );
    }
}
