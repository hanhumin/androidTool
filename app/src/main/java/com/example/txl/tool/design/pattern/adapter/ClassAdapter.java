package com.example.txl.tool.design.pattern.adapter;

public class ClassAdapter extends Voltage220 implements Voltage5{
    @Override
    public int getVoltage5() {
        int v =  super.getVoltage();
        //一些列变化返回5v电压
        int result =  v/44;
        return result;
    }
}
