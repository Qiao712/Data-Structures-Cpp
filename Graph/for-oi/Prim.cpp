/*使用优先队列优化*/
#include<iostream>
#include<deque>
#include<queue>
#include<climits>
#include<algorithm>
using namespace std;

const int N = 100001;
const int M = 500001;
const int NONE = 0;
struct Edge{
    int next = NONE;
    int to = 0;
    int c = 0;  //边权
}edges[M];
int head[N];
int cnt;

int n, m;

bool known[N];//是否已知(即是否已在树上且无法再改进)
int pre[N]; //储存树上某节点的前驱（若不需要可以不用）
int c[N];   //点i与树连接的边的权值

long long Prim(){
    for(int i = 0; i<=n; i++) c[i] = numeric_limits<int>::max();
    long long sum = 0;
    int num = 0;                        //计数访问到的点，判读图是否连通
    typedef pair<int,int> CVP;          //边权-点 对(使点连接到树上的边的权)
    priority_queue<CVP, vector<CVP>, greater<CVP>> q;
    
    //从点1开始
    q.push(make_pair(0,1));
    c[1]   = 0;

    while(!q.empty()){
        auto p = q.top();
        q.pop();
        int curr = p.second;
        int c_curr = p.first;
        if(known[curr]) continue;       //之前被覆盖的点被再次使用
        
        known[curr] = true;
        num++;
        sum += c_curr;

        for(int i = head[curr]; i!=0; i = edges[i].next){
            int to = edges[i].to;
            if(known[to]) continue;
            
            //使 使to点连接到树上的边的权值最小
            if(c[to] > edges[i].c){
                c[to] = edges[i].c;
                pre[to] = curr;
                //加入优先队列，如果之前被加入过，那就相当于覆盖掉之前那个
                q.push(make_pair(c[to], to));
            }
        }
    }

    if(num < n) return -1;  //不连通
    else return sum;
}

void add(int u, int v, int c){
    cnt++;
    edges[cnt].next = head[u];
    edges[cnt].c = c;
    edges[cnt].to = v;
    head[u] = cnt;
}

int main(){
    cin>>n>>m;

    int u,v,c;
    for(int i = 0; i<m; i++){
        cin>>u>>v>>c;
        //建立无向图
        add(u,v,c);
        add(v,u,c);
    }

    cout<<Prim();
}