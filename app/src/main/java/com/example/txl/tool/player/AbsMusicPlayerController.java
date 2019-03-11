package com.example.txl.tool.player;

import android.content.ComponentName;
import android.content.Context;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.util.Log;

import com.example.txl.tool.player.music.AbsPlayerService;
import com.example.txl.tool.player.music.IMusicPlayer;

import java.lang.ref.WeakReference;

/**
 * @author TXL
 * description :
 */
public abstract class AbsMusicPlayerController implements IMusicPlayer.IMusicPlayerEvents {
    protected final String TAG = getClass().getSimpleName();
    WeakReference<Context> _mContext;
    AbsPlayerService.PlayerAdapter _mMusicPlayer;
    ServiceConnection serviceConnection;

    public AbsMusicPlayerController(Context context) {
        _mContext = new WeakReference<>(context);
        serviceConnection = new ServiceConnection() {
            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {
                if(service instanceof IMusicPlayer){
                    _mMusicPlayer = (AbsPlayerService.PlayerAdapter) service;
                    _mMusicPlayer.setEventListener(AbsMusicPlayerController.this);
                }else {
                    Log.e(TAG,"serviceConnection _mMusicPlayer init error unBind service");
                    Context c = _mContext.get();
                    if (c != null){
                        c.unbindService(this);
                    }
                }
            }

            @Override
            public void onServiceDisconnected(ComponentName name) {
                _mMusicPlayer.removeEventListener(AbsMusicPlayerController.this);
            }
        };
    }

    protected void initPlayerService(Context context){

    }

    @Override
    public boolean onError(IMusicPlayer xmp, int code, String msg) {
        return false;
    }

    @Override
    public boolean onPrepared(IMusicPlayer player) {
        return false;
    }

    @Override
    public boolean onSeekComplete(IMusicPlayer player, long pos) {
        return false;
    }

    @Override
    public boolean onComplete(IMusicPlayer player) {
        return false;
    }

    @Override
    public boolean onBuffering(IMusicPlayer player, boolean buffering, float percentage) {
        return false;
    }

    @Override
    public boolean onProgress(IMusicPlayer player, long pos) {
        return false;
    }

    @Override
    public void onDestroy(IMusicPlayer player) {

    }

    @Override
    public boolean onPlay(IMusicPlayer player) {
        return false;
    }

    @Override
    public boolean onPause(IMusicPlayer player) {
        return false;
    }

    @Override
    public boolean onStop(IMusicPlayer player) {
        return false;
    }

    public void destroy(){
        _mContext.clear();
        _mContext = null;
        _mMusicPlayer = null;
        serviceConnection = null;
    }
}
