# GreedyAlgorithm
分阶段的进行，在每个阶段做当前已知最好的工作，而不考虑下一步的操作，产生局部最优解。

## 例
* Dijkstra
* Prim
* Kruskal
* 调度问题（先处理占用时间小的，使得平均等待时间最小）
* Huffman编码

### Huffman编码
用于文件压缩(file compression)
#### 前缀码 Character code
将字符等信息放在**叶结点**上，形参一颗**满树**。
从根节点出发，用1指示向左，用0指示向右，到达叶节点时得到的序列即为叶结点上这个信息（字符）的编码。
#### 哈夫曼算法 Huffman's Algorithm
如何构造一个总价值最小（使信息占用二进制位最小）的编码树->哈夫曼编码(Huffman Code)
* 核心思想：让出现频率高的字符在树中的深度更低（编码其所用的bit更少）
* 初始时，所有出现的字符各自构成各成**单节点的树**。一棵树的权值为它的树叶的频率的和
* 每次任意选取最小权值的两棵树T1和T2，并以任意形式合并这两棵树
* 最后得到最优的哈夫曼编码树