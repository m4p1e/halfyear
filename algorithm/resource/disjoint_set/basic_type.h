#include "linklist/singlylinklist.h"

typedef struct _disjoin_set_element{
    struct _disjoin_set_element *parent;
    void *attr;
    int rank;
}disjoin_set_element;

typedef struct _disjoint_set
{
    slinklist *disjoint_set_representatives;
}disjoint_set;

disjoin_set_element *disjoint_set_make_set(void *attr);
disjoin_set_element *disjoint_set_union(disjoin_set_element *ele1, disjoin_set_element *ele2);
disjoin_set_element *disjoin_set_find(disjoin_set_element *ele);
