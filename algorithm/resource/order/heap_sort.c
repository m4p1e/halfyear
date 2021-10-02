#include<stdio.h>
#include<stdlib.h>


//这里下标计算看起来有点怪异，实则我们考虑第一个元素的下标是0，而不是1，整体元素向前移动了一格
#define PARENT(_i)  ((_i+1)/2-1)   
#define LEFT_CHILD(_i) (2*_i + 1)
#define RIGHT_CHILD(_i) (2*_i + 2)

void printer(int *A, int length){
    for (int i = 0; i < length; i++)
    {
        printf("%d,",A[i]);
    }
    printf("\n");
}

void max_heapify(int *H, int length , int i){
    int highest, next;
    while(i < length/2){ // length/2到length-1都是叶子结点.
        next = i;
        highest = H[i];

        if(highest < H[LEFT_CHILD(i)]){ //比较左右孩子
            next = LEFT_CHILD(i);
            highest = H[LEFT_CHILD(i)];       
        }

        if(RIGHT_CHILD(i) < length && highest < H[RIGHT_CHILD(i)]){
            next = RIGHT_CHILD(i);  
            highest = H[RIGHT_CHILD(i)];
        }

        if(next == i){ //左右孩子均比当前结点小，那么这个地方堆的性质是保持的，直接退出
            break;
        }

        //exchange
        H[next] = H[i];
        H[i] = highest;
        i = next;
    }
}

void build_max_heap(int *H, int length){
    int i;

    //从非叶子开始从下自上建堆
    i = length/2-1;
    while (i >= 0)
    {   
        max_heapify(H, length, i);
        i--;
    }
    
}

void max_heap_sort(int *H, int length){
    int highest, i;
    i = 0;

    while (i < length)
    {   
        i++;
        highest = H[0]; //最大堆里面最大的
        H[0] = H[length-i]; //拿最后一格叶子结点出来替换
        H[length-i] = highest;
        max_heapify(H, length-i,0); //fixup
    }
    
}

void max_heap_insert(){
    //这里需要动态数据结构的支持，在这里是无法完善的. 
}

int max_heap_maximum(int *H, int length){
    if(length > 0){
        return H[0];
    }else{
        printf("taking element from empty heap!\n");
        exit(1);
    }
}

void max_heap_extract(){

}


int main(){
    setbuf(stdout,NULL);
    int A[]={2,5,3,1,6,4,5,9,1,3,5};
    //int A[]={2,5,3,1,6,4};
    build_max_heap(A, sizeof(A)/sizeof(int));
    printer(A, sizeof(A)/sizeof(int));
    max_heap_sort(A, sizeof(A)/sizeof(int));
    printer(A, sizeof(A)/sizeof(int));
    return 0;
}