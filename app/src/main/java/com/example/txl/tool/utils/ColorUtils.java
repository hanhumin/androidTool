package com.example.txl.tool.utils;

import android.graphics.Color;

import java.util.Random;

/**
 * @author TXL
 * description :
 */
public class ColorUtils {
    public static int randomColor(){
        Random random = new Random();
//        int []ranColor ={0x00FFFFCC,0xCCFFFF,0xff000000,0x00ffff00,0x00FFFFCC,0x00ff0000}; //0xff000000 | random.nextInt(0x00ffffff);
//        int randomcolor =random.nextInt(ranColor.length);
        int ranColor = 0xff000000 | random.nextInt(0x00ffffff);
        int color = Color.argb(1, random.nextInt(255) / 255, random.nextInt(255) / 255, random.nextInt(255) / 255);
        return ranColor;
    }
}
