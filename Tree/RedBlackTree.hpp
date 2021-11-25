#ifndef __RED_BLACK_TREE__
#define __RED_BLACK_TREE__
#include "BinarySearchTree.hpp"

template<typename Comparable>
class RedBlackTree{
    enum{RED = 1, BLACK = 0};
    using Color = bool;

    struct Node{
        Comparable element;
        Node* left;
        Node* right;
        Node* parent;
        Color color;
    };

    using Helper = BinaryTreeHelper<Node>;
public:
    RedBlackTree() : dump(new Node){
        dump->color = BLACK;
        dump->left = dump;
        dump->right = dump;
        dump->parent = dump;
        root = dump;
    }

    const Comparable&       findMin() const { return Helper::doFindMin(root, dump)->element; }
    const Comparable&       findMax() const { return Helper::doFindMax(root, dump)->element; }
    bool                    contain(const Comparable& x) const { return Helper::doFind(x, root, dump) != dump; }
    bool                    empty() const { return size_current == 0; }
    size_t                  size() const { return size_current; }
    void                    clear() { size_current = 0; Helper::doClear(root, dump); }

    size_t insert(const Comparable& x);
    size_t remove(const Comparable& x);

    RedBlackTree& operator=(const RedBlackTree& rhs);

    void _debug(){
        cout<<size()<<" nodes"<<endl;
        if(_check() == -1) cout<<"Not A Red-Black Tree"<<endl;
        print(root);
    }
    bool _check(){
        return _doCheck(root) != -1;
    }
    void print(Node* t, int layer = 0){
        cout<<t->element<<'('<<t->color<<')'<<endl;

        if(t->left != dump){
            for(int i = 0; i<=layer; i++) cout<<"  ";
            cout<<"L:";
            print(t->left, layer+1);   
        }

        if(t->right != dump){
            for(int i = 0; i<=layer; i++) cout<<"  ";
            cout<<"R:";
            print(t->right, layer+1);
        }
    }
    //check whether the tree conform the Black-Red Tree
    int _doCheck(Node* t, int layer = 0){
        if(t == dump) return 0;

        if(layer == 0 && t->color == RED) return -1;   //root must be black    

        if(t->color == RED && (t->right->color == RED || t->left->color == RED)) return -1;

        int num_back_l = _doCheck(t->left, layer+1);
        int num_back_r = _doCheck(t->right, layer+1);
        if(num_back_l == -1 || num_back_r == -1 || num_back_r != num_back_l) return -1;
        return num_back_r + (t->color == BLACK);
    }
private:
    Node*   doInsert(const Comparable& x, Node* t);
    void    doRemove(const Comparable& x);
    void    transplant(Node* u, Node* v);       //用v替代u
    Node*   insertFixup(Node* t);
    void    deleteFixup(Node* t);
    void    leftRotate(Node* t);
    void    rightRotate(Node* t);

    Node*  dump;
    size_t size_current = 0;
    Node*  root;
};

template<typename Comparable>
size_t RedBlackTree<Comparable>::insert(const Comparable& x){
    size_current++;
    root = doInsert(x, root);
    return size_current;
}

template<typename Comparable>
size_t RedBlackTree<Comparable>::remove(const Comparable& x){
    doRemove(x);
    return size_current;
}

template<typename Comparable>
typename RedBlackTree<Comparable>::Node* RedBlackTree<Comparable>::doInsert(const Comparable& x, Node* t){
    Node* new_node = new Node;
    new_node->element = x;
    new_node->left = dump;
    new_node->right = dump;
    new_node->parent = dump;
    new_node->color = RED;

    Node* p = dump;
    while(t != dump){
        p = t;
        if(x < t->element){
            t = t->left;
        }else{
            t = t->right;
        }
    }
    
    if(p == dump){
        new_node->color = BLACK;
        return new_node;
    }

    new_node->parent = p;
    if(x < p->element) p->left = new_node;
    else p->right = new_node;
    return insertFixup(new_node);         //自底向上地修复
}

template<typename Comparable>
typename RedBlackTree<Comparable>::Node* RedBlackTree<Comparable>::insertFixup(Node* t){
    //根的虚拟的父节点
    Node parent_root;
    parent_root.color = BLACK;
    parent_root.left = root;
    parent_root.right = dump;
    root->parent = &parent_root;

    while(t->parent->color == RED){
        Node* tp = t->parent;
        Node* tpp = t->parent->parent;
        if(tp->element < tpp->element){
            if(tpp->right->color == RED){
                //case1
                tpp->right->color = BLACK;
                tp->color = BLACK;
                t = tpp;
            }else{
                if( !(t->element < tp->element) ){
                    //case2->case3
                    leftRotate(tp);
                }
                //case3
                t = tpp->left;
                t->left->color = BLACK;
                rightRotate(tpp);
            }
        }else{
            //对称的情况
            if(tpp->left->color == RED){
                tpp->left->color = BLACK;
                tp->color = BLACK;
                t = tpp;
            }else{
                if(t->element < tp->element){
                    rightRotate(tp);
                }
                t = tpp->right;
                t->right->color = BLACK;
                leftRotate(tpp);
            }
        }

        t->color = RED;
    }

    //恢复根
    Node* new_root = parent_root.left;
    new_root->color = BLACK;
    new_root->parent = dump;
    return new_root;
}

