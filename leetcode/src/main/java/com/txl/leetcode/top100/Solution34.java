package com.txl.leetcode.top100;

//给定一个按照升序排列的整数数组 nums，和一个目标值 target。找出给定目标值在数组中的开始位置和结束位置。
//
// 如果数组中不存在目标值 target，返回 [-1, -1]。
//
// 进阶：
//
//
// 你可以设计并实现时间复杂度为 O(log n) 的算法解决此问题吗？
//
//
//
//
// 示例 1：
//
//
//输入：nums = [5,7,7,8,8,10], target = 8
//输出：[3,4]
//
// 示例 2：
//
//
//输入：nums = [5,7,7,8,8,10], target = 6
//输出：[-1,-1]
//
// 示例 3：
//
//
//输入：nums = [], target = 0
//输出：[-1,-1]
//
//
//
// 提示：
//
//
// 0 <= nums.length <= 105
// -109 <= nums[i] <= 109
// nums 是一个非递减数组
// -109 <= target <= 109
//
// Related Topics 数组 二分查找
// 👍 1069 👎 0


//leetcode submit region begin(Prohibit modification and deletion)
class Solution34 {
    public static void main(String[] args) {
        new Solution34().searchRange(new int[]{5, 7, 7, 8, 8, 10}, 8);
        new Solution34().searchRange(new int[]{5, 7, 7, 8, 8, 10}, 6);
        new Solution34().searchRange(new int[]{}, 6);
        new Solution34().searchRange(new int[]{1}, 1);
        new Solution34().searchRange(new int[]{0, 1}, 1);
    }

    public int[] searchRange(int[] nums, int target) {
        int left = halfSearch(nums, target, true);

        int right = halfSearch(nums, target, false);

        if(left<nums.length && left <= right && nums[left] == target && nums[right] == target){
            return new int[]{left,right};
        }

        return new int[]{-1, -1};
    }

    /**
     * @param left true 查找第一个比target小的数； false 查找第一个比target大的数
     */
    int halfSearch(int[] nums, int target, boolean left) {
        int start = 0;
        int end = nums.length - 1;
        int ans = 0;
        while (start <= end) {
            int mid = (start + end) / 2;
            if(left){
                if (nums[mid] >= target) { //nums[mid] < target  查找第一个大于等于target的数
                    end = mid - 1;
                    ans = mid;
                } else {
                    start = mid + 1;
                }
            }else {
                if (nums[mid] <= target) { //nums[mid] < target  查找第一个小于等于target的数
                    start = mid + 1;
                    ans = mid;
                } else {
                    end = mid - 1;
                }
            }


        }
        return ans;
    }
}
//leetcode submit region end(Prohibit modification and deletion)
