#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "hashtable/simple.h"

#define MIN(_a, _b) ((_a) > (_b) ? (_b) : (_a));
//#define SUFFIX_OFFSETOF(_str, _len, _k) (_str + _len - _k)
#define TARGET_STATE(_trans_func, _c, _is_exist) hashtable_find(_trans_func->target_states, _c, 1, _is_exist);
#define SET_TARGET_STATE(_trans_func, _c, _i) hashtable_insert(_trans_func->target_states, _c, 1, _i);

typedef struct _transfer_function{
    hashtable* target_states;
}transfer_function;

typedef struct _string_match_auto_machine
{
    int states_num;
    transfer_function **trans_func;
    int terminal_state;
    int initial_state;
}string_match_auto_machine;

int match_suffix(char *str, int str_len, char *suffix, int suffix_len){
    int i,j;

    if(str_len < suffix_len){
        return 0;
    }

    j = str_len-1;
    for (i = suffix_len-1; i >= 0; i--)
    {
        if(*(str+j) != *(suffix+i)){
            return 0;
        }
        j--;
    }
    
    return 1;
}


void generate_transfer_functions(string_match_auto_machine *smam, char *pattern, int pattern_len){
    int i,j,k, is_exist;
    char *temp_str;

    smam->trans_func = malloc(smam->states_num * sizeof(transfer_function));

    for (i = 0; i < smam->states_num; i++)
    {
        smam->trans_func[i] = malloc(sizeof(transfer_function));
        smam->trans_func[i]->target_states = create_hashtable();
    }
    
    temp_str = malloc(pattern_len);    

    for (i = 0; i <= pattern_len; i++)
    {
        for(j = 0; j < pattern_len; j++){
            
            /*maximum_suffix*/
            k = MIN(pattern_len, i+1);
            
            /*construct P_i~c*/
            memcpy(temp_str, pattern, i);
            temp_str[i] = pattern[j];

            while(!match_suffix(temp_str, i+1, pattern, k)){
                k--;
            }

            TARGET_STATE(smam->trans_func[i], pattern+j, &is_exist)
            if(!is_exist){
                printf("state: %d, accpet %c to state %d\n", i, pattern[j], k);
                SET_TARGET_STATE(smam->trans_func[i], pattern+j, (void *)(intptr_t)k);
            }
        }
    }
}

string_match_auto_machine *generate_string_match_auto_machine(char *pattern, int pattern_len){
    string_match_auto_machine *smam;
    smam = malloc(sizeof(string_match_auto_machine));
    smam->states_num = pattern_len+1;
    smam->terminal_state = pattern_len;
    smam->initial_state = 0;
    generate_transfer_functions(smam, pattern, pattern_len);

    return smam;
}

void run_string_match_auto_machine(string_match_auto_machine *smam, char *str, int str_len){
    int i, is_exist;
    int state, next_state;
    state = smam->initial_state;
    for (i = 0; i < str_len; i++)
    {
        next_state = TARGET_STATE(smam->trans_func[state], str+i, &is_exist);
        if(is_exist){
            state= next_state;
        }else{
            state = 0;
        }

        if(state == smam->terminal_state){
            printf("find pattern at offset %d\n", i-state+1);
        }
    }
    
}

int main(){
    char str[] = "aaaasssssa";
    char pattern[] = "ssss";
    string_match_auto_machine *smam;
    smam = generate_string_match_auto_machine(pattern, sizeof(pattern)-1);
    run_string_match_auto_machine(smam, str, sizeof(str)-1);
    return 0;
}




