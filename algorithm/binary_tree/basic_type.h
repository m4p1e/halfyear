
typedef struct __tree_node
{
    struct __tree_node* left;
    struct __tree_node* right;
    struct __tree_node* parent;
    void* attr;
}Bnode;

typedef struct __binary_tree
{
   Bnode *root; 
   int (*compare)(void *, void *); 
}Btree;


Btree* create_binary_tree(int (*compare)(void*, void*));
Bnode* create_binary_tree_node();
Bnode* binary_tree_search(Btree* T, void* attr);
Bnode* binary_tree_insert(Btree* T, void* attr);
void   binary_tree_delete(Btree* T, Bnode *node);
Bnode* binary_tree_maximum(Bnode *node);
Bnode* binary_tree_minimum(Bnode *node);
Bnode* binary_tree_successor(Bnode *node);
Bnode* binary_tree_replace_no_free(Btree*, Bnode*, Bnode*);



