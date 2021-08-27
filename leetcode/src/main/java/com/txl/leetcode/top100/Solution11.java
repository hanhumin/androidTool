package com.txl.leetcode.top100;

/**
 * Copyright (c) 2021, 唐小陆 All rights reserved.
 * author：txl
 * date：2021/6/2
 * description：
 */
//给你 n 个非负整数 a1，a2，...，an，每个数代表坐标中的一个点 (i, ai) 。在坐标内画 n 条垂直线，垂直线 i 的两个端点分别为 (i,
//ai) 和 (i, 0) 。找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。
//
// 说明：你不能倾斜容器。
//
//
//
// 示例 1：
//
//
//
//
//输入：[1,8,6,2,5,4,8,3,7]
//输出：49
//解释：图中垂直线代表输入数组 [1,8,6,2,5,4,8,3,7]。在此情况下，容器能够容纳水（表示为蓝色部分）的最大值为 49。
//
// 示例 2：
//
//
//输入：height = [1,1]
//输出：1
//
//
// 示例 3：
//
//
//输入：height = [4,3,2,1,4]
//输出：16
//
//
// 示例 4：
//
//
//输入：height = [1,2,1]
//输出：2
//
//
//
//
// 提示：
//
//
// n = height.length
// 2 <= n <= 3 * 104
// 0 <= height[i] <= 3 * 104
//
// Related Topics 数组 双指针
// 👍 2513 👎 0


//leetcode submit region begin(Prohibit modification and deletion)
class Solution11{
    public static void main(String[] args){

    }

    /**
     * 再次刷这个题目
     * */
    public int myMaxArea(int[] height){
        int max = 0;
        for (int i=0,j=height.length-1;i<j;){
            int len = j-i;
            int h = 0;
            if(height[i]<height[j]){//谁小，那么高度就以此为准，谁小就动谁，这样才有可能找到一个更大的矩形面积。如果动最大的，就是最小面积
                h = height[i];
                i++;
            }else {
                h = height[j];
               j--;
            }
            max = Math.max( max,h*len );
        }
        return max;
    }


    /**
     * 双指针
     * */
    public int maxArea(int[] height) {
        int max = 0;
        int start = 0, end = height.length-1;
        while (start<end){
            int len = end-start;
            int h = 0;
            if(height[start] <= height[end]){
                h = height[start];
                start++;
            }else {
                h = height[end];
                end--;
            }
            max = Math.max( max,h*len );
        }
        return max;
    }

    //暴力循环
    public int maxArea1(int[] height) {
        int max = 0;
        for (int l = 1; l<height.length; l++){
            for (int i=0;i<height.length;i++){
                int j = i+l;
                if(j>=height.length){
                    break;
                }
                int h = Math.min( height[i],height[j] );
                int result = l * h;
                if(result > max){
                    max = result;
                }
            }
        }
        return max;
    }
}
//leetcode submit region end(Prohibit modification and deletion)
