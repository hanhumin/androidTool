package com.txl.leetcode.top100;
//给你一个包含 n 个整数的数组 nums，判断 nums 中是否存在三个元素 a，b，c ，使得 a + b + c = 0 ？请你找出所有和为 0 且不重
//复的三元组。
//
// 注意：答案中不可以包含重复的三元组。
//
//
//
// 示例 1：
//
//
//输入：nums = [-1,0,1,2,-1,-4]
//输出：[[-1,-1,2],[-1,0,1]]
//
//
// 示例 2：
//
//
//输入：nums = []
//输出：[]
//
//
// 示例 3：
//
//
//输入：nums = [0]
//输出：[]
//
//
//
//
// 提示：
//
//
// 0 <= nums.length <= 3000
// -105 <= nums[i] <= 105
//
// Related Topics 数组 双指针
// 👍 3408 👎 0


import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution15 {
    public static void main(String[] args) {
//        for (List<Integer> integers : new Solution15().threeSum(new int[]{1, -1, -1, 0})) {
//        for (List<Integer> integers : new Solution15().threeSum(new int[]{-1,0,1,2,-1,-4})) {//排序后 -4， -1， -1， 0, 1, 2
//        for (List<Integer> integers : new Solution15().threeSum(new int[]{0,0,0,0})) {
//        for (List<Integer> integers : new Solution15().threeSum(new int[]{1,1,-2})) {
        for (List<Integer> integers : new Solution15().threeSum(new int[]{-1,0,1,2,-1,-4,-2,-3,3,0,4})) {//[[-4,0,4],[-4,1,3],[-3,-1,4],[-3,0,3],[-3,1,2],[-2,-1,3],[-2,0,2],[-1,-1,2],[-1,0,1]]
            System.out.println("++++++++++++++++++++++++++++++++++++++++++++");
            for (Integer i : integers) {
                System.out.print("  " + i);
            }
            System.out.println("");
            System.out.println("++++++++++++++++++++++++++++++++++++++++++++");
        }
    }

    public List<List<Integer>> threeSum(int[] nums) {
        if (nums == null || nums.length < 3) {
            return new ArrayList<>();
        }
        return solution3(nums);
    }

    /**
     * 暴力求解
     */
    List<List<Integer>> solution1(int[] nums) {
        Arrays.sort(nums);
        int length = nums.length;
        int lastI = Integer.MAX_VALUE, lastJ, lastK;
        List<List<Integer>> arrayList = new ArrayList<>();
        for (int i = 0; i < length; i++) {
            if (lastI == nums[i]) {
                continue;
            }
            //进入下一轮循环的时候
            lastJ = Integer.MAX_VALUE;
            lastI = nums[i];
            for (int j = i + 1; j < length; j++) {
                if (lastJ == nums[j]) {
                    continue;
                }
                lastJ = nums[j];
                lastK = Integer.MAX_VALUE;
                for (int k = j + 1; k < length; k++) {
                    if (lastK == nums[k]) {
                        continue;
                    }
                    lastK = nums[k];
                    if (lastI + lastJ + lastK == 0) {
                        ArrayList<Integer> integerArrayList = new ArrayList<>();
                        integerArrayList.add(lastI);
                        integerArrayList.add(lastJ);
                        integerArrayList.add(lastK);
                        arrayList.add(integerArrayList);
                    }
                }
            }
        }
        System.out.println("solution1  size " + arrayList.size());
        return arrayList;
    }

    /**
     * 官方题解
     */
    List<List<Integer>> solution2(int[] nums) {
        Arrays.sort(nums);
        List<List<Integer>> arrayList = new ArrayList<>();
        int lastI = Integer.MAX_VALUE;
        for (int i = 0; i < nums.length; i++) {
            if(nums[i] == lastI){
                continue;
            }
            lastI = nums[i];
            int start=i+1, end = nums.length - 1;
            int lastStart = Integer.MAX_VALUE;
            int lastEnd = Integer.MAX_VALUE;
            boolean changeStart = true;
            while (start<end){
                if(lastStart == nums[start] && changeStart){
                    start++;
                    continue;
                }
                if(lastEnd == nums[end] && !changeStart){
                    end--;
                    continue;
                }
                lastStart = nums[start];
                lastEnd = nums[end];
                int result = nums[i]+nums[start]+nums[end];
                if(result > 0){
                    changeStart = false;
                    end--;
                }else if(result < 0){
                    changeStart = true;
                    start++;
                }else {
                    ArrayList<Integer> integerArrayList = new ArrayList<>();
                    integerArrayList.add(nums[i]);
                    integerArrayList.add(nums[start]);
                    integerArrayList.add(nums[end]);
                    arrayList.add(integerArrayList);
                    start++;
                    changeStart = true;
                }
            }
        }
        return arrayList;
    }

    List<List<Integer>> solution3(int[] nums) {
        Arrays.sort(nums);
        int start=0, end = nums.length - 1;
        int lastStart = Integer.MAX_VALUE;
        int lastEnd = Integer.MAX_VALUE;
        List<List<Integer>> arrayList = new ArrayList<>();
        int position = -1;
        boolean changeStart = true;
        while (start<end){
            if(lastStart == nums[start] && changeStart){
                start++;
                continue;
            }
            if(lastEnd == nums[end] && !changeStart){
                end--;
                continue;
            }
            lastStart = nums[start];
            lastEnd = nums[end];
            position = halfFind(-(lastEnd + lastStart),start+1,end-1,nums);
            if(position != -1){
                ArrayList<Integer> integerArrayList = new ArrayList<>();
                integerArrayList.add(nums[start]);
                integerArrayList.add(nums[position]);
                integerArrayList.add(nums[end]);
                arrayList.add(integerArrayList);

                if(nums[position] > 0){//相加是lastEnd + lastStart是个负数 此时如果减小end,会得到一个更小的负数
                    changeStart = true;
                    start++;
                }else {
                    changeStart = false;
                    end--;
                }

            }else {
                if(lastStart+lastEnd>0){
                    end--;
                    changeStart = false;
                }else {
                    start++;
                    changeStart = true;
                }
            }
        }
        return arrayList;
    }

    int halfFind(int target, int start, int end,int[] nums){
        if(end<start){
            return -1;
        }

        while (start<=end){
            if(end-start<=1){
                if(nums[start] == target){
                    return start;
                }
                if(nums[end] == target){
                    return end;
                }
                return -1;
            }
            int mid = (start + end)/2;
            if(nums[mid] > target){
                end = mid;
            }else if(nums[mid] < target){
                start = mid;
            }else {
                return mid;
            }
        }
        return -1;
    }
}
//leetcode submit region end(Prohibit modification and deletion)
