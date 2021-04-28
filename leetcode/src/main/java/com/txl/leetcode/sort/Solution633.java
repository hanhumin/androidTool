package com.txl.leetcode.sort;

//给定一个非负整数 c ，你要判断是否存在两个整数 a 和 b，使得 a2 + b2 = c 。
//
//
//
// 示例 1：
//
// 输入：c = 5
//输出：true
//解释：1 * 1 + 2 * 2 = 5
//
//
// 示例 2：
//
// 输入：c = 3
//输出：false
//
//
// 示例 3：
//
// 输入：c = 4
//输出：true
//
//
// 示例 4：
//
// 输入：c = 2
//输出：true
//
//
// 示例 5：
//
// 输入：c = 1
//输出：true
//
//
//
// 提示：
//
//
// 0 <= c <= 231 - 1
//
// Related Topics 数学
// 👍 196 👎 0


import java.util.Arrays;
import java.util.Comparator;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution633 {

    //解题思路，这个正好是一个直角三角形，所以满足下面的条件
    //1.假设C开根号后的数是d
    //2.a+b>c
    //3.c-a<b
    public boolean judgeSquareSum(int c) {
        if(c == 0 || c == 1){
            return true;
        }
        double d =  Math.sqrt(c);//
        if(d * d == c){//c本身可以开根号，0 + d*d = c
            return true;
        }
        int a = 0, b = 0;

        return false;
    }
}
//leetcode submit region end(Prohibit modification and deletion)
