package com.txl.leetcode.sort;//以数组 intervals 表示若干个区间的集合，其中单个区间为 intervals[i] = [starti, endi] 。请你合并所有重叠的区间，并返
//回一个不重叠的区间数组，该数组需恰好覆盖输入中的所有区间。
//
//
//
// 示例 1：
//
//
//输入：intervals = [[1,3],[2,6],[8,10],[15,18]]
//输出：[[1,6],[8,10],[15,18]]
//解释：区间 [1,3] 和 [2,6] 重叠, 将它们合并为 [1,6].
//
//
// 示例 2：
//
//
//输入：intervals = [[1,4],[4,5]]
//输出：[[1,5]]
//解释：区间 [1,4] 和 [4,5] 可被视为重叠区间。
//
//
//
// 提示：
//
//
// 1 <= intervals.length <= 104
// intervals[i].length == 2
// 0 <= starti <= endi <= 104
//
// Related Topics 排序 数组
// 👍 915 👎 0


import java.util.Arrays;
import java.util.Comparator;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution56 {
    public int[][] merge(int[][] intervals) {
        Arrays.sort(intervals, new Comparator<int[]>() {
            @Override
            public int compare(int[] o1, int[] o2) {
                return o1[0]-o2[0];
            }
        });
        int length = intervals.length;
        int size = 0;
        int lastEnd = intervals[0][1];
        for (int i=1;i<length;i++){
            if(intervals[i][0]>lastEnd){
                lastEnd = intervals[i][1];
                size++;
                intervals[size][0] = intervals[i][0];
                intervals[size][1] = intervals[i][1];
            }else {
                intervals[size][1] = lastEnd = Math.max(intervals[i][1],lastEnd);
            }
        }
        int[][] result = new int[size+1][2];
        for (int i=0;i<size;i++){
            result[i][0] = intervals[i][0];
            result[i][1] = intervals[i][1];
        }
        return result;
    }
}
//leetcode submit region end(Prohibit modification and deletion)
