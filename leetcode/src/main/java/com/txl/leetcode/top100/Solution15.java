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
    public static void main(){

    }

    public List<List<Integer>> threeSum(int[] nums) {
        if(nums == null || nums.length<3){
            return new ArrayList<>();
        }
        return solution1(nums);
    }

    /**
     * 暴力求解
     * */
    List<List<Integer>> solution1(int[] nums){
        Arrays.sort(nums);
        int length = nums.length;
        int lastI = Integer.MAX_VALUE,lastJ = Integer.MAX_VALUE,lastK = Integer.MAX_VALUE;
        List<List<Integer>> arrayList = new ArrayList<>();
        for (int i=0;i<length;i++){
            if(lastI == nums[i]){
                continue;
            }
            lastI = nums[i];
            for (int j=i+1;j<length;j++){
                if(lastJ == nums[j]){
                    continue;
                }
                lastJ = nums[j];
                for (int k=j+1;k<length;k++){
                    if(lastK == nums[k]){
                        continue;
                    }
                    lastK = nums[k];
                    if(lastI + lastJ + lastK == 0){
                        ArrayList<Integer> integerArrayList = new ArrayList<>();
                        integerArrayList.add(lastI);
                        integerArrayList.add(lastJ);
                        integerArrayList.add(lastK);
                        arrayList.add(integerArrayList);
                    }
                }
            }
        }
        return arrayList;
    }

    class MathNode{
        int min,max;


        @Override
        public int hashCode() {
            return this.max+this.min;
//            return super.hashCode();
        }

        @Override
        public boolean equals(Object obj) {
            if(obj instanceof MathNode){
                if(((MathNode) obj).max == this.max && ((MathNode) obj).min == this.min){
                    return true;
                }
            }
            return super.equals(obj);
        }
    }
}
//leetcode submit region end(Prohibit modification and deletion)
