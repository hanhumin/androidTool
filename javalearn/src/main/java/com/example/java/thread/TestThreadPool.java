package com.example.java.thread;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Executor;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.PriorityBlockingQueue;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

public class TestThreadPool {
    public static void main(String[] args){
//        test01();
//        test02();
//        test03();
//        test04();
        testGlideAnimationPool();
    }

    //测试，没有核心线程，只有非核心  会创建多少个 线程
    private static void test02(){

        ThreadPoolExecutor executor = new ThreadPoolExecutor(0, 5, 30, TimeUnit.SECONDS, new ArrayBlockingQueue<Runnable>(128), new ThreadFactory() {
            @Override
            public Thread newThread(Runnable r) {
                System.out.println("test02 create new thread");
                return new Thread(r,"test02");
            }
        });

        for (int i=0;i<100;i++){
            final int temp = i;
            executor.execute(new TestRunnable() {
                @Override
                public void run() {
                    System.out.println("test02 我是第 "+temp+"个");
                }
            });
        }
        executor.shutdown();
    }
    //测试，没有核心线程，只有非核心  会创建多少个 线程  采用LinkedBlockingQueue 实现 workQueue
    private static void test03(){
        ThreadPoolExecutor executor = new ThreadPoolExecutor(0, 5, 30, TimeUnit.SECONDS, new LinkedBlockingQueue<Runnable>(), new ThreadFactory() {
            @Override
            public Thread newThread(Runnable r) {
                System.out.println("test03 create new thread");
                return new Thread(r,"test03");
            }
        });

        for (int i=0;i<100;i++){
            final int temp = i;
            executor.execute(new TestRunnable() {
                @Override
                public void run() {
                    System.out.println("test03 我是第 "+temp+"个");
                }
            });
        }
        executor.shutdown();
    }

    private static void test04(){
        ThreadPoolExecutor executor = new ThreadPoolExecutor(0, 5, 30, TimeUnit.SECONDS, new SynchronousQueue<Runnable>(), new ThreadFactory() {
            @Override
            public Thread newThread(Runnable r) {
                System.out.println("test04 create new thread");
                return new Thread(r,"test04");
            }
        });

        for (int i=0;i<100;i++){
            final int temp = i;
            executor.execute(new TestRunnable() {
                @Override
                public void run() {
                    System.out.println("test04 我是第 "+temp+"个");
                }
            });
        }
        executor.shutdown();
    }


    //测试在workQueue没有占满的时候会不会开启非核心线程
    private static void test01(){
        //总线程数为5  核心线程数为1  非核心4  测试是否会启动非核心线程在workQueue没有占满的时候
        ThreadPoolExecutor executor = new ThreadPoolExecutor(1, 5, 30, TimeUnit.SECONDS, new ArrayBlockingQueue<Runnable>(128), new ThreadFactory() {
            @Override
            public Thread newThread(Runnable r) {
                System.out.println("create new thread");
                return new Thread(r,"test01");
            }
        });

        for (int i=0;i<100;i++){
            final int temp = i;
            executor.execute(new TestRunnable() {
                @Override
                public void run() {
                    System.out.println("我是第 "+temp+"个");
                }
            });
        }
        executor.shutdown();
    }

    abstract static class TestRunnable implements Runnable,Comparable{
        @Override
        public int compareTo(Object o) {
            return -1;
        }

    }

    private static void testGlideAnimationPool(){
        ThreadPoolExecutor threadPoolExecutor = new ThreadPoolExecutor(
                0 /* corePoolSize */,
                4,
                100,
                TimeUnit.MILLISECONDS,
                new PriorityBlockingQueue<Runnable>()
                , new ThreadFactory() {
            @Override
            public Thread newThread(Runnable r) {
                System.out.println("create thread");
                return new Thread(r);
            }
        });
        threadPoolExecutor.execute(new TestRunnable() {
            @Override
            public void run() {
                try {
                    Thread.sleep(5000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println("我是第一个sleep 5s");
            }
        });
        threadPoolExecutor.execute(new TestRunnable() {
            @Override
            public void run() {
                try {
                    Thread.sleep(4000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println("我是第二个sleep 4s");
            }
        });
        threadPoolExecutor.execute(new TestRunnable() {
            @Override
            public void run() {
                try {
                    Thread.sleep(3000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println("我是第三个sleep 3s");
            }
        });
    }
}
