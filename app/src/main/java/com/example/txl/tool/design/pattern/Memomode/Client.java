package com.example.txl.tool.design.pattern.Memomode;

import android.view.View;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/22
 * description：
 */
public class Client {
    public static void main(String[] args){
        TankGame tankGame = new TankGame();
        tankGame.play();

        Caretaker caretaker = new Caretaker();
        //保存数据
        caretaker.archive( tankGame.createMemoto() );
        tankGame.quite();
        TankGame newTankGame = new TankGame();
        newTankGame.restore( caretaker.getMemoto() );
        newTankGame.play();
    }
}
