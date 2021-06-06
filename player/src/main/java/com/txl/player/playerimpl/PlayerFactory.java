package com.txl.player.playerimpl;

import android.content.Context;


import com.txl.player.base.IPlayer;

public abstract class PlayerFactory {
//    public static final String EXO_PLAYER = "exo_player";
//    public static final String IJK_PLAYER = "ijk_player";
//    public static final String MEDIA_PLAYER = "media_player";
//
//    private static WeakReference<IPlayer> lastPlayer = null;
//
//    public static void releaseLastPlayer(){
//        if(lastPlayer != null){
//            IPlayer player = lastPlayer.get();
//            lastPlayer = null;
//            if(player != null && !player.isRelease()){
//                player.reset();
//                player.release();
//            }
//        }
//    }
//
//    public static IPlayer createPlayer(PlayerFactory playerFactory,boolean multiple){
//        if(!multiple){
//            releaseLastPlayer();
//        }
//        IPlayer player = playerFactory.createPlayer();
//        lastPlayer = new WeakReference<>(player);
//        return player;
//    }
//
//    public static IPlayer createPlayer(Context context,String type){
//        return createPlayer(context, type,false);
//    }
//
//    /**
//     * @param multiple 是否是多个
//     * */
//    public static IPlayer createPlayer(Context context,String type,boolean multiple){
//        if(!multiple){
//            releaseLastPlayer();
//        }
//        IPlayer player;
//        switch (type){
//            case IJK_PLAYER:{
//                player =  new PlayerStateWrapper(new IJkPlayer());
//                break;
//            }
//            case MEDIA_PLAYER:{
//                player = new PlayerStateWrapper(new AndroidMediaPlayer(context));
//                break;
//            }
//            default:{
//                player = new PlayerStateWrapper(new GoogleExoPlayer(context));
//                break;
//            }
//        }
//        lastPlayer = new WeakReference<>(player);
//        return player;
//    }

    protected abstract IPlayer createPlayer(Context context);


}
