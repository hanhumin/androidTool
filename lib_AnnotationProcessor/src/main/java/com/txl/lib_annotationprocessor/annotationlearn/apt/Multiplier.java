package com.txl.lib_annotationprocessor.annotationlearn.apt;

/**
 * Copyright (c) 2020 唐小陆 All rights reserved.
 * author：txl
 * date：2020/6/1
 * description：
 */
@ExtractInterface("IMultiplier")
public class Multiplier {
    public int multiply(int x, int y){
        int total = 0;
        for (int i=0; i<x;  i++){
            total = add(total,y);
        }
        return total;
    }

    private int add(int x, int y){
        return x+y;
    }

    public static void main(String[] args){
        Multiplier m =  new Multiplier();
        System.out.println("11*16 = "+m.multiply(11,16));
    }
}
