package com.txl.player.playerimpl;

import android.content.Context;
import android.net.Uri;
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
 * 参考MediaPlayer 进行状态管理
 * */
public class PlayerStateWrapper implements IPlayer, IPlayerEventListener {
    private static final String TAG = PlayerStateWrapper.class.getSimpleName();

    private static final int STATE_NO_INVALID = -1;
    private static final int STATE_IDlE = 1;
    private static final int STATE_INIT = 1 << 1;
    private static final int STATE_PREPARING = 1 << 2;
    private static final int STATE_PREPARED = 1 << 3;
    private static final int STATE_STARTED = 1 << 4;
    private static final int STATE_STOPPED = 1 << 5;
    private static final int STATE_PAUSE = 1 << 6;
    private static final int STATE_PLAY_COMPLETE = 1 << 7;
    private static final int STATE_END = 1 << 8;
    private static final int STATE_ERROR = 1 << 9;

    private int _currentState;


    /**
     * 实际播放器
     * */
    private IPlayer _player;

    private IPlayerEventListener _playerEventListener;

    private PendingState mPendingState;

    public PlayerStateWrapper(IPlayer player) {
        if(player == null){
            throw new RuntimeException("player must not null");
        }
        this._player = player;
        _player.setPlayerEventListener(this);
        _currentState = STATE_IDlE;
    }


    @Override
    public VideoRenderComponent getVideoRenderComponent() {
        return _player.getVideoRenderComponent();
    }

    @Override
    public void setDataSource(Context context, Uri uri) throws IOException, IllegalArgumentException, SecurityException, IllegalStateException {
        if(!_hasState(_currentState,STATE_IDlE)){
            reset();
        }
        _player.setDataSource(context,uri);
        _currentState = STATE_INIT;

    }

    @Override
    public void setDataSource(Context context, Uri uri, Map<String, String> headers) throws IOException, IllegalArgumentException, SecurityException, IllegalStateException {
        if(!_hasState(_currentState,STATE_IDlE)){
            reset();
        }
        _player.setDataSource(context,uri,headers);
        _currentState = STATE_INIT;
    }

    @Override
    public void setDataSource(FileDescriptor fd) throws IOException, IllegalArgumentException, IllegalStateException {
        if(!_hasState(_currentState,STATE_IDlE)){
            reset();
        }
        _player.setDataSource(fd);
        _currentState = STATE_INIT;
    }

    @Override
    public void setDataSource(String path) throws IOException, IllegalArgumentException, SecurityException, IllegalStateException {
        if(!_hasState(_currentState,STATE_IDlE)){
            reset();
        }
        _player.setDataSource(path);
        _currentState = STATE_INIT;
    }

    @Override
    public String getDataSource() {
        return _player.getDataSource();
    }

    @Override
    public void prepareAsync() throws IllegalStateException {
        if(_hasState(_currentState,STATE_INIT | STATE_STOPPED)){
            _currentState = STATE_PREPARING;
            _player.prepareAsync();
        }else {
            Log.e(TAG, _player.getClass().getSimpleName() +" call prepareAsync failed current state is "+_currentState);
        }

    }

    @Override
    public void start() throws IllegalStateException {
        if(_hasState(_currentState,STATE_PREPARED | STATE_PAUSE | STATE_PLAY_COMPLETE | STATE_STARTED)){
            _player.start();
            _currentState = STATE_STARTED;
        }/*else if(_hasState(_currentState,STATE_PREPARING)){//正在 preparing的时候如果调用start， 在准备完后直接进行播放
            _currentState |= STATE_STARTED;
        }*/else {
            Log.e(TAG, _player.getClass().getSimpleName() +" call start failed current state is "+_currentState);
        }
    }

    @Override
    public void stop() throws IllegalStateException {
        if(_hasState(_currentState,STATE_PREPARED | STATE_PAUSE | STATE_PLAY_COMPLETE | STATE_STARTED | STATE_STOPPED)){
            _player.stop();
            _currentState = STATE_STOPPED;
        }else {
            Log.e(TAG, _player.getClass().getSimpleName() +" call stop failed current state is "+_currentState);
        }
    }

