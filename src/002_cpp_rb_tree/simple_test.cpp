#include <iostream>
#include <stdio.h>

using namespace std;


enum node_color {
    RED,
    BLACK
};

template<typename T>
struct TreeNode {
    T val;
    node_color color;
    TreeNode *parent, *left, *right;

    TreeNode(T val, node_color color, TreeNode *p, TreeNode *l, TreeNode *r) :
        val(val), color(color), parent(p), left(l), right(r) {};

    TreeNode(T val) :
        val(val), color(RED), parent(nullptr), left(nullptr), right(nullptr) {};
};

template<typename T>
class RBTree {
private:

public:
    TreeNode<T> *root;
    // TreeNode<T> *nil;
    int nodeCnt;
    // 左旋：成为右孩子的左节点
    void leftRotate(TreeNode<T> *node) {
        TreeNode<T> *r_child;
        if (!node || !node->right) {
            cout << "Invalid node or no right_children" << endl;
            return;
        }

        r_child = node->right;
        // 移动右孩子的左节点，腾空间
        if (r_child->left) {
            node->right = r_child->left;
            r_child->left->parent = node;
        } else {
            node->right = nullptr;
        }
        // 移动右孩子
        if (node->parent) {
            r_child->parent = node->parent;
            if (node == node->parent->left)
                node->parent->left = r_child;
            else
                node->parent->right = r_child;
        }
        // 移动node
        r_child->left = node;
        node->parent = r_child;
    }

    // 右旋：成为左孩子的右节点
    void rightRotate(TreeNode<T> *node) {
        TreeNode<T> *l_child;
        if (!node || !node->left) {
            cout << "Invalid node or no right_children" << endl;
            return;
        }

        l_child = node->left;
        // 移动左孩子的右节点，腾位置
        if (l_child->right) {
            node->left = l_child->right;
            l_child->right->parent = node;
        } else {
            node->left = nullptr;
        }

        // 移动左孩子
        if (node->parent) {
            l_child->parent = node->parent;
            if (node == node->parent->left)
                node->parent->left = l_child;
            else
                node->parent->right = l_child;
        }

        // 移动node
        node->parent = l_child;
        l_child->right = node;
    }

    // 插入修复
    void insertFixup(TreeNode<T> *curNode) {
        if (!curNode->parent || !curNode->parent->parent) {
            cout << "deep must greater than 2" << endl;
            return;
        }

        while (curNode->parent->color == RED) { // 新节点插入时都为RED
            if (curNode->parent == curNode->parent->parent->left) { // parent是一个左节点
                TreeNode<T> *rUncle = curNode->parent->parent->right;
                if (rUncle->color == RED) {
                    // 上一层都为红，层间隔染色：红-黑-红
                    curNode->parent->color = BLACK;
                    rUncle->color = BLACK;
                    curNode->parent->parent->color = RED;
                    curNode = curNode->parent->parent;
                } else { // 父亲为红，右叔为黑
                    /**
                     * 父亲为左，子为右，结构存在折线，调整结构为直线
                     * 
                     *          爷                  爷
                     *        /    \    左旋       /    \
                     *      父R    叔B  --->     子R     叔B
                     *         \                /
                     *          子R             父R
                     */
                    if (curNode == curNode->parent->right) { // 且当前节点为一个右节点，表明当前节点大于父亲小于右叔
                        curNode = curNode->parent;
                        leftRotate(curNode); // 重新排布，把父节点下移一层，当前节点上移
                    }
                    /**
                     * 1、第二层颜色不一致，调整颜色为红-黑-红
                     * 2、
                     *         爷                  爷R                   子B
                     *       /    \              /    \		  爷右旋     /  \
                     *      子R     叔B  -->     子B     叔B   -->     父R   爷R
                     *     /                   /                             \
                     *     父R                 父R                            叔B
					 *
                     */
                    curNode->parent->color = BLACK; // 上一层置为黑
                    curNode->parent->parent->color = RED; // 上上层置为红
                    rightRotate(curNode->parent->parent); // 右旋
                }
            } else { // parent是一个右节点
                TreeNode<T> *lUncle = curNode->parent->parent->left;
                if (lUncle->color == RED) {
                    // 上一层都为红，层间隔染色：红-黑-红
                    curNode->parent->color = BLACK;
                    lUncle->color = BLACK;
                    curNode->parent->parent->color = RED;
                    curNode = curNode->parent->parent;
                } else {
                    /**
                     * 父亲为右，子为左，结构存在折线，调整结构为直线
                     * 
                     *          爷                  爷
                     *        /    \    右旋       /   \
                     *      叔B    父R  --->     叔B     子R
                     *           /                       \
                     *          子R                       父R
                     */
                    if (curNode == curNode->parent->left) { // 左叔 < 当前节点 < 父节点
                        curNode = curNode->parent;
                        rightRotate(curNode);
                    }
                    /**
                     * 1、第二层颜色不一致，调整颜色为红-黑-红
                     * 2、
					 *
                     *      爷               爷R                  子B
                     *     /  \      染色    /  \      爷左旋    /  \
                     *   叔B   子R   -->   叔B   子B    -->     爷R  父R
                     *          \                 \            /
                     *           父R              父R         叔B
					 * 
                     */
                    curNode->parent->color = BLACK; // 重新染色
                    curNode->parent->parent->color = RED;
                    leftRotate(curNode->parent->parent); // 左旋
                }
            }

            if (!curNode->parent || !curNode->parent->parent) {
                cout << "Up to the top!" << endl;
                break;
            }
        }
        root->color = BLACK;
    }

