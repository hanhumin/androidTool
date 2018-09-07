package com.example.txl.tool.huaxiyun.player;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;


public abstract class AbsBasePlayerUiController implements IPlayer.IPlayerEvents,View.OnClickListener{
    protected Context context;
    protected ViewGroup parent;

    IPlayer playerController;

    public AbsBasePlayerUiController(IPlayer player, ViewGroup parent, Context context) {
        this.parent = parent;
        this.context = context;
        initView(context);
        playerController = player;
    }

    abstract void initView(Context context);

    /**
     * Add child elements to the player ui
     * */
    abstract void addContent(View view);

    public void setPlayerController(IPlayer playerController){
        this.playerController = playerController;
    }
}
