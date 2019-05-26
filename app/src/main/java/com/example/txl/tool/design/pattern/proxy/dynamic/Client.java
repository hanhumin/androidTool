package com.example.txl.tool.design.pattern.proxy.dynamic;

import com.example.txl.tool.design.pattern.proxy.staticproxy.RealSubject;
import com.example.txl.tool.design.pattern.proxy.staticproxy.Subject;

import java.lang.reflect.Proxy;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/27
 * description：
 */
public class Client {
    public static void main(String[] args){
        Subject realSubject = new RealSubject();
        DynamicProxy dynamicProxy = new DynamicProxy( realSubject );
        ClassLoader loader = realSubject.getClass().getClassLoader();

        Subject proxySubject = (Subject) Proxy.newProxyInstance( loader,new Class[]{Subject.class},dynamicProxy );
        proxySubject.visit();
    }
}
