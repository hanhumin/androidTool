package com.txl.leetcode.top100;

//实现获取 下一个排列 的函数，算法需要将给定数字序列重新排列成字典序中下一个更大的排列。
//
// 如果不存在下一个更大的排列，则将数字重新排列成最小的排列（即升序排列）。
//
// 必须 原地 修改，只允许使用额外常数空间。
//
//
//
// 示例 1：
//
//
//输入：nums = [1,2,3]
//输出：[1,3,2]
//
//
// 示例 2：
//
//
//输入：nums = [3,2,1]
//输出：[1,2,3]
//
//
// 示例 3：
//
//
//输入：nums = [1,1,5]
//输出：[1,5,1]
//
//
// 示例 4：
//
//
//输入：nums = [1]
//输出：[1]
//
//
//
//
// 提示：
//
//
// 1 <= nums.length <= 100
// 0 <= nums[i] <= 100
//
// Related Topics 数组
// 👍 1175 👎 0


import java.util.Arrays;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution31 {
    public void nextPermutation(int[] nums) {
        if(nums == null || nums.length == 1){
            return;
        }
        for (int i=nums.length-1;i>=0;i--){
            int origin = nums[i];
            for (int j=i;j<nums.length;j++){
                if( origin < nums[j]){
                    if(nums[i] > nums[j] || origin == nums[i]){//
                        int temp = nums[i];
                        nums[i] = nums[j];
                        nums[j] = temp;
                    }
                }
            }
            if(nums[i] != origin){//被交换过
                order(i+1,nums.length-1,nums);
                return;
            }
        }
        order(0,nums.length-1,nums);
    }

    private void order(int start,int end,int[] nums){
        if(start == end){
            return;
        }
        int[] temp = new int[end+1-start];
        if (end - start + 1 >= 0) System.arraycopy(nums, start, temp, 0, end - start + 1);
        Arrays.sort(temp);
        if (end - start + 1 >= 0) System.arraycopy(temp, 0, nums, start, end - start + 1);
    }
}
//leetcode submit region end(Prohibit modification and deletion)