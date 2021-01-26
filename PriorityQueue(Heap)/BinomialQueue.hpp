#include<vector>
const int NUM_OF_TREE_DEFAULT = 64;    //默认64课树，最多2^64个节点
template<typename Comparable>
class BinomialQueue{
    struct Node{
        Comparable element;
        Node*   leftChild = nullptr;
        Node*   nextSibling = nullptr;
    };
    using size_type = unsigned int;
    
public:
    BinomialQueue():trees(NUM_OF_TREE_DEFAULT, nullptr){};
    ~BinomialQueue();

    void        clear();                                                        //清空
    bool        empty() const;                                                  //是否为空
    const       BinomialQueue &operator=(const BinomialQueue& rhs);             //赋值
    void        insert(const Comparable& element);                                 //插入
    void        merge(BinomialQueue& rhs);//<-------------------------核心算法--//将rhs并入该对象，rhs将被清空
    void        merge_copy(const BinomialQueue& rhs);                           //将rhs并入该对象，rhs不会被清空
    Comparable     getMin() const;                                                 //获取最小值
    void        deleteMin();                                                    //删除最小值
    size_type   size() const;                                                   //获取大小
    
private:
    std::vector<Node*>  trees;
    size_type           size_current = 0;

    Node*               copyTree(Node* root);                                   //复制一棵树，返回复制出的树的根
    Node*               combineTrees(Node* t1, Node* t2);                       //合并两个树
    void                clearTree(Node* root);                                  //清空树
};

template<typename Comparable>
BinomialQueue<Comparable>::~BinomialQueue(){
    clear();
}

template<typename Comparable>
bool BinomialQueue<Comparable>::empty() const{
    return size_current == 0;
}

template<typename Comparable>
void BinomialQueue<Comparable>::clear(){
    for(int i = 0, j = 1; j <= size_current; j *= 2, i++){
        clearTree(trees[i]);
        trees[i] = nullptr;
    }

    size_current = 0;
}

template<typename Comparable>
const BinomialQueue<Comparable>& BinomialQueue<Comparable>::operator=(const BinomialQueue<Comparable>& rhs){
    clear();
    
    size_current = rhs.size_current;

    for(int i = 0, j = 1; j <= rhs.trees.size(); i++, j*=2){
        if(rhs.trees[i] != nullptr)
            trees[i] = copyTree(rhs.trees[i]);
    }

    return *this;
}

template<typename Comparable>
typename BinomialQueue<Comparable>::Node* BinomialQueue<Comparable>::copyTree(BinomialQueue<Comparable>::Node* root){
    if(root == nullptr) return nullptr;

    Node* root_new = new Node;
    root_new->element = root->element;
    
    //复制兄弟节点
    root_new->nextSibling = copyTree(root->nextSibling);
    //复制子节点
    root_new->leftChild = copyTree(root->leftChild);

    return root_new;
}

template<typename Comparable>
typename BinomialQueue<Comparable>::Node* BinomialQueue<Comparable>::combineTrees(Node* t1, Node* t2){
    if(t2->element < t1->element)
        std::swap(t1, t2);
    
    t2->nextSibling = t1->leftChild;
    t1->leftChild = t2;

    return t1;
}

template<typename Comparable>
void BinomialQueue<Comparable>::clearTree(Node* root){
    if(root == nullptr) return;
    clearTree(root->nextSibling);
    clearTree(root->leftChild);

    delete root;
}

template<typename Comparable>
void BinomialQueue<Comparable>::merge(BinomialQueue& rhs){
    if(rhs.size_current == 0) return;
    size_current += rhs.size_current;

    Node* carry = nullptr;              //进位上来的
    Node* t1    = nullptr;
    Node* t2    = nullptr;
    for(int i = 0, j = 1; j <= size_current; i++, j *= 2){
        t1 = trees[i];
        t2 = rhs.trees[i];
        rhs.trees[i] = nullptr;         //清除
        switch((t1 != nullptr) + (t2 != nullptr)*2 + (carry != nullptr)*4){
            case 1:                     //只有t1
                break;
            case 2:                     //只有t2
                trees[i] = t2;
                break;
            case 3:                     //有t1和t2
                trees[i] = nullptr;
                carry = combineTrees(t1, t2);
                break;
            case 4:                     //只有只有两个B(k)合成的B(k+1)
                trees[i] = carry;
                carry = nullptr;
                break;
            case 5:                     //有t1和进位的carry
                trees[i] = nullptr;
                carry = combineTrees(t1, carry);
                break;
            case 6:                     //有t2和进位的carry
                carry = combineTrees(t2, carry);
                break;
            case 7:                     //有t1，t2，carry
                trees[i] = carry;
                carry = combineTrees(t1, t2);
                break;
        }
    }

    rhs.size_current = 0;
}

template<typename Comparable>
void BinomialQueue<Comparable>::merge_copy(const BinomialQueue& rhs){
    BinomialQueue temp = rhs;
    merge(temp);
}

template<typename Comparable>
void BinomialQueue<Comparable>::insert(const Comparable& element){
    Node* carry = new Node;
    carry->element = element;

    for(int i = 0; i < trees.size(); i++){
        if(trees[i] == nullptr){                //有空的位置插入并退出
            trees[i] = carry;
            break;
        }else{
            carry = combineTrees(trees[i], carry);
            trees[i] = nullptr;
        }
    }

    size_current++;
}

template<typename Comparable>
Comparable BinomialQueue<Comparable>::getMin() const{
    if(empty()) throw "The Binomial Queue is empty";
    Comparable min_current;

    //记录第一个树的根节点
    int i, j;
    for(i = 0, j = 1; j <= size_current; i++, j *= 2)
        if(trees[i] != nullptr){
            min_current = trees[i]->element;
            break;
        }
    
    //寻找最小
    for(; j <= size_current; i++, j *= 2)
        if(trees[i] != nullptr && trees[i]->element < min_current)
            min_current = trees[i]->element;

    return min_current;
}

template<typename Comparable>
void BinomialQueue<Comparable>::deleteMin(){
    if(empty()) throw "The Binomial Queue is empty";
    
    //寻找最小根的树
    Comparable min_current;
    int i, j, p = 0;
    for(i = 0, j = 1; j <= size_current; i++, j *= 2){
        if(trees[i] != nullptr){
            min_current = trees[i]->element;
            p = i;
            break;
        }
    }
    for(; j <= size_current; i++, j *= 2){
        if(trees[i] != nullptr && trees[i]->element < min_current){
            min_current = trees[i]->element;
            p = i;
        }
    }

    //将根节点删除，产生若干小的树，作为一个二项队列
    BinomialQueue<Comparable> temp;    //临时构建一棵树
    Node* subTree = trees[p]->leftChild;
    Node* nextSubTree;
    for(int k = p-1; k >= 0; k--){
        nextSubTree = subTree->nextSibling;
        subTree->nextSibling = nullptr;

        temp.trees[k] = subTree;

        subTree = nextSubTree;
    }

    delete trees[p];
    trees[p] = nullptr;

    temp.size_current = -1;     //使合并后的大小-1
    merge(temp);
}

template<typename Comparable>
typename BinomialQueue<Comparable>::size_type BinomialQueue<Comparable>::size() const{
    return size_current;
}