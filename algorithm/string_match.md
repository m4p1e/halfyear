# 字符串模式匹配



### 0x00 提出问题

字符匹配问题的形式化定义

假设文本是一个长度为$n$的数组$T[1\cdots n]$，而模式是一个长度为$m$的数组$P[1\cdots m]$，其中$m \leq n$.  进一步假设$P$和$T$所含字符均来自于同一有限字母集$\sum$.   

1.  如果$0 \leq s \leq n-m$，并且$T[s+1,s+m] = P[1,m]$， 则称模式$P$在文本$T$中出现，且偏移为$s$. 
2.  如果$P$在$T$中以偏移$s$出现，那么称$s$是有效偏移. 否则称它是无效的. 



### 0x01朴素算法

时间复杂度为: $O(m(n-m+1))$



```python
navie_string_match(T,P)
	n = T.length
	m = P.length
	for s=0 to n-m
    	if T[s+1,s+m] == p[1,m]
    		print "found" 		
```



### 0x02 RK算法

现在统一假设字符串的字母集为$\{0,1,\cdots,9\}$，其中每个数字看做一个字符.   



**简单情况**

若$P$的长度$m$比较短，且将其转换至十进制的数字在计算机字的长度内，也就是说$P$的长度比较小，那么我们可以使用这样的算法

1. 计算$P$的十进制数字表示$p$.
2. 计算$n-m+1$个子串$T[s+1\cdots s+m ]$的数字表示$t_s$
3. 将$p$依次与$t_s$比较. 

对于$P$计算$p$，可以在时间$O(m)$完成，注意低位在$P[m]$. 
$$
p = P[m] + 10(P[m-1]+10(P[m-2]+\cdots+10(P[2]+10P[1])\cdots))
$$
对于计算$t_s$，可以使得在常数时间内根据$t_s$计算出$t_{s+1}$，在时间$O(n-m+1)$内计算出所有的$t_s$
$$
t_{s+1} = 10(t_s - 10^{m-1}*T[s+1])+T[s+1+m]
$$
实际上就是先将前m-1位都左移一位即忽略$T[s]$这里的高位，再补上最低位.  



**一般情况**

若不限制$P$的长度$m$，显然比较两个任意的长度数字是不实际的，因此这里考虑一个启发式的算法:

1.  选择一个合适的模数$q$.
2.  与简单情况一样计算$p$和$t_s$，并对它们模上$q$.  模出来的余数再作为对应的$p$和$t_s$. 
3.  若$p$和$t_s$相等，则其对应的字符串有可能是相同的; 若$p$和$t_s$不相等，则其对应的字符串一定是不相同的. 
4.  在$p$和$t_s$相等下，我们做进一步的字符串的比较，来确定是否相同. 

这里描述的实际是一个启发式的算法，相比于简单情况下，这里额外需要$O(m)$时间比较两个字符串最终是否相等. 

这里假设是$d$进制字母表，这里需要调整一下计算$t_{s+1}$的算法. 
$$
t_{s+1} = \left[ d(t_s - d^{m-1}*T[s+1])+T[s+1+m]\right]~\text{mod}~q
$$
为简化计算可以先$d^{m-1}~\text{mod}~q$来替换上式的$d^{m-1}$. 



- 最坏情况下所需时间为$O(m(n-m+1))$. 

- 算法的期望时间为$O(n+m)$. 



**分析RK算法**

RK算法其实就是对pattern取hash，同样对比较的字符串也逐长做hash.  对比朴素算法，在逐长做hash的时候可以做到$O(1)$，比较两个hash所需时间可以看做$O(1)$. 但是在最坏情况下RK算法和朴素算法所需时间都是一样的都需要$O(m(n-m+1))$. 



### 0x03 有限自动机算法

一个有限自动机$M$是一个5元组$(Q,q_0,A,\sum,\var)$，其中

- $Q$表示状态集合
- $q_0\in Q$是初始状态
- $A \subseteq Q$表示一个接受状态集合
- $\sum$ 是有限输入字母表
- $\var$表示函数$Q \times \sum \to Q$，称为转移函数. 

为了方便说明问题，可以递归定义一个终态函数$\phi(w) : \sum^* \to Q$，就是指$M$接受了字符串$w$之后最终落入了哪个状态，其递归定义为
$$
\phi(\varepsilon) = q_0, \\
\phi(wa) = \var(\phi(w),a)
$$

**Definition** 若字符串$w$是$x$的前缀，则记为$w \sqsubset x $，反之若$w$是$x$的后缀，则记为$w \sqsupset x$.  空串是任意字符串的前缀或者后缀. 

