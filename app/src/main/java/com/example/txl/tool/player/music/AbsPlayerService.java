package com.example.txl.tool.player.music;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import androidx.annotation.Nullable;

import java.util.ArrayList;
import java.util.List;

/**
 * @author TXL
 * description :音频播放服务
 */
public abstract class AbsPlayerService extends Service implements IMusicPlayer.IMusicPlayerEvents {

    IMusicPlayer _mPlayer;
    List<IMusicPlayer.IMusicPlayerEvents> musicPlayerEvents;
    boolean hasNotification;

    void initPlayer(){
        _mPlayer = createPlayer(this);
        if(musicPlayerEvents != null){
            musicPlayerEvents.clear();
            musicPlayerEvents = null;
        }
        _mPlayer.setEventListener(this);
    }

    private void addPlayerEventListener(IMusicPlayer.IMusicPlayerEvents event){
        if(musicPlayerEvents == null){
            musicPlayerEvents = new ArrayList<>();
        }
        musicPlayerEvents.add(event);
    }

    private void clearPlayerEventListener(){
        musicPlayerEvents.clear();
        musicPlayerEvents = null;
    }

    abstract IMusicPlayer createPlayer(Context context);

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return super.onStartCommand(intent, flags, startId);
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return new PlayerAdapter(this);
    }

    @Override
    public boolean onError(IMusicPlayer xmp, int code, String msg) {
        if(musicPlayerEvents == null){
            return false;
        }
        for (IMusicPlayer.IMusicPlayerEvents event : musicPlayerEvents){
            event.onError(xmp,code,msg);
        }
        return true;
    }

    @Override
    public boolean onPrepared(IMusicPlayer player) {
        if(musicPlayerEvents == null){
            return false;
        }
        for (IMusicPlayer.IMusicPlayerEvents event : musicPlayerEvents){
            event.onPrepared(player);
        }
        return true;
    }

    @Override
    public boolean onSeekComplete(IMusicPlayer player, long pos) {
        if(musicPlayerEvents == null){
            return false;
        }
        for (IMusicPlayer.IMusicPlayerEvents event : musicPlayerEvents){
            event.onSeekComplete(player,pos);
        }
        return true;
    }

    @Override
    public boolean onComplete(IMusicPlayer player) {
        if(musicPlayerEvents == null){
            return false;
        }
        for (IMusicPlayer.IMusicPlayerEvents event : musicPlayerEvents){
            event.onComplete(player);
        }
        return true;
    }

    @Override
    public boolean onBuffering(IMusicPlayer player, boolean buffering, float percentage) {
        if(musicPlayerEvents == null){
            return false;
        }
        for (IMusicPlayer.IMusicPlayerEvents event : musicPlayerEvents){
            event.onBuffering(player,buffering,percentage);
        }
        return true;
    }

    @Override
    public boolean onProgress(IMusicPlayer player, long pos) {
        if(musicPlayerEvents == null){
            return false;
        }
        for (IMusicPlayer.IMusicPlayerEvents event : musicPlayerEvents){
            event.onProgress(player,pos);
        }
        return true;
    }

    @Override
    public void onDestroy(IMusicPlayer player) {
        if(musicPlayerEvents == null){
            for (IMusicPlayer.IMusicPlayerEvents event : musicPlayerEvents){
                event.onDestroy(player);
            }
        }
        musicPlayerEvents = null;
    }

    @Override
    public boolean onPlay(IMusicPlayer player) {
        if(musicPlayerEvents == null){
            return false;
        }
        for (IMusicPlayer.IMusicPlayerEvents event : musicPlayerEvents){
            event.onPlay(player);
        }
        return true;
    }

    @Override
    public boolean onPause(IMusicPlayer player) {
        if(musicPlayerEvents == null){
            return false;
        }
        for (IMusicPlayer.IMusicPlayerEvents event : musicPlayerEvents){
            event.onPause(player);
        }
        return true;
    }

    @Override
    public boolean onStop(IMusicPlayer player) {
        if(musicPlayerEvents == null){
            return false;
        }
        for (IMusicPlayer.IMusicPlayerEvents event : musicPlayerEvents){
            event.onStop(player);
        }
        return true;
    }

    public static class PlayerAdapter extends Binder implements IMusicPlayer,INotification{
        AbsPlayerService absPlayerService;
        public PlayerAdapter(AbsPlayerService playerService) {
            this.absPlayerService = playerService;
            init();
        }


        @Override
        public void init() {
            absPlayerService.initPlayer();
        }

        @Override
        public void startNotification(boolean has) {
            absPlayerService.hasNotification = has;
        }

        @Override
        public long getCurrentPosition() {
            return absPlayerService._mPlayer.getCurrentPosition();
        }

        @Override
        public boolean seekTo(long pos) {
            absPlayerService._mPlayer.seekTo( pos );
            return true;
        }

        @Override
        public boolean stop() {
            absPlayerService._mPlayer.stop();
            return true;
        }

        @Override
        public boolean pause() {
            absPlayerService._mPlayer.pause();
            return true;
        }

        @Override
        public boolean play() {
            absPlayerService._mPlayer.play();
            return true;
        }

        @Override
        public boolean open(String url) {
            absPlayerService._mPlayer.open(url);
            return true;
        }

        @Override
        public void destroy() {
            absPlayerService.clearPlayerEventListener();
            absPlayerService = null;
        }

        @Override
        public void setEventListener(IMusicPlayerEvents listener) {
            absPlayerService.addPlayerEventListener(listener);
        }

        @Override
        public void removeEventListener(IMusicPlayerEvents listener) {
            absPlayerService.clearPlayerEventListener();
        }

        @Override
        public boolean isPlaying() {
            return absPlayerService._mPlayer.isPlaying();
        }

        @Override
        public Object getPlayTag() {
            return null;
        }

        @Override
        public void setPlayTag(Object tag) {

        }
    }

    public interface INotification{
        /**
         * 是否显示notification
         * */
        void startNotification(boolean has);
    }
}
