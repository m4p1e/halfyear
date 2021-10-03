#include<stdio.h>
#include<stdlib.h>

void printer(int *A, int length){
    for (int i = 0; i < length; i++)
    {
        printf("%d,",A[i]);
    }
    printf("\n");
}

void printer_2(int *A, int start, int end){
    printf("subsequence: ");
    for (int i = start; i <= end; i++)
    {   
        printf("%d,",A[i]);
    }
    printf("\n");
}

int partition(int *A, int start, int end){
    int r, offset, t, i;
    r = A[end];
    offset = end-1;
    i = start;
    printer_2(A,start,end);
    while (i < offset){    
        if(A[i] > r){ //设计原址性
            while (A[offset] > r && offset > i) //这一步找比r小的放到前面， 这里也意味着与r相同的数字，也放在下组
            {                                
                offset--;               
            }

            if(A[offset] <= r){ //只有比r小的时候才和A[i]做exchange.             
                t  = A[offset];
                A[offset] = A[i];
                A[i] = t;
                i++;
                offset--;
            }     
        }else{ //比r小直接加即可
            i++;
        }
    }

    if(A[offset] > r){ //offset是最后一个没有做判定的
        A[end] = A[offset];
        A[offset] = r;
        printer_2(A,start,end);
        return offset;
    }else{ //这是一个循环不变量保证的，offset后面都是比r大的.
        A[end] = A[offset+1];
        A[offset+1] = r;
        printer_2(A,start,end);
        return offset+1;
    }
}

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
    printer(A, sizeof(A)/sizeof(int));
    return 0;
}