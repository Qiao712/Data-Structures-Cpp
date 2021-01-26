using UINT = unsigned int;
template<typename Comparable>
class LeftistHeap {
//对外接口
public:
    LeftistHeap()       { }
    LeftistHeap(const LeftistHeap& rhs);
    ~LeftistHeap()      {clear();}

    void                merge(const LeftistHeap& rhs);      //与另一个堆合并-对外接口
    void                insert(const Comparable& element);  //插入单个元素
    void                deleteMin();                        //删除最小元素
    const Comparable&   getMin()   const;                   //获得最小元素
    void                clear();                            //清空堆,接口，启动clear_recursion
    bool                empty();
    const LeftistHeap&  operator=(const LeftistHeap& rhs);   //启动copy()


//核心算法
private:
    struct Node {
        Comparable element;
        Node* left              = nullptr;
        Node* right             = nullptr;
        UINT  npl               = 0;                        //NPL:null path length 零长路径
    };

    Node* root;
    Node* merge(Node* h1, Node* h2);                         //合并-启动
    void  clear_recursion(Node* at);                         //递归地删除
    Node* copy(const Node* h);                               //递归地复制

    void                print(Node* r);
};

//----------------------------------------------------------------------------------------------
//核心算法
template<typename Comparable>
typename LeftistHeap<Comparable>::Node* LeftistHeap<Comparable>::merge(Node* h1, Node* h2) {
    if (h1 == nullptr)
        return h2;
    if (h2 == nullptr)
        return h1;
    if (!(h1->element < h2->element))              //保证h1是根较小的堆
        swap(h1, h2);

    //将具有较大根值的堆 与 具有较小根值的堆的右子堆合并（递归地）
    if (h1->left == nullptr)                        //由于左子节点的零长路径大于或等于右子节点的零长路径，所以如果左子节点没有子节点，右子节点也没有
        h1->left = h2;                              //直接将h2作为这个没有子节点的根的左子树
                                                    //由于没有右子节点，所以nlp仍为0
    else {
        h1->right = merge(h1->right, h2);           //合并
        if (h1->right->npl > h1->left->npl)         //使左子堆的nlp>=右子堆的npl
            swap(h1->right, h1->left);

        h1->npl = h1->right->npl + 1;               //更新零长路径
    }

    return h1;
}

template<typename Comparable>
typename LeftistHeap<Comparable>::Node* LeftistHeap<Comparable>::copy(const Node* at){
    if (at == nullptr) return nullptr;

    Node* node = new Node;
    node->element = at->element;

    node->left = copy(at->left);
    node->right = copy(at->right);

    return node;
}

template<typename Comparable>
void LeftistHeap<Comparable>::clear_recursion(Node* at){
    if (at == nullptr) return;
    clear_recursion(at->left);
    clear_recursion(at->right);
    
    delete at;
    return;
}
//---------------------------------------------------------------------------------------------

template<typename Comparable>
LeftistHeap<Comparable>::LeftistHeap(const LeftistHeap& rhs){
    *this = rhs;
}

template<typename Comparable>
bool LeftistHeap<Comparable>::empty() {
    if (root == nullptr) return true;
    else return false;
}

template<typename Comparable>
void LeftistHeap<Comparable>::insert(const Comparable& element) {
    Node* node = new Node;
    node->element = element;

    root = merge(root, node);
}

template<typename Comparable>
void LeftistHeap<Comparable>::print(LeftistHeap<Comparable>::Node* r) {
    if (r == nullptr) return;

    cout << r->element << endl;

    print(r->left);
    print(r->right);
}

template<typename Comparable>
void LeftistHeap<Comparable>::merge(const LeftistHeap& rhs){
    Node* heap_copy = copy(rhs.root);       //拷贝堆
    this->root = merge(this->root,heap_copy);
}

template<typename Comparable>
void LeftistHeap<Comparable>::deleteMin() {
    Node *heap_left = root->left;
    Node *heap_right = root->right;
    delete root;
    root = merge(heap_left, heap_right);
}

template<typename Comparable>
const Comparable& LeftistHeap<Comparable>::getMin() const {
    return root->element;
}

template<typename Comparable>
void LeftistHeap<Comparable>::clear(){
    clear_recursion(root);
    root = nullptr;
}

template<typename Comparable>
const LeftistHeap<Comparable>& LeftistHeap<Comparable>::operator=(const LeftistHeap<Comparable>& rhs){
    clear();    //先释放原来空间
    
    this->root = copy(rhs.root);

    return rhs;
}