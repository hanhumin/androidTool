package com.example.txl.tool.design.pattern.factory.method;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/17
 * description：
 */
public class BWMCar extends Car {
    @Override
    void createBody() {
        System.out.println( "创建BWM车身" );
    }

    @Override
    void createWheel() {
        System.out.println( "创建BWM车轮" );
    }
}
