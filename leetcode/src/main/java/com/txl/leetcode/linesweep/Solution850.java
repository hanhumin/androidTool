package com.txl.leetcode.linesweep;

import java.util.Arrays;
import java.util.Comparator;

//我们给出了一个（轴对齐的）矩形列表 rectangles 。 对于 rectangle[i] = [x1, y1, x2, y2]，其中（x1，y1）是矩形
// i 左下角的坐标，（x2，y2）是该矩形右上角的坐标。
//
// 找出平面中所有矩形叠加覆盖后的总面积。 由于答案可能太大，请返回它对 10 ^ 9 + 7 取模的结果。
//
//
//
// 示例 1：
//
// 输入：[[0,0,2,2],[1,0,2,3],[1,0,3,1]]
//输出：6
//解释：如图所示。
//
//
// 示例 2：
//
// 输入：[[0,0,1000000000,1000000000]]
//输出：49
//解释：答案是 10^18 对 (10^9 + 7) 取模的结果， 即 (10^9)^2 → (-7)^2 = 49 。
//
//
// 提示：
//
//
// 1 <= rectangles.length <= 200
// rectanges[i].length = 4
// 0 <= rectangles[i][j] <= 10^9
// 矩形叠加覆盖后的总面积不会超越 2^63 - 1 ，这意味着可以用一个 64 位有符号整数来保存面积结果。
//
// Related Topics 线段树 Line Sweep
// 👍 71 👎 0
public class Solution850 {
    int maxY = 0;

    public int rectangleArea(int[][] rectangles) {

        int ans = 0;
        Arrays.sort(rectangles, new Comparator<int[]>() {
            @Override
            public int compare(int[] ints, int[] t1) {
                maxY = Math.max(maxY,t1[3]);
                return ints[0] == t1[0]?ints[3]-t1[3]:ints[0]-t1[0];
            }
        });
        for (int curent[]:rectangles){
            System.out.println("sort value is { "+curent[0]+", "+curent[1]+", "+curent[2]+", "+curent[3]+"}");
        }
        for (int i=1;i<=maxY;i++){//i 代表y坐标
            int[] end, prev_end = null;
            for (int[] curr : rectangles) {

                end = curr;
                if(prev_end != null){
                    if (prev_end[2] >= end[2]) {//后续被覆盖,不做处理

                    }else if(end[0]>prev_end[2]){//后续与前一个完全独立
                        if(i>curr[1] && i <= curr[3]) {//不在此区间范围内
                            ans += end[2]-end[0];
                        }
                        prev_end = end;
                    }else {//有相交区域
                        if(i>curr[1] && i <= curr[3]) {//不在此区间范围内
                            ans += end[2]-prev_end[2];
                        }
                        prev_end = end;
                    }
                }else {
                    if(i>curr[1] && i <= curr[3]){//不在此区间范围内
                        ans += end[2]-end[0];
                    }
                    System.out.println("first add num ans "+ans+"  num is "+(end[2]-end[0]));
                    prev_end = end;
                }
            }
        }
        long MOD = 1_000_000_007;
        ans %= MOD;
        return ans;
    }

    public static void main(String[] args){
        int ans = 5;
        ans += 5-3;
        System.out.println("ans is "+ans);
        System.out.println("result is "+new Solution850().rectangleArea(new int[][]{{0,0,2,2},{1,0,2,3},{1,0,3,1}}));
    }
}
