package com.txl.leetcode.code;

import java.util.HashMap;

public class Solution290 {

    public static void main(String[] args){
        System.out.println("result : "+new Solution290().wordPattern("abba","dog cat cat dog"));
    }


    public boolean wordPattern(String pattern, String s) {
        String[] words = s.split(" ");
        HashMap<Character,String> map = new HashMap<>();
        if(pattern.length() != words.length){
            System.out.println("length is not ==");
            return false;
        }

        for (int i =0; i<words.length;i++){
            String old = map.put(pattern.charAt(i),words[i]);
            if(old != null && !words[i].equals(old)){
                System.out.println("?? "+pattern.charAt(i) +"  v:  "+words[i]);
                return false;
            }
        }
        return true;
    }

    @Override
    public int hashCode() {
        return super.hashCode();
    }
}
