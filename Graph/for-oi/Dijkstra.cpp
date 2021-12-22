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
int head[N];
int p;
int n,m;

unsigned int lengths[N];  //记录长度
void shortest(int s){
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>> > pq;
    fill(lengths, lengths + n + 1, INT_MAX);

    lengths[s] = 0;
    pq.emplace(lengths[s],s);

    int len, v;
    int known = 0;                             //已知道节点数
    while(!pq.empty()){
        len = pq.top().first;
        v = pq.top().second;
        pq.pop();

        if(lengths[v] < len) continue;         //堆中无效的记录（lengths已经被更新了，但堆里的值还在，是无效的，抛弃）
                                               //细节：不能用<=，否则会使初始时的第一个队内节点被略过

        
        for(int p = head[v]; p != 0; p = edges[p].next){
            int to = edges[p].to, w = edges[p].weight;
            if(lengths[to] > lengths[v] + w){
                lengths[to] = lengths[v] + w;
                pq.emplace(lengths[to], to);
            }
        }
    }
}

int main(){
    freopen("1.txt", "r", stdin);
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
        cout<<lengths[i]<<' ';
    }

    return 0;
}