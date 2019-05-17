package com.example.txl.tool.regular.expression;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class LearnRegularExpression {
    public static void main(String[] args){
        matchPoint("36");
        matchPoint("36.");
        matchPoint("36.2");
        matchPoint("36.23");
        matchPoint("36.234");
        matchPoint("36..234");
    }



    /**
     * 匹配 .  比如数字小数后最多保留两位 464565.00
     * */
    private static boolean matchPoint(String string){
        //表达式的含义：一个或多个数字开始，包含0个或一个小数点加一位或两位的精度结尾（具体理解是：如果出现 . 那么它的后面最少有一个数字，最多两个）
        String regex = "^[0-9]+(.?[0-9]{1,2})?$";
        Pattern pattern = Pattern.compile(regex);
        Matcher m = pattern.matcher(string);
        if(m.find()){
            System.out.println("matchPoint "+string+ " is find ");
            return true;
        }else {
            System.out.println("matchPoint "+string+ " not find ");
            return false;
        }
    }
}
