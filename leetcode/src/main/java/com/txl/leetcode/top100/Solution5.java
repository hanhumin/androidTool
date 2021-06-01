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


import java.util.HashMap;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution5 {
    public static void main(String[] args){
        System.out.println(new Solution5().solution3("babad"));
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

    public String solution2(String s){
        int len = s.length();
        if (len < 2) {
            return s;
        }

        int maxLen = 1;
        int begin = 0;
        // dp[i][j] 表示 s[i..j] 是否是回文串
        boolean[][] dp = new boolean[len][len];
        // 初始化：所有长度为 1 的子串都是回文串
        for (int i = 0; i < len; i++) {
            dp[i][i] = true;
        }

        char[] charArray = s.toCharArray();
        // 递推开始
        // 先枚举子串长度
        for (int L = 2; L <= len; L++) {
            // 枚举左边界，左边界的上限设置可以宽松一些
            for (int i = 0; i < len; i++) {
                // 由 L 和 i 可以确定右边界，即 j - i + 1 = L 得
                int j = L + i - 1;
                // 如果右边界越界，就可以退出当前循环
                if (j >= len) {
                    break;
                }

                if (charArray[i] != charArray[j]) {
                    dp[i][j] = false;
                } else {
                    if (j - i < 3) {
                        dp[i][j] = true;
                    } else {
                        dp[i][j] = dp[i + 1][j - 1];
                    }
                }

                // 只要 dp[i][L] == true 成立，就表示子串 s[i..L] 是回文，此时记录回文长度和起始位置
                if (dp[i][j] && j - i + 1 > maxLen) {
                    maxLen = j - i + 1;
                    begin = i;
                }
            }
        }
        return s.substring(begin, begin + maxLen);
    }

    public String solution3(String s){
        if(s == null || s.length() == 1){
            return s;
        }
        int maxLen = 1;
        HashMap<PositionKey,Boolean> positionInfo1 = new HashMap<>();
        HashMap<PositionKey,Boolean> positionInfo2 = new HashMap<>();
        HashMap<PositionKey,Boolean> temp = null;
        int begin = 0;
        int len = s.length();
        for (int L = 2; L <= len; L++) {
            for (int i=0;i<=len-L;i++){
                int j = L + i -1;
                boolean value = s.charAt(i) == s.charAt(j);
                if(L == 2){
                    positionInfo1.put(new PositionKey(i,j), value);
                }else if(L == 3){
                    positionInfo2.put(new PositionKey(i,j), value);
                }else {
                    if(temp == null){
                        temp = new HashMap<>();
                    }
                    temp.put(new PositionKey(i,j), value && positionInfo1.get(new PositionKey(i+1,j-1)));
                }
                if(value && L > maxLen){//是回文
                    if(L == 2 || L == 3){
                        begin = i;
                        maxLen = L;
                    }else if(positionInfo1.get(new PositionKey(i+1,j-1))){
                        begin = i;
                        maxLen = L;
                    }
                }

            }
            if(temp != null){
                positionInfo1 = positionInfo2;
                positionInfo2 = temp;
                temp = null;
            }

        }
        return s.substring(begin,begin+maxLen);
    }

    private static class PositionKey{
        private final int i;
        private final int j;

        public PositionKey(int i, int j) {
            this.i = i;
            this.j = j;
        }

        @Override
        public boolean equals(Object obj) {
            if(obj instanceof  PositionKey){
                return ((PositionKey) obj).i == this.i && ((PositionKey) obj).j == this.j;
            }
            return false;
        }


        @Override
        public int hashCode() {//每一轮循环中的i值都是不相同的，可以直接用i 做hash
            return Integer.hashCode(i);
        }
    }


}
//leetcode submit region end(Prohibit modification and deletion)