template<typename Comparable>
void RedBlackTree<Comparable>::doRemove(const Comparable& x){
    Node* dlt = const_cast<Node*>(Helper::doFind(x, root, dump));
    if(dlt == dump) return; //not found

    size_current--;

    Color original_color = dlt->color;  //实际被删除的节点的颜色
    Node* original_place;               //实际被删除的节点的位置上新的节点， 需要从此开始修复
    if(dlt->left == dump){
        original_place = dlt->right;
        transplant(dlt, dlt->right);
        delete dlt;
    }else if(dlt->right == dump){
        original_place = dlt->left;
        transplant(dlt, dlt->left);
        delete dlt;
    }else{
        Node* dlt2 = const_cast<Node*>(Helper::doFindMin(dlt->right, dump));
        original_color = dlt2->color;
        
        //删除dlt2, 分两种情况（是其父节点的左子树的情况，是其父节点的右子树的情况）
        if(dlt2->parent == dlt) transplant(dlt->right, dlt2->right);
        else transplant(dlt2->parent->left, dlt2->right);
        original_place = dlt2->right;

        dlt->element = std::move(dlt2->element);
        delete dlt2;
    }
    
    //若被删除的节点的颜色为黑色，则需修复
    if(original_color == BLACK){
        deleteFixup(original_place);
    }
}

template<typename Comparable>
void RedBlackTree<Comparable>::transplant(Node* u, Node* v){
    Node* p = u->parent;
    v->parent = p;          //当v为空节点时也要修改其p，否则会删除的fixup会出错
    if(p == dump){
        //u为根
        root = v;
    }else{
        if(p->left == u) p->left = v;
        else p->right = v;
    }
}


template<typename Comparable>
void RedBlackTree<Comparable>::deleteFixup(Node* t){
    //上滤带额外黑色的节点，若t为红黑色，则直接去除红色使其成为黑色节点
    Node* sibling, *parent;
    while(t != root && t->color == BLACK){
        parent = t->parent;
        if(t == parent->left){
            sibling = parent->right;

            if(sibling->color == RED){
                sibling->color = BLACK;
                parent->color = RED;
                leftRotate(parent);             //case1 -> case2, case3, case4
                sibling = parent->right;
            }

            if(sibling->left->color == BLACK && sibling->right->color == BLACK){
                //case 2
                sibling->color = RED;
                t = parent;
            }else{
                if(sibling->right->color == BLACK /* && sibling->left->color == RED*/){
                    //case 3 -> case 4
                    sibling->color = RED;
                    sibling->left->color = BLACK;
                    rightRotate(sibling);
                    sibling = parent->right;
                }
                
                //case 4
                sibling->right->color = BLACK;
                sibling->color = parent->color;
                parent->color = BLACK;
                leftRotate(parent);
                
                t = root;
            }
        }else{
            sibling = parent->left;

            if(sibling->color == RED){
                sibling->color = BLACK;
                parent->color = RED;
                rightRotate(parent);             //case1 -> case2, case3, case4
                sibling = parent->left;
            }

            if(sibling->right->color == BLACK && sibling->left->color == BLACK){
                //case 2
                sibling->color = RED;
                t = parent;
            }else{
                if(sibling->left->color == BLACK /* && sibling->right->color == RED*/){
                    //case 3 -> case 4
                    sibling->color = RED;
                    sibling->right->color = BLACK;
                    leftRotate(sibling);
                    sibling = parent->left;
                }
                
                //case 4
                sibling->left->color = BLACK;
                sibling->color = parent->color;
                parent->color = BLACK;
                rightRotate(parent);
                
                t = root;
            }
        }
    }
    t->color = BLACK;   //红黑节点变为黑色
}

template<typename Comparable>
void RedBlackTree<Comparable>::leftRotate(Node* t){
    Node* tp = t->parent;
    Node* r = t->right;

    t->right = r->left;
    if(r->left != dump) r->left->parent = t;

    r->left = t;
    t->parent = r;

    r->parent = tp;
    if(tp == dump){
        root = r;
    }else{
        if(tp->left == t) tp->left = r;
        else tp->right = r;
    }
}

template<typename Comparable>
void RedBlackTree<Comparable>::rightRotate(Node* t){
    Node* tp = t->parent;
    Node* l = t->left;

    t->left = l->right;
    if(l->right != dump) l->right->parent = t;

    l->right = t;
    t->parent = l;
    
    l->parent = tp;
    if(tp == dump){
        root = l;
    }else{
        if(tp->left == t) tp->left = l;
        else tp->right = l; 
    }
}
#endif