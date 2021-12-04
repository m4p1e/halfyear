# 排序

### 0x00 Preliminary



如果输入数组中仅有常数个元素需要在排序过程中存储在数组之外，则称该算法是==原址的==(in place)，插入排序是原址的，其内层循环是非常紧凑的。



### 0x01 插入排序

#### 伪代码

```c
void insert_order (int *A, int length) {
    int index=1; //可以固定第一个元素
    int i;
    int t;
    for(;index < length; index++){
        i=index;
        while(i > 0 && A[i-1] > A[i]){
        	t=A[i];
            A[i]=A[i-1];
            A[i-1]=t;
            i--;
        }
    }
}

void insert_order2 (int *A, int length) {
    int index=1; //可以固定第一个元素
    int i;
    int key;
    for(;index < length; index++){
        i=index;
        key = A[i];
        while(i > 0 && A[i-1] > key){//去掉了两次元素交换
            A[i]=A[i-1];
            i--;
        }
        A[i] = key;
    }
}

void insert_order3 (int *A, int length) {
    int index=1; //可以固定第一个元素
    int i;
    int key;
    for(;index < length; index++){
        key = A[index];
        i = index-1;
        while(i >= 0 && A[i] > key){//去掉一次减法
            A[i+1]=A[i];
            i--;
        }
        A[i+1] = key;
    }
}
```

以上算法性能依次增加。



#### 详细分析

- 时间复杂度: $O(n^2)$
- 符合原址性
- 算法特点: 总是先排好一部分，每次取一个剩下的元素，再进行比较排序，余类推直到无剩余元素。
- 在最好的情况下只需要$O(n)$就能排好.





#### 希尔排序A[]

算法思想概要: 

1. 根据步长$k$将整个即将要被排序的数表划分为多个子表，即$L_i = \{A[i], A[i+k],\cdots,A[i+\lfloor \frac{n-i}{k} \rfloor k ]\},i=1,2,\cdots,k$，

2. 每个$L_i$分别进行插入排序，

3. 显然若$k = 1$，就是对整张表进行了一次插入排序，此时结果已经出来了;  若$k > 1$，减小步长$k$再回到第一步. 



算法的motivation: 插入排序在对小规模的或者已经有序的数表进行排序的时候效果会比较好，希尔算法就是根据这两个因素来进行优化的. 希尔提出的步长变换是

-  $k_1 = \lceil \frac{n}{2} \rceil$.
- $k_{i+1}= \lceil \frac{k_i}{2} \rceil$



```python
shell_sort(A)
	k = A.length/2 # 初始化
	while k >= 1
    	for i = k+1 to n #前k个元素在各自的子表中作为第一个元素，因此我们从k+1出发
            	temp = A[i]
            	j = i-k
            	while j >= 1 and A[j] > temp
                	A[i+k] = A[i]
                    j = j - k
        		A[j+k] = temp
        k = k/2 #步长变换         
```





### 0x02 并归排序

```c
void merge(int *A, int start, int mid, int end){
    int *L, *R;
    int L_len, R_len, i, j, index;
    
    L_len = (mid-start+1);
    L = malloc(L_len*sizeof(int));
    R_len = (end-mid);
    R = malloc(R_len*sizeof(int));
   
    /*构造两个手牌准备拼*/
    for (i = 0; i < L_len; i++)
    {
        L[i]=A[start+i];
    }

    for (i = 0; i < R_len; i++)
    {
        R[i]=A[mid+1+i];
    }

    i=0;
    j=0;
    index=start;    

    while (index <= end)
    {   
        if(i < L_len && j < R_len){
            if(L[i] < R[j]){
                A[index]=L[i];
                i++;    
            }else{
                A[index]=R[j];
                j++;
            }
        }else{//不用哨兵就没办法合并两个if
            if(i < L_len){
                A[index] = L[i];
                i++;    
            }else{
                A[index] = R[j];
                j++;     
            }
        }
        index++;
    }

    free(L);
    free(R);    
}

void merge_order (int *A, int start, int end) {
    int mid;
    if(start < end){
        mid = (start+end)/2;
        merge_order(A, start, mid);
        merge_order(A, mid+1, end);
        merge(A, start, mid, end);
    }
}
```



