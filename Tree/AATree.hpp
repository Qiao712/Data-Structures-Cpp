#ifndef __AA_TREE__
#define __AA_TREE__
#include "BinaryTree.hpp"

template<typename Comparable>
struct NodeWithLevel{
    Comparable element;
    NodeWithLevel* right;
    NodeWithLevel* left;
    unsigned int level = 0;
};

template<typename Comparable> 
class AATree : private BinaryTree<Comparable, NodeWithLevel<Comparable>>{
    public:
        using Node = NodeWithLevel<Comparable>;

        AATree():NIL(new Node()){
            NIL->left = NIL;
            NIL->right = NIL;
            NIL->level = 0;
            root = NIL;
        }
        ~AATree(){clear();}

        const Comparable&       findMin() const { return doFindMin(root, NIL)->element; }
        const Comparable&       findMax() const { return doFindMax(root, NIL)->element; }
        bool                    contain(const Comparable& x) const { return doFind(x, root, NIL) != NIL; }
        bool                    empty() const { return doCheckEmpty();}
        size_t                  size() const { return size_current; }
        void                    clear() { doClear(root, NIL); }
        void                    insert(const Comparable& x);
        void                    remove(const Comparable& x);

        bool debug(){
            print(root, 0, 0);
            return check(root);
        }
        void print(Node* t, int n, bool d){
            if(t == NIL){
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
        bool check(Node* t){
            if(t == NIL) return true;

            if(t->right != NIL && t->right->level == t->level && t->right->right->level == t->level){
                return false;
            }
            if(t->left != NIL && t->level == t->left->level){
                return false;
            }
            
            return check(t->right) && check(t->left);
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

        //共用的空节点
        Node* NIL;

        using BinaryTree<Comparable, Node>::doCheckEmpty;
        using BinaryTree<Comparable, Node>::doClear;
        using BinaryTree<Comparable, Node>::doCopy;
        using BinaryTree<Comparable, Node>::doFindMax;
        using BinaryTree<Comparable, Node>::doFindMin;
        using BinaryTree<Comparable, Node>::doFind;
        using BinaryTree<Comparable, Node>::root;
        using BinaryTree<Comparable, Node>::size_current;
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
    if(t == NIL){
        Node* new_node = new Node{x, NIL, NIL, 1};
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
    if(t == NIL){
        return NIL;
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
        return NIL;
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