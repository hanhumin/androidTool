package com.example.txl.tool.design.pattern.composite;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/28
 * description：
 */
public abstract class Component {
    /**
     * 节点名
     * */
    protected String name;

    public Component(String name) {
        this.name = name;
    }

    public abstract void doSomething();
}
