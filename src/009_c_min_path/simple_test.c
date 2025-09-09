#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

// static const unsigned int grid[][3] = {{1,3,1}, {1,5,1}, {4,2,1}};
static const unsigned int grid[][3] = {{1,2,3}, {4,5,6}};

void path_sum(int m, int n, int val)
{
    printf("grid[%d][%d]\n", m, n);
    if (m < ARRAY_LEN(grid)-1)
        path_sum(m+1, n, grid[m][n] + val);

    if (n < ARRAY_LEN(grid[0])-1)
        path_sum(m, n+1, grid[m][n] + val);

    if (m == ARRAY_LEN(grid)-1 && n == ARRAY_LEN(grid[0])-1)
        printf("path sum: %d\n", grid[m][n] + val);
}

/*
练习题：
给定一个包含非负整数的 m x n 网格 grid ，请找出一条机器人从左上角到右下角的路径，使得路径上的数字总和为最小。
说明：一个机器人每次只能向下或者向右移动一步。

*/
int main(int argc, const char **argv)
{
    path_sum(0, 0, 0);
    return 0;
}