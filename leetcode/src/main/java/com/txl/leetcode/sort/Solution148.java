package com.txl.leetcode.sort;

//给你链表的头结点 head ，请将其按 升序 排列并返回 排序后的链表 。
//
// 进阶：
//
//
// 你可以在 O(n log n) 时间复杂度和常数级空间复杂度下，对链表进行排序吗？
//
//
//
//
// 示例 1：
//
//
//输入：head = [4,2,1,3]
//输出：[1,2,3,4]
//
//
// 示例 2：
//
//
//输入：head = [-1,5,3,4,0]
//输出：[-1,0,3,4,5]
//
//
// 示例 3：
//
//
//输入：head = []
//输出：[]
//
//
//
//
// 提示：
//
//
// 链表中节点的数目在范围 [0, 5 * 104] 内
// -105 <= Node.val <= 105
//
// Related Topics 排序 链表
// 👍 1129 👎 0


//leetcode submit region begin(Prohibit modification and deletion)

import java.util.Comparator;
import java.util.Random;
import java.util.TreeSet;

/**
 * Definition for singly-linked list.
 * public class ListNode {
 * int val;
 * ListNode next;
 * ListNode() {}
 * ListNode(int val) { this.val = val; }
 * ListNode(int val, ListNode next) { this.val = val; this.next = next; }
 * }
 */
class Solution148 {

    public static ListNode createListNode(int size) {
        Random random = new Random();
        size = (int) (Math.random() * size) + 1;//加1保证链表不为空
        ListNode listNode = null;
        ListNode head = null;
        for (int i = 0; i < size; i++) {
            if (listNode == null) {
                listNode = new ListNode();
                head = listNode;
                listNode.val = (int) (Math.random() * 100);
//                listNode.val = random.nextInt();
            } else {
                listNode.next = new ListNode();
                listNode.next.val = (int) (Math.random() * 100);
//                listNode.next.val = random.nextInt();
                listNode = listNode.next;
            }
        }
        return head;
    }

    public static void main(String[] args) {
        for (int i = 0; i < 5000; i++) {
            ListNode listNode = createListNode(10);
            ListNode copy = copyNode(listNode);
            copy = new Solution148().solution1(copy);
//           listNode = new Solution148().solution2(listNode);
            listNode = new Solution148().solution2(listNode);
//            listNode = new Solution148().solution3(listNode);
            testNodeEquals(listNode, copy);
        }
    }

    private static void printNodeValue(ListNode listNode, String message) {
        System.out.println("====================================" + message + "========================================");
        System.out.print("{");
        while (listNode != null) {
            System.out.print(listNode.val + "，");
            listNode = listNode.next;
        }
        System.out.println("}");
    }

//    public ListNode sortList(ListNode head) {
//
//    }

    //解题方法1 ： 自己构建一个链表顺序插入  插入排序
    //注意在插入自己排序的链表里面插入的时候一定要先获取下一个节点
    //fixme 代码提交超出时间限制 时间复杂度 n^2
    public ListNode solution1(ListNode head) {
        //保存已经排序的链表的头结点
        ListNode sortHead = null;
        while (head != null) {
            ListNode next = head.next;
            head.next = null;
            if (sortHead == null) {
                sortHead = head;
            } else {
                ListNode temp = sortHead;//从排好序的链表头进行遍历插入
                ListNode pre = null;
                while (temp != null) {
                    if (head.val < temp.val) {//插入到之前
                        if (pre == null) {//头指针  把head放在新的链表头
                            head.next = sortHead;
                            sortHead = head;
                        } else {
                            pre.next = head;
                            head.next = temp;
                        }
                        break;
                    }
                    if (temp.next == null) {
                        temp.next = head;
                        break;
                    } else {
                        pre = temp;
                        temp = temp.next;
                    }
                }
            }
            head = next;
        }
        return sortHead;
    }

    //思路二 借助treeSet 将待排序的链表存储，然后依次更改位置
    public ListNode solution2(ListNode head) {
        TreeSet<ListNode> listNodes = new TreeSet<>(new Comparator<ListNode>() {
            @Override
            public int compare(ListNode o1, ListNode o2) {
                if (o1.val == o2.val) {
                    return -1;//为了保证不会将相同的数据过滤掉，后面来的总是比前面大
                } else if (o1.val < o2.val) {
                    return -1;
                }
                return 1;
//                return o1.val-o2.val;//越界就会出问题
            }
        });
        ListNode node = head;
        while (node != null) {
            listNodes.add(node);//会过滤掉相同的数据
            ListNode temp = node;
            node = node.next;
            temp.next = null;
        }
        ListNode lastNode = null;
        while (!listNodes.isEmpty()) {
            if (lastNode == null) {
                head = listNodes.pollFirst();
                lastNode = head;
            } else {
                lastNode.next = listNodes.pollFirst();
                lastNode = lastNode.next;
            }
        }
        return head;
    }


