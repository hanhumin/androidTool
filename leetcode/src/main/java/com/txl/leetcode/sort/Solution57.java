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
//        int[][] re = new Solution57().insert(new int[][]{{1,3},{6,9}},new int[]{2,5});
//        int[][] re = new Solution57().insert(new int[][]{{1,5}},new int[]{2,7});
//        int[][] re = new Solution57().insert(new int[][]{{1,5}},new int[]{6,8});
        int[][] re = new Solution57().insert(new int[][]{{0,5},{9,12}},new int[]{7,16});
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
        if(intervals == null){
            throw new IllegalArgumentException("intervals null");
        }
        if(newInterval == null){
            throw new IllegalArgumentException("newInterval is null");
        }
        if(newInterval.length == 0){
            return intervals;
        }
        if(intervals.length == 0){
            return new  int[][]{{newInterval[0],newInterval[1]}};
        }
        int length = intervals.length;
        int [][] temp = new int[length+1][2];
        boolean isInsert = false;
        if(intervals[0][0] >= newInterval[0]){
            temp[0][0] = newInterval[0];
            temp[0][1] = newInterval[1];
            isInsert = true;
        }else {
            temp[0][0] =  intervals[0][0];
            temp[0][1] =  intervals[0][1];
        }
        int resultLength = 1;
        int[] merge = null;
        for (int i=0;i<length;i++){
            //1.插入区间
            //2.合并区间

            //step 1 插入区间
            if(!isInsert && intervals[i][0]>newInterval[0]){//在这个位置之前插入,说明i-1位置 intervals[i-1][0]<newInterval[0]
                //插入分情况  与当前位置的关系
                // 1.与当前位置相交；
                // 2.与当前位置相离：
                //与上一个位置元素的关系
                // 1.相交
                // 2.相离
//                intervals[i][0] = Math.min(intervals[i][0],newInterval[0]);
//                intervals[i][1] = Math.max(intervals[i][1],newInterval[1]);
                merge = newInterval;
                i--;//这一次合并的是newInterval  二没有处理 intervals[i] 执行--下一次处理
                isInsert = true;
            }else {
                merge = intervals[i];
            }

            //step2 区间合并
            if(merge[0]<=temp[resultLength-1][1]){//当前数与前一个有交集
                temp[resultLength-1][1] =  Math.max(merge[1],temp[resultLength-1][1]);
            }else {
                if(isInsert){//如果在已经插入数据的情况下，逻辑到这里 说明后面的数据只需要按序摆放到temp数据里面

                }
                temp[resultLength][0] =  merge[0];
                temp[resultLength][1] =  merge[1];
                resultLength++;
            }
        }
        //执行完区间合并后再次排查是不是需要将数据插入到最后一个
        if(!isInsert && temp[resultLength-1][0] <= newInterval[0]){
            if(newInterval[0] > temp[resultLength-1][1]){
                temp[resultLength][0] = newInterval[0];
                temp[resultLength][1] = newInterval[1];
                resultLength++;
            }else {
                temp[resultLength-1][1] = Math.max(temp[resultLength-1][1],newInterval[1]);
            }

            isInsert = true;
        }
        int[][] result = new int[resultLength][2];
        for (int i=0;i<resultLength;i++){
            result[i][0] = temp[i][0];
            result[i][1] = temp[i][1];
        }
        return result;
    }

    //思路2 归并 合成区间返回新生成的数组  分为两种情况，使用递归和不使用递归
    public int[][] mSort(){
        return new int[][]{};
    }

    /**
     * 合并数组并插入
     * */
    public int[][] mergeAndInsert(int[][] left, int[][] right){
        return new int[][]{};
    }

}
//leetcode submit region end(Prohibit modification and deletion)
