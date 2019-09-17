package com.example.txl.tool.rxjava;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.example.txl.tool.R;

import io.reactivex.Observable;
import io.reactivex.ObservableEmitter;
import io.reactivex.ObservableOnSubscribe;
import io.reactivex.ObservableSource;
import io.reactivex.Observer;
import io.reactivex.Scheduler;
import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.disposables.Disposable;
import io.reactivex.functions.Function;
import io.reactivex.schedulers.Schedulers;


public class RxJavaDemoActivity extends AppCompatActivity {
    private final String TAG = RxJavaDemoActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate( savedInstanceState );
        setContentView( R.layout.activity_rxjava_demo );
        initView();
    }

    private void initView() {
        findViewById( R.id.tv_rxjava_basic_use ).setOnClickListener( new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                basicRxJavaUse();
            }
        } );
        findViewById( R.id.tv_rxjava_scheduling ).setOnClickListener( new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                rxJavascheduling1();
            }
        } );
        findViewById( R.id.tv_rxjava_map ).setOnClickListener( new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                rxJavaTransformation();
            }
        } );
    }

    private void basicRxJavaUse(){
        //创建被观察者
        Observable<String> observable =  Observable.create( new ObservableOnSubscribe<String>() {
            @Override
            public void subscribe(ObservableEmitter<String> emitter) throws Exception {
                Log.d( TAG, "Observable subscribe ");
                emitter.onNext( "我叫王大麻子" );
                emitter.onComplete();
            }
        } );
        //创建观察者
        Observer<String> observer = new Observer<String>() {
            @Override
            public void onSubscribe(Disposable d) {
                Log.d( TAG, "Observer onSubscribe ");
            }

            @Override


            public void onNext(String s) {
                Log.d( TAG, "Observer onNext "+s);
            }

            @Override
            public void onError(Throwable e) {
                Log.d( TAG, "Observer onError ");
            }

            @Override
            public void onComplete() {
                Log.d( TAG, "Observer onComplete ");
            }
        };
        //通过subscribe链接观察者和被观察者
        observable.subscribe( observer);
    }

    /**
     * RxJava调度
     * */
    private void rxJavascheduling(){
        //创建被观察者
        Observable<String> observable =  Observable.create( new ObservableOnSubscribe<String>() {
            @Override
            public void subscribe(ObservableEmitter<String> emitter) throws Exception {
                Log.d( TAG, "Observable subscribe 当前线程： "+Thread.currentThread().getName());
                emitter.onNext( "我叫王大麻子" );
                emitter.onComplete();
            }
        } );
        //创建观察者
        Observer<String> observer = new Observer<String>() {
            @Override
            public void onSubscribe(Disposable d) {
                Log.d( TAG, "Observer onSubscribe 当前线程： "+Thread.currentThread().getName());
            }

            @Override


            public void onNext(String s) {
                Log.d( TAG, "Observer onNext "+s+"  当前线程： "+Thread.currentThread().getName());
            }

            @Override
            public void onError(Throwable e) {
                Log.d( TAG, "Observer onError 当前线程： "+Thread.currentThread().getName());
            }

            @Override
            public void onComplete() {
                Log.d( TAG, "Observer onComplete 当前线程： "+Thread.currentThread().getName());
            }
        };
        //通过subscribe链接观察者和被观察者
        observable.subscribeOn( Schedulers.io() ).subscribe( observer);
    }

    private void rxJavascheduling1(){
        //创建被观察者
        Observable<String> observable =  Observable.create( new ObservableOnSubscribe<String>() {
            @Override
            public void subscribe(ObservableEmitter<String> emitter) throws Exception {
                Log.d( TAG, "Observable subscribe 当前线程： "+Thread.currentThread().getName());
                emitter.onNext( "我叫王大麻子" );
                emitter.onComplete();
            }
        } );
        //创建观察者
        Observer<String> observer = new Observer<String>() {
            @Override
            public void onSubscribe(Disposable d) {
                Log.d( TAG, "Observer onSubscribe 当前线程： "+Thread.currentThread().getName());
            }

            @Override


            public void onNext(String s) {
                Log.d( TAG, "Observer onNext "+s+"  当前线程： "+Thread.currentThread().getName());
            }

            @Override
            public void onError(Throwable e) {
                Log.d( TAG, "Observer onError 当前线程： "+Thread.currentThread().getName());
            }

            @Override
            public void onComplete() {
                Log.d( TAG, "Observer onComplete 当前线程： "+Thread.currentThread().getName());
            }
        };
        //通过subscribe链接观察者和被观察者
        observable.subscribeOn( Schedulers.io() ).observeOn( AndroidSchedulers.mainThread() ).subscribe( observer);
    }

   /**
    * 变换
    * */
   private void rxJavaTransformation(){
       //创建被观察者
       Observable<String> observable =  Observable.create( new ObservableOnSubscribe<String>() {
           @Override
           public void subscribe(ObservableEmitter<String> emitter) throws Exception {
               Log.d( TAG, "Observable subscribe 当前线程： "+Thread.currentThread().getName());
               emitter.onNext( "我叫王大麻子" );
               emitter.onComplete();
           }
       } );
       //创建观察者
       Observer<String> observer = new Observer<String>() {
           @Override
           public void onSubscribe(Disposable d) {
               Log.d( TAG, "Observer onSubscribe 当前线程： "+Thread.currentThread().getName());
           }

           @Override


           public void onNext(String s) {
               Log.d( TAG, "Observer onNext "+s+"  当前线程： "+Thread.currentThread().getName());
           }

           @Override
           public void onError(Throwable e) {
               Log.d( TAG, "Observer onError 当前线程： "+Thread.currentThread().getName());
           }

           @Override
           public void onComplete() {
               Log.d( TAG, "Observer onComplete 当前线程： "+Thread.currentThread().getName());
           }
       };
       //通过subscribe链接观察者和被观察者
       observable.map( new Function<String, String>() {
           @Override
           public String apply(String s) throws Exception {
               return s +" map 变换的尾巴";
           }
       } ).flatMap( new Function<String, ObservableSource<String>>() {
           @Override
           public ObservableSource<String> apply(final String s) throws Exception {
               return Observable.create( new ObservableOnSubscribe<String>() {
                   @Override
                   public void subscribe(ObservableEmitter<String> emitter) throws Exception {
                       emitter.onNext( s+"  在经过 flatMap" );
                   }
               } );
           }
       } ).subscribe( observer);
   }
}

