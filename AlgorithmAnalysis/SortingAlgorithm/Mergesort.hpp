#ifndef _MERGESORT_
#define _MERGESORT_
template<typename Element>
void _Mergesort(Element a[], Element tmp[], int left, int right){
    if(right == left) return;
    
    int mid = (left + right)/2;
    _Mergesort(a, tmp, left, mid);
    _Mergesort(a, tmp, mid + 1, right);
    
    //merge
    int i = left, j = mid+1, k = left;
    while(i <= mid && j <= right){
        if(a[i] < a[j]){
            tmp[k++] = a[i++];
        }else{
            tmp[k++] = a[j++];
        }
    }
    while(i <= mid)   tmp[k++] = a[i++];
    while(j <= right) tmp[k++] = a[j++];
    
    //copy
    for(int k = left; k <= right; k++){
        a[k] = tmp[k];
    }
}

template<typename Element>
void Mergesort(Element a[], int n){
    Element* tmp = new Element[n];  //临时空间
    _Mergesort(a, tmp, 0, n-1);
    delete[] tmp;
}
#endif