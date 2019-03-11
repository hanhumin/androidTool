package com.example.txl.tool.player.music;

import android.content.Context;
import android.media.MediaPlayer;
import android.net.Uri;
import android.util.Log;

import com.example.txl.tool.player.AndroidPlayer;

import java.io.IOException;
import java.lang.ref.WeakReference;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/3/11
 * description：音频播放
 */
public class AndroidMusicPlayer implements IMusicPlayer<String> {
    private static final String TAG = AndroidPlayer.class.getSimpleName();
    Context _ctx;
    MediaPlayer _mp;
    String _url;
    String tag;
    long _seekTarget;
    private boolean _singlePlayer = false;
    private boolean _disposablePlayer = false;

    int _playerState = PS_UNINITIALIZED;

    static final int PS_REBUILD = 0x20000;     // player service died, must rebuild player
    static final int PS_SURFACE_LOST = 0x10000;
    static final int PS_UNINITIALIZED = 0x8000;
    static final int PS_RELEASED = 0x4000;
    static final int PS_PREPARED = 0x2000;
    static final int PS_STOPPED = 0x1000;
    static final int PS_ERROR = 0x0800;
    static final int PS_SEEK_AGAIN = 0x0010;
    static final int PS_SEEKING = 0x0008;
    static final int PS_BUFFERING = 0x0004;
    static final int PS_PREPARING = 0x0002;
    static final int PS_PLAYING = 0x0001;

    private IMusicPlayer.IMusicPlayerEvents _listener;

    private static WeakReference<AndroidMusicPlayer> _currentPlayer;
    private WeakReference<AndroidMusicPlayer> _previousPlayer;

    private void _changeState(int removeState, int addState) {
        _playerState = (_playerState & ~removeState) | addState;
    }

    private boolean _hasState(int state) {
        return (_playerState & state) == state;
    }

    private boolean _hasAnyState(int state) {
        return (_playerState & state) != 0;
    }

    public AndroidMusicPlayer(Context context,boolean singlePlayer, boolean disposablePlayer){
        _ctx = context;
        _singlePlayer = singlePlayer;
        _disposablePlayer = disposablePlayer;
        _previousPlayer = _currentPlayer;
        _currentPlayer = new WeakReference<>(this);
    }

    public AndroidMusicPlayer(Context context,boolean singlePlayer) {
        this(context,singlePlayer, false);
    }

    @Override
    public void init() {
        _createMusicPlayer();
    }