    @Override
    public void pause() throws IllegalStateException {
        if(_hasState(_currentState, STATE_PAUSE | STATE_PLAY_COMPLETE | STATE_STARTED)){
            _player.pause();
            _currentState = STATE_PAUSE;
        }else {
            Log.e(TAG, _player.getClass().getSimpleName() +" call pause failed current state is "+_currentState);
        }

    }

    @Override
    public void setScreenOnWhilePlaying(boolean screenOn) {
        _player.setScreenOnWhilePlaying(screenOn);
    }

    @Override
    public int getVideoWidth() {
        if(_hasState(_currentState,STATE_IDlE | STATE_INIT | STATE_PREPARED | STATE_STARTED
                | STATE_PAUSE | STATE_STOPPED | STATE_PLAY_COMPLETE)){
            return _player.getVideoWidth();
        }
        return 0;
    }

    @Override
    public int getVideoHeight() {
        if(_hasState(_currentState,STATE_IDlE | STATE_INIT | STATE_PREPARED | STATE_STARTED
                | STATE_PAUSE | STATE_STOPPED | STATE_PLAY_COMPLETE)){
            return _player.getVideoHeight();
        }
        return 0;
    }

    @Override
    public boolean isPlaying() {
        //思考有没有必要不返回原来播放器的播放状态
        if(_hasState(_currentState, STATE_STARTED)){
            return true;
        }
        if(_hasState(_currentState,STATE_IDlE | STATE_INIT | STATE_PREPARED | STATE_STARTED
                | STATE_PAUSE | STATE_STOPPED | STATE_PLAY_COMPLETE)){
            return _player.isPlaying();
        }
        return false;
    }

    @Override
    public void seekTo(long msec) throws IllegalStateException {
        if(_hasState(_currentState,STATE_PREPARED | STATE_STARTED | STATE_PAUSE | STATE_PLAY_COMPLETE)){
            _player.seekTo(msec);
        }else {
            Log.e(TAG, _player.getClass().getSimpleName() +" call seekTo failed current state is "+_currentState);
        }
    }

    @Override
    public long getCurrentPosition() {
        if(_hasState(_currentState, STATE_PREPARED | STATE_STARTED
                | STATE_PAUSE | STATE_STOPPED | STATE_PLAY_COMPLETE)){
            return _player.getCurrentPosition();
        }
        return 0;
    }

    @Override
    public long getDuration() {
        if(_hasState(_currentState, STATE_PREPARED | STATE_STARTED
                | STATE_PAUSE | STATE_STOPPED | STATE_PLAY_COMPLETE)){
            return _player.getDuration();
        }else {
            Log.e(TAG, _player.getClass().getSimpleName() +" call getDuration failed current state is "+_currentState);
        }
        return 0;
    }

    @Override
    public void release() {
        _player.release();
        mPendingState = null;
        _currentState = STATE_END;
    }

    @Override
    public boolean isRelease() {
        return _player.isRelease();
    }

    @Override
    public void reset() {
        //重置前stop一下
        stop();
        _player.reset();
        mPendingState = null;
        _currentState = STATE_IDlE;
    }

    @Override
    public void setVolume(float leftVolume, float rightVolume) {
        if(_hasState(_currentState,STATE_IDlE | STATE_INIT | STATE_PREPARED | STATE_STARTED
                | STATE_PAUSE | STATE_STOPPED | STATE_PLAY_COMPLETE)){
            _player.setVolume(leftVolume, rightVolume);
        }
    }

    @Override
    public int getAudioSessionId() {
        return _player.getAudioSessionId();
    }

    @Override
    public void setLooping(boolean looping) {
        if(!_hasState(_currentState, STATE_ERROR)){
            _player.setLooping(looping);
        }else {
            Log.e(TAG, _player.getClass().getSimpleName() +" call setLooping failed current state is "+_currentState);
        }
    }

    @Override
    public boolean isLooping() {
        return _player.isLooping();
    }

    @Override
    public void setPlayerEventListener(IPlayerEventListener listener) {
        _playerEventListener = listener;
    }

