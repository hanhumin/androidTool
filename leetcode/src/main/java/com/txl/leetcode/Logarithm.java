package com.txl.leetcode;

import java.util.ArrayList;

/**
 * 对数器
 * */
public class Logarithm {
    /**
     * 生成一个长度为size，且以第一个元素排序无重叠的数据
     * @param size  数组长度
     * @param value 数据的范围
     * */
    public static int[][] generateStartSortArray(int size,int value){
        int length = (int) (Math.random() * size);
        int[][] res = new int[length][2];
        int lastEnd = 0;
        for(int i=0;i<length;i++){
            int v1 = (int) (Math.random()*value);
            int v2 = (int) (Math.random()*value);
            if(v2 < v1){
                int temp = v1;
                v1 = v2;
                v2 = temp;
            }
            if(v1 <= lastEnd){
                v1 += lastEnd;
                v2 += lastEnd;
            }
            if(v1 == lastEnd){//在此检测v1的值，当上一轮检测的时候，如果v1 = 0 那么 现在 v1 == lastEnd
                v1 += 1;
                v2 += 1;
            }
            lastEnd = v2;
            res[i][0] = v1;
            res[i][1] = v2;
        }
        return res;
    }

    /**
     * 随机生成一个长度为2，起始位置在 value的有序数组
     * */
    public static int[] generateInterval(int value){
        int v1 = (int) (Math.random()*value);
        int v2 = (int) (Math.random()*value);
        if(v2 < v1){
            int temp = v1;
            v1 = v2;
            v2 = temp;
        }
        return new int[]{v1,v2};
    }


    /**
     * @param m 最大参赛人数
     * @param n 最大投票人数
     * */
    public static String[] generate1366(int m, int n){
        int rm = (int) (Math.random()*m);
        int rn = (int) (Math.random()*n);
        String[] strings = new String[rn];
        String ss = createNoRepetitionString(rm);
        for (int i=0; i< rn;i++){
            strings[i] = randomString(ss);
        }
        return strings;
    }

    /**
     * 创建无重复字符串
     * */
    private static String createNoRepetitionString(int l){
        ArrayList<Character> arrayList = new ArrayList<>();
        arrayList.add('A');
        arrayList.add('B');
        arrayList.add('C');
        arrayList.add('D');
        arrayList.add('E');
        arrayList.add('F');
        arrayList.add('G');
        arrayList.add('H');
        arrayList.add('I');
        arrayList.add('J');
        arrayList.add('K');
        arrayList.add('L');
        arrayList.add('M');
        arrayList.add('N');
        arrayList.add('O');
        arrayList.add('P');
        arrayList.add('Q');
        arrayList.add('R');
        arrayList.add('S');
        arrayList.add('T');
        arrayList.add('U');
        arrayList.add('V');
        arrayList.add('W');
        arrayList.add('X');
        arrayList.add('Y');
        arrayList.add('Z');

        StringBuilder sb = new StringBuilder();
        if(l > 26){
            l = 26;
        }
        for(int i=0; i<l;i++){
            int m = (int) (Math.random()*arrayList.size());
            sb.append(arrayList.remove(m));
        }
        return new String(sb);
    }

    private static String randomString(String string){
        if(string.length() <= 1){
            return string;
        }
        StringBuilder stringBuilder = new StringBuilder();
        ArrayList<Integer> integers = new ArrayList<>();
        for (int i=0;i<string.length();i++){
            integers.add(i);
        }
        while (integers.size() > 0){
            int index = (int) (Math.random()*integers.size());
            Integer integer = integers.remove(index);
            stringBuilder.append(string.charAt(integer));
        }
        return new String(stringBuilder);
    }
}
