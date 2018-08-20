package com.example.txl.tool.inter.process.communication;

import android.os.Parcel;
import android.os.Parcelable;

import java.io.Serializable;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/8/14
 * description：
 */
public class User implements Serializable {
    private static final long serialVersionUID = 1L;

    public int userId;
    public String userName;
    public boolean isMale;
//    public Book book;

//    public User(Parcel in) {
//        userId = in.readInt();
//        userName = in.readString();
//        isMale = in.readInt() == 1;
//        book = in.readParcelable( Thread.currentThread().getContextClassLoader() );
//    }

//    public static final Creator<User> CREATOR = new Creator<User>() {
//        @Override
//        public User createFromParcel(Parcel in) {
//            return new User( in );
//        }
//
//        @Override
//        public User[] newArray(int size) {
//            return new User[size];
//        }
//    };

    public User(int userId, String userName, boolean isMale) {
        this.userId = userId;
        this.userName = userName;
        this.isMale = isMale;
//        this.book = book;
    }

//    @Override
//    public int describeContents() {
//        return 0;
//    }
//
//    @Override
//    public void writeToParcel(Parcel dest, int flags) {
//        dest.writeInt( userId );
//        dest.writeString( userName );
//        dest.writeInt( isMale ? 1:0 );
//        dest.writeParcelable( book,0 );
//    }
}
