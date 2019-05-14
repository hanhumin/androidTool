package com.example.txl.tool.design.pattern.builder;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/15
 * description：
 */
public class Director {
    Builder mBuilder;

    public Director(Builder mBuilder) {
        this.mBuilder = mBuilder;
    }

    public void construct(String board,String display){
        mBuilder.buildBoard( board );
        mBuilder.buildDisplay( display );
        mBuilder.buildOs();
    }

    public static void main(String[] args){
        Builder builder = new MacbookBuilder();
        Director director = new Director( builder );
        director.construct( "XXX主板","XXX显示器" );
        System.out.print( "电脑信息："+builder.create().toString() );
    }
}
