#include "basic_type.h"
#include<stdlib.h>
#include<stdio.h>

void binary_tree_in_order_tree_walk(Btree *T, void (*visit)(void *))
{   
    Bnode *x;
    int done;
    x = T->root;
    done = 0;

    while(!done)
    {   
    
        while(x->left != NULL || x->right != NULL){
            while(x->left != NULL)
                x = x->left;
            visit(x->attr);
            if(x->right != NULL)
                x = x->right;
        }
        
        if(x == T->root || x == x->parent->right){
            visit(x->attr);
        }

        while(x != T->root && (x->parent->right == x || x->parent->right == NULL )){
            if(x->parent->right == NULL)
                visit(x->parent->attr);
            x = x->parent;    
        }

        if(x != T->root){
            visit(x->parent->attr);
            x = x->parent->right;
        }else{
            done = 1;
        }
    }
}