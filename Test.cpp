#include<iostream>
#include"Tree\AVLTree.hpp"
#include"Tree\BinarySearchTree.hpp"
#include"Tree\SplayTree.hpp"
#include"List\Vector.hpp"
#include"List\List.hpp"


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
    cout<<"aaa";
    char a[16];
    cout<<sizeof(a)<<"bytes";
}


