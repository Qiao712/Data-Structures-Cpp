/**
 * 通过简单的BFS找增广路径
 * 使用链式前向星储存边
*/
#include<limits>
#include<iostream>
#include<queue>
using namespace std;

const int NONE = 0;
const int M = 5001; //最大边数
const int N = 1000; //最大点数
struct Edge{
    int next = NONE;
    int c;          //容量
    int from;       //因为要倒着储存路径
    int to;
}edge[M*2];
int cnt = 2;        //编号从2开始，才能通过i^1得到其另一半，作为反向边
int head[N];

int n,m,s,t;        //分别表示点的个数、有向边的个数、源点序号、汇点序号

//BFS找到一条增广路径
int path[N];        //储存一条增广路径，path[i] 路径上进入i的边
bool visit[N];
bool bfs(){
    for(int i = 0; i<=n; i++) visit[i] = path[i] = 0;

    queue<int> q;
    q.push(s);
    visit[s] = true;

    while(!q.empty()){
        int curr = q.front();
        q.pop();

        for(int i = head[curr]; i != NONE; i = edge[i].next){
            int to = edge[i].to;
            if(edge[i].c == 0 || visit[to]) continue;

            path[to] = i;
            if(to == t) return true;
            q.push(to);
            visit[to] = true;
        }
    }
    
    return false;
}

long long Edmond_Karp(){
    long long max_flow = 0;        
    while(true){
        //未找到，结束算法
        if(!bfs()) break;

        //找到该路径上容量最小的边的容量
        int delta = numeric_limits<int>::max();
        for(int i = t; i != s; i = edge[path[i]].from){
            delta = min(delta, edge[path[i]].c);
        }

        //更新边与反向边
        for(int i = t; i != s; i = edge[path[i]].from){
            edge[path[i]].c -= delta;
            edge[path[i] ^ 1].c += delta;
        }

        max_flow += delta;
    }

    return max_flow;
}

void add(int from, int to, int c){
    edge[cnt].next = head[from];
    edge[cnt].c = c;
    edge[cnt].to = to;
    edge[cnt].from = from;
    head[from] = cnt;
    cnt++;
}

int main(){
    cin>>n>>m>>s>>t;
    
    int u,w,c;
    for(int i = 0; i<m; i++){
        cin>>u>>w>>c;
        add(u,w,c);
        add(w,u,0); //同时加入反向边
    }

    cout<<Edmond_Karp();
}