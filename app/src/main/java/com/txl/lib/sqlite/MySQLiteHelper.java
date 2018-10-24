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
public class MySQLiteHelper extends SQLiteOpenHelper {

    private IDatabaseChangeListener listener;

    public static String table_todo = "table_todo";

    public MySQLiteHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int version) {
        super( context, name, factory, version );
    }

    public MySQLiteHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int version, DatabaseErrorHandler errorHandler) {
        super( context, name, factory, version, errorHandler );
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        if(listener != null){
            listener.onDatabaseCreate( db );
        }
        String sql = "create table if not exists " + table_todo + " (id integer primary key autoincrement, completeDate INTEGER, completeDateStr TEXT, content TEXT," +
                "date INTEGER, dateStr TEXT, status INTEGER, title TEXT, type INTEGER, userId INTEGER)";
        db.execSQL( sql );
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        if(listener != null){
            listener.onDatabaseUpgrade( db ,oldVersion,newVersion);
        }
    }

    @Override
    public void onDowngrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        if(listener != null){
            listener.onDatabaseDowngrade( db ,oldVersion,newVersion);
        }
    }

    @Override
    public void onConfigure(SQLiteDatabase db) {
        super.onConfigure( db );
    }

    public void setOnDatabaseChangeListener(IDatabaseChangeListener listener) {
        this.listener = listener;
    }

    public interface IDatabaseChangeListener{
        void onDatabaseCreate(SQLiteDatabase db);
        void onDatabaseUpgrade(SQLiteDatabase db, int oldVersion, int newVersion);
        void onDatabaseDowngrade(SQLiteDatabase db, int oldVersion, int newVersion);
    }
}
