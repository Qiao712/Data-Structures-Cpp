# Vector
vector是表的可增长的数组实现
在常量时间内通过索引随机访问,但插入删除较耗时
## 支持操作：
* size
* clear 
* empty
* push_back
* pop_back
* back
* front
* insert
* erase
* --↑与list共有
* operator[] 不包含边界检查
* at 包含边界检查
* capacity 获得内部容量
* reserve 设置容量

## 实现细节：
![image](Vector.png)