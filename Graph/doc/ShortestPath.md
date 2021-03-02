## 最短路径算法
* 赋权路径长 weighted path length
* 无权路径长 unweighted path length
* 负值圈 negative-cost cycle

## 单元最短路径算法
*E:边的条数，V:顶点个数*
### 无权路径长
1. 从优先队列中弹出距离为d的顶点
2. 用该节点，将其邻接的未知的节点的距离更新为d+1，并将其入队
3. 1、2循环直至队列为空，初始时队列以含有起点开始(距离为0)
### 赋权路径长
#### Dijkstra
1. 初始时起点的距离赋为0，其余赋为无穷大
2. 找出未知的节点中距离最小的(使用*优先队列*较快)，标记其为已知(它不可能再被减小了)，用其更新其邻接的顶点的距离
3. 循环2，直至全部已知
6ff
* （如果使用不支持Decrease操作的堆，可以直接插入这个顶点的新的更小的距离数据，来屏蔽原来的旧数据。由于一个顶点可能多次出现在优先队列中，所有获取最小距离的顶点时，要判断一下这个顶点是否已知。）
* 使用邻接表与优先队列时为O(E logV) (*使用斐波那契堆(Fibonacci Heap)可以达到O(E+VlogV)*)
* **无法处理带负边值的图**
* **若图无圈时，可以使用拓扑顺序选择最小距离的顶点**（因为按照拓扑顺序，该顶点没有从未知顶点进入的边，其无法再被更新）O(E + V)

#### Bellman-Ford
1. 初始时起点的距离赋为0，其余赋为无穷大，先将起点入队
2. 将一个顶点出队，用该顶点到其邻接的顶点的边，更新其邻接的顶点的距离，并将其邻接的顶点入队（不在队列中时）
3. 循环2，直到队列为空
* 使用邻接表O(EV)
* 可以处理**带负边值但不带负值圈**的图

## 多源最短路径算法
### Floyd