#ifndef __BINARY_SEARCH_TREE_WITH_HEIGHT_INFORMATION__
#define __BINARY_SEARCH_TREE_WITH_HEIGHT_INFORMATION__
/*
*规定右子节点大于其父节点，左子节点小于其夫节点
*/
template<typename Comparable>
class BinarySearchTree_WithHeightInformation{
    protected:
        struct Node{
            Comparable   element;
            Node*        right = nullptr;
            Node*        left  = nullptr;
            unsigned int height = 0;
        };
    public:
        using size_type = unsigned int;

        BinarySearchTree_WithHeightInformation() = default;
        virtual ~BinarySearchTree_WithHeightInformation();
        
        const Comparable&       findMin() const;                            //返回最小元素
        const Comparable&       findMax() const;                            //返回最大元素
        bool                    contains(const Comparable& x) const;        //是否包含该元素
        bool                    empty() const;                              //是否为空
        size_type               size() const;                               //返回大小
        void                    clear();                                    //清空接口，调用void clear_recursion(Node* t);
        
        void                    insert(const Comparable& x);                //插入接口，调用void insert(const Comparable& x, Node* t); 
        void                    remove(const Comparable& x);                //移除接口，调用void remove_recursion(const Comparable& x, Node* t);  

        const BinarySearchTree_WithHeightInformation& operator=(const BinarySearchTree_WithHeightInformation& rhs);     //调用Node* copy_recursion(Node* t);
        void print(){print(root);}
        void                    print(Node* at);
        
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

template<typename Comparable>
void BinarySearchTree_WithHeightInformation<Comparable>::clear_recursion(Node* t){
    if(t == nullptr) return;
    clear_recursion(t->left);
    clear_recursion(t->right);

    delete t;
}

template<typename Comparable>
typename BinarySearchTree_WithHeightInformation<Comparable>::Node* BinarySearchTree_WithHeightInformation<Comparable>::copy_recursion(Node* t){
    if(t == nullptr) return nullptr;
    Node* new_node = new Node;
    new_node->element = t->element;
    new_node->height  = t->height;
    new_node->left    = copy_recursion(t->left);
    new_node->right   = copy_recursion(t->right);
    return new_node;
}

template<typename Comparable>
typename BinarySearchTree_WithHeightInformation<Comparable>::Node* BinarySearchTree_WithHeightInformation<Comparable>::findMax(Node* t) const{
    Node* node = root;
    while(node->right != nullptr){
        node = node->right;
    }

    return node;
}

template<typename Comparable>
typename BinarySearchTree_WithHeightInformation<Comparable>::Node* BinarySearchTree_WithHeightInformation<Comparable>::findMin(Node* t) const{
    Node* node = root;
    while(node->left != nullptr){
        node = node->left;
    }

    return node;
}

template<typename Comparable>
typename BinarySearchTree_WithHeightInformation<Comparable>::Node* BinarySearchTree_WithHeightInformation<Comparable>::find(const Comparable& x, Node* t) const {
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
template<typename Comparable>
BinarySearchTree_WithHeightInformation<Comparable>::~BinarySearchTree_WithHeightInformation(){
    clear();
}

template<typename Comparable>
void BinarySearchTree_WithHeightInformation<Comparable>::print(Node* at){
    if(at == nullptr) return;
    std::cout<<at->element;
    if(at->left) std::cout<<' '<<at->left->element;
    if(at->right)  std::cout<<' '<<at->right->element;
    std::cout<<std::endl;
    print(at->right);
    print(at->left);
}

template<typename Comparable>
const Comparable& BinarySearchTree_WithHeightInformation<Comparable>::findMax() const{
    return findMax(root)->element;
}

template<typename Comparable>
const Comparable& BinarySearchTree_WithHeightInformation<Comparable>::findMin() const{
    return findMin(root)->element;
}

template<typename Comparable>
const BinarySearchTree_WithHeightInformation<Comparable>&  BinarySearchTree_WithHeightInformation<Comparable>::operator=(const BinarySearchTree_WithHeightInformation<Comparable>& rhs){
    if(this == &rhs){
        clear();
        root = copy_recursion(rhs.root);
        size_current = rhs.size_current;
    }

    return *this;
}

template<typename Comparable>
void BinarySearchTree_WithHeightInformation<Comparable>::insert(const Comparable& x){
    root = insert(x, root);
}

template<typename Comparable>
void BinarySearchTree_WithHeightInformation<Comparable>::remove(const Comparable& x){
    root = remove_recursion(x, root);   //size--在remove_recursion
}

template<typename Comparable>
bool BinarySearchTree_WithHeightInformation<Comparable>::contains(const Comparable& x) const{
    if(find(x, root)) return true;
    else        return false;
}

template<typename Comparable>
void BinarySearchTree_WithHeightInformation<Comparable>::clear(){
    clear_recursion(root);
    size_current = 0;
    root = nullptr;
}

template<typename Comparable>
typename BinarySearchTree_WithHeightInformation<Comparable>::size_type BinarySearchTree_WithHeightInformation<Comparable>::size() const{
    return size_current;
}

template<typename Comparable>
bool BinarySearchTree_WithHeightInformation<Comparable>::empty() const{
    return size_current == 0;
}

#endif