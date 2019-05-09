package com.example.txl.tool.design.pattern.observer;

public class Main {

    public static void main(String[] args){
        Person xiaoM = new Person("小明"){
            @Override
            public void xiaYuLa() {
                super.xiaYuLa();
                System.out.println(name+" 下雨了，明天出去旅游记得带雨伞");
            }
        };

        WeatherObservable weatherObservable = new WeatherObservable();
        weatherObservable.registerObserver(xiaoM);

        //天气预报局通知下雨
        weatherObservable.notifyXiaYu();

    }
}

class Person implements IWeatherObserver{
    public Person(String name) {
        this.name = name;
    }

    String name;

    @Override
    public void xiaYuLa() {

    }
}