详细分析

- 时间复杂度$O(n\lg n)$
- 算法特点: 选择一个位置将原序列拆成两个子序列分开排序（通常我们选择一份为2，或者随机选择），最后把排好序的部分序列在组合起来.  最主要是merge这个操作如何在$O(n)$下实现. 
- 最坏的情况下需要比较次数: $n\lceil \lg n\rceil - 2^{\lceil \lg n\rceil} + 1$. 
- 最好的情况下需要比较次数是最坏情况下的一般



### 0x03 堆排序

见二叉树中堆.   

时间复杂度为$O(n\ln n)$. 



### 0x04 快排

```c
int better_partition(int *A, int start, int end){
    int r,i,j,t;

    r = A[end];
    i = start-1; // i是用来记录那些比r小的元素中下标最大的，
    j = start; // j就是一个普通的index, 用于变量A[start-end].


    //这里会维持几个循环不变量: 
    //1. A[start,i] 都是小于等于r的
    //2. A[i+1，j-1] 都是大于r的
    while(j < end){
        if(A[j] <= r){ // A[i+1]和A[j]安全的替换
            i = i + 1;\&\&
            t = A[j];
            A[j] = A[i];
            A[i] = t;            
        }
        j++;
    }

    A[end] = A[i+1]; //这个替换是安全的
    A[i+1] = r;

    return i+1;
    
}

void quicksort(int *A, int start, int end){
    int mid;
    if(end > start){\&\&
        mid = better_partition(A,start,end);
        printf("mid = %d\n",mid);
        quicksort(A,start,mid-1);
        quicksort(A,mid+1,end);
    }
}   


int main(){
    setbuf(stdout,NULL);
    int A[]={2,5,3,1,6,4,5,9,1,3,5};
    quicksort(A, 0 , sizeof(A)/sizeof(int) - 1);
    return 0;
}
```

详细分析

- 时间复杂度$O(n\ln n)$
- 最坏情况下时间复杂度是$O(n^2)$. 
- 原址排序
- 实际排序应用最好的选择
- 算法特点:  选择序列中的一个数$r$，将比$r$小的归入数组$A$，将比$r$大的归入数组$B$，再分别排序$A,B$.  如此递归.  相比并排排序，它在解决子问题之前就做了一次排序，所以排完之后不需要再merge. 



###  0x05 特殊情况下的线性排序



**\&\&Theorem** 在最坏情况下，任何比较排序算法都需要做$\Omega(n \ln n)$次比较.  



**计数排列**

```python
counting-sort(A,B,k)
	let C[0..k] be a new array
	for i=0 to k
		C[i] = 0
	for j = 1 to A.length
    	C[A[j]] = C[A[j]] + 1
    for i=1 to k
    	C[i] = C[i]+C[i-1] #C[i] represent the number less than or equal(<=) i 
    for j=A.length downto 1
    	B[C[A[j]]] = A[j] #has C[A[j]] numbers less than or equal A[j], so we can put A[j] at the C[A[j]]
        C[A[j]] = C[A[j]]-1 #the position of C[A[j]] already used, so we let C[A[j]]-1 be the next position for put C[A[j]]
    	
```



- 前提假设: $n$个输入元素中的每一个都是在区间$[0,k]$内的整数，其中$k$也为整数. 
- 算法特点:  设置一个$k+1$长度的数组$C$，对于每一个输入的$x$，执行对应的$C[x] = C[x] + 1$.  执行完这样的操作之后，遍历数组$C$，将其记录的数字一字排开，这样访问访问每一个$C[i]$的时候，我们都可以知道$i$前面有元素，将$i$排在它们后面即可. 
- 分析性质: 整个运行过程所需时间为$O(n+k)$， 如果$k = O(n)$，那么显然该排序的运行时间为$O(n)$. 
- 计数算法不是原址的. 
- 计数排序是稳定的，稳定是指: 两个相同元素在输出数组中的次序和它们在输入数组中的次序是一样的.  ==这也是为什么计数排序最后一个循环是降序的==



**基数排序**

