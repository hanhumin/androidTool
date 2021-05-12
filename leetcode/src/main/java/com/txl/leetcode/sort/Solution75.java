package com.txl.leetcode.sort;

//给定一个包含红色、白色和蓝色，一共 n 个元素的数组，原地对它们进行排序，使得相同颜色的元素相邻，并按照红色、白色、蓝色顺序排列。
//
// 此题中，我们使用整数 0、 1 和 2 分别表示红色、白色和蓝色。
//
//
//
//
//
//
// 示例 1：
//
//
//输入：nums = [2,0,2,1,1,0]
//输出：[0,0,1,1,2,2]
//
//
// 示例 2：
//
//
//输入：nums = [2,0,1]
//输出：[0,1,2]
//
//
// 示例 3：
//
//
//输入：nums = [0]
//输出：[0]
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
// n == nums.length
// 1 <= n <= 300
// nums[i] 为 0、1 或 2
//
//
//
//
// 进阶：
//
//
// 你可以不使用代码库中的排序函数来解决这道题吗？
// 你能想出一个仅使用常数空间的一趟扫描算法吗？
//
// Related Topics 排序 数组 双指针
// 👍 879 👎 0


//leetcode submit region begin(Prohibit modification and deletion)
class Solution75 {

    private static int[] valueGenerate(int size) {
        int re = (int) (Math.random() * size);
        int[] nums = new int[re];
        for (int i = 0; i < re; i++) {
            nums[i] = (int) (Math.random() * 3);
        }
        return nums;
    }

    public static void main(String[] args) {
//        new Solution75().quickSort(new int[]{0,2,2,2,1});
//        if(true){
//            return;
//        }

        for (int i = 50000; i > 0; i--) {
            int[] nums = valueGenerate( 10 );
            int[] num2 = nums.clone();
            System.out.println( "======================origin============================" );
            System.out.print( "{" );
            for (int n : nums) {
                System.out.print( n + "," );
            }
            System.out.print( "}" );
            System.out.println( "" );
            if (nums.length != num2.length) {
                throw new RuntimeException( "result length not equals" );
            }
            new Solution75().solution1( nums );
            new Solution75().quickSort( num2 );
            for (int j = 0; j < nums.length; j++) {
                if (num2[j] != nums[j]) {
                    System.out.println( "======================nums============================" );
                    System.out.print( "{" );
                    for (int n : nums) {
                        System.out.print( n + "," );
                    }
                    System.out.print( "}" );
                    System.out.println( "" );
                    System.out.println( "======================num2============================" );
                    System.out.print( "{" );
                    for (int n : num2) {
                        System.out.print( n + "," );
                    }
                    System.out.print( "}" );
                    System.out.println( "" );
                    throw new RuntimeException( "result not equals" );
                }
            }
        }
    }


    public void sortColors(int[] nums) {
        if (nums == null) {
            throw new RuntimeException( "nums is null" );
        }
        solution1( nums );
    }

    //思路一：使用桶排序
    private void solution1(int[] nums) {
        int count0 = 0, count1 = 0, count2 = 0;
        for (int num : nums) {
            switch (num) {
                case 0:
                    count0++;
                    break;
                case 1:
                    count1++;
                    break;
                default:
                    count2++;
            }
        }
        for (int i = 0; i < nums.length; i++) {
            if (count0 > 0) {
                nums[i] = 0;
                count0--;
                continue;
            }
            if (count1 > 0) {
                nums[i] = 1;
                count1--;
                continue;
            }
            if (count2 > 0) {
                nums[i] = 2;
                count2--;
            }
        }
    }


    //思路二：快速排序
    private void quickSort(int[] nums) {
        int start = -1;
        int more = nums.length;
        int compare = 1;
        for (int i = 0; i < more; i++) {
            if (start == more) {
                break;
            }
            if (nums[i] < compare) {
                start++;
                if (i != start) {
                    swap( nums, start, i );
                }

            } else if (nums[i] > compare) {
                more--;
                if (i != more) {
                    swap( nums, more, i );
                    i--;
                }
            }
        }
    }

    private void swap(int[] nums, int position1, int position2) {
        nums[position1] = nums[position1] ^ nums[position2];
        nums[position2] = nums[position1] ^ nums[position2];
        nums[position1] = nums[position1] ^ nums[position2];
//        try {
//            int temp = nums[position1];
//            nums[position1] = nums[position2];
//            nums[position2] = temp;
//        } catch (Exception e) {
//            System.out.println( "position1 = " + position1 + "  position2 = " + position2 );
//            throw e;
//        }
    }
}
//leetcode submit region end(Prohibit modification and deletion)
