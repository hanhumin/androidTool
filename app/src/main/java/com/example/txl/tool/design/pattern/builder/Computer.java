package com.example.txl.tool.design.pattern.builder;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/15
 * description：
 */
public abstract class Computer {
    protected String mBoard;
    protected String mDisplay;
    protected String mOs;

    protected Computer() {
    }

    public void setmBoard(String mBoard) {
        this.mBoard = mBoard;
    }

    public void setmDisplay(String mDisplay) {
        this.mDisplay = mDisplay;
    }

    public abstract void setOs() ;

    @Override
    public String toString() {
        return "Computer{" +
                "mBoard='" + mBoard + '\'' +
                ", mDisplay='" + mDisplay + '\'' +
                ", mOs='" + mOs + '\'' +
                '}';
    }
}
