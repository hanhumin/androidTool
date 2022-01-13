package com.txl.leetcode.top100;
//给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串 s ，判断字符串是否有效。
//
// 有效字符串需满足：
//
//
// 左括号必须用相同类型的右括号闭合。
// 左括号必须以正确的顺序闭合。
//
//
//
//
// 示例 1：
//
//
//输入：s = "()"
//输出：true
//
//
// 示例 2：
//
//
//输入：s = "()[]{}"
//输出：true
//
//
// 示例 3：
//
//
//输入：s = "(]"
//输出：false
//
//
// 示例 4：
//
//
//输入：s = "([)]"
//输出：false
//
//
// 示例 5：
//
//
//输入：s = "{[]}"
//输出：true
//
//
//
// 提示：
//
//
// 1 <= s.length <= 104
// s 仅由括号 '()[]{}' 组成
//
// Related Topics 栈 字符串
// 👍 2454 👎 0


import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution20 {
    public static void main(String[] args){
        System.out.println("isValid : "+new Solution20().isValid("([)]"));
        System.out.println("isValid : "+new Solution20().isValid("()"));
    }

    public boolean isValid(String s) {
        if(s.length() % 2 == 1){//只能成对出现
            return false;
        }
        // ( { [  欠考虑  无法处理 "([)]" 的问题
//        int kCount = 0, mCount = 0,fCount = 0;
//
//        for(int i=0; i<s.length(); i++){
//            char c = s.charAt(i);
//            switch (c){
//                case '(':{
//                    kCount++;
//                    break;
//                }
//                case '{':{
//                    mCount++;
//                    break;
//                }
//                case '[':{
//                    fCount++;
//                    break;
//                }
//
//                case ')':{
//                    kCount--;
//                    if(kCount < 0){
//                        return false;
//                    }
//                    break;
//                }
//                case '}':{
//                    mCount--;
//                    if(mCount < 0){
//                        return false;
//                    }
//                    break;
//                }
//                case ']':{
//                    fCount--;
//                    if(fCount < 0){
//                        return false;
//                    }
//                    break;
//                }
//            }
//        }
//        if(mCount > 0 || kCount > 0 || fCount > 0){
//            return false;
//        }

        LinkedList<Character> stack = new LinkedList<>();
        Map<Character, Character> hashMap = new HashMap<Character, Character>() ;
        hashMap.put(')', '(');
        hashMap.put(']', '[');
        hashMap.put('}', '{');
        for (int i=0;i<s.length();i++){
            Character character = s.charAt(i);
            if(hashMap.containsKey(character)){//包含则说明是右边的需要出栈
                Character m = hashMap.get(character);
//                Character s = stack.poll();
                if(stack.isEmpty() || !m.equals(stack.pollLast())){
                    return false;
                }
            }else {
                stack.add(character);
            }
        }
        //栈中还有余数，说明没配对完
        return stack.size() <= 0;
    }
}
//leetcode submit region end(Prohibit modification and deletion)