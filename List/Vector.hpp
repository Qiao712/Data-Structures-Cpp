#ifndef _VECTOR_
#define _VECTOR_
template<typename Element>
class Vector{
    public:
        explicit            Vector(int size_ = 0);
                            Vector(const Vector& rhs);
                            ~Vector() {clear();}                                //不要忘了析构函数啊

        using iterator = Element*;                                              //迭代器
        iterator            begin() {return elements;};
        iterator            end() {return elements+size_cur;};

        const Vector&       operator=(const Vector& rhs);
        Element&            operator[](unsigned int i);
        unsigned int        size() const {return size_cur;};
        void                clear();
        bool                empty() const {return size_cur == 0;};
        void                push_back(const Element& element);
        void                pop_back();
        iterator            find(const Element& element) const;                 //找到元素,若不存在返回尾后
        iterator            insert(const iterator& p, const Element& element);  //在p后插入
        iterator            erase(const iterator& p);                           //删除p
        void                reserve(unsigned int size_);                        //重新分配空间
        const Element&      back() const {return *(elements+size_cur-1);};
        const Element&      front() const {return *elements;};

    private:
        unsigned int        size_cur;
        unsigned int        capacity;
        Element*            elements;
};

template<typename Element>
Vector<Element>::Vector(int size_):size_cur(size_){
    capacity = size_cur*2;

    elements = new Element[capacity];
}

template<typename Element>
Vector<Element>::Vector(const Vector& rhs){
    operator=(rhs);
}

template<typename Element>
const Vector<Element>& Vector<Element>::operator=(const Vector& rhs){
    if(this != &rhs){               //不要忘记处理自赋值的情况
        clear();
    
        size_cur = rhs.size_cur;
        capacity = rhs.capacity;
        elements = new Element[capacity];
        
        for(int i = 0; i<size_cur; i++){
            elements[i] = rhs.elements[i];
        }
    }
    return *this;
}

template<typename Element>
Element& Vector<Element>::operator[](unsigned int i){
    return elements[i];
}

template<typename Element>
void Vector<Element>::clear(){
    delete[] elements;
    size_cur = 0;
    capacity = 0;
}

template<typename Element>
void Vector<Element>::push_back(const Element& element){
    //若空间不足则抛弃原来空间，创建的新空间是原空间大小的两倍
    if(capacity < size_cur + 1){
        reserve((size_cur + 1)*2);
    }

    elements[size_cur] = element;
    size_cur++;
}

template<typename Element>
void Vector<Element>::pop_back(){
    if(!empty()) size_cur--;
}

template<typename Element>
typename Vector<Element>::iterator Vector<Element>::find(const Element& element) const{
    for(int i = 0; i<size_cur; i++){
        if(elements[i] == element) return elements + i;
    }
    return elements + size_cur;
}

template<typename Element>
typename Vector<Element>::iterator Vector<Element>::erase(const iterator& p){   
    if(p - elements < size_cur){
        size_cur--;

        for(iterator i = p; i < end(); i++){
            *i = *(i+1);
        }
    }

    return p;
}

template<typename Element>
typename Vector<Element>::iterator Vector<Element>::insert(const iterator& p, const Element& element){
    //若空间不足则抛弃原来空间，创建的新空间是原空间大小的两倍
    if(capacity < size_cur + 1){
        reserve((size_cur + 1)*2);
    }

    size_cur++;
    for(iterator i = elements + size_cur - 1; i > p + 1; i--){
        *i = *(i-1);
    }

    *(p+1) = element;

    return p+1;
}

template<typename Element>
void Vector<Element>::reserve(unsigned int size_){
    capacity = size_;

    Element* new_elements = new Element[capacity];

    size_cur = capacity < size_cur ? capacity : size_cur;

    for(int i = 0; i<size_cur; i++){
        new_elements[i] = elements[i];
    }

    delete[] elements;
    elements = new_elements;
    
}
#endif