定义函数$\sigma_P(w) : w \to \{1,2,\cdots,m\}$表示$w$的后缀中蕴含$P$的最长子串的长度，即
$$
\sigma_P(w) = \max\{k: P_k \sqsupset x\}
$$
例如设$P=ab$，则$\sigma_P(\varepsilon)= 0,\sigma_P(ccaca) = 1,\sigma_P(ccab) = 2$.



**Proposition** 对于一个长度为$m$的模式$P$，$\sigma_P(w) = m$当且仅当$P \sqsupset w$. 



最主要是如何对一个模式$P$生成一个字符串匹配自动机?

对于给定模式$P=[1\cdots m]$

1. 状态集合为$\{1,2,\cdots,m\}$. 开始状态$q_0 = 0$，并且状态$m$是唯一接受的状态.

2. 对于任意的状态$q$和字符$a$，转移函数定义为$\var(q,a) = \sigma_P(P_qa)$，其中$P_q$表示$P$的长度为$q$前缀. 




**Lemma** 对任意的字符串$w$和字符$a$，有$\sigma_P(wa) \leq \sigma_P(w) +1$.

**Lemma** 对任意的字符串$w$和字符$a$，若$q=\sigma_P(w)$，则$\sigma_P(wa) = \sigma_P(P_qa)$. 

**Proposition** 上述定义的自动机，有等式$\phi(T_i) = \sigma_P(T_i)$成立，其中$T_i$表示$T$的一个子串. 



生成转移函数的代码

```python
generate_transfer_function(P,C)
	m = P.length
	for q = 0 to m
		for a in C 
        	#由前面的lemma, 对于长度q的后缀子串，P_qa的最大后缀子串长度最大为q+1, 这里q+2是为了和后面do-while操作保持一致
            #同时最大后缀子串只能是P本身. 
        	k = min(m,q+1) 
            while !is_suffix(P_qa, P_k)
            	k = k - 1
            transfer_func(q,a) = k   
    return transfer_func  

finite_automation_search(S, transfer_func, m) #m是terminal state
    m = S.length
    k = 0
    for i = 1 to m
    	k = transfer_func(k, S[i]);
    	if k == m
    		print "found at offset" i-m
```



需要$O(m|\sum|)$的预处理时间和$O(n)$的匹配时间. 



### 0x04 KMP

这个算法比较神奇，不需要显示地构造转移函数，也能实现$O(n)$的匹配时间.  其关键在于在失败匹配之后，我们根据某些信息跳过一些字符串

![image-20210917174022467](/home/maple/halfyear/images/image-20210917174022467.png)

上面利用了$P_3$是$P_5$的最长后缀子串.  这些信息我们都是可以提前计算出来，我们可以用一个$\pi$数组来记录，例如这里可以使得$\pi[5]=3$，那么在$P_6$匹配失败之后，也就是已经成功匹配了$P_5$，我们将匹配的指针往后移动$5-\pi[5]$.



那么如何设计这个$\pi$呢？ 也就是说当已经成功匹配了$P_q$之后，紧着失败了，那么最小的偏移应该是多少？最小的偏移等价于找到最长的后缀子串.  形式化地可以解释为给定子串$P_q$，求满足$P_k \sqsubset P_q$的$k$的最大值，其中$k < p$.  即
$$
\pi[q] = \max\{k : k < q~\text{且}~P_k \sqsubset P_q\}
$$
下面算法的构造是看起来不是那么容易.  首先需要理解一些分析性质才行. 



**Lemma** 定义
$$
\pi^*[q] = \{\pi[q],\pi^{(2)}[q],\pi^{(3)}[q],\cdots,\pi^{(t)}[q]\}
$$
其中$\pi^{(i)}[q]=\pi[\pi^{(i-1)}[q]]$，$\pi^{(t)}[q] = 0$.  那么$\pi^*[q] = \{k : P_k \sqsubset P_q\}$，即$\pi^*[q]$是$P_q$所有可能的后缀下标集合. 