    private void _createMusicPlayer(){
        if(_singlePlayer && _previousPlayer != null){
            AndroidMusicPlayer prevPlayer = _previousPlayer.get();
            if(prevPlayer != null){
                prevPlayer._disposePlayer();
            }
        }
        MediaPlayer mediaPlayer = new MediaPlayer();
        mediaPlayer.setOnPreparedListener( new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                if (_mp != mp) {
                    return;
                }
                _onPrepared(mp);
            }
        } );
        mediaPlayer.setOnSeekCompleteListener(new MediaPlayer.OnSeekCompleteListener() {
            @Override
            public void onSeekComplete(MediaPlayer mp) {
                if (_mp != mp) {
                    return;
                }
                _onSeekComplete(mp);
            }
        });
        mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                if (_mp != mp) {
                    return;
                }
                _onCompletion(mp);
            }
        });
        mediaPlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mp, int what, int extra) {
                if (_mp != mp) {
                    return true;
                }
                return _onError(mp, what, extra);
            }
        });
        mediaPlayer.setOnInfoListener(new MediaPlayer.OnInfoListener() {
            @Override
            public boolean onInfo(MediaPlayer mp, int what, int extra) {
                if (_mp != mp) {
                    return true;
                }
                return _onInfo(mp, what, extra);
            }
        });
    }

    private void _onPrepared(MediaPlayer mp) {
        _changeState(PS_PREPARING, PS_PREPARED);
        if (_hasState(PS_STOPPED)) {
            _mp.stop();
        } else if (_hasState(PS_SEEKING)) {
            _mp.seekTo((int) _seekTarget);
        } else if (_hasState(PS_PLAYING)) {
            _mp.start();
        }

        IMusicPlayerEvents listener = _listener;
        if (listener != null) {
            listener.onPrepared(this);
        }
    }

    private void _onSeekComplete(MediaPlayer mp) {
        if (_hasAnyState(PS_SEEK_AGAIN)) {
            _changeState(PS_SEEK_AGAIN, 0);
            _mp.seekTo((int) _seekTarget);
            return;
        }

        _changeState(PS_SEEKING, 0);
        if (_hasState(PS_STOPPED)) {
            _mp.stop();
        } else if (_hasState(PS_PLAYING)) {
            _mp.start();
        } else {
            _mp.pause();
        }

        if (isMediaStopped()) {
            return;
        }
        IMusicPlayerEvents listener = _listener;
        if (listener != null) {
            listener.onSeekComplete(this, getCurrentPosition());
        }
    }

    private boolean isMediaStopped() {
        return !_hasState(PS_PREPARED) || _hasAnyState(PS_STOPPED | PS_RELEASED | PS_UNINITIALIZED);
    }

    private void _onCompletion(MediaPlayer mp) {
        if (isMediaStopped()) {
            return;
        }
        IMusicPlayerEvents listener = _listener;
        if (listener != null) {
            listener.onComplete(this);
        }
    }

    private boolean _onError(MediaPlayer mp, int what, int extra) {
        _changeState(0, PS_ERROR);
        if (what == MediaPlayer.MEDIA_ERROR_SERVER_DIED) {
            _changeState(0, PS_REBUILD);
        }
        IMusicPlayerEvents listener = _listener;
        if (listener != null) {
            return listener.onError(this, what, String.valueOf(extra));
        }
        return true;
    }

    private boolean _onInfo(MediaPlayer mp, int what, int extra) {
        if (isMediaStopped()) {
            return false;
        }
        IMusicPlayerEvents listener = _listener;
        if (listener == null) {
            return false;
        }
        if (what == MediaPlayer.MEDIA_INFO_BUFFERING_END) {
            listener.onBuffering(this, false, 100.0f);
        } else if (what == MediaPlayer.MEDIA_INFO_BUFFERING_START) {
            listener.onBuffering(this, true, 0.0f);
        }
        return false;
    }

    private void _disposePlayer() {
        MediaPlayer mp = _mp;
        _mp = null;
        if (mp != null) {
            mp.setOnErrorListener(null);
            mp.setOnBufferingUpdateListener(null);
            mp.setOnSeekCompleteListener(null);
            mp.setOnCompletionListener(null);
            mp.setOnPreparedListener(null);
            mp.setOnInfoListener(null);
            mp.setDisplay(null);
            _changeState(PS_PREPARED | PS_PREPARING | PS_STOPPED | PS_ERROR, PS_REBUILD | PS_RELEASED);
            mp.release();
        }
    }

    @Override
    public long getCurrentPosition() {
        if (_hasState(PS_PREPARED)) {
            if (_hasAnyState(PS_SEEKING | PS_BUFFERING)) {
                return _seekTarget;
            }
            int currentPosition = _mp.getCurrentPosition();
            if (currentPosition >= _mp.getDuration()) {
                return _seekTarget;
            }
            _seekTarget = currentPosition;
            return _seekTarget;
        }
        return 0;
    }

    @Override
    public boolean seekTo(long pos) {
        if (_hasState(PS_PREPARED | PS_SEEKING)) {
            _seekTarget = pos;
            _changeState(0, PS_SEEK_AGAIN);
            return true;
        }

        _changeState(0, PS_SEEKING);
        _seekTarget = pos;
        if (_hasState(PS_PREPARED)) {
            _mp.seekTo((int) pos);
        }
        return true;
    }

    @Override
    public boolean stop() {
        if (_disposablePlayer && _hasAnyState(PS_PREPARED | PS_PREPARING | PS_ERROR)) {
            _disposePlayer();
            return true;
        }

        MediaPlayer mp = _mp;
        if (mp == null) {
            return false;
        }
        _changeState(0, PS_STOPPED);
        if (_hasState(PS_PREPARED) && !_hasState(PS_ERROR)) {
            _changeState(PS_PLAYING, 0);
            mp.stop();
        } else if (_hasAnyState(PS_REBUILD | PS_RELEASED)) {
            _changeState(_playerState & ~(PS_REBUILD | PS_RELEASED), 0);
        } else {
            _changeState(_playerState, PS_STOPPED);
            mp.reset();
        }
        return true;
    }

    @Override
    public boolean pause() {
        _changeState(PS_PLAYING, 0);
        if (_hasState(PS_PREPARED)) {
            _mp.pause();
        }
        return true;
    }

    @Override
    public boolean play() {
        _changeState(PS_STOPPED, PS_PLAYING);
        if (_hasState(PS_PREPARED)) {
            _mp.start();
        }
        return true;
    }

    @Override
    public boolean open(String url) {
        if (_disposablePlayer && _hasAnyState(PS_PREPARED | PS_PREPARING | PS_ERROR)) {
            _disposePlayer();
        }

        if (_hasAnyState(PS_REBUILD | PS_RELEASED)) {
            _changeState(PS_REBUILD | PS_RELEASED | PS_PREPARED | PS_PREPARING | PS_STOPPED | PS_ERROR, 0);
            _createMusicPlayer();
        }

        MediaPlayer mp = _mp;
        if (mp == null) {
            return false;
        }
        try {
            _url = url;
            Log.d(TAG, "playUrl = " + _url);
            if (_hasAnyState(PS_STOPPED | PS_PREPARED | PS_PREPARING)) {
                mp.reset();
            }
            mp.setDataSource(_ctx, Uri.parse(_url));
            _changeState(PS_BUFFERING | PS_PREPARED, PS_PREPARING);
            if (!_hasState(PS_UNINITIALIZED)) {
                mp.prepareAsync();
            }
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            Log.e(TAG, e.toString());
        }
        return false;
    }

    @Override
    public void destroy() {
        _changeState(_playerState, PS_UNINITIALIZED);
        MediaPlayer mp = _mp;
        _mp = null;
        if (mp != null) {
            if (mp.isPlaying()) {
                mp.reset();
            }
            mp.setDisplay(null);
            mp.release();
        }
    }

    @Override
    public void setEventListener(IMusicPlayerEvents listener) {
        _listener = listener;
    }

    @Override
    public void removeEventListener(IMusicPlayerEvents listener) {
        _listener = null;
    }

    @Override
    public boolean isPlaying() {
        return _mp != null && _hasState(PS_PLAYING | PS_PREPARED) && !_hasAnyState(PS_STOPPED | PS_UNINITIALIZED | PS_RELEASED);
    }

    @Override
    public String getPlayTag() {
        return tag;
    }

    @Override
    public void setPlayTag(String tag) {
        this.tag = tag;
    }
}
