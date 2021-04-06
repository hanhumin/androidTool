package com.txl.leetcode;

import java.util.HashSet;
import java.util.Iterator;

//给你一个区间列表，请你删除列表中被其他区间所覆盖的区间。
//
// 只有当 c <= a 且 b <= d 时，我们才认为区间 [a,b) 被区间 [c,d) 覆盖。
//
// 在完成所有删除操作后，请你返回列表中剩余区间的数目。
//
//
//
// 示例：
//
//
//输入：intervals = [[1,4],[3,6],[2,8]]
//输出：2
//解释：区间 [3,6] 被区间 [2,8] 覆盖，所以它被删除了。
//
//
//
//
// 提示：
//
//
// 1 <= intervals.length <= 1000
// 0 <= intervals[i][0] < intervals[i][1] <= 10^5
// 对于所有的 i != j：intervals[i] != intervals[j]
//
// Related Topics 贪心算法 排序 Line Sweep
// 👍 40 👎 0
public class Solution1288 {
    //自己处理的第一种解法，暴力求解
    public int removeCoveredIntervals(int[][] intervals) {
        int length = intervals.length;
        HashSet<Node> items = new HashSet<>();
        //两种情况，当前数组中的元素大于插入的元素;插入的元素大于数组中的元素。
        for (int i = 0; i < length; i++) {
            HashSet<Node> removeItems = new HashSet<>();
            Iterator<Node> iterator = items.iterator();
            Node insert = new Node();
            insert.start = intervals[i][0];
            insert.end = intervals[i][1];


            boolean needInsert = true;
            while (iterator.hasNext()){
                Node node = iterator.next();
                if(node.start <= intervals[i][0] && node.end >= intervals[i][1]){//待插入的数据被包含
                    needInsert = false;
                    break;
                }else if(node.start >= intervals[i][0] && node.end <= intervals[i][1]){
                    removeItems.add(node);
                }
            }
            items.removeAll(removeItems);
            if(needInsert){
                items.add(insert);
            }
        }
        return items.size();
    }

    static class Node{
        int start;
        int end;

        @Override
        public boolean equals(Object o) {
            if(o instanceof Node){
                if(this.start == ((Node) o).start && this.end == ((Node) o).end){
                    return true;
                }
            }
            return false;
        }
    }
}
