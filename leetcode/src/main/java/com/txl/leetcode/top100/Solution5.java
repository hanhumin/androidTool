package com.txl.leetcode.top100;

//给你一个字符串 s，找到 s 中最长的回文子串。
//
//
//
// 示例 1：
//
//
//输入：s = "babad"
//输出："bab"
//解释："aba" 同样是符合题意的答案。
//
//
// 示例 2：
//
//
//输入：s = "cbbd"
//输出："bb"
//
//
// 示例 3：
//
//
//输入：s = "a"
//输出："a"
//
//
// 示例 4：
//
//
//输入：s = "ac"
//输出："a"
//
//
//
//
// 提示：
//
//
// 1 <= s.length <= 1000
// s 仅由数字和英文字母（大写和/或小写）组成
//
// Related Topics 字符串 动态规划
// 👍 3683 👎 0


//leetcode submit region begin(Prohibit modification and deletion)
class Solution5 {
    public static void main(String[] args){
        System.out.println(new Solution5().longestPalindrome("babad"));
    }

    public String longestPalindrome(String s) {
        return solution1(s);
    }

    /**
     * 判断是否是回文
     * */
    private boolean isPalindrome(String s){
        int len = s.length();
        for (int i = 0; i < len / 2; i++) {
            if (s.charAt(i) != s.charAt(len - i - 1)) {
                return false;
            }
        }
        return true;
    }

    /**
     * 暴力求解
     * */
    private String solution1(String s){
        if(s == null || s.length() == 0){
            return s;
        }
        for (int l = s.length(); l>1;l--){
            int start = 0; int end = l;
            while (end<=s.length()){
                String s1 = s.substring(start,end);
                if(isPalindrome(s1)){
                    return s1;
                }
                start++;
                end++;
            }
        }
        return s.charAt(0)+"";
    }
}
//leetcode submit region end(Prohibit modification and deletion)
