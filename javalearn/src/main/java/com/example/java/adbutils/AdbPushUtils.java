package com.example.java.adbutils;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;

public class AdbPushUtils {
    public static void main(String[] args) {
        //fixme 当push 路径中包含中文的时候文件的后缀名会丢失
        pushDirToAndroid("F:\\MediaProvider_test\\", "/system/media/", false);
//        deletePushFile("F:\\MediaProvider_test\\", "/system/media/", false);
    }

    /***
     * @param containCurrentDir 推送的时候是否包含当前文件夹
     * */
    private static void pushDirToAndroid(String sourceDir, String targetDir, boolean containCurrentDir) {
        Runtime runtime = Runtime.getRuntime();
        try {
            Process process = runtime.exec("adb devices");
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line = null;
            //第一排无用
            int count = -1;
            while ((line = bufferedReader.readLine()) != null && line.length() != 0) {
                System.out.println(line);
                count++;
            }
            bufferedReader.close();
            System.out.println("count = " + count);
            //当前只支持一个设备
            if (count != 1) {
                return;
            }
            System.out.println("start push  = ");
            if (containCurrentDir) {
                pushDirToAndroid(runtime, sourceDir, targetDir);
            } else {
                File file = new File(sourceDir);
                if (file.exists() && file.isDirectory()) {
                    File[] files = file.listFiles();
                    if (files != null) {
                        for (File f : files) {
                            pushDirToAndroid(runtime, f.getAbsolutePath(), targetDir);
                        }
                    }
                } else {
                    pushDirToAndroid(runtime, sourceDir, targetDir);
                }
            }

            System.out.println("end push  = ");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void callCmd(String cmd, Runtime runtime) {
        System.out.println("call cmd : " + cmd);
        try {
            Process p = runtime.exec(cmd);
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String line = null;
            int count = 0;
            while ((line = bufferedReader.readLine()) != null) {
                System.out.println((count++) + line);
            }
            bufferedReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    private static void pushDirToAndroid(Runtime runtime, String sourceDir, String targetDir) throws IOException {
        File file = new File(sourceDir);
        if (file.exists()) {//文件存在
            if (file.isDirectory()) {
                String target = targetDir  + file.getName() + "/";
                //创建文件夹
//                System.out.println("dir name = "+target);
//                showResult(runtime.exec("adb shell mkdir "+target));
                String cmd = "adb shell mkdir " + target;
                callCmd(cmd, runtime);
                File[] files = file.listFiles();
                if (files != null) {
                    for (File f : files) {
                        pushDirToAndroid(runtime, f.getAbsolutePath(), target);
                    }
                }
            } else if (file.isFile()) {
                String cmd = "adb push " + file.getAbsolutePath() + " " + targetDir;
                callCmd(cmd, runtime);
            }
        }
    }


    private static void deletePushFile(String sourceDir, String targetDir, boolean containCurrentDir){
        Runtime runtime = Runtime.getRuntime();
        try {
            Process process = runtime.exec("adb devices");
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line = null;
            //第一排无用
            int count = -1;
            while ((line = bufferedReader.readLine()) != null && line.length() != 0) {
                System.out.println(line);
                count++;
            }
            bufferedReader.close();
            System.out.println("count = " + count);
            //当前只支持一个设备
            if (count != 1) {
                return;
            }
            System.out.println("start push  = ");
            if (containCurrentDir) {
                deleteAndroidDir(runtime, sourceDir, targetDir);
            } else {
                File file = new File(sourceDir);
                if (file.exists() && file.isDirectory()) {
                    File[] files = file.listFiles();
                    if (files != null) {
                        for (File f : files) {
                            deleteAndroidDir(runtime, f.getAbsolutePath(), targetDir);
                        }
                    }
                } else {
                    deleteAndroidDir(runtime, sourceDir, targetDir);
                }
            }

            System.out.println("end push  = ");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void deleteAndroidDir(Runtime runtime, String sourceDir, String targetDir){
        File file = new File(sourceDir);
        if (file.exists()) {//文件存在
            if(file.isDirectory()){
                String cmd = "adb shell rm -rf "+targetDir+file.getName();
                callCmd(cmd,runtime);
            }else {
                String cmd = "adb shell rm -rf "+targetDir+file.getName();
                callCmd(cmd,runtime);
            }
        }

    }


}
