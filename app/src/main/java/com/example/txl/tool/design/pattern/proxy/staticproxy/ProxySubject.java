package com.example.txl.tool.design.pattern.proxy.staticproxy;


/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/27
 * description：
 */
public class ProxySubject extends Subject {
    RealSubject realSubject;

    public ProxySubject(RealSubject realSubject) {
        this.realSubject = realSubject;
    }

    @Override
    public void visit() {
        realSubject.visit();
    }
}
