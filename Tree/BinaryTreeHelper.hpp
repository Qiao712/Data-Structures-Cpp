#ifndef __BINARY_TREE_HELPER__
#define __BINARY_TREE_HELPER__

template<typename Node>
class BinaryTreeHelper{
    public:
        using Comparable = decltype(Node::element);

        static void                    doClear(Node* t, Node* nil);                          
        static Node*                   doCopy(const Node* t, Node* nil);                     
        static const Node*             doFindMin(const Node* t, const Node* nil);
        static const Node*             doFindMax(const Node* t, const Node* nil);
        static const Node*             doFind(const Comparable& x, const Node* t, const Node* nil);

        static void                    doPrint(const Node* t, const Node* nil,int n);
    private:
        BinaryTreeHelper() = delete;
};

template<typename Node>
void BinaryTreeHelper<Node>::doClear(Node* t, Node* nil){
    if(t == nil) return;
    doClear(t->left, nil);
    doClear(t->right, nil);

    delete t;
}

template<typename Node>
Node* BinaryTreeHelper<Node>::doCopy(const Node* t, Node* nil){
    if(t == nil) return nil;
    Node* new_node = new Node;
    new_node->element = t->element;
    new_node->left    = doCopy(t->left, nil);
    new_node->right    = doCopy(t->right, nil);
    return new_node;
}

template<typename Node>
const Node* BinaryTreeHelper<Node>::doFindMax(const Node* t, const Node* nil){
    const Node* node = t;
    while(node->right != nil){
        node = node->right;
    }

    return node;
}

template<typename Node>
const Node* BinaryTreeHelper<Node>::doFindMin(const Node* t, const Node* nil){
    const Node* node = t;
    while(node->left != nil){
        node = node->left;
    }

    return node;
}

template<typename Node>
const Node* BinaryTreeHelper<Node>::doFind(const Comparable& x, const Node* t, const Node* nil){
    const Node* node = t;
    while(node != nil){
        if(node->element == x){
            return node;
        }else if(x < node->element){
            node = node->left;
        }else{
            node = node->right;
        }
    }
    return nil;
}

template<typename Node>
void BinaryTreeHelper<Node>::doPrint(const Node* t, const Node* nil, int n){
    if(t == nil){
        return;
    }

    cout<<t->element<<endl;

    if(t->left != nil){
        for(int i = 0; i<n+1; i++) cout<<"  ";
        cout<<"L:";
        doPrint(t->left, nil, n+1);
    }
    if(t->right != nil){
        for(int i = 0; i<n+1; i++) cout<<"  ";
        cout<<"R:";
        doPrint(t->right, nil, n+1);
    }
}

#endif