#include "stdio.h"
#include "stdlib.h"
#include "queue/min_queue.h"

typedef struct _simple_attr
{
    int num;
}simple_attr;

int compare_integer(Bnode *a,  Bnode *b){
    return (((simple_attr *)(a->attr))->num - ((simple_attr *)(b->attr))->num);
}

int decrease_integer(Bnode *a, int key){
    ((simple_attr *)(a->attr))->num = key;
}

Btree *build_Huffman_tree(int *A, int length){
    min_queue *Q;
    Btree *tree;
    Bnode *node, *min1, *min2;
    Q = create_min_queue(compare_integer, decrease_integer);
    for(int i = 0; i < length; i++){
        node = create_binary_tree_node();
        node->attr = malloc(sizeof(simple_attr));
        ((simple_attr *)(node->attr))->num = A[i];
        min_queue_insert(Q,node);
    }

    while(min_queue_size(Q) != 1){
        min1 = min_queue_extract_min(Q);
        min2 = min_queue_extract_min(Q);
        node = create_binary_tree_node(Q);
        node->attr = malloc(sizeof(simple_attr));
        node->left = min1;
        node->right = min2;
        min1->parent = node;
        min2->parent = node;
        ((simple_attr *)(node->attr))->num =  ((simple_attr *)(min1->attr))->num +  ((simple_attr *)(min2->attr))->num;
        min_queue_insert(Q,node);
    }

    tree = create_binary_tree(NULL);
    tree->root = min_queue_extract_min(Q);
    return tree;
}

/* 计算整棵树的权重 */
int view_Huffman_tree(Bnode *node, int depth){
    int left, right;
    if(node->left){
        left = view_Huffman_tree(node->left, depth+1);
    }else{
        return ((simple_attr *)(node->attr))->num * depth;
    }

    if(node->right){
        right = view_Huffman_tree(node->right, depth+1);
    }
    
    return left + right;
}


int main(){
    setbuf(stdout,NULL);
    int A[]={2,4,5,7};
    int weight;
    Btree *Huffman_tree;
    Huffman_tree = build_Huffman_tree(A,sizeof(A)/sizeof(int));
    weight = view_Huffman_tree(Huffman_tree->root,0);
    printf("weight is : %d\n", weight);
    return 0;
}