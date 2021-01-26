#include<iostream>
using namespace std;

const int N = 100001;
const int M = 200001;
const int NONE = 0;
struct Edge{
    int next = NONE;
    int to = 0;
    int weight = 0;
}edges[M];
int head[N],p;
int n,m;

//DFS
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
    int s;
    cin>>n>>m>>s;

    //读入(v,w)以及其权值t
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

    print(s);
}