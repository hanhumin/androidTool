package com.txl.leetcode.code;

import com.txl.leetcode.ListNode;

public class Solution92 {

    public static void main(String[] args){
        Solution92 solution92 = new Solution92();
        ListNode head = solution92.array2List(new int[]{1,2,3,4,5});
        solution92.printList(head);
        System.out.println("value ==== ===== ");
        solution92.printList(solution92.reverseBetween(head,2,4));
//        head = solution92.array2List(new int[]{3,5});
//        System.out.println("value ==== ===== ");
//        solution92.printList(solution92.reverseBetween(head,1,2));
    }

    private void printList(ListNode head){
        while (head!= null){
            System.out.println("  value : "+head.val);
            head = head.next;
        }
    }

    public ListNode array2List(int[] array){
        ListNode head = null;
        ListNode pre = null;
        for (int i=0; i<array.length;i++){
            ListNode listNode = new ListNode(array[i]);
            if(head == null){
                head = listNode;
            }
            if(pre != null){
                pre.next = listNode;
            }
            pre = listNode;
        }
        return head;
    }

    public ListNode reverseBetween(ListNode head, int left, int right) {
        if(left == right || head == null){
            return head;
        }
        int step = 1;
        ListNode temp = head;
        ListNode startPre = null, endNext = null;
        ListNode start = null, end = null;
        while (temp != null){
            if(left-1 == step){
                startPre = temp;
                start = temp.next;
                end = start;
            }
            if(step > left && step <= right){
                //链表翻转
                ListNode node = temp.next;
                temp.next = start;
                start = temp;
                temp = node;
                if(step == right){
                    endNext = temp.next;
                    break;
                }
            }else {//链表向前遍历
                temp = temp.next;
            }
            step++;
        }
        if(startPre != null){
            startPre.next = start;
        }
        if(endNext != null){
            end.next = endNext;
        }
        return head;
    }

    //交换 left right位置
//    public ListNode reverseBetween(ListNode head, int left, int right) {
//        if (left == right) {
//            return head;
//        }
//        if(head == null){
//            return null;
//        }
//        int leftC = 1;
//        ListNode leftPre = null, rightPre = null;
//        ListNode leftNode = null, rightNode = null;
//        ListNode temp = head;
//        while (temp != null) {
//            if (leftC == left-1) {
//                leftPre = temp;
//                leftNode = temp.next;
//            }
//            if (leftC == right - 1) {
//                rightPre = temp;
//                rightNode = temp.next;
//                break;
//            }
//            leftC++;
//            temp = temp.next;
//        }
//        if(rightPre == null){
//            return head;
//        }
//        if(leftPre == null){//说明要替换的是第一个节点
//            leftNode = head;
//        }
//
//        //有可能自己指向自己
//        if(rightPre != leftNode){//如果相同说明两个节点相邻
//            temp = leftNode.next;
//        }else {
//            temp = leftNode;
//        }
//
//        rightPre.next = leftNode;
//        leftNode.next = rightNode.next;
//        if(leftPre != null){
//            leftPre.next = rightNode;
//        }else {
//            head = rightNode;
//        }
//
//        rightNode.next = temp;
//        return head;
//    }
}
