# Divide and Conquer Algorithm
* divide 分: 递归解决较小的问题（子问题）
* conquer 治: 从子问题的解构建原问题的解

## 运行时间分析
T(N) = aT(N/b) + Θ(N^k)  
* 表示：把规模为N的问题分解成a个规模为N/b的问题，然后使用O(N^k)附加工作
* 解
    * T(N) = O(N^logb(a)) 若a>b^k
    * T(N) = O(N^k * logN) 若a=b^k
    * T(N) = O(N^k) 若a < b^k
    * (解P277)

## 例
* 平面最近点对O(NlogN)
* 归并排序
* 快速排序