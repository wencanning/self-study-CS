#include <bits/stdc++.h>
using namespace std;



class SegmentTree {
    private:
        inline int lc(int i) { return i << 1; }
        inline int rc(int i) { return i << 1 | 1; }
    
        struct Node {
            int l, r;
            int mx, mi;
            int add;
        };
        
        vector<Node> tree;
    
        void build(int p, int l, int r) {
            tree[p] = {l, r, 0, 0, 0};
            if(l == r) return ;
            int mid = (l + r) / 2;
            build(lc(p), l, mid);
            build(rc(p), mid+1, r);
        }
    
        void pushup(int p) {
            Node& rt = tree[p];
            Node& l = tree[lc(p)];
            Node& r = tree[rc(p)];
            rt.mx = max(l.mx, r.mx);
            rt.mi = min(l.mi, r.mi);
        }
    
        void pushdown(int p) {
            Node& rt = tree[p];
            apply(lc(p), rt.add);
            apply(rc(p), rt.add);
            rt.add = 0;
        }
    
        void apply(int p, int val) {
            tree[p].add += val;
            tree[p].mi += val;
            tree[p].mx += val;
        }
    
        void update(int p, int l, int r, int val) {
            if(l > r) return ;
            if(l <= tree[p].l && tree[p].r <= r) {
                apply(p, val);
                return ;
            }
            int mid = (tree[p].l + tree[p].r) / 2;
            pushdown(p);
            if(l <= mid) update(lc(p), l, r, val);
            if(r > mid) update(rc(p), l, r, val);
            pushup(p);
        }
    
        int find_first(int p, int l, int r, int target) {
            if(l > r) return -1;
            if(l > tree[p].r || r < tree[p].l || tree[p].mx < target || tree[p].mi > target)
                return -1;
            if(tree[p].l == tree[p].r)
                return tree[p].l;
            
            //查询的时候懒标记一定要下放
            pushdown(p);
            int mid = (tree[p].l + tree[p].r) / 2;
            int idx = find_first(lc(p), l, r, target);
            if(idx < 0) {
                idx = find_first(rc(p), l, r, target);
            }
            return idx;
        }
    public:
        SegmentTree(const vector<int>& a) {
            int len = a.size();
            // 线段树空间的极限情况是 4*n
            tree.resize(4*len + 1);
            build(1, 0, len - 1);
        }
    
        // 区间加减
        void update(int l, int r, int val) {
            update(1, l, r, val);
        }
    
        // 线段树上二分
        int find_first(int l, int r, int target) {
            return find_first(1, l, r, target);
        }
    };
    
    

int main() {
    vector<int> arr;
    SegmentTree tree = SegmentTree(arr);


    return 0;
}