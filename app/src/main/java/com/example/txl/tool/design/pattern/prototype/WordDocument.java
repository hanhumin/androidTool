package com.example.txl.tool.design.pattern.prototype;

import java.util.ArrayList;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/5/16
 * description：
 */
public class WordDocument implements Cloneable {
    private String mText;
    private ArrayList<String> mImages = new ArrayList<>(  );

    public String getmText() {
        return mText;
    }

    public void setmText(String mText) {
        this.mText = mText;
    }

    public ArrayList<String> getmImages() {
        return mImages;
    }

    public void addImages(String images) {
        this.mImages.add( images );
    }

    @Override
    protected WordDocument clone() {
        WordDocument wordDocument = null;
        try {
            wordDocument = (WordDocument) super.clone();
            wordDocument.mText = this.mText;
            //浅拷贝 对象复制
//            wordDocument.mImages = this.mImages;

            //深拷贝 对象也进行clone
            wordDocument.mImages = (ArrayList<String>) this.mImages.clone();
        } catch (CloneNotSupportedException e) {
            e.printStackTrace();
        }
        return wordDocument;
    }

    public void showDocumentInfo(){
        System.out.println( "-----------------------------------------------------" );
        System.out.println( mText );
        for (String simage:mImages){
            System.out.println( simage );
        }
        System.out.println( "====================================================" );
    }
}
