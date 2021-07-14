package com.txl.leetcode.top100;

//给你一个二叉树，请你返回其按 层序遍历 得到的节点值。 （即逐层地，从左到右访问所有节点）。
//
//
//
// 示例：
//二叉树：[3,9,20,null,null,15,7],
//
//
//    3
//   / \
//  9  20
//    /  \
//   15   7
//
//
// 返回其层序遍历结果：
//
//
//[
//  [3],
//  [9,20],
//  [15,7]
//]
//
// Related Topics 树 广度优先搜索 二叉树
// 👍 926 👎 0


//leetcode submit region begin(Prohibit modification and deletion)

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

/**
 * Definition for a binary tree node.
 * public class TreeNode {
 *     int val;
 *     TreeNode left;
 *     TreeNode right;
 *     TreeNode() {}
 *     TreeNode(int val) { this.val = val; }
 *     TreeNode(int val, TreeNode left, TreeNode right) {
 *         this.val = val;
 *         this.left = left;
 *         this.right = right;
 *     }
 * }
 */
class Solution102 {
    public List<List<Integer>> levelOrder(TreeNode root) {

        Queue<TreeNode> queue = new LinkedList();
        Queue<TreeNode> temp = new LinkedList();

        List<List<Integer>> list = new ArrayList<>();
        if(root == null){
            return list;
        }
        queue.offer(root);
        ArrayList<Integer> integers = new ArrayList<>();
        list.add(integers);
        while (!queue.isEmpty() || !temp.isEmpty()){
            if(queue.isEmpty()){
                queue.addAll(temp);
                temp.clear();
                integers = new ArrayList<>();
                list.add(integers);
            }
            TreeNode treeNode = queue.poll();
            integers.add(treeNode.val);
            if(treeNode.left!=null){
                temp.add(treeNode.left);
            }
            if(treeNode.right != null){
                temp.add(treeNode.right);
            }
        }
        return list;
    }



    private static class TreeNode {
        TreeNode left;
        TreeNode right;
        int val;

        TreeNode() {
        }

        TreeNode(int val) {
            this.val = val;
        }

        TreeNode(int val, TreeNode left, TreeNode right) {
            this.val = val;
            this.left = left;
            this.right = right;
        }
    }
}
//leetcode submit region end(Prohibit modification and deletion)
