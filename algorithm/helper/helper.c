#include "helper.h"

int str2int(char *str, int len){
    int i, sum;
    sum = 0;
    for(i = 0; i < len; i++){
        if('9' < str[i] || str[i] < '0'){
            break;
        }
        sum = sum * 10 + str[i]-'0'; 
    }

    return sum;
}