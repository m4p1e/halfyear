# 图上的算法



### 0x00 图的定义



**基本定义**

用$G=(V,E)$表示一张图，其中$V$表示图中的结点，$E$表示图上两结点的边。 如果对某条具体的边进行区分，可以分为无向图和有向图。 根据$V$和$E$之间关系，若$|E| << |V|^2$ ，则称$G$是一个稀疏图，若$|E| \sim |V|^2$，则称$G$是一个稠密图。



### 0x01 图的两种表示



- 邻接链表

  把图上的所有的结点$V$用一张链表$L$连起来，其中$L$上每个的节点$v$又是一张链表$L_v$的头，表示从$v$出发的边所对应的结点，即$v \to u$，则$u \in L_v$.

- 邻接矩阵

  初始化一个$|V| \times |V|$ 零矩阵$A=\mathbf{0}$，若存在$u \to v \in E$， 则标记$A_{uv} = 1$，显然我们可能用正整数标记图上不同的结点。

两种表示方式都可以很自然的表示无向图和有向图，特别地如果是无向图，那么对应的邻接矩阵是一个对称矩阵。 



**偏向使用**

- 邻接链表
  - 稀疏图
  - 若查找$u \to v$，需要遍历$L_u$。
  - 空间存储为$O(|V|+|E|)$，存储空间使用上可能小于邻接矩阵，但渐进上是一样的。
  - 鲁棒性高。
- 邻接矩阵
  - 稠密图
  - 查找某条边是否存在是极为方便的。
  - 空间存储度为$ O(|V|^2)$，在无向图下，由于是对称矩阵，所以只需要对角线和对角线上放的空间。
  - 每个记录项只需要1位空间。



### 0x02 广搜

广度优先搜索是图上一种遍历算法:

1. 选择以某个源节点$s$为开始，初始化一个队列$Q$，把$s$的子节点都扔到里面，

2. 再依次从$Q$里面取节点出来探索，

3. 若碰到第一次探索的节点，则将所有的子节点也加到队列$Q$里面。

4. 直到$Q$为空时。

   

```python
BFS(G, s):
    for each vertex u in G.V-{s}:  # 初始化
        u.color = WHITE
        u.d = infinity
        u.pi = NIL
        
    s.color = GRAY
    s.d = 0
    s.pi = NIL
    Q = {}
    ENQUEUE(Q, s)
    while Q is not empty:
        u = DEQUEUE(Q)
        for each v in G.Adj[u]:
            if v.color == WHITE:
                v.color = GRAY
                v.d = u.d + 1
                v.pi = u
                ENQUEUE(Q, v)
        u.color = BLACK
```



**基本分析性质**

- 广搜的==复杂度==$O(E+V)$. 



**关于Coloring的介绍** 已经探索过的结点标记为黑色，未被探索但是为黑色结点的边界结点标记为灰色，其余的结点标记为白色。

**关于结点属性d**  用$v.d$表示从当且广搜树根结点$s$到$v$的路径长度。



**Definition** 对于给定$G=(V,E)$和源结点$s$在广搜下，定义$G$的前驱子图为$G_\pi = (V_\pi,E_\pi)$，其中$V_\pi = \{v \in : v.\pi \neq NIL\} \cup \{s\},E_\pi = \{(v.\pi, v): v \in V-\{s\}\}$.  若$G_\pi$上每一个结点$s$都可达，且只包含唯一的简单路径，而且是最短路径，则称$G_\pi$是一个==广度优先树==（广度优先生成树）.  $E_\pi$中的边称为树边。

**Lemma**  BFS会生成一个广度优先树。

**Definition** 在广搜的过程中，若在探索$u \to v$时，其中$v$是白色结点，则在对应的广搜树中，称$u$是$v$的前驱或者父节点，记为$u = v.\pi$。 

**Definition** 记$\var(s,u)$表示$G$ 上$s$到$u$之间的最短路径。

**Theorem** 以源结点$s$为根结点的广搜树上的结点$v$，都是$G$上可以由$s$可达的，且树上$s \leadsto v$是$G$上$s$到$v$的==最短路径==. 特别地若$u$是$v$的直接前驱，则$\var(s,v) = \var(s,u)+1$。

**Lemma** 给定$G=(V,E)$，设$s \in V$，则对任意边$(u,v) \in E$，有$\var(s,v) \leq \var(s,u) + 1$。 

这是一个有趣的bounder，无所谓$s$与$u,v$是否可达。

**Lemma** 对广搜树上任意的结点$v$，都有$\var(s,v) \leq v.d$.   

只要证明这个上界等号是成立的，则可以说明广搜得到的确实是最短路径。 

**Lemma** 在广搜的过程中任意深刻队列$Q$中最多只包含两个不同的$d$值，即考虑$Q=\{v_1,\cdots,v_r\}$，则有$v_r.d \leq v_1.d+1$，同时有

$v_i \leq v_{i+1}, i = 1,\cdots,r_1$.   换句话说$v_i.d$不降，且也不大于最后一个结点$v_1.d+1$，这是更为强的一个结论。

这是刻画广搜总是从边界水平向外推进的最根本的的原因， 需要考虑$Q$在入列和出列时操作。 

**Lemma** 在广搜的过程中若$v_i$比$v_j$先入列，且它们在某一时刻都在队列$Q$里面，则$v_i.d \leq v_j.d$ 。



### 0x03 深搜

深度优先搜索的步骤：

