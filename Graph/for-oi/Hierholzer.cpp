/*求欧拉环路/路径（无向图）（字典序最小的）
* 
*/
#include<iostream>
#include<vector>
#include<list>
#include<algorithm>
using namespace std;

const int N = 20001;
const int M = 100001;
struct Edge{
    int to;
    int isDeleted = false;
}edges[M];
int cnt = 1;          //从2起始
vector<int> table[N]; //邻接表
int p[N];             //某个节点当前遍历到的边
int d[N];             //节点的度
int n, m;

//添加边
void add(int v, int w){
    edges[++cnt].to = w;
    table[v].push_back(cnt);
    edges[++cnt].to = v;
    table[w].push_back(cnt);
}

list<int> circuit;  //保存该次得到的路径
int Hierholzer(int v){
    int size = table[v].size();
    for(int& i = p[v]; i<size; i++){
        int e = table[v][i];
        if(edges[e].isDeleted) continue;
        int to = edges[e].to;

        //删除走过的边
        edges[e].isDeleted = true;
        edges[e^1].isDeleted = true;

        Hierholzer(to);
    }

    circuit.push_back(v);   //最后加入！！！
}

int main(){
    cin>>n>>m;

    int v,w;
    for(int i = 0; i<m; i++){
        cin>>v>>w;
        d[v]++;
        d[w]++;
        add(v,w);
    }

    //为了得到字典序小的路径
    for(int i = 1; i<=n; i++){
        sort(table[i].begin(), table[i].end(), [](const int& x, const int& y)->bool
        {
            return edges[x].to < edges[y].to; 
        });
    }

    //找一个最小的度为奇数的点开始
    bool flag = false;
    for(int i = 1; i<=n; i++){
        if(d[i]&1){  //度为奇数
            Hierholzer(i);
            flag = true;
            break;
        }
    }

    //如果没有度为奇数的点，则可形成环路
    if(!flag){
        for(int i = 1; i<=n; i++)
            if(d[i] != 0){
                Hierholzer(i);
                break;
            }
    }

    //倒序输出
    for(auto i = circuit.rbegin(); i!=circuit.rend(); i++){
        cout<<*i<<endl;
    }
}