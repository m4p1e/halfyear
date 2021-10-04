#include "helper.h"

int str2int(char *str, int len){
    int i, sum, neg;
    sum = 0;
    
    if(*str == '-'){
        neg = 1;
    }else{
        neg = 0;
    }

    for(i = neg; i < len; i++){
        if('9' < str[i] || str[i] < '0'){
            break;
        }
        sum = sum * 10 + str[i]-'0'; 
    }

    if(neg){
        return -sum;
    }

    return sum;
}