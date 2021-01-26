/*
非递归的遍历
后序遍历（先序遍历）
中序遍历
层序遍历
*/

#include<iostream>
#include<stack>
#include<queue>
using namespace std;

struct Node{
    int element;
    Node* l = nullptr;
    Node* r = nullptr;
    bool isVisited = 0;
};

//后序遍历-非递归
void postorderTraversal(Node* t){
    stack<Node*> s1;
    stack<Node*> s2;                //以先序遍历把节点入栈，再按出栈顺序执行操作，即把先序遍历倒过来
    s1.push(t);

    Node* tmp;
    while(!s1.empty()){
        tmp = s1.top();
        s1.pop();
        s2.push(tmp);

        if(tmp->l != nullptr) s1.push(tmp->l);
        if(tmp->r != nullptr) s1.push(tmp->r);
    }

    while(!s2.empty()){
        tmp = s2.top();
        s2.pop();

        cout<<tmp->element<<' ';
    }
}
//后序遍历-递归
void postorderTraversal_recursion(Node* t){
    if(t == nullptr) return;
    postorderTraversal_recursion(t->l);
    postorderTraversal_recursion(t->r);
    cout<<t->element<<' ';
}

//中序遍历-非递归的
bool isVisited(Node* n){
    return n->isVisited;
}
void inorderTraversal(Node* t){
    stack<Node*> s1;
    s1.push(t);

    Node* tmp;
    while(!s1.empty()){
        tmp = s1.top();
        
        if(tmp->l == nullptr || isVisited(tmp->l)){
            s1.pop();
            tmp->isVisited = true;
            
            //do
            cout<<tmp->element<<' ';

            if(tmp->r != nullptr) s1.push(tmp->r);
        }else{
            s1.push(tmp->l);
        }
    }
}

//中序遍历-非递归的-无需标记
void inorderTraversal_non_record(Node* t){
    stack<Node*> s;

    do{
        //t:当前遍历到的节点,一直向左节点移动,一直移动到最左端
        while(t != nullptr){
            s.push(t);
            t = t->l;   //向左节点移动
        }
        
        //按出栈的顺序访问靠左的节点
        if(!s.empty()){
            t = s.top();
            s.pop();

            cout<<t->element<<' ';
            
            t = t->r;   //下一个要中序访问的子树，即其右节点
        }
    }while(!s.empty() || t != nullptr);
}

//递归-中序遍历
void inorderTraversal_recursion(Node* t){
    if(t == nullptr){
        return;
    }
    inorderTraversal_recursion(t->l);
    cout<<t->element<<' ';
    inorderTraversal_recursion(t->r);
}

//层序遍历
void levelorderTraversal(Node* t){
    deque<Node*> q;
    q.push_back(t);

    while(!q.empty()){
        t = q.front();
        q.pop_front();
        
        if(t == nullptr) continue;

        cout<<t->element<<' ';
        
        q.push_back(t->l);
        q.push_back(t->r);
    }
}

int main(){
    Node* n = new Node[10];
    for(int i = 0; i<10; i++){
        n[i].element = i;
    }

    n[0].l = &n[1];
    n[1].l = &n[2];
    n[2].r = &n[3];
    n[0].r = &n[4];
    n[4].l = &n[5];

    inorderTraversal(&n[0]);
    cout<<endl;
    inorderTraversal_non_record(&n[0]);
    cout<<endl;
    inorderTraversal_recursion(&n[0]);
    cout<<endl;
    postorderTraversal(&n[0]);
    cout<<endl;
    postorderTraversal_recursion(&n[0]);
    cout<<endl;
    levelorderTraversal(&n[0]);
}