#include<stdio.h>
#include<stdlib.h>
#include "basic_type.h"

typedef struct _node_attr{
    int num;
    int balance_factor;
}node_attr;

#define BF(_N) (((node_attr *)(_N->attr))->balance_factor)
#define SET_BF(_N,_num) ((node_attr *)(_N->attr))->balance_factor = _num

int compare_two_integers(void* attr1, void* attr2){
    return ((node_attr *)(attr1))->num - ((node_attr *)(attr2))->num;
}


void avl_rotate_left(Btree *T, Bnode *parent, Bnode *right_child){
   
    binary_tree_replace_no_free(T,parent,right_child);
    parent->parent = right_child;
    parent->right = right_child->left;
    right_child->left = parent;
    
    if(BF(right_child) == 0){
        SET_BF(parent, -1);
        SET_BF(right_child, 1);
    }else{//right_child.bf = -1, parent.bf = 2
        SET_BF(parent, 0);
        SET_BF(right_child, 0);
    }

    //返回新的子树根结点
    return right_child;
}


void avl_rotate_right(Btree *T, Bnode *parent, Bnode *left_child){
    
    binary_tree_replace_no_free(T,parent,left_child);
    parent->parent = left_child;
    parent->left = left_child->right;
    left_child->right = parent;

    if(BF(left_child) == 0){ //仅发生在delete下
        SET_BF(parent, 1);
        SET_BF(left_child, -1);
    }else{ //left_child.bf = 1, parent.bf = 2
        SET_BF(parent, 0);
        SET_BF(left_child, 0);
    }

    return left_child;
}

void avl_rotate_left_right(Btree *T, Bnode *parent, Bnode *left_child){
    Bnode *node;
    
    node = left_child->right;
    
    //rotate_left
    binary_tree_replace_no_free(T, left_child, node);
    left_child->right = node->left;
    node->left = left_child;

    //rotate_right
    binary_tree_replace_no_free(T, parent, node);
    parent->left = node->right;
    node->right = parent;

    //调整平衡因子

    if(BF(node) == 1){
        SET_BF(parent, -1);
        SET_BF(left_child,0);
    }else if(BF(node) == 0){
        SET_BF(parent, 0);
        SET_BF(left_child, 0);
    }else{
        SET_BF(parent, 0);
        SET_BF(left_child, 1);
    }

    SET_BF(node,0);
    return node;
}

void avl_rotate_right_left(Btree *T, Bnode *parent, Bnode *right_child){
    Bnode *node;
    
    node = right_child->left;
    
    //rotate_right
    binary_tree_replace_no_free(T, right_child, node);
    right_child->left = node->right;
    node->right = right_child;

    //rotate_left
    binary_tree_replace_no_free(T, parent, node);
    parent->right = node->left;
    node->left = parent;

    //调整平衡因子
    if(BF(node) == 1){
        SET_BF(parent, 0);
        SET_BF(right_child,-1);
    }else if(BF(node) == 0){
        SET_BF(parent, 0);
        SET_BF(right_child, 0);
    }else{
        SET_BF(parent, 1);
        SET_BF(right_child, 0);
    }

    SET_BF(node,0);
    return node; 
}

void avl_tree_insert(Btree *T, int num){
    Bnode *parent,*node;
    node_attr *attr = malloc(sizeof(node_attr));
    attr->num = num;
    attr->balance_factor = 0;
    node = binary_tree_insert(T, attr);
    while (node->parent != NULL)
    {   
        parent = node->parent;
        if(parent->left == node){
            if(BF(parent) > 0){
                if(BF(node) == 1){
                    avl_rotate_right(T, parent, node);                         
                }else{
                    avl_rotate_left_right(T, parent, node);  
                }
                //rotate操作之后总能恢复原树高
                break;
            }else{
                if(BF(parent) == -1){
                    SET_BF(parent, 0);
                    break;
                }else{
                    SET_BF(parent, 1);
                }
            }
        }else{
            if(BF(parent) < 0){
                if(BF(node) == -1){
                    avl_rotate_left(T, parent, node);                         
                }else{
                    avl_rotate_right_left(T, parent, node);    
                }
                break;
            }else{
                if(BF(parent) == 1){
                    SET_BF(parent, 0);
                    break;
                }else{
                    SET_BF(parent, -1);
                }
            }
        }

        node = parent;
    }
}

void avl_tree_delete(Btree *T, Bnode* node){
    Bnode *replace_node = NULL;
    Bnode *parent, *child;
    Bnode *should_be_free;
    int record;
    
    should_be_free = NULL;
    child = NULL; //用于记录那颗子树高度减少了. 

    if(node->right != NULL ){
        replace_node = binary_tree_minimum(node->right);
        if(replace_node != node->right){
            /* 这里没有用简单的replace操作，是因为可以少一个判断*/
            if(replace_node->right != NULL){
                child = replace_node->right; 
                replace_node->parent->left = replace_node->right;
                replace_node->right->parent = replace_node->parent;
            }

            if(child == NULL){ 
                //相当于这里子树空了，我们需要构造一个结点来完成后续的fixup操作. 最后删除即可
                child = should_be_free = malloc(sizeof(node));
                child->parent = replace_node->parent;
                replace_node->parent->left = child;
            }

            replace_node->right = node->right;
            node->right->parent = replace_node;
        }


        if(child == NULL){
            if(replace_node->right != NULL){
                //如果replace_node后继存在，直接用这个后继结点当child即可.
                child = replace_node->right;
            }else{
                child = should_be_free = malloc(sizeof(node));
                child->parent = replace_node;
                replace_node->right = child;
            }
        }

        replace_node->left = node->left;
        if(node->left != NULL){
            node->left->parent = replace_node;
        }

    }else if(node->left != NULL){
        replace_node = node->left;
        child = replace_node;
    }else{
        replace_node = NULL;
        child = replace_node;
    }

    binary_tree_replace_no_free(T,node,replace_node);
    free(node);

    if(child == NULL){ //根结点被删除
        return;
    }

    while(child->parent != NULL){
        parent = child->parent;
        if(child == parent->left){
            if(BF(parent) < 0){
                if(BF(parent->right) > 0){
                    avl_rotate_right_left(T, parent, parent->right);
                }else{
                    record = BF(parent->right);
                    avl_rotate_left(T, parent, parent->right);
                    if(record == 0){
                        break;
                    }
                }    
            }else{
                if(BF(parent) == 0){
                    SET_BF(parent, -1);
                    break;
                }

                SET_BF(parent, 0);
            }
        }else{
            if(BF(parent) > 0){
                if(BF(parent->left) < 0){
                    avl_rotate_left_right(T, parent, parent->left); 
                }else{
                    record = BF(parent->left);
                    avl_rotate_right(T, parent, parent->left);
                    if(record == 0){ //选择之后会平衡
                        break;
                    }
                }    
            }else{
                if(BF(parent) == 0){
                    SET_BF(parent, 1);
                    break;
                }

                SET_BF(parent, 0);
            }
        }

        child = parent;
    }

    if(should_be_free != NULL){
        binary_tree_replace_no_free(T, should_be_free, NULL);
        free(should_be_free);
    }
}

int main(){
    Btree *tree = create_binary_tree(compare_two_integers);
    return 0;
}

