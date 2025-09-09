#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <unordered_map>

using namespace std;


class LruCache {

    struct elementDesc {
        int val;
        int active;

        elementDesc(int val, int active) : val(val), active(active) {};
    };

private:
    unsigned int capacity=3;
    unordered_map<int, elementDesc *> cacheMap; // <key, <val, active>>
    int count;

public:

    LruCache() : capacity(3) {};
    LruCache(int cap) : capacity(cap) {};
    virtual ~LruCache() {};

    int findInactiveKey() {
        int key = -1, active = -1;
        for (auto it : cacheMap) {
            printf("key-val: %d->%d, active=%d\n", it.first, it.second->val, it.second->active);
            if (key < 0) {
                key = it.first;
                active = it.second->active;
            } else if (it.second->active < active) {
                key = it.first;
                active = it.second->active;
            }
            printf("current inactive key: %d, active=%d\n", key, active);
        }
        printf("Inactive key: %d, active=%d\n", key, active);
        return key;
    }

    int get(int key) {
        auto it =  cacheMap.find(key);
        if (it == cacheMap.end()) {
            cout << "Cannot find key: " << key << endl;
            return -1;
        }

        it->second->active++; // 活跃次数加1
        printf("%d -> %d, active value=%d", key, it->second->val, it->second->active);
        return it->second->val;
    }

    void put(int key, int val) {

        if (cacheMap.size() == capacity) {
            // 删除不活跃的键值对
            int key = findInactiveKey();
            cacheMap.erase(key);
        }
        elementDesc *elem = new elementDesc(val, 0);
        cacheMap.emplace(key, elem);
    }
};



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
    LruCache *lc = new LruCache(3);
    lc->put(1,1);
    lc->put(2,2);
    lc->put(3,3);
    lc->get(2);
    lc->get(3);
    lc->put(4,4);
    lc->get(1);

    return 0;
}