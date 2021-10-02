#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "basic_type.h"
#include "hashtable/simple.h"
#include "queue/basic_type.h"


int time = 0;
int order = 0;

#define SIMPLE_VERTEX(_v)  ((simple_vertex *)(_v->attr))
#define SIMPLE_VERTEX_DTIME(_v) (((simple_vertex *)(_v->attr))->discovered_time)
#define SIMPLE_VERTEX_FTIME(_v) (((simple_vertex *)(_v->attr))->finished_time)
#define SIMPLE_VERTEX_PI(_v) (((simple_vertex *)(_v->attr))->parent) 
#define SIMPLE_SHORTEST_PATH(_v) (((simple_vertex *)(_v->attr))->shortest_path) 
#define SIMPLE_TOPOLOGICAL_ORDER(_v) (((simple_vertex *)(_v->attr))->topological_order)
#define SIMPLE_STRONGLY_CONNECTED_COMPONENT(_v) (((simple_vertex *)(_v->attr))->strongly_connected_component)
#define SET_SIMPLE_VERTEX_PI(_v, _pi) ((simple_vertex *)(_v->attr))->parent = _pi
#define SET_SIMPLE_VERTEX_DTIME(_v, _t)  ((simple_vertex *)(_v->attr))->discovered_time = _t
#define SET_SIMPLE_VERTEX_FTIME(_v, _t)  ((simple_vertex *)(_v->attr))->finished_time = _t
#define SET_SIMPLE_SHORTEST_PATH(_v, _p)  ((simple_vertex *)(_v->attr))->shortest_path = _p
#define SET_SIMPLE_TOPOLOGICAL_ORDER(_v, _o)  ((simple_vertex *)(_v->attr))->topological_order = _o
#define SET_SIMPLE_STRONGLY_CONNECTED_COMPONENT(_v, _c)  ((simple_vertex *)(_v->attr))->strongly_connected_component = _c

typedef struct _simple_vertex{
    char *keyword;
    struct _simple_vertex *parent;
    int discovered_time;
    int finished_time;
    int shortest_path;
    int topological_order;
    int strongly_connected_component;
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
    return v;
}

graph_via_linklist *unserialize(char *vertexs, char *edges, hashtable *ht){
    graph_via_linklist *G;
    graph_vertex_based_linklist *src, *dst;
    simple_vertex *vertex;
    
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
        }else if(*p == ','){
            keyword = malloc(p-q+1);
            memcpy(keyword,q,p-q);
            *(keyword + (p-q)) = '\0';  
            dst = hashtable_find(ht, keyword, p-q+1);
            free(keyword);
            add_edge_to_graph_via_linklist(src, dst, 0);
            q = p+1;
        }
    }

    keyword = malloc(p-q+1);
    memcpy(keyword,q,p-q);
    *(keyword + (p-q)) = '\0';  
    dst = hashtable_find(ht, keyword, p-q+1);
    free(keyword);
    add_edge_to_graph_via_linklist(src, dst, 0);

    return G;
}   

void node_printer(simple_vertex *v){
    printf("%s(%d/%d; parent = %s; shortest_path = %d; topological_order = %d; strongly_connected_component= %d)", v->keyword, v->discovered_time, v->finished_time, v->parent ? v->parent->keyword : "null", v->shortest_path, v->topological_order, v->strongly_connected_component);
}

void edge_printer(simple_vertex *src, simple_vertex *dst, int weight){
    printf("%s", dst->keyword);
}


void dfs_visit(graph_via_linklist *G, graph_vertex_based_linklist *vertex, int is_topological_order, stack *decreasingly_finished_time_stack, int strongly_connected_component_order){
    slinklist_node *adj_vertex_linked_node;
    graph_vertex_based_linklist *adj_vertex;
    
    if(strongly_connected_component_order > 0){
        SET_SIMPLE_STRONGLY_CONNECTED_COMPONENT(vertex, strongly_connected_component_order);
    }

    adj_vertex_linked_node = slinklist_first(vertex->adj_vertex);
    while(adj_vertex_linked_node){
        adj_vertex = ((adj_vertex_based_linklist *)(adj_vertex_linked_node->attr))->adj_node;
        if(!SIMPLE_VERTEX_DTIME(adj_vertex)){
            SET_SIMPLE_VERTEX_PI(adj_vertex, SIMPLE_VERTEX(vertex));
            SET_SIMPLE_VERTEX_DTIME(adj_vertex, ++time);
            if(strongly_connected_component_order > 0){
                SET_SIMPLE_STRONGLY_CONNECTED_COMPONENT(adj_vertex, strongly_connected_component_order);
            }
            dfs_visit(G, adj_vertex, is_topological_order, decreasingly_finished_time_stack, strongly_connected_component_order);
        }
        adj_vertex_linked_node = adj_vertex_linked_node->next;
    }

    if(is_topological_order){
        SET_SIMPLE_TOPOLOGICAL_ORDER(vertex, order);
        order--;
    }

    if(decreasingly_finished_time_stack){
        stack_push(decreasingly_finished_time_stack, vertex->attr);
    }

    SET_SIMPLE_VERTEX_FTIME(vertex, ++time);
}

