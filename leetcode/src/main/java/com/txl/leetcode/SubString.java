package com.txl.leetcode;

import java.util.HashSet;
import java.util.Set;
//给定一个字符串，请你找出其中不含有重复字符的 最长子串 的长度。
//
//
//
// 示例 1:
//
//
//输入: s = "abcabcbb"
//输出: 3
//解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
//
//
// 示例 2:
//
//
//输入: s = "bbbbb"
//输出: 1
//解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。
//
//
// 示例 3:
//
//
//输入: s = "pwwkew"
//输出: 3
//解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。
//     请注意，你的答案必须是 子串 的长度，"pwke" 是一个子序列，不是子串。
//
//
// 示例 4:
//
//
//输入: s = ""
//输出: 0
//
//
//
//
// 提示：
//
//
// 0 <= s.length <= 5 * 104
// s 由英文字母、数字、符号和空格组成
//
// Related Topics 哈希表 双指针 字符串 Sliding Window
// 👍 5114 👎 0

/**
 * Copyright (c) 2021 唐小陆 All rights reserved.
 * author：txl
 * date：2021/3/14
 * description：给定一个字符串，请你找出其中不含有重复字符的 最长子串 的长度。
 * 解题思路：这个是一个滑动窗口问题，当前不包含重复字符时向右增加不包含重复子串的长度，
 * 否则从左减少至不包含重复字符串的长度为止
 */
class SubString {
    //版本一，以一个HashSet来存储没有重复的子串
    public int lengthOfLongestSubstring(String s) {
        if (s == null || s.length() == 0) {
            return 0;
        }
        int size = s.length();
        //用于保存无重复的字符
        Set<Character> characterHashSet = new HashSet<Character>();
        //滑动窗口的起始位置
        int start = 0;
        int max = 0;
        for (int i = 0; i < size; i++) {
            char item = s.charAt(i);
            while (characterHashSet.contains(item)) {
                characterHashSet.remove(s.charAt(start));
                start++;
            }
            characterHashSet.add(item);
            max = Math.max(max, characterHashSet.size());
        }
        return max;
    }

    /**
     * 不利用其它容器存储，直接利用字符串本身来进行处理
     */
    public int lengthOfLongestSubstring2(String s) {
        if (s == null || s.length() == 0) {
            return 0;
        }
        int size = s.length();
        int start = 0;
        int max = 1;
        for (int i = 0; i < size; i++) {
            for (int j = start; j < i; j++) {
                if(s.charAt(i) == s.charAt(j)){
                    start = j+1;
                    break;//子串中没有重复的
                }
            }
            max = Math.max(max,i-start+1);
        }
        return max;
    }
}
