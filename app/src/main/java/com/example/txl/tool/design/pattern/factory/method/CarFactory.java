package com.example.txl.tool.design.pattern.factory.method;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/17
 * description：
 */
public class CarFactory implements ICar{

    @Override
    public <T extends Car> T createCar(Class<T> car) {
        Car c =  null;
        try {
            c = car.newInstance();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        return (T) c;
    }
}
