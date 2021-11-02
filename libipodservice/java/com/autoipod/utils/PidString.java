package com.autoipod.utils;

public class PidString {
    static public String converPidArrayToStr(byte b[])
    {
        String str = "";
        for(int i=0;i<b.length;i++)
            str = str+converByteToStr(b[i]);
        return str;
    }

    static public String converByteToStr(byte b)
    {
        String str = Integer.toHexString(b);
        if(str.length() == 1)
            str = "0"+str;
        else if(str.length() > 2)
        {
            str = str.substring(str.length()-2,str.length());
        }
        str = "0x"+str;
        return str;
    }

    static public byte[] covertStrToByteArray(String str)
    {
        if(!str.startsWith("0x"))
            return null;
        String[] byteStrArray = str.split("0x");
        if(byteStrArray==null || byteStrArray.length == 0)
            return null;
        byte[] byteArray = new byte[byteStrArray.length-1];
        for(int j=1;j<byteStrArray.length;j++) {
            byteArray[j-1] = Integer.valueOf(byteStrArray[j], 16).byteValue();
            // Log.i("ppp","converByteToStr[j]="+converByteToStr(byteArray[j-1] ));

        }
        return byteArray;
    }
}
