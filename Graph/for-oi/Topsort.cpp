#include<iostream>
#include<deque>
#include<vector>
using namespace std;

const int N = 10000;
const int M = 500;
int g[N][M];
int p[N];
int d[N];   //储存入度
int n,m;

bool topsort(vector<int>& order){
    deque<int> q;

    //先
    int v;
    for(v = 1; v <= n; v++){
        if(d[v] == 0) q.push_back(v);
    }
    
    for(int i = 0; i<n; i++){
        if(q.empty()) return false;

        v = q.front();
        q.pop_front();
        order.push_back(v);
        for(int j = 0; j<p[v]; j++){
            int next = g[v][j];
            d[next]--;
            if(d[next] == 0){
                q.push_back(next);
            }
        }

    }

    return true;
}

int main(){
    cin>>n>>m;

    int v,w;
    for(int i = 0; i<m; i++){
        cin>>v>>w;
        g[v][p[v]++] = w;

        d[w]++;
    }

    vector<int> order;
    topsort(order);
    
    for(int i : order){
        cout<<i<<' ';
    }
}