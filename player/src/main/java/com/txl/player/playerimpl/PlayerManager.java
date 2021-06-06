package com.txl.player.playerimpl;

import android.content.Context;
import android.util.Log;

import com.txl.player.base.IPlayer;
import com.txl.player.playerimpl.mediaplayer.AndroidMediaPlayer;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

/**
 * Copyright (c) 2020 唐小陆 All rights reserved.
 * author：txl
 * date：2020/4/22
 * description：全局唯一的播放器，在单独的页面需要自己处理恢复播放逻辑
 * */
public class PlayerManager {
    private static final String TAG = "PlayerManager";
    private static PlayerManager playerManager;
    private static PlayerFactory defaultPlayerFactory = new PlayerFactory() {
        @Override
        protected IPlayer createPlayer(Context context) {
            return new PlayerThreadSwitchWrapper(new PlayerStateWrapper(new AndroidMediaPlayer(context)));
        }
    };

    private static PlayerFactory playerFactory;

    /**
     * 要设置的话，需要在第一次调用获取播放器之前设置
     * */
    public static void setPlayerFactory(PlayerFactory playerFactory){
        PlayerManager.playerFactory = playerFactory;
    }

    private IPlayer player;
    private IPlayer playerProxy;

    public static IPlayer getIPlayer(Context context){
        if(playerManager == null){
            synchronized (PlayerManager.class){
                playerManager =  new PlayerManager(context);
            }
        }
        return playerManager.playerProxy;
    }

    private PlayerManager(Context context){
        if(playerFactory !=null){
            player = playerFactory.createPlayer(context);
        }else {
            player = defaultPlayerFactory.createPlayer(context);
        }
//        player = PlayerFactory.createPlayer(context,PlayerFactory.MEDIA_PLAYER);
        player.setEnableMediaCodec(true);
        playerProxy = (IPlayer) Proxy.newProxyInstance(player.getClass().getClassLoader(),player.getClass().getInterfaces(),new PlayerProxy(player));
    }

    /**
     * 在应用程序结束的时候释放播放器资源
     * 此后播放器都
     * */
    public static void releasePlayerManager(){
        Log.d(TAG,"releasePlayerManager");
        PlayerManager temp = playerManager;
        playerManager = null;
        temp.player.release();
        temp.player = null;
        temp.playerProxy = null;
    }

    private static class PlayerProxy implements InvocationHandler{
        private IPlayer player;

        private PlayerProxy(IPlayer player) {
            this.player = player;
        }

        @Override
        public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
            try {
                if(method.getName().equals("release")){
                    player.reset();
                }else {
                    method.setAccessible(true);
                    return method.invoke(player,args);
                }
            }catch (Exception e){
                e.printStackTrace();
            }
            return null;
        }
    }
}
