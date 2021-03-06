#include "basic_type.h"
#include "hashtable/simple.h"
#include "helper/helper.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h> 
#include<queue/min_queue.h>
#include<disjoint_set/basic_type.h>

#define SIMPLE_VERTEX(_v)  ((simple_vertex *)(_v->attr))
#define SIMPLE_VERTEX_DTIME(_v) (((simple_vertex *)(_v->attr))->discovered_time)
#define SIMPLE_VERTEX_FTIME(_v) (((simple_vertex *)(_v->attr))->finished_time)
#define SIMPLE_VERTEX_PI(_v) (((simple_vertex *)(_v->attr))->parent) 
#define SIMPLE_SHORTEST_PATH(_v) (((simple_vertex *)(_v->attr))->shortest_path) 
#define SIMPLE_TOPOLOGICAL_ORDER(_v) (((simple_vertex *)(_v->attr))->topological_order)
#define SIMPLE_STRONGLY_CONNECTED_COMPONENT(_v) (((simple_vertex *)(_v->attr))->strongly_connected_component)
#define SIMPLE_DISJOINT_SET(_v) (((simple_vertex *)(_v->attr))->disjoint_set_e)
#define SET_SIMPLE_VERTEX_PI(_v, _pi) ((simple_vertex *)(_v->attr))->parent = _pi
#define SET_SIMPLE_VERTEX_DTIME(_v, _t)  ((simple_vertex *)(_v->attr))->discovered_time = _t
#define SET_SIMPLE_VERTEX_FTIME(_v, _t)  ((simple_vertex *)(_v->attr))->finished_time = _t
#define SET_SIMPLE_SHORTEST_PATH(_v, _p)  ((simple_vertex *)(_v->attr))->shortest_path = _p
#define SET_SIMPLE_TOPOLOGICAL_ORDER(_v, _o)  ((simple_vertex *)(_v->attr))->topological_order = _o
#define SET_SIMPLE_STRONGLY_CONNECTED_COMPONENT(_v, _c)  ((simple_vertex *)(_v->attr))->strongly_connected_component = _c
#define SET_SIMPLE_DISJOINT_SET(_v,_s) ((simple_vertex *)(_v->attr))->disjoint_set_e = _s

typedef struct _simple_vertex{
    char *keyword;
    struct _simple_vertex *parent;
    int discovered_time;
    int finished_time;
    int shortest_path;
    int topological_order;
    int strongly_connected_component;
    disjoin_set_element *disjoint_set_e;
}simple_vertex;

simple_vertex *create_simple_vertex(char *keyword){
    simple_vertex *v;
    v = malloc(sizeof(simple_vertex));
    v->keyword = keyword;
    v->discovered_time = 0;
    v->finished_time = 0;
    v->shortest_path = 0;
    v->topological_order = 0;
    v->strongly_connected_component = 0;
    v->disjoint_set_e = NULL;
    return v;
}

graph_via_linklist *unserialize(char *vertexs, char *edges, hashtable *ht, int is_directed){
    graph_via_linklist *G;
    graph_vertex_based_linklist *src, *dst;
    simple_vertex *vertex;
    int weight;
    
    char *p,*q, *keyword;
    
    q = vertexs;
    p = q;
    G = create_graph_via_linklist();

    for( ; *p != '\0' ; p++){
        if(*p == ','){
            keyword = malloc(p-q+1);
            memcpy(keyword,q,p-q);
            *(keyword + (p-q)) = '\0';
            vertex = create_simple_vertex(keyword);
            hashtable_insert(ht,keyword, p-q+1, add_vertex_to_graph_via_linklist(G, vertex));
            q = p+1;
        }
    }

    //the last
    keyword = malloc(p-q+1);
    memcpy(keyword,q,p-q);
    *(keyword + (p-q)) = '\0';
    vertex = create_simple_vertex(keyword);
    hashtable_insert(ht,keyword, p-q+1, add_vertex_to_graph_via_linklist(G, vertex));

    q = edges;
    p = q;
    for( ; *p != '\0' ; p++){
        if(*p == '-'){
            keyword = malloc(p-q+1);
            memcpy(keyword,q,p-q);
            *(keyword + (p-q)) = '\0';
            src = hashtable_find(ht, keyword, p-q+1);
            free(keyword);
            q = p+1;
        }else if(*p == ':'){
            keyword = malloc(p-q+1);
            memcpy(keyword,q,p-q);
            *(keyword + (p-q)) = '\0';  
            dst = hashtable_find(ht, keyword, p-q+1);
            free(keyword);
            q = p+1;
        }else if(*p == ','){
            weight = str2int(q, p-q);
            add_edge_to_graph_via_linklist(src, dst, weight);
            if(!is_directed){
              add_edge_to_graph_via_linklist(dst, src, weight);  
            }
            q = p+1;
        }
    }

    weight = str2int(q, p-q);
    add_edge_to_graph_via_linklist(src, dst, weight);
    if(!is_directed){
        add_edge_to_graph_via_linklist(dst, src, weight);  
    }

    return G;
}

