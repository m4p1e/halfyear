# 线性表

### 0x00 基本定义



**Definition** 线性表是由$n$个数据元素组成的有限序列$a[0],a[1],\cdots,a[n-1]$.  

**Definition** 按照线性表存储结构，可以分为

1. 顺序表，其元素存储在空间上是连续的，也是我们常称的数组。
2. 链表表，其元素之间通过指针相连接，在存储在可能是离散的。



**Definition** ==双向链表==$L$的每个元素都是一个对象，每个对象有一个关键字和两个指针$next，prev$. 设$x$为$L$上的某个元素，则$x.next$链接$x$的后继元素，而$x.prev$链接$x$的前驱.  $L$的第一个元素称为该链表的==头==，它是没有前驱的; $L$的最后一个元素称为该链表的==尾==，它是没有后继的。 



**Definition** ==单链表==$L$是在双向链表的基础上去掉了记录前驱的指针。



**Definition** 若链表是==已排序==的，则链表的线性顺序与链表中的关键字的线性顺序是一致. 若链表是==未排序==的，则各元素可以以任何顺序出现.



**Definition** 在双向链表中，将表头的$prev$指针指向表尾，而将表尾的$next$指针指向表头，我们就称该双向链表为一个循环链表。



### 0x01 线性表上的操作

主要一些操作

1. 插入结点
   - 总在是`L.head`插入，因此复杂度只需要$O(1)$.
2. 删除结点
   - 删除某个具体的关键字节点需要$O(n)$时间复杂度，在链表上的操作删除一个元素只需要$O(1)$.
3. 搜索结点
   - 时间复杂度$O(n)$.s 



### 0xFF 考点

**Problem** 在$O(n)$时间内逆转一个长度为$n$的单链表且只能用常数大小的额外存储空间.

```python
reverse_singly_list(L)
	x = L.head
    pre = NIL
    suc = NIL
    while x != NIL
    	suc = x.next
        x.next = pre
        pre = x
        x = suc
	L.head = pre
    return L
```

------

**Problem** 利用一个指针实现双链表.

假设所有指针都是视为$k$位整数型，每个列表元素有一个指针$x.np$，我们要用它来表示next和prev. 这里我们利用$\text{XOR}$来做，利用它的性质
$$
x~\text{XOR}~x~\text{XOR}~y = y \\
y~\text{XOR}~x~\text{XOR}~y = x
$$
 我们使得$x.np = x.next ~ \text{XOR} ~ x.prev$.  由于列表头$L.head.prev = nil$的特殊性，我们可以实现解码$x.np$来实现遍历整张列表. 

```python
list_search(L,k)
	prev = NIL
	x = L.head
	while x != NIL and x.key != k
		next = x.np ^ prev
        prev = x
        x = next
	return x

list_insert(L,x)
	y = L.head
    prev = NIL
   	if y == NIL
    	L.head = x
        x.np = prev ^ NIL
    else
    	x.np = prev ^ y
        y.np = y.np ^ prev ^ x
                
list_delete(L,x)
	prev = NIL
    y = L.head
    #要从头开始解码得到x.prev
    while y != x
    	next = y.np ^ prev
        prev = y
        y = next
    x_prev = prev
    x_next = x.np ^ prev
    
    if x_prev != NIL #判断x是不是L.head
    	x_prev.np = x_prev.np ^ x ^ x_next
    else 
    	L.head = x_next
	
    if x_next != NIL #判断x是不是L.tail
    	x_next.np = x_next.np ^ x ^ x_prev    
```

我们如果记录一个队尾指针$L.tail$就可以在$O(1)$时间内完成对该链表的逆转

```python
list_reverse(L)
	t = L.head
    L.head = L.tail
    L.tail = t
```

