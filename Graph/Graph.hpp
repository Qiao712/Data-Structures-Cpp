#include "../List/List.hpp"
#include "../List/Vector.hpp"
#include <iostream>

typedef unsigned int Vertex;
const unsigned int NONE_VERTEX = -1;
/**
 * 有向图
*/
class Graph{
    public:
        Graph(unsigned int size);    //size 节点个数

        void add(Vertex from, Vertex to);         //添加边
        void remove(Vertex from, Vertex to);      //移除边
        
        void print_BF(Vertex root);                  //打印与root相通的一部分图，层序(广度优先)
        void printAdjacency();                      //打印每个顶点所邻接的顶点
    private:
        unsigned int size;
        
        //使用邻接表储存图
        Vector<List<Vertex>> table;
};

Graph::Graph(unsigned int size){
    table.reserve(size+1);
}

void Graph::add(Vertex from, Vertex to){
    //先判断是否已经加入，防止重复加入
    List<Vertex>& list = table[from];
    for(auto i = list.begin(); i != list.end(); i++){
        if(*i == to){
            return;
        }
    }

    table[from].push_back(to);
}

void Graph::remove(Vertex from, Vertex to){
    List<Vertex>& list = table[from];
    for(auto i = list.begin(); i != list.end(); i++){
        if(*i == to){
            list.erase(i);
            return;
        }
    }
}

void Graph::print_BF(Vertex root){
    List<Vertex> queue;   //队列
    queue.push_back(root);
    queue.push_back(NONE_VERTEX);     //用NONE_NODE来分割层
    
    Vertex cur;
    while(queue.front() != NONE_VERTEX || queue.back() != NONE_VERTEX){    //仅剩下一个分割用的NODE_VERTEX时退出循环
        cur = queue.front();
        queue.pop_front();

        if(cur == NONE_VERTEX){
            std::cout<<std::endl;
            queue.push_back(NONE_VERTEX);
        }
        else{
            std::cout<<cur<<' ';

            //邻接的顶点
            for(auto i = table[cur].begin(); i != table[cur].end(); i++){
                queue.push_back(*i);
            }
        }
    }
}

void Graph::printAdjacency(){
    for(int i = 0; i<=size; i++){
        std::cout<<i<<':';
        for(auto j = table[i].begin(); j != table[i].end(); j++){
            std::cout<<*j<<' ';
        }
        std::cout<<std::endl;
    }
}