package com.txl.leetcode;

/**
 * Copyright (c) 2021 唐小陆 All rights reserved.
 * author：txl
 * date：2021/3/14
 * description： 最小覆盖子串
 */

import com.txl.leetcode.linesweep.Solution850;

/**
 *
 * 给你一个字符串 s 、一个字符串 t 。返回 s 中涵盖 t 所有字符的最小子串。如果 s 中不存在涵盖 t 所有字符的子串，则返回空字符串 "" 。
 *
 * 注意：如果 s 中存在这样的子串，我们保证它是唯一的答案。
 *
 *  
 *
 * 示例 1：
 *
 * 输入：s = "ADOBECODEBANC", t = "ABC"
 * 输出："BANC"
 * 示例 2：
 *
 * 输入：s = "a", t = "a"
 * 输出："a"
 *  
 *
 * 提示：
 *
 * 1 <= s.length, t.length <= 105
 * s 和 t 由英文字母组成
 *  
 *
 * 进阶：你能设计一个在 o(n) 时间内解决此问题的算法吗？
 *
 * 来源：力扣（LeetCode）
 * 链接：https://leetcode-cn.com/problems/minimum-window-substring
 * 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。*/
class Solution76 {

    public static void main(String[] argus){
        System.out.println( "result :: "+new Solution76().minWindow("ADOBECODEBANC","ABC") );
        new Solution850.Node();
    }

    public String minWindow(String s, String t) {
        if(s == null || s.length() ==0){
            return "";
        }
        if(t == null || t.length() ==0){
            return "";
        }
        if(s.length() < t.length()){
            return "";
        }
        int start = 0;
        int end = t.length()-1;
        for (int i = end; i<s.length();i++){
            boolean containAll = true;
            String result = s.substring(start,end);
            for (int j =  0;j<t.length();j++){//判断是不是包含所有的字符串
                if(!hasChar(result,t.charAt(j))){
                    containAll = false;
                    break;
                }
            }
            if(containAll) {//包含所有
                if(result.length() == t.length()){
                    return result;
                }else {//减小起点指针
                    for (int j=start+1;end-j>=t.length();j++){
                        if(containAll(s.substring(j,end),t)){
                            start = j;
                        }else {
                            break;
                        }
                    }
                    if(i == s.length()-1){
                        return s.substring(start,end);
                    }
                }
            }
            end++;
        }
        return "";
    }

    private boolean hasChar(String s , char c){
        if(s == null || s.length() == 0){
            return false;
        }
        for (int i = 0; i<s.length();i++){
            if(c == s.charAt(i)){
                return true;
            }
        }
        return false;
    }

    private boolean containAll(String s , String subString){
        if(s == null || s.length() == 0){
            return false;
        }

        if(subString == null || subString.length() == 0){
            return true;
        }
        if(subString.length() > s.length()){
            return false;
        }
        for (int i = 0; i<subString.length();i++){
            if(!hasChar(s,subString.charAt(i))){
                return false;
            }
        }
        return true;
    }

}
