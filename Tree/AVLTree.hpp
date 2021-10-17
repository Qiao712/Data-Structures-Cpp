#ifndef  __AVLTREE_TREE__ 
#define __AVLTREE_TREE__                                                    //宏标记 启用BinarySearchTree的Node中height的定义
#include "BinarySearchTree.hpp"                                             //引用专门用来继承的带有高度信息的二叉查找树

template<typename Comparable>
struct NodeWithHeight{
    Comparable          element;
    NodeWithHeight*     right = nullptr;
    NodeWithHeight*     left  = nullptr;
    unsigned int height = 0;
};

template<typename Comparable>
class AVLTree : private BinarySearchTreeTemplate<Comparable, NodeWithHeight<Comparable>>{
    public:
        using Node = NodeWithHeight<Comparable>;           //使用父类的Node，带有height
        
        using BinarySearchTreeTemplate<Comparable, Node>::findMin;                            //返回最小元素
        using BinarySearchTreeTemplate<Comparable, Node>::findMax;                            //返回最大元素
        using BinarySearchTreeTemplate<Comparable, Node>::contain;                           //是否包含该元素
        using BinarySearchTreeTemplate<Comparable, Node>::empty;                              //是否为空
        using BinarySearchTreeTemplate<Comparable, Node>::size;                               //返回大小
        using BinarySearchTreeTemplate<Comparable, Node>::clear;
        void insert(const Comparable& x);                                   //插入-接口,复写
        void remove(const Comparable& x);                                   //删除-接口，删除指定节点（非懒惰删除）
    private:
        using BinarySearchTreeTemplate<Comparable, Node>::root;
        using BinarySearchTreeTemplate<Comparable, Node>::size_current;

        int   getHeight(Node* t);
        Node* insert(const Comparable& x, Node* t);                         //插入,复写
        Node* singleRightRotate(Node* root);                               //单旋转，左边高
        Node* singleLeftRotate(Node* root);
        Node* doubleRightRotate(Node* root);
        Node* doubleLeftRotate(Node* root);

        Node* remove(const Comparable& x, Node* t);                         //递归地删除，并旋转
        void  updateHeight(Node *t, unsigned height);                       //更新高度,将节点t的高度更新为height，并影响子节点
};

//核心算法-----------------------------------------------------------------------
template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::insert(const Comparable& x, Node *t){
    if(t == nullptr){
        t = new Node;
        t->element = x;
    }else if(x < t->element){
        t->left = insert(x, t->left);
        //旋转判定
        if(getHeight(t->left) - getHeight(t->right) == 2){
            if(x < t->left->element)
                t = singleRightRotate(t);      //左边外侧
            else
                t = doubleRightRotate(t);      //左边内侧
        }
    }else{
        t->right = insert(x, t->right);
        //旋转判定
        if(getHeight(t->right) - getHeight(t->left) == 2){
            if(x < t->right->element)
                t = doubleLeftRotate(t);      //右边外侧
            else
                t = singleLeftRotate(t);      //右边内测
        }
    }

    t->height = std::max(getHeight(t->left), getHeight(t->right)) + 1;

    return t;
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::remove(const Comparable& x, Node* root){
    if(root == nullptr) return nullptr;

    if(x < root->element){
        root->left = remove(x, root->left);
    }else if(x > root->element){
        root->right = remove(x, root->right);
    }else{
        if(root->left && root->right){  //有两个子节点
            Comparable max_in_left = findMax(root->left)->element;
            root->element = max_in_left;
            root->left    = remove(max_in_left, root->left);
        }else if(root->left){
            Node* left = root->left;
            delete root;
            root = left;
        }else{
            Node* right = root->right;
            delete root;
            root = right;
        }
    }
    
    //***易忽略***
    if(root == nullptr) return root;

    //更新高度,要在旋转前更新
    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;

    //旋转
    if(getHeight(root->left) - getHeight(root->right) == 2){                //左旋转
        if(getHeight(root->left->left) >= getHeight(root->left->right)){    //单旋转
            root = singleRightRotate(root);
        }else{                                                              //双旋转
            root = doubleRightRotate(root);
        }
    }else if(getHeight(root->right) - getHeight(root->left) == 2){          //右旋转
        if(getHeight(root->right->right) >= getHeight(root->right->left)){  //单旋转
            root = singleLeftRotate(root);
        }else{                                                              //双旋转
            root = doubleLeftRotate(root);
        }
    }

    return root;
}

template<typename Comparable>
int AVLTree<Comparable>::getHeight(Node* t){
    if(t == nullptr)
        return -1;
    else
        return t->height;
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::singleRightRotate(Node *root){
    Node* const k = root->left;
    root->left = k->right;
    k->right = root;

    //更新高度
    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;
    k->height = std::max(getHeight(k->left), getHeight(k->right)) + 1;
    
    return k;
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::singleLeftRotate(Node *root){
    Node* const k = root->right;
    root->right = k->left;
    k->left = root;

    //更新高度
    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;
    k->height = std::max(getHeight(k->left), getHeight(k->right)) + 1;

    return k;
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::doubleRightRotate(Node *root){
    // root->left = singleLeftRotate(root->left);   //旋转到外侧
    // root       = singleRightRotate(root);
    Node* const k3 = root;
    Node* const k1 = root->left;
    Node* const k2 = k1->right;
    
    k1->right = k2->left;
    k3->left  = k2->right;
    
    k2->left  = k1;
    k2->right = k3;

    k1->height = std::max(getHeight(k1->left), getHeight(k1->right)) + 1;
    k3->height = std::max(getHeight(k3->left), getHeight(k3->right)) + 1;
    k2->height = std::max(k1->height, k3->height) + 1;
    
    return k2;
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::doubleLeftRotate(Node *root){
    // root->right = singleRightRotate(root->right);  //旋转到外侧
    // root        = singleLeftRotate(root);
    Node* const k3 = root;
    Node* const k1 = root->right;
    Node* const k2 = k1->left;

    k1->left  = k2->right;
    k3->right = k2->left;

    k2->right = k1;
    k2->left  = k3;

    k1->height = std::max(getHeight(k1->left), getHeight(k1->right)) + 1;
    k3->height = std::max(getHeight(k3->left), getHeight(k3->right)) + 1;
    k2->height = std::max(k1->height, k3->height) + 1;

    return k2;
}

//接口-------------------------------
template<typename Comparable>
void AVLTree<Comparable>::insert(const Comparable& x){
    size_current++;
    root = insert(x, root);
}

template<typename Comparable>
void AVLTree<Comparable>::remove(const Comparable& x){
    //保证存在，因为要对高度进行更新，不存在可不行
    if(contain(x)){
        size_current--;
        root = remove(x, root);
    }
}

#endif