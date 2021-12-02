#ifndef __AA_TREE__
#define __AA_TREE__
#include "BinaryTreeHelper.hpp"

template<typename Comparable> 
class AATree{
    struct Node{
        Comparable element;
        Node* right;
        Node* left;
        unsigned int level = 0;
    };

    using Helper = BinaryTreeHelper<Node>;
public:
    AATree():dump(new Node()){
        dump->left = dump;
        dump->right = dump;
        dump->level = 0;
        root = dump;
    }
    ~AATree(){
        clear();
        delete dump;
    }

    const Comparable&       findMin() const { return Helper::doFindMin(root, dump)->element; }
    const Comparable&       findMax() const { return Helper::doFindMax(root, dump)->element; }
    bool                    contain(const Comparable& x) const { return Helper::doFind(x, root, dump) != dump; }
    bool                    empty() const { return size_current == 0;}
    size_t                  size() const { return size_current; }
    void                    clear() { size_current = 0; Helper::doClear(root, dump); }

    void                    insert(const Comparable& x);
    void                    remove(const Comparable& x);

    bool _debug(){
        print(root, 0, 0);
        return _check(root);
    }
    void print(Node* t, int n, bool d){
        if(t == dump){
            return;
        }

        if(d == 1){
            for(int i = 0; i<n; i++) cout<<"  ";
            cout<<"L:"<<t->element<<' '<<t->level<<endl;
        }else{
            for(int i = 0; i<n; i++) cout<<"  ";
            cout<<"R:"<<t->element<<' '<<t->level<<endl;
        }
        print(t->left, n+1, 1);
        print(t->right, n+1, 0);
    }
    //检查是否符合AATree
    bool _check(Node* t){
        if(t == dump) return true;

        if(t->right != dump && t->right->level == t->level && t->right->right->level == t->level){
            return false;
        }
        if(t->left != dump && t->level == t->left->level){
            return false;
        }
        
        return _check(t->right) && _check(t->left);
    }
private:
    Node* doRemove(const Comparable& x, Node* t);
    //用于在递归过程中储存 最底层实际删除的节点 和 要删除的目标节点
    Node* last_ptr, *delete_ptr;

    Node* doInsert(const Comparable& x, Node* t);
    Node* skew(Node* x);
    Node* split(Node* x);
    Node* singleRightRotation(Node* x);
    Node* singleLeftRotation(Node* x);

    size_t size_current = 0;
    Node* root;
    Node* dump;        
};

template<typename Comparable>
void AATree<Comparable>::insert(const Comparable& x){
    size_current++;
    root = doInsert(x, root);
}

template<typename Comparable>
void AATree<Comparable>::remove(const Comparable& x){
    if(contain(x)){
        size_current--;
        last_ptr = nullptr;
        delete_ptr = nullptr;
        root = doRemove(x, root);
    }
}

template<typename Comparable>
typename AATree<Comparable>::Node* AATree<Comparable>::doInsert(const Comparable& x, Node* t){
    if(t == dump){
        Node* new_node = new Node{x, dump, dump, 1};
        return new_node;
    }

    if(x < t->element){
        t->left = doInsert(x, t->left);
    }else{
        t->right = doInsert(x, t->right);
    }

    t = skew(t);
    t = split(t);
    return t;
}

template<typename Comparable>
typename AATree<Comparable>::Node* AATree<Comparable>::doRemove(const Comparable& x, Node* t){
    if(t == dump){
        return dump;
    }

    //一直递归到树的最底层，并记录路径上找到的要被删除的目标的节点
    last_ptr = t;
    if(x < t->element){
        t->left = doRemove(x, t->left);
    }else{
        delete_ptr = t;
        t->right = doRemove(x, t->right);
    }

    //如果到达最底层
    if(t == last_ptr){
        if(delete_ptr != last_ptr)
            delete_ptr->element = last_ptr->element;
        delete t;
        return dump;
    }

    // if(t->left->level < t->level - 1){  //来自左边
    //     t->level--;                     //则需降低t
    //     if(t->right->level > t->level) t->right->level--;   //降低可能导致t比t->right低1层,需降低t->right
    //     t->right = skew(t->right);
    //     t->right->right = skew(t->right->right);
    //     t = split(t);
    //     t->right = split(t->right);
    // }else if(t->right->level < t->level - 1){    //来自右边
    //     t->level--;
    //     if(t->right->level > t->level) t->right->level--;
    //     t = skew(t);
    //     t->right = skew(t->right);
    //     t = split(t);
    // }
    //简化
    if(t->left->level < t->level - 1 || t->right->level < t->level - 1){  //来自左边
        t->level--;                     //则需降低t
        if(t->right->level > t->level) t->right->level--;   //降低可能导致t比t->right低1层,需降低t->right
        t = skew(t);
        t->right = skew(t->right);
        t->right->right = skew(t->right->right);
        t = split(t);
        t->right = split(t->right);
    }

    return t;
}

template<typename Comparable>
typename AATree<Comparable>::Node* AATree<Comparable>::skew(Node* x){
    if(x->left->level == x->level){
        x = singleRightRotation(x);
    }
    return x;
}

template<typename Comparable>
typename AATree<Comparable>::Node* AATree<Comparable>::split(Node* x){
    if(x->right->level == x->level && x->right->right->level == x->level){
        x = singleLeftRotation(x);
        x->level++;
    }
    return x;
}

template<typename Comparable>
typename AATree<Comparable>::Node* AATree<Comparable>::singleRightRotation(Node* x){
    Node* t = x->left;
    x->left = t->right;
    t->right = x;
    return t;
}

template<typename Comparable>
typename AATree<Comparable>::Node* AATree<Comparable>::singleLeftRotation(Node* x){
    Node* t = x->right;
    x->right = t->left;
    t->left = x;
    return t;
}
#endif