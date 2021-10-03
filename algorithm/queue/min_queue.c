#include "min_queue.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

min_queue *create_min_queue(int (*compare)(void *, void *), int (*decrease)(void *, int)){
    min_queue *q;

    q = malloc(sizeof(min_queue));
    q->compare = compare;
    q->decrease = decrease;
    q->length = 0;
    q->reverse_binding = NULL;

    return q;
}

void min_queue_set_reverse_binding(min_queue *q, void (*reverse_binding)(Bnode *node, void *attr)){
    q->reverse_binding = reverse_binding;
}

void min_heapify(min_queue *q, Bnode *node){
    Bnode *lowest;
    void *attr;

    while(node->left != NULL || node->right != NULL){
        lowest = node;
        if(node->left != NULL){
            if(q->compare(lowest->attr, node->left->attr) > 0 ){
                lowest = node->left;
            }
        }

        if(node->right != NULL){
            if(q->compare(lowest->attr, node->right->attr) > 0 ){
                lowest = node->right;
            }
        }

        if(lowest == node){
            break;
        }

        //exchange the attr of two suitable node
        attr = node->attr;
        node->attr = lowest->attr;
        lowest->attr = attr;

        if(q->reverse_binding){
            q->reverse_binding(lowest, lowest->attr);
            q->reverse_binding(node, node->attr);
        }

        node = lowest;
    }    
}

Bnode *min_queue_next_leaf_node(min_queue *q){
    int depth, num;
    Bnode *leaf_node, *node;

    leaf_node = malloc(sizeof(Bnode));
    leaf_node->left = NULL;
    leaf_node->right = NULL;

    
    if(q->length == 0){
        leaf_node->parent = NULL;
        q->top = leaf_node;
        q->length++;
        return leaf_node;
    }

    q->length++;
    depth = log2(q->length);
    num = q->length - exp2(depth) + 1;
    node = q->top;

    while(depth > 0){ //这里的意图就是不断的判定在左子树还是右子树. 
        if(exp2(depth)/2 >=  num){
            if(node->left == NULL){
                node->left = leaf_node;
                leaf_node->parent = node;
            }else{
                node = node->left;
            }
        }else{
            if(node->right == NULL){
                node->right = leaf_node;
                leaf_node->parent = node;
            }else{
                node = node->right;
            }

            num = num-exp2(depth)/2;
        }
        depth--;
    }

    return leaf_node;
}

void *min_queue_top(min_queue *q){
    if(q->top != NULL){
        return q->top->attr;
    }

    return NULL;
}


void min_queue_search(){
    //配合下面的decrease_key, 你先得找到这个node才行. 
}

Bnode *min_queue_decrease_key(min_queue *q, Bnode *node, int key){
    void *attr;
    q->decrease(node->attr, key);

    while(node->parent != NULL){
        if(q->compare(node->parent->attr, node->attr) > 0){
            attr = node->attr;
            node->attr = node->parent->attr;
            node->parent->attr = attr;
            if(q->reverse_binding){
                q->reverse_binding(node, node->attr);
                q->reverse_binding(node->parent, node->parent->attr);
            }
        }else{
            break;
        }

        node = node->parent;
    }

    return node;
}

Bnode *min_queue_insert(min_queue *q, void *attr){
    Bnode *leaf_node;

    leaf_node = min_queue_next_leaf_node(q);
    leaf_node->attr = attr;

    while(leaf_node->parent != NULL){
        if(q->compare(leaf_node->parent->attr, leaf_node->attr) > 0){
            attr = leaf_node->attr;
            leaf_node->attr = leaf_node->parent->attr;
            leaf_node->parent->attr = attr;
            if(q->reverse_binding){
                q->reverse_binding(leaf_node, leaf_node->attr);
                q->reverse_binding(leaf_node->parent, leaf_node->parent->attr);
            }
        }else{
            break;
        }

        leaf_node = leaf_node->parent;
    }

    return leaf_node;        
}


Bnode *min_queue_last_leaf_node(min_queue *q){
    int depth, num;
    Bnode *leaf_node;
    
    if(q->length == 1){
       return q->top;
    }

    depth = log2(q->length);
    num = q->length - exp2(depth) + 1;
    leaf_node = q->top;

    while(depth > 0){
        if(exp2(depth)/2 >=  num){
            leaf_node = leaf_node->left;
        }else{
            leaf_node = leaf_node->right;
            num = num - exp2(depth)/2;
        }
        depth--;
    }

    return leaf_node;
}

void *min_queue_extract_min(min_queue *q){
    Bnode *leaf_node;
    void *attr;

    if(q->length == 0){
        return NULL;
    }

    leaf_node = min_queue_last_leaf_node(q);
    q->length--;    
    attr = q->top->attr;
    if(q->reverse_binding){
        q->reverse_binding(NULL, attr);
    }
    if(leaf_node == q->top){
        q->top = NULL;
    }else{
        q->top->attr = leaf_node->attr;
        if(q->reverse_binding){
            q->reverse_binding(q->top, q->top->attr);
        }
        if(leaf_node == leaf_node->parent->left){
            leaf_node->parent->left = NULL;
        }else{
            leaf_node->parent->right = NULL;
        }

        min_heapify(q,q->top);
    }

    free(leaf_node);
    return attr;
}

int min_queue_size(min_queue *q){
    return q->length;
}

void node_visiter(Bnode *node, void (*node_printer)(void *attr), int space){
    if(!node){
        return;
    }

    space += 10;
    node_visiter(node->right, node_printer, space);

    printf("\n");
    for(int i = 10; i < space; i++){
        printf(" ");
    }

    node_printer(node->attr);
    printf("\n");

    node_visiter(node->left, node_printer, space);
}

void min_queue_printer(min_queue *q, void (*node_printer)(void *attr)){
    Bnode *node = q->top;
    node_visiter(node, node_printer, 0);
}