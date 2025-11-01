#include <bits/stdc++.h>
using namespace std;
/*
    树状数组

    动态点更新，区间求和

    - 查询：依次累加以i为右端点的关键区间，下一个关键区间为 i - lowbit(i) 
    - 更新: 依次更新以i为右端点，包含pos处元素的关键区间，下一个关键区间为 i + lowbit(i)

*/



template <typename T>
class FenwickTree {
    vector<T> tree;

public:
    FenwickTree(int n) : tree(n+1) {}
    
    FenwickTree(const vector<T>& nums) : tree(nums.size()+1) {
        for(int i = 1; i <= nums.size(); i++) {
            tree[i] += nums[i-1];
            int nxt = i + (i & -i);
            if(nxt <= nums.size()) {
                tree[nxt] += tree[i];
            }
        }
    }

    void update(int i, T val) {
        for(; i < tree.size(); i += i & -i) {
            tree[i] += val;
        }
    }

    T pre(int i) const {
        T res = 0;
        for(; i > 0; i &= i-1) { // i -= i & -i 的另一种写法
            res += tree[i];
        }
        return res;
    }
    
    T query(int l, int r) const {
        if(r < l) {
            return 0;
        }
        return pre(r) - pre(l-1);
    }
};