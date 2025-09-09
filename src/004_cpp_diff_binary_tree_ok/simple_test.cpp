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

    // subRoot是上一层选定的根节点，start是该根节点孩子的范围，该根节点不在该范围内

    vector<TreeNode*> generateChild(int left, int right) {
        vector<TreeNode*> rootVec;
        if (right < left) {
            rootVec.push_back(nullptr); // 即使不存在该范围，也需要在内部添加一个nullptr方便后续遍历
            return rootVec;
        }

        // 区间内的每一个元素都有可能成为根
        for (int i = left; i <= right; i++) {
            vector<TreeNode*> leftNodes = generateChild(left, i-1); // 构造当前根的左子树
            vector<TreeNode*> rightNodes = generateChild(i+1, right); // 构造当前根的左子树
            
            // 左子树和右子树同样都有多种可能，通过穷举法逐个组合，并链接到一个根节点上
            for (TreeNode* leftNode : leftNodes) {
                for (TreeNode* rightNode : rightNodes) {
                    TreeNode* rootNode = new TreeNode(i);
                    rootNode->left = leftNode;
                    rootNode->right = rightNode;
                    rootVec.push_back(rootNode);
                }
            }
        }
        return rootVec;
    }


    vector<TreeNode*> generateTrees(int n) {
        if (n < 1) {
            cout << "Number of tree node must greater than 1: " << n << endl;
            return solutionsVec;
        }

        maxElemVal = n;
        solutionsVec = generateChild(1, n);
    
        cout << "Generate trees, count of solutions: " << solutionsVec.size() << endl;
        return solutionsVec;
    }


    void printChild(TreeNode *node) {
        if (!node || (!node->left && !node->right)) // 空节点或者叶子节点，不进行打印
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

        cout << "[";
        for (TreeNode* solution: solutionsVec) {
            cout << "[" << solution->val; // 打印括号和根
            printChild(solution);
            cout << "], ";
        }
        cout << "]" << endl;
    }

    void releaseChild(TreeNode *node) {
        if (!node) // 空节点跳过
            return;

        if (node->left) {
            releaseChild(node->left); // 先释放左节点
            node->left = nullptr;
        }
    
        if (node->right) {
            releaseChild(node->right); // 再释放右节点
            node->right = nullptr;
        }

        // 释放自己
        cout << "release node: " << node->val << endl;
        delete node;
    }

    void releaseSolutions() {
        cout << "Enter release!" << endl;
        for (TreeNode* solution: solutionsVec) {
            releaseChild(solution);
        }
    }

    virtual ~Solution() {
        releaseSolutions();
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