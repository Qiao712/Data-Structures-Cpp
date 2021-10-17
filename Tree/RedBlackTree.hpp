#ifndef __RED_BLACK_TREE__
#define __RED_BLACK_TREE__
#include "BinarySearchTree.hpp"

#include <iostream>
using namespace std;

template<typename Comparable>
struct NodeWithColor{
    Comparable element;
    NodeWithColor* left = nullptr;
    NodeWithColor* right = nullptr;
    bool color;
};

template<typename Comparable>
class RedBlackTree : private BinarySearchTreeTemplate<Comparable, NodeWithColor<Comparable>>{
    public:
        using Node = NodeWithColor<Comparable>;
        
        using BinarySearchTreeTemplate<Comparable, Node>::findMin;                            //返回最小元素
        using BinarySearchTreeTemplate<Comparable, Node>::findMax;                            //返回最大元素
        using BinarySearchTreeTemplate<Comparable, Node>::contain;                           //是否包含该元素
        using BinarySearchTreeTemplate<Comparable, Node>::empty;                              //是否为空
        using BinarySearchTreeTemplate<Comparable, Node>::size;                               //返回大小
        using BinarySearchTreeTemplate<Comparable, Node>::clear;
        void insert(const Comparable& x);                                   //插入-接口,复写
        void remove(const Comparable& x);                                   //删除-接口，删除指定节点（非懒惰删除）

        void debug(){
            cout<<size()<<" nodes"<<endl;
            if(check(root) == -1) cout<<"Not A Red-Black Tree"<<endl;
            print(root);
        }
        void print(Node* t, int layer = 0){
            cout<<t->element<<'('<<t->color<<')'<<endl;

            if(t->left != nullptr){
                for(int i = 0; i<=layer; i++) cout<<"  ";
                cout<<"L:";
                print(t->left, layer+1);   
            }

            if(t->right != nullptr){
                for(int i = 0; i<=layer; i++) cout<<"  ";
                cout<<"R:";
                print(t->right, layer+1);
            }
        }
        //check whether the tree conform the Black-Red Tree
        int check(Node* t, int layer = 0){
            if(t == nullptr) return 0;

            if(layer == 0){
                if(t->color == RED) return -1;   //root must be black    
            }

            if(t->color == RED && (getColor(t->right) == RED || getColor(t->left) == RED)){
                return -1;
            }

            int num_back_l = check(t->left, layer+1);
            int num_back_r = check(t->right, layer+1);
            if(num_back_l == -1 || num_back_r == -1 || num_back_r != num_back_l){
                return -1;
            }

            return num_back_r + (t->color == BLACK);
        }
    private:
        static Node null_node;

        using BinarySearchTreeTemplate<Comparable, Node>::root;
        using BinarySearchTreeTemplate<Comparable, Node>::size_current;
        
        enum COLOR{RED, BLACK};

        void doInsert(const Comparable& item);                                          //插入,复写
        inline void handleReorient(Node* X, Node* P, Node* GP, Node* GGP);              //处理X
        inline void singleRightRotation(Node* node, Node* parent);
        inline void singleLeftRotation(Node* node, Node* parent);

        Node* doRemove(const Comparable& item);
        
        inline bool getColor(Node *node);
};

template<typename Comparable>
void RedBlackTree<Comparable>::insert(const Comparable& item){
    size_current++;
    doInsert(item);
}

template<typename Comparable>
void RedBlackTree<Comparable>::doInsert(const Comparable& item){
    if(root == nullptr){
        root = new Node;
        root->color = BLACK;
        root->element = item;
        return;
    }
    
    //避免边界问题的“根”节点，真正的根节点作为其右子节点
    Node v_root;
    v_root.right = root;
    v_root.color = BLACK;

    //记录自顶向下时的节点
    Node* X = root, *P = &v_root, *GP = &v_root, *GGP = &v_root;
    
    //向下，并消除路径上某个节点的两个子节点同为红色的情况
    while(X != nullptr){
        //消除
        if(X->left != nullptr && X->right != nullptr && 
           X->left->color == RED && X->right->color == RED){
            handleReorient(X, P, GP, GGP);
            v_root.right->color = BLACK;   //保证根为黑色
        }

        //向下
        GGP = GP; GP = P; P = X;
        if(item < X->element){
            X = X->left;
        }else{
            X = X->right;
        }
    }

    Node* new_node = new Node;
    new_node->color = RED;
    new_node->element = item;
    if(item < P->element) P->left = new_node;
    else P->right = new_node;

    handleReorient(new_node, P, GP, GGP);
    v_root.right->color = BLACK;

    root = v_root.right;
}

