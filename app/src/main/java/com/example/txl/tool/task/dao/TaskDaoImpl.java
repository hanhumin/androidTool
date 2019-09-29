package com.example.txl.tool.task.dao;

import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import androidx.annotation.NonNull;

import java.util.List;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/10/23
 * description：
 */
public class TaskDaoImpl implements ITaskDao<TaskBean> {

    private String table_name = "table_todo";
    private SQLiteOpenHelper helper;

    public TaskDaoImpl(@NonNull SQLiteOpenHelper helper) {
        this.helper = helper;
        createTable();
    }

    @Override
    public void createTable() {
        //在本地没有数据服务器有数据的情况下可能会出现id主键冲突。如何解决？
        String sql = "create table if not exists " + table_name + " (id integer primary key autoincrement, taskid integer,completeDate INTEGER, completeDateStr TEXT, content TEXT," +
                "date INTEGER, dateStr TEXT, status INTEGER, title TEXT, type INTEGER, userId INTEGER)";
        SQLiteDatabase database = helper.getWritableDatabase();
        database.execSQL( sql );
        database.close();
    }

    @Override
    public List<TaskBean> queryAllTask() {
        SQLiteDatabase database = helper.getWritableDatabase();
        Cursor cursor = database.query( table_name,null,"",null,"","","" );
        while (cursor.moveToNext()){

        }
        cursor.close();
        database.close();
        return null;
    }

    @Override
    public void queryTaskByStatus() {

    }

    @Override
    public void addTask(TaskBean task) {
        String key = " (taskid, completeDate, completeDateStr, content, date, dateStr, status, title, type, userId) ";
        String value = task.getTaskId() +", "+task.getCompleteDate()+","+task.getCompleteDateStr()+","+task.getContent()+","+task.getDate()+","+task.getDateStr()+","
                +task.getStatus()+","+task.getTitle()+","+task.getType()+","+task.getUserId();
        String sql = "insert into "+table_name+key+" VALUES  ("+value+")";
        SQLiteDatabase database = helper.getWritableDatabase();
        database.execSQL( sql );
        database.close();
    }

    @Override
    public void update(TaskBean task) {
        String value = "completeDate = "+task.getCompleteDate()+
                ",completeDateStr = "+task.getCompleteDateStr()+
                ", content = "+task.getContent()+
                ",date = "+task.getDate()+
                ",dateStr = "+task.getDateStr()+
                ",status = "+task.getStatus()+
                ",title = "+task.getTitle()+
                ",type = "+task.getType()+
                ",userId = "+task.getUserId();
        String sql = "UPDATE "+table_name+" SET "+value+" WHERE taskid ="+task.getTaskId();
        SQLiteDatabase database = helper.getWritableDatabase();
        database.execSQL( sql );
        database.close();
    }

    @Override
    public void delete() {

    }

    @Override
    public void onDatabaseCreate(SQLiteDatabase db) {

    }

    @Override
    public void onDatabaseUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        if(newVersion == 2 && oldVersion == 1){
            String sql = "alter table  " + table_name + " add column taskid integer";
            db.execSQL( sql );
        }
    }

    @Override
    public void onDatabaseDowngrade(SQLiteDatabase db, int oldVersion, int newVersion) {

    }
}
