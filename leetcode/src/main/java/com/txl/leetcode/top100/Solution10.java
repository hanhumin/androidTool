package com.txl.leetcode.top100;

//给你一个字符串 s 和一个字符规律 p，请你来实现一个支持 '.' 和 '*' 的正则表达式匹配。
//
//
// '.' 匹配任意单个字符
// '*' 匹配零个或多个前面的那一个元素
//
//
// 所谓匹配，是要涵盖 整个 字符串 s的，而不是部分字符串。
//
//
// 示例 1：
//
//
//输入：s = "aa" p = "a"
//输出：false
//解释："a" 无法匹配 "aa" 整个字符串。
//
//
// 示例 2:
//
//
//输入：s = "aa" p = "a*"
//输出：true
//解释：因为 '*' 代表可以匹配零个或多个前面的那一个元素, 在这里前面的元素就是 'a'。因此，字符串 "aa" 可被视为 'a' 重复了一次。
//
//
// 示例 3：
//
//
//输入：s = "ab" p = ".*"
//输出：true
//解释：".*" 表示可匹配零个或多个（'*'）任意字符（'.'）。
//
//
// 示例 4：
//
//
//输入：s = "aab" p = "c*a*b"
//输出：true
//解释：因为 '*' 表示零个或多个，这里 'c' 为 0 个, 'a' 被重复一次。因此可以匹配字符串 "aab"。
//
//
// 示例 5：
//
//
//输入：s = "mississippi" p = "mis*is*p*."
//输出：false
//
//
//
// 提示：
//
//
// 0 <= s.length <= 20
// 0 <= p.length <= 30
// s 可能为空，且只包含从 a-z 的小写字母。
// p 可能为空，且只包含从 a-z 的小写字母，以及字符 . 和 *。
// 保证每次出现字符 * 时，前面都匹配到有效的字符
//
// Related Topics 字符串 动态规划 回溯算法
// 👍 2148 👎 0


//leetcode submit region begin(Prohibit modification and deletion)
class Solution10 {
    //fixme 后期重点回顾  题目比容易理解与找到状态转移方程
    public static void main(String[] args) {
        System.out.println( "test ===== " + "ab0".substring( 0, 0 ) );
//        System.out.println(""+new Solution10().isMatch("aa","a"));
//        System.out.println(""+new Solution10().isMatch("aa","a*"));
//        if (!new Solution10().isMatch( "aa", "a*" )) {
//            throw new RuntimeException( "aa not match a*" );
//        }
//        System.out.println(""+new Solution10().isMatch("ab",".*"));
//        System.out.println(""+new Solution10().isMatch("ab","a.*"));
//        if (!new Solution10().isMatch( "ab", "a.*" )) {
//            throw new RuntimeException( "ab not match a.*" );
//        }
//        if (!new Solution10().isMatch( "aaa", "ab*a*c*a" )) {
//            throw new RuntimeException( "aaa not match ab*a*c*a" );
//        }
//        if (!new Solution10().isMatch( "aab", "c*a*b" )) {
//            throw new RuntimeException( "aab not match c*a*b" );
//        }
//        if (!new Solution10().isMatch( "mississippi", "mis*is*ip*." )) {
//            throw new RuntimeException( "mississippi not match mis*is*ip*." );
//        }
//        if (new Solution10().isMatch( "a", ".*..a*" )) {
//            throw new RuntimeException( "a  match .*..a*" );
//        }
//        if (!new Solution10().isMatch( "ab", ".*.." )) {
//            throw new RuntimeException( "ab  not match .*.." );
//        }
//        if (!new Solution10().isMatch( "aaabaaaababcbccbaab", "c*c*.*c*a*..*c*" )) {//c*c*.*c*a* . .* c*
//            throw new RuntimeException( "aaabaaaababcbccbaab  not match c*c*.*c*a*..*c*" );
//        }
        if (!new Solution10().isMatch( "cbbbaccbcacbc", "b*.*" )) {
            throw new RuntimeException( "cbbbaccbcacbc  not match b*.*" );
        }
        if (!new Solution10().isMatch( "cbbbaccbcacbcca", "b*.*b*a*.a*b*.a*" )) {
            throw new RuntimeException( "cbbbaccbcacbcca  not match b*.*b*a*.a*b*.a*" );
        }
//        System.out.println(""+new Solution10().isMatch("aaa","ab*a*c*a"));
    }

    public boolean isMatch(String s, String p) {
        return solution1( s, p );
    }

    /**
     * 循环求解
     * 这样不行  aaa 匹配不上 a*a  需要从后向前
     */
    public boolean solution1(String s, String p) {
        int sStart = s.length() - 1;
        int pStart = p.length() - 1;
        while (true) {
//            if(sStart < 0  && pStart >=0){//"" 可以和 "a*b*" 匹配
//                return false;
//            }
            if (sStart >= 0 && pStart < 0) {
                return false;
            }
            if (sStart < 0 && pStart < 0) {
                return true;
            }
            System.out.println( "输入：  " + s.substring(0,sStart+1) + "         " + p.substring(0,pStart+1) + "        " + sStart + "    " + pStart );
            Character pc = p.charAt( pStart );
            Character sc = sStart < 0 ? null : s.charAt( sStart );
            Character prePc = null;
            if (pStart - 1 >= 0) {
                prePc = p.charAt( pStart - 1 );
            }
            if (pc.equals( '*' )) {//遇到*就递归
                if (prePc != null && prePc.equals( '.' )) {
                    if (pStart == 1) {//当匹配字符串的长度
                        return true;
                    }
                    String np = p.substring( 0, Math.max( pStart - 1, 0 ) );
                    if(sc == null){
                        return solution1( "",np );
                    }else {
                        boolean result = false;
                        //编写测试用例的时候需要，尽可能的让测试用例走到每一个分支，并且测试对应的边界条件。
                        while (sStart >= -1) {//为什么是-1  因为当sStart 是0 的时候，表示匹配第一个字符没有被 .*匹配
                            result = solution1( s.substring( 0, sStart + 1 ), np );
                            sStart--;
                            if (result) {
                                break;
                            }
                        }
                        return result;
                    }
                } else if (prePc != null && !prePc.equals( sc )) {//前一个与当前值不等，直接跳过当前
                    pStart -= 2;
                    continue;
                } else {//当前那个数等于*之前的那个数
                    String np = p.substring( 0, Math.max( pStart - 1, 0 ) );
                    boolean result = false;
                    while (true) {
                        result = solution1( s.substring( 0, sStart + 1 ), np );//第一次一个都不匹配
                        if (result) {
                            break;
                        }
                        sStart--;
                        if (sStart < 0) {
                            result = solution1( "", np );//sStart < 0 时说明主串循环完毕
                            break;
                        }
                        sc = s.charAt( sStart );
                        if(!sc.equals( prePc )){
                            result = solution1( s.substring( 0, sStart + 1 ), np );//把相同的全部匹配完成
                            break;
                        }
                    }
                    return result;
                }
            }
            if (!pc.equals( sc ) && !pc.equals( '.' )) {
                return false;
            }
            if(sc == null){
                return false;
            }
            sStart--;
            pStart--;
        }
    }
}
//leetcode submit region end(Prohibit modification and deletion)

