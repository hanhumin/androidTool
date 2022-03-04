package com.example.txl.tool.inter.process.communication;


import com.example.txl.tool.utils.SystemPropertiesUtils;

public class AidlConfig {
    public static boolean TEST_BINDER_SERVICE = true;

    public static void config() {
        TEST_BINDER_SERVICE
                = SystemPropertiesUtils.getBoolean("persist.txl.test.binder.service", true);
    }
}
