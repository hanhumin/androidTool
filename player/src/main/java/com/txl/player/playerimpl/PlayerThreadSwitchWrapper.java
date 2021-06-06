package com.txl.player.playerimpl;

import android.content.Context;
import android.net.Uri;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.TextureView;

import androidx.annotation.Nullable;

import com.txl.player.base.IPlayer;
import com.txl.player.base.IPlayerEventListener;
import com.txl.player.base.VideoRenderComponent;

import java.io.FileDescriptor;
import java.io.IOException;
import java.util.Map;

/**
 * Copyright (c) 2020 唐小陆 All rights reserved.
 * author：txl
 * date：2020/6/17
 * description：将某些耗时操作移动到子线程中来进行,为播放器添加线程切换的功能
 */
public class PlayerThreadSwitchWrapper implements IPlayer, IPlayerEventListener {
    private static final String TAG = PlayerThreadSwitchWrapper.class.getSimpleName();

    private IPlayer mPlayer;

    private HandlerThread mHandlerThread;
    /**
     * 子线程handler
     */
    private Handler mMediaHandler;
    /**
     * 主线程handler
     */
    private Handler mMainHandler;

    private IPlayerEventListener mPlayerEventListener;

    public PlayerThreadSwitchWrapper(IPlayer mPlayer) {
        this.mPlayer = mPlayer;
        mMainHandler = new Handler(Looper.getMainLooper());
        mHandlerThread = new PlayerHandlerThread("ThreadSwitchPlayer::" + mPlayer);
        mHandlerThread.start();
        mPlayer.setPlayerEventListener(this);
    }

    private Handler getMediaHandler() {
        return mMediaHandler != null ? mMediaHandler : mMainHandler;
    }


    @Override
    public VideoRenderComponent getVideoRenderComponent() {
        return mPlayer.getVideoRenderComponent();
    }

