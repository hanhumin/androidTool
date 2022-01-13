package com.txl.leetcode.code;

import java.util.Stack;

/**
 * 394. 字符串解码
 * 给定一个经过编码的字符串，返回它解码后的字符串。
 * <p>
 * 编码规则为: k[encoded_string]，表示其中方括号内部的 encoded_string 正好重复 k 次。注意 k 保证为正整数。
 * <p>
 * 你可以认为输入字符串总是有效的；输入字符串中没有额外的空格，且输入的方括号总是符合格式要求的。
 * <p>
 * 此外，你可以认为原始数据不包含数字，所有的数字只表示重复的次数 k ，例如不会出现像 3a 或 2[4] 的输入。
 * <p>
 * <p>
 * <p>
 * 示例 1：
 * <p>
 * 输入：s = "3[a]2[bc]"
 * 输出："aaabcbc"
 * 示例 2：
 * <p>
 * 输入：s = "3[a2[c]]"
 * 输出："accaccacc"
 * 示例 3：
 * <p>
 * 输入：s = "2[abc]3[cd]ef"
 * 输出："abcabccdcdcdef"
 * 示例 4：
 * <p>
 * 输入：s = "abc3[cd]xyz"
 * 输出："abccdcdcdxyz"
 */
public class Solution394 {
    public static void main(String[] args){
        Solution394 solution394 = new Solution394();
        System.out.println(solution394.decodeString("3[a]2[bc]"));
        System.out.println(solution394.decodeString("3[a2[c]]"));
        System.out.println(solution394.decodeString("2[abc]3[cd]ef"));
        System.out.println(solution394.decodeString("abc3[cd]xy"));
        //"zzzyypqjkjkefjkjkefjkjkefjkjkefyypqjkjkefjkjkefjkjkefjkjkefef"
        //实际结果 "zzzyypqjkjkefpqjkjkefpqjkjkefpqjkjkefef"
        System.out.println(solution394.decodeString("3[z]2[2[y]pq4[2[jk]e1[f]]]ef"));
    }

    //还有两种实现思路
    //1. 用一个栈维护，遇到左括号就将前面的数字、字符串入栈，遇到右括号将前面的字符数字出栈
    //2. 使用递归的方式，遇到左括号就调用一次
    public String decodeString(String s) {
        StringBuilder resultBuilder = new StringBuilder();
        //重复字符串的个数
        StringBuilder preNumBuilder = new StringBuilder();
        //重复字符串
        StringBuilder repeatStringBuilder = new StringBuilder();
        Stack<String> numStack = new Stack<>();
        Stack<String> charStack = new Stack<>();
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (c >= '0' && c <= '9') {//数字
                preNumBuilder.append(c);
            }else if(c == '['){
                //3[a2[c]]
                //数字入栈
                String p = new String(preNumBuilder);
                preNumBuilder = new StringBuilder();
                if("".equals(p)){//没有技术保证有1个
                    p = "1";
                }
                numStack.add(p);
                //字符串入栈
                String repeatString = new String(repeatStringBuilder);
                repeatStringBuilder = new StringBuilder();
                if(numStack.size()<=1){//保证numStack栈顶的数字是处理当前正在拼装的字符
                    continue;
                }
                charStack.add(repeatString);


            }else if(c == ']'){
                String p = numStack.pop();
                //理论上入栈的数据不可能是空字符串 前面会自动处理成1
                if("".equals(p)){
                    System.out.println("error repeatCount is null string");
                    continue;
                }
                int repeat = Integer.parseInt(p);
                String repeatString = new String(repeatStringBuilder);
                repeatStringBuilder = new StringBuilder();
                for (int j=0;j<repeat;j++){
                    repeatStringBuilder.append(repeatString);
                }
                if(!charStack.isEmpty()){
                    String pre = charStack.pop();
                    StringBuilder temp = repeatStringBuilder;
                    repeatStringBuilder = new StringBuilder();
                    repeatStringBuilder.append(pre).append(temp);
                }else {
                    resultBuilder.append(repeatStringBuilder);
                    repeatStringBuilder = new StringBuilder();
                }
            }else if(!numStack.empty()){//数字栈不为空，
                repeatStringBuilder.append(c);
            }else {//遇到字符
                resultBuilder.append(c);
                repeatStringBuilder = new StringBuilder();
                preNumBuilder = new StringBuilder();
            }
        }
        return new String(resultBuilder);
    }


}
