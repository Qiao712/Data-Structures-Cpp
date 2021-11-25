#include <iostream>
using namespace std;

typedef int ElemType;

constexpr char LEFT = 0x01;         //left是否储存的是 前序线索
constexpr char RIGHT = 0x02;        //right是否储存的是 后序线索
struct Node{
    char thread;
    Node* left;
    Node* right;
    ElemType element;
};

//插入的时候建立线索
Node* insert(Node* t, const ElemType x){
    Node* new_node = new Node;
    new_node->element = x;
    new_node->thread = LEFT | RIGHT;

    if(t == nullptr){
        new_node->left = nullptr;
        new_node->right = nullptr;
        return new_node;
    }

    Node* cur = t;
    while(true){
        if(x < cur->element){
            if(cur->thread & LEFT){
                new_node->left = cur->left;         //前驱 为 其父节点原来的前驱
                new_node->right = cur;              //后续 为 其父节点
                
                cur->left = new_node;
                cur->thread ^= LEFT;
                break;
            }
            cur = cur->left;
        }else{
            if(cur->thread & RIGHT){
                new_node->right = cur->right;
                new_node->left = cur;

                cur->right = new_node;
                cur->thread ^= RIGHT;
                break;
            }
            cur = cur->right;
        }
    }

    return t;
}

Node* getNext(Node* t){
    if(t->thread & RIGHT){
        return t->right;
    }else{
        t = t->right;
        while( !(t->thread & LEFT) ) t = t->left;
        return t;
    }
}
void inorderTraverse(Node* t){
    while( !(t->thread & LEFT) ) t = t->left;
    
    while(t != nullptr){
        cout<<t->element<<' ';
        t = getNext(t);
    }
}

int main(){
    Node* root = nullptr;
    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 20);
    root = insert(root, 2);
    root = insert(root, 7);
    root = insert(root, 6);
    root = insert(root, 8);
    root = insert(root, 15);

    inorderTraverse(root);
}