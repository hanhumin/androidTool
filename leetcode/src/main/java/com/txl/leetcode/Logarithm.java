package com.txl.leetcode;

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
}
