package com.example.java.thread;

public class TestThread {
    public static void main(String[] args){
        testThreadJoin();
    }


    private static void testThreadJoin(){
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                System.out.println("thread print");
                try {
                    Thread.sleep(5000);
                    System.out.println("thread print sleep end");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });
        try {
            thread.start();
            thread.join();
            System.out.println("main print");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