- 前提假设: $n$个输入均为$k$进制$d$位数字.
- 算法特点: 从最低位开始对全体数字进行排序，直到最高位.  这个过程完毕，则排序完成.  但是需要注意使用的排序算法需要是==稳定的==.
- 分析性质: 如果每一次位排序可以看做一个计数排序，因此整个运行过程需要时间$O(d(n+k))$.  



**Lemma** 给定$n$个$b$位的二进制数，如果可以找到某个整数$r \leq b$使得，对$[0,2^r-1]$上的数排序可以用参数为$k = 2^r-1$计数排序在$O(n+2^r)$时间内完成，那么我们就可以在$O((b/r)(n+2^r))$时间内排好这$n$个数.

应用上面这个lemma，我们可以强制将每个关键字分解为了$\lceil b/r\rceil$个$r$位的二进制数.  在一般情况下对于给定的$n$和$b$，我们希望选择的$r$可以得到的最优的运行时间.   

1. 如果$b < \lfloor \lg n\rfloor$， 那么对于任意的$r \leq b$都有$O((b/r)(n+2^r)) = O(n)$.  
2. 如果$b \geq  \lfloor \lg n\rfloor$，选择$r = \lfloor \lg n\rfloor$将得到偏差不超过常数系数范围内的最优时间代价. 

\&\&通常情况下，考虑$b = O(\lg n)$，我们可以取$r \approx \lg n$，那么应用上述lemma所需时间只需要$O(n)$.  



**桶排序**

- 前提假设: 假设$n$输入是均匀独立的分布在$[1,0)$上.
- 算法特点: 将区间$[1,0)$划分为$n$个相同大小的部分区间，每一个部分区间称为桶.  每个输入将均匀的落在每个桶里面，排序手法就是先对每个桶进行排序，再按次序遍历每个部分区间将元素一字列开即可. 

- 分析性质: 排序总代价为$O(n) + \sum\limits_{i =0}^{n-1}O(n_i^2)$，其中$n_i$表示第$i$桶里面的元素个数期望，平方阶是因为排序算法的最差也是$n^2$.  

  关于求$n_i^2$，可以看做$X \sim B(n,\frac{1}{n})$， 求$X^2$的期望，那么显然有$E(X^2) = 1 + n(\frac1n)(1-\frac1n) = 2 - \frac1n$，因此上述总的代价为$O(n)$. 

 

### 0x06 最坏情况下的一个界

**Definition** 若排序的最终结果依赖于各元素之前的比较，称这类算法为比较算法. 

显然前面提到的非线性排序都是比较算法. 



**Definition** 若给定任意$n$个元素，特定地排序算法的运行过程都对应了这$n$个元素的一个比较序列.  所有可能的比较序列我们可以通过一颗完全二叉树来描述，其中每个结点代表特定两个元素比较$<a_i,a_j>$，根结点到任意一个叶结点的简单路径对应一个完整比较序列.  

为什么可以揉成一个完全二叉树呢？因为每一次两个元素比较$<a_i,a_j>$ 会产生两个结果$a_i \leq a_j$或$a_i > a_j$ .  不同的排序算法会利用这个信息对应两个不同的后续比较过程，我们把这两个过程抽象成了两个不同的子树. 



**Theorem** 在最坏情况下，任何比较排序算法都需要做$\Omega(n\lg n)$比较.

*proof*. 在最坏情况下我们就是要考察决策树的高度，即根结点到叶子结点的最长路径. $n$个元素最多有$n!$种比较序列，那么这个决策树最少有$n!$个叶子结点.  自然有等式
$$
2^h \geq n!
$$
这里就可以得出$h \geq \ln(n!)$，更进一步我们利用Stirling's公式$\ln(n!) = n\ln n - n + \Theta(\ln n)$就可以得到命题中的结果.



**Problem** 对任意7个关键字进行排序，至少需要多少次关键字的比较?

注意这里的“任意”，并不说对某一串特殊情况的关键字，只要考虑最坏的情况.  使用前面的定理，这里的下界$\lceil \lg n! \rceil$.   

### 0xFF 考点

**算法改进**

- 插入排序
  - 若将插入的过程改成二分法，可以将最坏时间改善到$O(n\lg n)$.
  
