/**
 * High Level Preflow Push
 * 即优先推送高度高的点
 * 外加两个优化：初始化高度时，初始化为距离汇点的最短路径（无权值）。
 *              当某高度的节点的数量为0时，将大于该高度的所有节点的高度设为n+1
*/
#include<limits>
#include<iostream>
#include<queue>
using namespace std;

const int MAX_INF = numeric_limits<int>::max();
const long long INF = numeric_limits<long long>::max();
const int NONE = 0;
const int M = 120001; //最大边数
const int N = 1201; //最大点数
struct Edge{
    int next = NONE;
    int c;  //残余量
    int to;
}edge[M*2];
int cnt = 2;    //编号从2开始，才能通过i^1得到其另一半，作为反向边
int head[N];
int h[N];
long long e[N]; //超额流
int gap[N*2];     //高度为i的节点的数量, 注意数组大小要大，高度会比节点数量大

int n,m,s,t;    //分别表示点的个数、有向边的个数、源点序号、汇点序号

struct Compare{
    bool operator()(int a, int b){return h[a] < h[b];}
};
bool vis[N];
priority_queue<int, vector<int>, Compare> q;

//bfs:初始化高度
bool vis_bfs[N];
void bfs(){
    for(int i = 0; i<=n; i++) h[i] = -1;

    queue<int> q;
    q.push(t);
    h[t] = 0;
    vis_bfs[t] = true;

    while(!q.empty()){
        int curr = q.front();
        q.pop();

        for(int i = head[curr]; i != 0; i = edge[i].next){
            int to = edge[i].to;
            if(edge[i^1].c != 0 && !vis_bfs[to]){
                h[to] = h[curr] + 1;
                q.push(to);
                vis_bfs[to] = true;
            }
        }
    }
}

bool push(int u){
    for(int i = head[u]; i != 0 && e[u] != 0; i = edge[i].next){
        int to = edge[i].to;
        if(edge[i].c != 0 && h[u] == h[to] + 1){
            long long flow_delta = min((long long)edge[i].c, e[u]);

            edge[i].c -= flow_delta;
            edge[i^1].c += flow_delta;
            e[u] -= flow_delta;
            e[to] += flow_delta;

            if(!vis[to]){
                q.push(to);
                vis[to] = true;
            }
        }
    }

    return (e[u] > 0);
}

void relabel(int u){
    int min_h = MAX_INF;
    for(int i = head[u]; i != 0; i = edge[i].next){
        if(edge[i].c > 0) min_h = min(min_h, h[edge[i].to]);
    }
    h[u] = min_h + 1;
}

long long GetMaxFlow(){
    //初始化
    for(int i = head[s]; i != 0; i = edge[i].next){
        e[s] -= edge[i].c;
        e[edge[i].to] += edge[i].c;
        edge[i^1].c = edge[i].c;
        edge[i].c = 0;

        q.push(edge[i].to);
        vis[edge[i].to] = true;
    }
    bfs();
    h[s] = n;

    //统计高度
    for(int i = 1; i<=n; i++) if(h[i] != -1) gap[h[i]]++;
    
    //不断推送重贴标签直至没有溢出的点(除汇点)
    while(!q.empty()){
        int curr = q.top();
        q.pop();
        vis[curr] = false;

        //排除汇点源点
        if(curr == s || curr == t) continue;

        while(push(curr)){
            gap[h[curr]]--;
            if(! gap[h[curr]])
                for(int i = 1; i<=n; i++)
                    if(i != t && i != s && h[i] > h[curr] && h[i] < n+1) h[i] = n+1;
            relabel(curr);
            gap[h[curr]]++;
        }
    }

    return e[t];
}

void add(int from, int to, int c){
    edge[cnt].next = head[from];
    edge[cnt].c = c;
    edge[cnt].to = to;
    head[from] = cnt;
    cnt++;
}

int main(){
    cin>>n>>m>>s>>t;
    
    long long u,w,c;
    for(int i = 0; i<m; i++){
        cin>>u>>w>>c;
        add(u,w,c);
        add(w,u,0); //同时加入反向边
    }

    cout<<GetMaxFlow();
}