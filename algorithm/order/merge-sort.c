#include<stdio.h>
#include<stdlib.h>

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

void printer(int *A, int length){
    for (int i = 0; i < length; i++)
    {
        printf("%d,",A[i]);
    }
    printf("\n");
}

int main(){
    int A[]={2,5,3,1,6,4};
    printer(A, sizeof(A)/sizeof(int));
    merge_order(A, 0, sizeof(A)/sizeof(int)-1);
    printer(A, sizeof(A)/sizeof(int));
}