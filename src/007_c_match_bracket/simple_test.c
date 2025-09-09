#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct sign_stack {
    char *bottom;
    char *top;
    unsigned int len;
};

static struct sign_stack g_sign_stack;

static void stack_push(char sign)
{

    if (!g_sign_stack.top) {
        printf("Pls init first!\n");
        return;
    }

    if (g_sign_stack.top == (g_sign_stack.bottom + g_sign_stack.len - 1)) {
        printf("Stack full!\n");
        return;
    }

    *g_sign_stack.top = sign;
    printf("push sign: %c\n", *g_sign_stack.top);
    g_sign_stack.top++;
}

static char stack_pop(void)
{
    char sign;
    if (!g_sign_stack.top) {
        printf("Pls init first!\n");
        return '\0';
    }

    if (g_sign_stack.top == g_sign_stack.bottom) {
        printf("Empty stack!\n");
        return '\0';
    }

    g_sign_stack.top--;
    sign = *g_sign_stack.top;
    *g_sign_stack.top = 0;
    printf("pop sign: %c\n", sign);
    return sign;
}


static int stack_init(unsigned int len)
{
    g_sign_stack.bottom = malloc(len);
    if (!g_sign_stack.bottom) {
        printf("Alloc stack memory failed!\n");
        return -1;
    }
    g_sign_stack.len = len;
    memset(g_sign_stack.bottom, 0, len);
    g_sign_stack.top = g_sign_stack.bottom;
    return 0;
}

static void stack_destroy(void)
{
    if (g_sign_stack.bottom)
        free(g_sign_stack.bottom);
}

/*
练习题：
给定一个只包括 '(', ')', '{', '}', '\[', '\]' 的字符串 s ，判断字符串是否有效。

有效字符串需满足：
    左括号必须用相同类型的右括号闭合。
    左括号必须以正确的顺序闭合。
    每个右括号都有一个对应的相同类型的左括号。

解题思路:
    - 创建一个栈，遇到左括号就入栈对应的右括号
    - 遇到右括号就出栈对比
*/
int main(int argc, const char **argv)
{
    int err = 0, i= 0;
    char ch;
    // 通过命令行输入测试的字符串
    if (argc < 2) {
        printf("Pls input test string!\n");
        printf("Usage: ./simple_test <test_string>\n");
        return -1;
    }

    if (stack_init(strlen(argv[1]))) {
        printf("Init stack failed!\n");
        return -1;
    }

    while (1) {
        ch = argv[1][i++];
        if (ch == '\0')
            break;

        if (ch == '(')
            stack_push(')');
        else if (ch == '{') 
            stack_push('}');
        else if (ch == '[') 
            stack_push(']');
        else if (ch == ')') {
            if (ch != stack_pop()) {
                printf("false\n");
                err = -1;
                goto res_free;
            }
        } else if (ch == '}') {
            if (ch != stack_pop()) {
                printf("false\n");
                err = -1;
                goto res_free;
            }
        } else if (ch == ']') {
            if (ch != stack_pop()) {
                printf("false\n");
                err = -1;
                goto res_free;
            }
        } else {
            printf("Unsupport char: %d\n", ch);
            printf("false\n");
            goto res_free;
        }
    }
    if (g_sign_stack.top == g_sign_stack.bottom) // 已全部出栈
        printf("true\n");
    else
        printf("false\n");

res_free:
    stack_destroy();
    return err;
}