1. 选择以某个源结点$s$，选择它最近的出发边进行探索，
2. 对每一个的结点（无论是新的或者探索过的），都会选择它最近的出发边，且该出发边对应的端点没有被探索过进行探索。（DFS-VISIT）
3. 若新发现的结点没有出发边，则回到它在搜索过称的前驱结点进行继续探索。
4. 若$s$的出发边都被探索完毕，则在$G$上选择一个没有被探索过的结点继续探索。 （DFS）



````python
DFS(G):
    for each vertex u in G.V:  # 初始化
        u.color = WHITE
        u.pi = NIL
    time = 0
    for each vertex u in G.V:
        if u.color == WHITE:
            DFS-VISIT(G, u)  # 以u为源结点进行深度优先探索
            
DFS-VISIT(G, u):
    time = time + 1
    u.d = time
    u.color = GRAY            # 发现
    for each v in G.Adj[u]:   # 探索u的邻接结点
        if v.color == WHITE:  # 若发现新的未探索结点
            v.pi = u
            DFS-VISIT(G, v)   # 优先递归探索新的结点v
            # 探索完v后，循环继续进行，探索u的下一个邻接结点
    u.color = BLACK
    time = time + 1
    u.f = time                # 探索完成            
````



**基本分析性质**

- 时间复杂度为$O(V+E)$. 
- 深搜相对于广搜而言它会遍历整张图，而广搜只会从一个结点开始，遍历所有它可达的结点. 



**关于时间区间** 将$v$第一次被发现的时间记为$v.d$，完成对$v$所有出发边并回到$v$时的时间记为$v.f$. 

**关于Coloring的介绍** 已经探索完成的的结点标记为黑色，出发边还没有探索完毕的结点标记为灰色，其余的结点标记为白色。



**Theorem** $G=(V,E)$在深搜下， 对于任意$u,v \in V$, 仅满足下述三种关系中的一种

1. 若$[u.d,u.f] \subset [v.d,v.f]$ ，则$u,v$在同一棵深度优先树中，且$u$是$v$的子孙结点;
2. 若$[v.d,v.f] \subset [u.d,u.f]$ ，则$u,v$在同一棵深度优先树中，且$v$是$u$的子孙结点;
3.  若$[v.d,v.f]$和$ [u.d,u.f]$ 完全分离，则$u,v$不在同一颗深度优先树中。



**Definition** 当DFS-VISIT生成以$u$为根结点的树称为深度优先树，即$T_u = (V_u, E_u)$，其中$V_u = \{v \in V: u.d < v.d < v.f < u.f \} \cup \{u\}, ~ E_u = \{(v.\pi,v): v \in {V_u}-{u}\}$.   

**Definition** 在深搜下，$G$的前驱子图定义为$G_\pi = (V,E_\pi)$，其中$E_\pi = \{(v.\pi,v): v.\pi \neq NIL\}$.   $G_\pi$是由多个深度优先树构成的深度优先森林。

**Definition** 对$G$进行深搜，根据生成的深度优先森林$G_\pi$，我们定义4种类的边：

1. **树边**（tree edge）  若$(u,v) \in E_\pi$，即在深搜的过程结点$v$是因为对边$(u,v)$的探索而首先被发现的，则$(u,v)$是树边。
2. **后向边** （back edge）若$(u,v) \in E$，且$v$是$u$在深搜树上的祖先结点。
3. **前向边** （forward edge）若$(u,v) \in E$，且$v$是$u$在深搜树上的子孙结点。
4. **横向边** （cross edge）不满足上述三种边的所有边， 即两个不同深搜树的边或同一个深搜树两个互不为子孙结点的边（没什么特殊关系的两点的边）。

注意对边的分类是指: 我们在深搜的过程中第一次碰到某个边它是怎样的而言.  

**Proposition**  在深搜过程中，探索$(u,v)$时，$v$的颜色可以给定一些关于$(u,v)$的信息

1. 若$v$是白色，则$(u,v)$是一条树边;
2. 若$v$是灰色，则$(u,v)$是一条后向边;
3. 若$v$是黑色，则$(u,v)$可能是一条前向边或者横向边。



**Theorem** 对无向图进行深搜的过程中，不会出现前向边和横向边。换言之只有树边和后向边. 

对任意一条边$(u,v)$，我们可以不失一般性的假设两个顶点$v.d < u.d$来分别讨论边是什么类型. 



### 0x04 拓扑序

对于一个有向无环图$G$，其==拓扑排序==是$G$中所有结点的一种线性次序，该次序满足: 若$(u,v) \in G$，则$u$在拓扑序中应处于$u$的前面，因为可以想象如果$G$有环，是不可能排出一个拓扑序的。 



进行拓扑排序的步骤：

1. 对$G$先进行深搜，计算出所有结点的完成时间$v.f$，
2. 将所有结点按照$v.f$从大到小的顺序依次排列，得到的就是拓扑序。



**基本分析性质**

- 时间复杂度和深搜的复杂度保持一致$O(V+E)$
- $v.f$最大的结点对所有结点安全，先处理它一定是safe的. 



**Lemma** 一个有向图$G$无环当且仅当对其深搜时不产生后向边。



### 0x05 强连通分量

有向图$G=(V,E)$的一个==强连通分量==一个最大结点集合$C \subseteq V$，对应任意的$u,v \in C$，路径$u \leadsto v$和$v \leadsto u$，即$C$中任意两个结点都是相互可达的。