- 并归排序
  - 哨兵的存在使得我们在while里面可以只用一个条件语句。
  - 可以去掉哨兵，那么只要一个堆里面的元素取完了就退出while，可以直接把另一个堆里面剩下的元素直接全部拷贝过来. 这样做你需要写两个while.  
  - 当子问题的规模变的足够小的时候，假设对应子问题为$k$.  不进一步对该子问题并归排序处理，而是直接使用插入排序. 在这种情况下最坏的运行时间为$O(nk + n\lg(n/k))$.  
  
- 快排

  - 小的子问题用插入排序

  - 对一个元素全部相同的数组进行排序的时候，每次部分排序操作返回的pivot都会是最后一个元素. 这样会造成性能的损失，因此考虑当元素全部相同时返回中间index，即返回$\lfloor (p+r)/2 \rfloor$

    ```python
    partition(A, p, r)
    	x = A[r]
    	i = p-1
    	n = 0 #记录与x相同的元素个数
        for j=p to r-1
        	if A[j] <= x
            	if A[j] == x
                	n=n+1
                i = i + 1    
              	exchange A[i] with A[j]
        exchange A[i+1] with  A[r]
        return i+1 - (n/2 + 1) #n/2 + 1表示取n/2的上界
    ```

  - median of three:  记录一个最坏的例子`1 2 3 4 5 6 7 8 0 `

  - 三向切割: partition总是返回一个元素作为pivot，我们可以使用一个元素集合来作为pivot，即与被选择$x$相同元素我们找出来将其排除不进入递归的quicksort. 此时partition会返回两个下标low,high使得$A[low-high]$之间的元素都是相等的. 

    ```python 
    partition(A,p,r)
    	x = A[p]
        low = p #维护两个index
        hight = p
        for j = p+1 to r
        	if A[j] < x
            	exchange A[low] with A[j]
                low = low + 1
                high = high + 1
                if(high != j)
                	exchange A[high] with A[j]    
            else if a[j] == x
            	high = high + 1
            	exchange A[j] with A[high]
         return low,high       
    ```
  
    
  
  - 尾递归
  
    ```python
    tail-recursive-quicksort(A,p,r)
    	while p < r
    		q = partition(A,p,r)
            if 2q > p + r #这样做的目的是我优先处理较小的子数组，让其早点处理完，尽量使得栈的深度较小
            	tail-recursive-quicksort(A,q+1,r)
                r = q-1
           	else
            	tail-recursive-quicksort(A,p,q-1)
                p = q+1
                
    ```
  
    



*proof*. ==对并归排序+k插入排序的最坏运行时间的证明==.  对每个$\lceil n/k \rceil$个长度为$k$的分布使用插入排序所需的总时间为$O(\lceil n/k \rceil \cdot k^2) = O(nk)$.  再考虑并归排序所需的合并总时间，因为$k\cdot2^h \leq n$，因此最多分解问题$\ln(n/k)$次，即合并总时间为$O(n \ln(n/k))$.  综上两个运行时间加起来就是总的运行时间



**算法选择** 

- 当输入越有序时，插入算法要不快排更有优势. 



**排序算法补充**

- 冒泡排序
  - 最坏运行时间为$O(n^2)$. 



**算法应用**

**Problem** 合并k个升序链表

方法1:  使用分治法，时间复杂度$n\lg k$

```python
merge(L, p, r)
	if p == r
    	return L[p]
    if p > r
    	return NIL
    q = (p+r)/2 #floor
    return merge_two_lists(merge(L, p, q), merge(L, q+1, r))
```

方法2: 使用最小堆，时间复杂度$n \lg k$. 最小堆总是维护$k$个元素. 

```python
kmerge(L)
	L2 = create_singly_linked_list()
	Q = create_min_priority_queue()
	for i = 1 to L.length
		insert(L, L[i].head)
	while !empty(Q)
    	e = extract_min(Q)
    	insert_at_tail(L2,e)
    	succ = e.next
    	if succ != NIL
    		insert(Q, succ)	
    return L2
```



**Problem** 描述一个运行时间为$O(n\lg n)$ 的算法，给定一个包含$n$数的集合$S$和另外一个整数$x$，该算法能确定$S$中是否存在两个其和刚好为$x$的元素. 

**Solution 1.  Sorting**

