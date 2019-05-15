package com.example.txl.tool.design.pattern.builder;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/15
 * description：
 */
public abstract class Builder {
    public abstract void buildBoard(String board);
    public abstract void buildDisplay(String display);
    public abstract void buildOs();
    public abstract Computer create();
}
