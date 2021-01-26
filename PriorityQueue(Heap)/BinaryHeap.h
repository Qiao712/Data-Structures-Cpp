#include<vector>
using namespace std;
using UINT = unsigned int;

template<typename Comparable>
class BinaryHeap{
    public:
        explicit                BinaryHeap(UINT _size = 100);
        explicit                BinaryHeap(const vector<Comparable> &items);
        
        bool                    empty() const;
        const                   Comparable & getMin() const;
        void                    insert(const Comparable &x);
        void                    deleteMin();
        //void                  operator=(const BinaryHeap& rhs);
        //void makeEmpty();

    private:
        UINT size = 0;                      //!!!size 与vector中的元素个数不一致（从[1]开始存放元素）!!!
        vector<Comparable>      array;      //!!!从下标1开始存放元素!!!

        //子节点/父节点编号获取 - 完全二叉树（complete binary tree)特色
        UINT getParent(UINT node)       const { return node/2; }
        UINT getLeftChild(UINT node)    const { return node*2; }
        UINT getRightChild(UINT node)   const { return node*2+1; }

        
        void buildHeap();                ////构建二叉堆,使无序的树有序，用于 接收vector的构造函数
        void percolateDown(UINT hole);   //下滤操作,用于buildHeap(),deleteMin()
};

template<typename Comparable>
BinaryHeap<Comparable>::BinaryHeap(UINT _size){
    array.resize(_size);
}

template<typename Comparable>
BinaryHeap<Comparable>::BinaryHeap(const vector<Comparable> &items):
array(items.size()*2),size(items.size()){                             //扩容
    for(UINT i = 0; i < items.size(); i++){
        array[i+1] = items[i];
    }
    buildHeap();
}

//是否为空
template<typename Comparable>
bool BinaryHeap<Comparable>::empty() const{
    return (bool)size;
}

//插入元素
template<typename Comparable>
void BinaryHeap<Comparable>::insert(const Comparable &x){
    size++;

    //判断vector是否需要扩容
    if(array.size() < size+1) array.resize((size+1)*2);

    //上滤(percolate up)
    UINT hole;
    for(hole = size; hole > 1 && array[ getParent(hole) ] > x; hole = getParent(hole)){
        array[hole] = array[getParent(hole)];
    }

    array[hole] = x;
}

//返回最小的元素
template<typename Comparable>
const Comparable &BinaryHeap<Comparable>::getMin() const{
    return array[1];
}

//删除最小的元素
template<typename Comparable>
void BinaryHeap<Comparable>::deleteMin(){
    if(empty()) return;

    array[1] = array[size];     //用最后一个元素覆盖根节点上的元素
    size--;
    percolateDown(1);            //下滤
}

//构建二叉堆,使无序的树有序，用于 接收vector的构造函数
template<typename Comparable>
void BinaryHeap<Comparable>::buildHeap(){
    for(int i = size/2; i >= 1; i--){
        percolateDown(i);
    }
}

//下滤
template<typename Comparable>
void BinaryHeap<Comparable>::percolateDown(UINT objective){
    Comparable objective_element = array[objective];
    
    UINT child;
    UINT hole;
    for(hole = objective; getLeftChild(hole) <= size; hole = child){
        UINT left = getLeftChild(hole);
        UINT right = getRightChild(hole);

        //child = 最小的子节点
        if(right <= size && array[right] < array[left]) child = right;      //right <= size，判断hole是否有两个子节点
        else  child = left;
        
        if(objective_element > array[child]) array[hole] = array[child];    //下移
        else break;
    }

    array[hole] = objective_element;
}