**Definition** 用$G^T=(V,E^T)$表示$G=(V,E)$的转置图，即若$(u,v) \in E$，则$(v,e) \in E^T$.

**Lemma** 图$G^T$和$G$的强连通分量相同。



寻找强连通的步骤:

1. 对$G$进行一次深搜，计算出每个结点的完成时间$v.f$，
2. 对$G^T$在main loop中按照结点$v.f$递减的顺序再进行一次深搜，此时每个深搜树都是一个强连通分量。



**基本分析性质**

- 时间复杂度也和深搜保证一致$O(V+E)$，其中$V+E$等价与输入图的规模大小，所以这里可以说相对于$G$而言消耗了线性时间. 



**Definition** 有向图$G$的==分量图==示为$G^{SCC} = (V^{SCC},E^{SCC})$，设$G$的强连通分量为$C_1,\cdots,C_r$，将每个连通分量收缩为一个点记为$V^{SCC}=\{v_1,\cdots,v_2\}$.  若对于$x \in C_i, y \in C_j$，边$(x,y)$存在，则$(v_i,v_j) \in E^{SCC}$.  



**Lemma** 设$C,C'$为有向图$G$上的两个不同的强连通图，设$u,v \in C$和$u',v' \in C'$，若存在路径$u \leadsto u'$，则必不存在$v' \leadsto v$.

这是在说明分量图是一个有向的无环图。



**Definition** 将结点的发现实际和完成时间推广到结点的集合上:  $d(U)= \min\limits_{u \in U}\{u.d\},~ f(U)= \max\limits_{u \in U}\{u.d\}$.  下述在谈论这些时间是指的第一次深搜计算出来的时间。 

