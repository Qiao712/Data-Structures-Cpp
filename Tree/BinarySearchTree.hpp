/*
* 规定右子节点大于 等于 其父节点，左子节点小于其夫节点
* 重复加入：直接插入新节点，不计数
*/
#ifndef __BINARY_SEARCH_TREE__
#define __BINARY_SEARCH_TREE__

#include "BinaryTreeHelper.hpp"

template<typename Comparable>
class BinarySearchTree{
    struct Node{
        Comparable element;
        Node* right;
        Node* left;
    };
    
    using Helper = BinaryTreeHelper<Node>;
public:
    BinarySearchTree() = default;
    BinarySearchTree(const BinarySearchTree& rhs){
        size_current = rhs.size_current;
        root = Helper::doCopy(rhs.root, nullptr);
    }
    ~BinarySearchTree(){clear();}

    const Comparable&       findMin() const { return Helper::doFindMin(root, nullptr)->element; }
    const Comparable&       findMax() const { return Helper::doFindMax(root, nullptr)->element; }
    bool                    contain(const Comparable& x) const { return Helper::doFind(x, root, nullptr) != nullptr; }
    bool                    empty() const { return size_current == 0; }
    size_t                  size() const { return size_current; }
    void                    clear() { size_current = 0; Helper::doClear(root, nullptr); }

    size_t                  insert(const Comparable& x);
    size_t                  remove(const Comparable& x);     

    BinarySearchTree&       operator=(const BinarySearchTree& rhs);

    void                    _debug(){Helper::doPrint(root, nullptr, 0);}   
private:
    Node*                   doRemove(const Comparable& x, Node* t);
    Node*                   doInsert(const Comparable& x, Node* t);

    size_t size_current = 0;
    Node* root = nullptr;
};

template<typename Comparable>
size_t BinarySearchTree<Comparable>::insert(const Comparable& x){
    root = doInsert(x, root);
    size_current++;
    return size_current;
}

template<typename Comparable>
size_t BinarySearchTree<Comparable>::remove(const Comparable& x){
    root = doRemove(x, root);
    return size_current;
}

template<typename Comparable>
BinarySearchTree<Comparable>& BinarySearchTree<Comparable>::operator=(const BinarySearchTree& rhs){
    if(&rhs == this) return *this;
    clear();
    size_current = rhs.size_current;
    root = Helper::doCopy(rhs.root, nullptr);
    return *this;
}

template<typename Comparable>
typename BinarySearchTree<Comparable>::Node* BinarySearchTree<Comparable>::doInsert(const Comparable& x, Node* t){
    Node* node_new = new Node;
    node_new->element = x;
    node_new->left = nullptr;
    node_new->right = nullptr;

    if(t == nullptr){ 
        return node_new;
    }

    Node* node = t;
    while(true){
        if(x < node->element){
            if(node->left == nullptr){
                node->left = node_new;
                break;
            }else{
                node = node->left;
            }
        }else{
            if(node->right == nullptr){
                node->right = node_new;
                break;
            }else{
                node = node->right;
            }
        }
    }

    return t;
}

template<typename Comparable>
typename BinarySearchTree<Comparable>::Node* BinarySearchTree<Comparable>::doRemove(const Comparable& x, Node* t){    
    Node* last_ptr = nullptr;         //最终指向被删除目标节点的右子树中最小的节点 或 它自己
    Node* delete_ptr = nullptr;       //最终指向被要删除的目标节点
    Node* parent = nullptr;           //追踪last_ptr的父节点
    Node* cur = t;

    while(cur != nullptr){
        if(x < cur->element){
            parent = cur;
            cur = cur->left; 
        }else if(cur->element < x){
            parent = cur;
            cur = cur->right;
        }else{
            delete_ptr = cur;
            last_ptr = cur;
            
            //右子树不为空
            if(cur->right != nullptr){
                parent = cur;
                last_ptr = cur->right;
                while(last_ptr->left != nullptr){
                    parent = last_ptr;
                    last_ptr = last_ptr->left;
                }
                delete_ptr->element = std::move(last_ptr->element);
            }

            //删除last
            if(parent == nullptr){              //delete_ptr == last_ptr， last_ptr为根
                t = last_ptr->left;
            }else if(delete_ptr == last_ptr){   //delete_ptr == last_ptr 不为根
                if(last_ptr == parent->left) parent->left = last_ptr->left;
                else parent->right = last_ptr->left;
            }else{                              //delete_ptr 有右子树
                if(last_ptr == parent->left) parent->left = last_ptr->right;
                else parent->right = last_ptr->right;
            }
            delete last_ptr;
            size_current--;

            break;
        }
    }

    //返回新根
    return t;
}


#endif