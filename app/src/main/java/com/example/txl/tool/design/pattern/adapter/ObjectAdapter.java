package com.example.txl.tool.design.pattern.adapter;

public class ObjectAdapter implements Voltage5 {
    Voltage220 voltage220 = new Voltage220();
    @Override
    public int getVoltage5() {
        return voltage220.voltage/44;
    }
}
