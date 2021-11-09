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



### 0x03 堆排序

见二叉树中堆.   

时间复杂度为$O(n\ln n)$. 



### 0x04 快排

```c
int better_partition(int *A, int start, int end){
    int r,i,j,t;\&\&

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



**基数排序**

- 前提假设: $n$个输入均为$k$进制$d$位数字.
- 算法特点: 从最低位开始对全体数字进行排序，直到最高位.  这个过程完毕，则排序完成. 
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

  - 三向切割: partition总是返回一个元素作为pivot，我们可以使用一个元素集合来作为pivot，即与被选择$x$相同元素我们找出来将其排除不进入递归的quicksort. 

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



**Promble**  描述一个算法用于寻找$n$个元素的数组$A$里面有所有的逆序对数量. 

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



