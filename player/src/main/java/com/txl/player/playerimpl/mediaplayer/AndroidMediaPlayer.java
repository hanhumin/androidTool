package com.txl.player.playerimpl.mediaplayer;

import android.content.Context;
import android.graphics.PixelFormat;
import android.graphics.SurfaceTexture;
import android.media.MediaPlayer;
import android.net.Uri;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.TextureView;
import android.view.View;

import androidx.annotation.Nullable;

import com.txl.player.base.AbsBasePlayer;

import java.io.FileDescriptor;
import java.io.IOException;
import java.util.Map;

public class AndroidMediaPlayer extends AbsBasePlayer {
    private MediaPlayer _mediaPlayer;
    private Context _contex;
    private SurfaceHolder _surfaceHolder;
    private SurfaceHolder.Callback _callback = new SurfaceHolder.Callback() {
        @Override
        public void surfaceCreated(SurfaceHolder holder) {
            Log.d(TAG, "surfaceCreated");
            _mediaPlayer.setDisplay(holder);
            if (renderChangeListener != null) {
                renderChangeListener.surfaceCreated(holder);
            }
        }

        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            Log.d(TAG, "surfaceChanged");
            _mediaPlayer.setDisplay(holder);
            if (renderChangeListener != null) {
                renderChangeListener.surfaceChanged(holder, format, width, height);
            }
        }

        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {
            Log.d(TAG, "surfaceDestroyed");
            try {
                _mediaPlayer.setSurface(null);
            } catch (Exception e) {
                e.printStackTrace();
            }
            if (renderChangeListener != null) {
                renderChangeListener.surfaceDestroyed(holder);
            }
        }
    };
    private TextureView.SurfaceTextureListener surfaceTextureListener = new TextureView.SurfaceTextureListener() {
        @Override
        public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
            Log.d(TAG, "onSurfaceTextureAvailable  width = " + width + " height = " + height);
            _mediaPlayer.setSurface(new Surface(surface));
        }

        @Override
        public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
            Log.d(TAG, "onSurfaceTextureSizeChanged  width = " + width + " height = " + height);
            _mediaPlayer.setSurface(new Surface(surface));
        }

        @Override
        public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
            Log.d(TAG, "onSurfaceTextureSizeChanged");
