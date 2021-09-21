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
    int r,i,j,t;

    r = A[end];
    i = start-1; // i是用来记录那些比r小的元素中下标最大的，
    j = start; // j就是一个普通的index, 用于变量A[start-end].


    //这里会维持几个循环不变量: 
    //1. A[start,i] 都是小于等于r的
    //2. A[i+1，j-1] 都是大于r的
    while(j < end){
        if(A[j] <= r){ // A[i+1]和A[j]安全的替换
            i = i + 1;
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
    if(end > start){
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



