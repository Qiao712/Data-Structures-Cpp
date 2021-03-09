/*求无向图的割点*/
#include<iostream>
using namespace std;

const int N = 20001;
const int M = 200002;
struct Edge{
    int to;
    int next = 0;
}edges[M];
int cnt;
int head[N];
int n, m;

int art[N]; //标记割点

int step;
int num[N],low[N],parent[N];
void Tarjan(int v){
    num[v] = low[v] = ++step;   //记录num，和暂时的low

    for(int i = head[v]; i!=0; i = edges[i].next){
        int to = edges[i].to;

        if(!num[to]){               //前向边
            parent[to] = v;
            Tarjan(to);
            low[v] = min(low[v], low[to]);

            if(low[to] >= num[v])
                art[v] = true;
        }else if(parent[v] != to){  //背向边(注意排除父节点)
            low[v] = min(low[v], num[to]);
        }
    }
}


void add(int v, int w){
    cnt++;
    edges[cnt].next = head[v];
    edges[cnt].to = w;
    head[v] = cnt;
}

int main(){
    cin>>n>>m;

    int v,w;
    for(int i = 0; i<m; i++){
        cin>>v>>w;
        add(v,w);
        add(w,v);
    }
    
    //假设图连通
    Tarjan(1);

    //根的特判
    int child = 0; 
    for(int j = head[1]; j!=0; j = edges[j].next)
        if(parent[edges[j].to] == 1) child++;
    art[1] = child > 1 ? true : false;

    for(int i = 1; i<=n; i++){
        if(art[i]) cout<<i<<' ';
    }
}