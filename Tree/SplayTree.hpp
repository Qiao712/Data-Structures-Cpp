/*
 *  自顶向下伸展树， 比自底向上更易实现
 *  不允许插入重复元素
*/
#ifndef __SPLAY_TREE__
#define __SPLAY_TREE__
#include "BinaryTreeHelper.hpp"

template <typename Comparable>
class SplayTree
{
    struct Node{
        Comparable element;
        Node* left = nullptr;
        Node* right = nullptr;
    };

    using Helper = BinaryTreeHelper<Node>;
public:
    SplayTree() = default;
    SplayTree(const SplayTree& rhs){
        size_current = rhs.size_current;
        root = Helper::doCopy(rhs.root, nullptr);
    }

    const Comparable&       findMin();
    const Comparable&       findMax();
    bool                    contain(const Comparable& x);
    bool                    empty() const { return size_current == 0; }
    size_t                  size() const { return size_current; }
    void                    clear() { size_current = 0; Helper::doClear(root, nullptr); }

    size_t                  insert(const Comparable& x);
    size_t                  remove(const Comparable& x);     

    SplayTree&              operator=(const SplayTree& rhs);

    void _debug(){
        Helper::doPrint(root, nullptr, 0);
    }
private:
    Node* doInsert(const Comparable& x, Node* t);
    Node* splay(const Comparable& x, Node* t); //伸展，将元素x旋转到根上

    size_t size_current = 0;
    Node*  root = nullptr;
    Node *singleLeftRotate(Node *t); //单旋转
    Node *singleRightRotate(Node *t);
};

template <typename Comparable>
bool SplayTree<Comparable>::contain(const Comparable &x)
{
    if(root == nullptr) return false;

    root = splay(x,root);

    return (root->element == x);
}

template <typename Comparable>
SplayTree<Comparable>& SplayTree<Comparable>::operator=(const SplayTree& rhs){
    if(this == &rhs) return *this;
    clear();
    size_current = rhs.size_current;
    root = Helper::doCopy(rhs.root, nullptr);
    return *this;
}

template <typename Comparable>
typename SplayTree<Comparable>::Node* SplayTree<Comparable>::splay(const Comparable& x, Node* t)
{
    Node head;
    Node* max_in_left = &head;
    Node* min_in_right = &head;

    while(true){
        if(x < t->element){
            if(t->left == nullptr) break;

            min_in_right->left = t;
            min_in_right = t;
            t = t->left;
            min_in_right->left = nullptr;
        }else if(t->element < x){
            if(t->right == nullptr) break;

            max_in_left->right = t;
            max_in_left = t;
            t = t->right;
            max_in_left->right = nullptr;
        }else{
            break;
        }
    }

    min_in_right->left = t->right;
    max_in_left->right = t->left;
    t->left = head.right;
    t->right = head.left;
    return t;
}

template <typename Comparable>
size_t SplayTree<Comparable>::insert(const Comparable& x){
    root = doInsert(x, root);
    return size_current;
}

template <typename Comparable>
typename SplayTree<Comparable>::Node* SplayTree<Comparable>::doInsert(const Comparable& x, Node* t){
    Node* node_new = new Node;
    node_new->element = x;
    node_new->left = nullptr;
    node_new->right = nullptr;

    size_current++;

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
        }else if(node->element < x){
            if(node->right == nullptr){
                node->right = node_new;
                break;
            }else{
                node = node->right;
            }
        }else{
            //不允许出现重复元素，否则splay会出现问题
            size_current--;
            break;
        }
    }

    return t;
}

template <typename Comparable>
const Comparable &SplayTree<Comparable>::findMin()
{
    const Comparable &x = Helper::doFindMin(root, nullptr)->element;
    
    //旋转上来
    root = splay(x, root);
    return x;
}

template <typename Comparable>
const Comparable &SplayTree<Comparable>::findMax()
{
    const Comparable &x = Helper::doFindMax(root, nullptr)->element;
    
    //旋转上来
    root = splay(x, root);
    return x;
}

template <typename Comparable>
size_t SplayTree<Comparable>::remove(const Comparable &x)
{
    if(root != nullptr){
        root = splay(x, root);
        if(root->element == x){
            Node* new_root;
            if(root->left == nullptr){
                new_root = root->right;
            }else{
                new_root = splay(x, root->left);  //使得root->left中最大的节点伸展到根，成为新的根节点（进而无右子节点）
                new_root->right = root->right;
            }
            delete root;
            root = new_root;
            size_current--;
        }
    }
    
    return size_current;
}

#endif