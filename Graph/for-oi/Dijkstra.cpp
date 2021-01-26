/*使用优先队列优化*/
#include<iostream>
#include<deque>
#include<queue>
#include<climits>
using namespace std;

const int N = 100001;
const int M = 500001;
const int NONE = 0;
struct Edge{
    int next = NONE;
    int to = 0;
    int weight = 0;
}edges[M];
unsigned int length[N];  //记录长度
int head[N];
int p;
int n,m;

bool known[N];
int num;    //优先队列中实际有效的节点的个数    --   因为无法直接删除该节点在原来优先队列中的记录
bool inQueue[N];    //是否在优先队列中（实际有效的）
void shortest(int s){
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>> > pq;
    for(int i = 0; i<=n; i++) length[i] = INT_MAX;

    length[s] = 0;
    pq.emplace(length[s],s);
    num++;
    inQueue[s] = true;

    int c,nxt;
    while(num){
        c = pq.top().second;
        pq.pop();

        //防止队列里的废弃的距离被使用
        if(known[c]) continue;

        num--;
        known[c] = true;

        for(int i = head[c]; i != NONE; i = edges[i].next){
            nxt = edges[i].to;
            
            if(!known[nxt] && length[nxt] > length[c] + edges[i].weight){
                length[nxt] = length[c] + edges[i].weight;

                //没有加入过队列
                if(!inQueue[nxt]){
                    num++;  //队列中实际有效节点数量加一
                    inQueue[nxt] = true;
                }
                pq.emplace(length[nxt],nxt);    //将该距离记录到优先队列中
            }
        }
    }
}

int visit[N];
void print(int r){
    if(visit[r]) return;
    visit[r] = true;
    cout<<r<<endl;
    for(int i = head[r]; i != NONE; i = edges[i].next){
        print(edges[i].to);
    }
}

int main(){
    if(freopen("in.txt", "r", stdin) == nullptr) cout<<"error\n";
    freopen("out.txt", "w", stdout);
    int s;
    cin>>n>>m>>s;

    int v,w,t;
    for(int i = 0; i<m; i++){
        cin>>v>>w>>t;
        
        p++;
        
        edges[p].weight = t;
        edges[p].to     = w;

        //链接
        edges[p].next   = head[v];
        head[v] = p;
    }

    shortest(s);

    for(int i = 1; i<=n; i++){
        cout<<length[i]<<' ';
    }

    return 0;
}