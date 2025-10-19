#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 1e5;

int is_prime[MAXN+10];


/*
    O(n loglogn)
    n/2 + n/3 + n/5 + n/7 + n/11 + ... = n × (1/2 + 1/3 + 1/5 + 1/7 + 1/11 + ...
    ∑(1/p) ~ log log n   (p ≤ n的素数)
*/
int main() {
    // 1. 首先将所有数置为素数(0,1除外)
    for(int i = 2; i <= MAXN; i++) {
        is_prime[i] = 1;
    }
    // 2. 开始遍历
    for(int i = 2; (ll)i*i <= MAXN; i++) {
        if(is_prime[i]) {
            for(int j = i*i; j <= MAXN; j += i) {
                is_prime[j] = 0;
            }
        }
    }
    /*
        细节解释：
        1. 外层循环中的 i*i <= MAXN： 因为内层循环的初始化为i*i
        2. 内层循环中的 j = i*i： 所有小于i*i的数都被筛掉了，因为i*k其中k是小于
        i的素数，那么在i=k时，i*k就被筛掉了。
        3. 内层i*i不加long long：外层已经加过判断过了

        注意乘法爆long long的问题
    */
}
