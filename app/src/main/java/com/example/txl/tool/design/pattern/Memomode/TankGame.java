package com.example.txl.tool.design.pattern.Memomode;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/22
 * description：
 */
public class TankGame {
    /**
     * 关卡
     * */
    private int mLevel = 1;
    /**
     * 生命数
     * */
    private int mLife = 5;
    /**
     * 武器
     * */
    private String mWeapon = "普通枪";

    public void play(){
        System.out.println( String.format( "坦克大战进行中 ： 第%d关,你还有%d条性命,您的武器是%s",mLevel ,mLife,mWeapon) );
        System.out.println( "捡到坦克生命值加1" );
        mLife++;
        System.out.println( "捡到武器，武器净化" );
        mWeapon = "超级武器";
        System.out.println( "过关" );
        mLevel++;
    }

    public void quite(){
        System.out.println( "----------------开始退出-------------------" );
        System.out.println( "当前进度"+this.toString() );
        System.out.println( "----------------退出完成-------------------" );
    }

    public Memoto createMemoto(){
        Memoto memoto = new Memoto();
        memoto.mLevel = mLevel;
        memoto.mLife = mLife;
        memoto.mWeapon = mWeapon;
        return memoto;
    }

    public void restore(Memoto memoto){
        mLevel = memoto.mLevel;
        mLife = memoto.mLife;
        mWeapon = memoto.mWeapon;
        System.out.println( "恢复游戏 "+this.toString() );
    }

    @Override
    public String toString() {
        return "TankGame{" +
                "mLevel=" + mLevel +
                ", mLife=" + mLife +
                ", mWeapon='" + mWeapon + '\'' +
                '}';
    }
}
