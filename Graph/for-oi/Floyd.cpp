#include<iostream>
#include<deque>
#include<climits>
using namespace std;

const int N = 1000;
const int M = 200;
long long f[N][N] = {INT_MAX};    //邻接矩阵
int n,m;

void shortest(int s){
    for(int k = 1; k<=n; k++){
        for(int i = 1; i<=n; i++){
            for(int j = 1; j<=n; j++){
                f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
            }
        }
    }
}
    
int main(){
    if(freopen("in.txt", "r", stdin) == nullptr) cout<<"error\n";
    freopen("out.txt", "w", stdout);
    int s;
    cin>>n>>m>>s;

    for(int i = 0; i<=n; i++){
        for(int j = 0; j<=n; j++){
            f[i][j] = INT_MAX;
        }
    }

    int v,w,t;
    for(int i = 0; i<m; i++){
        cin>>v>>w>>t;
        f[v][w] = t;
    }

    shortest(s);

    for(int i = 0; i<=n; i++){
        for(int j = 0; j<=n; j++){
            if(f[i][j] == INT_MAX) cout<<"INF"<<' ';
            else cout<<f[i][j]<<' ';
        }
        cout<<endl;
    }
}