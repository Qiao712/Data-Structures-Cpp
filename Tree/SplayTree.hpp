/*
 *  自顶向下伸展树， 比自底向上更易实现
*/
#ifndef __SPLAY_TREE__
#define __SPLAY_TREE__
#include "BinarySearchTree.hpp"

template <typename Comparable>
class SplayTree : private BinarySearchTree<Comparable>
{
    using Node = typename BinarySearchTree<Comparable>::Node;

public:
    using BinarySearchTree<Comparable>::empty;                         
    using BinarySearchTree<Comparable>::size;
    using BinarySearchTree<Comparable>::clear;

    const Comparable&    findMin(); //返回最小元素
    const Comparable&    findMax(); //返回最大元素
    bool                 contain(const Comparable &x); //复写contain，添加把被访问元素旋转上来的操作
    bool                 remove(const Comparable &x);  //复写remove，使用把目标元素旋转上来的方式进行
    void                 insert(const Comparable &x);  //SplayTree版本的插入
private:
    using BinarySearchTree<Comparable>::root;

    Node *splay(const Comparable &x, Node *at); //伸展，将元素x旋转到根上

    //Node *singleRotationLeft(Node *at); //单旋转
    //Node *singleRotationRight(Node *at);
};

template <typename Comparable>
bool SplayTree<Comparable>::contain(const Comparable &x)
{
    root = splay(x,root);

    if (root->element == x)
        return true;
    else
        return false;
}

template <typename Comparable>
void SplayTree<Comparable>::insert(const Comparable &x)
{
    if(root == nullptr){
        root = new Node;
        root->element = x;
        root->left = nullptr;
        root->right = nullptr;
        return;
    }
    root = splay(x, root);
    Node* new_node = new Node;
    new_node->element = x;

    //若干新根小于 等于（和搜索时保持一致） 插入元素
    if(root->element <= x){
        new_node->left = root;
        new_node->right = root->right;
        root->right = nullptr;
    }else{
        new_node->right = root;
        new_node->left = root->left;
        root->left = nullptr;
    }

    root = new_node;
}

//核心算法--伸展
template <typename Comparable>
typename SplayTree<Comparable>::Node *SplayTree<Comparable>::splay(const Comparable &x, Node *at)
{
    Node header;    //header->right 表示 L子树， header->left 表示 R子树
    Node* max_of_left = &header;
    Node* min_of_right = &header;

    //较简化的版本
    while(at->element != x){
        if(x < at->element){
            if(at->left == nullptr) break;
            Node* x = at;
            Node* y = at->left;
            x->left = nullptr;
            
            min_of_right->left = x;
            min_of_right = x;
            at = y;
        }else if(x > at->element){
            if(at->right == nullptr) break;
            Node* x = at;
            Node* y = at->right;
            x->right = nullptr;
            
            max_of_left->right = x;
            max_of_left = x;
            at = y;
        }
    }

    //最后组成一棵树
    max_of_left->right = at->left;
    min_of_right->left = at->right;
    at->left  = header.right;
    at->right = header.left;

    return at;
}

template <typename Comparable>
const Comparable &SplayTree<Comparable>::findMin()
{
    const Comparable &x = BinarySearchTree<Comparable>::findMin();
    
    //旋转上来
    bool t;
    int length;
    root = splay(x, root);

    return x;
}

template <typename Comparable>
const Comparable &SplayTree<Comparable>::findMax()
{
    const Comparable &x = BinarySearchTree<Comparable>::findMax();
    
    //旋转上来
    bool t;
    int length;
    root = splay(x, root);

    return x;
}

template <typename Comparable>
bool SplayTree<Comparable>::remove(const Comparable &x)
{
    if (!contain(x)) return false;

    BinarySearchTree<Comparable>::size_current--;

    Node *right = root->right;
    Node *left  = root->left;
    delete root;

    //没有左子树
    if (left == nullptr){
        root = right;
        return true;
    }

    root = left;                                    //成为新根
    findMax();                                                                    //找到最大值并旋转上来
    root->right = right;

    return true;
}

// template <typename Comparable>
// typename SplayTree<Comparable>::Node *SplayTree<Comparable>::singleRotationLeft(Node *at)
// {
//     Node *k = at->left;
//     at->left = k->right;
//     k->right = at;

//     return k;
// }

// template <typename Comparable>
// typename SplayTree<Comparable>::Node *SplayTree<Comparable>::singleRotationRight(Node *at)
// {
//     Node *k = at->right;
//     at->right = k->left;
//     k->left = at;

//     return k;
// }

#endif