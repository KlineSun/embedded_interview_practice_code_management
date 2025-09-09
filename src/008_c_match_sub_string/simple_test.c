#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
练习题：
给定一个字符串 s ，请你找出其中不含有重复字符的 最长子串 的长度。


*/
int main(int argc, const char **argv)
{
    int err = 0, i= 0, j = 0, max = 0;
    char ch;
    char *sub_str = NULL;

    // 通过命令行输入测试的字符串
    if (argc < 2) {
        printf("Pls input test string!\n");
        printf("Usage: ./simple_test <test_string>\n");
        return -1;
    }

    sub_str = malloc(strlen(argv[1]));
    if (!sub_str) {
        printf("Alloc sub string memory failed!\n");
        return -1;
    }

    while (1) {
        ch = argv[1][i++];
        if (ch == '\0')
            break;
        
        if (strstr(sub_str, &ch)) {
            printf("sub string: %s, len=%d\n", sub_str, j);
            max = max < j ? j : max;

            memset(sub_str, 0, strlen(argv[1]));
            j = 0;
        }
        sub_str[j++] = ch;
    }
    printf("%d\n", max);

    free(sub_str);
    return err;
}