#include<stdio.h>

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

void printer(int *A, int length){
    for (int i = 0; i < length; i++)
    {
        printf("%d",A[i]);
    }
    printf("\n");
}

int main(){
    int A[]={2,5,3,1,6,4};
    printer(A,sizeof(A)/sizeof(int));
    insert_order3(A,sizeof(A)/sizeof(int));
    printer(A,sizeof(A)/sizeof(int));
}