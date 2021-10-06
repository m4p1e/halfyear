#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _kmp{
    int *shift;
    char *pattern;
    int pattern_len;
}kmp;



void compute_prefix_shift_function(kmp *kmp_s, char *patten, int pattern_len){
    int i, maximum_suffix_index;
    kmp_s->shift = malloc(pattern_len * sizeof(int));
    kmp_s->shift[0] = -1;
    kmp_s->pattern_len = pattern_len;
    kmp_s->pattern = malloc(sizeof(pattern_len));
    memcpy(kmp_s->pattern, patten, pattern_len);    

    maximum_suffix_index = -1;
    for (i = 1; i < pattern_len; i++)
    {   
        while (maximum_suffix_index > -1 && patten[maximum_suffix_index+1] != patten[i])
        {
            maximum_suffix_index = kmp_s->shift[maximum_suffix_index];
        }
        
    
        if(patten[maximum_suffix_index+1] == patten[i]){
            maximum_suffix_index++;
        }

        kmp_s->shift[i] = maximum_suffix_index; 
    }
    
}

kmp *create_kmp(char *pattern, int pattern_len){
    kmp *kmp_s;
    kmp_s = malloc(sizeof(kmp));
    compute_prefix_shift_function(kmp_s, pattern, pattern_len);
    return kmp_s;
}

void kmp_run(kmp* kmp_s, char *str, int str_len){
    int i, matched_index;

    //at the beginning, we dont match any character, so matched_index = -1;
    matched_index = -1;
    for (i = 0; i < str_len; i++)
    {

        while(matched_index > -1 && kmp_s->pattern[matched_index + 1] != str[i]){ //if next character match failed, ajdust next index character should be matched
            matched_index = kmp_s->shift[matched_index];
        }

        if(kmp_s->pattern[matched_index + 1] == str[i]){ // for case:  matched_index == -1 
            matched_index++;
        }

        if(matched_index == kmp_s->pattern_len-1){
            printf("find pattern at offset %d\n", i-kmp_s->pattern_len+1);
            matched_index = kmp_s->shift[matched_index]; //ajdust next index character should be matched
        }

        // if(str[i] == kmp_s->pattern[matched_index+1]){
        //     matched_index++;
        //     if(matched_index == kmp_s->pattern_len-1){
        //         printf("find pattern at offset %d\n", i-kmp_s->pattern_len+1);
        //         i = i + matched_index-kmp_s->shift[matched_index];
        //         matched_index = kmp_s->shift[matched_index];
        //     }else{
        //         i++;
        //     }
        // }else{
        //     if(matched_index == -1){
        //         i++;
        //     }else{
        //         i = i + matched_index-kmp_s->shift[matched_index];
        //         matched_index = kmp_s->shift[matched_index];
        //     }
        // }
    }
}

int main(){
    kmp *kmp_s;
    char str[] = "aaaasssssa";
    char pattern[] = "ssss";
    kmp_s = create_kmp(pattern, sizeof(pattern)-1);
    kmp_run(kmp_s, str, sizeof(str)-1);
    return 0;
}