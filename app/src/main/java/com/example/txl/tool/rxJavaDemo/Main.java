package com.example.txl.tool.rxJavaDemo;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;

import io.reactivex.Flowable;
import io.reactivex.Observable;
import io.reactivex.ObservableEmitter;
import io.reactivex.ObservableOnSubscribe;
import io.reactivex.Observer;
import io.reactivex.disposables.Disposable;
import io.reactivex.functions.BiFunction;
import io.reactivex.functions.Consumer;
import io.reactivex.functions.Function;

public class Main {
    public static void hello(String... args) {
        Flowable.fromArray(args).subscribe(new Consumer<String>() {
            @Override
            public void accept(String s) throws Exception {
                System.out.println("Hello " + s + "!");
            }
        });
    }

    public static void main(String[] arga){
//        hello("Ben", "George");

        //创建被观察者
        Observable<String> o = Observable.fromArray("a", "b", "c");
        Integer[] list = {5, 6, 7, 8};
        Observable<Integer> o1 = Observable.fromArray(list);
        Observable<String> o2 = Observable.just("one object");

//        customObservable();
//        asynchronousCustomObservable();
//        operationObservable();
        zipOperationObservable();
        try {
            Thread.sleep(50000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }


    /**
     * 同步Observable
     * */
    public static void customObservable(){
        Observable<String> o3 = Observable.create(new ObservableOnSubscribe<String>(){
            @Override
            public void subscribe(ObservableEmitter<String> emitter) throws Exception {
                for(int i=0; i<=5; i++){
                    emitter.onNext("i = "+i);
                }
                emitter.onComplete();
            }
        });
        o3.subscribe(new Observer<String>() {
            @Override
            public void onSubscribe(Disposable d) {

            }

            @Override
            public void onNext(String s) {
                System.out.println("onNext   s "+s);
            }

            @Override
            public void onError(Throwable e) {

            }

            @Override
            public void onComplete() {

            }
        });
    }
    /**
     * 异步的Observable
     * */
    public static void asynchronousCustomObservable(){
        Observable<String> o3 = Observable.create(new ObservableOnSubscribe<String>(){
            @Override
            public void subscribe(final ObservableEmitter<String> emitter) throws Exception {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            Thread.sleep(5000);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                        for(int i=0; i<5; i++){
                            emitter.onNext("i = "+i);
                        }
                        emitter.onComplete();
                    }
                },"异步Observable线程").start();

            }
        });
        o3.subscribe(new Observer<String>() {
            @Override
            public void onSubscribe(Disposable d) {

            }

            @Override
            public void onNext(String s) {
                System.out.println("onNext   s "+s+"  线程名字："+Thread.currentThread().getName());
            }

            @Override
            public void onError(Throwable e) {

            }

            @Override
            public void onComplete() {

            }
        });
    }

    /**
     *用运算符转换Observable
     * */
    public static void operationObservable(){
        Observable.create(new ObservableOnSubscribe<String>(){
            @Override
            public void subscribe(final ObservableEmitter<String> emitter) throws Exception {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            Thread.sleep(5000);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                        for(int i=0; i<75; i++){
                            emitter.onNext("i = "+i);
                        }
                        emitter.onComplete();
                    }
                },"异步Observable线程").start();

            }
        })
                .skip(10)
                .take(5)
                .map(new Function<String, String>() {
                    @Override
                    public String apply(String s) throws Exception {
                        return s+" _Function";
                    }
                })
                .subscribe(new Observer<String>() {
            @Override
            public void onSubscribe(Disposable d) {

            }

            @Override
            public void onNext(String s) {
                System.out.println("onNext   value  "+s);
            }

            @Override
            public void onError(Throwable e) {

            }

            @Override
            public void onComplete() {

            }
        });
    }

    /**
     * zip操作
     * */
    public static void zipOperationObservable(){
        Observable<String> o = Observable.fromArray("a", "b", "c","d","f");
        Integer[] listI = {5, 6, 7, 8};
        Observable<Integer> o1 = Observable.fromArray(listI);
        Observable<String> o2 = Observable.create(new ObservableOnSubscribe<String>() {
            @Override
            public void subscribe(final ObservableEmitter<String> emitter) throws Exception {
                emitter.onNext("少年= ");
                emitter.onComplete();
            }
        });
        /**
         * zip中的ObservableSource必须是同步的
         * */
        Observable.zip(o, o1, new BiFunction<String, Integer, String>() {
            @Override
            public String apply(String s, Integer integer) throws Exception {
                return "我是："+s+integer;
            }
        }).subscribe(new Observer<String>() {
            @Override
            public void onSubscribe(Disposable d) {

            }

            @Override
            public void onNext(String s) {
                System.out.println(s);
            }

            @Override
            public void onError(Throwable e) {

            }

            @Override
            public void onComplete() {

            }
        });
    }
}
