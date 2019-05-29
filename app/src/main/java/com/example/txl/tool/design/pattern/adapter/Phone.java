package com.example.txl.tool.design.pattern.adapter;

public class Phone {
    Voltage5 voltage5;

    public void setAdapter(Voltage5 voltage5){
        this.voltage5 = voltage5;
    }

    /**
     * 充电
     * */
    public void power(){
        System.out.println("充电电压 "+voltage5.getVoltage5()+" V");
    }

    public static void main(String[] args){
        Phone phone = new Phone();

        ObjectAdapter objectAdapter = new ObjectAdapter();
        phone.setAdapter(objectAdapter);
        phone.power();

        //出去旅游
        OtherAdapter otherAdapter = new OtherAdapter();
        phone.setAdapter(otherAdapter);
        phone.power();
    }
}
