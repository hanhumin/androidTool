package com.txl.leetcode.top100;
//给定一个二叉树, 找到该树中两个指定节点的最近公共祖先。
//
// 百度百科中最近公共祖先的定义为：“对于有根树 T 的两个节点 p、q，最近公共祖先表示为一个节点 x，满足 x 是 p、q 的祖先且 x 的深度尽可能大（
//一个节点也可以是它自己的祖先）。”
//
//
//
// 示例 1：
//
//
//输入：root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 1
//输出：3
//解释：节点 5 和节点 1 的最近公共祖先是节点 3 。
//
//
// 示例 2：
//
//
//输入：root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 4
//输出：5
//解释：节点 5 和节点 4 的最近公共祖先是节点 5 。因为根据定义最近公共祖先节点可以为节点本身。
//
//
// 示例 3：
//
//
//输入：root = [1,2], p = 1, q = 2
//输出：1
//
//
//
//
// 提示：
//
//
// 树中节点数目在范围 [2, 105] 内。
// -109 <= Node.val <= 109
// 所有 Node.val 互不相同 。
// p != q
// p 和 q 均存在于给定的二叉树中。
//
// Related Topics 树 深度优先搜索 二叉树
// 👍 1220 👎 0


//leetcode submit region begin(Prohibit modification and deletion)

import java.util.LinkedList;
import java.util.Queue;
import java.util.Stack;

/**
 * Definition for a binary tree node.
 * public class TreeNode {
 * int val;
 * TreeNode left;
 * TreeNode right;
 * TreeNode(int x) { val = x; }
 * }
 */
class Solution236 {

    public static void main(String[] args){
        TreeNode root = new TreeNode(3);
        TreeNode treeNode5 = new TreeNode(5);
        TreeNode treeNode1 = new TreeNode(1);
        root.left = treeNode5;
        root.right = treeNode1;
        TreeNode treeNode6 = new TreeNode(6);
        TreeNode treeNode2 = new TreeNode(2);
        treeNode5.left = treeNode6;
        treeNode5.right = treeNode2;
        TreeNode treeNode0 = new TreeNode(0);
        TreeNode treeNode8 = new TreeNode(8);
        treeNode1.left = treeNode0;
        treeNode1.right = treeNode8;
        TreeNode treeNode7 = new TreeNode(7);
        TreeNode treeNode4 = new TreeNode(4);
        treeNode2.left = treeNode7;
        treeNode2.right = treeNode4;

//        TreeNode treeNode = new Solution236().lowestCommonAncestor(root,new TreeNode(5),new TreeNode(1));
        TreeNode treeNode = new Solution236().lowestCommonAncestor(root,new TreeNode(6),new TreeNode(0));
        System.out.println("ans "+treeNode.val);

        treeNode1 = new TreeNode(1);
        treeNode2 = new TreeNode(2);
        TreeNode treeNode3 = new TreeNode(3);
        treeNode4 = new TreeNode(4);
        treeNode1.left = treeNode2;
        treeNode1.right = treeNode3;
        treeNode2.right = treeNode4;
         treeNode = new Solution236().lowestCommonAncestor(treeNode1,new TreeNode(4),new TreeNode(3));
        System.out.println("ans2 "+treeNode.val);

    }

    /**
     * 对二叉树进行前序遍历
     * */
    public TreeNode lowestCommonAncestor(TreeNode root, TreeNode p, TreeNode q) {
        if(root == null || root.val == p.val || root.val == q.val){
            return root;
        }
        Stack<TreeNode> stack = new Stack<>();
        TreeNode ans = null;
        while (root != null || !stack.isEmpty()){
            if(root == null){
                root = stack.pop();
                if(ans != null && ((root.left != null && root.left.val == ans.val) || (root.right != null && root.right.val == ans.val))){
                    ans = root;
                }
                root = root.right;
            }
            if(root.val == p.val || root.val == q.val){
                if(ans == null){
                    ans = root;
                }else {
                    //找到了第二个
                    break;
                }
            }
            if(root.right != null){
                stack.add(root);
            }
            root = root.left;
        }

        return ans;
    }



    public static class TreeNode {
        int val;
        TreeNode left;
        TreeNode right;

        TreeNode(int x) {
            val = x;
        }
    }
}
//leetcode submit region end(Prohibit modification and deletion)
