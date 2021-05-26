package com.txl.leetcode.sort;
//现在有一个特殊的排名系统，依据参赛团队在投票人心中的次序进行排名，每个投票者都需要按从高到低的顺序对参与排名的所有团队进行排位。
//
// 排名规则如下：
//
//
// 参赛团队的排名次序依照其所获「排位第一」的票的多少决定。如果存在多个团队并列的情况，将继续考虑其「排位第二」的票的数量。以此类推，直到不再存在并列的情况。
//
// 如果在考虑完所有投票情况后仍然出现并列现象，则根据团队字母的字母顺序进行排名。
//
//
// 给你一个字符串数组 votes 代表全体投票者给出的排位情况，请你根据上述排名规则对所有参赛团队进行排名。
//
// 请你返回能表示按排名系统 排序后 的所有团队排名的字符串。
//
//
//
// 示例 1：
//
// 输入：votes = ["ABC","ACB","ABC","ACB","ACB"]
//输出："ACB"
//解释：A 队获得五票「排位第一」，没有其他队获得「排位第一」，所以 A 队排名第一。
//B 队获得两票「排位第二」，三票「排位第三」。
//C 队获得三票「排位第二」，两票「排位第三」。
//由于 C 队「排位第二」的票数较多，所以 C 队排第二，B 队排第三。
//
//
// 示例 2：
//
// 输入：votes = ["WXYZ","XYZW"]
//输出："XWYZ"
//解释：X 队在并列僵局打破后成为排名第一的团队。X 队和 W 队的「排位第一」票数一样，但是 X 队有一票「排位第二」，而 W 没有获得「排位第二」。
//
//
// 示例 3：
//
// 输入：votes = ["ZMNAGUEDSJYLBOPHRQICWFXTVK"]
//输出："ZMNAGUEDSJYLBOPHRQICWFXTVK"
//解释：只有一个投票者，所以排名完全按照他的意愿。
//
//
// 示例 4：
//
// 输入：votes = ["BCA","CAB","CBA","ABC","ACB","BAC"]
//输出："ABC"
//解释：
//A 队获得两票「排位第一」，两票「排位第二」，两票「排位第三」。
//B 队获得两票「排位第一」，两票「排位第二」，两票「排位第三」。
//C 队获得两票「排位第一」，两票「排位第二」，两票「排位第三」。
//完全并列，所以我们需要按照字母升序排名。
//
//
// 示例 5：
//
// 输入：votes = ["M","M","M","M"]
//输出："M"
//解释：只有 M 队参赛，所以它排名第一。
//
//
//
//
// 提示：
//
//
// 1 <= votes.length <= 1000
// 1 <= votes[i].length <= 26
// votes[i].length == votes[j].length for 0 <= i, j < votes.length
// votes[i][j] 是英文 大写 字母
// votes[i] 中的所有字母都是唯一的
// votes[0] 中出现的所有字母 同样也 出现在 votes[j] 中，其中 1 <= j < votes.length
//
// Related Topics 排序 数组
// 👍 52 👎 0


import com.txl.leetcode.Logarithm;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;

import java.util.TreeSet;


//leetcode submit region begin(Prohibit modification and deletion)
class Solution1366 {


    public static void main(String[] args){
        for (int i=1;i>0;i--){
//            String[] votes = Logarithm.generate1366(5,5);
//        String[] votes = new String[]{"DXUO","UDXO","UOXD","DOXU"};
        String[] votes = new String[]{"BCA","CAB","CBA","ABC","ACB","BAC"};
            System.out.println("============================================start========================================================");
            for (String s:votes){
                System.out.println("value : "+s);
            }
            System.out.println("=============================================end=======================================================");
            Solution1366 solution1366 = new Solution1366();
            String v1 = solution1366.rankTeams(votes);
            String v2 = solution1366.solution2(votes);
            if(!v1.equals(v2)){
                throw new RuntimeException("not equals v1 is "+v1+"   v2 is "+v2);
            }
        }

    }

    public String rankTeams(String[] votes) {
        return solution1(votes);
    }


    private String solution2(String[] votes){
        if(votes == null || votes.length == 0 || votes[0] == null || votes[0].length() == 0){
            return "";
        }
        ArrayList<OrderNode> orderList = new ArrayList<>();
        int length = votes[0].length();
        for (int i=0;i<length;i++){
            orderList.add(new OrderNode(votes[0].charAt(i),0));
        }
        sort2(new int[][]{{0,votes[0].length()-1}},0,votes,orderList);
        StringBuilder sb = new StringBuilder();
        for (OrderNode o:orderList){
            sb.append(o.character);
        }
        return new String(sb);
    }


