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
    public static void main(String[] args){
        System.out.println(""+new Solution10().isMatch("aa","a"));
        System.out.println(""+new Solution10().isMatch("aa","a*"));
        System.out.println(""+new Solution10().isMatch("ab",".*"));
    }

    public boolean isMatch(String s, String p) {
        return solution1(s, p);
    }

    /**
     * 循环求解
     * 这样不行  aaa 匹配不上 a*a  需要从后向前
     * */
    public boolean solution1(String s, String p){
        int sStart = s.length()-1;
        int pStart = p.length()-1;
        while (true){
            if(sStart < 0  && pStart >=0){
                return false;
            }
            if(sStart  >= 0 && pStart < 0){
                return false;
            }
            if(sStart < 0  && pStart < 0){
                return true;
            }
            Character pc = p.charAt(pStart);
            Character sc = s.charAt(sStart);
            Character prePc = null;
            if(pStart-1 >= 0){
                prePc = p.charAt(pStart-1);
            }
            if(pc.equals('*')){
                if(prePc != null && prePc.equals('.'));
            }
            if(prePc != null && prePc.equals('*')){//*可以匹配无限个
                if(pc.equals('.')){
                    //匹配任意多个任意字符
                    sStart = s.length();
                    pStart+=2;
                    continue;
                }else {
                    while (pc.equals(sc)){
                        sStart++;
                        if(sStart >= s.length()){
                            break;
                        }
                        sc = s.charAt(sStart);
                    }
                    pStart+=2;//需要跳过当前和下一个* 因此+2
                    continue;
                }

            }
            if(!pc.equals(sc) && !pc.equals('.')){
                return false;

            }
            sStart++;
            pStart++;
        }
    }
}
//leetcode submit region end(Prohibit modification and deletion)

