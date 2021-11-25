#pragma once
#ifndef _MY_LIST_
#define _MY_LIST_

#include <functional>
#include <thread>
#include <future>
#include <utility>
using std::future;
using std::async;
using std::swap;
//要求T类型可以默认初始化
template<class T>
class List{
private:
    struct Node{
        T element;
        Node* last;
        Node* next;
    };
    Node* head;
    Node* tail;
    unsigned int  _size;

    //为1时禁止复制
    bool copy_lock = 0;

    //排序
    typedef std::function<bool(const T& a, const T& b)> Compare;          //用于接收比较函数，用function包装可以传入匿名函数
    typedef std::pair<Node*, Node*> Range;                                //储存两个指向节点的指针，指示范围[first, second]
    template<typename Pred> Range merge_sort(Node* left, int part_size, Pred cmp);           //归并排序，返回每该段的新范围
    template<typename Pred> Node* merge(Node* first, Node* mid, Node* last, Pred cmp);       //将有序的两端[first,mid)和[mid, last)合并，保证last不会改变，返回新的头节点指针
public:
    //仿照STL的迭代器，用于遍历元素
    class iterator{
        friend class List<T>;
        private:
            Node* p;
            iterator(Node* p_, unsigned int* size_):p(p_), size(size_) { }
            unsigned int* size;                         //用于修改表中的_size
        public:
            iterator() : p(nullptr) { }                 //默认构造一个无效的迭代器
            T& operator*(){ return p->element; }
            T* operator->(){ return &(p->element); }
            iterator& operator++();
            iterator operator++(int);
            iterator& operator--();
            iterator operator--(int);
            bool operator==(const iterator& rhs) const;
            bool operator!=(const iterator& rhs) const;
            
            void removeSelf();
            bool isValid() { return p != nullptr; }
    };

    List();
    ~List();

    //元素x插入到it指向的节点前
    iterator insert(const iterator& it, const T& x);
    //删除it指向的元素
    iterator remove(const iterator& it);
    //在链表末尾添加元素
    iterator push(const T& x);
    //删除链表中最后一个元素
    void pop();
    //返回元素个数
    unsigned int size() { return _size; }
    //清空
    void clear();
    //获取最后一个元素
    T& back();
    //获取迭代器，由于遍历
    iterator begin();
    iterator end();
    //排序
    void sort(Compare cmp);

    //复制控制
    void copyLock(bool lock) { copy_lock = lock; }
    bool canCopy() { return copy_lock; }

    List& operator=(const List& );
    List& operator=(List&& );
    List(const List& );
    List(List&& );
};


//------实现----------

template<class T>
List<T>::List() : _size(0)
{
    head = new Node;
    tail = new Node;
    head->next = tail;
    tail->last = head;
};

template<class T>
List<T>::List(const List& rhs)
{
    head = new Node;
    tail = new Node;
    head->next = tail;
    tail->last = head;
    
    operator=(rhs);
};

template<class T>
List<T>::List(List&& rhs)
{
    _size = rhs._size;
    //移动资源
    head = rhs.head;
    tail = rhs.tail;
    rhs.head = nullptr;
    rhs.tail = nullptr;
};

//销毁所有元素
template<class T>
List<T>::~List(){
    if(head == nullptr && tail == nullptr) return;
    clear();
    delete head;
    delete tail;
}

template<class T>
typename List<T>::iterator List<T>::insert(const iterator& it, const T& x){
    Node* newNode = new Node;
    newNode->last = it.p->last;
    newNode->next = it.p;
    newNode->element = x;
    it.p->last->next = newNode;
    it.p->last = newNode;

    _size++;
    return iterator(newNode, &_size);
}

template<class T>
typename List<T>::iterator List<T>::remove(const iterator& it){
    iterator nxt(it.p->next, &_size);
    it.p->last->next = it.p->next;
    it.p->next->last = it.p->last;

    _size--;
    delete it.p;
    return nxt;
}

template<class T>
typename List<T>::iterator List<T>::push(const T& x){
    return insert(end(), x);
}

template<class T>
void List<T>::pop(){
    Node* l = tail->last;
    T t = l->element;
    
    l->last->next = tail;
    tail->last = l->last;
    
    delete l;
    _size--;
}

template<class T>
void List<T>::clear(){
    _size = 0;
    Node* i = head->next, *j; 
    while(i != tail){
        j = i->next;
        delete i;
        i = j;
    }

    head->next = tail;
    tail->last = head;
}

template<class T>
typename List<T>::iterator List<T>::begin(){
    return iterator(head->next, &_size);
}

template<class T>
typename List<T>::iterator List<T>::end(){
    return iterator(tail, &_size);
}

