package com.txl.lib_annotationprocessor.annotationlearn.database;

import java.lang.annotation.Annotation;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;

/**
 * Copyright (c) 2020 唐小陆 All rights reserved.
 * author：txl
 * date：2020/5/31
 * description：
 */
public class TableCreator {
    public static void main(String[] args) throws Exception{
        if(args.length < 1){
            System.out.println("arguments: annotation classes");
            System.exit(0);
        }
        for (String className : args){
            Class<?> cl = Class.forName(className);
            DBTable dbTable = cl.getAnnotation(DBTable.class);
            if(dbTable == null){
                System.out.println("No DbTable annotation in class "+className);
                continue;
            }
            String tableName = dbTable.name();
            //if the name is empty use class name
            if(tableName.length() < 1){
                tableName = cl.getName().toUpperCase();
            }
            List<String> columnDefs = new ArrayList<>();
            for (Field field:cl.getDeclaredFields()){
                String columnName = null;
                Annotation[] annotations = field.getAnnotations();
                if(annotations.length < 1){
                    continue;
                }
                if(annotations[0] instanceof SQLInteger){
                    SQLInteger sqlInteger = (SQLInteger) annotations[0];
                    //use the filed name if name not specified
                    if(sqlInteger.name().length() < 1){
                        columnName = field.getName().toUpperCase();
                    }else {
                        columnName = sqlInteger.name();
                    }
                    columnDefs.add(columnName + " INT"+getConstraints(sqlInteger.constraints()));
                }
                if(annotations[0] instanceof SQLString){
                    SQLString sqlString = (SQLString) annotations[0];
                    //use the filed name if name not specified
                    if(sqlString.name().length() < 1){
                        columnName = field.getName().toUpperCase();
                    }else {
                        columnName = sqlString.name();
                    }
                    columnDefs.add(columnName + " VARCHAR("+sqlString.value()+")"+getConstraints(sqlString.constraints()));
                }
                StringBuilder createCommand = new StringBuilder("CREATE TABLE "+tableName+"(");
                for (String columnDef : columnDefs){
                    createCommand.append("\n    "+columnDef+",");
                }
                String tableCreate = createCommand.substring(0,createCommand.length())+");";
                System.out.println("Table Creation SQL for "+className+" is : \n"+tableCreate);

            }
        }
    }

    private static String getConstraints(Constraints con){
        String constraints = "";
        if(!con.allowNull()){
            constraints += " NOT NULL";
        }
        if(con.primaryKey()){
            constraints += " PRIMARY KEY";
        }
        if(con.unique()){
            constraints += " UNIQUE";
        }
        return constraints;
    }

}