    @Override
    public void setDataSource(final Context context, final Uri uri) throws IOException, IllegalArgumentException, SecurityException, IllegalStateException {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                try {
                    mPlayer.setDataSource(context, uri);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    @Override
    public void setDataSource(final Context context, final Uri uri, final Map<String, String> headers) throws IOException, IllegalArgumentException, SecurityException, IllegalStateException {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                try {
                    mPlayer.setDataSource(context, uri, headers);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    @Override
    public void setDataSource(final FileDescriptor fd) throws IOException, IllegalArgumentException, IllegalStateException {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                try {
                    mPlayer.setDataSource(fd);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    @Override
    public void setDataSource(final String path) throws IOException, IllegalArgumentException, SecurityException, IllegalStateException {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                try {
                    mPlayer.setDataSource(path);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    @Override
    public String getDataSource() {
        return mPlayer.getDataSource();
    }

    @Override
    public void prepareAsync() throws IllegalStateException {
        //这里使用handler是为了让 prepareAsync 从消息队列中依次执行
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.prepareAsync();
            }
        });
    }

    @Override
    public void start() throws IllegalStateException {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.start();
            }
        });
    }

    @Override
    public void stop() throws IllegalStateException {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.stop();
            }
        });
    }

    @Override
    public void pause() throws IllegalStateException {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.pause();
            }
        });
    }

    @Override
    public void setScreenOnWhilePlaying(final boolean screenOn) {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.setScreenOnWhilePlaying(screenOn);
            }
        });
    }

    @Override
    public int getVideoWidth() {
        return mPlayer.getVideoWidth();
    }

    @Override
    public int getVideoHeight() {
        return mPlayer.getVideoHeight();
    }

    @Override
    public boolean isPlaying() {
        return mPlayer.isPlaying();
    }

    @Override
    public void seekTo(final long msec) throws IllegalStateException {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.seekTo(msec);
            }
        });
    }

    @Override
    public long getCurrentPosition() {
        return mPlayer.getCurrentPosition();
    }

    @Override
    public long getDuration() {
        return mPlayer.getDuration();
    }

    @Override
    public void reset() {
        if (mMediaHandler != null) {//每次设置播放源之前会调用reset,后面添加进来的消息可能失效
            mMediaHandler.removeCallbacksAndMessages(null);
        }
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.reset();
            }
        });
    }

    @Override
    public void release() {
        if (mMediaHandler != null) {
            mMediaHandler.removeCallbacksAndMessages(null);
        }
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.release();
                mPlayer = null;
                if(mHandlerThread != null){
                    mHandlerThread.quit();
                    mHandlerThread = null;
                }
            }
        });
        mMediaHandler = null;
    }

    @Override
    public boolean isRelease() {
        return mPlayer.isRelease();
    }

    @Override
    public void setVolume(final float leftVolume, final float rightVolume) {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.setVolume(leftVolume, rightVolume);
            }
        });
    }

    @Override
    public int getAudioSessionId() {
        return mPlayer.getAudioSessionId();
    }

    @Override
    public void setLooping(final boolean looping) {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.setLooping(looping);
            }
        });
    }

    @Override
    public boolean isLooping() {
        return mPlayer.isLooping();
    }

    @Override
    public void setPlayerEventListener(IPlayerEventListener listener) {
        mPlayerEventListener = listener;
    }

    @Override
    public void setEnableMediaCodec(boolean isEnable) {
        mPlayer.setEnableMediaCodec(isEnable);
    }

    @Override
    public void setOnRenderChangeListener(OnRenderChangeListener renderChangeListener) {
        mPlayer.setOnRenderChangeListener(renderChangeListener);
    }

    @Override
    public void setAudioStreamType(final int streamtype) {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.setAudioStreamType(streamtype);
            }
        });
    }

    @Override
    public void clearVideoSurface() {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.clearVideoSurface();
            }
        });
    }

    @Override
    public void clearVideoSurface(final Surface surface) {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.clearVideoSurface(surface);
            }
        });
    }

    @Override
    public void setVideoSurface(@Nullable final Surface surface) {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.setVideoSurface(surface);
            }
        });
    }

    @Override
    public void setVideoSurfaceHolder(final SurfaceHolder surfaceHolder) {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.setVideoSurfaceHolder(surfaceHolder);
            }
        });
    }

    @Override
    public void clearVideoSurfaceHolder(final SurfaceHolder surfaceHolder) {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.clearVideoSurfaceHolder(surfaceHolder);
            }
        });
    }

    @Override
    public void setVideoSurfaceView(final SurfaceView surfaceView) {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.setVideoSurfaceView(surfaceView);
            }
        });
    }

    @Override
    public void clearVideoSurfaceView(final SurfaceView surfaceView) {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.clearVideoSurfaceView(surfaceView);
            }
        });
    }

    @Override
    public void setVideoTextureView(final TextureView textureView) {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.setVideoTextureView(textureView);
            }
        });
    }

    @Override
    public void clearVideoTextureView(final TextureView textureView) {
        getMediaHandler().post(new Runnable() {
            @Override
            public void run() {
                mPlayer.clearVideoTextureView(textureView);
            }
        });
    }

    @Override
    public void onPrepared(final IPlayer mp) {
        if (mPlayerEventListener != null) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    mPlayerEventListener.onPrepared(mp);
                }
            });
        }
    }

    @Override
    public void onCompletion(final IPlayer mp) {
        if (mPlayerEventListener != null) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    mPlayerEventListener.onCompletion(mp);
                }
            });
        }
    }

    @Override
    public void onBufferingUpdate(final IPlayer mp, final int percent) {
        if (mPlayerEventListener != null) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    mPlayerEventListener.onBufferingUpdate(mp, percent);
                }
            });
        }
    }

    @Override
    public void onSeekComplete(final IPlayer mp) {
        if (mPlayerEventListener != null) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    mPlayerEventListener.onSeekComplete(mp);
                }
            });
        }
    }

    @Override
    public void onVideoSizeChanged(final IPlayer mp, final int width, final int height) {
        if (mPlayerEventListener != null) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    mPlayerEventListener.onVideoSizeChanged(mp, width, height);
                }
            });
        }
    }

    @Override
    public boolean onError(final IPlayer mp, final int what, final int extra, final String message) {
        if (mPlayerEventListener != null) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    mPlayerEventListener.onError(mp, what, extra, message);
                }
            });
        }
        return true;
    }

    @Override
    public boolean onInfo(final IPlayer mp, final int what, final int extra) {
        if (mPlayerEventListener != null) {
            mMainHandler.post(new Runnable() {
                @Override
                public void run() {
                    mPlayerEventListener.onInfo(mp, what, extra);
                }
            });
        }
        return true;
    }

    private class PlayerHandlerThread extends HandlerThread {

        private PlayerHandlerThread(String name) {
            super(name);
            Log.d(TAG,"PlayerHandlerThread create");
        }

        public PlayerHandlerThread(String name, int priority) {
            super(name, priority);
        }

        @Override
        protected void onLooperPrepared() {
            super.onLooperPrepared();
            Log.d(TAG,"onLooperPrepared");
            mMediaHandler = new Handler(getLooper());
        }

    }
}
