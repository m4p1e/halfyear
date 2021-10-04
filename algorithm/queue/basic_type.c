#include "basic_type.h"
#include "stdlib.h"

queue * create_queue(){
    queue *q;
    q = malloc(sizeof(queue));
    q->head = NULL;
    q->tail = NULL;
    q->length = 0;
    return q;
}

void queue_push(queue *q, void *attr){
    queue_element *ele;
    ele = malloc(sizeof(ele));
    ele->next = NULL;
    ele->attr = attr;
    if(q->tail == NULL){
        q->tail = ele;
        q->head = ele;
    }else{
        q->tail->next = ele;
    }

    q->tail = ele;
    q->length++;
}

void* queue_pop(queue *q){
    queue_element *ele;
    void *attr;

    if(q->head != NULL){
        ele = q->head;
        q->head = ele->next;    
    }else{
        return NULL;
    }

    if(q->head == NULL){
        q->tail = NULL;
    }

    q->length--;    
    attr = ele->attr;
    free(ele);
    return attr;
}

int queue_size(queue *q){
    return q->length;
}

int queue_is_empty(queue *q){
    return q->length == 0; 
}

stack *create_stack(){
    stack *s;
    s = malloc(sizeof(stack));
    s->top = NULL;
    s->depth = 0;
    return s;
}

void stack_push(stack *s, void *attr){
    stack_element *ele;
    ele = malloc(sizeof(stack_element));
    ele->prev = s->top;
    ele->attr = attr;
    s->top = ele;
    s->depth++;
}

void *stack_pop(stack *s){
    stack_element *ele;
    void *attr;
    ele = s->top;
    if(!ele){
        return NULL;
    }

    attr = ele->attr;
    s->top = ele->prev;
    free(ele);
    s->depth--;
    return attr;
}

int stack_depth(stack *s){
    return s->depth;
}

int stack_is_empty(stack *s){
    return s->depth == 0;
} 