//仿照标准库的归并排序，比标准库慢3倍（可能因为返回Range）
template<class T>
template<typename Pred>
typename List<T>::Range List<T>::merge_sort(Node* first, int part_size, Pred cmp) {
    //抄个标准库真。。麻烦
    //归并排序，返回该范围排序后的范围[first, last)
    if (part_size == 1) {
        return Range(first, first->next);
    }
    else if(part_size == 2){
        if (!cmp(first->element, first->next->element))
            swap(first->element, first->next->element);
        return Range(first, first->next->next);
    }
    
    Range first_part = merge_sort(first, part_size / 2, cmp);                //排序前半部分,返两个新的指示范围的指针[first_part.first, first_part.second)，该范围的头节点会被改变，first_part.first 可能不等于first
    Range last_part  = merge_sort(first_part.second, part_size - part_size / 2, cmp);    //排序后半部分[first_part.second, last)， 前半部分的尾后即该部分的开始，且排序后也可能被改变
    Node* new_first = merge<decltype(cmp)>(first_part.first, last_part.first, last_part.second, cmp);          //合并已排序的[first, last_part.first)和[last_part.first, last_part.last)

    return Range(new_first, last_part.second);  //返回新的范围
}

template<class T>
template<typename Pred>
typename List<T>::Node* List<T>::merge(Node* first, Node* mid, Node* last, Pred cmp) {
    //合并[first, mid) 与 [mid, last), 自己写的太慢了，只能参照标准库里的方法
    if (first == mid || mid == last) return first;
    //先提前找到新的头节点（first），以备返回
    Node* new_first;
    if (cmp(mid->element, first->element)) {
        new_first = mid;
    }
    else {
        new_first = first;
        //后移first，直到指向第一个 不“小于” mid 元素
        do {
            first = first->next;
            if (first == mid) return new_first; //如果前一部分为空，则可结束
        } while (cmp(first->element, mid->element));
    }

    while (true){
        //在[mid, last)中找到第一个 不“小于” first 的元素
        Node* run_start = mid;
        do {
            run_start = run_start->next;
        } while (run_start != last && cmp(run_start->element, first->element));

        //将[mid, run_start)接到first前
        Node* front_part_end = mid->last;
        Node* mid_part_end = run_start->last;
        front_part_end->next = run_start;
        run_start->last = front_part_end;
        first->last->next = mid;
        mid->last = first->last;
        mid_part_end->next = first;
        first->last = mid_part_end;
        
        mid = run_start;    //新的分界

        //后半部分为空，可以结束了
        if (mid == last) return new_first;

        //后移first，使其指向第一个不 “小于” mid 的元素
        do {
            first = first->next;
            if (first == mid) return new_first;
        } while (cmp(first->element, mid->element));
    }
}

//为了保持排序稳定，要使两个元素相等时cmp返回true。插入排序
template<class T>
void List<T>::sort(Compare cmp)
{
    if (_size == 0) return;
    merge_sort<decltype(cmp)>(head->next, _size, cmp);
}

template<class T>
T& List<T>::back(){
    return tail->last->element;
}

template<class T>
List<T>& List<T>::operator=(const List& rhs){
    if(&rhs == this) return *this;        //处理自赋值

    //复制链表
    clear();
    _size = rhs._size;
    
    //逐项复制
    if(_size == 0) return *this;
    Node* new_node, *last_node = head;
    for(Node* p = rhs.head->next; p != rhs.tail; p = p->next){
        new_node = new Node;
        new_node->element = p->element;
        new_node->last = last_node;
        last_node->next = new_node;
        last_node = new_node;
    }
    new_node->next = tail;
    tail->last = new_node;

    return *this;
}

template<class T>
List<T>& List<T>::operator=(List&& rhs){
    if(&rhs == this) return *this;        //处理自赋值

    clear();
    _size = rhs._size;

    //移动资源
    head = rhs.head;
    tail = rhs.tail;
    rhs.head = nullptr;
    rhs.tail = nullptr;
}

//迭代器的实现
template<class T>
typename List<T>::iterator& List<T>::iterator::operator++(){
    p = p->next;
    return *this;
}
template<class T>
typename List<T>::iterator List<T>::iterator::operator++(int){
    iterator t(p, size);
    p = p->next;
    return t;
}

template<class T>
typename List<T>::iterator& List<T>::iterator::operator--(){
    p = p->last;
    return *this;
}

template<class T>
typename List<T>::iterator List<T>::iterator::operator--(int){
    iterator t(p);
    p = p->last;
    return t;
}

template<class T>
bool List<T>::iterator::operator==(const iterator& rhs) const{
    return rhs.p == p;
}

template<class T>
bool List<T>::iterator::operator!=(const iterator& rhs) const{
    return rhs.p != p;
}

template<class T>
void List<T>::iterator::removeSelf(){
    p->last->next = p->next;
    p->next->last = p->last;

    (*size)--;
    delete p;

    p = nullptr;    //置为nullptr，表示无效
}

#endif