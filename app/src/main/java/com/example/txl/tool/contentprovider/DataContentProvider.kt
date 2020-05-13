package com.example.txl.tool.contentprovider

import android.content.ContentProvider
import android.content.ContentValues
import android.database.Cursor
import android.net.Uri
import android.util.Log

/**
 * Copyright (c) 2020 唐小陆 All rights reserved.
 * author：txl
 * date：2020/5/8
 * description：数据查询
 */
class DataContentProvider : ContentProvider() {
    private val tag = "DataContentProvider"

    private var dbHelper:DBHelper? = null

    override fun insert(uri: Uri, values: ContentValues?): Uri? {
        return uri
    }

    override fun query(uri: Uri, projection: Array<String>?, selection: String?, selectionArgs: Array<String>?, sortOrder: String?): Cursor? {
        val cursor  = dbHelper?.readableDatabase?.query(DBHelper.USER_TABLE_NAME,projection,null,selectionArgs,null,null,sortOrder)
        Log.d(tag,"call query cursor is $cursor")
        return cursor
    }

    override fun onCreate(): Boolean {
        dbHelper = DBHelper(this.context)
        val db = dbHelper?.writableDatabase

        db?.execSQL("delete from ${DBHelper.USER_TABLE_NAME}");
        db?.execSQL("insert into ${DBHelper.USER_TABLE_NAME} values(1,'XW');")
        db?.execSQL("insert into ${DBHelper.USER_TABLE_NAME} values(2,'XZ');")
        return true
    }

    override fun update(uri: Uri, values: ContentValues?, selection: String?, selectionArgs: Array<String>?): Int {
        return 0
    }

    override fun delete(uri: Uri, selection: String?, selectionArgs: Array<String>?): Int {
        return 0
    }

    override fun getType(uri: Uri): String? {
        return null
    }
}