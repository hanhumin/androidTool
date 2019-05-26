package com.example.txl.tool.design.pattern.proxy.staticproxy;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/27
 * description：
 */
public class RealSubject extends Subject {
    @Override
    public void visit() {
        System.out.println( "RealSubject" );
    }
}
