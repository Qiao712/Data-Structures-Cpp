/*用于实现 LRU(最近最少使用)缓存*/

#include<unordered_map>
using std::unordered_map;

template<typename Key, typename Value>
class LinkedHashMap{
    struct Node{
        Key key;
        Value value;
        Node* next;
        Node* prev;
    };

    public:
        LinkedHashMap(size_t capacity){
            createList(capacity);
        }
        ~LinkedHashMap(){
            delete[] nodes;
        }

        size_t getCapacity() const { return _capacity; }
        size_t size() const { return current_size; }

        void put(const Key& key, const Value& value);
        bool get(const Key& key, Value& value) const;
        bool remove(const Key& key);
    private:
        void createList(size_t capacity);
        void moveToFront(Node* p);
        void moveToBack(Node* p);

        size_t _capacity;
        size_t current_size = 0;
        Node* head;
        Node* tail;
        Node* nodes;
        unordered_map<Key, Node*> hash_map;
};

template<typename Key, typename Value>
void LinkedHashMap<Key, Value>::createList(size_t capacity){
    nodes = new Node[capacity+2];
    for(int i = 0; i<capacity+2; i++){
        nodes[i].prev = &nodes[i-1];
        nodes[i].next = &nodes[i+1];
    }
    nodes[0].prev = nullptr;
    nodes[capacity+1].next = nullptr;
    head = &nodes[0];
    tail = &nodes[capacity+1];

    _capacity = capacity;
}

template<typename Key, typename Value>
void LinkedHashMap<Key, Value>::moveToFront(Node* p){
    p->prev->next = p->next;
    p->next->prev = p->prev;
    
    p->next = head->next;
    p->prev = head;
    head->next->prev = p;
    head->next = p;
}

template<typename Key, typename Value>
void LinkedHashMap<Key, Value>::moveToBack(Node* p){
    p->prev->next = p->next;
    p->next->prev = p->prev;
    
    p->next = tail;
    p->prev = tail->prev;
    tail->prev->next = p;
    tail->prev = p;
}

template<typename Key, typename Value>
void LinkedHashMap<Key, Value>::put(const Key& key, const Value& value){
    auto it = hash_map.find(key);
    if(it == hash_map.end()){
        Node* p = tail->prev;
        hash_map.erase(p->key);
        moveToFront(p);
        p->key = key;
        p->value = value;
        hash_map[key] = p;
    }else{
        Node* p = it->second;
        moveToFront(p);
        p->value = value;
    }
}

template<typename Key, typename Value>
bool LinkedHashMap<Key, Value>::get(const Key& key, Value& value) const{
    auto it = hash_map.find(key);
    if(it == hash_map.end()){
        return false;
    }

    Node* p = it->second;
    value = p->value;
    const_cast<LinkedHashMap*>(this)->moveToFront(p);
    return true;
}

template<typename Key, typename Value>
bool LinkedHashMap<Key, Value>::remove(const Key& key){
    auto it = hash_map.find(key);
    if(it == hash_map.end()){
        return false;
    }

    Node* p = it->second;
    moveToBack(p);
    hash_map.erase(it);
    return true;
}