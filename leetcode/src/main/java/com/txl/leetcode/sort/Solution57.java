package com.txl.leetcode.sort;
//给你一个 无重叠的 ，按照区间起始端点排序的区间列表。
//
// 在列表中插入一个新的区间，你需要确保列表中的区间仍然有序且不重叠（如果有必要的话，可以合并区间）。
//
//
//
// 示例 1：
//
//
//输入：intervals = [[1,3],[6,9]], newInterval = [2,5]
//输出：[[1,5],[6,9]]
//
//
// 示例 2：
//
//
//输入：intervals = [[1,2],[3,5],[6,7],[8,10],[12,16]], newInterval = [4,8]
//输出：[[1,2],[3,10],[12,16]]
//解释：这是因为新的区间 [4,8] 与 [3,5],[6,7],[8,10] 重叠。
//
// 示例 3：
//
//
//输入：intervals = [], newInterval = [5,7]
//输出：[[5,7]]
//
//
// 示例 4：
//
//
//输入：intervals = [[1,5]], newInterval = [2,3]
//输出：[[1,5]]
//
//
// 示例 5：
//
//
//输入：intervals = [[1,5]], newInterval = [2,7]
//输出：[[1,7]]
//
//
//
//
// 提示：
//
//
// 0 <= intervals.length <= 104
// intervals[i].length == 2
// 0 <= intervals[i][0] <= intervals[i][1] <= 105
// intervals 根据 intervals[i][0] 按 升序 排列
// newInterval.length == 2
// 0 <= newInterval[0] <= newInterval[1] <= 105
//
// Related Topics 排序 数组
// 👍 413 👎 0


import java.util.Arrays;
import java.util.Comparator;


class Solution57 {



    public static void main(String[] args){
        int[][] re = new Solution57().insert(new int[][]{{1,3},{6,9}},new int[]{2,5});
        for (int i=0;i<re.length;i++){
            System.out.println("item is "+re[i][0]+"  "+re[i][1]);
        }
        re = new Solution57().insert(new int[][]{{1,2},{3,5},{6,7},{8,10},{12,16}},new int[]{4,8});
        System.out.println("========================================");
        for (int i=0;i<re.length;i++){
            System.out.println("item is "+re[i][0]+"  "+re[i][1]);
        }
    }

    //思路1  暴力求解
    public int[][] insert(int[][] intervals, int[] newInterval) {
        if(intervals == null || intervals.length == 0){
            throw new IllegalArgumentException("intervals is empty or null");
        }
        if(newInterval == null){
            throw new IllegalArgumentException("newInterval is null");
        }
        if(newInterval.length == 0){
            return intervals;
        }
        int length = intervals.length;
        int resultLength = 0;
        for (int i=0;i<length;i++){
            if(intervals[i][1]<newInterval[0] || newInterval[1]<intervals[i][0]){//区间外
                resultLength++;
            }else if(intervals[i][1]>=newInterval[0] && newInterval[0]>=intervals[i][0]){//区间向后
                intervals[i][1] = Math.max( intervals[i][1],newInterval[1]);
                resultLength++;
            }else {//区间向前,需要考虑区间合并的事情
                intervals[i][0] = Math.min( intervals[i][0],newInterval[0]);
                if(intervals[resultLength][1]<=intervals[i][0]){//区间能够合并,考虑一次需要合并多少区间？是否存在向前推导呢？
                    intervals[resultLength][1] = Math.max( intervals[resultLength][1],intervals[i][1]);
                }else {
                    resultLength++;
                }
            }
        }
        int[][] result = new int[resultLength][2];
        for (int i=0;i<resultLength;i++){
            result[i][0] = intervals[i][0];
            result[i][1] = intervals[i][1];
        }
        return result;
    }

    //思路2 归并 合成区间返回新生成的数组  分为两种情况，使用递归和不使用递归
}
//leetcode submit region end(Prohibit modification and deletion)
