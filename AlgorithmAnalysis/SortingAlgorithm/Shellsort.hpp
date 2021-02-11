#ifndef _SHELLSORT_
#define _SHELLSORT_
template<typename Element>
void Shellsort(Element a[], int n){
    int j,tmp;
    for(int d = n/2; d > 0; d>>=1){ //使用希尔增量
        for(int i = d; i < n; i++){
            tmp = a[i];
            for(j = i; j >= d && a[j - 1] > tmp; j -= d)
                a[j] = a[j - 1];
            a[j] = tmp;
        }
    }
}
#endif