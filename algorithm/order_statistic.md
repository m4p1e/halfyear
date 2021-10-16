# 顺序统计量



### 0x00 选择问题

**Definition** 给定由$n$个元素组成的集合，第$i$个顺序统计量是该集合中第$i$小的元素. 

**Definition** 当$n$为奇数的时候，其中位数是$(2n+1)/2$; 当$n$为偶数的时候，其中位数有两个，分别是$\lfloor(n+1)/2 \rfloor,\lceil (n+1)/2 \rceil$. 

**Definition** 给定一个包含$n$个数的集合$A$和一个整数$i, 1 \leq i \leq n$.  求元素$x \in A$，其中$A$中有且仅有$i-1$个小于$x$的元素. 这样的问题我们称之为选择问题.



### 0x01 最大最小值

**annotation** 独立寻找最小值或者最大值所需要比较的次数为$\Theta(n)$，且是渐进最优的. 

**annotation** 同时寻找最小值和最大值。可以仅通过$3\lfloor n/2 \rfloor$次比较. 

```python
minimum_and_maximum(A)
	i = 1
    min = neg_inf
    max = pos_inf
    for i to |A|/2
    	if A[2i-1] > A[2i]
        	if(A[2i-1] > max){
                max = A[2i-1]
            }
            
            if(A[2i] < min){
                min = A[2i]
            }
        else
        	#symmetric as bleow
        	...
```



