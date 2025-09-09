#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

static const unsigned int test_ary[] = {1,2,3,1};
// static const unsigned int test_ary[] = {2, 7, 9, 3, 1, 5, 6};

static int g_max_sum = 0;

void intervel_sum(int start, int end, int sum) {

    if ((start+2) >= end) {
        printf("sum %d\n", sum);
        if (g_max_sum < sum)
            g_max_sum = sum;
        printf("=========================\n");
    }

    if ((start+2) < end) {
        for (int i = start+2; i < end; i++) {
            printf("Add %d\n", test_ary[i]);
            intervel_sum(i, end, test_ary[i]+sum);
            
        }
    }

}

/*
练习题：
你是一个专业的小偷，计划偷窃沿街的房屋。每间房内都藏有一定的现金，影响你偷窃的唯一制约因素
就是相邻的房屋装有相互连通的防盗系统，如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警。

给定一个代表每个房屋存放金额的非负整数数组，计算你 不触动警报装置的情况下 ，一夜之内能够偷窃
到的最高金额。

*/
int main(int argc, const char **argv)
{
    intervel_sum(0, ARRAY_LEN(test_ary), test_ary[0]);

    intervel_sum(1, ARRAY_LEN(test_ary), test_ary[1]);

    printf("Max sum: %d\n", g_max_sum);
    return 0;
}