/*求强连通分量 Strongly Connected Components，SCC*/
#include<iostream>
#include<stack>
using namespace std;

const int N = 20001;
const int M = 200002;

struct Edge{
    int to;
    int next = 0;
};
//原图
Edge edges[M];
int head[N];
//反向图
Edge r_edges[M];
int r_head[N];
int cnt;
int n, m;

//记录节点dfs的次序
bool vis[N];
stack<int> vs;  //按dfs的次序储存点，小的先入栈

void dfs(int v){
    vis[v] = true;
    for(int i = head[v]; i!=0; i = edges[i].next){
        int to = edges[i].to;
        if(!vis[to]) dfs(to);
    }
    vs.push(v);
}

//反向图中的dfs
int mark;       //该次dfs标记点时所用的颜色
int color[N];   //标记属于同一个连通分量的点
void r_dfs(int v){
    cout<<v<<' '<<color[v]<<endl;
    
    color[v] = mark;    //标记
    for(int i = r_head[v]; i!=0; i = r_edges[i].next){
        int to = r_edges[i].to;
        if(color[to] == 0) r_dfs(to);
    }
}

void Kosaraju(){
    //多次dfs遍历所有点
    for(int i = 1; i<=n; i++){
        if(!vis[i]) dfs(i);
    }

    //从原图dfs中先处理的点开始 在反图中dfs
    while(!vs.empty()){
        int to = vs.top();
        vs.pop();
        if(color[to]) continue;

        mark++;
        r_dfs(to);

        cout<<"-----------------"<<endl;
    }
}

void add(int v, int w){
    cnt++;
    //加入原图
    edges[cnt].next = head[v];
    edges[cnt].to = w;
    head[v] = cnt;
    //反向图
    r_edges[cnt].next = r_head[w];
    r_edges[cnt].to = v;
    r_head[w] = cnt;
}

int main(){
    freopen("0.in", "r", stdin);

    cin>>n>>m;
    int v,w;
    for(int i = 0; i<m; i++){
        cin>>v>>w;
        add(v,w);
    }
    
    Kosaraju();
}