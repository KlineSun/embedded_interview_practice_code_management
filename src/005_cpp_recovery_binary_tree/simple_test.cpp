#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <limits>

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
    TreeNode* rawTree;
    vector<int> errIdxVec;
    vector<TreeNode*> orderVec;

    void swap(TreeNode* nodeA, TreeNode* nodeB) {
        int val = 0;

        printf("swap order %d <==> %d\n", nodeA->val, nodeB->val);
        val = nodeA->val;
        nodeA->val = nodeB->val;
        nodeB->val = val;
    }

    void verify(TreeNode* node) {
        if (!node)
            return;
        verify(node->left);
        orderVec.emplace_back(node); // 将节点指针保存在有序数组中
        verify(node->right);
    }

    void recoverTree(TreeNode* root) {
        unsigned int i = 0;
        rawTree = root;
        verify(root);

        for (i = 0; i < orderVec.size(); i++) {
            if (errIdxVec.empty()) {
                if (orderVec[i]->val > orderVec[i+1]->val) {
                    printf("Error node[%d]: %d\n", i, orderVec[i]->val);
                    errIdxVec.push_back(i);
                    i++; // 跳过下一个坐标的判断
                }
            } else {
                if (orderVec[i]->val < orderVec[i-1]->val) {
                    printf("Error node[%d]: %d\n", i, orderVec[i]->val);
                    errIdxVec.push_back(i);
                }
            }
        }

        if (errIdxVec.size() == 2) {
            swap(orderVec[errIdxVec[0]], orderVec[errIdxVec[1]]);
        }

        if (errIdxVec.size() == 1) {
            swap(orderVec[errIdxVec[0]], orderVec[errIdxVec[0]+1]);
        }
    }


    void printChild(TreeNode *node) {
        if (!node /*|| (!node->left && !node->right)*/) // 空节点或者叶子节点，不进行打印
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

    void printTree() {
        cout << "[" << rawTree->val; // 打印括号和根
        printChild(rawTree);
        cout << "]" << endl;
    }

    void releaseNode(TreeNode *node) {
        if (!node) // 空节点跳过
            return;

        if (node->left) {
            releaseNode(node->left); // 先释放左节点
            node->left = nullptr;
        }
    
        if (node->right) {
            releaseNode(node->right); // 再释放右节点
            node->right = nullptr;
        }

        // 释放自己
        cout << "release node: " << node->val << endl;
        delete node;
    }

    Solution() : rawTree{nullptr} {};
    virtual ~Solution() {};
};

TreeNode* buildRawTree() {
    TreeNode* rawTree;

    // [1,3,null,null,2]
    // rawTree = new TreeNode(1, (new TreeNode(3)), nullptr);
    // rawTree->left->right = new TreeNode(2);

    // [3,1,4,null,null,2]
    rawTree = new TreeNode(3, (new TreeNode(1)), (new TreeNode(4)));
    rawTree->right->left = new TreeNode(2);

    // TreeNode *lchild, *rchild, *parent;
    // rchild = new TreeNode(60, (new TreeNode(40)), (new TreeNode(70)));
    // lchild = new TreeNode(55);
    // parent = new TreeNode(50, lchild, rchild);
    // rawTree = new TreeNode(30, (new TreeNode(10)), parent);

    return rawTree;
}


/*
练习题：
    给你二叉搜索树的根节点 root ，该树中的 恰好 两个节点的值被错误地交换。请在不改变其结构的情况下，恢复这棵树 。

解题思路：
    堆排序
*/
int main(int argc, const char **argv)
{
    Solution s;

    s.recoverTree(buildRawTree());

    return 0;
}