package com.txl.leetcode.linesweep;

import com.txl.leetcode.teststatic.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;

//我们给出了一个（轴对齐的）矩形列表 rectangles 。 对于 rectangle[i] = [x1, y1, x2, y2]，其中（x1，y1）是矩形
// i 左下角的坐标，（x2，y2）是该矩形右上角的坐标。
//
// 找出平面中所有矩形叠加覆盖后的总面积。 由于答案可能太大，请返回它对 10 ^ 9 + 7 取模的结果。
//
//
//
// 示例 1：
//
// 输入：[[0,0,2,2],[1,0,2,3],[1,0,3,1]]
//输出：6
//解释：如图所示。
//
//
// 示例 2：
//
// 输入：[[0,0,1000000000,1000000000]]
//输出：49
//解释：答案是 10^18 对 (10^9 + 7) 取模的结果， 即 (10^9)^2 → (-7)^2 = 49 。
//
//
// 提示：
//
//
// 1 <= rectangles.length <= 200
// rectanges[i].length = 4
// 0 <= rectangles[i][j] <= 10^9
// 矩形叠加覆盖后的总面积不会超越 2^63 - 1 ，这意味着可以用一个 64 位有符号整数来保存面积结果。
//
// Related Topics 线段树 Line Sweep
// 👍 71 👎 0
public class Solution850 {
//    int maxY = 0;
    int o = 0;
    public int rectangleArea(final int[][] rectangles) {
        new Test.TestStaticName();
        int ans = 0;
//        maxY = rectangles[0][3];
        System.out.println("length is "+rectangles.length);
        final int[][] order = new int[rectangles.length][2];
        order[0][0] = rectangles[0][1];
        order[0][1] = rectangles[0][3];
        Arrays.sort(rectangles, new Comparator<int[]>() {
            @Override
            public int compare(int[] ints, int[] t1) {
//                maxY = Math.max(maxY,ints[3]);

                if(o<rectangles.length){
                    order[o][0] = ints[1];
                    order[o][1] = ints[3];
                    o++;
                }

                return ints[0] == t1[0]?ints[3]-t1[3]:ints[0]-t1[0];
            }
        });

        Arrays.sort(order, new Comparator<int[]>() {
            @Override
            public int compare(int[] ints, int[] t1) {
                o++;
                return ints[0] == t1[0]?ints[1]-t1[1]:ints[0]-t1[0];
            }
        });
        ArrayList<Node> nodes = new ArrayList<>();
        Node node;
        node = new Node();
        node.start = order[0][0];
        node.end = order[0][1];
        nodes.add(node);
        Node last = node;
        for (int curr[]:order){
            if(last.end >= curr[1]){//被包含，什么都不做
            }else if(curr[0]>last.end){//超出
                node = new Node();
                node.start = curr[0];
                node.end = curr[1];
                nodes.add(node);
                last = node;
            }else {//交叉
                node.end = curr[1];
            }
        }
        for (int curent[]:rectangles){
            System.out.println("sort value is { "+curent[0]+", "+curent[1]+", "+curent[2]+", "+curent[3]+"}");
        }
        System.out.println("******************************************************************************************");
        for (int curr[]:rectangles){
            System.out.println("sort curr value is { "+curr[0]+", "+curr[1]+"}");
        }
        System.out.println("******************************************************************************************");
        for(Node item:nodes){
            System.out.println("sort nodes is { "+item.start+", "+item.end+"}");
        }
        long MOD = 1_000_000_007;
        if(true){
            return 0;
        }
        if(rectangles.length == 1){
            ans = (int) ((rectangles[0][3]-rectangles[0][1])%MOD * (rectangles[0][2]-rectangles[0][0])%MOD);
        }else {
            for(Node item:nodes){
                for (int i=item.start;i<=item.end;i++){//i 代表y坐标
//        int i = 3;
                    int[] end, prev_end = null;
                    for (int[] curr : rectangles) {

                        end = curr;
                        if(prev_end != null){
                            if (prev_end[2] >= end[2]) {//后续被覆盖,不做处理
                                System.out.println("i is "+i+"  do nothing ans "+ans);
                            }else if(end[0]>=prev_end[2]){//后续与前一个完全独立
                                if(inRange(i,curr)) {//不在此区间范围内
                                    ans += end[2]-end[0];
                                    System.out.println("i is "+i+"  or ans "+ans+"  num is "+(end[2]-end[0]));
                                    ans %= MOD;
                                    prev_end = end;
                                }
                            }else {//有相交区域
                                if(inRange(i,curr)) {//不在此区间范围内
                                    ans += end[2]-prev_end[2];
                                    System.out.println("i is "+i+"  and ans "+ans+"  num is "+(end[2]-prev_end[2]));
                                    ans %= MOD;
                                    prev_end = end;
                                }
                            }
                        }else {
                            if(inRange(i,curr)){//不在此区间范围内
                                ans += end[2]-end[0];
                                prev_end = end;
                                ans %= MOD;
                            }
                            System.out.println("i is "+i+"  first add num ans "+ans+"  num is "+(end[2]-end[0]));
                        }
                    }
                }
            }

        }


        return ans;
    }

    private boolean inRange(int y,int[] curr){
        return y > curr[1] && y <= curr[3];
    }

   public static class Node{
        int start;
        int end;

        @Override
        public boolean equals(Object o) {
            if(o instanceof Solution1288.Node){
                return this.start == ((Solution1288.Node) o).start && this.end == ((Solution1288.Node) o).end;
            }
            return false;
        }
    }

    public static void main(String[] args){
//        System.out.println("result is "+new Solution850().rectangleArea(new int[][]{{0,0,2,2},{1,0,2,3},{1,0,3,1}}));
//        System.out.println("result is "+new Solution850().rectangleArea(new int[][]{{0,0,1,1},{2,2,3,3}}));
//        System.out.println("result is "+new Solution850().rectangleArea(new int[][]{{0,0,1000000000,1000000000}}));
//        System.out.println("result is "+new Solution850().rectangleArea(new int[][]{{0,0,3,3},{2,0,5,3},{1,1,4,4}}));
        int m[][] = new int[][]{{224386961,128668997,546647847,318900555},{852286866,238086790,992627088,949888275},{160239672,137108804,398130330,944807066},{431047948,462092719,870611028,856851714},{736895365,511285772,906155231,721626624},{289309389,607009433,558359552,883664714},{780746435,397872372,931219192,863727103},{573523994,124874359,889018012,471879750},{619886375,149607927,727026507,446976526},{51739879,716225241,115331335,785850603},{171077223,267051983,548436248,349498903},{314437215,169054168,950814572,481179241},{64126215,646689712,595562376,829164135},{926011655,481539702,982179297,832455610},{40370235,231510218,770233582,851797196},{292546319,45032676,413358795,783606009},{424366277,369838051,453541063,777456024},{211837048,142665527,217366958,952362711},{228416869,402115549,672143142,644930626},{755018294,194555696,846854520,939022548},{192890972,586071668,992336688,759060552},{127869582,392855032,338983665,954245205},{665603955,208757599,767586006,276627875},{260384651,10960359,736299693,761411808},{46440611,559601039,911666265,904518674},{54013763,90331595,332153447,106222561},{73093292,378586103,423488105,826750366},{327100855,516514806,676134763,653520887},{930781786,407609872,960671631,510621750},{35479655,449171431,931212840,617916927}};
        System.out.println("result is "+new Solution850().rectangleArea(m));
    }
}
