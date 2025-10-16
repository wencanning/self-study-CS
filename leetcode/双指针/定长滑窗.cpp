#include <bits/stdc++.h>
using namespace std;


void solve() {
    string s;
    int k, ans;
    //枚举的区间为 [i-k+1, i]
    for(int i = 0; i < s.length(); i++) {
        // 1. 插入
        
        if(i-k+1 >= 0) {
            // 2. 更新答案
            ans = max(ans, 0);

            // 3. 删除
        }
    }
}