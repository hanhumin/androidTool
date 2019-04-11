package com.example.txl.tool.gank.io.video;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.graphics.SurfaceTexture;
import android.media.MediaPlayer;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.example.txl.tool.R;

import java.io.IOException;
import java.util.Map;

/**
 * 测试多个TextureView同时使用的情况
 * */
public class MoreTextureActivity extends AppCompatActivity {

    private TextureView textureView1, textureView2;
    private MediaPlayer mediaPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_more_texture );
        mediaPlayer = new MediaPlayer();
        mediaPlayer.setLooping(true);
        textureView1 = findViewById( R.id.more_texture_activity_TextureView1 );
        textureView1.setSurfaceTextureListener( new TextureView.SurfaceTextureListener() {
            @Override
            public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {

                mediaPlayer.setSurface( new Surface( surface ) );
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
        } );
        try {
            mediaPlayer.setOnPreparedListener( new MediaPlayer.OnPreparedListener() {
                @Override
                public void onPrepared(MediaPlayer mp) {
                    mp.start();
                }
            } );
            mediaPlayer.setOnCompletionListener( new MediaPlayer.OnCompletionListener() {
                @Override
                public void onCompletion(MediaPlayer mp) {
                    mediaPlayer.setSurface( new Surface( textureView2.getSurfaceTexture() ) );

//                        try {
//                            mediaPlayer.reset();
//                            mediaPlayer.setDataSource( MoreTextureActivity.this, Uri.parse("https://aweme.snssdk.com/aweme/v1/playwm/?video_id=v0200ff70000bdcrer2k781i9vhhqsvg&line=0") );
//                            mediaPlayer.prepareAsync();
//                        } catch (IOException e) {
//                            e.printStackTrace();
//                        }
                }
            } );
            mediaPlayer.setDataSource( MoreTextureActivity.this, Uri.parse("https://aweme.snssdk.com/aweme/v1/playwm/?video_id=v0200ff50000bd0p9ur6936mllnbeo40&line=0") );
            mediaPlayer.prepareAsync();
        } catch (IOException e) {
            e.printStackTrace();
        }
        textureView2 = findViewById( R.id.more_texture_activity_TextureView2 );
        TextView tvPlay1 = findViewById(R.id.tv_play_1);
        tvPlay1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);//横屏
                ViewGroup viewGroup = (ViewGroup) textureView1.getParent();
                viewGroup.removeAllViews();
                LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
                textureView1.setLayoutParams(params);
                viewGroup.addView(textureView1);
            }
        });
        TextView tvPlay2 = findViewById(R.id.tv_play_2);
        tvPlay2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mediaPlayer.setSurface( new Surface( textureView2.getSurfaceTexture() ) );
                textureView2.setVisibility(View.VISIBLE);
                textureView1.setVisibility(View.INVISIBLE);
            }
        });
    }

    @Override
    protected void onStop() {
        super.onStop();
        mediaPlayer.stop();
        mediaPlayer.release();
    }
}
