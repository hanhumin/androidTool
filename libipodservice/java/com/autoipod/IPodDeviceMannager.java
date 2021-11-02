package com.autoipod;

import android.content.Context;
import android.support.v4.util.ArrayMap;
import java.util.Map;

public class IPodDeviceMannager {

    private static Map<String,IPodDevice> deviceMap = new ArrayMap<>();
    private static String activeUUID;

    public static void addIpodDevice(Context context,String uuid)
    {
        activeUUID = uuid;
        IPodDevice ipodDevice = new IPodDevice(context,uuid);
        deviceMap.put(uuid,ipodDevice);
    }

    public static IPodDevice getActiveDevice()
    {
        return deviceMap.get(activeUUID);
    }


    public static IPodDevice getIPodDevice(String uuid)
    {
        return deviceMap.get(uuid);
    }

    public static void removeIPodDevice(String uuid)
    {
        deviceMap.remove(uuid);
    }

}
