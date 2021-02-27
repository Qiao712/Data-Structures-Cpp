/**
 * Improved Shortest Augumenting Path
 * 比Dinic更优一些的最大流算法
 * 不需要多次BFS分层，只要进行一次逆向的BFS（从t到s），再DFS中动态的分层。
 * 其基于DFS中对一个节点进行增广后，再次增广时的路径不可能比该路径更短。
*/
#include<limits>
#include<iostream>
#include<queue>
using namespace std;

const long long INF = numeric_limits<long long>::max();
const int NONE = 0;
const int M = 5001; //最大边数
const int N = 1000; //最大点数
struct Edge{
    int next = NONE;
    int c;  //容量
    int from; //因为要倒着储存路径
    int to;
}edge[M*2];
int cnt = 2;    //编号从2开始，才能通过i^1得到其另一半，作为反向边
int head[N];

int n,m,s,t;    //分别表示点的个数、有向边的个数、源点序号、汇点序号

//倒序分层
int d[N];
void BFS(){
    for(int i = 0; i<=n; i++) d[i] = -1;
    
    queue<int> q;
    q.push(t);
    d[t] = 0;

    while(!q.empty()){
        int curr = q.front();
        q.pop();

        for(int i = head[curr]; i!=0; i = edge[i].next){
            int to = edge[i].to;
            
            if(d[to] == -1){
                d[to] = d[curr] + 1;
                q.push(to);
            }
        }
    }
}

long long DFS(int curr, long long max_flow){
    if(curr == t || max_flow == 0) return max_flow;

    long long flow = 0;
    int min_d = n;  //记录残量不为0的出边连接到的最层数最小的点的层数

    for(int i = head[curr]; i != 0; i = edge[i].next){
        int to = edge[i].to;

        if(d[to] == d[curr] - 1 && edge[i].c){
            long long flow_delta = DFS(to, min(max_flow, (long long)edge[i].c));
            flow += flow_delta;
            max_flow -= flow_delta;
            edge[i].c -= flow_delta;
            edge[i^1].c += flow_delta;
        }
 
        if(edge[i].c > 0 && d[to] < min_d) min_d = d[to];   //找出增广接收后其连接到的层数最小的点的层数,不要放在if里啊..
        // //精确地找出最小层数,不如这样👆快
        // for(int j = head[curr]; j != 0; j = edge[j].next){
        //     if(edge[j].c) min_d = min(min_d, d[edge[j].to]);
        // }
        if(max_flow == 0) break;
    }

    //更新层
    d[curr] = min_d + 1;
    return flow;
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
    freopen("0.in", "r", stdin);
    cin>>n>>m>>s>>t;
    
    int u,w,c;
    for(int i = 0; i<m; i++){
        cin>>u>>w>>c;
        add(u,w,c);
        add(w,u,0); //同时加入反向边
    }

    long long max_flow = 0;
    BFS();
    while(d[s]<n){
        max_flow += DFS(s, INF);
    }

    cout<<max_flow;

    return 0;
}