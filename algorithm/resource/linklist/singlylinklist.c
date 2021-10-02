#include "singlylinklist.h"
#include<stdlib.h>


slinklist *create_slinklist(){
    slinklist* list = malloc(sizeof(slinklist));
    list->head = NULL;
    return list;
}

slinklist_node *slinklist_first(slinklist *list){
    return list->head;
}

slinklist_node *slinklist_next(slinklist_node *cur){
    return cur->next;
}

slinklist_node *slinklist_add(slinklist *list, void *attr){
    slinklist_node *node = malloc(sizeof(slinklist_node));
    slinklist_node *head = list->head;
    node->attr = attr;

    if(head == NULL){
        list->head = node;
        return node;
    }

    while(head->next != NULL){
        head = head->next;
    }

    head->next = node;
    return node;
}

slinklist_node *slinklist_insert_after(slinklist_node *cur, void *attr){

}

void *slinklist_delete_by_attr(slinklist *list, void *attr){

}

void *slinklist_delete_by_position(slinklist *list, int position){

}

void *slinklist_delete_by_node(slinklist *list, slinklist_node *node){

}

void *get_slinklist_node_attr(slinklist_node *cur){
    return cur->attr;
}


