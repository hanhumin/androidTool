package com.txl.leetcode.top100;
//给定一个仅包含 0 和 1 、大小为 rows x cols 的二维二进制矩阵，找出只包含 1 的最大矩形，并返回其面积。
//
//
//
// 示例 1：
//
//
//输入：matrix = [["1","0","1","0","0"],["1","0","1","1","1"],["1","1","1","1","1"]
//,["1","0","0","1","0"]]
//输出：6
//解释：最大矩形如上图所示。
//
//
// 示例 2：
//
//
//输入：matrix = []
//输出：0
//
//
// 示例 3：
//
//
//输入：matrix = [["0"]]
//输出：0
//
//
// 示例 4：
//
//
//输入：matrix = [["1"]]
//输出：1
//
//
// 示例 5：
//
//
//输入：matrix = [["0","0"]]
//输出：0
//
//
//
//
// 提示：
//
//
// rows == matrix.length
// cols == matrix[0].length
// 0 <= row, cols <= 200
// matrix[i][j] 为 '0' 或 '1'
//
// Related Topics 栈 数组 动态规划 矩阵 单调栈
// 👍 960 👎 0


//leetcode submit region begin(Prohibit modification and deletion)
class Solution85 {


    public static void main(String[] args){
        int result = new Solution85().maximalRectangle(new char[][]{{'0','1'},{'0','1'}});
        System.out.println("end "+        +result);
        int result1 = new Solution85().maximalRectangle(new char[][]{{'1','1'}});
        System.out.println("end "+        +result1);
        result1 = new Solution85().maximalRectangle(new char[][]{{'0','1'}});
        System.out.println("end "+        +result1);
    }

    /**
     * 先扩展二维坐标直到扩展不动，在扩展第一个，
     */
    public int maximalRectangle(char[][] matrix) {
        if (matrix == null || matrix.length == 0 || matrix[0].length == 0) {
            return 0;
        }

        int max = 0;
        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix[0].length; j++) {
                if (matrix[i][j] == '1' && max<(matrix.length-i)*(matrix[0].length-j)) {
                    max = Math.max(max, getMax(i, j, matrix.length - 1, matrix[0].length - 1, matrix));
                }
            }
        }
        return max;
    }

    private int getMax(int i, int j, int maxI, int maxJ, char[][] matrix) {

        int startJ = j;
        int max = 1;
        while (startJ < maxJ) {//找到最大的横向距离
            startJ++;
            if (matrix[i][startJ] != '1') {
                maxJ = startJ-1;
                break;
            }
        }
        max = Math.max(maxJ - j+1, max);

        for (int startI = i+1; startI <= maxI; startI++) {
            if (matrix[startI][j] != '1') {
                //第一个就不满足，直接跳出循环
                break;
            }
            for (startJ = j; startJ <= maxJ; startJ++) {
                if (matrix[startI][startJ] != '1') {
                    maxJ = startJ - 1;
                }
                if (startJ >= maxJ) {
                    max = Math.max((maxJ - j + 1) * (startI - i + 1), max);
                }
            }
        }

        return max;
    }


    /**
     * 时间最少的题解：
     * */
    class Solution {
        public int maximalRectangle(char[][] matrix) {
            if (matrix == null || matrix.length == 0 || matrix[0].length == 0) return 0;
            int m = matrix.length;
            int n = matrix[0].length;
            int[] dp = new int[n + 1];
            int res = 0;
            for (int row = 0; row < m; row++) {
                updateHeight(dp, row, matrix);
                res = Math.max(res, maxArea(dp));
            }
            return res;
        }

        private void updateHeight(int[] dp, int row, char[][] matrix) {
            for (int column = 0; column < matrix[row].length; column++) {
                dp[column] = matrix[row][column] == '1' ? dp[column] + 1 : 0;
            }
        }

        private int maxArea(int[] heights) {
            int l = 0, r = 0;
            int n = heights.length;
            int[] pos = new int[n + 1];
            pos[l] = -1;
            int res = 0;
            while (r < n) {
                while (l > 0 && heights[r] < heights[pos[l]]) {
                    int h = heights[pos[l--]];
                    res = Math.max(res, h * (r- pos[l] - 1));
                }
                pos[++l] = r++;
            }
            return res;
        }
    }
}
//leetcode submit region end(Prohibit modification and deletion)
