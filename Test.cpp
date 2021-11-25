#include<iostream>
#include<set>
#include<vector>
#include<cstdlib>
#include<algorithm>
#include<limits>
using namespace std;

#include"Tree\BinarySearchTree.hpp"
#include"Tree\AVLTree.hpp"
#include"Tree\SplayTree.hpp"
#include"Tree\RedBlackTree.hpp"

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
int findMax(multiset<int>& s){
    int m = numeric_limits<int>::min();
    for(int i : s) m = max(m, i);
    return m;
}
int findMin(multiset<int>& s){
    int m = numeric_limits<int>::max();
    for(int i : s) m = min(m, i);
    return m;
}

template<typename Tree>
bool test(){
    Tree t;
    multiset<int> s;

    int op;
    for(int i = 0; i<10000; i++){
        cout<<i+1<<' ';
        op = rand() % 6;
        switch(op){
            case 0:{
                int i = rand();
                t.insert(i);
                s.insert(i);
                cout<<"insert "<<i<<endl;

                break;
            }
            case 1:{
                cout<<"Find Max"<<endl;
                if(t.empty()) break;
                int m1 = t.findMax();
                int m2 = findMax(s);
                
                if(m1 != m2){
                    cout<<"FindMax error"<<endl;
                    return false;
                }
                break;
            }
            case 2:{
                cout<<"Find Min"<<endl;
                if(t.empty()) break;
                int m1 = t.findMin();
                int m2 = findMin(s);
                
                if(m1 != m2){
                    cout<<"FindMin error"<<endl;
                    return false;
                }
                break;
            }
            case 3:{
                cout<<"Contain"<<endl;
                int i = rand();
                if(t.contain(i) != (s.find(i) != s.end())){
                    cout<<"Contain error"<<endl;
                    return false;
                }
                break;
            }
            case 4:{
                cout<<"Size"<<endl;
                if(t.size() != s.size()){
                    cout<<t.size()<<endl;
                    cout<<s.size()<<endl;
                    cout<<"Size error"<<endl;
                    return false;
                }
                break;
            }
            case 5:{
                int i = rand();
                t.remove(i);
                auto it = s.find(i);
                if(it != s.end()) s.erase(it);

                cout<<"remove "<<i<<endl;
                if(t.size() != s.size()){
                    cout<<t.size()<<endl;
                    cout<<s.size()<<endl;
                    cout<<"Remove error"<<endl;
                    return false;
                }
                break;
            }
            case 6:{
                cout<<"Remove Min"<<endl;
                if(t.empty()) break;
                int m1 = t.findMin();
                int m2 = findMin(s);
                t.remove(m1);
                auto it = s.find(m2);
                if(it != s.end()) s.erase(it);
                break;
            }
        }

        if(t.size() != s.size()){
            cout<<"Size not right"<<endl;
            cout<<t.size()<<endl;
            cout<<s.size()<<endl;
            return false;
        }
    }
    return true;
}

template<typename Tree>
bool test_insert_and_remove(){
    Tree t;
    multiset<int> s;

    int op;
    for(int i = 0; i<1000; i++){
        cout<<i+1<<endl;
        op = rand() % 2;
        switch(op){
            case 0:{
                int i = rand()%100;
                t.insert(i);
                s.insert(i);
                cout<<"insert "<<i<<endl;
                break;
            }

            case 1:{
                int i = rand()%100;

                cout<<"remove "<<i<<endl;
                int old_size = t.size();

                int s1 = t.remove(i);
                if(s1 < old_size){
                    cout<<"remove success"<<endl;
                }
                auto it = s.find(i);
                if(it != s.end()){
                    s.erase(it);
                    cout<<"remove success"<<endl;
                }
                break;
            }
        }
        if(t.size() != s.size()){
            cout<<t.size() <<' '<<s.size()<<endl;
            
            while(!t.empty()){
                int m = t.findMax();
                cout<<m<<' ';
                t.remove(m);
            }
            cout<<"vs\n";
            for(int i : s) cout<<i<<' ';
            cout<<endl;

            return false;
        }
        for(int i : s){
            if(! t.contain(i)){
                cout<<"Not con."<<t.size()<<' '<<s.size()<<endl;

                while(!t.empty()){
                    int m = t.findMax();
                    cout<<m<<' ';
                    t.remove(m);
                }
                cout<<"-----------------"<<endl;
                for(int i : s) cout<<i<<' ';
                cout<<endl;
                
                return false;
            }
        }
    }

    //测试copy
    Tree t2(t);
    for(int i : s){
        if(! t2.contain(i)){
            cout<<"Copy error."<<endl;
            return false;
        }
    }
    if(t2.size() != s.size()) cout<<"Copy size error."<<endl;

    Tree t3;
    t3 = t;
    for(int i : s){
        if(! t3.contain(i)){
            cout<<"Copy error."<<endl;
            return false;
        }
    }
    if(t3.size() != s.size()) cout<<"Copy size error."<<endl;

    //内容是否一致
    for(int i : s){
        int m1 = t.findMin();
        t.remove(m1);
        int m2 = t2.findMin();
        t2.remove(m2);
        int m3 = t3.findMin();
        t3.remove(m3);
        cout<<'('<<i<<','<<m1<<')'<<endl;
        if(m1 != i /*|| m2 != i || m3 != i*/){
            cout<<"Insert or Remove Error"<<endl;
            //return false;
        }
    
    }
    cout<<"SUCCESS"<<endl;
    return true;
}

