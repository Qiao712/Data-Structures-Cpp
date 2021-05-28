//Tarjan算法-求强连通分支(Strongly Connected Components，SCC)
#include<iostream>
using namespace std;

const int M = 1e5+1;
const int N = 1e5+1;
struct Edge{
    int to;
    int next;
}edges[M];
int head[N];
int cnt;

void add(int from, int to){
    edges[++cnt].to = to;
    edges[cnt].next = head[from];
    head[from] = cnt;
}

int num[N], low[N];
void Tarjan(int v, int p){
    num[v] = p;
    low[v] = p;
    for(int i = head[v]; i!=0; i = edges[i].next){
        int to = edges[i].to;

        Tarjan(v, p+1);

        if(num[to]) low[v] = min(num[to], low[v]);  //通过背向边（返回）
        else low[v] = min(low[to], low[v]);         //通过其子节点返回

        //判断割点
        if(num[v] <= low[v]){
            cout<<v<<endl;
        }
    }
}

int main(){
    freopen("in.txt", "r", stdin);
    int n,m;
    cin>>n>>m;

    int u,v;
    for(int i = 0; i<m; i++){
        cin>>u>>v;
        add(u,v);
        add(v,u);
    }

    Tarjan(1,0);
}