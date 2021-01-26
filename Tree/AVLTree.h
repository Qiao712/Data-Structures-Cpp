#define __AVLTREE_NODE__                                                    //宏标记 启用BinarySearchTree的Node中height的定义
#include"BinarySearchTree_WithHeightInformation.h"                          //引用专门用来继承的带有高度信息的二叉查找树
#define BinarySearchTree BinarySearchTree_WithHeightInformation

template<typename Comparable>
class AVLTree : public BinarySearchTree<Comparable>{
    public:
        using Node = typename BinarySearchTree<Comparable>::Node;           //使用父类的Node，带有height

        void insert(const Comparable& x);                                   //插入-接口,复写
        void remove(const Comparable& x);                                   //删除-接口，删除指定节点（非懒惰删除）
    private:
        int   getHeight(Node* t);
        Node* insert(const Comparable& x, Node* t);                         //插入,复写
        Node* singleRotationLeft(Node* root);                               //单旋转，左边高
        Node* singleRotationRight(Node* root);
        Node* doubleRotationLeft(Node* root);
        Node* doubleRotationRight(Node* root);

        Node* remove(const Comparable& x, Node* t);                         //递归地删除，并旋转
        void  updateHeight(Node *t, unsigned height);                       //更新高度,将节点t的高度更新为height，并影响子节点
};



#ifndef  __AVLTREE_TREE__ 
#define __AVLTREE_TREE__

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
                t = singleRotationLeft(t);      //左边外侧
            else
                t = doubleRotationLeft(t);      //左边内侧
        }
    }else{
        t->right = insert(x, t->right);
        //旋转判定
        if(getHeight(t->right) - getHeight(t->left) == 2){
            if(x < t->right->element)
                t = doubleRotationRight(t);      //右边外侧
            else
                t = singleRotationRight(t);      //右边内测
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
            Comparable max_in_left = BinarySearchTree<Comparable>::findMax(root->left)->element;
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
            root = singleRotationLeft(root);
        }else{                                                              //双旋转
            root = doubleRotationLeft(root);
        }
    }else if(getHeight(root->right) - getHeight(root->left) == 2){          //右旋转
        if(getHeight(root->right->right) >= getHeight(root->right->left)){  //单旋转
            root = singleRotationRight(root);
        }else{                                                              //双旋转
            root = doubleRotationRight(root);
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

// template<typename Comparable>
// void AVLTree<Comparable>::updateHeight(Node *t, unsigned height){
    
// }

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::singleRotationLeft(Node *root){
    Node* const k = root->left;
    root->left = k->right;
    k->right = root;

    //更新高度
    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;
    k->height = std::max(getHeight(k->left), getHeight(k->right)) + 1;
    
    return k;
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::singleRotationRight(Node *root){
    Node* const k = root->right;
    root->right = k->left;
    k->left = root;

    //更新高度
    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;
    k->height = std::max(getHeight(k->left), getHeight(k->right)) + 1;

    return k;
}

template<typename Comparable>
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::doubleRotationLeft(Node *root){
    // root->left = singleRotationRight(root->left);   //旋转到外侧
    // root       = singleRotationLeft(root);
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
typename AVLTree<Comparable>::Node* AVLTree<Comparable>::doubleRotationRight(Node *root){
    // root->right = singleRotationLeft(root->right);  //旋转到外侧
    // root        = singleRotationRight(root);
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
    BinarySearchTree<Comparable>::size_current++;
    BinarySearchTree<Comparable>::root = insert(x, BinarySearchTree<Comparable>::root);
}

template<typename Comparable>
void AVLTree<Comparable>::remove(const Comparable& x){
    //保证存在，因为要对高度进行更新，不存在可不行
    if(BinarySearchTree<Comparable>::contains(x)){
        BinarySearchTree<Comparable>::root = remove(x, BinarySearchTree<Comparable>::root);
    }
}

#endif
#undef BinarySearchTree