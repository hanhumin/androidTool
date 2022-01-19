package com.txl.leetcode.code;

public class Solution211 {
    class WordDictionary {
        //是否为结束标记
        boolean isEnd = false;
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
            root.isEnd = true;
        }

        public boolean search(String word) {
            return search(this,word,0);
        }

        private boolean search(WordDictionary wordDictionary,String word, int index){
            if(wordDictionary == null){
                return false;
            }
            WordDictionary root = wordDictionary;
            for (int i = index; i < word.length(); i++) {
                //遇到. 表示当前节点下的所有可用子节点都可以遍历
                //如果搜索的最后一个字符是 . 在进入下一级循环的时候 因为index = word.length() 不能进入 循环，判断依据是root.isEnd  逻辑ok
                //如果搜索的字符包含 .
                if(word.charAt(i) == '.'){
                    for (int j = 0; j < 26; j++) {
                        if(search(root.wordDictionaries[j],word,i+1)){
                            return true;
                        }
                    }
                    return false;
                }else {
                    int p = word.charAt(i) - 'a';
                    if(root.wordDictionaries[p] != null){
                        root = root.wordDictionaries[p];
                    }else {//没有找到直接返回false
                        return false;
                    }
                }
            }
            return root!= null && root.isEnd;
        }
    }
}
