package com.txl.leetcode.top100;
//给你一个只包含 '(' 和 ')' 的字符串，找出最长有效（格式正确且连续）括号子串的长度。
//
//
//
//
//
// 示例 1：
//
//
//输入：s = "(()"
//输出：2
//解释：最长有效括号子串是 "()"
//
//
// 示例 2：
//
//
//输入：s = ")()())"
//输出：4
//解释：最长有效括号子串是 "()()"
//
//
// 示例 3：
//
//
//输入：s = ""
//输出：0
//
//
//
//
// 提示：
//
//
// 0 <= s.length <= 3 * 104
// s[i] 为 '(' 或 ')'
//
//
//
// Related Topics 字符串 动态规划
// 👍 1343 👎 0


import java.util.Deque;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution32 {
    public int longestValidParentheses(String s) {
        return solution1(s);
    }

    /**
     * 暴力求解
     * 超出时间限制  时间复杂度 n^3
     */
    private int solution1(String s) {
        if (s == null || s.length() < 2) {
            return 0;
        }
        int l = s.length();
        while (l>=2){
            if(l%2 != 0){
                l--;
                continue;
            }
            for (int i=0;i<s.length();i++){
                if(i+l > s.length()){
                    break;
                }
                String sub = s.substring(i,l+i);
                if(isValid(sub)){
                    return l;
                }
            }
            l--;
        }
        return 0;
    }

    public boolean isValid(String s) {
        if (s.length() % 2 == 1) {//只能成对出现
            return false;
        }
        LinkedList<Character> stack = new LinkedList<>();
        Map<Character, Character> hashMap = new HashMap<Character, Character>();
        hashMap.put(')', '(');
        for (int i = 0; i < s.length(); i++) {
            Character character = s.charAt(i);
            if (hashMap.containsKey(character)) {//包含则说明是右边的需要出栈
                Character m = hashMap.get(character);
                if (stack.isEmpty() || !m.equals(stack.pollLast())) {
                    return false;
                }
            } else {
                stack.add(character);
            }
        }
        //栈中还有余数，说明没配对完
        return stack.size() <= 0;
    }


    /**
     * 动态规划
     * */
    private int solution2(String s){
        if (s == null || s.length() < 2) {
            return 0;
        }
        int l = s.length();
        int max = 0;
        int[] dp = new int[s.length()];
        for (int i=1;i<l;i++){
            if (s.charAt(i) == ')') {
                if (i - dp[i - 1] > 0 && s.charAt(i - dp[i - 1] - 1) == '(') {// i - dp[i - 1] > 0 是为了保证  i - dp[i - 1] - 1 是正常存在的
                    dp[i] = dp[i - 1] + ((i - dp[i - 1]) >= 2 ? dp[i - dp[i - 1] - 2] : 0) + 2;
                }
                max = Math.max(max, dp[i]);
            }
        }
        return max;
    }

    private int solution3(String s){
        int max = 0;
        Deque<Integer> stack = new LinkedList<Integer>();
        stack.push(-1);
        for (int i = 0; i < s.length(); i++) {
            if (s.charAt(i) == '(') {
                stack.push(i);
            } else {
                stack.pop();
                if (stack.isEmpty()) {
                    stack.push(i);
                } else {
                    max = Math.max(max, i - stack.peek());
                }
            }
        }
        return max;
    }

//    private int solution4(String s){
//        int max = 0;
//        Deque<Integer> stack = new LinkedList<Integer>();
//        for (int i = 0; i < s.length(); i++) {
//            if (s.charAt(i) == '(') {
//                stack.push(i);
//            } else {
//                if(!stack.isEmpty()){
//                    Integer integer = stack.pop();
//                    max = Math.max(max, i - integer+1);
//                }
//            }
//        }
//        return max;
//    }
}
//leetcode submit region end(Prohibit modification and deletion)