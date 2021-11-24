# 并查集



### 0x00 基本定义



一个并查集（disjoint-set data structure）用于维护一堆不相交的集合$\mathcal{S} = \{S_1,\cdots,S_k\}$.  其中$S_i$表示第$i$集合的一个代表元，它是这个集合里面任意一个成员. 



在并查集上主要3个操作， 其中$x$表示一个==元素对象==. 

1.  make-set(x)  新建一个集合，这个集合只包含$x$.   因为集合之间是不相交，所以$x$只会存在于这个新建的集合中.
2.  union(x,y)  合并$x$和$y$各自所在的集合组成一个新的集合.  此时会消除集合$S_x$和$S_y$. 
3.  find-set(x) 返回一个指针，这个指针指向包含$x$的集合代表. 

如何定义额外的操作？

1. 对某个对象新增元素，可以使用make-set，再进行union.
2. 删除元素？



### 0x01 不相交集合的链表表示



链表表示的特点:

1. 每一个集合内的对象$x$，用一个单链表串起来，其中每个单链表结点中还多一个指向==集合对象==的指针$x.head$. 
2. 集合对象$S_i$包含一个head属性和tail属性，head属性指向链表的第一元素，tail指向链表的最后一个对象。

![image-20210911103043583](/home/maple/halfyear/images/image-20210911103043583.png)



在上述构造下，此时的$x$就是单链表的一个结点:

1. make-set(x) 创建一个集合对象和只有一个结点的单链表即可，只需要$O(1)$的时间.  
2. find-set(x)  直接访问$x.head$即可，只需要$O(1)$的时间.  
3. union(x,y)  直接将$y$所在的单链表拼接到$x$所在的单链表，同时需要更新$S_x.tail$和新拼进来链表结点$y.head$，并且删除$S_y$.  这过程所花费的时间需要与$y$所在链表的长度呈线性关系. 



对于链表表现下的$union$操作，若考虑在链表拼接的时候，总是把较短的表拼接到较长的表中，这一策略叫做==加权合并启发式策略==. 



**Proposition** 使用不相交集合的链表表示和加权合并启发式策略，一个具有$m$个$make,union,find-set$操作的序列（其中有$n$个make操作）需要时间为$O(m+n\lg n)$.  



### 0x02 不相交集合森林

若每个不相交的集合表示成一颗树，那么所有不相交集合放在一起就是一个不相交森林.  

集合对应的单个树的构造:

1. 树根指向自己;
2. 每个内部结点指向其父节点. 



![image-20210911103310722](/home/maple/halfyear/images/image-20210911103310722.png)



在上述构造下，用$x$表示一个元素对象:

1. make-set(x)  创建一个只含单个结点$x$的树;
2. find-set(x)  沿着$x$的父节点指针向上找到树根;
3. union-set(x,y)  将$x$所在树的树根指向$y$所在的树根.



**对union-set(x,y)的优化**

自然地优化方式就是在做union操作的时候，将内部结点较少的树的树根指向内部结点较多的树。这样做的好处是使得新生成的树在执行find-set操作的时候， 每个结点平均向上查找的次数要少。

对每个结点$x$维护一个属性$x.rank$用于记录$x$高度的上界，即$x$到叶子结点的最长路径上的边数 .  注意这里没有显式的维护每个结点子树的内部结点个数，在实际操作的时候是将具有较小秩的根指向具有较大秩的根，这一做法叫==按秩合并==.   



```python
make_set(x)
	x.p = x
    x.rank = 0

union(x,y)
	link(find_set(x),find_set(y))
    
link(x,y)
	if x.rank > y.rank
    	y.p = x
    else x.p = y
    	 if x.rank == y.rank
        	y.rank = y.rank+1
```



**Lemma** 每个结点的秩最大为$\lfloor \ln n \rfloor$.

**Proposition** 对于一个具有$n$个make-set的长度为$m$的操作序列采取按秩合并的操作，使用的时间为$O(m\ln n)$. 



**对find-set(x)的优化**

可以直接让树上内部结点指向所在根结点，那么在向上查找路径会可能就会变短，这一策略称作==路径压缩==.  路径压缩的策略应用在find-set操作中，如下伪代码，这个find-set会记录它查找的过程，那么可能下一次查询相同结点的时候，就可以直接使用记录的这个指针，这个方法也叫==两趟方法==（two-pass method）.   这个做的正确性是因为结点的向上查找的部分路径在union操作下是保持不变的。



```python
find-set(x)
	if x != x.p
		x.p = find-set(x.p)
	return x.p		
```



**基本分析性质**

- 单独使用按秩合并的时间复杂度为$O(m\ln n)$，$m$操作序列中有$n$个make-set，下同.
- 单调使用路径压缩的时间复杂度为$O(n + f\cdot (1+\log_{2+\frac{f}{n}}n))$，其中有$f$个find-set.
- 同时使用按秩合并和路径压缩时间复杂度为$O(m\alpha(n))$，对于所有实际应用均有 $\alpha(n) \leq 4$ 

 



### 0xFF 考点

**Problem** 利用disjoint-set来计算无向图$G$的连通量. 

```python
connected_components(G)
	for each v in G.v
		make_set(v)
	for each (u,v) in G.E
    	if find_set(u) != find_set(v)
    		union(u,v)
    
same_component(u,v)
	if find_set(u) != find_set(v)
		return true
	else
    	return false 
```

