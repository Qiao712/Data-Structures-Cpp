#ifndef _DISJOINTSET_
#define _DISJOINTSET_
#include "../List/Vector.hpp"
/*
* 按秩合并、路径压缩
*/
class DisjointSet{
    public:
        DisjointSet(int size);

        void unionSet(int a, int b);    //union被关键词占用
        bool hasRelation(int a, int b);
    //private:
        int find(int a);

        Vector<int> parent;
};

DisjointSet::DisjointSet(int size){
    parent.reserve(size+1);
    for(int i = 0; i<size+1; i++){
        parent[i] = 0;
    }
}

int DisjointSet::find(int a){
    if(parent[a] > 0){
        parent[a] = find(parent[a]);
        return parent[a];
    }else{
        return a;
    }

    return 0; //消除警告
}

bool DisjointSet::hasRelation(int a, int b){
    return find(a) == find(b);
}

void DisjointSet::unionSet(int a, int b){
    int root_a = find(a);
    int root_b = find(b);

    //注意储存的是秩的负数
    if(parent[root_a] < parent[root_b]){
        parent[root_b] = root_a;
    }else if(parent[root_a] > parent[root_b]){
        parent[root_a] = root_b;
    }else{
        parent[root_b] = root_a;
        parent[root_a]--;  //秩加一
    }
}
#endif