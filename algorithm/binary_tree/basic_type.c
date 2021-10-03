#include "basic_type.h"
#include<stdio.h>
#include<stdlib.h>


Btree* create_binary_tree(int (*compare)(void*, void*)){
    Btree* tree = malloc(sizeof(Btree));
    tree->compare = compare;
    tree->root = NULL;
    return tree;
}

Bnode* create_binary_tree_node(){
    Bnode* node = malloc(sizeof(Bnode));
    node->right = NULL;
    node->left = NULL;
    node->parent = NULL;
    node->attr = NULL;
    return node;
}


Bnode* binary_tree_search(Btree* T, void* attr){
    Bnode *node = T->root;
    int order;
    while(node != NULL){
        order = T->compare(node, attr);
        if(order == 0){
            return node;
        }else if(order > 0){
            node = node->left;
        }else{
            node = node->right;
        }
    }

    return NULL;   
}

Bnode* binary_tree_insert(Btree* T, void* attr){
    Bnode *node = T->root;
    int order;
    Bnode *new_node = create_binary_tree_node();

    while(node != NULL){
        order = T->compare(node, attr);
        if(order >= 0){
            if(node->left == NULL){
                node->left == new_node;   
                break; 
            }else{
                node = node->left;    
            }
        }else{
            if(node->right == NULL){
                node->right == new_node;   
                break; 
            }else{
                node = node->right;    
            }
        }
    }

    new_node->parent = node;
    if(T->root == NULL){
        T->root = new_node;
    }

    return new_node;
}

Bnode* binary_tree_maximum(Bnode *node){
    while(node->right != NULL){
        node = node->right;
    }
    return node;
}

Bnode* binary_tree_minimum(Bnode *node){
    while(node->left != NULL){
        node = node->left;
    }
    return node;
}

Bnode* binary_tree_successor(Bnode *node){
    if(node->right != NULL){
        return binary_tree_minimum(node->right);
    }

    while(node->parent != NULL){
        if(node == node->parent->left){
            return node->parent;
        }
        node = node->parent;
    }

    return NULL;
}

Bnode* binary_tree_replace_no_free(Btree* T,Bnode* old, Bnode* new){
    if(old->parent != NULL){
        if(new != NULL){
            new->parent = old->parent;
        } 
        if(old->parent->left == old){
            old->parent->left = new;
        }else{
            old->parent->right = new;
        }
    }else{
        T->root = new;
    }  
}

void binary_tree_delete(Btree *T, Bnode *node){
    Bnode *replace_node = NULL;
    if(node->right != NULL ){
        replace_node = binary_tree_minimum(node->right);
        if(replace_node != node->right){
            /* 这里没有用简单的replace操作，是因为可以少一个判断*/
            if(replace_node->right != NULL){ 
                replace_node->parent->left = replace_node->right;
                replace_node->right->parent = replace_node->parent;
            }
            replace_node->right = node->right;
            node->right->parent = replace_node;
        }

        replace_node->left = node->left;
        if(node->left != NULL){
            node->left->parent = replace_node;
        }

    }else if(node->left != NULL){
        replace_node = node->left;
    }else{
        replace_node = NULL;
    }

    binary_tree_replace_no_free(T,node,replace_node);
    free(node);
}

