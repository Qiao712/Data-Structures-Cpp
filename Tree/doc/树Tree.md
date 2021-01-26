# 树 Tree
## 术语:
* root 根
* edge 边
* child 子节点
* parent 父节点
* leaf 叶节点
* sibling 兄弟节点
* path 路径：从节点n1 到 nk的路径定义为：节点n1 n2 n3 ...... nk 的序列， 使得对于1<=i<k的节点ni是ni+1的父节点。路径的长(length)即为该路径上的边的数量， 即k-1.
* depth 
深度：从**根节点**到ni的唯一路径的长。特别的，根的深度为0.
* internal path length 内部路径长：一棵树的所有节点的**深度**的和称为内部路径长。
* height 高度：从ni到一片树叶的**最长**路径的长。特别的，叶节点的高度都为0.
* ancestor 
* descendant
* proper ancestor
* proper descendant
* preorder traversal 先序遍历：对节点的处理在对其子节点的处理之前
* postorder traversal 后序遍历：对节点的处理在对其子节点的处理之后
* inorder traversal 中序遍历：先处理左节点，再根节点，最后右节点（对于二叉树）
* level-order traversal 层序遍历：深度为D的节点要比深度为D+1的节点之前处理

## 类型
* binary tree 二叉树：每个节点不多于两个子节点。 平均深度O(√N)
* 二叉查找树
* AVL树(Adelson-Velskii和Landis) 带有平衡(balance)条件的二叉树