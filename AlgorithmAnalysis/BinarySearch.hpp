//序列a升序，返回其中第一个 大于x的元素 的下标 [0, n) (std::upper_bound)
template<typename Element>
int upper_bound(const Element a[], int n, const Element& x){
    //l可以看作 可能大于x的最小的元素的下标, l之前[0,l-1]的元素已确定比x小
    //r可以看作 比x大的元素最小的下标，r和r之后[r,n)的元素一定都比x大， r即当前待定的结果
    int l = 0, r = n, mid;
    while(l < r){
        mid = (l + r)/2;
        if(a[mid] > x){
            r = mid;
        }else{
            l = mid+1;
        }
    }

    return r;   //等价于 l+1
}

//序列a升序，返回其中第一个 大于等于x的元素 的下标 [0, n) (std::lower_bound)
template<typename Element>
int lower_bound(const Element a[], int n, const Element& x){
    int l = 0, r = n, mid;
    while(l < r){
        mid = (l + r)/2;
        if(a[mid] >= x){
            r = mid;
        }else{
            l = mid + 1;
        }
    }
    return r;
}

