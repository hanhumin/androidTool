package com.example.txl.tool.utils;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Copyright (c) 2019, 唐小陆 All rights reserved.
 * author：txl
 * date：2019/7/11
 * description：对时间进行比较
 */
public class DateCompare {
    public static int compareDate(String data1, String data2, String format){
        DateFormat df = new SimpleDateFormat(format);
        try {
            Date dt1 = df.parse(data1);
            Date dt2 = df.parse(data2);
            if (dt1.getTime() > dt2.getTime()) {
                return 1;
            } else if (dt1.getTime() < dt2.getTime()) {
                return -1;
            } else {
                return 0;
            }
        } catch (Exception exception) {
            exception.printStackTrace();
        }
        return 0;
    }
}
