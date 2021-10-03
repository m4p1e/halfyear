/*doubly link list*/

typedef struct  _dlinklist_node{
    struct _slinklist_node *next;
    struct _slinklist_node *prev;
    void *attr;
}dlinklist_node;

typedef struct _slinklist{
    dlinklist_node *head;
}dlinklist;