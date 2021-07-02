package com.txl.leetcode.top100;

//给定一个 n × n 的二维矩阵 matrix 表示一个图像。请你将图像顺时针旋转 90 度。
//
// 你必须在 原地 旋转图像，这意味着你需要直接修改输入的二维矩阵。请不要 使用另一个矩阵来旋转图像。
//
//
//
// 示例 1：
//
//
//输入：matrix = [[1,2,3],[4,5,6],[7,8,9]]
//输出：[[7,4,1],[8,5,2],[9,6,3]]
//
//
// 示例 2：
//
//
//输入：matrix = [[5,1,9,11],[2,4,8,10],[13,3,6,7],[15,14,12,16]]
//输出：[[15,13,2,5],[14,3,4,1],[12,6,8,9],[16,7,10,11]]
//
//
// 示例 3：
//
//
//输入：matrix = [[1]]
//输出：[[1]]
//
//
// 示例 4：
//
//
//输入：matrix = [[1,2],[3,4]]
//输出：[[3,1],[4,2]]
//
//
//
//
// 提示：
//
//
// matrix.length == n
// matrix[i].length == n
// 1 <= n <= 20
// -1000 <= matrix[i][j] <= 1000
//
// Related Topics 数组 数学 矩阵
// 👍 911 👎 0


//leetcode submit region begin(Prohibit modification and deletion)
class Solution48 {

    public static void main(String[] args){
        int[][] matrix = new int[][]{{1,2,3},{4,5,6},{7,8,9}};
        new Solution48().rotate(matrix);
        System.out.println(matrix);
    }

    public void rotate(int[][] matrix) {
        int n = matrix.length;

        for (int i = 0; i < n / 2; i++) {//开启循环
            int step = n - i * 2 -1;//每一层循环，每一步要易懂的位置
            for (int j = 0; j < step; j++) {//此时的step代表需要移动的个数

                //保存最后一列的数据
                int temp = matrix[i+j][i+step];
                // 第一行数据给到最后一列
                matrix[i+j][i+step] = matrix[i][i+j];

                //保存最后一行数据  x 随着step增加而减小
                int temp2 = matrix[i+step][i+step-j];
                //最后一列数据到最后一行
                matrix[i+step][i+step-j] = temp;

                //保存第一列的数据
                temp =  matrix[i+step-j][i];
                matrix[i+step-j][i] = temp2;

                //第一列的数据转移到第一行
                matrix[i][i+j] = temp;
            }
        }
    }
}
//leetcode submit region end(Prohibit modification and deletion)
