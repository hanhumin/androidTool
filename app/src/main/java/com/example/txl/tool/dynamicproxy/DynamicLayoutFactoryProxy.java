package com.example.txl.tool.dynamicproxy;

import android.app.Activity;
import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Window;

import androidx.core.view.LayoutInflaterCompat;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

public class DynamicLayoutFactoryProxy {
    private String TAG = DynamicLayoutFactoryProxy.class.getSimpleName();
    /**
     * 动态代理Factory  还是代理LayoutInflater?
     * */
    public void setProxyFactory2(Activity activity){
        final LayoutInflater.Factory2 originFactory2 = activity.getLayoutInflater().getFactory2();
        if(originFactory2 != null){

            try {
                Log.d(TAG,activity.getClass().getSimpleName()+" in activity do dynamic proxy ");
                LayoutInflater inflater = activity.getLayoutInflater();
                Field mFactorySet = LayoutInflater.class.getDeclaredField("mFactorySet");
                mFactorySet.setAccessible(true);
                mFactorySet.setBoolean(inflater,false);
                LayoutInflater.Factory2 factory2 = (LayoutInflater.Factory2) Proxy.newProxyInstance(originFactory2.getClass().getClassLoader(), originFactory2.getClass().getInterfaces(), new InvocationHandler() {
                    @Override
                    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
                        Log.d(TAG,"执行动态代理方法 :: " +method.getName() +" args "+args.length );
                        Object o = null;
                        if(args.length == 3){
                            o = args[2];
                        }else if(args.length == 4){
                            o = args[3];
                        }
                        if(o instanceof AttributeSet){
                            Log.d(TAG,"获取到属性 :: "+o.getClass().getSimpleName() );
                        }
                        method.setAccessible(true);
                        return method.invoke(originFactory2,args);
                    }
                });

                LayoutInflaterCompat.setFactory2(activity.getLayoutInflater(), factory2);
            } catch (NoSuchFieldException e) {
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            }

        }

    }
}