void node_printer(simple_vertex *v){
    printf("%s(%d/%d; parent = %s; shortest_path = %d; topological_order = %d; strongly_connected_component= %d)", v->keyword, v->discovered_time, v->finished_time, v->parent ? v->parent->keyword : "null", v->shortest_path, v->topological_order, v->strongly_connected_component);
}

void edge_printer(simple_vertex *src, simple_vertex *dst, int weight, int mark){
    printf("%s:%d; minimum_spanning_edge=%d", dst->keyword, weight, mark);
}

int comapre_edge_weight(adj_vertex_based_linklist *v, adj_vertex_based_linklist *u){
    return v->weight - u->weight;
}

void decrease_edge_weight(adj_vertex_based_linklist *v, int weight){
    v->weight = weight;
}

void kruskal_minimum_spanning_tree(graph_via_linklist *G){
    int i;
    min_queue *Q;
    slinklist_node *adj_vertex_linked_node;
    graph_vertex_based_linklist *adj_vertex;
    adj_vertex_based_linklist *edge;


    Q = create_min_queue(comapre_edge_weight, decrease_edge_weight);   
    for(i = 0; i < G->vertexs_num; i++){
        SET_SIMPLE_DISJOINT_SET(G->map[i], disjoint_set_make_set(G->map[i]));
        adj_vertex_linked_node = slinklist_first(G->map[i]->adj_vertex);
        while(adj_vertex_linked_node){
            min_queue_insert(Q, adj_vertex_linked_node->attr);
            adj_vertex_linked_node = adj_vertex_linked_node->next;
        }
    }

    while(min_queue_size(Q) != 0){
        edge = min_queue_extract_min(Q);
        if(disjoin_set_find(SIMPLE_DISJOINT_SET(edge->head)) != disjoin_set_find(SIMPLE_DISJOINT_SET(edge->adj_node))){
            edge->mark = 1;
            disjoint_set_union(SIMPLE_DISJOINT_SET(edge->head), SIMPLE_DISJOINT_SET(edge->adj_node));
        }
    }
}

void prim_minimum_spanning_tree(graph_via_linklist *G){

}

void bellman_ford_shortest_path(graph_via_linklist *G){

}

void dag_shortest_path(graph_via_linklist *G){

}

void dijkstra_shortest_path(graph_via_linklist *G){

}

int main(){
    setbuf(stdout,NULL);
    graph_via_linklist *G;
    graph_vertex_based_linklist *source;
    hashtable *ht;
    ht = create_hashtable();
    char *vertexs = "a,b,c,d,e,f,g,h,i";
    char *edges = "a-b:4,a-h:8,b-c:8,b-h:11,c-d:7,c-f:4,c-i:2,d-e:9,d-f:14,e-f:10,f-g:2,g-i:6,g-h:1,h-i:7";
    G = unserialize(vertexs, edges, ht, 0);
    source = hashtable_find(ht, "u", sizeof("u"));
    kruskal_minimum_spanning_tree(G);
    graph_via_linklist_printer(G,node_printer,edge_printer);
    return 0;
}