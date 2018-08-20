package com.example.txl.tool.inter.process.communication;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/14
 * description：
 */
public class Book implements Parcelable {
    public int bookId;
    public String bookName;

    protected Book(Parcel in) {
    }

    public Book(int bookId, String bookName) {
        this.bookId = bookId;
        this.bookName = bookName;
    }

    public static final Creator<Book> CREATOR = new Creator<Book>() {
        @Override
        public Book createFromParcel(Parcel in) {
            return new Book( in );
        }

        @Override
        public Book[] newArray(int size) {
            return new Book[size];
        }
    };

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt( bookId );
        dest.writeString( bookName );
    }
}