template<typename Comparable>
void RedBlackTree<Comparable>::handleReorient(Node* X, Node* P, Node* GP, Node* GGP){
    //颜色反转
    X->color = RED;
    if(X->left) X->left->color = BLACK;
    if(X->right) X->right->color = BLACK;
    
    //检查并消除X变为红色后与P同为红色的情况
    if(P->color == RED){
        GP->color = RED;   
        //进行旋转操作
        if(P == GP->left){
            if(X == P->left){
                singleRightRotation(GP,GGP);
                P->color = BLACK;
            }else{
                singleLeftRotation(P,GP);
                singleRightRotation(GP, GGP);
                X->color = BLACK;
            }
        }else{
            if(X == P->right){
                singleLeftRotation(GP,GGP);
                P->color = BLACK;
            }else{
                singleRightRotation(P, GP);
                singleLeftRotation(GP,GGP);
                X->color = BLACK;
            }
        }
    }
}

template<typename Comparable>
void RedBlackTree<Comparable>::singleRightRotation(Node* node, Node* parent){
    Node* l = node->left;
    Node* r = node->right;
    node->left = l->right;
    l->right = node;

    //重新连接到树上
    if(node == parent->left) parent->left = l;
    else parent->right = l;
}

template<typename Comparable>
void RedBlackTree<Comparable>::singleLeftRotation(Node* node, Node* parent){
    Node* l = node->left;
    Node* r = node->right;
    node->right = r->left;
    r->left = node;
    
    //重新连接到树上
    if(node == parent->left) parent->left = r;
    else parent->right = r;
}

template<typename Comparable>
void RedBlackTree<Comparable>::remove(const Comparable& x){
    if(contain(x)){
        size_current--;
        root = doRemove(x);
    }
}

//--------------------------------------------------------------
template<typename Comparable>
typename RedBlackTree<Comparable>::Node* RedBlackTree<Comparable>::doRemove(const Comparable& item){
    Node v_root;
    v_root.color = RED;
    v_root.right = root;
    v_root.left = nullptr;
 
    Node *X = root, *P = &v_root, *GP = &v_root, *GGP = &v_root, *S = v_root.left;
    enum DIRECTION{LEFT = 0, RIGHT = 1, EQUAL};
    int direction = RIGHT;
    const Comparable* item_to_delete = &item;

    while(true){
        if(X->element == *item_to_delete) direction = EQUAL;
        else direction = *item_to_delete < X->element ? LEFT : RIGHT;
        
        //X两个子节点都为黑色的情况
        if(getColor(X->left) == BLACK && getColor(X->right) == BLACK){
            X->color = RED;
            P->color = BLACK;
            if(S != nullptr){    
                if(getColor(S->right) == RED){
                    if(X == P->left){
                        singleLeftRotation(P, GP);
                    }else{
                        singleLeftRotation(S,P);
                        singleRightRotation(P,GP);
                    }
                }else if(getColor(S->left) == RED){
                    if(X == P->left){
                        singleRightRotation(S, P);
                        singleLeftRotation(P, GP);
                    }else{
                        singleRightRotation(P, GP);
                    }
                }else{
                    S->color = RED;
                }
            }
        }else{
            //X有红色的子节点
            if(getColor(X->left) == RED && getColor(X->right) == RED){
                //两个子节点都是红色
                X->color = BLACK;
            }else if(getColor(X->right) == RED){
                //右子节点为红色
                X->right->color = BLACK;
                X->color = RED;
                S = X->right->right;
                singleLeftRotation(X, P);
                GP = P;
                P = P->right;
            }else{
                //左
                X->left->color = BLACK;
                X->color = RED;
                S = X->left->left;
                singleRightRotation(X, P);
                GP = P;
                P = P->left;
            }
        }

        if(direction == EQUAL){
            if(X->left != nullptr && X->right != nullptr){
                Comparable& min_in_right = findMin(X->right)->element;
                X->element = min_in_right;       //copy
                item_to_delete = &min_in_right;  //改变要删除的目标
            }else if(X->left != nullptr){
                if(X == P->left) P->left = X->left;
                else P->right = X->left;
                return v_root.right;
            }else if(X->right != nullptr){
                if(X == P->left) P->left = X->right;
                return v_root.right;
            }else{
                if(X == P->left) P->left = nullptr;
                else P->right = nullptr;

                delete X;
                return v_root.right;
            }
        }else{
            GGP = GP;
            GP = P;
            P = X;
            if(direction == LEFT){
                S = X->right;
                X = X->left;
            }else{
                S = X->left;
                X = X->right;
            }
        }
    }

    //不应在此被结束
    return v_root.right;
}

template<typename Comparable>
bool RedBlackTree<Comparable>::getColor(Node* node){
    return node == nullptr ? BLACK : node->color;
}
#endif