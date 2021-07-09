package com.txl.leetcode.top100;

//给定 n 个非负整数，用来表示柱状图中各个柱子的高度。每个柱子彼此相邻，且宽度为 1 。
//
// 求在该柱状图中，能够勾勒出来的矩形的最大面积。
//
//
//
//
//
// 以上是柱状图的示例，其中每个柱子的宽度为 1，给定的高度为 [2,1,5,6,2,3]。
//
//
//
//
//
// 图中阴影部分为所能勾勒出的最大矩形面积，其面积为 10 个单位。
//
//
//
// 示例:
//
// 输入: [2,1,5,6,2,3]
//输出: 10
// Related Topics 栈 数组 单调栈
// 👍 1425 👎 0


import java.util.LinkedList;
import java.util.Stack;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution84 {
    public int largestRectangleArea(int[] heights) {
        if(heights == null || heights.length == 0){
            return 0;
        }
        int[] left = new int[heights.length];
        int[] right = new int[heights.length];
        int max = heights[0];
        //维护一个从小到大的栈
        Stack<Integer> stack = new Stack<>();
        for (int i = 0; i < heights.length; ++i) {
            while (!stack.isEmpty() && heights[stack.peek()] >= heights[i]) {
                stack.pop();
            }
            left[i] = (stack.isEmpty() ? -1 : stack.peek());
            stack.push(i);
        }
        stack.clear();
        for (int i = heights.length-1; i >= 0; i--) {
            while (!stack.isEmpty() && heights[stack.peek()] >= heights[i]) {
                stack.pop();
            }
            right[i] = (stack.isEmpty() ? heights.length : stack.peek());
            stack.push(i);
        }
        for (int i=0;i<heights.length;i++){
            max = Math.max(max,heights[i]*(right[i]-left[i]-1));
        }
        return max;
    }

    public int largestRectangleArea1(int[] heights) {
        int N = heights.length, res = 0, i;
        int[] leftBounds = new int[N + 1], rightBounds = new int[N];
        for (i = 1; i < N; i++) {
            int h = heights[i], l = i - 1;
            while (l >= 0 && h <= heights[l]) {
                if (h < heights[l]) {
                    rightBounds[l] = i - 1;
                    res = Math.max(res, (rightBounds[l] - leftBounds[l] + 1) * heights[l]);
                }
                l = leftBounds[l] - 1;
            }
            leftBounds[i] = l + 1;
        }
        i = N;
        while (i > 0) {
            if (heights[i - 1] > 0) {
                rightBounds[i - 1] = N - 1;
                res = Math.max(res, (rightBounds[i - 1] - leftBounds[i - 1] + 1) * heights[i - 1]);
            }
            i = leftBounds[i - 1];
        }
        return res;
    }
}
//leetcode submit region end(Prohibit modification and deletion)

