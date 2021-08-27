package com.txl.leetcode.code;

import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;

public class Leetcode1 {
    public int[] twoSum(int[] nums, int target) {
//        Arrays.sort(nums);//不能对其排序，要得到原始位置  不能用双指针搞
        //key 存储值  value 存储位置
        HashMap<Integer,Integer> hashMap = new HashMap<>();
        for (int i=0;i<nums.length;i++){
            if(hashMap.containsKey(target-nums[i])){
                return new int[]{hashMap.get(target-nums[i]),i};
            }
            hashMap.put(nums[i],i);
        }
        return new int[0];
    }

    /**
     * 为什么  twoSum1  比 twoSum2 快
     * */
    public int[] twoSum1(int[] nums, int target) {
        for (int i = 1; i < nums.length; i++) {
            for (int j = i; j < nums.length; j++) {
                if (nums[j] + nums[j - i] == target) return new int[] {j, j - i};
            }
        }
        return new int[0];
    }

    public int[] twoSum2(int[] nums, int target) {
        for(int i=0 ;i< nums.length; i++ ){
            for(int j=i+1; j<nums.length; j++ ){
                if (nums[i] + nums[j] == target) return new int[] {i,j};
            }
        }
        return null;
    }
}
