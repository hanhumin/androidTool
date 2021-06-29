package com.txl.leetcode.top100;

//给定一个不含重复数字的数组 nums ，返回其 所有可能的全排列 。你可以 按任意顺序 返回答案。
//
//
//
// 示例 1：
//
//
//输入：nums = [1,2,3]
//输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
//
//
// 示例 2：
//
//
//输入：nums = [0,1]
//输出：[[0,1],[1,0]]
//
//
// 示例 3：
//
//
//输入：nums = [1]
//输出：[[1]]
//
//
//
//
// 提示：
//
//
// 1 <= nums.length <= 6
// -10 <= nums[i] <= 10
// nums 中的所有整数 互不相同
//
// Related Topics 数组 回溯
// 👍 1424 👎 0


import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution46 {
    public List<List<Integer>> permute(int[] nums) {
        List<Integer> output = new ArrayList<>();
        for (int i=0;i<nums.length;i++){
            output.add(nums[i]);
        }
        List<List<Integer>> result = new ArrayList<>();
        backtrack(result,output,0,nums.length);
        return result;
    }

    private void backtrack(List<List<Integer>> result,List<Integer> output,int position,int end) {
        if(position == end){
            result.add(new ArrayList<Integer>(output));
            return;
        }
        for (int i=position;i<end;i++){
            Collections.swap(output,i,position);
            backtrack(result,output,position+1,end);
            Collections.swap(output,i,position);
        }
    }

//    /**
//     * 回溯
//     * */
//    private List<List<Integer>> solution1(int[] nums){
//        List<List<Integer>> res = new ArrayList<List<Integer>>();
//
//        List<Integer> output = new ArrayList<Integer>();
//        for (int num : nums) {
//            output.add(num);
//        }
//
//        int n = nums.length;
//        backtrack(n, output, res, 0);
//        return res;
//    }
//
//    void backtrack(int n, List<Integer> output, List<List<Integer>> res, int first) {
//        // 所有数都填完了
//        if (first == n) {
//            res.add(new ArrayList<Integer>(output));
//        }
//        for (int i = first; i < n; i++) {
//            // 动态维护数组
//            Collections.swap(output, first, i);
//            // 继续递归填下一个数
//            backtrack(n, output, res, first + 1);
//            // 撤销操作
//            Collections.swap(output, first, i);
//        }
//    }
}
//leetcode submit region end(Prohibit modification and deletion)
