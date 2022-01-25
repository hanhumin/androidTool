package com.txl.leetcode.code;

public class Solution3 {
    public static void main(String[] args){

        System.out.println("??? "+new Solution3().lengthOfLongestSubstring("abcabcbb"));
    }

    public int lengthOfLongestSubstring(String s) {
        if(s == null || s.length() == 0){
            return 0;
        }
        int size = s.length();

        int start = 0, end = 0;
        int max = 0;
        Node[] temp = new Node[256];
        for (;end<size;end++){
           char ch = s.charAt(end);
           Node node = temp[ch];
           if(node != null){//说明原来有数据,需要把start 到 position的数据全部清除
               for (int i=start;i<= node.position;i++){
                   temp[s.charAt(i)] = null;
               }
               start = node.position+1;
           }
           temp[ch] = new Node(end);
           max = Math.max(max,end-start+1);
        }
        return max;
    }

    static class Node{
        //原来在字符串中的位置
        private final int position;

        public Node(int position) {
            this.position = position;
        }
    }
}
