package com.txl.leetcode.code;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

public class Solution212 {

    public static void main(String[] args){
        //fixme 为什么是空数组？
        System.out.println("result is : "+new Solution212().findWords(new char[][]{{'a'}},new String[]{"a"}));
    }

    int[][] dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    public List<String> findWords(char[][] board, String[] words) {
        WordDictionary wordDictionary = new WordDictionary();
        for (String word : words) {
            wordDictionary.addWord(word);
        }
        HashSet<String> result = new HashSet<>();
        for (int i=0; i< board.length; i++){
            for (int j = 0; j < board[0].length;j++){
                dfs(board,wordDictionary,result,i,j);
            }
        }
        return new ArrayList<>(result);
    }

    private void dfs(char[][] board, WordDictionary wordDictionary, HashSet<String> result, int i0, int j0){
        if(!"".equals(wordDictionary.word)){//查找到了以此结尾的单词
            result.add(wordDictionary.word);
        }
        //i0 j0 是网格中将要处理的位置
        char ch = board[i0][j0];
        if(ch == '#'){//当前位置访问过，退出
            return;
        }
        int index = ch - 'a';
        if(wordDictionary.wordDictionaries[index] == null){//这个字符在当前树中不存在
            return;
        }
        board[i0][j0] = '#';
        for (int i=0;i<dirs.length;i++){
            int ii = dirs[i][0] + i0;
            int jj = dirs[i][1] + j0;
            if(ii < board.length && ii >= 0 && jj < board[0].length && jj >= 0){
                dfs(board,wordDictionary.wordDictionaries[index],result,ii,jj);
            }
        }
        board[i0][j0] = ch;
    }

    class WordDictionary {
        //是否为结束标记
        String word = "";
        WordDictionary[] wordDictionaries = new WordDictionary[26];
//        WordDictionary root;

        public WordDictionary() {

        }

        public void addWord(String word) {
            WordDictionary root = this;
            for (int i = 0; i < word.length(); i++) {
                //没必要考虑这个输入的时候有. 题目已经明确输入只有小写字母
//                char c = word.charAt(i) == '.' ? 'a' : word.charAt(i);
                int index = word.charAt(i) - 'a';
                if(root.wordDictionaries[index] == null){
                    root.wordDictionaries[index] = new WordDictionary();
                }
                root = root.wordDictionaries[index];
            }
            //记录插入的字符
            root.word = word;
        }
    }
}
