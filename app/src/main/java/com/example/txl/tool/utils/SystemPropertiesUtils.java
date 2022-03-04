package com.example.txl.tool.utils;

import java.lang.reflect.Method;

public class SystemPropertiesUtils {
    private static Class<?> SystemPropertiesClass;

    static {
        try {
            SystemPropertiesClass = Class.forName("android.os.SystemProperties");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    public static String getProperty(String key, String defaultValue) {
        String value = defaultValue;
        try {
            Class<?> c = SystemPropertiesClass;
            Method get = c.getMethod("get", String.class, String.class);
            value = (String) (get.invoke(c, key, "unknown"));
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            return value;
        }
    }

    public static void setProperty(String key, String value) {
        try {
            Class<?> c = SystemPropertiesClass;
            Method set = c.getMethod("set", String.class, String.class);
            set.invoke(c, key, value );
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static boolean getBoolean(String key, boolean defaultValue){
        try {
            Class<?> c = SystemPropertiesClass;
            Method get = c.getMethod("getBoolean", String.class, Boolean.class);
            return (Boolean) (get.invoke(c, key, defaultValue));
        } catch (Exception e) {
            e.printStackTrace();
            return defaultValue;
        }
    }
}
