/*
 *  自底向上地旋转
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
private:
    using BinarySearchTree<Comparable>::root;

    Node *splay(const Comparable &x, Node *at, bool &left_or_right, int &length); //伸展，递归的，将元素x旋转到根上

    Node *doubleRightRotation(Node *at); //双旋转
    Node *doubleLeftRotation(Node *at);
    Node *singleRightRotation(Node *at); //单旋转
    Node *singleLeftRotation(Node *at);

    Node *zigZigLeftRotation(Node *at); //一字型旋转
    Node *zigZigRightRotation(Node *at);
    Node *zigZagLeftRotation(Node *at); //之字形旋转
    Node *zigZagRightRotation(Node *at);
};

template <typename Comparable>
bool SplayTree<Comparable>::contain(const Comparable &x)
{
    bool t;
    int length;
    root = splay(x, root, t, length);

    if (length == 0)
        return true;
    else
        return false;
}

//核心算法--伸展
//length储存该节点到x的路径长度
//left_or_right储存x是at的左子节点，还是右子节点。0表示left，1表示right.
template <typename Comparable>
typename SplayTree<Comparable>::Node *SplayTree<Comparable>::splay(const Comparable &x, Node *at, bool &left_or_right, int &length)
{
    static const bool LEFT = 0;
    static const bool RIGHT = 1;

    if (at == nullptr)
    {
        length = 2; //没有找到，路径长返回大于等于2的数，使旋转不会被执行
        return nullptr;
    }

    bool direct_next; //储存下一层

    if (x == at->element)
    {
        length = 0; //找到，返回自己到自己的路径长 0
        return at;
    }
    else if (x < at->element)
    {
        at->left = splay(x, at->left, direct_next, length);
        length++;
        left_or_right = LEFT; //记录该层方向
    }
    else
    {
        at->right = splay(x, at->right, direct_next, length);
        length++;
        left_or_right = RIGHT;
    }

    //根处的旋转情况
    if (length == 1 && at == root)
    {
        if (left_or_right == LEFT)
            at = singleRightRotation(at);
        if (left_or_right == RIGHT)
            at = singleLeftRotation(at);

        length = 0;
    }

    if (length == 2)
    {
        if (left_or_right == LEFT)
        {
            if (direct_next == LEFT)
                at = zigZigRightRotation(at);
            if (direct_next == RIGHT)
                at = zigZagRightRotation(at);
        }
        else
        {
            if (direct_next == RIGHT)
                at = zigZigLeftRotation(at);
            if (direct_next == LEFT)
                at = zigZagLeftRotation(at);
        }

        length = 0;
    }

    return at;
}

template <typename Comparable>
const Comparable &SplayTree<Comparable>::findMin()
{
    const Comparable &x = BinarySearchTree<Comparable>::findMin();
    
    //旋转上来
    bool t;
    int length;
    root = splay(x, root, t, length);

    return x;
}

template <typename Comparable>
const Comparable &SplayTree<Comparable>::findMax()
{
    const Comparable &x = BinarySearchTree<Comparable>::findMax();
    
    //旋转上来
    bool t;
    int length;
    root = splay(x, root, t, length);

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

template <typename Comparable>
typename SplayTree<Comparable>::Node *SplayTree<Comparable>::singleRightRotation(Node *at)
{
    Node *k = at->left;
    at->left = k->right;
    k->right = at;

    return k;
}

template <typename Comparable>
typename SplayTree<Comparable>::Node *SplayTree<Comparable>::singleLeftRotation(Node *at)
{
    Node *k = at->right;
    at->right = k->left;
    k->left = at;

    return k;
}

template <typename Comparable>
typename SplayTree<Comparable>::Node *SplayTree<Comparable>::doubleRightRotation(Node *at)
{
    at->left = singleLeftRotation(at->left); //旋转到外侧
    at = singleRightRotation(at);

    return at;
}

template <typename Comparable>
typename SplayTree<Comparable>::Node *SplayTree<Comparable>::doubleLeftRotation(Node *at)
{
    at->right = singleRightRotation(at->right); //旋转到外侧
    at = singleLeftRotation(at);

    return at;
}

template <typename Comparable>
typename SplayTree<Comparable>::Node *SplayTree<Comparable>::zigZagRightRotation(Node *at)
{
    return doubleRightRotation(at);
}

template <typename Comparable>
typename SplayTree<Comparable>::Node *SplayTree<Comparable>::zigZagLeftRotation(Node *at)
{
    return doubleLeftRotation(at);
}

template <typename Comparable>
typename SplayTree<Comparable>::Node *SplayTree<Comparable>::zigZigRightRotation(Node *at)
{
    Node *g = at;
    Node *p = g->left;
    Node *x = p->left;

    p->left = x->right;
    x->right = p;

    g->left = p->right;
    p->right = g;

    return x;
}

template <typename Comparable>
typename SplayTree<Comparable>::Node *SplayTree<Comparable>::zigZigLeftRotation(Node *at)
{
    Node *g = at;
    Node *p = g->right;
    Node *x = p->right;

    p->right = x->left;
    x->left = p;

    g->right = p->left;
    p->left = g;

    return x;
}

#endif