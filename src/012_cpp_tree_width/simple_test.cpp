#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>

using namespace std;


struct TreeNode {
    int val;
    bool fake;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : fake(false), val(0), left(nullptr), right(nullptr) {}
    TreeNode(bool fake) : fake(fake), val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : fake(false), val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : fake(false), val(x), left(left), right(right) {}
};


class Solution {

public:
    vector<int> widthVec; // 每层的宽度值列表
    TreeNode* treeRoot;

    void traverse(TreeNode* node, int high) {
        if (widthVec.size() < high) {
            widthVec.emplace_back(new int(0));
        }
        if (node) {
            widthVec[high-1]++;
        }
    }

    int treeWidth(TreeNode* root) {
        treeRoot = root;

        traverse(root, 1); // 根节点为第一层
        sort(widthVec.begin(), widthVec.end(), std::greater<int>());
        return widthVec[0];
    }

    Solution() {
        treeRoot = nullptr;
    }

    virtual ~Solution() {
    }
};

TreeNode* buildRawTree() {
    TreeNode* treeRoot;

    // [1,3,null,null,2]
    // treeRoot = new TreeNode(1, (new TreeNode(3)), nullptr);
    // treeRoot->left->right = new TreeNode(2);

    // [3,1,4,null,null,2]
    // treeRoot = new TreeNode(3, (new TreeNode(1)), (new TreeNode(4)));
    // treeRoot->right->left = new TreeNode(2);

    // TreeNode *lchild, *rchild, *parent;
    // rchild = new TreeNode(60, (new TreeNode(55)), (new TreeNode(70)));
    // lchild = new TreeNode(40);
    // parent = new TreeNode(50, lchild, rchild);
    // treeRoot = new TreeNode(30, (new TreeNode(10)), parent);

    // [5,3,6,2,4,null,7]
    // TreeNode *lchild, *rchild;
    // rchild = new TreeNode(6, nullptr, (new TreeNode(7)));
    // lchild = new TreeNode(3, (new TreeNode(2)), (new TreeNode(4)));
    // treeRoot = new TreeNode(5, lchild, rchild);

    // [5,3,6,2,4,null,7]
    // TreeNode *lchild, *rchild, *parent;
    // rchild = new TreeNode(8, nullptr, (new TreeNode(9)));
    // lchild = new TreeNode(6, (new TreeNode(5)), nullptr);
    // parent = new TreeNode(7, lchild, rchild);
    // treeRoot = new TreeNode(5, nullptr, parent);

    // [1,3,2,5,3,null,9]
    TreeNode *lchild, *rchild;
    rchild = new TreeNode(2, nullptr, (new TreeNode(9)));
    lchild = new TreeNode(3, (new TreeNode(5)), (new TreeNode(3)));
    treeRoot = new TreeNode(1, lchild, rchild);

    return treeRoot;
}


/*
练习题：
给你一棵二叉树的根节点 root ，返回树的 最大宽度 。
树的 最大宽度 是所有层中最大的 宽度 。
每一层的 宽度 被定义为该层最左和最右的非空节点（即，两个端点）之间的长度。将这个二叉树视作与
满二叉树结构相同，两端点间会出现一些延伸到这一层的 null 节点，这些 null 节点也计入长度。
题目数据保证答案将会在 32 位 带符号整数范围内。
*/
int main(int argc, const char **argv)
{
    Solution s;

    s.treeWidth(buildRawTree());

    return 0;
}