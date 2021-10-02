#include "binary_tree/basic_type.h"

/*
    关于这里变长数据结构的存储，有两种选择
    1. 变长数组
    2. 二叉树
    它们都是可以来表示二叉堆的，这里使用树来操作
*/
typedef struct _min_queue
{   
    Bnode *top;
    int (*compare)(void *, void *);
    int (*decrease)(void *, int);
    int length;
}min_queue;

min_queue *create_min_queue(int (*compare)(void *, void *), int (*decrease)(void *, int));
void *min_queue_top(min_queue *q);
void min_queue_insert(min_queue *q, void *attr);
void *min_queue_extract_min(min_queue *q);
int min_queue_size(min_queue *q);