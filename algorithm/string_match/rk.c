#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

/*times 33 */
uint32_t get_hash_value_by_string(char *str, int length){
    uint32_t hash = 0;
    for (int i = 0; i < length; i++)
    {
        hash = (hash << 7) + str[i]; // ((hash << 32) + hash) == hash * 33
    }

    return hash;
}

/*快速幂*/
uint32_t binpow(uint32_t a, uint32_t b){
  uint32_t res = 1;
  
  while (b > 0) {
    if (b & 1) res = res * a;
    a = a * a;
    b >>= 1;
  }

  return res;
}

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

void rk_string_match(char *str, int str_len, char *pattern, int pattern_len){
    uint32_t pattern_hash, substr_hash, multiplier;
    int times, i;

    if(str_len < pattern_len){
        return;
    }

    multiplier = binpow(UINT32_C(128), (uint32_t)pattern_len-1);

    times = str_len-pattern_len + 1;
    pattern_hash = get_hash_value_by_string(pattern, pattern_len);
    substr_hash = get_hash_value_by_string(str, pattern_len);

    i = 0;
    do{
        if(pattern_hash == substr_hash){
            if(strcmp(str + i, pattern_len, pattern, pattern_len)){
                printf("find pattern at offset %d\n", i);
            }
        }

        i++;

        if(i > times){
            break;
        }

        substr_hash = ((substr_hash - (str[i-1] * multiplier)) << 7) + str[i+pattern_len-1];
    }while(1);
    
}

int main(){
    char str[] = "assss";
    char pattern[] = "ssss";
    rk_string_match(str, sizeof(str)-1, pattern, sizeof(pattern)-1);
    return 0;
}