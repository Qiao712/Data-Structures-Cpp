#pragma pack(8)
#include<iostream>

using namespace std;

#include"Tree\AVLTree.hpp"
#include"Tree\BinarySearchTree.hpp"
#include"Tree\SplayTree.hpp"
#include"List\Vector.hpp"
#include"List\List.hpp"
#include"PriorityQueue(Heap)\LeftistHeap.hpp"
#include"AlgorithmAnalysis\Quickselect.hpp"



// struct Node{
//     Node* l = nullptr;
//     Node* r = nullptr;

//     char e;
//     void print(){
//         cout<<e<<endl;
//     }
// };

// void print(Node* at){
//     if(at == nullptr) return;
//     cout<<at->e<<' ';
//     if(at->r != nullptr)
//         cout<<at->r->e<<' ';
//     if(at->l != nullptr)
//         cout<<at->l->e<<' ';
//     cout<<endl;

//     print(at->r);
//     print(at->l);
// }

// class A{
//     public:
//         A(int m){
//             cout<<"A:"<<m<<endl;
//         }
//         int f(int i){
//             cout<<"A:"<<i<<endl;
//             return i;
//         }
// };

// class B:public A{
//     public:
//         B(int m):A(m){

//         }
//         int f(int i){
//             cout<<"B:"<<i<<endl;
//             return i;
//         }
//         double f(double i){
//             cout<<"B:"<<i<<endl;
//             return i;
//         }
    
// };
struct A{
    char a;
    double b;
};
struct B{
    char a;
    int b;
    char c;
};
struct C{
    char a;
    short b;
    int c;
};

struct D{
    int a: 1;
    int b: 1;
    int c: 1;
};

int main(){
    int a[10] = {21,55,12,3,66,5,12,0,9,44};
    cout<<Quickselect(a, 10, 6);
    // for(auto i: a){
    //     cout<<i<<endl;
    // }
}