**Lemma**  设$C,C'$为有向图$G$上的两个不同的强连通图，设$u \in C$和$v \in C'$，若边$(u,v) \in E$存在，则$f(C) > f(C')$.   

这个结论想一下还是很显然的，因为$(u,v)$的存在，在深搜经过这条边的时候，在遍历完$C'$之前，$C$是不会遍历完的。

**Corollary** 设$C,C'$为有向图$G$上的两个不同的强连通图，设$u \in C$和$v \in C'$，若边$(u,v) \in E^T$存在，则$f(C') > f(C)$. 

这个推论和前面的lemma是等价，它可以保证我们在$G^T$上深搜的时候，我们对完成时间最大的连通分量进行搜索是safe的，主要确保不会跑到其他的连通分量上去，显然这样的边在$G^T$不存在，因为$G^T$上只存在从完成时间较小的连通分量到完成时间较大的边。



### 0x06 最小生成树

对于一个连通无向图$G(V,E)$，其每条的权重为$w(u,v)$.  $G$的==最小生成树==首先是$G$上的一个无环子图$T$，且$T$上边的权重之和$\sum\limits_{u,v \in T} w(u,v)$是最小的.



构造最小生成树的两种算法Kruskal算法和Prim算法都是基于贪心算法，它们共同的思想为

1. 管理一个边的集合$A$，$A$是某个最小生成树的子集;
2. 在某时刻往$A$里面增加边$(u,v)$的时候，$A\cup\{(u,v)\}$依然满足是某个最小生成树的子集;
3. 直到$A$为一个生成树为止. 

其中边$(u,v)$我们称为==安全边==.



**Definition** 无向图$G=(V,E)$的一个==切割==是对$V$的一个划分$(S,V-S)$.  若边$(u,v)$的一个端点位于$S$，另一个端点位于$V-S$，则称$(u,v)$==横跨==切割$(S,V-S)$.  

**Definition** 在所有横跨切割$(S,V-S)$的边中，权重最小的边称为==轻量级边==.  一般地，如果一条边是满足某个性质的所有边中权重最小的，则称该边是满足所给性质的一个==轻量级边==.

**Definition** 如果集合$A$中不存在横跨该切割的边，则称该切割==尊重==集合$A$. 



![image-20210911154236861](/home/maple/.config/Typora/typora-user-images/image-20210911154236861.png)



**Theorem** 若边集合$A$是某个最小生成树的子集，设切割$(S,V-S)$尊重$A$，那么横跨$(S,V-S)$权重最小的边$(u,v)$，对于集合$A$而言是安全的. 



**Corollary** 若边集合$A$是某个最小生成树的子集，设子图$G_A=(V,A)$中的一个连通分量(子树)为$C = (V_C,E_C)$. 若$(u,v)$是$G$上连接$C$和$G_A$中其他连通分量的一条轻量级边，则边$(u,v)$对于$A$是安全的. 

*proof*.  这是显然的，因为切割$(V_C,V-V_C)$是尊重$A$的. 若该切割不是尊重$A$的，那么存在一条$(u,v) \in A$，其中$u \in V_C$和$v \in V-V_C$，那么$(u,v) \in C$，这就矛盾了.   



#### Kruskal算法



是一个贪心算法，其算法的核心思想

1. 将$G$看做一个连接森林. 
2. 在初始阶段$G$的每个结点就是一棵树，设该连接森林上连接两颗树的权重最小的边为$(u,v)$， 此时$A=\empty$，因此每一结点都是$G_A$上的一个连通分量，设$(u,v)$连接$C_1$和$C_2$，那么显然$(u,v)$是$C_1$连接其他连通分量的一条轻量级边，因此$(u,v)$对$A$是安全的. 此时再将$u,v$合并称一棵树，此时$G$依然是一颗连接森林. 
3. 注意此时$G$上连接任意两棵树的边，都可以作为一个横跨尊重$A$的切割（因为$A$上边已经被收缩起来了），那么依然寻找$G$上连接两颗树的权重最小的边，因此对$A$而言这个边是安全的，然后再将这条边收缩起来. 
4. 余类推，直到$A$称为一个生成树. 

```python
minimum_spannning_tree_kruskal(G)
	A={};
    for each v in G.V
    	make_set(v)
    edges = sort_edges_into_nondecreasing_order(G) #这里可以用二叉堆来排，所需时间复杂度为 Eln E
    for each (u,v) in edges
    	if find_set(v) != find_set(u) #不遍历A已经覆盖了得结点
        	insert (u,v) in A
        	union(u,v)    
	return A
```



**基本分析性质**

- Kruskal算法所需的时间复杂度为$O(E\lg E)$或者$O(E\lg V)$. 首先总共有$|V|+|E|$个disjoint_set上的操作，其中$|V|$个make-set操作，$|E|$个union和find_set操作， 那么其所需时间为$O((|V|+|E|)\alpha(V))$. 又因为这里假设是一个连通图，那么$|E| \geq |V|-1$，因此前面的复杂度可以写成$O(|E|\alpha(V))$.  其中$\alpha(V) \leq \lg V$，因此复杂度可以写为$O(E\lg E)$ .  特别地，如果再注意到$|E| < |V|^2$，那么时间复杂度也可以表示$O(E \lg V)$.



#### Prim算法

与Kruskal算法不同的是，Prim算法维护的集合$A$总是一颗树，这颗树的生成从任意结点开始，直到覆盖$G$的所有结点. 但也是一个贪心算法. 



其算法核心:

1.  初始阶段任意选择$G$上的结点$u$，将连接$u$和其他结点权重最小的边$(u,v)$加入到$A=\empty \cup (u,v)$中.
2.  在$G$上寻找连接$A$与$A$之外的结点的所有边，找权重最小的边加入$A$中.
3.  余类推，直到$A$成为一颗最小生成树.

在构造Prim算法的过程中，如何快速的找到连接树$A$中权重最小的边是最值得关注的

```python
minimum_spanning_tree_prim(G,r){
	for each u in G.V
    	u.key = positive_infinite
    	u.parent = nil
    r.key = 0
    Q = create_min_queue(G.V)
    while Q != empty
    	u = extract_min(Q)
    	for each v in u.adj
    		if v in Q  and w(u,v) < v.key
    			v.parent = u
    			v.key = w(u,v) #注意这里是有min_queue_decrease过程隐藏在里面
}
```

这里最小队列$Q$总是维护着其他结点连接$A$的权重最小的边. 



**基本分析性质**

- Prime算法复杂度为$O(E\ln V+V\ln V)$，这里有$|E| \geq |V|-1$，因此复杂度可以写成$O(E\ln V)$. 



#### 找出全部的最小生成树

**Proposition** 最小生成树不唯一，但是权重值是唯一的



**solution** 

1. 首先使用Kruskal算法或者Prim算法计算出一个最小生成树$S$.  

2. 尝试用$e_1 \in E-S$替换$e_2 \in S$，且$w(e_1) = w(e_2)$. 



### 0x07 最短路径

不同图下的最短路径:

1. 在无向图上最短路径，就是边最少的简单路径.  

2. 在带权有向图上最短路径，就是一条所有边权重加起来最小的简单路径.  



单源最短路径主要有涵盖下面三个问题

1.  单目的地最短路径问题 给定目的地$t$，求所有其他结点$v$到它的最短路径
2.  单结点对最短路径问题 给定两个结点$u,v$，求它们的最短路径
3.  所有结点对最短路径问题 求图上所有结点对$(u,v)$的最短路径



**Theorem** 最短路径的子路径也是最短路径



**关于有向图上的负权重的边**

负权重和正权重一样做加法就行，没有区别.  只要不出现权重为负值的环路，就可以确保每个源结点可达的结点都有最短路径. 



**环路**

有两种环路:

1. 环路上的边权重加起来为负数的环路

   若给定源结点$s$可以到达该环路，那么从该环路可达的结点对$s$而言是没有最短路径的.  因为路径的权重可以无限的小，所以最短路径是不可能包含一个负值的环路的.

2. 环路上的边权重加起来为正数的环路

   一个最短路径上也是不可能包含一个环路的，因为只要从该路径上删除这个环路就可以得到一条权重更小的路径. 

3. 环路上的边权重加起来为0的环路

   一个最短路径上若有权重为0的环路，那么总可以删除它，得到一个权重与原来一样的最短路径，我们可以重复这个操作知道最短路径上没有权重为0的环路

综上可以找到最短路径中是没有环路的，即它是一条简单路径.  ==因此我们专注寻找最短的简单路径==. 



**Definition** 从源结点$s$可达的所有结点和它们的最短路径组成的是一个==最短路径树==. 



**松弛操作**

在每个结点上维护一个$v.d$表示源结点$s$到$v$的最短路径的上界，relax操作就是通过$v$的前驱结点来更新$v.d$和$v.p$，因为$s$到$v$总要通过$v$的前驱结点. 

```
init(G,s)
	for each v in G.V
		v.d = maximum
		v.p = nil
	s.d = 0	

relax(u, v, w)
	if v.d > u.d + w(u,v)
		v.d = u.d + w(u,v)
		v.p = u
```

可以看到一次relax操作可以维持让$v.d$非增. 



**Proposition** ==三角不等式==  $\delta(s,v) \leq \delta(s,u) + w(u,v)$

**Proposition** ==路径松弛性质== 如果$p = <v_0, v_1,\cdots,v_k>$是从$v_0$到$v_k$的一条最短路径，如果我们按照$(v_0,v_1),\cdots ,(v_{k-1},v_k)$的次序依次做松弛操作，则最后得到的$v_k.d$就是最短路径.  该性质与其他的松弛操作无关，即使有其他的松弛操作串插在里面.  就是说必须做完前述的松弛操作之后才能得到最短路径. 



#### Bellman-ford算法

根据前面路径松弛的性质暴力计算，要求一对结点$(v_0,v_k)$最短路径，若它们之间存在最短路径，那么只要按照这个最短路径的依次做松弛操作.  显然我们是不可能提前预知那一条是最短路径，因此考虑对$G$上所有边都做$|G.V|-1$（简单路径上最多有$|G.V|$个结点，因此最多有$|G.V|-1$条边）次relax操作，这样对每一对可达的结点而言，总有一个松弛序列对应它们的最短路径.  

它的优势是可以处理带负值的有向图.   

```python
bellman_ford(G,w,s)
	init(G,s)
	for i = 1 to |G.v|-1
		for each (u,v) in G.E
			relax(u,v,w)
	for each (u,v) in G.E #有结点的v.d可以任意小，代表有负权重的环
    	if v.d > u.d + w(u,v)
    		return false
    return true		
```



**基本分析性质**

- 时间复杂度为$O(VE)$.

  

#### DAG算法

这个算法是针对==有向无环图==下的Bellmen-ford算法改进.    其主要思想如果$u,v$之间存在一条简单路径，那么在拓扑序中$u$应位于$v$的前面，因此我们可以只需要按照拓扑序的来一次relax操作，就可以计算出源节点到所有可达结点的最短路径. 

```python
dag(G,w,s)
	linear_order = topologically_sort_graph(G)
    init(G,s)
	for each v in linear_order #遍历整张邻接矩阵表需要
    	for each u in v.adj
        	relax(v,u,w)            
```

DAG算法的时间复杂度为$O(V+E)$. 



#### Dijkstra算法

此时要求是有向图上所有的边的权重都是==非负的==.  Dijkstra算法本质是一个贪心算法，这里我们要对所有结点$v.d$维护一个最小队列，因为每条边的权重非负，才能确保我们做一次贪心操作的正确性，不会出现多条边正负抵消而最短的效果. 

主要步骤:

1. 初始化所有结点$v.d = \infty$，其源结点$s.d = 0$，这是为了第一个从$Q$里面拿$s$出来

2. 对所有结点按照$v.d$为关键字构造一个最小队列$Q$

3. 从$Q$里面去一个$v.d$最小的结点出来，丢到结点集合$S$中

4. 更新$v$的所有邻接的结点的$v.d$和$v.p$，同时维护$Q$

5. 重复34操作直到$Q$为空，得到$S$就是所有从$s$可达的结点$v$，其$v.d$表示$s$到$v$的最短路径. 

   

Dijkstra算法依赖最小队列的实现，因此需要看一下关于改进最小队列的实现. 

```python
dijkstra(G,w,s)
	init(G,s)
	S = {}
	Q = build_minimum_queue(G.V)
	while Q != empty
		u = extract_min(Q) #此时最小队列extract_min操作时间我们可以理解为O(ln v), 而insert和delete_key认为是O(1)
		add(S,u)
		for each v in u.adj #这里总共花费的时间，可以理解为整张邻接链表，这里也隐藏着一个min_queue_decrease_key
			relax(u,v,w)
```



**基本分析性质**

- 时间复杂度为$O((V+E)\lg V)$. 





### 0x08 所有结点多的最短路径问题

#### 边迭代



**Definition** 定义$G$的邻接矩阵对应的权重矩阵$W=(w_{ij})$为
$$
w_{ij} = \left\{ \begin{array}{ll}  
0 & i=j \\
\text{有向边}(i,j)\text{的权重} & i \neq j, (i,j) \in E \\
\infty & i \neq j, (i,j) \notin E

\end{array} \right.
$$


**Definition** 定义$l_{ij}^{(m)}$为结点$i$到结点$j$至多包含$m$条边的任意路的最小权重. 当$m = 0$时
$$
l_{ij}^{(0)} = \left\{ \begin{array}{ll} 
0 & i = j \\
\infty & i \neq j
\end{array} \right.
$$
当$m \geq 1$时
$$
l_{ij}^{(m)} = \min( l_{ij}^{(m-1)},\min_{1 \leq k \leq n}\{ l_{ik}^{(m-1)}+w_{kj} \}).
$$
其中$n$表示$G$有$n$个结点. 当$k = j$时，$l_{ij}^{m-1} + w_{jj} = l_{ij}^{m-1}$，那么
$$
l_{ij}^{(m)} = \min_{1 \leq k \leq n}\{ l_{ik}^{(m-1)}+w_{kj} \}.
$$


**Proposition** 给定有$n$个结点的有向图$G$，则
$$
\delta(i,j) = l_{ij}^{(n-1)} = l_{ij}^{(n)} = l_{ij}^{(n+1)} = \cdots
$$
*proof*. 最短路径是简单路径. 



**Definition** 给定两个$n$阶矩阵$A=(a_{ij}),B=(b_{ij})$，定义运算$\cdot :A \times A \to A$，设$A \cdot B = C$，那么
$$
c_{ij} = \min_{1 \leq k \leq n}\{ a_{ik}+b_{kj} \}.
$$
**Definition** 定义$L^{(1)}=W$，并递归定义
$$
L^{(m)} = L^{(m-1)} \cdot W.
$$
**Propositon**  结合律
$$
(A \cdot B ) \cdot C = A \cdot (B \cdot C) 
$$
*proof*.  
$$
[(A \cdot B) \cdot C](i,j) = \min_{1 \leq k \leq n}\{ \min_{1 \leq q \leq n}\{ a_{iq}+b_{qk} \}+ c_{kj} \} = \min_{1 \leq k \leq n}\{ \min_{1 \leq q \leq n}\{ a_{iq}+b_{qk}+c_{kj} \} \} \\
[A \cdot (B \cdot C)](i,j) = \min_{1 \leq k \leq n}\{ a_{ik} + \min_{1 \leq q \leq n}\{ b_{kq}+c_{qj} \}\} = \min_{1 \leq k \leq n}\{ \min_{1 \leq q \leq n}\{ a_{ik} + b_{kq}+c_{qj} \}\}
$$
任意重命名其中一个式子两个index就可得到另外一个. 



**Lemma**
$$
L^{(2m)} = L^{(m)}\cdot L^{(m)}.
$$
*proof*.  
$$
L^{(2m)} = \underbrace{W \cdot W \cdot \cdots \cdot W\cdot W}_{2m} = \underbrace{(W \cdot W \cdot \cdots \cdot W)}_{m}\cdot \underbrace{(W \cdot W \cdot \cdots \cdot W)}_{m} = L^{(m)} \cdot L^{(m)}.
$$
的没有

**Definition** 定义前驱结点矩阵$\prod=(\pi_{ij})$，其中$\pi_{ij}$表示结点$i$到$j$的最短路径中$j$的前驱结点.   定义结点$i$的前驱子图为$G_{\pi,i} = (V_{\pi,i},E_{\pi,i})$，其中
$$
V_{\pi,i} = \{j \in V: \pi_{ij} \neq NIL\}\cup \{i\} ~~~ E_{\pi,i} = \{(\pi_{ij},j): j \in V_{\pi,i}-{i}\}
$$


这里的定义充分利用最短路径的子路径还是最短路径. 



**朴素算法**

下述是算法是给定$L^k$求$L^{k+1}$的一个过程

```python
shortest_paths(L, W)
	n = L.dimension
    L2 = create_new_matrix(n)
    for i = 1 to n #实现前面我们定义一种特殊的矩阵乘法
    	for j = 1 to n
        	L2(i,j) = inf
            for k = 1 to n
            	L2(i,j) = min(L2(i,j), L(i,k)+W(k,j))
    return L2            
```

基本分析性质:

- 时间复杂度为$O(n^3)$.  求最短路径调用$n-1$次需要时间$O(n^4)$. 



**重复平方计算**

我们的目标是计算出$L^{(n-1)}$，那么可以考虑只计算$L^{(2)},L^{(4)},\cdots,L^{(2m)}$，最终只需要保证$2m >= n-1$就行. 

```python
faster_shortest_paths(W)
	n = W.dimension
	L = W
	m = 1
	while(m < n-1)
		L = shortest_paths(L,L);
		m = 2m
	return L	
```



基本分析性质:

- 时间复杂度为$O(n^3\lg n)$，但是其前面常数因子要小于朴素算法中的常数因子。
- 这里还有一个上界$2m \leq 2n-2$，只需要计算$\lceil \lg (n-1) \rceil$次所定义的矩阵运算.



**根据计算出来的权重图构造前驱子图**

遍历每个$L$的元素$l_{ij}$

- 若$i = j$ 或者$l_{ij} = \infty$，$\pi_{ij} = \text{NIL}$
- 若$i \neq j$ 且$l_{ij} < \infty$ ， 那么遍历$L$第$j$列或者第$i$行，由$l_{ij} = l_{ik}+ l_{kj}$确定$k$，$k$即为$i$到$j$最短路径上$j$的前驱. 

```python
genetrate_precessor(L)
	n =  L.dimension
	P = create_new_matrix(n)
	for i = 1 to n
		for j = 1 to n
    		if(L(i,j) < inf)
            	if(i == j)
                	P(i,j) = NIL
                else 
                	for q = 1 to n
                    if L(i,q) + L(q,j) == L(i,j)
                    	P(i,j) = q
            else
            	P(i,j) = NIL
                
```



在前驱子图上打印$(i,j)$的路径

```python
print_pair_shortest_path(P, i, j)
	if i == j
		print i
	elseif P[i,j] == NIL
    	print "no path"
    else print_pair_shortest_path(P, i, P[i,j]) #要打印 i->k->j的路径，那么需要先打印i->k的路径
    	print j
```



#### Floyd-Warshall算法

*motivation*

前面用于描述最短路径的方式是通过图上的边的个数来刻画，即简单路径长度最多为$n-1$，我们就可以把可能的最短路径种类按长度分类$L_0,L_1,L_2,\cdots,L_m$, 我们只需要分别求出在不同类别路径下的最短路径，再取一个最小值.  其中$L_m$的计算是可以依赖$L_{m-1}$来迭代计算的，于是有了前述的算法。 

现在我们来换个思路，我们尝试通过图上的部分结点来刻画最短路径，将图上不同结点的子集按大小分类$\empty,\{1\},\{1,2\},\{1,2,3\} \cdots \{1,2,3,\cdots,n\}$.

若考虑结点$i$和结点$j$的最短路径，我们可以分别计算以上述结点子集作为它们的可选的中间结点的最短路径，就是说比如我取结点子集$\{1,2\}$, 我们就考虑从$i$到$j$只包含中间结点$\{1,2\}$的路径$p$，把权重最小的$p$求出来，当我们取遍所有子集的时候再整体求一个最小值.  那么真对不同子集来计算最短路径的时候是否也可以迭代来做呢？ 答案是可以的.



**Definition** 定义$d_{ij}^{(k)}$从结点$i$到$j$的所有中间结点全部取自结点子集$\{1,2,\cdots,k\}$的一条最短路径的权重. 



**Proposition** $d_{ij}^{(0)} = w_{ij}$.  当$k \geq 1$时
$$
d_{ij}^{(k)}=\min(d_{ij}^{(k-1)}, d_{ik}^{(k-1)} + d_{kj}^{(k-1)})
$$
*Proof*.  当$k = 0$时，那么$i$到$j$不能包含其他结点的路径只可能是$(i,j)$，因此$d_{ij}^{(0)} = w_{ij}$.

当$k \geq 1$时， 若结点$k$是$i$到$j$某条最短路径$p$上的中间结点，那么$p$可以看做$i \leadsto k \leadsto j$，那么此时中间路径$i \leadsto k$和$k \leadsto j$的中间结点都可能含于$\{1,2,\cdots,k-1\}$，此时可以转换为两个子问题$d_{ik}^{(k-1)}$和$d_{kj}^{(k-1)}$.  
$$
d_{ij}^{(k)} = d_{ik}^{(k-1)} + d_{kj}^{(k-1)}
$$
若结点$k$不是$i$到$j$上某条最短路径$p$上的中间结点，那么此时路径$p$上的其他中间结点可能位于$\{1,2,\cdots,k-1\}$，相当于加个$k$进来没什么影响. 因此
$$
d_{ij}^{(k)} = d_{ij}^{(k-1)}
$$
最后我们再取一个$\min$操作即可.  



**Proposition** $d_{ij}^{n} = \delta(i,j)$. 



**Definition** 定义矩阵$D^{(k)} = (d_{ij}^{(k)})$， 那么$D^{(0)} = W$.



其算法伪代码为

```python
floyd_warshall(W)
	n = W.dimension
    D1 = W
    D2 = create_new_matrix(n)
    for k = 0 to n 
    	for i = 1 to n
        	for j = 1 to n
            	D2(i,j) = min(D1(i,j), D1(i,k)+D1_(k,j));
        D1 = D2       
        
        
    return D1    
```



基本分析性质

- 时间复杂度为$O(n^3)$. 



**Folyd-Warshall算法上构建前驱子图**

**Definition** 定义$\pi_{ij}^{(k)}$为从结点$i$到结点$j$的一条所有中间结点都取自结点子集$\{1,2,\cdots,k\}$的最短路径上$j$的前驱结点.  定义矩阵$\prod^{(k)}=(\pi_{ij}^{(k)})$. 



**Proposition** $\prod = \prod^{(n)}$



**Proposition** 当$k=0$时 
$$
\pi_{ij}^{(0)} = \left\{ \begin{array}{ll} 
\text{NIL} & i = j ~\text{or} ~ w_{ij} = \infty \\
i & i \neq j ~\text{and}~ w_{ij} < \infty
\end{array} \right.
$$
当$k \geq 1$时
$$
\pi_{ij}^{(k)} = \left\{ \begin{array}{ll} 
\pi_{ij}^{(k-1)} & d_{ij}^{(k-1)} \leq d_{ik}^{(k-1)} + d_{kj}^{(k-1)} \\
\pi_{kj}^{(k-1)} & d_{ij}^{(k-1)} > d_{ik}^{(k-1)} + d_{kj}^{(k-1)}
\end{array} \right.
$$
*proof*.  若$k=0$时，结果是显然的.  

若$k \geq 1$时，若$d_{ij}^{(k-1)} > d_{ik}^{(k-1)} + d_{kj}^{(k-1)}$时，考虑路径$i \leadsto k \leadsto j$，这里可以确定$k \neq j$（不然前面不等式就取等号了），因此$k \leadsto j$的最短路径(在$k$的限制下)上的$j$前驱和$i \leadsto k \leadsto j$(在$k$的限制下)是保持一致的. 其余情况类似讨论.  



算法的伪代码

```python
floyd_warshall_with_precessor(W)
	n = W.dimension
    D1 = W
    D2 = create_new_matrix(n)
    
    P1 = create_new_matrix(n)
    P2 = create_new_matrix(n)
    
    for i = 1 to n
    	for j = 1 to n
        	if(W(i,j) < inf)
                P(i,j) = i       
    
    for k = 1 to n 
    	for i = 1 to n
        	for j = 1 to n
            	if(D1(i,j) > D1(i,k)+D1_(k,j)){
                    P2(i,j) = P1(k,j)
                    D2(i,j) = D1(i,k)+D1_(k,j)
                else
                    P2(i,j) = P1(i,j)
                    D2(i,j) = D1(i,j)
        D1 = D2
        P1 = P2
    
    return D1,P1
```



#### Floyd-Warshall算法应用于构造传递闭包



**Definition** 定义有向图$G$的传递闭包为图$G^* = (V, E^*)$，其中$E^* = \{(i,j): \text{如果$G$包含一条$i$到$j$的路径}\}$。



**Solution** 计算一个有向图$G$的传递闭包，可以将$G$的每条边权重赋予$1$，再用Floyd-Warshall算法，最终如果$d_{ij} < n$那么$i$到$j$之间就存在一条路径.  时间复杂度依然为$O(n^3)$. 



**Solution** 进一步优化上述算法，可以使用逻辑运算符号$\vee$和$\wedge$来替换Floyd-Warshall算法中的$\min$和$+$，还需要调整一下初始的权重矩阵，使得那么$\infty$权重变成$0$. 这样迭代矩阵中的元素用一个bit就足够了. 



#### Johnson算法



**Definition** 给定图$G$和其权重函数$w$，若给定另外一个权重函数$\widehat{w}$满足

- 给定任意两个结点$u,v \in V$，给定$u,v$之间的一条路径$p$，那么$p$在使用权重函数$w$是$u$到$v$的一条最短路径，当且仅当$p$在使用权重函数$\widehat{w}$是$u$到$v$的一条最短路径. 
- 对于任意的边$(u,v) \in E$，有$\widehat{w}(u,v) >= 0$.

则称$\widehat{w}$是$w$是在最短路径的条件下的一个非负权重等价. 若仅满足第一个条件，则称它们是等价的.  



**Lemma** 给定图$G$和其权重函数$w$，引入一个辅助函数$h: V \to \mathbb{R}$，定义权重函数$\widehat{w}$
$$
\widehat{w}(u,v) = w(u,v) + h(u)-h(v).
$$
那么$w$和$\widehat{w}$是在最短路径的条件下是等价.  而且$G$在使用$w$时不包含负值的环路当且仅当在$\widehat{w}$不包含负值环路. 



*proof.* 设$p =(v_0,v_1,\cdots,v_k)$是结点$v_0$到$v_k$的一条最短路径，那么这条路径在使用$\widehat{w}$时的总权重为
$$
\sum\limits_{i=1}^{k} \widehat{w}(v_{i-1},v_i) =\sum\limits_{i=1}^{k} w(v_{i-1},v_i) + h(v_{i-1})-h(v_i) = \sum\limits_{i=1}^{k} [w(v_{i-1},v_i) ] + h(v_0) -h(v_k).
$$
因此这个总权重只与$p$在$w$下计算的总权重和两个端点的$h(v_0),h(v_k)$有关，那么任意两条$v_0$到$v_1$之间路径的权重在$w$下的大小关系在$\widehat(w)$下依然满足(比如比较差值，后面两个函数值一减就消去了).

假设$p =(v_0,v_1,\cdots,v_k)$是一条环路，即$v_0 = v_k$，那么根据上式则有
$$
\sum\limits_{i=1}^{k} \widehat{w}(v_{i-1},v_i) = \sum\limits_{i=1}^{k} [w(v_{i-1},v_i) ]
$$
因此一个存在负环当且仅当另一个也存在负环.  



**Solution** ==如何构造一个$h(x)$使得$\widehat{w}$非负==.  在$G$上添加一个新的结点$s$，并且对所有的$v \in G$引入边$(s,v)$，同时设置$w(s,v) = 0$.   令
$$
h(v) = \delta(s,v)
$$
于是在三角不等式性质下有
$$
h(v) \leq h(u) + w(u,v) \Rightarrow  w(u,v) + h(u)-h(v) \geq 0
$$
非常巧妙！对于计算$h(v)$，我们以$s$为源结点使用一下Bellman-Ford算法即可. 

 

**Solution** 在计算出$\widehat{w}$ 之后，我们只需要对每个结点做一次dijkstra算法即可.  算法运行实际包括一次Bellman-Ford运行时间$O(VE)$，$|V|$次dijkstra运行时间为$O(V(V+E)\lg V)$，那么总的时间为$O(V(V+E)\lg V)$. 



### 0x99 考点

**Problem** 给定图$G$的邻接矩阵表示，设计一个$O(V)$的算法用于判定$G$是否存在通用汇点，即入度为$|V|-1$且出度$0$的结点.  ==这是图上少有的运行时间为$O(V)$的算法== 

```python
is_contain_universal_sink(G)
	i=j=1
	while i < |V| and j < |V|
		if G[i,j] = 1
			i = i+1
		else if G[i,j] = 0
        	j = j+1
    check if node i is universal sink
```

这个算法比较巧妙，它的运行时间最多为$2|V|$，即$O(V)$.  我们来证明一下这个算法的正确性:  如果$G$存在一个通用汇点$i$，它对应的第$i$行全部是$0$，第$i$列全部是$1$，那么

- 如果上述算法走到了第$i$行，那么它会一直增加$j$直到退出循环.  
- 如何确保该算法一定会走到第$i$行呢?  我们可以换个方式思考，如果它没能走到第$i$行就退出循环了，会出现什么情况？
  - 走到某一行$j$全为0
    - 结点$j$不是通用汇点，这种情况下我们的目的就已经实现了
    - 结点$j$不是通用汇点，它的出度为0，说明$G$一定也没有通用汇点，我们的目的也实现了.
  - 走到某一行$j$不全为$0$，说明前$j$个结点没有公共目标结点，显然$G$也不会有通用汇点。

------

**Problem** 
