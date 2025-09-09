#include <iostream>
#include <stdio.h>

using namespace std;

struct ListNode {
    int val;
    struct ListNode *next;

    ListNode(int val) : val(val), next(nullptr) {};
};

static const int node_cnt = 10;
static const int loop_entry = 4;

// 判断一个链表是否有环
bool judge_list_loop(ListNode *list_head, ListNode **loop_entry)
{
    ListNode *slow, *fast;
    if (list_head == nullptr || loop_entry == nullptr) {
        cout << "Invalid paramter!" << endl;
        return false;
    }

    slow = list_head;
    fast = list_head;

    do {
        // slow走一步，fast走两步
        slow = slow->next;
        fast = fast->next;
        if (fast)
            fast = fast->next;
        else
            return false; // 到了链表末尾，说明无环

        if (slow == fast) break;
    } while (slow && fast);

    slow = list_head;
    do {
        // 各走一步
        slow = slow->next;
        fast = fast->next;

        if (slow == fast) break;
    } while (slow && fast);

    *loop_entry = slow;
    return true;
}

/**
 * 练习题：
 *  判断一个链表是否有环，如果有环，则输出环入口的节点值
 * 
 * 解题方法：
 *  使用快慢指针，慢指针+1，快指针+2，如果链表有环，则最终快慢指针总会相遇
 *  
 * 计算入口：
 *  假设有环，从head到环入口entry的路程为A，entry到相遇点meet的距离为B，meet到entry的距离为C
 *  则有：fast的路程为slow路程的2倍，因为速度是两倍
 *  所以：2*(A+B) = A+B + n*(B+C), 其中n表示快指针可能在环中循环了n次了
 *  然后: A+B = n*(B+C) ==> A = (n-1)(B+C)+C，可以看出，A的距离就是环长度的整数倍再加上C的距离；
 *  最后，如果两个指针都以同样的速度分别从head和meet出发，最终快指针会在环中循环n次+c，最终走到环的入口和慢指针相遇
 * 
 */
int main(int argc, const char **argv)
{
    int i = 1;
    ListNode *head = new ListNode(i++); // head
    ListNode *tmp = head, *entry = head;

    while (i <= node_cnt) {
        tmp->next = new ListNode(i++);
        tmp = tmp->next;
    }

    cout << "Total construct list node: " << i-1 << endl;
    while (entry) {
        if (entry->val == loop_entry) {
            tmp->next = entry; // 末尾节点的next指针指向环入口
            break;
        }
        entry = entry->next;
    }

    tmp = head;
    for (i = 0; i < 2*node_cnt; i++) {
        printf("node[%d]->", tmp->val);
        tmp = tmp->next;
    }
    cout << endl;

    if (judge_list_loop(head, &entry)) {
        cout << "Exist loop,  loop_entry=" << entry->val << endl;
    } else {
        cout << "Not exist loop" << endl;
        return -1;
    }
    return 0;
}