    @Override
    public void setEnableMediaCodec(boolean isEnable) {
        _player.setEnableMediaCodec(isEnable);
    }

    @Override
    public void setOnRenderChangeListener(OnRenderChangeListener renderChangeListener) {
        _player.setOnRenderChangeListener(renderChangeListener);
    }

    @Override
    public void setAudioStreamType(int streamtype) {
        _player.setAudioStreamType(streamtype);
    }

    private boolean _hasState(int currentState, int targetState){
        return (currentState & targetState) != 0;
    }

    private boolean handlePendingState(){
        if(mPendingState != null){

        }
        return false;
    }

    /**
     * 添加待处理的状态
     * */
    private void appendPendingState(int pendingState){
        if(mPendingState == null){
            mPendingState = new PendingState();
        }
        mPendingState.pendingState = pendingState;
    }

    @Override
    public void onPrepared(IPlayer mp) {
        _currentState = STATE_PREPARED;
        Log.d(TAG,_player.getClass().getSimpleName() +" onPrepared ");
        if(_playerEventListener != null){
            _playerEventListener.onPrepared(mp);
        }
    }

    @Override
    public void onCompletion(IPlayer mp) {
        _currentState = STATE_PLAY_COMPLETE;
        Log.d(TAG,_player.getClass().getSimpleName() +" onCompletion ");
        if(_playerEventListener != null){
            _playerEventListener.onCompletion(mp);
        }
    }

    @Override
    public void onBufferingUpdate(IPlayer mp, int percent) {
        if(_playerEventListener != null){
            _playerEventListener.onBufferingUpdate(mp,percent);
        }
    }

    @Override
    public void onSeekComplete(IPlayer mp) {
        Log.d(TAG,_player.getClass().getSimpleName() +" onSeekComplete ");
        if(_playerEventListener != null){
            _playerEventListener.onSeekComplete(mp);
        }
    }

    @Override
    public void onVideoSizeChanged(IPlayer mp, int width, int height) {
        Log.d(TAG,_player.getClass().getSimpleName() +" onVideoSizeChanged width =  "+width+" height = "+height);
        if(_playerEventListener != null){
            _playerEventListener.onVideoSizeChanged(mp,width,height);
        }
    }

    @Override
    public boolean onError(IPlayer mp, int what, int extra,String message) {
        Log.e(TAG,_player.getClass().getSimpleName() +" onError what =  "+what+" extra = "+extra +" message :"+message);
        _currentState = STATE_ERROR;
        if(_playerEventListener != null){
            return _playerEventListener.onError(mp,what,extra,message);
        }
        return true;
    }

    @Override
    public boolean onInfo(IPlayer mp, int what, int extra) {
        if(_playerEventListener != null){
            return _playerEventListener.onInfo(mp,what,extra);
        }
        return false;
    }

    @Override
    public void clearVideoSurface() {
        _player.clearVideoSurface();
    }

    @Override
    public void clearVideoSurface(Surface surface) {
        _player.clearVideoSurface(surface);
    }

    @Override
    public void setVideoSurface(@Nullable Surface surface) {
        _player.setVideoSurface(surface);
    }

    @Override
    public void setVideoSurfaceHolder(SurfaceHolder surfaceHolder) {
        _player.setVideoSurfaceHolder(surfaceHolder);
    }

    @Override
    public void clearVideoSurfaceHolder(SurfaceHolder surfaceHolder) {
        _player.clearVideoSurfaceHolder(surfaceHolder);
    }

    @Override
    public void setVideoSurfaceView(SurfaceView surfaceView) {
        _player.setVideoSurfaceView(surfaceView);
    }

    @Override
    public void clearVideoSurfaceView(SurfaceView surfaceView) {
        _player.clearVideoSurfaceView(surfaceView);
    }

    @Override
    public void setVideoTextureView(TextureView textureView) {
        _player.setVideoTextureView(textureView);
    }

    @Override
    public void clearVideoTextureView(TextureView textureView) {
        _player.clearVideoTextureView(textureView);
    }

    private static class PendingState{
        int pendingState = STATE_NO_INVALID;
    }
}