-  将$S$进行从小到大排序得到$Q$.
-  设置两个下标:  $l= 0, r=Q.size-1$
- 构造一个循环$while(l < r)$
  - 如果$Q[l] + Q[r] == x$，那么就得到了我们需要的pair
  - 如果$Q[l] + Q[r] < x$，那么$l=l+1$.
  - 如果$Q[l] + Q[r] > x$，那么$r=r-1$. 

这个解法依赖排序算法，只要我们选择一个运行时间为$O(n\lg n)$的排序算法就可以.  

实际上我们构造了一串和的数列去逼近$x$，其正确性我们可以分两个方面来讨论，

- 若$Q$不存在这样一对pair使得它们的和是$x$，那么显然上述算法也是得不到结果的; 
- 若$Q$至少存在这样这一对pair:  
  - 我们需要证明一个循环不等式， 假设这个pair为$(a,b)$，其中$a,b$表示$Q$的一个index，不失一般性假设$a < b$. 我们需要证明$l\leq a \leq b \leq r$.
    - 初始时显然是满足的.
    
    - 在一次循环中，若$Q[l] + Q[r] == x$，那么直接输出结果，退出循环; 若$Q[l] + Q[r] < x$， 即$Q[l]+Q[r] < Q[a] + Q[b]$，因为$Q[b] < Q[r]$，所以$Q[l] < Q[a]$，即$l < a$，从而$l+1 \leq a$，因此依然保持满足; 若$Q[l] + Q[r] > x$，同理可以得到$b < r$，从而$b \leq r-1$.
    
    - 循环终止条件$l > r$，显然它不满足上述不等式，因此在循环结束之前就已经退出了. 
    
      

**Solution 2 Hashing**

- 创建一个空的hashtable $T$
- 构造一个循环$i = 0 ~\text{to}~S.size$
  - 首先查询$T$是否存在$x-S[i]$，如果存在，那么我们已经找到了这样一对pair.
  - 如果不存在则把$S[i]$放进去. 

这个方法比较直接，但是需要使用一个hashtable. 正确性也是比较显然的. 



**Solution 3 Decomposition** 

这个方法是有限制，假设$S$的元素都是非负数.

- 创建一个大小为$x+1$的元素全为0的数组$R$
- 遍历$S$，若$S[i] \leq x$，则$R[S[i]] = R[S[i]] + 1$.
- 遍历$R$，$i = 0 \to \lfloor\frac{x}{2}\rfloor$ ，
  -  若$R[i]> 0 ~ \text{and} ~R[x-i] > 0$，那么我们则得到了这样一对pair
- 若上述没有得到我们想要的pair. 
  - 若$x$是一个偶数且$R[\frac{x}{2}] > 1$，那么我们也这样一对pair.

这相当于做一个整数的分解，这样所需的运行时间只需要是依赖$x$的，即$O(x+n)$. 

------



**Problem**  描述一个算法用于寻找$n$个元素的数组$A$里面有所有的逆序对数量. 

**Solution**  对$A$做一次类似并归排序的操作，同时修改merge过程，设左右两堆元素分别为$L$和$R$.

```python
reverse-order(A,p,r)
	if p < r
		q = p+r/2 # floor
        reverse = reverse-order(A,p,q) + reverse-order(A,q+1,r) + reverse-merge(A,p,q,r)
	else 
		reverse = 0
    return reverse

reverse-merge(A,p,q,r)
    #init L and R same with merge-sort
	#...
    reverse = 0
    i=1
    j=1
    for k=p to r
    	if i <= L.length and (j > R.length or L[i] <= L[j])
        	A[k] = L[i]
            i++
        else 
        	A[k] = R[j]
            j++
            reserve = reserve + (L.length - i + 1) # L里面还剩余的元素相对于A[j]来说都是逆序数	
    return reverse
```



------

**Problem**  medium of three类型的快排的分析性质. 

设给定一个$n$个互异元素的数组$A$（$n \geq 3$）， 假设$A'$为排好的数组.  我们要探究的medium of three方法究竟能对产生一个好的划分带来什么影响？ 因此首先我们要给出挑出的中位数概率分布，设$p_i = P\{x= A'[i]\}$.  那么
$$
p_i = \frac{(i-1)(n-i)}{C_n^3} = \frac{6(i-1)(n-i)}{n(n-1)(n-2)}
$$
计算上述概率的过程为:

