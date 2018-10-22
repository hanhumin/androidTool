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
    private SQLiteOpenHelper helper;
    private SQLiteDatabase sqLiteDatabase;

    public DBManager(Context context) {
        helper = new MySQLiteHelper( context, MySQLiteHelper.DATABESE_NAME, null, MySQLiteHelper.DB_VERSION);
        sqLiteDatabase = helper.getReadableDatabase();
    }

    public SQLiteOpenHelper getHelper(){
        return helper;
    }

    public void destory(){
        sqLiteDatabase.close();
        helper.close();
    }
}
