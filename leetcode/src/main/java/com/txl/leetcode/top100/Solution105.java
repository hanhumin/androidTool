package com.txl.leetcode.top100;
//给定一棵树的前序遍历 preorder 与中序遍历 inorder。请构造二叉树并返回其根节点。
//
//
//
// 示例 1:
//
//
//Input: preorder = [3,9,20,15,7], inorder = [9,3,15,20,7]
//Output: [3,9,20,null,null,15,7]
//
//
// 示例 2:
//
//
//Input: preorder = [-1], inorder = [-1]
//Output: [-1]
//
//
//
//
// 提示:
//
//
// 1 <= preorder.length <= 3000
// inorder.length == preorder.length
// -3000 <= preorder[i], inorder[i] <= 3000
// preorder 和 inorder 均无重复元素
// inorder 均出现在 preorder
// preorder 保证为二叉树的前序遍历序列
// inorder 保证为二叉树的中序遍历序列
//
// Related Topics 树 数组 哈希表 分治 二叉树
// 👍 1109 👎 0


//leetcode submit region begin(Prohibit modification and deletion)

import java.util.HashMap;

/**
 * Definition for a binary tree node.
 * public class TreeNode {
 * int val;
 * TreeNode left;
 * TreeNode right;
 * TreeNode() {}
 * TreeNode(int val) { this.val = val; }
 * TreeNode(int val, TreeNode left, TreeNode right) {
 * this.val = val;
 * this.left = left;
 * this.right = right;
 * }
 * }
 */
class Solution105 {
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

    /**
     * 递归实现
     */
    public TreeNode buildTree(int[] preorder, int[] inorder) {

        HashMap<Integer, Integer> map = new HashMap<>();
        int l = inorder.length;
        for (int i = 0; i < l; i++) {
            map.put(inorder[i], i);
        }
       return findLeftAndRight(0,l-1,0,l-1,preorder,inorder,map);
    }


    private TreeNode findLeftAndRight(int preStart, int preEnd, int inorderStart, int inorderEnd,int[] preorder, int[] inorder,HashMap<Integer, Integer> map) {
        if(preEnd >= preStart){
            //前序遍历第一个为根节点
            TreeNode root = new TreeNode(preorder[preStart]);
            int index = map.get(root.val);
            int preCount = index-inorderStart;//左节点数目
            root.left = findLeftAndRight(preStart+1,preStart+preCount,inorderStart,index-1,preorder,inorder,map);
            root.right = findLeftAndRight(preStart+1+preCount,preEnd,index+1,inorderEnd,preorder,inorder,map);
            return root;
        }
        return null;
    }


}
//leetcode submit region end(Prohibit modification and deletion)
