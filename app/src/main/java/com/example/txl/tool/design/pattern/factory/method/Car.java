package com.example.txl.tool.design.pattern.factory.method;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/17
 * description：
 */
public abstract class Car {
    /**
     * 创建车身
     * */
    abstract void createBody();

    abstract void createWheel();
}