    private void sort2(int[][] area,int position,String[] votes,ArrayList<OrderNode> orderList){
        if(area == null || area.length == 0 || position >= votes[0].length()){
            return;
        }
        HashMap<Character,OrderNode> orderNodeHashMap = new HashMap<>();
        for (int i=0;i<votes.length;i++){
            char c  = votes[i].charAt(position);
            OrderNode orderNode = orderNodeHashMap.get(c);
            if(orderNode != null){
                orderNode.value ++;
            }else {
                orderNode = new OrderNode(c, 1);
            }
            orderNodeHashMap.put(c,orderNode);
        }

        for (int i=0;i<area.length;i++){
            int start = area[i][0];
            int end = area[i][1];
            ArrayList<OrderNode> arrayList = new ArrayList<>();
            int temp = start;
            while (temp<=end){
                OrderNode originNode = orderList.get(temp);
                OrderNode newNode = orderNodeHashMap.get(originNode.character);
                if(newNode == null){
                    newNode = new OrderNode(originNode.character,0);
                }
                arrayList.add(newNode);
                temp++;
            }
            Collections.sort(arrayList, new Comparator<OrderNode>() {
                @Override
                public int compare(OrderNode o1, OrderNode o2) {
                    return o2.value-o1.value;
                }
            });
            for (int j=0;j<arrayList.size();j++){
                orderList.set(start+j,arrayList.get(j));
            }
            int lastValue = -1;
            int lastIndex = start;
            ArrayList<int[]> areaList = new ArrayList<>();
            for (int j=start;j<=end;j++){
                OrderNode orderNode = orderList.get(j);
                if(orderNode.value != lastValue){
                    if( lastIndex!= -1 && j-1>lastIndex){//至少需要保证两个数
                        areaList.add(new int[]{lastIndex,j-1});
                    }
                    lastValue = orderNode.value;
                    lastIndex = j;
                }
            }
            if(lastIndex != end){
                areaList.add(new int[]{lastIndex,end});
            }
            if(areaList.size() > 0){
                int[][] myArea = new int[areaList.size()][2];
                for (int k=0;k<areaList.size();k++){
                    myArea[k][0] = areaList.get(k)[0];
                    myArea[k][1] = areaList.get(k)[1];
                }
                sort2(myArea,position+1,votes,orderList);
            }
        }

    }

    /**
     * */
    private String solution1(String[] votes){
        if(votes == null || votes.length == 0 || votes[0] == null || votes[0].length() == 0){
            return "";
        }
        ArrayList<OrderNode> orderList = new ArrayList<>();
        int length = votes[0].length();
        for (int i=0;i<length;i++){
            orderList.add(new OrderNode(votes[0].charAt(i),0));
        }
        sort(0,votes[0].length()-1,orderList,0,votes);
        StringBuilder sb = new StringBuilder();
        for (OrderNode o:orderList){
            sb.append(o.character);
        }
        return new String(sb);
    }

    /**
     * @param start 需要进行排序处理的列表的开始坐标
     * @param end 需要进行排序处理的列表的结束坐标
     * @param orderList 需要进行排序的列表
     * @param position votes[] 数组字符串第几个位置
     * @param votes 输入字符串
     * */
    private void sort(int start,int end,ArrayList<OrderNode> orderList,int position,String[] votes){
        if(start >= end){
            return;
        }
        if(position >= votes[0].length()){
            return;
        }
        TreeSet<OrderNode> treeSet = new TreeSet<>(new Comparator<OrderNode>() {
            @Override
            public int compare(OrderNode o1, OrderNode o2) {
                if(o1.value == o2.value){
                    return 1;
                }
                return o2.value-o1.value;
            }
        });
        HashMap<Character,OrderNode> orderNodeHashMap = new HashMap<>();
        for(int i=start;i<=end;i++){
            OrderNode orderNode = orderList.get(i);
            orderNode.value = 0;
            orderNodeHashMap.put(orderNode.character,orderNode);
        }

        for (String s:votes){
            Character character = s.charAt(position);
            if(orderNodeHashMap.containsKey(character)){
                OrderNode orderNode = orderNodeHashMap.get(character);
                orderNode.value++;
                orderNodeHashMap.put(character,orderNode);
            }
        }
        treeSet.addAll(orderNodeHashMap.values());
        for (int i=start;i<=end;i++){
            orderList.set(i, treeSet.pollFirst());
        }
        int lastValue = -1;
        int lastIndex = start;
        int count = 0;
        for (int i=start;i<=end;i++){
            OrderNode orderNode = orderList.get(i);
            if(orderNode.value != lastValue){
                sort(lastIndex,i-1,orderList,position+1,votes);//不相等，需要排序
                lastValue = orderNode.value;
                lastIndex = i;
                count = 0;
            }else {
                count++;
            }

        }
        if(count > 0){
            sort(lastIndex,end,orderList,position+1,votes);//不相等，需要排序
        }

    }

    private static class OrderNode{
        Character character;
        int value = 0;

        public OrderNode(Character character, int value) {
            this.character = character;
            this.value = value;
        }

        @Override
        public boolean equals(Object obj) {
            if(obj instanceof OrderNode && this.character == ((OrderNode) obj).character){
                return true;
            }
            return false;
        }

        @Override
        public int hashCode() {
            return character.hashCode();
        }
    }

}
//leetcode submit region end(Prohibit modification and deletion)
