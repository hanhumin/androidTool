package com.txl.leetcode.top100;

//给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。答案可以按 任意顺序 返回。
//
// 给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。
//
//
//
//
//
// 示例 1：
//
//
//输入：digits = "23"
//输出：["ad","ae","af","bd","be","bf","cd","ce","cf"]
//
//
// 示例 2：
//
//
//输入：digits = ""
//输出：[]
//
//
// 示例 3：
//
//
//输入：digits = "2"
//输出：["a","b","c"]
//
//
//
//
// 提示：
//
//
// 0 <= digits.length <= 4
// digits[i] 是范围 ['2', '9'] 的一个数字。
//
// Related Topics 深度优先搜索 递归 字符串 回溯算法
// 👍 1360 👎 0


import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution17 {

    public List<String> letterCombinations(String digits) {
        List<String> combinations = new ArrayList<String>();
        if (digits.length() == 0) {
            return combinations;
        }
        Map<Character, String> phoneMap = new HashMap<Character, String>() {{
            put('2', "abc");
            put('3', "def");
            put('4', "ghi");
            put('5', "jkl");
            put('6', "mno");
            put('7', "pqrs");
            put('8', "tuv");
            put('9', "wxyz");
        }};
        backtrack(combinations, phoneMap, digits, 0, new StringBuffer());
        return combinations;
    }

    public void backtrack(List<String> combinations, Map<Character, String> phoneMap, String digits, int index, StringBuffer combination) {
        if (index == digits.length()) {
            combinations.add(combination.toString());
        } else {
            char digit = digits.charAt(index);
            String letters = phoneMap.get(digit);
            int lettersCount = letters.length();
            for (int i = 0; i < lettersCount; i++) {
                combination.append(letters.charAt(i));
                backtrack(combinations, phoneMap, digits, index + 1, combination);
                combination.deleteCharAt(index);
            }
        }
    }

    public List<String> letterCombinations1(String digits) {
        if(digits == null || digits.length() == 0){
            return new ArrayList<>();
        }
        ArrayList<String> result = new ArrayList<>();
        Map<Character, String> phoneMap = new HashMap<>();
        phoneMap.put('2', "abc");
        phoneMap.put('3', "def");
        phoneMap.put('4', "ghi");
        phoneMap.put('5', "jkl");
        phoneMap.put('6', "mno");
        phoneMap.put('7', "pqrs");
        phoneMap.put('8', "tuv");
        phoneMap.put('9', "wxyz");

        for (int i=0; i<digits.length(); i++){
            char c = digits.charAt(i);
            if(phoneMap.containsKey(c)){
                ArrayList<String> temp = new ArrayList<>();
                String current = phoneMap.get(c);
                int originLength = result.size();
                if(originLength == 0){
                    for (int j=0;j<current.length();j++){
                        result.add(""+current.charAt(j));
                    }
                }else {//假设原来是 输入23  此时数组中存在 a b c 三个字符串
                    for (int k=0;k<originLength;k++){//第一次循环后 ad ae af
                        String origin = result.get(k);
                        for (int j=0;j<current.length();j++){
                            result.add(origin+current.charAt(j));
                        }
                    }
                    result.subList(0, originLength).clear();
                }


            }
        }
        return result;
    }

    /**
     * 为什么比官方题解慢
     * */
    public List<String> letterCombinations2(String digits) {
        if(digits == null || digits.length() == 0){
            return new ArrayList<>();
        }
        ArrayList<String> result = new ArrayList<>();
        Map<Character, String> phoneMap = new HashMap<>();
        phoneMap.put('2', "abc");
        phoneMap.put('3', "def");
        phoneMap.put('4', "ghi");
        phoneMap.put('5', "jkl");
        phoneMap.put('6', "mno");
        phoneMap.put('7', "pqrs");
        phoneMap.put('8', "tuv");
        phoneMap.put('9', "wxyz");

        for (int i=0; i<digits.length(); i++){
            char c = digits.charAt(i);
            if(phoneMap.containsKey(c)){
                ArrayList<String> temp = new ArrayList<>();
                String current = phoneMap.get(c);
                int originLength = result.size();
                for (int j=0;j<current.length();j++){
                    if(originLength == 0){
                        temp.add(""+current.charAt(j));
                    }else {
                        for (int k=0;k<originLength;k++){//第一次循环后 ad ae af
                            String origin = result.get(k);
                            temp.add(origin+current.charAt(j));
                        }
                    }
                }
                result = temp;

            }
        }
        return result;
    }

}
//leetcode submit region end(Prohibit modification and deletion)
