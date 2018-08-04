package com.example.txl.tool.utils;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.List;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/7/31
 * description：
 */
public class FileUtils {

    private static final String TAG =  FileUtils.class.getSimpleName();
    public static SQLiteDatabase openDatabase(Context context,String packageName, String filePath) {

        String path = "data/data/"+packageName+ File.separator+"";
        Log.d( TAG,"path = "+path );

        return null;
    }

    public static File getDataFile(){
        File dataFile = Environment.getDataDirectory();
        Log.d( TAG,"path = "+dataFile.getAbsolutePath() );
        return dataFile;
    }

    public static List<String> getAppDataBases(Context context) throws FileNotFoundException {
        String path = "data/data/"+context.getPackageName()+ File.separator+"databases";
        File databasesFile = new File( path );
        if(!databasesFile.exists()){
            throw new FileNotFoundException( "文件不存在" );
        }else {
            String[] files = databasesFile.list(new DBFileFilter());
            List<String> list = new ArrayList<>(  );
            for (int i=0; i<files.length; i++){
                list.add( files[i] );
                Log.d( TAG,String.format( "file[%d] = %s",i,files[i] ) );
            }
            return list;
        }
    }

    public static class DBFileFilter implements FilenameFilter{

        public DBFileFilter() {
        }

        @Override
        public boolean accept(File dir, String name) {
            return !name.contains( "-journal" );
        }
    }
}
