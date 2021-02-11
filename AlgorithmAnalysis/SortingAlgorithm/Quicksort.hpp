#ifndef _QUICKSORT_
#define _QUICKSORT_
#include<InsertionSort.hpp>

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
void _Quicksort(Element a[], int left, int right){
    if(right - left < 20){
        InsertionSort(a+left, right-left+1);    //对小于20的数组直接使用插入排序
        return;
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

    _Quicksort(a, left, i-1);
    _Quicksort(a, i+1, right);
    
    return;
}

template<typename Element>
void Quicksort(Element a[], int n){
    _Quicksort(a, 0, n-1);
}
#endif