*Proof.* 两边夹来证.  $\pi^*[q] \subseteq \{k : P_k \sqsubset P_q\}$是显然.  另一方向，假设$\{k : P_k \sqsubset P_q\} - \pi^*[q] \neq \emptyset$， 取$j$是$\{k : P_k \sqsubset P_q\} - \pi^*[q]$中最大的那个值，可以肯定是必有$j < \pi[q]$，那么我们就可以取$j'$是$\pi^*[q]$中比$j$大的最小值（前面的条件保证了$j'$的存在性），那么这里有$\pi[j'] = j$，从而$j \in \pi^*[q]$，这和假设是矛盾的.  



**Lemma** 如果$\pi[q] > 0$，则$\pi[q]-1 \in \pi^*[q-1]$.

*Proof.* 下图可以说明问题

 ![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAccAAAEPCAYAAAA3Rw9rAAAAAXNSR0IArs4c6QAABct0RVh0bXhmaWxlACUzQ214ZmlsZSUyMGhvc3QlM0QlMjJ3d3cuaW9kcmF3LmNvbSUyMiUyMG1vZGlmaWVkJTNEJTIyMjAyMS0wOS0xN1QxMiUzQTE0JTNBMTguODgzWiUyMiUyMGFnZW50JTNEJTIyNS4wJTIwKFgxMSUzQiUyMExpbnV4JTIweDg2XzY0KSUyMEFwcGxlV2ViS2l0JTJGNTM3LjM2JTIwKEtIVE1MJTJDJTIwbGlrZSUyMEdlY2tvKSUyMENocm9tZSUyRjkzLjAuNDU3Ny42MyUyMFNhZmFyaSUyRjUzNy4zNiUyMiUyMHZlcnNpb24lM0QlMjIxMy4zLjYlMjIlMjBldGFnJTNEJTIybmM5b2FCQmJUejV4OTJmOV91Mm0lMjIlMjB0eXBlJTNEJTIyZGV2aWNlJTIyJTNFJTNDZGlhZ3JhbSUyMGlkJTNEJTIyaUlwQXBZd0JqekJtSGRGSDhGZmQlMjIlM0U3VmhOYzVzd0VQMDFISk5CU0h3ZFl6dE5MNTNKVEE1cFRoMEZGTkJFUnE0UXNaMWZYd21FUVVCaVowemJTYWNuYXglMkZhWmZYMmFTWHN3T1Y2ZHlQd0p2JTJGR1U4SWN6MDEzRGx3NW5nZEFHS2tmamV3YkpQQU1rQW1hTnBEYkFYZjBsUmpQRnExb1NrcUROWkRrbkVtNnNjR0VGd1ZKcElWaElmaldudmJFV1dvQkc1eVJFWENYWURaRzcya3E4d2FOZkxmRHZ4S2E1ZTJiZ1d1ZXJIRTcyUUJsamxPJTJCN1VIdzJvRkx3YmxzUnV2ZGtqQk5uczNMbHplZUhoSVRwSkNuT0hpTnd3dG1sVm1ieVV2dTI4VUtYaFVwMGZOZEJ5NjJPWlhrYm9NVCUyRlhTcnlxdXdYSzZac29BYW1uQkVTTEo3TXlWd1dLaFNDT0ZySXNWZVRURU9FTURHeFlnRGhDYkV0cU1hb2JqQjhoN055SENLVFhXelElMkJpT0FEVXdIRXp6QVklMkZ6UVlyMFNtdElXUVV2aUwxJTJCc3FQeXU2YnEwdk9OJTJCV0NZMCUyQlBWcm0lMkZzVzZOUVdmYTl0UDNRUnRSRzUxZGJyV09URzBrSGFpMTVKUkppRlZoaWtSRnBRU2RVcE1kNHElMkIwJTJCNFMwbUNNT1N2dGhKVEZYQnZPR1dVJTJGWGlROEdET0xZTEhneENOT3N4WG4weER3S0ZhQkFJREFJMUxJd0MxYUk0TFBza25hRFpkQktwWnRnSjVjSzlkS0gzdmxxVWNVc0VWZGtTTVZaUUZNWWZrVkJ0RGNQOTE5WGYwcFYlMkZYRmZxeU5qb1lWSUp0bDhJbkR6ckFoeHJ6RjBYMTFaS2hUb1hLUyUyQlVyVmpReFppamRTTUlMS0lRR25WdUNJTnh3V1AlMkYlMkZNNGQlMkZIbm1DaTdtWWc0Q1pER25UcjB4ZFRHYzJDdmglMkJkU0ZJJTJCcHVmempoNHVjRmNNTFZpRVcxU0dremhCbk5OQiUyQkpXcXp1SVF0TkJWWFhwU3Z6WUUzVFZMc3ZCQ25wSzM2c1ElMkJsR3M5RmJwczdjWHpqJTJCU3NlcUpDJTJCYkM5OU05d2tVJTJCWmUlMkJUVzU3JTJGenpTaDBCMFByZlJCTGZQbjU1VFA0SkRUb094WHNIdm9UUSUyQnZ0UHRQWHRrZzVkUzhPZkRSVjRmdmslMkJVc2ZzMmE2Um40eVN2QkxuUnJxdW9BNjZLckg0bDhvM1hrak11Nmh3Z2VNU0E2R2lsYWpOSlBxT2dXNjdmdVNCNzBVU3ZRTjc1NUxlZllQJTJGSUNRVW1kT3VGM2dSM2NBYnV3RlF6dUFBakFqOWRPMERCb0IwQWQ5eGkwVHo5UUpuZDkzRnp4ZXIlMkJaWURYdndBJTNEJTNDJTJGZGlhZ3JhbSUzRSUzQyUyRm14ZmlsZSUzRTRE72sAABusSURBVHhe7d0LsF1Vfcfxbx6EGALElgSKOMqQoWjR+IRiHGgAYxlxClZnOgU1iBWilSGlplRaiI7YibbFoharMkGB1qojOEMlIb7QmijWFiP1QTMUNaZEaAkQEEIenRX3xcvl3rPPvWfvs9fje2ZSU+4+a/3/n//i/DjnnnvuNLwpoIACCiigwJMEpumhgAIKKKCAAk8WMBw9EQoooIACCowRMBw9EgoooIACChiOngEFFFBAAQV6C/jM0ROigAJNC/wrcBywp1r4HuBK4G9rNroQOBZ4c9MFuZ4CkxUwHCcr5vUKKFAnEMLxI8B11YWLgHXAm4Av9Liz4Vgn69eH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)

**Corollary** 如果设集合$E_{q-1}$为
$$
E_{q-1} = \{ k \in \pi^*[q-1]: P[k+1] = P[q]\}
$$
 则对于$q=2,3,\cdots,m$，有
$$
\pi[q] = \left\{ 
\begin{array}{ll}
0 & E_{q-1} = \emptyset \\
1+\max\{k \in E_{q-1}\} & other
\end{array} \right.
$$


我们根据最后这个推论:

1. $\pi[q]$值在是$\pi^*[q-1]$中满足$P[k+1]=P[q]$最大的值$k$加上1. 
2. 如果$\pi^*[q-1]$为空，那么$\pi[q] = 0$.  
3. $\pi[1] = 0$. 

使得可以递归的计算$\pi[q]$. 

```python
compute_prefix_function(P)
	m = P.length
	pi[1] = 0 
	k = 0
	for q=2 to m
    	#k总是等于pi[q-1]
		while k > 0 and P[k+1] != P[q] #第二个lemma
			k = pi[k] #这里用了第一个lemma，遍历pi[q-1]中的所有k    
        if P[k+1] == P[q] 
        	k = k+1 #第二个lemma    
        pi[q] = k 
        
kmp_search(S, P, pi)
   n = S.length
   k = 0 #k表示已经匹配了的最长前缀长度 
   for i=1 to n
    
        #找到最长前缀k
        while k > 0 and S[i] != P[k+1]
    		k = pi[k]
        
		#判定是否匹配k+1
    	if S[i] = P[k+1]
    		k = k + 1;
    	
        #k == P.length，即完成一次匹配
    	if k == P.length
    		print "found at offset" i-m;
            #开始下一轮匹配
    		k = pi[k]
```



分析性质

- 预处理需要$O(m)$时间，分析这个时间最主要是弄清for里面的while所消耗的时间. 首先$k$最多只能增加$m$次，而while里面的过程是一个$k$减小的过程，由于$k$是非负数，那么最多只可能减少$m$次，因此while消耗的总时间是$O(m)$. 
- 搜索时间需要$O(n)$时间，与上述是类似的分析手法. 



### 0xFF

**Problem** 当模式$P$中的所有字符都不相同时，如何加速朴素算法?

假设一次比较过程失败发生在$P[k]$这里

- 当$k\geq 3$时，我们可以知道成功匹配了$P[1,k-1]$对应$T[s+1, s+k-1]$. 由前提条件$P$中的每个字符是互异的，那么$T[s+2,s+k-1]$之中的字符肯定都是不等于$P[1]$的，因此我们可以将$s$向前移动$k-1$，即下一次从$T[s+k-1+1]$和$P[1]$开始比较. 
- 当$k==1$时，依然只能往前移动一位.
- 当$k==2$时，还是只能往前移动一位，中间没有位数让我们可以跳跃. 

```python
navie_string_match(T,P)
	n = T.length
	m = p.length
	for s=0 to n-m
    	i = 1
    	while T[i+s] == P[i]
    		i++
    	if i == m+1 #说明成功比较了m次
    		print "found with shift" s
    	else
        	#非常奇怪这里尽然不连续
        	if i == 1 or i == 2 # 
            	s = s + 1
        	else
        		s = s + i-1 #
    		
```



 







