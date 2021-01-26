template<typename Element>
class List{
    struct Node{
        Element element;
        Node*   next;
        Node*   last;
    };

    public:
        List();
        List(const List& rhs);
        ~List();

        //迭代器--没加越界判断
        class const_iterator{
            friend class List<Element>;
            public:
                const Element&  operator*() const  { return p->element; }
                //const_iterator& operator+(unsigned int n);
                //const_iterator& operator-(unsigned int n);
                const_iterator& operator++() { p = p->next; return *this; }                                 //++i
                const_iterator& operator--() { p = p->last; return *this; }
                const_iterator  operator++(int) { const_iterator old(*this); p = p->next; return old;}      //i++
                const_iterator  operator--(int) { const_iterator old(*this); p = p->last; return old;}
                bool            operator==(const const_iterator& rhs) { return p == rhs.p; }
                bool            operator!=(const const_iterator& rhs) { return p != rhs.p; }
            protected:
                const_iterator(Node* p_):p(p_){   }
                Node*           p;
                //List          *list;                                                                               //指向其所属的迭代器
        };
        class iterator : public const_iterator{ //
            friend class List<Element>;

            public:
                Element&    operator*() { return const_iterator::p->element; }
                // iterator& operator+(unsigned int n);
                // iterator& operator-(unsigned int n);
                iterator&   operator++() { const_iterator::operator++(); return *this; }                              //++i
                iterator&   operator--() { const_iterator::operator--(); return *this; }
                iterator    operator++(int) {iterator old(*this); const_iterator::operator++(); return old;}           //i++
                iterator    operator--(int) {iterator old(*this); const_iterator::operator++(); return old;}
            private:
                iterator(Node* p):const_iterator(p){  }
        };

        const_iterator  cbegin() { return const_iterator(header->next); }
        const_iterator  cend() { return const_iterator(tail->p); }
        iterator        begin() { return iterator(header->next); }
        iterator        end() { return iterator(tail); }

        const List&     operator=(const List& rhs);
        unsigned int    size() const;
        void            clear();
        bool            empty() const {return header->next == tail;}
        void            push_back(const Element& element);
        void            pop_back();
        void            push_front(const Element& element);
        void            pop_front();
        const Element&  back() {return tail->last->element;}
        const Element&  front() {return header->next->element;}
        iterator        find(const Element& rhs);
        iterator        insert(const iterator& p, const Element& element);
        iterator        erase(const iterator& p);
    private:
        Node*           header;
        Node*           tail;
};

template<typename Element>
List<Element>::List(){
    header = new Node;
    tail   = new Node;
    
    header->next = tail;
    header->last = nullptr;
    tail->next   = nullptr;
    tail->last   = header;
}

template<typename Element>
List<Element>::List(const List& rhs){
    header = new Node;
    tail   = new Node;
    
    header->next = tail;
    header->last = nullptr;
    tail->next   = nullptr;
    tail->last   = header;

    operator=(rhs);
}

template<typename Element>
List<Element>::~List(){
    clear();
    //不要忘记两个哨兵节点
    delete header;
    delete tail;
}

//-----------------------------------------------------------------------------------
template<typename Element>
const List<Element>& List<Element>::operator=(const List& rhs){
    if(&rhs == this) *this;

    clear();

    Node* p = header;
    for(Node* i = rhs.header->next; i != rhs.tail; i = i->next){
        Node* tmp = new Node;
        tmp->element = i->element;
        p->next = tmp;
        tmp->last = p;
        p = tmp;
    }
    p->next = tail;
    tail->last = p;

    return *this;
}

template<typename Element>
void List<Element>::clear(){
    Node* p = header->next;
    Node* tmp;
    while(p != tail){
        tmp = p->next;
        delete p;
        p = tmp;
    }

    tail->last = header;
}

template<typename Element>
unsigned int List<Element>::size() const{
    unsigned int sum = 0;
    for(Node* i = header->next; i != tail; i = i->next) sum++;
    return sum;
}

template<typename Element>
void List<Element>::push_back(const Element& element){
    Node* node = new Node;
    node->element = element;

    node->next = tail;
    node->last = tail->last;
    tail->last->next = node;
    tail->last = node;
}

template<typename Element>
void List<Element>::pop_back(){
    if(tail->last == header) return;

    Node* node = tail->last;
    node->last->next = tail;
    tail->last = node->last;

    delete node;
}

template<typename Element>
void List<Element>::push_front(const Element& element){
    Node* node = new Node;
    node->element = element;

    node->next = header->next;
    node->last = header;
    header->next->last = node;
    header->next = node;
}

template<typename Element>
void List<Element>::pop_front(){
    if(header->next == tail) return;

    Node* node = header->next;
    node->next->last = header;
    header->next = node->next;

    delete node;
}

template<typename Element>
typename List<Element>::iterator List<Element>::find(const Element& element){
    Node* p = header->next;
    while(p != tail){
        if(p->element == element)
            break;
        p = p->next;
    }
    return p;
}

//可以通过在const_iterator和iterator中添加一个指向List的指针来判断是否是该List所有的迭代器
template<typename Element>
typename List<Element>::iterator List<Element>::insert(const iterator& pos, const Element& element){
    Node* p = pos.p;

    Node* node = new Node;
    node->element = element;
    
    node->last = p;
    node->next = p->next;
    p->next->last = node;
    p->next = node;

    return iterator(node);
}

template<typename Element>
typename List<Element>::iterator List<Element>::erase(const iterator& pos){
    Node* node = pos.p;
    iterator pos_next = pos;
    pos_next++;

    node->last->next = node->next;
    node->next->last = node->last;

    delete node;
    
    return pos_next;
}