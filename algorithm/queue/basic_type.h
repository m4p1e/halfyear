/* */

typedef struct __queue_element{
    struct __queue_element *next;
    void *attr;
}queue_element;

typedef struct __queue
{
    queue_element *head;
    queue_element *tail;
    int length;
}queue;

queue * create_queue();
void queue_push(queue *q, void *attr);
void* queue_pop(queue *q);
int queue_size(queue *q);
int queue_is_empty(queue *q);


typedef struct __stack_element{
    struct __stack_element *prev;
    void *attr;
}stack_element;

typedef struct __stack
{
    stack_element *top;
    int depth;
}stack;

stack *create_stack();
void stack_push(stack *s, void *attr);
void *stack_pop(stack *s);
int stack_depth(stack *s);
int stack_is_empty(stack *s);



