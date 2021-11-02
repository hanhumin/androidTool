package com.autoipod.utils;

public class Observable<T> extends java.util.Observable {
    private T data;
    public void setData(T data)
    {
        this.data = data;
        setChanged();
        notifyObservers(data);
    }

    public T getData()
    {
        return data;
    }
}