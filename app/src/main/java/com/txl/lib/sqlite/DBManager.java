package com.txl.lib.sqlite;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import java.util.ArrayList;
import java.util.List;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/10/19
 * description：数据库管理
 */
public class DBManager implements MySQLiteHelper.IDatabaseChangeListener {
    public static final String DATABESE_NAME = "todo.db";
    public static int DB_VERSION = 1;

    private MySQLiteHelper helper;
    private SQLiteDatabase sqLiteDatabase;
    private List<MySQLiteHelper.IDatabaseChangeListener> listeners;

    public DBManager() {

    }

    public void createDatabase(Context context) {
        helper = new MySQLiteHelper( context, DATABESE_NAME, null, DB_VERSION);
        helper.setOnDatabaseChangeListener( this );
        sqLiteDatabase = helper.getReadableDatabase();
    }

    public DBManager(Context context, String dbName, int dbVersion) {
        helper = new MySQLiteHelper( context, dbName, null, dbVersion);
        sqLiteDatabase = helper.getReadableDatabase();
    }

    public void addDatabaseChangeListener(MySQLiteHelper.IDatabaseChangeListener listener){
        if (null == listeners){
            listeners = new ArrayList<>(  );
        }
        listeners.add( listener );
    }

    public SQLiteOpenHelper getHelper(){
        return helper;
    }

    public SQLiteDatabase getSqLiteDatabase() {
        return sqLiteDatabase;
    }

    public void destory(){
        if(null != listeners){
            listeners.clear();
        }
        sqLiteDatabase.close();
        helper.close();
    }

    @Override
    public void onDatabaseCreate(SQLiteDatabase db) {
        if(null != listeners){
            for (MySQLiteHelper.IDatabaseChangeListener listener : listeners){
                listener.onDatabaseCreate( db );
            }
        }
    }

    @Override
    public void onDatabaseUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        if(null != listeners){
            for (MySQLiteHelper.IDatabaseChangeListener listener : listeners){
                listener.onDatabaseUpgrade( db ,oldVersion,newVersion);
            }
        }
    }

    @Override
    public void onDatabaseDowngrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        if(null != listeners){
            for (MySQLiteHelper.IDatabaseChangeListener listener : listeners){
                listener.onDatabaseDowngrade( db ,oldVersion,newVersion);
            }
        }
    }
}
