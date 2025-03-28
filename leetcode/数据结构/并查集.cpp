#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5 + 10;

namespace dsu {
    int pre[MAXN];

    void init(int n) {
        for(int i = 1; i <= n; i++) {
            pre[i] = i;
        }
    }

    int find(int x) {
        return pre[x] == x ? x : pre[x] = find(pre[x]);
    }
    //特别注意merge的写法
    void merge(int x, int y) {
        pre[find(x)] = find(y);
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }
}