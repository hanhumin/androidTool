package com.txl.leetcode.top100;
//给你一个整数数组 nums ，数组中的元素 互不相同 。返回该数组所有可能的子集（幂集）。
//
// 解集 不能 包含重复的子集。你可以按 任意顺序 返回解集。
//
//
//
// 示例 1：
//
//
//输入：nums = [1,2,3]
//输出：[[],[1],[2],[1,2],[3],[1,3],[2,3],[1,2,3]]
//
//
// 示例 2：
//
//
//输入：nums = [0]
//输出：[[],[0]]
//
//
//
//
// 提示：
//
//
// 1 <= nums.length <= 10
// -10 <= nums[i] <= 10
// nums 中的所有元素 互不相同
//
// Related Topics 位运算 数组 回溯
// 👍 1234 👎 0


import java.util.ArrayList;
import java.util.List;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution78 {
    public List<List<Integer>> subsets(int[] nums) {
        List<List<Integer>> ans = new ArrayList<>();
        pickOne(nums,0,new ArrayList<Integer>(),ans);
        return ans;
    }

    private void pickOne(int[] nums, int pickPosition,List<Integer> temp, List<List<Integer>> ans){
        if(pickPosition == nums.length){
            ans.add(new ArrayList<Integer>(temp));
            return;
        }
        pickOne(nums,pickPosition+1,temp,ans);

        temp.add(nums[pickPosition]);
        pickOne(nums,pickPosition+1,temp,ans);
        temp.remove(temp.size()-1);
    }
}
//leetcode submit region end(Prohibit modification and deletion)