- 从$n$个元素里面挑3个元素一共有$C_n^3$种可能.
- 当我们选定$A'[i]$之后，以它作为中位数的$3-\text{Set}$ 的形式一定是在$A'[i]$的左边挑一个元素，然后在$A'[i]$右边挑一个元素. 因此这样的挑法有$(i-1)(n-i)$种可能. 

最好划分就是直接把$A$的中位数挑出来了，相对于平凡的快排实现，medium of three能给把这种挑法的可能提高多少呢？$A$的中位数为$A'[\lceil n/2 \rceil]$，那么
$$
\lim\limits_{n \to \infty} \frac{\frac{6(\lceil n/2 \rceil-1)(n-\lceil n/2 \rceil)}{n(n-1)(n-2)}}{\frac{1}{n}} = \lim\limits_{n \to \infty} \frac{6(\lceil n/2 \rceil \lfloor n/2 \rfloor-\lfloor n/2 \rfloor)}{(n-1)(n-2)} = \frac{3}{2}
$$
其中$\frac1n$表示平凡快排下选择中位数的概率. 

进一步我们认为一个“好”的划分意味着主元选择$x=A'[i]$，其中$n/3 \leq i \leq 2n/3$. 那么medium of three下主元落在这个区间的概率是多少呢？ 直接取个积分，同时对$n$取极限
$$
\lim\limits_{n \to \infty} \sum\limits_{i = n/2}^{2n/3} \frac{6(i-1)(n-i)}{n(n-1)(n-2)} = \int_{n/2}^{2n/3} \frac{6(i-1)(n-i)}{n(n-1)(n-2)} = \frac{13}{27}. 
$$
在平凡的快排下这个概率应该为$1/3$.  

------

**Problem** 设计算法对$n$个区间$[a_i,b_i]$进行模糊排序，即最后得到一个区间序列$I_1,I_2,\cdots,I_n$使得对任意$i < j$都能找到$c_i \in I_i,c_j \in I_j$满足$c_i \leq c_j$.  

可以通过对每个区间的左端点进行排序，同时利用区间重叠这个性质加速我们的操作.  如果两个区间有重叠的部分，那么这个两个区间之前的order是没有限制的，分析一个极端情况: 如果$n$个区间都相互重叠，那么它是天然满足模糊排序输出的条件，这种情况下我们不需要对所给区间再排序.  当我们选择用快排来解决上述问题的时候，当我们选择一个区间作为pivot的时候，我们可以找出所有与它重叠的区间，它们构成的集合可以作为一个pivot，这就好比我们在快排里面利用三向法处理相同元素所做的的操作. 

```python
find_intersection(A, p, r)
	low = A[r].a; #A[r]是我们选择作为pivot的区间
    high = A[r].b
    for i=p to r-1
    	if A[i].a <= b and A[i].b >= a # 当两个区间[a,b],[c,d]不相交的条件为a > d or c>b，因此if条件是它的一个对立的形式
    		low = max(A[i].a, a) # 取两个区间的交
    		high = min(A[i].b, b)
   	return a,b 

parition_right(A,a,p,r) #右边都是A[j].a > a 且和A[r]不相交的区间 
	i = p-1
    for j=p to r-1
    	if A[j].a <= a
        	i=i+1
            exchange A[j] with A[i]
    exchange A[i+1] with A[r]
	return i+1
	
parition_left(A,a,p,t) #当且
	i = p -1
    for j=p to t-1
    	if A[j].b < b
        	i=i+1
        	exchange A[j] with A[i]
    exchange A[i+1] with A[t]
    return i+1
    
fuzz_order(A,p,r)
	a,b = find_intersection(A, p, r)
    t = partion_right(A,a,p,r)
    q = partion_left(A,b,p,t)
    fuzz_order(A, t+1, r)
    fuzz_order(A, p, q-1)

    
```

------

**Problem** 设计一个算法，它能够对于任何给定的介于$0$到$k$之间的$n$个整数进行预处理，然后在$O(1)$时间内回答输入的$n$个整数有多少个元素落在$[a,b]$内.  该算法总的运行时间为$O(n+k)$.

