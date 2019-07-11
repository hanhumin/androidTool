package com.example.txl.tool.inter.process.communication;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/7/9
 * description：
 */
public class Book implements Parcelable {
    private int bookId;
    private String name;
    private String author;

    public Book(int bookId, String name, String author) {
        this.bookId = bookId;
        this.name = name;
        this.author = author;
    }

    protected Book(Parcel in) {
        bookId = in.readInt();
        name = in.readString();
        author = in.readString();
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
        dest.writeString( name );
        dest.writeString( author );
    }

    public int getBookId() {
        return bookId;
    }

    public void setBookId(int bookId) {
        this.bookId = bookId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    @Override
    public String toString() {
        return "Book{" +
                "bookId=" + bookId +
                ", name='" + name + '\'' +
                ", author='" + author + '\'' +
                '}';
    }
}
