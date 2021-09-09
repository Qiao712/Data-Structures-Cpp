#ifndef __RED_BLACK_TREE__
#define __RED_BLACK_TREE__
#include "BinarySearchTree.hpp"

template<typename Comparable>
class RedBlackTree : private BinarySearchTree<Comparable>{
    public:
        using Node = typename BinarySearchTree<Comparable>::Node;           //使用父类的Node，带有height
        
        using BinarySearchTree<Comparable>::findMin;                            //返回最小元素
        using BinarySearchTree<Comparable>::findMax;                            //返回最大元素
        using BinarySearchTree<Comparable>::contains;                           //是否包含该元素
        using BinarySearchTree<Comparable>::empty;                              //是否为空
        using BinarySearchTree<Comparable>::size;                               //返回大小
        using BinarySearchTree<Comparable>::clear;
        void insert(const Comparable& x);                                   //插入-接口,复写
        void remove(const Comparable& x);                                   //删除-接口，删除指定节点（非懒惰删除）
    private:


};

#endif