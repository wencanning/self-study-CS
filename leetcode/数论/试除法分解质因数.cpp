#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

/*
    对正整数质因数分解
    O(sqrt(n))
*/

class PrimeFactorization {
public:
    vector<int> fatorization(int n) {
        vector<int> res;
        // 先处理 2
        while(n % 2 == 0) {
            res.push_back(2);
            n /= 2;
        }
        // 剩下的所有因子都是奇数, 故可以跳过所有偶数
        // 大于sqrt(n)的质因数最多只有一个
        for(int i = 3; i <= sqrt(n); i += 2) {
            while(n % i == 0) {
                res.push_back(i);
                n /= i;
            }
        }
        // 大于sqrt(n)的质因数
        if(n > 2) 
            res.push_back(n);
        return res;
    }    
};

int main() {
    PrimeFactorization p;
    vector<int> vec = p.fatorization(645351);
    for(auto i : vec) 
        cout << i << " ";
    cout << endl;
    return 0;
}