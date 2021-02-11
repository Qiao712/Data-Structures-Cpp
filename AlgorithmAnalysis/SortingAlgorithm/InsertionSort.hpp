#ifndef _INSERTIONSORT_
#define _INSERTIONSORT_
template<typename Element>
void InsertionSort(Element a[], int n){
    int j,tmp;
    for(int i = 1; i<n; i++){
        tmp = a[i];
        // for(j = i; j > 0; j--){
        //     if(a[j-1] < tmp) break;     //注意：不要直接写a[i]，因为其之后会立即被a[i-1]覆盖...
        //     a[j] = a[j-1];
        // }
        for(j = 1; j > 0 && a[j - 1] > tmp; j--)
            a[j] = a[j - 1];
        a[j] = tmp;
    }
}
#endif