以计数排序作为预处理得到一个counting数组$C$，其中$C[i]$表示小于等于$i$的整数个数. 因此有$C[b]-C[a-1]$个元素落在$[a,b]$中. 

------

**Problem** 设计算法在$O(n)$时间内，对$0$对$n^3-1$区间内的$n$个整数进行排序. 

首先将每个整数转换成$n$进制，因此每个整数最多有$\log_n n^3 =3$位，再采用基数排序.  

------

**Problem** 修改对$n$个在区间$[1,k]$的整数计数排序，额外使用$O(k)$大小空间使得其变为原址排序. 

```python
counting_sort_in_place(A,B,k)
	let C[1..k] be a new array
	for i=0 to k
		C[i] = 0
	for j = 1 to A.length
    	C[A[j]] = C[A[j]] + 1
    B = C #拷贝一个B数组出来表示每个整数i的个数.   
    for i=1 to k
    	C[i] = C[i]+C[i-1] #C[i] represent the number less than or equal(<=) i 
    
    #此时利用B和C，我们可以直接确定A[j]的正确位置C[A[j]-1] + B[A[j]]. 
    for j=1 to A.length
    	#当A[i]的位置不是i的时候，我们把它交换到正确的位置，直到A[i]位置是i. 
    	while B[A[j]] > 0 and C[A[j]-1] + B[A[j]] != j 
        	t = A[j]
            swap A[j] with A[C[A[j]-1] + B[A[j]]]
        	B[t] = B[t]-1
    return A        
```

------

**Problem** 给定一个整数数组，其中每个整数的位数可能不同，但所有整数位数之和为$n$. 设计一个算法，使其可以在$O(n)$时间内对该数组进行排序. 

我们的方法是使用桶排序:

1. 给每个$1$到$n$分布设置一个桶，把相同位数的整数分别都扔到对应的桶里面. 
2. 对每个桶里面的元素进行基数排序.
3. 最后依次把所有的桶里面的元素聚合. 

------

**Problem** 给定一个字符串数组，其中每个字符串的长度可能不同，但所有字符串总长为$n$. 设计一个算法，使其可以在$O(n)$时间内对该数组进行排序(字符串排序遵从字典序).

我们方法是递归使用桶排序

1.  给字母表里面所有的字符都设置一个桶，然后按照每个字符串的第一个字符把它们丢到对应的桶里面.
2.  对每个桶里面的字符串，去掉第一个字符串.  我们把去掉字符串之后为空的字符串从桶里依次取出来聚合. 
3.  继续对剩下的字符串执行上述操作，直到排序完成. 

------

**Problem** 设计一个算法在$O(n\lg n/k)$对一个长度为$n$个元素的数组进行k-sort. 其中k-sort是指
$$
\frac{\sum\limits_{j=i}^{i+k-1}A[j]}{k} \leq \frac{\sum\limits_{j=i+1}^{i+k}A[j]}{k}
$$
我们只需要用快排将其排到规模大小为$k$的子数组就行，因此只需要时间为$O(n\lg n/k)$. 

------

**Problem** 给定一个长度为$n$元素的k-sort数组$A$，在时间$O(n\lg k)$将其完全排序.

这里有一个lemma是**一个长度为$n$的数组是k-sort当且仅当对任意的$i = 1,2,\cdots,n-k$有$A[i] \leq A[i+k]$**.   因此我们可以构造出来$k$个完全排序的列表$L_i = \{A[i], A[i+k],\cdots,A[i+\lfloor \frac{n-i}{k} \rfloor k ]\}$，按照前面对$k$个链表的并归排序可知我们需要$O(n \lg k)$时间. 

------

**Problem** 若给定每个待比较元素有两个属性$k_1,k_2$,  排序结果需要瞒住字典序的形式$k_1$大则大，$k_1$相同且$k_2$大则大. 我们应当如何选择排序算法？

- 首先比较$k_2$，再比较$k_1$. 
- 比较$k_2$的排序算法可以任意选择. 
- 比较$k_1$的排序算法需要具有稳定性. 

结果可以推广到一般情况$m$个属性上. 

------

**Problem** 如何在$1000$个数里面找最小的$10$个最小元素?

构造最小堆当前来说是最好的. 



