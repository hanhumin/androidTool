package com.example.txl.tool.design.pattern.factory.method;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/17
 * description：
 */
public class Test {
    public static void main(String[] args){
        ICar factory = new CarFactory();
        Car car1 = factory.createCar( BCCar.class );
        car1.createWheel();
        car1.createBody();

        Car car2 = factory.createCar( BWMCar.class );
        car2.createWheel();
        car2.createBody();
    }
}
