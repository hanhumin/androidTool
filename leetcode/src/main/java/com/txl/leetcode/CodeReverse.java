package com.txl.leetcode;

/**
 * 整数反转
 * 给你一个 32 位的有符号整数 x ，返回将 x 中的数字部分反转后的结果。
 *
 * 如果反转后整数超过 32 位的有符号整数的范围 [−231,  231 − 1] ，就返回 0。
 *
 * 假设环境不允许存储 64 位整数（有符号或无符号）。
 *  
 *
 * 示例 1：
 *
 * 输入：x = 123
 * 输出：321
 * 示例 2：
 *
 * 输入：x = -123
 * 输出：-321
 * 示例 3：
 *
 * 输入：x = 120
 * 输出：21
 * 示例 4：
 *
 * 输入：x = 0
 * 输出：0
 *
 * 来源：力扣（LeetCode）
 * 链接：https://leetcode-cn.com/problems/reverse-integer
 * 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
 * */
public class CodeReverse {


    public static void main(String[] args){
        CodeReverse codeReverse = new CodeReverse();
        System.out.println( "  "+codeReverse.reverse( 100 ) );
        System.out.println( "  "+codeReverse.reverse( 515 ) );
        System.out.println( "  "+codeReverse.reverse( 654 ) );
        System.out.println( "  "+codeReverse.reverse( 38999 ) );
        System.out.println( "  "+codeReverse.reverse( 1534236469 ) );
    }

    public int reverse(int x) {
        if(x == 0){
            return x;
        }
        int result = 0;
        while (x != 0){
            int temp = x % 10;

            //int 取值范围是  -2147483648  2147483647  如果当前值 result 大于 214748364 那么result * 10 必然大于int的最大值
            if(result > 214748364 || (result == 214748364 && temp > 7)){
                return 0;
            }
            //int 取值范围是  -2147483648  2147483647  如果当前值 result 小于 -214748364 那么result * 10 必然小于int最小值
            if(result<-214748364 || (result == -214748364 && temp < -8)){
                return 0;
            }
            result = result*10+temp;
            x = x/10;
        }
        return result;
    }
}