template<typename Tree, typename Ty>
bool test_insert(Tree& t, int n){
    multiset<int> s;
    for(int i = 0; i<n; i++){
        int r = rand()%100;
        t.insert(r);
        s.insert(r);
        if(!t._check()){
            cout<<"Insert Error"<<endl;
            return 0;
        }
        cout<<"insert "<<r<<endl;
    }

    cout<<"size: "<<t.size()<<' '<<s.size()<<endl;

    //内容是否一致
    for(int i : s){
        int m1 = t.findMin();
        t.remove(m1);
        cout<<'('<<i<<','<<m1<<')'<<endl;
        if(!t._check()) cout<<"Not a RedBlackTree"<<endl;
        if(m1 != i){
            cout<<"Insert or Remove Error"<<endl;
            return false;
        }
    }
    cout<<"INSERT SUCCESS"<<endl;
    return true;
}

template<typename Tree, typename Ty>
bool test_remove(Tree& t, int n){
    set<int> s;
    for(int i = 0; i<n; i++){
        int r = rand()%10;
        t.insert(r);
        s.insert(r);
        cout<<"insert "<<r<<endl;
    }

    for(int i = 0; i<n; i++){
        int r = rand()%10;

        t.remove(r);
        
        auto it = s.find(r);
        if(it != s.end()) s.erase(it);

        cout<<"remove "<<r<<" "<<t.size()<<' '<<s.size()<<endl;
    }

    t._debug();

    for(int i : s) cout<<i<<' ';
    cout<<"--------------"<<endl;
    while(t.size()){
        int m = t.findMin();
        cout<<m<<' ';
        t.remove(m);
    }

    // 内容是否一致
    // for(int i : s){
    //     int m1 = t.findMin();
    //     t.remove(m1);
    //     cout<<'('<<i<<','<<m1<<')'<<endl;
    //     if(m1 != i){
    //         cout<<"Insert or Remove Error"<<endl;
    //         return false;
    //     }
    // }    
    // cout<<"REMOVE SUCCESS"<<endl;
}

int main(){
    RedBlackTree<int> rbt;
    // test_insert<RedBlackTree<int>, int>(rbt, 1000);
    // test_remove<SplayTree<int>, int>(rbt, 10);
    // test_insert_and_remove<SplayTree<int>>();

    int a[] = {10,85,15,70,20,60,30,50,65,80};
    int len = sizeof(a) / sizeof(int);
    for(int i = 0; i<len; i++){
        rbt.insert(a[i]);
        cout<<"insert "<<a[i]<<endl;
        if(!rbt._check()) return 0;
    }
    
    // for(int i = 0; i<len; i++){
    //     rbt.contain(a[i]);
    //     rbt._debug();
    //     cout<<"------------------------------------\n";
    // }

    rbt._debug();
    cout<<"---------------------------------\n";

    for(int i = 0; i<len; i++){
        int m = rbt.findMax();
        cout<<m<<endl;
        rbt.remove(m);
        rbt._debug();
        cout<<(rbt._check() ? "OK" : "WRONG")<<endl;
        cout<<"---------------------------------\n";
    }
    // cout<<rbt.size();
    // rbt.remove(34);
    // cout<<rbt.size();
}