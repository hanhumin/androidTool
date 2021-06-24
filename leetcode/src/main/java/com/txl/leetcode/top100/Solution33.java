package com.txl.leetcode.top100;

//整数数组 nums 按升序排列，数组中的值 互不相同 。
//
// 在传递给函数之前，nums 在预先未知的某个下标 k（0 <= k < nums.length）上进行了 旋转，使数组变为 [nums[k], nums[
//k+1], ..., nums[n-1], nums[0], nums[1], ..., nums[k-1]]（下标 从 0 开始 计数）。例如， [0,1,2
//,4,5,6,7] 在下标 3 处经旋转后可能变为 [4,5,6,7,0,1,2] 。
//
// 给你 旋转后 的数组 nums 和一个整数 target ，如果 nums 中存在这个目标值 target ，则返回它的下标，否则返回 -1 。
//
//
//
// 示例 1：
//
//
//输入：nums = [4,5,6,7,0,1,2], target = 0
//输出：4
//
//
// 示例 2：
//
//
//输入：nums = [4,5,6,7,0,1,2], target = 3
//输出：-1
//
// 示例 3：
//
//
//输入：nums = [1], target = 0
//输出：-1
//
//
//
//
// 提示：
//
//
// 1 <= nums.length <= 5000
// -10^4 <= nums[i] <= 10^4
// nums 中的每个值都 独一无二
// 题目数据保证 nums 在预先未知的某个下标上进行了旋转
// -10^4 <= target <= 10^4
//
//
//
//
// 进阶：你可以设计一个时间复杂度为 O(log n) 的解决方案吗？
// Related Topics 数组 二分查找
// 👍 1413 👎 0


//leetcode submit region begin(Prohibit modification and deletion)
class Solution33 {
    public static void main(String[] args){
        System.out.println("result : "+ new Solution33().search(new int[]{4,5,6,7,0,1,2},0));
        System.out.println("result : "+ new Solution33().search(new int[]{1},0));
        System.out.println("result : "+ new Solution33().search(new int[]{3,1},3));
    }

    public int search(int[] nums, int target) {
        if(nums == null){
            return -1;
        }
        if(nums[nums.length-1] >= nums[0]){//未被旋转
            return halfSearch(nums,0,nums.length-1,target);
        }
        int max = findMaxPosition(nums);
        if(target>=nums[0]){
            return halfSearch(nums,0,max,target);
        }else {
            return halfSearch(nums,max+1,nums.length-1,target);
        }
    }


    int findMaxPosition(int[] nums){
        int start = 0;
        int end = nums.length-1;
        while (start<=end){
            int mid = (start + end)/2;
            if(nums[mid] > nums[mid+1]){
                return mid;
            }else {
                if(nums[mid] > nums[0]){//mid落在左侧区域
                    start = mid+1;
                }else {
                    end = mid;
                }
            }
        }
        return -1;
    }

    int halfSearch(int[] nums, int start, int end,int target){
        while (start<=end){
            if(start == end){
                if(target == nums[start]){
                    return start;
                }
                break;
            }
            int mid = (start+end)/2;
            if(nums[mid] == target){
                return mid;
            }else if(nums[mid] < target){
                start = mid+1;
            }else {
                end = mid;
            }
        }
        return -1;
    }
}
//leetcode submit region end(Prohibit modification and deletion)
