# Graph 图
## 术语
* graph 图：一个图G=(V,E)，由顶点(vertex)集V和边(edge)集E组成。
* edge 边：一个点对(v,w)。有时称为弧(arc)。边有时带有**权重(weight)或值(cost)**
* digraph 有向图：点对有序，则图就是有向的(directed graph),叫做**有向图**。
* undirected graph 无向图
* adjacent 邻接：(v,w)∈E时，顶点v和w邻接（有序）。
* path 路径: 一个顶点序列w1,w2,...wn,使得(wi,wi+1)属于E(1<=i<N)。（简单路径：路径上各顶点都是互异的，第一个点和最后一个顶点可能相同。）
* path length 路径长：该路径上的边数。
    * weighted path length 赋权路径长：路径上所有边的权值的和
    * unweighted path length 无权路径长：该路径上的边数
* loop 环：路径v,v,一个顶点自己到自己。
* cycle 圈：w1 = wn且长至少为1的路径。（如果该路径为简单路径，则该圈为简单圈。对于无向图，要求边是互异的。）
* acyclic 无圈的
* DAG 无圈图：没有圈的图
* connected 连通的：在一个**无向图**中，从每一个顶点到每一个其他顶点都存在一条路径。
* strongly connected 强连通的：在一个**有向图**中，从每一个顶点到每一个其他顶点都存在一条路径。
* underlying graph 基础图：弧上去掉方向所形成的图。
* weakly connected 弱连通的：一个**有向图**不是强连通的，但它的基础图是连通的，则称它为弱连通的。
* complete graph 完全图：每一对顶点之间都存在一条边的图
* indegree 入度：v的入度为边(u,v)的数量，即指向该点的边的数量。
* outdegree 出度
* breadth-first search(BFS) 广度优先搜索：按层处理顶点，距开始点最近的那些顶点首先被处理。
* depth-first search(DFS) 深度优先搜索
* depth-first spanning tree 深度优先生成树
* biconnected 双连通 如果一个连通的无向图的任一顶点删除之后，剩下的图仍然连通，这样的无向图就是双连通的。
* articulation point 割点 被删除后使图不再连通的那些顶点叫做割点

## 图的表示/储存
### 邻接矩阵(adjacency matrix): 储存稠密的(dense)图
用一个二维矩阵储存图，A[u][v] = 1 表示存在边(u,v), A[u][v] = 0 表示不存在。也可以储存权值，用特殊的数(∞、0)表示不存在。
### 邻接表(adjacency list): 储存稀疏的(sparse)图
用一个表储存一个节点邻接的节点
### 直接存边
### 链式前向星
将起点相同的边集中起来,不同起点的边都在一个数组中，但互不干扰
```
next[i]       //下一条与这条边起点相同边
head[x]       //所加入的以x为起点的最后一条边的编号
to[i]         //该条边的终点
p = 0         //记录以及加入的边的数量

add(v, w){
    tp[p] = w;
    next[p] = head[v];  //接起来
    head[v] = p;
    p++;
}
```

## 算法
* 拓扑排序 topological-sort
    1. 初始：找到所有入度为0的节点加入队列
    2. 弹出队首元素，将其加入排序中，将其指向的点的入度-1
    3. 如果这个被指向的元素的入度减为了0，则将其加入队列
    4. 重复2，3直到队列为空
    5. 如果没有完全排序成功，则说明图中有环

* 最短路径
    * 出现负值圈(negative-cost cycle)时，该问题不确定
    * 单源最短路：
        * Dijkstra     O(M logN) （使用二叉堆）
        * Bellman-Ford O(NM)
    * 任意两节点间最短路：Floyd O(N^3)

* 最大网络流 
    * Push-Relabel算法(HLPP优化，BFS优化，GAP优化)

* 最小生成树
    * Prim O(M logN) （使用二叉堆）
    * Kruskal O(M logN)

* 求割点 Tarjan
    1. 深度优先搜索，并记录每个点的被访问的次序num[i]
    2. 再一次深度优先搜索，记录每个节点不经过父节点，而经过若干前向边和**一条**背向边可以到达的点的最小的num  
    
        一个点v的low[v] 是👇中最小的
        * num[v]
        * 其所有子节点w的low[w]
        * 其背向边所指向的点w的num[w]
    3. 其在深度优先生成树中，对于根节点，有两个及以上的子节点时，为割点；对于非根节点，若其low >= num，则为割点。

* 欧拉路径/环路
    * 对于无向图G，G上存在欧拉环路**当且仅当**G是连通的且没有奇度顶点。
    * 如果恰有2个奇度顶点，可以形成欧拉路径，但不能形成环路。（分别作为起点和中点）
    * Hierholzer 若不需要排序O(M + N)

* 强连通分支
    有向图的一些子集，这些子集是强连通的
    * Kosaraju 两次dfs O(M+N)