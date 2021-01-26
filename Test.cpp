#include<iostream>
#include"Tree\AVLTree.h"
#include"Tree\BinarySearchTree.h"
#include"Tree\SplayTree.h"
#include"List\Vector.h"
#include"List\List.h"


using namespace std;

struct Node{
    Node* l = nullptr;
    Node* r = nullptr;

    char e;
    void print(){
        cout<<e<<endl;
    }
};

void print(Node* at){
    if(at == nullptr) return;
    cout<<at->e<<' ';
    if(at->r != nullptr)
        cout<<at->r->e<<' ';
    if(at->l != nullptr)
        cout<<at->l->e<<' ';
    cout<<endl;

    print(at->r);
    print(at->l);
}

class A{
    public:
        A(int m){
            cout<<"A:"<<m<<endl;
        }
        int f(int i){
            cout<<"A:"<<i<<endl;
            return i;
        }
};

class B:public A{
    public:
        B(int m):A(m){

        }
        int f(int i){
            cout<<"B:"<<i<<endl;
            return i;
        }
        double f(double i){
            cout<<"B:"<<i<<endl;
            return i;
        }
    
};

int main(){
    List<int> l;

    for(int i = 0; i<10; i++){
        l.push_back(i);
    }

    List<int> l2(l);

    cout<<l.size()<<endl;

    while(!l.empty()){
        cout<<l.back()<<endl;
        l.pop_back();
    }
    cout<<"--------------------"<<endl;
    auto i = l2.find(3);
    i = l2.erase(i);
    l2.insert(i, 3);
    for(auto i = l2.begin(); i != l2.end(); i++){
        cout<<*i<<endl;
    }
}


