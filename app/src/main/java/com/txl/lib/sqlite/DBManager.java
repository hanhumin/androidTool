package com.txl.lib.sqlite;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/10/19
 * description：数据库管理
 */
public class DBManager {
    public static final String DATABESE_NAME = "todo.db";
    public static int DB_VERSION = 1;

    private SQLiteOpenHelper helper;
    private SQLiteDatabase sqLiteDatabase;

    public DBManager(Context context) {
        helper = new MySQLiteHelper( context, DATABESE_NAME, null, DB_VERSION);
        sqLiteDatabase = helper.getReadableDatabase();
    }

    public DBManager(Context context, String dbName, int dbVersion) {
        helper = new MySQLiteHelper( context, dbName, null, dbVersion);
        sqLiteDatabase = helper.getReadableDatabase();
    }

    public SQLiteOpenHelper getHelper(){
        return helper;
    }

    public SQLiteDatabase getSqLiteDatabase() {
        return sqLiteDatabase;
    }

    public void destory(){
        sqLiteDatabase.close();
        helper.close();
    }
}
