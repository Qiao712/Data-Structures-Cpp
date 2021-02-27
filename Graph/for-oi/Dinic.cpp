/**
 * Dinic算法
 * 求最大流， 通过给图分层来加快寻找增广路径
*/
#include<iostream>
#include<queue>
#include<limits>
using namespace std;

const long long INF = numeric_limits<long long>::max();
const int NONE = 0;
const int M = 50001; //最大边数
const int N = 1000; //最大点数
struct Edge{
    int from;
    int to;
    long long c;
    int next = NONE;
}edge[M*2];
int head[N];
int cnt = 2;

//分别表示点的个数、有向边的个数、源点序号、汇点序号
int n,m,s,t;

//分层， 找到收点：true
int d[N];
bool BFS(){
    for(int i = 0; i<=n; i++) d[i] = -1;

    queue<int> q;
    q.push(s);
    d[s] = 0;

    while(!q.empty()){
        int curr = q.front();
        q.pop();

        for(int i = head[curr]; i!=0; i = edge[i].next){
            int to = edge[i].to;

            if(d[to] == -1 && edge[i].c > 0){
                d[to] = d[curr] + 1;
                q.push(to);
            }
        }
    }

    return (d[t] != -1);
}

//递归地寻找增广路径
//curr 当前节点
//max_flow 到当前点最大的流
//返回增加地流
//动态的分流
int time = 0;
long long DFS(int curr, long long max_flow){
    if(time == 98) cout<<d[curr]<<endl;
    if(curr == t || max_flow == 0) return max_flow;

    long long flow = 0;
    for(int i = head[curr]; i!=0; i = edge[i].next){
        int to = edge[i].to;

        if(d[to] == d[curr] + 1 && edge[i].c > 0){
            long long flow_delta = DFS(to, min(max_flow, edge[i].c));
            edge[i].c -= flow_delta;
            edge[i^1].c += flow_delta;
            max_flow -= flow_delta;
            flow += flow_delta;
        }

        if(max_flow == 0) break;
    }

    // 这个判断非常重要！！！否则某些特殊情况会慢上天
    //flow == 0说明该点与无法联通，屏蔽掉这个点
    if(flow == 0){
        d[curr] = -1;
    }

    return flow;
}

long long Dinic(){
    long long max_flow = 0;
    while(BFS()){
        cout<<++time<<endl;
        max_flow += DFS(s, INF);
    }
    return max_flow;
}

void add(int from, int to, int c){
    edge[cnt].to = to;
    edge[cnt].next = head[from];
    edge[cnt].c = c;
    head[from] = cnt;
    cnt++;
}

int main(){
    cin>>n>>m>>s>>t;

    int v,w,c;
    for(int i = 0; i<m; i++){
        cin>>v>>w>>c;
        add(v,w,c);
        add(w,v,0);
    }

    cout<<Dinic();
}