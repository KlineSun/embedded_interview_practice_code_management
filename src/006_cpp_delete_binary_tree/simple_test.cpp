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


class Solution {
    struct TreeNodePro
    {
        TreeNode *parent;
        TreeNode *self;

        TreeNodePro(TreeNode *myself) : parent(nullptr), self(myself) {};
        TreeNodePro(TreeNode *myself, TreeNode *myparent) : parent(myparent), self(myself) {};
    };

public:
    TreeNode* treeRoot;
    TreeNodePro* delProNode;
    TreeNodePro* replaceProNode;

    void repalceProNode(TreeNodePro* node, TreeNodePro* replace) {
        printf("replace %d --> %d\n", replace->self->val, node->self->val);

        // 取出替换的节点
        if (replace->self ==  replace->parent->left) {
            // 替换者是一个左节点，一般是局部最大值，正常没有右节点
            if (replace->self->left)
                // cout << "error: min node have no left" << endl;
                replace->parent->left = replace->self->left;
            else if (replace->self->right)
                replace->parent->left = replace->self->right;
            else {
                // 是叶子节点，直接置为null即可
                replace->parent->left = nullptr;
            }
        } else {
            // 替换者是一个右节点，一般是局部最小值，正常没有左节点
            if (replace->self->left)
                replace->parent->right = replace->self->left;
            else if (replace->self->right)
                // cout << "error: max node have no right" << endl;
                replace->parent->right = replace->self->right;
            else {
                cout << "leaf node" << endl;
                // 是叶子节点，直接置为null即可
                replace->parent->right = nullptr;
            }
        }

        // 链接删除位置的父节点
        if (node->parent) {
            if (node->self == node->parent->left)
                node->parent->left = replace->self;
            else
                node->parent->right = replace->self;
        } else {
            // node是根节点
            cout << "deleted node is root" << endl;
            treeRoot = replace->self;
        }
        // 链接子节点
        replace->self->left = node->self->left;
        replace->self->right = node->self->right;
    }

    void findMax(TreeNode* node, TreeNode* parent, TreeNodePro *result) {

        while (node->right)
        {
            parent = node;
            node = node->right;
        }
        result->parent = parent;
        result->self = node;
    }

    void findMin(TreeNode* node, TreeNode* parent, TreeNodePro *result) {
        while (node->left)
        {
            parent = node;
            node = node->left;
        }
        result->parent = parent;
        result->self = node;
    }

    void preorder(TreeNode* node, TreeNode* parent,int key) {
        if (!node)
            return;
        
        cout << "Enter with node: " << node->val << endl;
        if (key == node->val) {
            cout << "found target node: " << node->val << endl;
            delProNode = new TreeNodePro(node, parent);
            // 找子树下面的能够替换的值
            if (node->left) {
                // 找左节点下的最大值
                findMax(node->left, node, replaceProNode);
                repalceProNode(delProNode, replaceProNode);
            } else if (node->right) {
                // 找右节点下的最小值
                findMin(node->right, node, replaceProNode);
                repalceProNode(delProNode, replaceProNode);
            } else {
                // 叶子节点，直接删除即可
                cout << "Leaf node, no need replace!" << endl;
                if (!parent)
                    treeRoot = nullptr; // 空树
                else if (node == parent->right)
                    parent->right = nullptr;
                else if (node == parent->left)
                    parent->left = nullptr;
            }
            cout << "delete node: " << node->val << endl;
            // delete delProNode.self;
            return;
        }

        if (key < node->val)
            preorder(node->left, node, key);

        if (key > node->val)
            preorder(node->right, node, key);
    }


    TreeNode* deleteNode(TreeNode* root, int key) {
        treeRoot = root;
        preorder(treeRoot, nullptr, key);

        printTree();
        return treeRoot;
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
        if (treeRoot) {
            cout << "[" << treeRoot->val; // 打印括号和根
            printChild(treeRoot);
            cout << "]" << endl;
        } else {
            //空树
            cout << "[ nullptr ]" << endl;
        }

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

    Solution() {
        treeRoot = nullptr;
        delProNode = new TreeNodePro(nullptr, nullptr);
        replaceProNode = new TreeNodePro(nullptr, nullptr);
    }

    virtual ~Solution() {
        delete delProNode;
        delete replaceProNode;
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

    // [2,1]
    // treeRoot = new TreeNode(2, (new TreeNode(1)), nullptr);

    // [0]
    // treeRoot = new TreeNode(0);

    // [5,3,6,2,4,null,7]
    // TreeNode *lchild, *rchild;
    // rchild = new TreeNode(6, nullptr, (new TreeNode(7)));
    // lchild = new TreeNode(3, (new TreeNode(2)), (new TreeNode(4)));
    // treeRoot = new TreeNode(5, lchild, rchild);

    // [5,3,6,2,4,null,7]
    TreeNode *lchild, *rchild, *parent;
    rchild = new TreeNode(8, nullptr, (new TreeNode(9)));
    lchild = new TreeNode(6, (new TreeNode(5)), nullptr);
    parent = new TreeNode(7, lchild, rchild);
    treeRoot = new TreeNode(5, nullptr, parent);

    return treeRoot;
}


/*
练习题：
给定一个二叉搜索树的根节点 root 和一个值 key，删除二叉搜索树中的 key 对应的节点，并保证二叉搜索树的性质不变。返回二叉搜索树（有可能被更新）的根节点的引用。

一般来说，删除节点可分为两个步骤：
首先找到需要删除的节点；
如果找到了，删除它。

解题思路：
    堆排序
*/
int main(int argc, const char **argv)
{
    Solution s;

    s.deleteNode(buildRawTree(), 7);

    return 0;
}