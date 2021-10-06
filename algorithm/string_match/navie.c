#include<stdio.h>
#include<stdlib.h>

int strcmp(char *src, int src_len, char *dst, int dst_len){
    int i;
    
    if(src_len != dst_len){
        return 0;
    }

    for (i = 0; i < src_len; i++)
    {
        if(*(src+i) != *(dst+i)){
            return 0;
        }
    }

    return 1;   
}

void navie_string_match(char *str, int str_len, char *pattern, int pattern_len){
    int i, times;
    times = str_len-pattern_len;

    for (i = 0; i <= times; i++)
    {
        if(strcmp(str+i, pattern_len, pattern, pattern_len)){
            printf("find pattern at offset %d\n", i);
        }
    }
}

int main(){
    char str[] = "aaaasssssa";
    char pattern[] = "ssss";
    navie_string_match(str, sizeof(str)-1, pattern, sizeof(pattern)-1);
    return 0;
}