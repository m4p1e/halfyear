#include<stdlib.h>
#include<string.h>
#include "simple.h"

/*times 33 */
uint32_t get_hash_value_by_string(char *str, int length){
    uint32_t hash = UINT32_C(5381);
    for (int i = 0; i < length; i++)
    {
        hash = ((hash << 5) + hash) + str[i]; // ((hash << 32) + hash) == hash * 33
    }

    //防止hash为0，高位置1
    return hash | UINT32_C(0x80000000);
}

hashtable *create_hashtable(){
    hashtable* ht = malloc(sizeof(hashtable));
    memset(ht, '\0', sizeof(hashtable));
    return ht;
}

void *hashtable_find(hashtable *ht, char *key, int len, int *is_exist){
    uint32_t hash,idx;
    bucket *bt; 
    hash = get_hash_value_by_string(key, len);
    idx = hash % 41;

    bt = ht->table[idx];

    while(bt){
        if(len == bt->len && !memcmp(key, bt->key, len)){
            if(is_exist){
                *is_exist = 1;
            }
            return bt->val;
        }
        bt = bt->next;
    }

    if(is_exist){
        *is_exist = 0;
    }

    return NULL;
}

void *hashtable_find_bucket(hashtable *ht, char *key, int len, uint32_t str_hash){
    uint32_t hash,idx;
    bucket *bt; 

    if(str_hash){
        hash = str_hash;
    }else{
        hash = get_hash_value_by_string(key, len);
    }

    idx = hash % 41;

    bt = ht->table[idx];

    while(bt){
        if(len == bt->len && !memcmp(key, bt->key, len)){
            return bt;
        }
        bt = bt->next;
    }

    return NULL;
}

void hashtable_insert(hashtable *ht, char *key, int len, void *val){
    uint32_t hash,idx;
    bucket *bt;

    hash = get_hash_value_by_string(key, len);
    idx = hash % 41;


    bt = hashtable_find_bucket(ht, key, len, hash);
    
    if(bt){
        bt->val = val;
    }else{
        bt = malloc(sizeof(bucket));
        bt->val = val;
        bt->key = malloc(sizeof(len));
        memcpy(bt->key, key, len);
        bt->len = len;
        bt->next = ht->table[idx];
        ht->table[idx] = bt;
    }
}

void hashtable_delete(hashtable *ht, char *key, int len){
    uint32_t hash,idx;
    bucket *bt, *prev;

    hash = get_hash_value_by_string(key, len);
    idx = hash % 41;

    bt = ht->table[idx];
    while(bt){
        if(len == bt->len && !memcmp(key, bt->key, len)){
            prev->next = bt->next;
            free(bt->key);
            free(bt);
            break;
        }
        prev = bt;
        bt = bt->next;
    }
}





