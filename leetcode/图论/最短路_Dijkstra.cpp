#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, int> P;

const int INF = 0x3f3f3f3f;


struct edge { int to, cost; };


void Dijkstra(int n, int s) {
    vector<vector<edge>> g = vector(n+10,vector<edge>());
    vector<int> vis = vector(n+10, 0);
    vector<int> dis = vector(n+10, INF);

    priority_queue<P, vector<P>, greater<P>> pq;
    pq.push({0, s});
    dis[s] = 0;
    
    while(!pq.empty()) {
        P p = pq.top(); 
        pq.pop();
        int u = p.second;
        if(vis[u])
            continue;
        vis[u] = 1;
        for(int i = 0; i < g[u].size(); i++) {
            int v = g[u][i].to;
            int c = g[u][i].cost;
            if(dis[v] > dis[u] + c) {
                dis[v] = dis[u] + c;
                pq.push({dis[v], v});
            }
        }
    }
}
