
void solve() {
    int n, l, res;
    l = 0;
    //枚举的区间为 [l, i]
    for(int i = 0; i < n; i++) {
        //1. 插入 
        
        //2. 删除
        while(l <= i) {
            
        }

        //3. 更新答案
        if(l <= i && i-l+1 > res)
            res = i-l+1;
    } 
}