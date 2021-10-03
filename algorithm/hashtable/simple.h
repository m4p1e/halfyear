/* 定长哈希表 */
#include "linklist/singlylinklist.h"
#include<stdint.h>

typedef struct _bucket{
    int len;
    char *key;
    void *val;
    struct _bucket *next;
}bucket;


typedef struct _hashtable
{
    bucket *table[41];
}hashtable;

hashtable *create_hashtable();
void *hashtable_find(hashtable *ht, char *key, int len);
void hashtable_insert(hashtable *ht, char *key, int len, void *val);
void hashtable_delete(hashtable *ht, char *key, int len);

