# 栈和队列



### 0x00 基本定义

先进先出队列，先进后出栈. 



实现方式:

1.  栈最基本需要一个栈顶指针，常见操作为插入push，取pop，检测栈的状态(空栈，满栈，栈的长度)。

2.  队列最基本需要两个指针一个队头和队尾，常见操作为插入enqueue，取dequeue，检测队列状态. 



存储结构：

1. 数组类型的连续空间。
2. 链表类型的离散空间。



### 0x01 考点



**1. 两个栈实现一个队列**

给定两个栈$S_1,S_2$和它们对应的操作函数$empty(S),push(S),pop(S)$.

```python
enqueue(x)
	push(S_1,x)

dequeue()
	if(empty(S_1)){
        print "error!"
    }
    
	while !empty(S_1) #导出来导进去，好无聊
    	y = pop(S_1)
        push(S_2,x)
	z = pop(S_2)
    while !empty(S_2)
    	y = pop(S_2)
        push(S_1,y)
    return z

empty()
	return empty(S_1)

full()
	return false
	
```

如何简化dequeue过程？ 可以不用考虑$S_1$总是存储是完整的队列。 这样$S_1.top$为队尾，$S_2.top$为队头. 

```python
dequeue()
	if empty(S_2)
        while !empty(S_1) 
    	y = pop(S_1)
        push(S_2,x)
    
    if empty(S_2)
    	print "error"
    else
    	return pop(S_2)
```





**2. 两个队列实现一个栈**

给定两个队列$Q_1,Q_2$和它们对应的操作函数$empty(S),full(S),enqueue(Q),dequeue(Q)$

```python
push(x)
	enqueue(Q_1,x)
pop()
	if !empty(Q_1)
        while !empty(Q_1) 
            y = dequeue(Q_1)
            if(!empty(Q_1)) #最后一个不应该插进去
                enqueue(Q_2,y)       
    else 
    	print "error"
    t = Q_1
    Q_1 = Q_2
    Q_2 = t
    return y

empty()
	return empty(Q_1)
```

当然也可以把push操作调整成$O(n)$

```python
push(x)
	Q_1.enqueue(x)
    while empty(Q_2)
    	y = Q_2.dequeue()
    	Q_1.enqueue(y)
    t = Q_2
    Q_2 = Q_1
    Q_1 = t
    
pop(x)
	return Q_2.dequeue()

empty()
	return empty(Q_1)
```



