package com.example.java.javalable;

public class JavaLabel {
    public static void main(String[] args){
        testLabelBreak();
        testLabelC();
    }

    private static void testLabelBreak(){
        int m = 0;
        label1:
        while (true){
            label2:
            for (;;){
                m++;
                if(m == 500){
                    System.out.println("执行 break label2 ");
                    break label2;
                }
                if(m == 800){
                    break label1;
                }

            }
        }
        System.out.println("testLabelBreak finish m is "+m);
    }
    private static void testLabelC(){
        int m = 0;
        label1:
        while (true){
            System.out.println("testLabelC label loop");
            for (;;){
                m++;
                if(m == 500){

                    System.out.println("testLabelC label ");
                    //还会继续执行标签所在的循环
                    continue label1;
                }
                if(m > 800){
                    //结束标签所在的循环
                    break label1;
                }
            }
        }
        System.out.println("testLabelC finish m = "+m);
    }
}
