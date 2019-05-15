package com.example.txl.tool.design.pattern.builder;


/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/15
 * description：
 */
public class MacbookBuilder extends Builder {
    private Computer mC = new MacBook();
    @Override
    public void buildBoard(String board) {
        mC.setmBoard( board );
    }

    @Override
    public void buildDisplay(String display) {
        mC.setmDisplay( display );
    }

    @Override
    public void buildOs() {
        mC.setOs();
    }

    @Override
    public Computer create() {
        return mC;
    }
}
