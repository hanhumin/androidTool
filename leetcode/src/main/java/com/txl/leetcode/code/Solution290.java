package com.txl.leetcode.code;

import java.util.HashMap;

public class Solution290 {

    public static void main(String[] args){
        System.out.println("result : "+new Solution290().wordPattern("abba","dog cat cat dog"));
        System.out.println("result : "+new Solution290().wordPattern("abba","dog cat cat fish"));
    }


    public boolean wordPattern(String pattern, String s) {
        String[] words = s.split(" ");
        //他们的关系是一一对应
        HashMap<Character,String> map = new HashMap<>();
        HashMap<String,Character> map2 = new HashMap<>();
        if(pattern.length() != words.length){
            System.out.println("length is not ==");
            return false;
        }

        for (int i =0; i<words.length;i++){
            String old = map.put(pattern.charAt(i),words[i]);
            if((old != null && !words[i].equals(old))){
                System.out.println("?? "+pattern.charAt(i) +"  v:  "+words[i]);
                return false;
            }
            Character old2 = map2.put(words[i],pattern.charAt(i));
            if(old2 != null && old2 != pattern.charAt(i)){
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