//            _mediaPlayer.setSurface(null);
            return false;
        }

        @Override
        public void onSurfaceTextureUpdated(SurfaceTexture surface) {
            Log.d(TAG, "onSurfaceTextureUpdated");
//            _mediaPlayer.setSurface(new Surface(surface));
        }
    };
    private View _displayView;

    public AndroidMediaPlayer(Context contex) {
        this._contex = contex;
        _mediaPlayer = new MediaPlayer();
        _mediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                if (mPlayerEventListener != null) {
                    mPlayerEventListener.onPrepared(AndroidMediaPlayer.this);
                }
            }
        });
        _mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                if (mPlayerEventListener != null) {
                    mPlayerEventListener.onCompletion(AndroidMediaPlayer.this);
                }
            }
        });
        _mediaPlayer.setOnBufferingUpdateListener(new MediaPlayer.OnBufferingUpdateListener() {
            @Override
            public void onBufferingUpdate(MediaPlayer mp, int percent) {
                if (mPlayerEventListener != null) {
                    mPlayerEventListener.onBufferingUpdate(AndroidMediaPlayer.this, percent);
                }
            }
        });
        _mediaPlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mp, int what, int extra) {
                if (mPlayerEventListener != null) {
                    return mPlayerEventListener.onError(AndroidMediaPlayer.this, what, extra, "播放器加载出错！");
                }
                return false;
            }
        });

        _mediaPlayer.setOnSeekCompleteListener(new MediaPlayer.OnSeekCompleteListener() {
            @Override
            public void onSeekComplete(MediaPlayer mp) {
                if (mPlayerEventListener != null) {
                    mPlayerEventListener.onSeekComplete(AndroidMediaPlayer.this);
                }
            }
        });
        _mediaPlayer.setOnInfoListener(new MediaPlayer.OnInfoListener() {
            @Override
            public boolean onInfo(MediaPlayer mp, int what, int extra) {
                if (mPlayerEventListener != null) {
                    return mPlayerEventListener.onInfo(AndroidMediaPlayer.this, what, extra);
                }
                return false;
            }
        });
        _mediaPlayer.setOnVideoSizeChangedListener(new MediaPlayer.OnVideoSizeChangedListener() {
            @Override
            public void onVideoSizeChanged(MediaPlayer mp, int width, int height) {
                if (mPlayerEventListener != null) {
                    mPlayerEventListener.onVideoSizeChanged(AndroidMediaPlayer.this, width, height);
                }
            }
        });
    }

    @Override
    public void setDataSource(Context context, Uri uri) throws IOException, IllegalArgumentException, SecurityException, IllegalStateException {
        _mediaPlayer.setDataSource(context, uri);
    }

    @Override
    public void setDataSource(Context context, Uri uri, Map<String, String> headers) throws IOException, IllegalArgumentException, SecurityException, IllegalStateException {
        _mediaPlayer.setDataSource(context, uri, headers);
    }

    @Override
    public void setDataSource(FileDescriptor fd) throws IOException, IllegalArgumentException, IllegalStateException {
        _mediaPlayer.setDataSource(fd);
    }

    @Override
    public void setDataSource(String path) throws IOException, IllegalArgumentException, SecurityException, IllegalStateException {
        _mediaPlayer.setDataSource(path);
    }

    @Override
    public String getDataSource() {
        throw new RuntimeException("not imp");
    }

    @Override
    public void prepareAsync() throws IllegalStateException {
        _mediaPlayer.prepareAsync();
    }

    @Override
    public void start() throws IllegalStateException {
        _mediaPlayer.start();
    }

    @Override
    public void stop() throws IllegalStateException {
        _mediaPlayer.stop();
    }

    @Override
    public void pause() throws IllegalStateException {
        _mediaPlayer.pause();
    }

    @Override
    public void setScreenOnWhilePlaying(boolean screenOn) {
        _mediaPlayer.setScreenOnWhilePlaying(screenOn);
    }

    @Override
    public int getVideoWidth() {
        return _mediaPlayer.getVideoWidth();
    }

    @Override
    public int getVideoHeight() {
        return _mediaPlayer.getVideoWidth();
    }

    @Override
    public boolean isPlaying() {
        return _mediaPlayer.isPlaying();
    }

    @Override
    public void seekTo(long msec) throws IllegalStateException {
        _mediaPlayer.seekTo((int) msec);
    }

    @Override
    public long getCurrentPosition() {
        return _mediaPlayer.getCurrentPosition();
    }

    @Override
    public long getDuration() {
        return _mediaPlayer.getDuration();
    }

    @Override
    public void release() {
        reset();
        MediaPlayer mp = _mediaPlayer;
        _mediaPlayer = null;
        clearVideoSurfaceHolder(_surfaceHolder);
        mp.setDisplay(null);
        mp.release();
        super.release();
    }

    @Override
    public void reset() {
        _mediaPlayer.reset();
    }

    @Override
    public void setVolume(float leftVolume, float rightVolume) {
        _mediaPlayer.setVolume(leftVolume, rightVolume);
    }

    @Override
    public int getAudioSessionId() {
        return _mediaPlayer.getAudioSessionId();
    }

    @Override
    public void setLooping(boolean looping) {
        _mediaPlayer.setLooping(looping);
    }

    @Override
    public boolean isLooping() {
        return _mediaPlayer.isLooping();
    }

    @Override
    public void setAudioStreamType(int streamtype) {
        _mediaPlayer.setAudioStreamType(streamtype);
    }

    @Override
    public void clearVideoSurface() {
        throw new RuntimeException("not impl");
    }

    @Override
    public void clearVideoSurface(Surface surface) {
        throw new RuntimeException("not impl");
    }

    @Override
    public void setVideoSurface(@Nullable Surface surface) {
        if (surface != null && surface.isValid()) {
            _mediaPlayer.setSurface(surface);
        } else {
            throw new RuntimeException("surface is null or invalid");
        }

    }

    @Override
    public void setVideoSurfaceHolder(SurfaceHolder surfaceHolder) {
        clearVideoSurfaceHolder(_surfaceHolder);
        _surfaceHolder = surfaceHolder;
        surfaceHolder.setFormat(PixelFormat.TRANSPARENT);
        if (surfaceHolder.getSurface().isValid()) {
            _mediaPlayer.setDisplay(surfaceHolder);
        }
        surfaceHolder.addCallback(_callback);
    }

    @Override
    public void clearVideoSurfaceHolder(SurfaceHolder surfaceHolder) {
        if (_surfaceHolder != null && _surfaceHolder == surfaceHolder) {
            _surfaceHolder.removeCallback(_callback);
            _surfaceHolder = null;
        }
    }

    @Override
    public void setVideoSurfaceView(SurfaceView surfaceView) {
        if (surfaceView != null) {
            _displayView = surfaceView;
            surfaceView.setZOrderMediaOverlay(true);
            setVideoSurfaceHolder(surfaceView.getHolder());
        }
    }

    @Override
    public void clearVideoSurfaceView(SurfaceView surfaceView) {
        if (_displayView != null && _displayView == surfaceView) {
            if (surfaceView.getHolder() == _surfaceHolder) {
                _surfaceHolder.removeCallback(_callback);
                _surfaceHolder = null;
            }
            _displayView = null;
        }
    }

    @Override
    public void setVideoTextureView(TextureView textureView) {
        if (_displayView instanceof TextureView) {
            clearVideoTextureView((TextureView) _displayView);
        }
        if (textureView != null) {
            _displayView = textureView;
            if (textureView.isAvailable()) {
                _mediaPlayer.setSurface(new Surface(textureView.getSurfaceTexture()));
            }
            textureView.setSurfaceTextureListener(surfaceTextureListener);
        }

    }

    @Override
    public void clearVideoTextureView(TextureView textureView) {
        if (_displayView != null && _displayView == textureView) {
            _displayView = null;
            _mediaPlayer.setSurface(null);
            textureView.setSurfaceTextureListener(null);
        }
    }
}
