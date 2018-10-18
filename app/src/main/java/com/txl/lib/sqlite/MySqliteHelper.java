package com.txl.lib.sqlite;

import android.content.Context;
import android.database.DatabaseErrorHandler;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/10/17
 * description：
 */
public class MySqliteHelper extends SQLiteOpenHelper {
    public static final String DATABESE_NAME = "todo.db";
    public static int DB_VERSION = 1;
    public static String table_name = "table_todo";

    public MySqliteHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int version) {
        super( context, name, factory, version );
    }

    public MySqliteHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int version, DatabaseErrorHandler errorHandler) {
        super( context, name, factory, version, errorHandler );
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        String sql = "create table if not exists " + table_name + " (id integer primary key, completeDate INTEGER, completeDateStr TEXT, content TEXT," +
                "date INTEGER, dateStr TEXT, status INTEGER, title TEXT, type INTEGER, userId INTEGER)";
        db.execSQL( sql );
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {

    }

    @Override
    public void onConfigure(SQLiteDatabase db) {
        super.onConfigure( db );
    }
}
