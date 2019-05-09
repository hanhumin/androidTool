package com.example.txl.tool.design.pattern.observer;

public class WeatherObservable extends Observable<IWeatherObserver>{
    void notifyXiaYu(){
        for (IWeatherObserver iWeatherObserver: mObservers){
            iWeatherObserver.xiaYuLa();
        }
    }
}
