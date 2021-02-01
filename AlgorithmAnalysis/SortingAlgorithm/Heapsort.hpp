inline int getLeftChild(int i){
    return i*2 + 1;
}

template<typename Element>
void percolateDown(Element a[], const int& n, const int& node){
    int next, i = node , tmp = a[node];
    
    for(i = node; getLeftChild(i) < n; i = next){
        next = getLeftChild(i);
        if(next + 1 < n && a[next] < a[next + 1]) next++;

        if(tmp > a[next]) break;

        a[i] = a[next];
    }
    a[i] = tmp;
}

template<typename Element>
void Heapsort(Element a[], int n){
    //buildHeap
    for(int i = (n-1)/2; i >= 0; i--){
        percolateDown(a, n, i);
    }

    //n次DeleteMax
    for(int i = n-1; i >= 0; i--){
        std::swap(a[0], a[i]);
        percolateDown(a, i, 0);    //堆在减小不要传n
    }
}