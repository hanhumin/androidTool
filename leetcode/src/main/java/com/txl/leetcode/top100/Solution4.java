package com.txl.leetcode.top100;
//给定两个大小分别为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。请你找出并返回这两个正序数组的 中位数 。
//
//
//
// 示例 1：
//
//
//输入：nums1 = [1,3], nums2 = [2]
//输出：2.00000
//解释：合并数组 = [1,2,3] ，中位数 2
//
//
// 示例 2：
//
//
//输入：nums1 = [1,2], nums2 = [3,4]
//输出：2.50000
//解释：合并数组 = [1,2,3,4] ，中位数 (2 + 3) / 2 = 2.5
//
//
// 示例 3：
//
//
//输入：nums1 = [0,0], nums2 = [0,0]
//输出：0.00000
//
//
// 示例 4：
//
//
//输入：nums1 = [], nums2 = [1]
//输出：1.00000
//
//
// 示例 5：
//
//
//输入：nums1 = [2], nums2 = []
//输出：2.00000
//
//
//
//
// 提示：
//
//
// nums1.length == m
// nums2.length == n
// 0 <= m <= 1000
// 0 <= n <= 1000
// 1 <= m + n <= 2000
// -106 <= nums1[i], nums2[i] <= 106
//
//
//
//
// 进阶：你能设计一个时间复杂度为 O(log (m+n)) 的算法解决此问题吗？
// Related Topics 数组 二分查找 分治算法
// 👍 4121 👎 0


import com.txl.leetcode.Logarithm;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution4 {
    public static void main(String[] args) {
        for (int i = 0; i < 5000; i++) {
            int[] nums1 = Logarithm.generate4(10, 100);
            int[] nums2 = Logarithm.generate4(10, 100);
//            int nums1[] = new int[]{13, 94, 179, 245, 285, 348};
//            int nums2[] = new int[]{27, 52};
            double d = new Solution4().findMedianSortedArrays(nums1, nums2);
//            double d2 = new Solution4().solution2(nums1, nums2);
            double d2 = new Solution4().solution3(nums1, nums2);
            if (d != d2) {
                System.out.println("++++++++++++++++++++++++++++num1+++++++++++++++++++++++++++++++++++++++++++");
                for (int num : nums1) {
                    System.out.print(num + ",  ");
                }
                System.out.println("");
                System.out.println("++++++++++++++++++++++++++++num2+++++++++++++++++++++++++++++++++++++++++++");
                for (int num : nums2) {
                    System.out.print(num + ",  ");
                }
                System.out.println("");
                System.out.println("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
                System.out.println("d  " + d + "  d2 " + d2);
                throw new RuntimeException("not equals");
            }
        }

    }

    public double findMedianSortedArrays(int[] nums1, int[] nums2) {
        return solution1(nums1, nums2);
    }


    /**
     * 注意到题目两个数组都是有序的。
     * 操作思路，每次去除一个最大数和一个最小数
     */
    public double solution1(int[] nums1, int[] nums2) {
        int leftStart = 0, leftEnd = nums1.length - 1, rightStart = 0, rightEnd = nums2.length - 1;
        int leftL = leftEnd - leftStart + 1;
        int rightL = rightEnd - rightStart + 1;
        if (rightL == 0 && leftL == rightL) {
            return 0;
        }
        while (leftL + rightL > 2) {
            if (leftStart > leftEnd) {//左侧已经没有可供选择的数
                rightStart++;
                rightEnd--;
            } else if (rightStart > rightEnd) {//右侧没有可供选择的数
                leftStart++;
                leftEnd--;
            } else {
                if (nums1[leftStart] < nums2[rightStart]) {
                    leftStart++;
                } else {
                    rightStart++;
                }
                if (nums1[leftEnd] > nums2[rightEnd]) {
                    leftEnd--;
                } else {
                    rightEnd--;
                }
            }
            leftL = leftEnd - leftStart + 1;
            rightL = rightEnd - rightStart + 1;
        }
        if (leftL == 0) {
            return (nums2[rightStart] + nums2[rightEnd]) / 2.0;
        } else if (rightL == 0) {
            return (nums1[leftStart] + nums1[leftEnd]) / 2.0;
        } else {
            return (nums1[leftStart] + nums2[rightStart]) / 2.0;
        }
    }


    /**
     * 二分查找
     */
    public double solution2(int[] nums1, int[] nums2) {
        int leftL = nums1 != null ? nums1.length : 0;
        int rightL = nums2 != null ? nums2.length : 0;
        int k = (leftL + rightL) / 2 + 1;
        if ((leftL + rightL) % 2 == 1) {
            return getK(nums1, nums2, k);
        } else {
            return (getK(nums1, nums2, k) + getK(nums1, nums2, k - 1)) / 2;
        }
    }

    /**
     * 查找第k个元素
     */
    private double getK(int[] nums1, int[] nums2, int k) {
        int leftL = nums1 != null ? nums1.length : 0;
        int rightL = nums2 != null ? nums2.length : 0;
        int leftStart = 0;
        int rightStart = 0;
        while (true) {
            if (leftStart == leftL) {
                if (nums2 == null || nums2.length == 0) {
                    return 0;
                }
                return nums2[rightStart + k - 1];
            }
            if (rightStart == rightL) {
                return nums1[leftStart + k - 1];
            }
            if (k == 1) {
                return Math.min(nums1[leftStart], nums2[rightStart]);
            }
            int half = k / 2;
            int newIndex1 = Math.min(leftStart + half, leftL) - 1;
            int newIndex2 = Math.min(rightStart + half, rightL) - 1;
            int pivot1 = nums1[newIndex1], pivot2 = nums2[newIndex2];
            if (pivot1 <= pivot2) {
                k -= (newIndex1 - leftStart + 1);
                leftStart = newIndex1 + 1;
            } else {
                k -= (newIndex2 - rightStart + 1);
                rightStart = newIndex2 + 1;
            }
        }
    }


    public double solution3(int[] nums1, int[] nums2) {

        if (nums1.length > nums2.length) {
            return solution3(nums2, nums1);
        }
        int m = nums1.length;
        int n = nums2.length;
        if (m == 0 && m == n) {
            return 0;
        }
        int start = 0, end = m;
        int median1 = 0, median2 = 0;
        while (start <= end) {
            int i = (start + end) / 2;
            int j = (m + n + 1) / 2 - i;
            int nums1LeftMax = i == 0 ? Integer.MIN_VALUE : nums1[i - 1];
            int nums1RightMin = i == m ? Integer.MAX_VALUE : nums1[i];
            int nums2LeftMax = j == 0 ? Integer.MIN_VALUE : nums2[j - 1];
            int nums2RightMin = j == n ? Integer.MAX_VALUE : nums2[j];
            if (nums1LeftMax <= nums2RightMin) {
                median1 = Math.max(nums1LeftMax, nums2LeftMax);
                median2 = Math.min(nums1RightMin, nums2RightMin);
                start = i + 1;
            } else {
                end = i - 1;
            }
        }
        if ((m + n) % 2 == 0) {
            return (median1 + median2) / 2.0;
        } else {
            return median1;
        }
    }
}
//leetcode submit region end(Prohibit modification and deletion)
