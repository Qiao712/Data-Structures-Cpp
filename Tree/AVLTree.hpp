#ifndef __AVLTREE_TREE__ 
#define __AVLTREE_TREE__
#include "BinaryTreeHelper.hpp"

template<typename Comparable>
class AVLTree{
    struct Node{
        Comparable  element;
        Node*       right;
        Node*       left;
        size_t      height = 0;
    };

    using Helper = BinaryTreeHelper<Node>;
public:
    AVLTree() : dump(new Node) { 
        dump->right = dump;
        dump->left = dump;
        root = dump;
    }
    AVLTree(const AVLTree& rhs){
        size_current = rhs.size_current;
        root = Helper::doCopy(rhs.root, dump);
    }

    const Comparable&       findMin() const { return Helper::doFindMin(root, dump)->element; }
    const Comparable&       findMax() const { return Helper::doFindMax(root, dump)->element; }
    bool                    contain(const Comparable& x) const { return Helper::doFind(x, root, dump) != dump; }
    bool                    empty() const { return size_current == 0; }
    size_t                  size() const { return size_current; }
    void                    clear() { size_current = 0; Helper::doClear(root, dump); }

    size_t                  insert(const Comparable& x);
    size_t                  remove(const Comparable& x);     

    AVLTree&                operator=(const AVLTree& rhs);

    bool _check(){ return _check(root);}
    bool _check(Node* t){
        if(t == dump) return true;
        
        int diff = t->right->height - t->left->height;
        if(diff > 1) return false;

        if(_check(t->left) == false) return false;
        if(_check(t->right) == false) return false;
        return true;
    }
    bool _debug(){
        Helper::doPrint(root, dump, 0);
        return _check(root);
    }
private:
    Node*                   doInsert(const Comparable& x, Node* t); 
    Node*                   doRemove(const Comparable& x, Node* t);
    Node*                   doRemoveLeftest(Node* t, Node* x);   //删除找到t树中最小的节点，与x的值交换后删除
    Node*                   fixup(Node* t);

    Node*                   singleRightRotate(Node* t);
    Node*                   singleLeftRotate(Node* t);
    Node*                   doubleRightRotate(Node* t);
    Node*                   doubleLeftRotate(Node* t);

    Node*  dump;
    size_t size_current = 0;
    Node*  root;
};

template<typename Comparable>
size_t AVLTree<Comparable>::insert(const Comparable& x){
    root = doInsert(x, root);
    size_current++;
    return size_current;
}

template<typename Comparable>
size_t AVLTree<Comparable>::remove(const Comparable& x){
    root = doRemove(x, root);
    return size_current;
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::singleRightRotate(Node* t){
    Node* l = t->left;
    t->left = l->right;
    l->right = t;

    t->height = std::max(t->left->height, t->right->height) + 1;
    l->height = std::max(l->left->height, l->right->height) + 1;
    return l;
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::singleLeftRotate(Node* t){
    Node* r = t->right;
    t->right = r->left;
    r->left = t;

    t->height = std::max(t->left->height, t->right->height) + 1;
    r->height = std::max(r->left->height, r->right->height) + 1;
    return r;
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::doubleRightRotate(Node* t){
    Node* l = t->left;
    Node* lr = l->right;

    l->right = lr->left;
    t->left = lr->right;
    lr->left = l;
    lr->right = t;

    l->height = std::max(l->left->height, l->right->height) + 1;
    t->height = std::max(t->left->height, t->right->height) + 1;
    lr->height = std::max(lr->left->height, lr->right->height) + 1;
    return lr;
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::doubleLeftRotate(Node* t){
    Node* r = t->right;
    Node* rl = r->left;

    r->left = rl->right;
    t->right = rl->left;
    rl->right = r;
    rl->left = t;

    r->height = std::max(r->left->height, r->right->height) + 1;
    t->height = std::max(t->left->height, t->right->height) + 1;
    rl->height = std::max(rl->left->height, rl->right->height) + 1;
    return rl;
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::fixup(Node* t){
    if(t->left->height == t->right->height + 2){
        if(t->left->left->height > t->left->right->height) t = singleRightRotate(t);
        else t = doubleRightRotate(t);
    }else if(t->right->height == t->left->height + 2){
        if(t->right->right->height > t->right->left->height) t = singleLeftRotate(t);
        else t = doubleLeftRotate(t);
    }

    //update height
    t->height = std::max(t->left->height, t->right->height) + 1;
    return t;
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::doInsert(const Comparable& x, Node* t){
    if(t == dump){
        Node* new_node = new Node;
        new_node->left = dump;
        new_node->right = dump;
        new_node->element = x;
        new_node->height = 1;
        return new_node;
    }

    if(x < t->element){
        t->left = doInsert(x, t->left);
    }else{
        t->right = doInsert(x, t->right);
    }

    //fix up
    return fixup(t);
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::doRemove(const Comparable& x, Node* t){
    if(t == dump) return dump;

    if(x < t->element){
        t->left = doRemove(x, t->left);
    }else if(t->element < x){
        t->right = doRemove(x, t->right);
    }else{
        size_current--;

        if(t->right != dump){
            t->right = doRemoveLeftest(t->right, t);
        }else{
            Node* l = t->left;
            delete t;
            return l;
        }
    }

    return fixup(t);
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::doRemoveLeftest(Node* t, Node* x){
    if(t->left == dump){
        Node* r = t->right;
        x->element = std::move(t->element);
        delete t;
        return r;
    }else{
        t->left = doRemoveLeftest(t->left, x);
    }

    return fixup(t);
}

template<typename Comparable>
AVLTree<Comparable>& AVLTree<Comparable>::operator=(const AVLTree& rhs){
    if(this == &rhs){
        return *this;
    }
    clear();
    size_current = rhs.size_current;
    root = Helper::doCopy(rhs.root, dump);
    return *this;
}

#endif