    // 删除修复
    void deleteFixup(TreeNode<T> *x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) { // 左节点
                TreeNode<T> *bro = x->parent->right; // 兄弟节点
                if (bro->color == RED) { // 颜色不一致，兄弟染黑，父亲染红
                    bro->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    bro = x->parent->right;
                }
                if (bro->left->color == BLACK && bro->right->color == BLACK) { // 兄弟的孩子为黑，兄弟染红
                    bro->color = RED;
                    x = x->parent;
                } else {
                    if (bro->right->color == BLACK) {
                        bro->left->color = BLACK;
                        bro->color = RED;
                        rightRotate(bro);
                        bro = x->parent->right;
                    }
                    bro->color = x->parent->color;
                    x->parent->color = BLACK;
                    bro->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                TreeNode<T> *bro = x->parent->left;
                if (bro->color == RED) {
                    bro->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    bro = x->parent->left;
                }
                if (bro->right->color == BLACK && bro->left->color == BLACK) {
                    bro->color = RED;
                    x = x->parent;
                } else {
                    if (bro->left->color == BLACK) {
                        bro->right->color = BLACK;
                        bro->color = RED;
                        leftRotate(bro);
                        bro = x->parent->left;
                    }
                    bro->color = x->parent->color;
                    x->parent->color = BLACK;
                    bro->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void insert(T val) {
        TreeNode<T> *new_node = new TreeNode<T>(val);
        TreeNode<T> *cmp = nullptr, *p = nullptr;
        if (!new_node) {
            cout << "Construct node failed!" << endl;
            return;
        }

        cout << "Insert a new node!" << endl;
        if (!root) {
            cout << "Construct root node!" << endl;
            root = new_node;
            root->color = BLACK; // 根节点必须为黑色
            return;
        }
        cmp = root;
        // 找到适合放置该value的空位
        do {
            p = cmp;
            if (val < cmp->val)
                cmp = cmp->left;
            else
                cmp = cmp->right;
        } while (cmp);

        new_node->parent = p;
        if (val < p->val)
            p->left = new_node;
        else
            p->right = new_node;

        insertFixup(new_node);
    }

    TreeNode<T> *find_min(TreeNode<T> *tree_head) {
        TreeNode<T> *tmp = nullptr, *min = nullptr;
        if (!tree_head) {
            cout << "tree is empty!" << endl;
            return nullptr;
        }

        tmp = tree_head;
        do {
            min = tmp;
            tmp = tmp->left;
        } while (tmp);

        return min;
    }

    TreeNode<T> *find_max(TreeNode<T> *tree_head) {
        TreeNode<T> *tmp = nullptr, *max = nullptr;
        if (!tree_head) {
            cout << "tree is empty!" << endl;
            return nullptr;
        }

        tmp = tree_head;
        do {
            max = tmp;
            tmp = tmp->right;
        } while (tmp);

        return max;
    }

    // 删除一个节点
    void remove(T val) {
        TreeNode<T> *tmp = nullptr, *p = nullptr, *subs = nullptr;

        if (!root) {
            cout << "tree is empty!" << endl;
            return;
        }
        tmp = root;

        do {
            if (tmp->val == val) {
                // 找最接近的两个值进行替换
                if (tmp->left) {
                    subs = find_max(tmp->left); // 左子树的最大值
                    subs->parent->right = nullptr; // 取走替换的元素
                    // 链接parent
                    subs->parent = tmp->parent;
                    if (tmp != root)
                        tmp->parent->left = subs;
                    // 链接右节点
                    if (tmp->right) {
                        tmp->right->parent = subs;
                        subs->right = tmp->right;
                    }
                    // 链接左节点
                    if (subs != tmp->left) {
                        subs->left = tmp->left;
                        tmp->left->parent = subs;
                    }
                } else if (tmp->right) {
                    subs = find_min(tmp->right); // 右子树的最小值

                    subs->parent->left = nullptr; // 取走替换的元素
                    // 链接parent
                    subs->parent = tmp->parent;
                    if (tmp != root)
                        tmp->parent->right = subs;
                    // 链接左节点
                    if (tmp->left) {
                        tmp->left->parent = subs;
                        subs->left = tmp->left;
                    }
                    // 链接右节点
                    if (subs != tmp->right) {
                        subs->right = tmp->right;
                        tmp->right->parent = subs;
                    }
                } else {
                    // 删除的节点是叶子，无需替换
                }

                // 删除修复
                deleteFixup(subs);
                delete tmp;
            } else if (tmp->val > val) {
                tmp = tmp->left;
            } else {
                tmp = tmp->right;
            }
        } while(tmp);
    }

    void printNode(TreeNode<T> *node, int deep) {
        std::string indent;
        if (!node) return;
        
        indent.reserve(deep * 2);
        for (int i = 0; i < deep; i++)
            indent += "  ";

        printNode(node->right, deep+1);
        cout << indent << node->val << (node->color==RED?"R":"B")  << endl;
        printNode(node->left, deep+1);
    }

    void printTree() {
        // 中序遍历法，横向打印
        TreeNode<T> *tmp = nullptr;
        printNode(root, 0);

        tmp = root->left;
        if (!tmp) {
            cout << "Invalid left root child!" << endl;
            return;
        }
        cout << "  " << tmp->left->val << endl;
        cout << tmp->val << endl;
        cout << "  " << tmp->right->val << endl;
    }

    // 后续遍历法
    TreeNode<T> *find_leaf(TreeNode<T> *node) {
        if (!node) {
            // cout << "node is null!" << endl;
            return nullptr;
        }

        find_leaf(node->left);
        find_leaf(node->right);

        // 无子节点的叶子，删除当前节点
        cout << "delete node" << node->val << endl;
        delete node;
        return nullptr;
    }

    void destroyTree() {
        root = find_leaf(root);
    }

    RBTree() : root(nullptr), nodeCnt(0) {};
    virtual ~RBTree() {
        if (root)
            destroyTree();
    }
};


/*
练习题：
    红黑树
    定义：
        红黑树(Red-Black Tree)又称RB Tree,它是一个二叉查找树(二叉搜索树)?,
        每个节点包含一个存储为来表示节点的颜色,节点的颜色可以是红色或者黑色;
    性质：
        1.每个节点的颜色不是红色就是黑色
        2.根节点的颜色是黑色
        3.叶子节点(NULL)的颜色是黑色
        4.每个红色节点,它的子节点必须是黑色
        5.一个节点到该节点所有子孙节点的路径包含相同的黑色节点数,该性质保证最大路径长度不会超过最小路径长度的2倍,
          从而保证红黑树是一颗近似平衡二叉树.

左旋：成为右孩子的左节点
右旋：成为左孩子的右节点
*/
int main(int argc, const char **argv)
{
    RBTree<int> tree;

    tree.insert(35); // 根节点，固定之后就不能移动了
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(60);

    tree.printTree();
    return 0;
}