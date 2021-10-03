/*implement of disjoint set*/
#include "basic_type.h"
#include<stdlib.h>

disjoin_set_element *disjoint_set_make_set(void *attr){
    disjoin_set_element *ele;
    ele = malloc(sizeof(disjoin_set_element));
    ele->parent = ele;
    ele->rank = 0;
    ele->attr = attr;
    return ele;
}

disjoin_set_element *disjoint_set_union(disjoin_set_element *ele1, disjoin_set_element *ele2){
    ele1 = disjoin_set_find(ele1);
    ele2 = disjoin_set_find(ele2);
    if(ele1->rank > ele2->rank){
        ele2->parent = ele1;
    }else{
        ele1->parent = ele2;
        if(ele1->rank = ele2->rank){
            ele2->rank++;
        }
    }   
}


disjoin_set_element *disjoin_set_find(disjoin_set_element *ele){

    if(ele->parent != ele){
        ele->parent = disjoin_set_find(ele->parent);
    }

    return ele->parent;
}   
