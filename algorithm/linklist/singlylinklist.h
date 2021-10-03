/*singly link list*/

#ifndef singly_linked_list

#define singly_linked_list

typedef struct  _slinklist_node{
    struct _slinklist_node *next;
    void *attr;
}slinklist_node;

typedef struct _slinklist{
    slinklist_node *head;
}slinklist;

slinklist *create_slinklist();
slinklist_node *slinklist_first(slinklist *list);
slinklist_node *slinklist_next(slinklist_node *cur);
slinklist_node *slinklist_add(slinklist *list, void *attr);

#endif


