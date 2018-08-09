package com.example.txl.tool.gank.io.video;

import android.graphics.SurfaceTexture;
import android.media.MediaPlayer;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Surface;
import android.view.TextureView;

import com.example.txl.tool.R;

import java.io.IOException;

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
        textureView1 = findViewById( R.id.more_texture_activity_TextureView1 );
        textureView1.setSurfaceTextureListener( new TextureView.SurfaceTextureListener() {
            @Override
            public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {

                mediaPlayer.setSurface( new Surface( surface ) );
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

                        try {
                            mediaPlayer.reset();
                            mediaPlayer.setDataSource( MoreTextureActivity.this, Uri.parse("https://aweme.snssdk.com/aweme/v1/playwm/?video_id=v0200ff70000bdcrer2k781i9vhhqsvg&line=0") );
                            mediaPlayer.prepareAsync();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                } );
                try {
                    mediaPlayer.setDataSource( MoreTextureActivity.this, Uri.parse("https://aweme.snssdk.com/aweme/v1/playwm/?video_id=v0200ff50000bd0p9ur6936mllnbeo40&line=0") );
                    mediaPlayer.prepareAsync();
                } catch (IOException e) {
                    e.printStackTrace();
                }
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
        textureView2 = findViewById( R.id.more_texture_activity_TextureView2 );

    }

    @Override
    protected void onStop() {
        super.onStop();
        mediaPlayer.stop();
        mediaPlayer.release();
    }
}
