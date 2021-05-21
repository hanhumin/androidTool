package com.txl.leetcode.sort;
//给定一个包含 [0，n) 中不重复整数的黑名单 blacklist ，写一个函数从 [0, n) 中返回一个不在 blacklist 中的随机整数。
//
// 对它进行优化使其尽量少调用系统方法 Math.random() 。
//
// 提示:
//
//
// 1 <= n <= 1000000000
// 0 <= blacklist.length < min(100000, N)
// [0, n) 不包含 n ，详细参见 interval notation 。
//
//
// 示例 1：
//
//
//输入：
//["Solution","pick","pick","pick"]
//[[1,[]],[],[],[]]
//输出：[null,0,0,0]
//
//
// 示例 2：
//
//
//输入：
//["Solution","pick","pick","pick"]
//[[2,[]],[],[],[]]
//输出：[null,1,1,1]
//
//
// 示例 3：
//
//
//输入：
//["Solution","pick","pick","pick"]
//[[3,[1]],[],[],[]]
//输出：[null,0,0,2]
//
//
// 示例 4：
//
//
//输入：
//["Solution","pick","pick","pick"]
//[[4,[2]],[],[],[]]
//输出：[null,1,3,1]
//
//
// 输入语法说明：
//
// 输入是两个列表：调用成员函数名和调用的参数。Solution的构造函数有两个参数，n 和黑名单 blacklist。pick 没有参数，输入参数是一个列表
//，即使参数为空，也会输入一个 [] 空列表。
// Related Topics 排序 哈希表 二分查找 Random
// 👍 67 👎 0


import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Set;

//leetcode submit region begin(Prohibit modification and deletion)
public class Solution710 {
    int n;
    int[] blacklist;
    Random r = new Random();
    /**
     * 白名单长度
     */
    int blacklistLength = 0;

    /**
     * 白名单长度
     * */
    int wlen = 0;
    Map<Integer,Integer> m = new HashMap<>();

    public Solution710(int n, int[] blacklist) {
        this.n = n;
        this.blacklist = blacklist;

        /*********************************黑名单映射 start********************************************/
        wlen = n - blacklist.length;
        Set<Integer> w = new HashSet<>();
        for (int i = wlen; i < n; i++) w.add(i);//注意下这个位置，白名单只添加了wl之后的数据
        for (int x : blacklist) w.remove(x);
        Iterator<Integer> wi = w.iterator();
        for (int x : blacklist)//如何确保，前序列的黑名单不会映射到 wl之前的白名单呢？
            if (x < wlen)
                m.put(x, wi.next());
        /***********************************黑名单映射 end******************************************/

        //升序排序
        Arrays.sort(this.blacklist);
        blacklistLength = blacklist == null ? 0 : blacklist.length;
    }


    /**
     * 使用集合 保存白名单
     */
    List<Integer> w = new ArrayList<>();

