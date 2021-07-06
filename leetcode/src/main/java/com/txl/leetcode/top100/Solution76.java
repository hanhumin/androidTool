package com.txl.leetcode.top100;

/**
 * Copyright (c) 2021 唐小陆 All rights reserved.
 * author：txl
 * date：2021/3/14
 * description： 最小覆盖子串
 */

import com.txl.leetcode.linesweep.Solution850;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

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


    /**
     * 使用滑动窗口来做
     * 开始的时候，两个指针都在0位置，
     * */
    Map<Character, Integer> ori = new HashMap<Character, Integer>();
    Map<Character, Integer> cnt = new HashMap<Character, Integer>();

    public String minWindow(String s, String t) {
        int tLen = t.length();
        for (int i = 0; i < tLen; i++) {
            char c = t.charAt(i);
            ori.put(c, ori.getOrDefault(c, 0) + 1);
        }
        int l = 0, r = -1;
        int len = Integer.MAX_VALUE, ansL = -1, ansR = -1;
        int sLen = s.length();
        while (r < sLen) {
            ++r;
            if (r < sLen && ori.containsKey(s.charAt(r))) {
                cnt.put(s.charAt(r), cnt.getOrDefault(s.charAt(r), 0) + 1);
            }
            while (check() && l <= r) {
                if (r - l + 1 < len) {
                    len = r - l + 1;
                    ansL = l;
                    ansR = l + len;
                }
                if (ori.containsKey(s.charAt(l))) {
                    cnt.put(s.charAt(l), cnt.getOrDefault(s.charAt(l), 0) - 1);
                }
                ++l;
            }
        }
        return ansL == -1 ? "" : s.substring(ansL, ansR);
    }

    public boolean check() {
        Iterator iter = ori.entrySet().iterator();
        while (iter.hasNext()) {
            Map.Entry entry = (Map.Entry) iter.next();
            Character key = (Character) entry.getKey();
            Integer val = (Integer) entry.getValue();
            if (cnt.getOrDefault(key, 0) < val) {
                return false;
            }
        }
        return true;
    }

    /**
     * 会超出时间限制
     * */
    public String minWindow1(String s, String t) {
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

class Solution {

    /**
     * 同样的思路，为什么会比官方题解快？
     * n 是 t 的长度
     * 判断 就是 数组的查找比 map快 数组时间复杂度是 O(1)  map是O(lg n)
     * 同时 官方check的时间复杂度是t 的长度 n * lg n  而 此处为 O(1)
     * 利用了字符的取值范围
     * */
    public String minWindow(String s, String t) {

        int sLen = s.length();
        int tLen = t.length();
        if ( sLen == 0  || tLen == 0 || sLen < tLen ){
            return "";
        }
        char[] chars1 = s.toCharArray();
        char[] chars2 = t.toCharArray();


        int [] winFren = new int[128];
        int [] tFren = new int[128];


        for (char c : chars2) {
            tFren[c]++;
        }

        // 窗口子串包含t字符的频数,超过的部分不计算
        int distance = 0;
        // 子串最小长度
        int minlen = sLen+1;
        // 子串的起始点
        int start = 0;
        // 左指针
        int left = 0;
        // 右指针
        int right = 0;

        while (right < sLen){

            if (tFren[chars1[right]] == 0) {//t中没有这个字符，跳过
                right++;
                continue;
            }
            if (winFren[chars1[right]] <tFren[chars1[right]]) {
                distance++;
            }
            winFren[chars1[right]]++;
            right++;

            while (distance == tLen) {

                if ( right - left < minlen){
                    minlen =  right - left;
                    start = left;
                }


                if (tFren[chars1[left]] == 0) {
                    left++;
                    continue;
                }
                if (winFren[chars1[left]] == tFren[chars1[left]]) {
                    distance--;
                }
                winFren[chars1[left]]--;
                left++;
            }
        }

        if (minlen == sLen+1)return "";

        return s.substring(start,start+minlen);
    }
}