    //有没有可能使用 归并排序的思想来处理呢？
    //递归版  将链表分成两个
    public ListNode solution3(ListNode head) {

        return sortList(head, null);
    }


    /**
     * @param head 链表表头
     * @param tail 链表尾
     *             这个是来自官方的样例，处理时没有把tail包含进链表
     *             递归版本
     */
    public ListNode sortList(ListNode head, ListNode tail) {
        if (head == null) {
            return head;
        }
        if (head.next == tail) {//next 等于tail 相当于下一个链表的开始位置
            head.next = null;
            return head;
        }
        ListNode fast = head, slow = head;
        while (fast != tail) {
            fast = fast.next;
            slow = slow.next;
            if (fast != tail) {
                fast = fast.next;
            }
        }
        ListNode mid = slow;
        ListNode list1 = sortList(head, mid);
        ListNode list2 = sortList(mid, tail);
        ListNode sorted = mergeListNode(list1, list2);
        return sorted;
    }

    /**
     * 非递归版归并排序
     * 非递归版代码技巧如下：
     * 1.为了方便
     */
    public ListNode sortList2(ListNode head) {
        if (head == null) {
            return head;
        }
        int length = 0;
        ListNode prev = head;
        while (prev != null) {
            length++;
            prev = prev.next;
        }
        ListNode dummyHead = new ListNode(0, head);//生成一个节点方便处理头结点

        for (int step = 1; step < length; step <<= 1) {
            prev = dummyHead;
            ListNode curr = dummyHead.next;
            while (curr!=null){
                ListNode head1 = curr;
                for (int i = 1; i < step && curr.next != null; i++) {
                    curr = curr.next;
                }
                ListNode head2 = curr.next;
                curr.next = null;
                curr = head2;
                for (int i = 1; i < step && curr.next != null; i++) {
                    curr = curr.next;
                }
                ListNode next = null;
                if (curr != null) {
                    next = curr.next;
                    curr.next = null;
                }
                prev.next = mergeListNode(head1, head2);
                while (prev.next != null) {
                    prev = prev.next;
                }
                curr = next;
            }
        }
        return dummyHead.next;
    }

    public ListNode mergeListNode(ListNode left, ListNode right) {
        ListNode sort = null, sortEnd = null;
        while (left != null && right != null) {
            if (right.val < left.val) {
                if (sort == null) {
                    sort = right;
                } else {
                    sortEnd.next = right;
                }
                sortEnd = right;
                right = right.next;
            } else {
                if (sort == null) {
                    sort = left;
                } else {
                    sortEnd.next = left;
                }
                sortEnd = left;
                left = left.next;
            }
        }
        if (left == null) {//当left为空 直接将right 拼接到最后
            sortEnd.next = right;
        } else if (right == null) {
            sortEnd.next = left;
        }
        return sort;
    }

    //fixme 快速排序思路


    public static void testCopyNode() {
        for (int i = 0; i < 500; i++) {
            ListNode listNode = createListNode(10);
            ListNode copy = copyNode(listNode);
            testNodeEquals(listNode, copy);
        }
    }

    private static void testNodeEquals(ListNode listNode, ListNode copy) {
        while (listNode != null) {
            if (listNode != copy && listNode.val == copy.val) {
                listNode = listNode.next;
                copy = copy.next;
            } else {
                printNodeValue(listNode, "listNode");
                printNodeValue(copy, "copy");
                throw new RuntimeException("test failed");
            }
        }
    }


    /**
     * 深度拷贝一个链表
     */
    public static ListNode copyNode(ListNode listNode) {
        ListNode newNode = null, node = null;
        while (listNode != null) {
            if (node == null) {
                node = new ListNode();
                node.val = listNode.val;
                newNode = node;
            } else {
                node.next = new ListNode();
                node.next.val = listNode.val;
                node = node.next;
            }
            listNode = listNode.next;
        }
        return newNode;
    }

    public static class ListNode {
        int val;
        ListNode next;

        ListNode() {
        }

        ListNode(int val) {
            this.val = val;
        }

        ListNode(int val, ListNode next) {
            this.val = val;
            this.next = next;
        }
    }
}
//leetcode submit region end(Prohibit modification and deletion)