    public void solutionList() {
        Set<Integer> W = new HashSet<>();
        for (int i = 0; i < n; i++) W.add(i);
        for (int x : blacklist) W.remove(x);
        w.addAll(W);

        System.out.println("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
        System.out.print("[");
        for (int m = 0; m < w.size(); m++) {
            if (m == w.size() - 1) {
                System.out.print(" " + w.get(m));
            } else {
                System.out.print("" + w.get(m) + ", ");
            }
        }
        System.out.println("]");
        System.out.println("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
    }

    public int pickList(int k) {
        return w.get(k);
    }

    public int nextK() {
        return r.nextInt(n - blacklistLength);
    }


    /**
     * 直接使用二分法查找，
     * 在【0,n)中，除了黑名单，剩下的数都在白名单中
     */
    public int pick() {
        int k = nextK();
        return pick2fen(k);
    }

    /**
     * 参考官方
     * */
    private int pick2fengf(int k) {
        System.out.println("pick2fen start");
        if (blacklistLength == 0) {
            return k;
        }
        int l = 0, h = blacklist.length - 1;
        while (l < h) {
            int mid = (l + h + 1) / 2;
            //blacklist[mid]-mid 的含义是 当前黑名单中的数据 - 黑名单顺序数  = 当前位置之前可以插入的白名单个数
            int preCountW = blacklist[mid] - mid;
            if (preCountW > k) {//最大的坐标是 preCountW-1
                h = mid - 1;
            } else {
                l = mid;
            }
        }
        System.out.println("pick2fen end       l : " + l + " h: " + h);
        if (blacklist[l] - l <= k && l == h) {
            return l + k + 1;
        } else {
            return k;
        }
    }

    private int pick2fen(int k) {
        System.out.println("pick2fen start   "+k);
        if (blacklistLength == 0) {
            return k;
        }
        int l = 0, h = blacklist.length - 1;
        while (l != h) {
            int mid = (l + h) / 2;
            //blacklist[mid]-mid 的含义是 当前黑名单中的数据 - 黑名单顺序数  = 当前位置之前可以插入的白名单个数
            int preCountW = blacklist[mid] - mid;
            if (preCountW >= k+1) {//第k个实际有k+1个数  mid 是包含的
                h = mid;//因为 此时blacklist[mid]前肯定包含 k+1个白名单，但是不能确定 blacklist[mid-1]的情况，因此h不能减少
            } else {
                l = mid+1;//mid+1的左边也不能确定能够放置多少个？
            }
        }

        int preCountW = blacklist[h] - h;
        System.out.println("pick2fen end       l = " + l + " h= " + h+"  preCountW ="+preCountW +" k = "+k);
        if (preCountW >= k+1) {//preCountW >= k+1 保证落在第h个数的左侧
            //在h之前共有白名单的个数
            // preCountW -(k+1) 为当前位置距离第k个空白元素的间距  那么第k个的位置在哪里呢？k的坐标从0开始
            // 第k个到 第preCountW-1个的间距是 preCountW-1 -k ;总间距是 preCountW -k;
            // blacklist[h] - preCountW +k =h + preCountW - preCountW + k
            return  h+k;
        } else {
            //h + preCountW + (k - preCountW)
            return h+k+1;
        }
    }


    private static class Item {
        int n;
        int[] blacklist;

        public Item(int n, int[] blacklist) {
            this.n = n;
            this.blacklist = blacklist;
        }

        private static Item createItem() {
            int n = (int) (Math.random() * 10);
            int bl = (int) (Math.random() * n);
            int[] blacklist = new int[bl];
            Set<Integer> set = new HashSet<>();
            while (set.size() < bl) {
                set.add((int) (Math.random() * n));
            }
            int i = 0;
            for (Integer integer : set) {
                blacklist[i] = integer;
                i++;
            }
            return new Item(n, blacklist);
        }
    }

    public static void main(String[] args) {
//        test2fen();
        testMap();
    }


    /**
     * 测试黑名单映射
     * */
    private static void testMap(){
        Solution710 solution710 = new Solution710(12,new int[]{0,2,4,5,6,7,8,11});
        Iterator<Integer> iterator = solution710.m.keySet().iterator();
        while (iterator.hasNext()){
            Integer integer = iterator.next();
            System.out.println("key : "+integer+" --> value "+solution710.m.get(integer));
        }
    }

    /**
     * 测试二分
     */
    public static void test2fen() {
        for (int i = 0; i < 5000; i++) {
            Item item = Item.createItem();
            Solution710 solution710 = new Solution710(item.n, item.blacklist);
            solution710.solutionList();
            System.out.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ "+solution710.n);
            System.out.print("[");
            for (int m = 0; m < solution710.blacklist.length; m++) {
                if (m == solution710.blacklist.length - 1) {
                    System.out.print(" " + solution710.blacklist[m]);
                } else {
                    System.out.print("" + solution710.blacklist[m] + ", ");
                }
            }
            System.out.println("]");
            System.out.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
            for (int j = 0; j < solution710.n - solution710.blacklistLength; j++) {
                int nextK = solution710.nextK();
                int pickList = solution710.pickList(nextK);
                int pick2fen = solution710.pick2fen(nextK);
                if (pickList != pick2fen) {

                    throw new RuntimeException("not equals position " + j + " nextK is : " + nextK + "  " + "pick list is " + pickList + " pick2fen:  " + pick2fen + " n is:  " + solution710.n);
                }
            }
        }
    }

}
/**
 * Your Solution object will be instantiated and called as such:
 * Solution obj = new Solution(n, blacklist);
 * int param_1 = obj.pick();
 */
//leetcode submit region end(Prohibit modification and deletion)
