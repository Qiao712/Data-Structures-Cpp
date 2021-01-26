#include<iostream>
#include<deque>
#include<climits>
using namespace std;

const int N = 100001;
const int M = 200;
int g[N][M];
int e[N][M];   //边权
unsigned int length[N];  //记录长度
int p[N];
int n,m;

bool inQueue[N];    //是否以入队
void shortest(int s){
    deque<int> q;
    for(int i = 0; i<=n; i++) length[i] = INT_MAX;
    length[s] = 0;

    int c,nxt;
    q.push_back(s);
    inQueue[s] = true;

    while(!q.empty()){
        c = q.front();
        q.pop_front();
        inQueue[c] = false;

        for(int i = 0; i<p[c]; i++){          
            nxt = g[c][i];

            //若可以松弛并不在队列中
            if(length[nxt] > length[c] + e[c][i]){
                length[nxt] = length[c] + e[c][i]; //松弛

                //入队
                if(!inQueue[nxt]){
                    q.push_back(nxt);
                    inQueue[nxt] = true;
                }
            }
        }
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
        g[v][p[v]] = w;
        e[v][p[v]++] = t;
    }

    shortest(s);

    for(int i = 1; i<=n; i++){
        cout<<length[i]<<' ';
    }
}