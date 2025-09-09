#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    int maxElemVal;
    vector<TreeNode*> solutionsVec; // 保存每种解决方案的root节点
    int curRootVal; // 当前解决方案根节点的索引

    // subRoot是上一层选定的根节点，start是该根节点孩子的范围，该根节点不在该范围内
    void linkChild(TreeNode* head, TreeNode* curRoot, int start, int end) {
        TreeNode *child = nullptr, *subRoot = nullptr;
        if (end <= start || !curRoot || start < 1) {
            return; // 单个元素，没有子节点关系
        }

        // 链接左区间
        for (int i = start; i < curRoot->val; i++) {
            if (i == curRoot->val)
                continue;

            // 左区间内的每一个元素轮流当根；
            subRoot = head + (i-1);
            curRoot->left = subRoot;
            linkChild(head, subRoot, start, curRoot->val-1);
        }
        
        for (int i = start; i <= end; i++) {
            if (i == curRoot->val)
                continue;

            child = head + (i-1);
            if (child->val < curRoot->val) {
                printf("link %d left --> %d", curRoot->val, child->val);
                curRoot->left = child;
            } else {
                printf("link %d right --> %d", curRoot->val, child->val);
                curRoot->right = child;
            }
                
            // 再次重新链接新分割出来的两个节点，子节点作为下一层的根
            linkChild(head, child, start, i-1); // 左子树
            linkChild(head, child, i+1, end); // 右子树
        }
    }

    TreeNode* alloc_one_solution(int root, int n, TreeNode* baseArray) {
        // vector<TreeNode*> oneSolution; //一个解决方案的向量表，节点间的内在联系尚未建立
        TreeNode* newArray = nullptr;
        if (root < 1 || n < 1 || root > n) {
            cout << "Invalid parameter in alloc_one_solution."  << endl;
            return nullptr;
        }

        // 分配为连续的内存
        newArray = new TreeNode[n];
        for (int i = 1; i <= n; i++) {
            newArray[i - 1] = i;
            // 如果提供了base，深拷贝base模板的节点间关系
            if (baseArray) {
                int idx = 0;
                if (baseArray[i-1].left) {
                    idx =  baseArray[i-1].left->val - 1; // 坐标等于val -1
                    newArray[i-1].left = &newArray[idx];
                    cout << "Point to left: " << newArray[i-1].left->val  << endl;
                }
                if (baseArray[i-1].right) {
                    idx =  baseArray[i-1].right->val - 1; // 坐标等于val -1
                    newArray[i-1].right = &newArray[idx];
                    cout << "Point to right: " << newArray[i-1].right->val  << endl;
                }
            }
        }

        // 根节点存入向量表
        solutionsVec.emplace_back(&newArray[root-1]);
        // cout << "alloc_one_solution() Current root: " << newArray[root-1].val << endl;
        return &newArray[0];
    }

    vector<TreeNode*> generateTrees(int n) {
        TreeNode *head = nullptr;
        if (n < 1) {
            cout << "Number of tree node must greater than 1: " << n << endl;
            return solutionsVec;
        }

        maxElemVal = n;
        for (curRootVal = 1; curRootVal <= n; curRootVal++) {
            head = alloc_one_solution(curRootVal, n, nullptr);
            linkChild(head, head+(curRootVal-1), 1, n);
        }

        cout << "Generate trees, count of solutions: " << solutionsVec.size() << endl;
        return solutionsVec;
    }

    // void printTree(TreeNode *node, int deep) {
    //     // 前序遍历
    //     if (!node) {
    //         return;
    //     }

    //     if (deep > 0)
    //         cout << ", ";
    //     cout << node->val;

    //     if (!node->left && !node->right) // 叶子节点不继续遍历
    //         return;

    //     // 遍历左节点
    //     if (node->left)
    //         printTree(node->left, deep+1);
    //     else
    //         cout << ", null"; // 有右分支无左分支
    //     // 遍历右节点
    //     if (node->right)
    //         printTree(node->right, deep+1);
    //     else
    //         cout << ", null"; // 有左分支无右分支
    // }

    void printChild(TreeNode *node) {
        if (!node)
            return;

        if (node->left)
            cout << ", " << node->left->val;
        else
            cout << ", null";

        if (node->right)
            cout << ", " << node->right->val;
        else
            cout << ", null";

        if (!node->left && !node->right)
            return;

        printChild(node->left);
        printChild(node->right);
    }

    void printSolutions() {
        int numLoop = solutionsVec.size(), i = 0;

        cout << "[";
        for (i = 0; i < numLoop; i++) {// 每次打印一个solution
            if (i > 0)
                cout << ", ";
            cout << "[" << solutionsVec[i]->val; // 先
            printChild(solutionsVec[i]);
            cout << "]";
        }
        cout << "]" << endl;
    }
};


/*
练习题：
    给你一个整数 n ，请你生成并返回所有由 n 个节点组成且节点值从 1 到 n 互不相同的不同 二叉搜索树 。可以按 任意顺序 返回答案。

解题思路：
    1、在[1, n]这个区间内，分别选定不同的根节点，例如x，划定为两个区间[1, x]和[x+1, n]
    2、然后在子区间再次分别选定一个值作为左子树和右子树的根节点，如xl，xr，将区间再次划定为[start, delim]和[delim, end];
    3、如此递归，直至区间长度start等于end


*/
int main(int argc, const char **argv)
{
    Solution s;
    std::vector<TreeNode*> trees_vec;
    int testNum = 1;

    if (argc > 1) {
        int num = atoi(argv[1]);
        if (num > 0 && num <100)
            testNum = num;
    }
    cout << "test number: " << testNum << endl;

    trees_vec = s.generateTrees(testNum);
    s.printSolutions();

    return 0;
}