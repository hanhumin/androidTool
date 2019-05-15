package com.example.txl.tool.design.pattern.prototype;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/16
 * description：
 */
public class Test {
    public static void main(String[] args){
        WordDocument originWordDocument = new WordDocument();
        originWordDocument.setmText( "我是原始对象" );
        originWordDocument.addImages( "图一" );
        originWordDocument.addImages( "图二" );
        originWordDocument.addImages( "图三" );

        originWordDocument.showDocumentInfo();

        WordDocument wordDocument = originWordDocument.clone();

        wordDocument.setmText( "我是克隆对象" );
        wordDocument.addImages( "克隆图片" );

        wordDocument.showDocumentInfo();
        originWordDocument.showDocumentInfo();
    }
}
