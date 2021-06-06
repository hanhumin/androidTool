package com.txl.player.utils;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

/**
 * 时间转换工具
 * */
public class TimeUtils {
    /**
     * @param seconds 时间 单位为秒
     * @param needShowHour 返回格式是否包含小时 true 返回格式  00:00:00    ;  false 返回格式  00:00
     * */
    public static String convertTimeFormat(int seconds,boolean needShowHour) {
        if(seconds<0)
            seconds=0;
//		seconds = seconds/1000;
        int hh = seconds / 3600;
        int mm = seconds % 3600 / 60;
        int ss = seconds % 60;
        String strTemp = null;
        if (0 != hh||needShowHour) {
            strTemp = String.format(Locale.getDefault(),"%02d:%02d:%02d", hh, mm, ss);
        }
        else {
            strTemp = String.format(Locale.getDefault(),"%02d:%02d", mm, ss);
        }
        return strTemp;
    }

    /**
     * 按照指定的格式转换时间
     * @param format "yyyy-MM-dd HH:mm:ss"
     * */
    public static String getCurrentDataFormat(String format){
        Date currentTime = new Date();
        return getDataFormat(currentTime,format);
    }

    /**
     * 按照指定的格式转换时间
     * @param format "yyyy-MM-dd HH:mm:ss"
     * @param date 时间
     * */
    public static String getDataFormat(Date date, String format){
        try {

            SimpleDateFormat formatter = new SimpleDateFormat(format);
            String dateString = formatter.format(date);
            return dateString;
        }catch (Exception e){
            e.printStackTrace();
        }
        return "";
    }
}
