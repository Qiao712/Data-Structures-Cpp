/*
* 规定右子节点大于 等于 其父节点，左子节点小于其夫节点
* 重复加入：直接插入新节点，不计数
*/
#ifndef __BINARY_SEARCH_TREE__
#define __BINARY_SEARCH_TREE__

template<typename Comparable>
struct DefaultNode{
    Comparable      element;
    DefaultNode*    right = nullptr;
    DefaultNode*    left  = nullptr;
};

template<typename Comparable, typename Node>
class BinarySearchTreeTemplate{
    public:
        using size_type = unsigned int;

        BinarySearchTreeTemplate() = default;
        virtual ~BinarySearchTreeTemplate();
        
        const Comparable&       findMin() const;                            //返回最小元素
        const Comparable&       findMax() const;                            //返回最大元素
        bool                    contain(const Comparable& x) const;        //是否包含该元素
        bool                    empty() const;                              //是否为空
        size_type               size() const;                               //返回大小
        
        void                    clear();                                    //清空接口，调用void clear_recursion(Node* t);
        void                    insert(const Comparable& x);                //插入接口，调用void insert(const Comparable& x, Node* t); 
        void                    remove(const Comparable& x);                //移除接口，调用void remove_recursion(const Comparable& x, Node* t);  

        BinarySearchTreeTemplate&       operator=(const BinarySearchTreeTemplate& rhs);     //调用Node* copy_recursion(Node* t);
        //void                    print_(Node* at);
        //void                    print();
        
    protected:
        Node*                   root = nullptr;
        size_type               size_current = 0;


        Node*                   insert(const Comparable& x, Node* t);                         //插入
        void                    clear_recursion(Node* t);                                     //清空
        Node*                   remove_recursion(const Comparable& x, Node* t);               //移除某元素
        Node*                   copy_recursion(Node* t);                                      //递归实现复制
        Node*                   findMax(Node* t) const;                                       //返回最大元素节点的指针
        Node*                   findMin(Node* t) const;                                       //返回最小元素的指针
        Node*                   find(const Comparable& x, Node* t) const;                           //找到包含某元素的节点并返回其其地址
};

//BinarySearchTree
template<typename Comparable>
using BinarySearchTree = BinarySearchTreeTemplate<Comparable, DefaultNode<Comparable>>;

/*核心算法--删除节点---------------------------------------------------------------------------------------------------------*/
template<typename Comparable, typename Node>
Node* BinarySearchTreeTemplate<Comparable, Node>::remove_recursion(const Comparable& x, Node* t){    
    if(t == nullptr){
        return nullptr;
    }else if(x == t->element){  //找到了该元素,进行删除
        if(t->left && t->right){
            //所储存的元素被复制了一次！（待改进）
            Comparable min_in_right = findMin(t->right)->element;
            t->element = min_in_right;
            t->right = remove_recursion(min_in_right, t->right);
        }else if(t->left){
            Node* tmp = t;
            t = t->left;
            delete tmp;
        }else{  //只有右节点或无子节点
            Node* tmp = t;
            t = t->right;
            delete tmp;
        }

        size_current--;
    }else if(x < t->element){
        t->left = remove_recursion(x, t->left);
    }else{
        t->right = remove_recursion(x, t->right);
    }

    return t;
}

template<typename Comparable, typename Node>
void BinarySearchTreeTemplate<Comparable, Node>::clear_recursion(Node* t){
    if(t == nullptr) return;
    clear_recursion(t->left);
    clear_recursion(t->right);

    delete t;
}

template<typename Comparable, typename Node>
Node* BinarySearchTreeTemplate<Comparable, Node>::copy_recursion(Node* t){
    if(t == nullptr) return nullptr;
    Node* new_node = new Node;
    new_node->element = t->element;
    new_node->left    = copy_recursion(t->left);
    new_node->right    = copy_recursion(t->right);
    return new_node;
}

template<typename Comparable, typename Node>
Node* BinarySearchTreeTemplate<Comparable, Node>::findMax(Node* t) const{
    Node* node = t;
    while(node->right != nullptr){
        node = node->right;
    }

    return node;
}

template<typename Comparable, typename Node>
Node* BinarySearchTreeTemplate<Comparable, Node>::findMin(Node* t) const{
    Node* node = t;
    while(node->left != nullptr){
        node = node->left;
    }

    return node;
}

template<typename Comparable, typename Node>
Node* BinarySearchTreeTemplate<Comparable, Node>::insert(const Comparable& x, Node* t){
    size_current++;

    Node* node_new = new Node;
    node_new->element = x;

    //如果根为空则设为根
    if(t == nullptr){ 
        t = node_new;
        return t;
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

template<typename Comparable, typename Node>
Node* BinarySearchTreeTemplate<Comparable, Node>::find(const Comparable& x, Node* t) const{
    Node* node = t;
    while(node != nullptr){
        if(node->element == x){
            return node;
        }else if(x < node->element){
            node = node->left;
        }else{
            node = node->right;
        }
    }
    return nullptr;
}

//接口----------------------------------------------------------------------------------------------------------------
template<typename Comparable, typename Node>
BinarySearchTreeTemplate<Comparable, Node>::~BinarySearchTreeTemplate(){
    clear();
}

// template<typename Comparable, typename Node>
// void BinarySearchTreeTemplate<Comparable, Node>::print(){
//     print_(root);
// }

// template<typename Comparable, typename Node>
// void BinarySearchTreeTemplate<Comparable, Node>::print_(Node* at){
//     if(at == nullptr) return;
//     std::cout<<at->element<<' '<<(at->left ? at->left->element : '\0')<<' '<<(at->right ? at->right->element : '\0')<<endl;
//     print_(at->right);
//     print_(at->left);
// }

template<typename Comparable, typename Node>
const Comparable& BinarySearchTreeTemplate<Comparable, Node>::findMax() const{
    return findMax(root)->element;
}

template<typename Comparable, typename Node>
const Comparable& BinarySearchTreeTemplate<Comparable, Node>::findMin() const{
    return findMin(root)->element;
}

template<typename Comparable, typename Node>
BinarySearchTreeTemplate<Comparable, Node>&  BinarySearchTreeTemplate<Comparable, Node>::operator=(const BinarySearchTreeTemplate<Comparable, Node>& rhs){
    if(this == &rhs){
        clear();
        root = copy_recursion(rhs.root);
        size_current = rhs.size_current;
    }

    return *this;
}

template<typename Comparable, typename Node>
void BinarySearchTreeTemplate<Comparable, Node>::insert(const Comparable& x){
    root = insert(x, root);
}

template<typename Comparable, typename Node>
void BinarySearchTreeTemplate<Comparable, Node>::remove(const Comparable& x){
    root = remove_recursion(x, root);   //size--在remove_recursion
}

template<typename Comparable, typename Node>
bool BinarySearchTreeTemplate<Comparable, Node>::contain(const Comparable& x) const{
    if(find(x, root)) return true;
    else        return false;
}

template<typename Comparable, typename Node>
void BinarySearchTreeTemplate<Comparable, Node>::clear(){
    clear_recursion(root);
    size_current = 0;
    root = nullptr;
}

template<typename Comparable, typename Node>
typename BinarySearchTreeTemplate<Comparable, Node>::size_type BinarySearchTreeTemplate<Comparable, Node>::size() const{
    return size_current;
}

template<typename Comparable, typename Node>
bool BinarySearchTreeTemplate<Comparable, Node>::empty() const{
    return size_current == 0;
}

#endif