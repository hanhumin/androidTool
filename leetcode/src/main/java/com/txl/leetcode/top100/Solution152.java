package com.txl.leetcode.top100;

//给你一个整数数组 nums ，请你找出数组中乘积最大的连续子数组（该子数组中至少包含一个数字），并返回该子数组所对应的乘积。
//
//
//
// 示例 1:
//
// 输入: [2,3,-2,4]
//输出: 6
//解释: 子数组 [2,3] 有最大乘积 6。
//
//
// 示例 2:
//
// 输入: [-2,0,-1]
//输出: 0
//解释: 结果不能为 2, 因为 [-2,-1] 不是子数组。
// Related Topics 数组 动态规划
// 👍 1196 👎 0


//leetcode submit region begin(Prohibit modification and deletion)
class Solution152 {
    /**
     * 思路1： 直接采用暴力循环求解
     * <p>
     * 思路2:  动态规划
     * 当前位置如果是一个负数的话，那么我们希望以它前一个位置结尾的某个段的积也是个负数，这样就可以负负得正，并且我们希望这个积尽可能「负得更多」，即尽可能小。如果当前位置是一个正数的话，我们更希望以它前一个位置结尾的某个段的积也是个正数，并且希望它尽可能地大。于是这里我们可以再维护一个 f_{\min}(i)f
     * min
     * ​
     * (i)，它表示以第 ii 个元素结尾的乘积最小子数组的乘积
     */
    public int maxProduct1(int[] nums) {
        if (nums == null || nums.length == 0) {
            return 0;
        }
        int length = nums.length;
        int[] maxF = new int[length];
        int[] minF = new int[length];
        System.arraycopy(nums, 0, maxF, 0, length);
        System.arraycopy(nums, 0, minF, 0, length);
        for (int i = 1; i < length; i++) {
            maxF[i] = Math.max(nums[i], Math.max(nums[i] * maxF[i - 1], nums[i] * minF[i - 1]));
            minF[i] = Math.min(nums[i], Math.min(nums[i] * maxF[i - 1], nums[i] * minF[i - 1]));
        }

        int ans = maxF[0];
        for (int i = 1; i < length; ++i) {
            ans = Math.max(ans, maxF[i]);
        }
        return ans;
    }

    public int maxProduct(int[] nums) {
        if (nums == null || nums.length == 0) {
            return 0;
        }
        int length = nums.length;
        int lastMax = nums[0];
        int lastMin = nums[0];
        int ans =  nums[0];
        for (int i = 1; i < length; i++) {
            int mx = lastMax, mn = lastMin;
            lastMax = Math.max(nums[i], Math.max(nums[i] * mx, nums[i] * mn));
            lastMin = Math.min(nums[i], Math.min(nums[i] * mx, nums[i] * mn));
            ans = Math.max(ans, lastMax);
        }
        return ans;
    }
}
//leetcode submit region end(Prohibit modification and deletion)
