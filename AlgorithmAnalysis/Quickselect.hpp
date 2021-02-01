//基于快速排序思想选择问题算法
//选择第k最小值
#include"SortingAlgorithm\InsertionSort.hpp"

//三数中值分割法找到pivot,将pivot放到right-1位置上
template<typename Element>
Element Median3(Element a[], int left, int right){
    int mid = (left + right)/2;

    if(a[left] > a[mid]) std::swap(a[left], a[mid]);
    if(a[mid] > a[right]) std::swap(a[mid], a[right]);
    if(a[left] > a[mid]) std::swap(a[left], a[mid]);

    std::swap(a[mid], a[right-1]);
    return a[right-1];
}

template<typename Element>
Element _Quickselect(Element a[], int k, int left, int right){
    if(right - left < 20){
        InsertionSort(a+left, right-left+1);    //对小于20的数组直接使用插入排序
        return *(a+left+k);
    }

    int pivot = Median3(a, left, right);
    int mid = (left + right)/2;
    int i = left, j = right-1;      //使得从left+1，和right-2位置开始交换<-三数中值分割法已提前做了一些工作
    while(i <= j){
        while(a[++i] < pivot);
        while(a[--j] > pivot);
        if(i < j) std::swap(a[i], a[j]);
        else break;
    }
    std::swap(a[i], a[right-1]);

    if(k < i) return _Quickselect(a, k, left, i-1);
    else if(k == i) return a[k];
    else return _Quickselect(a, k-i-1, i+1, right);
}

template<typename Element>
Element Quickselect(Element a[], int n, int k){
    return _Quickselect(a, k, 0, n-1);
}