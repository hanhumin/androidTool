package com.txl.lib.sqlite;

import android.content.Context;
import android.database.sqlite.SQLiteOpenHelper;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/10/19
 * description：
 */
public class SqliteUtils {
    SQLiteOpenHelper helper;

    public void createHelper(Context context){
        helper = new MySqliteHelper( context, MySqliteHelper.DATABESE_NAME, null,MySqliteHelper.DB_VERSION);
    }
}
