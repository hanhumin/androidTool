package com.txl.leetcode.top100;
//给定一个 m x n 二维字符网格 board 和一个字符串单词 word 。如果 word 存在于网格中，返回 true ；否则，返回 false 。
//
// 单词必须按照字母顺序，通过相邻的单元格内的字母构成，其中“相邻”单元格是那些水平相邻或垂直相邻的单元格。同一个单元格内的字母不允许被重复使用。
//
//
//
// 示例 1：
//
//
//输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "AB
//CCED"
//输出：true
//
//
// 示例 2：
//
//
//输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "SE
//E"
//输出：true
//
//
// 示例 3：
//
//
//输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "AB
//CB"
//输出：false
//
//
//
//
// 提示：
//
//
// m == board.length
// n = board[i].length
// 1 <= m, n <= 6
// 1 <= word.length <= 15
// board 和 word 仅由大小写英文字母组成
//
//
//
//
// 进阶：你可以使用搜索剪枝的技术来优化解决方案，使其在 board 更大的情况下可以更快解决问题？
// Related Topics 数组 回溯 矩阵
// 👍 952 👎 0


//leetcode submit region begin(Prohibit modification and deletion)
class Solution79 {

    public static void main(String[] args) {
        boolean result = new Solution79().exist(new char[][]{{'A', 'B', 'C', 'E'}, {'S', 'F', 'C', 'S'}, {'A', 'D', 'E', 'E'}}, "SEE");
        System.out.println("result : " + result);
    }

    /**
     * 遍历整个数组，当数组中的字符与字符串中的相同时说明相等，它的下一个在此数组有上下左右四种选择，
     * 通过维护当前访问过的元素数组，来判断，如果某个路径不存在重复访问，且字符串到了最后。那么说明查找到了
     *
     *
     * 优化， 假设字符串是 AAAAC   数组  AAAAA  SSSSC 针对前4个就有重复的查找，怎么将这个重复计算优化掉呢？
     */

    public boolean exist(char[][] board, String word) {
        //记录某个节点是否被访问
        boolean[][] visit = new boolean[board.length][board[0].length];
        for (int m = 0; m < board.length; m++) {
            for (int n = 0; n < board[0].length; n++) {
                if (check(visit, 0, board, m, n, word)) {
                    return true;
                }
            }
        }
        return false;
    }

    private boolean check(boolean[][] visit, int position, char[][] board, int m, int n, String word) {
        if (position == word.length()) {
            return true;
        }
        if (m >= board.length || m < 0 || n >= board[0].length || n < 0 || word.charAt(position) != board[m][n]) {
            return false;
        }
        if (visit[m][n]) {//原来访问过这个节点
            return false;
        }
        if (word.charAt(position) == board[m][n]) {//当前位置匹配
            visit[m][n] = true;
            //有四个方向来进行查找
            if (check(visit, position + 1, board, m, n + 1, word)
                    || check(visit, position + 1, board, m + 1, n, word)
                    || check(visit, position + 1, board, m - 1, n, word)
                    || check(visit, position + 1, board, m, n - 1, word)) {
                return true;
            }
            visit[m][n] = false;
            return false;
        }
        return false;
    }
}
//leetcode submit region end(Prohibit modification and deletion)
