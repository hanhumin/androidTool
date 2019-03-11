package com.example.txl.tool.player.music;

import android.content.Context;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/3/11
 * description：使用mediaPlayer播放
 */
public class AndroidMusicPlayerService extends AbsPlayerService{
    @Override
    IMusicPlayer createPlayer(Context context) {
        return new AndroidMusicPlayer(context,true,true);
    }
}
