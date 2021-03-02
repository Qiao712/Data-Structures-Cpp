#include<iostream>
#include<queue>
#include<limits>
using namespace std;

const int M = 200001; //最大边数
const int N = 5001; //最大点数
int n, m;
class Edge{
public:
    Edge(int v_, int u_, int c_): v(v_), u(u_), c(c_){ }
    int v,u;
    int c;
    bool operator<(const Edge& rhs) const {return c > rhs.c; }
};
priority_queue<Edge> q;

/**按秩合并的并查集**/
int f[N];
int Find(int x){
    if(f[x] <= 0){
        return x;
    }else{
        return f[x] = Find(f[x]);
    }
}
void Union(int root_x, int root_y){
    if(f[root_x] > f[root_y]){
        f[root_x] = root_y;
    }else if(f[root_x] < f[root_y]){
        f[root_y] = root_x;
    }else{
        f[root_x] = root_y;
        f[root_y]--;
    }
}
/*******************/

int Kruskal(){
    int num = 0;    //已添加的边的条数
    int sum = 0;    //总权重

    while(num < n - 1){
        if(q.empty()) return -1;    //不连通则返回-1

        Edge edge = q.top();
        q.pop();    //使用引用，一定要注意什么时候弹出
        int root_u = Find(edge.u);
        int root_v = Find(edge.v);

        if(root_u == root_v) continue;

        Union(root_u, root_v);
        sum += edge.c;
        num++;
    }

    return sum;
}

int main(){
    cin>>n>>m;

    int v,w,c;
    for(int i = 0; i<m; i++){
        cin>>v>>w>>c;
        q.emplace(v,w,c);
    }

    cout<<Kruskal();
}