void dfs(graph_via_linklist *G, graph_vertex_based_linklist *source, int is_topological_order, stack *decreasingly_finished_time_stack){
    slinklist_node *vertex_linked_node;
    if(is_topological_order){
        order = G->vertexs_num;
    }

    SET_SIMPLE_VERTEX_DTIME(source, ++time);
    dfs_visit(G, source, is_topological_order, decreasingly_finished_time_stack, 0);
    for(int i = 0; i < G->vertexs_num; i++){
        if(!SIMPLE_VERTEX_DTIME(G->map[i])){
            SET_SIMPLE_VERTEX_DTIME(G->map[i], ++time);
            dfs_visit(G, G->map[i], is_topological_order, decreasingly_finished_time_stack, 0);
        }
    }
}

void bfs(graph_via_linklist *G, graph_vertex_based_linklist *source){
    queue *Q;
    graph_vertex_based_linklist *vertex;
    slinklist_node *adj_vertex_linked_node;
    graph_vertex_based_linklist *adj_vertex;

    for(int i=0; i < G->vertexs_num; i++){
        SET_SIMPLE_SHORTEST_PATH(G->map[i], -1);
    }
    
    SET_SIMPLE_SHORTEST_PATH(source, 0);
    Q = create_queue();
    queue_push(Q,source);
    while (!queue_is_empty(Q))
    {
        vertex = queue_pop(Q);
        adj_vertex_linked_node = slinklist_first(vertex->adj_vertex);
        while(adj_vertex_linked_node){
            adj_vertex = ((adj_vertex_based_linklist *)(adj_vertex_linked_node->attr))->adj_node;
            if(!SIMPLE_VERTEX_PI(adj_vertex)){
                SET_SIMPLE_VERTEX_PI(adj_vertex, SIMPLE_VERTEX(vertex));
                SET_SIMPLE_SHORTEST_PATH(adj_vertex, SIMPLE_SHORTEST_PATH(vertex)+1);
                queue_push(Q, adj_vertex);
            }
            adj_vertex_linked_node = adj_vertex_linked_node->next;
        }
    }
}

/*non-cyclic graph*/
void toplogical_order(graph_via_linklist *G){
    graph_vertex_based_linklist *source;
    if(G->vertexs_num  == 0){
        return;
    }

    source = G->map[0];
    dfs(G, source, 1, NULL);
}


void strongly_connected_component(graph_via_linklist *G){
    graph_via_linklist *transposed_G = transpose_graph_via_linklist(G);
    graph_vertex_based_linklist *source;
    stack *decreasingly_finished_time_stack;
    int strongly_connected_component_order;
    if(G->vertexs_num  == 0){
        return;
    }

    decreasingly_finished_time_stack = create_stack();
    source = G->map[0];
    dfs(G, source, 0, decreasingly_finished_time_stack);

    for(int i=0; i < transposed_G->vertexs_num; i++){
        SET_SIMPLE_VERTEX_DTIME(transposed_G->map[i], 0);
        SET_SIMPLE_VERTEX_FTIME(transposed_G->map[i], 0);
    }

    strongly_connected_component_order = 1;
    while (!stack_is_empty(decreasingly_finished_time_stack))
    {
        source = search_vertex_by_attr_of_graph_via_linklist(transposed_G, stack_pop(decreasingly_finished_time_stack));
        if(SIMPLE_STRONGLY_CONNECTED_COMPONENT(source) == 0){
            //printf("strongly_connected_component = %d\n", strongly_connected_component);
            time = 0;
            SET_SIMPLE_VERTEX_DTIME(source, ++time);
            dfs_visit(transposed_G, source, 0, NULL, strongly_connected_component_order);
            strongly_connected_component_order++;
        }
    }
}



int main(){
    setbuf(stdout,NULL);
    graph_via_linklist *G;
    graph_vertex_based_linklist *source;
    hashtable *ht;
    ht = create_hashtable();
    char *vertexs = "u,v,w,x,y,z";
    char *edges = "u-v,v-y,u-x,y-x,x-v,w-y,w-z,z-z";
    G = unserialize(vertexs, edges, ht);
    source = hashtable_find(ht, "u", sizeof("u"));
    //dfs(G, source, 0);
    //bfs(G, source);
    //toplogical_order(G);
    strongly_connected_component(G);
    graph_via_linklist_printer(G,node_printer,edge_printer);
    return 0;
}